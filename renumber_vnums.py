#!/usr/bin/env python3
"""
Renumber MUD area vnums to be sequential starting from 1, with no gaps.
ceiling.are is left unchanged (all its vnums are 65000+).

After renumbering, updates:
  - All area files in area.lst
  - src/ hardcoded vnum constants
  - docs/ documentation files
"""

import os
import re
import sys

AREA_DIR = '/home/user/acktng/area'
SRC_DIR = '/home/user/acktng/src'
DOCS_DIR = '/home/user/acktng/docs'
SKIP_AREAS = {'ceiling.are'}


# ---------------------------------------------------------------------------
# Step 1: Read area.lst
# ---------------------------------------------------------------------------

def read_area_lst():
    areas = []
    with open(os.path.join(AREA_DIR, 'area.lst')) as f:
        for line in f:
            s = line.strip()
            if s and not s.startswith('$'):
                areas.append(s)
    return areas


# ---------------------------------------------------------------------------
# Step 2: Collect all defined vnums from each area file
# ---------------------------------------------------------------------------

def collect_area_vnums(areas):
    """Returns {area_file: sorted_list_of_vnums}."""
    result = {}
    for area in areas:
        if area in SKIP_AREAS:
            continue
        path = os.path.join(AREA_DIR, area)
        if not os.path.exists(path):
            print(f'WARNING: {path} not found', file=sys.stderr)
            continue
        vnums = set()
        in_entity = False
        for line in open(path):
            s = line.strip()
            if not s.startswith('#'):
                continue
            token = s[1:]
            if token in ('ROOMS', 'MOBILES', 'OBJECTS'):
                in_entity = True
            elif token.isdigit():
                v = int(token)
                if v > 0 and in_entity:
                    vnums.add(v)
            elif token == '0':
                pass  # #0 terminator, stay in entity mode tracking
            else:
                in_entity = False
        result[area] = sorted(vnums)
    return result


# ---------------------------------------------------------------------------
# Step 3: Build old_vnum -> new_vnum mapping
# ---------------------------------------------------------------------------

def build_mapping(area_list, area_vnums):
    """Build {old_vnum: new_vnum} global mapping, processing areas in order.

    NOTE: Some vnums are legitimately shared across areas (e.g. same number used
    for a room in one area and an object in another - valid because rooms/mobs/objs
    have separate lookup tables in ACK MUD). When a vnum is shared, we assign it
    one new number (the first time it is encountered), so both entities map to the
    same new number - preserving the original relationship.
    """
    mapping = {}
    next_v = 1
    for area in area_list:
        if area in SKIP_AREAS:
            continue
        for old in area_vnums.get(area, []):
            if old not in mapping:          # Don't double-count shared vnums
                mapping[old] = next_v
                next_v += 1
    return mapping


def remap(v_str, mapping, allow_neg1=False):
    """Remap a vnum string. Returns new string or original if not in mapping."""
    if allow_neg1 and v_str == '-1':
        return '-1'
    try:
        vi = int(v_str)
        if vi <= 0:
            return v_str
        if vi in mapping:
            return str(mapping[vi])
        return v_str
    except ValueError:
        return v_str


# ---------------------------------------------------------------------------
# Step 4: Transform a single area file
# ---------------------------------------------------------------------------

def transform_area_file(path, mapping, area_vnums_for_this_area):
    """Transform area file in-place using the vnum mapping."""
    with open(path) as f:
        lines = f.readlines()

    result = []
    section = None

    # Exit parsing state (for ROOMS section)
    exit_phase = 0  # 0=normal, 1=after D<dir> (in desc), 2=in keyword str, 3=ready for numeric

    # Object parsing state (for OBJECTS section - to handle ITEM_PIECE values)
    obj_tilde_count = 0   # number of tilde-terminated strings seen for current object
    obj_item_type = None
    obj_past_type_line = False  # saw the item_type line
    obj_values_next = False    # next content line is the values line

    for raw_line in lines:
        line = raw_line
        s = raw_line.rstrip('\n\r')
        stripped = s.strip()

        # ------------------------------------------------------------------
        # Section header detection: #WORD (not #<digits>)
        # ------------------------------------------------------------------
        if stripped.startswith('#') and len(stripped) > 1 and not stripped[1].isdigit():
            token = stripped[1:].split()[0] if len(stripped) > 1 else ''
            if token in ('AREA', 'ROOMS', 'MOBILES', 'OBJECTS',
                         'RESETS', 'SHOPS', 'SPECIALS', 'OBJFUNS', '$'):
                section = token
                exit_phase = 0
                obj_tilde_count = 0
                obj_item_type = None
                obj_past_type_line = False
                obj_values_next = False
            result.append(line)
            continue

        # ------------------------------------------------------------------
        # AREA section: update V directive
        # ------------------------------------------------------------------
        if section == 'AREA':
            if stripped.startswith('V '):
                parts = stripped.split()
                if len(parts) == 3:
                    # Compute new min/max from mapped vnums for this area
                    if area_vnums_for_this_area:
                        new_min = mapping.get(area_vnums_for_this_area[0],
                                              area_vnums_for_this_area[0])
                        new_max = mapping.get(area_vnums_for_this_area[-1],
                                              area_vnums_for_this_area[-1])
                    else:
                        new_min = int(parts[1])
                        new_max = int(parts[2])
                    result.append(f'V {new_min} {new_max}\n')
                    continue
            result.append(line)
            continue

        # ------------------------------------------------------------------
        # ROOMS section
        # ------------------------------------------------------------------
        if section == 'ROOMS':
            # Entity vnum definition
            if stripped.startswith('#') and stripped[1:].isdigit():
                old_v = int(stripped[1:])
                if old_v == 0:
                    result.append(line)
                    continue
                new_v = mapping.get(old_v, old_v)
                result.append(f'#{new_v}\n')
                exit_phase = 0
                continue

            # Exit directive (D<digit>)
            if len(stripped) == 2 and stripped[0] == 'D' and stripped[1].isdigit():
                exit_phase = 1
                result.append(line)
                continue

            # Track exit tilde strings
            if exit_phase > 0:
                if exit_phase < 3:
                    # We're in desc (phase 1) or keyword string (phase 2)
                    if '~' in stripped:
                        exit_phase += 1
                    result.append(line)
                    continue
                elif exit_phase == 3:
                    # Numeric exit line: <flags> <key_vnum> <dest_vnum>
                    parts = stripped.split()
                    if len(parts) >= 3:
                        flags = parts[0]
                        key = remap(parts[1], mapping, allow_neg1=True)
                        dest = remap(parts[2], mapping, allow_neg1=True)
                        # Preserve any trailing content (shouldn't be any, but just in case)
                        trailing = s[len(s.rstrip())]  if s != s.rstrip() else ''
                        # Reconstruct with original indentation
                        indent = s[:len(s) - len(s.lstrip())]
                        result.append(f'{indent}{flags} {key} {dest}\n')
                    else:
                        result.append(line)
                    exit_phase = 0
                    continue

            # 'S' ends a room record
            if stripped == 'S':
                exit_phase = 0

            result.append(line)
            continue

        # ------------------------------------------------------------------
        # MOBILES section
        # ------------------------------------------------------------------
        if section == 'MOBILES':
            if stripped.startswith('#') and stripped[1:].isdigit():
                old_v = int(stripped[1:])
                if old_v == 0:
                    result.append(line)
                    continue
                new_v = mapping.get(old_v, old_v)
                result.append(f'#{new_v}\n')
                continue
            result.append(line)
            continue

        # ------------------------------------------------------------------
        # OBJECTS section
        # ------------------------------------------------------------------
        if section == 'OBJECTS':
            # Entity vnum definition
            if stripped.startswith('#') and stripped[1:].isdigit():
                old_v = int(stripped[1:])
                if old_v == 0:
                    result.append(line)
                    # Reset object tracking
                    obj_tilde_count = 0
                    obj_item_type = None
                    obj_past_type_line = False
                    obj_values_next = False
                    continue
                new_v = mapping.get(old_v, old_v)
                result.append(f'#{new_v}\n')
                obj_tilde_count = 0
                obj_item_type = None
                obj_past_type_line = False
                obj_values_next = False
                continue

            # Track through object fields
            if obj_tilde_count < 3:
                # Still reading name/short_descr/description tilde-terminated strings
                if '~' in stripped:
                    obj_tilde_count += 1
                result.append(line)
                continue

            if not obj_past_type_line:
                # This is <item_type> <extra_flags> <wear_flags> <item_apply>
                parts = stripped.split()
                if parts:
                    try:
                        obj_item_type = int(parts[0])
                    except ValueError:
                        obj_item_type = None
                obj_past_type_line = True
                obj_values_next = True
                result.append(line)
                continue

            if obj_values_next:
                # Values line: <v0> <v1> <v2> <v3> ... <v9>
                obj_values_next = False
                if obj_item_type == 29:  # ITEM_PIECE: v0=prev, v1=next, v2=result
                    parts = stripped.split()
                    if len(parts) >= 3:
                        v0 = remap(parts[0], mapping)
                        v1 = remap(parts[1], mapping)
                        v2 = remap(parts[2], mapping)
                        rest = parts[3:]
                        new_vals = ' '.join([v0, v1, v2] + rest)
                        indent = s[:len(s) - len(s.lstrip())]
                        result.append(f'{indent}{new_vals}\n')
                        continue
                result.append(line)
                continue

            # After values line: A/E/L trailing entries or next object
            # These don't contain vnums (A=apply, E=extra-desc, L=level)
            result.append(line)
            continue

        # ------------------------------------------------------------------
        # RESETS section
        # ------------------------------------------------------------------
        if section == 'RESETS':
            if not stripped or stripped == 'S':
                result.append(line)
                continue

            cmd = stripped[0] if stripped else ''
            parts = stripped.split()
            indent = s[:len(s) - len(s.lstrip())]

            if cmd == 'M' and len(parts) >= 5:
                # M ifflag mob_vnum limit room_vnum [notes...]
                mob_v = remap(parts[2], mapping)
                room_v = remap(parts[4], mapping)
                notes = '   ' + ' '.join(parts[5:]) if len(parts) > 5 else ''
                result.append(f'{indent}{parts[0]} {parts[1]} {mob_v} {parts[3]} {room_v}{notes}\n')
                continue

            elif cmd == 'O' and len(parts) >= 5:
                # O ifflag obj_vnum limit room_vnum [notes...]
                obj_v = remap(parts[2], mapping)
                room_v = remap(parts[4], mapping)
                notes = '   ' + ' '.join(parts[5:]) if len(parts) > 5 else ''
                result.append(f'{indent}{parts[0]} {parts[1]} {obj_v} {parts[3]} {room_v}{notes}\n')
                continue

            elif cmd == 'G' and len(parts) >= 3:
                # G ifflag obj_vnum limit [notes...]
                obj_v = remap(parts[2], mapping)
                notes = '   ' + ' '.join(parts[4:]) if len(parts) > 4 else ''
                result.append(f'{indent}{parts[0]} {parts[1]} {obj_v} {parts[3] if len(parts) > 3 else ""}{notes}\n')
                continue

            elif cmd == 'E' and len(parts) >= 5:
                # E ifflag obj_vnum limit wear_loc [notes...]
                obj_v = remap(parts[2], mapping)
                # parts[4] is wear_loc, NOT a vnum - leave unchanged
                notes = '   ' + ' '.join(parts[5:]) if len(parts) > 5 else ''
                result.append(f'{indent}{parts[0]} {parts[1]} {obj_v} {parts[3]} {parts[4]}{notes}\n')
                continue

            elif cmd == 'D' and len(parts) >= 4:
                # D ifflag room_vnum door state [notes...]
                room_v = remap(parts[2], mapping)
                notes = '   ' + ' '.join(parts[5:]) if len(parts) > 5 else ''
                result.append(f'{indent}{parts[0]} {parts[1]} {room_v} {parts[3]} {parts[4] if len(parts) > 4 else ""}{notes}\n')
                continue

            elif cmd == 'R' and len(parts) >= 3:
                # R ifflag room_vnum max_door [notes...]
                room_v = remap(parts[2], mapping)
                rest = ' '.join(parts[3:])
                result.append(f'{indent}{parts[0]} {parts[1]} {room_v} {rest}\n')
                continue

            elif cmd == 'P' and len(parts) >= 5:
                # P ifflag obj_vnum limit cont_vnum [notes...]
                obj_v = remap(parts[2], mapping)
                cont_v = remap(parts[4], mapping)
                notes = '   ' + ' '.join(parts[5:]) if len(parts) > 5 else ''
                result.append(f'{indent}{parts[0]} {parts[1]} {obj_v} {parts[3]} {cont_v}{notes}\n')
                continue

            result.append(line)
            continue

        # ------------------------------------------------------------------
        # SHOPS section: <keeper_mob_vnum> <buy_types...> <profit_buy> <profit_sell> <open> <close>
        # ------------------------------------------------------------------
        if section == 'SHOPS':
            if stripped == '0':
                result.append(line)
                continue
            parts = stripped.split()
            if parts and parts[0].isdigit():
                keeper_v = remap(parts[0], mapping)
                rest = ' '.join(parts[1:])
                indent = s[:len(s) - len(s.lstrip())]
                result.append(f'{indent}{keeper_v} {rest}\n')
                continue
            result.append(line)
            continue

        # ------------------------------------------------------------------
        # SPECIALS section: M <mob_vnum> <spec_fn>
        # ------------------------------------------------------------------
        if section == 'SPECIALS':
            if stripped.startswith('M '):
                parts = stripped.split(None, 2)
                if len(parts) >= 2:
                    mob_v = remap(parts[1], mapping)
                    rest = ' ' + parts[2] if len(parts) > 2 else ''
                    indent = s[:len(s) - len(s.lstrip())]
                    result.append(f'{indent}M {mob_v}{rest}\n')
                    continue
            result.append(line)
            continue

        # ------------------------------------------------------------------
        # OBJFUNS section: O <obj_vnum> <fn_name>
        # ------------------------------------------------------------------
        if section == 'OBJFUNS':
            if stripped.startswith('O '):
                parts = stripped.split(None, 2)
                if len(parts) >= 2:
                    obj_v = remap(parts[1], mapping)
                    rest = ' ' + parts[2] if len(parts) > 2 else ''
                    indent = s[:len(s) - len(s.lstrip())]
                    result.append(f'{indent}O {obj_v}{rest}\n')
                    continue
            result.append(line)
            continue

        # Default: pass through
        result.append(line)

    with open(path, 'w') as f:
        f.writelines(result)


# ---------------------------------------------------------------------------
# Step 5: Update src/ files with hardcoded vnums
# ---------------------------------------------------------------------------

def update_src_files(mapping):
    """Update hardcoded vnum constants in source files.

    Strategy:
    - config.h / invasion.h: use _replace_define_vnum (safe: only matches #define VNUM lines)
    - C source files: use _replace_literal_vnums_in_file ONLY (single-pass, avoids cascading)
      Do NOT apply both to the same file.
    """

    # --- config.h: ONLY replace #define *VNUM* lines ---
    config_path = os.path.join(SRC_DIR, 'headers', 'config.h')
    _replace_define_vnum(config_path, mapping)

    # --- invasion.h: ONLY replace #define *VNUM* lines ---
    inv_path = os.path.join(SRC_DIR, 'headers', 'invasion.h')
    _replace_define_vnum(inv_path, mapping)

    # Touch ack.h so make knows to rebuild all .o files that depend on it.
    # The Makefile dependency is: $(OBJDIR)/%.o: %.c headers/ack.h
    # config.h and invasion.h are included by ack.h, but make doesn't track
    # transitive includes, so we must touch ack.h explicitly.
    ack_h = os.path.join(SRC_DIR, 'headers', 'ack.h')
    if os.path.exists(ack_h):
        import time
        t = time.time()
        os.utime(ack_h, (t, t))

    # --- C source files: single-pass literal replacement ---
    # Only caravan.c is safe for blanket replacement (all numbers are route vnums).
    # save.c, act_mob.c, act_move.c have non-vnum numbers that would be wrongly changed.
    for src_file in ['caravan.c']:
        path = os.path.join(SRC_DIR, src_file)
        _replace_literal_vnums_in_file(path, mapping)

    # --- save.c: has a local #define TEMP_VNUM that needs updating ---
    _replace_define_vnum(os.path.join(SRC_DIR, 'save.c'), mapping)

    # --- act_mob.c: only the fairy godmother mob vnum check needs updating ---
    _replace_specific_vnums(os.path.join(SRC_DIR, 'act_mob.c'), mapping, [
        1026,   # fairy godmother mob vnum (withered_depths)
    ])

    # --- act_move.c: only the recall_vnum comparisons need updating ---
    _replace_specific_vnums(os.path.join(SRC_DIR, 'act_move.c'), mapping, [
        3001,   # ROOM_VNUM_TEMPLE (recall default)
        3110,   # INVASION_SPAWN_VNUM (recall override)
    ])

    # --- quest.c: postmaster mob vnums in switch statement ---
    _replace_specific_vnums(os.path.join(SRC_DIR, 'quest.c'), mapping, [
        13001,  # Kiess canonical postmaster
        13021,  # Kiess alias postmaster
        14001,  # Kowloon canonical postmaster
        14021,  # Kowloon alias postmaster
    ])

    # --- test files: only update tests with known vnum literals ---
    # test_invasion.c: INVASION_SPAWN_VNUM assertion
    _replace_specific_vnums(os.path.join(SRC_DIR, 'tests', 'test_invasion.c'), mapping, [3110])
    # test_quest.c: postmaster vnum assertions
    _replace_specific_vnums(os.path.join(SRC_DIR, 'tests', 'test_quest.c'), mapping, [
        13001, 13021, 14001, 14021, 3015,
    ])


def _replace_define_vnum(path, mapping):
    """Replace numeric values in #define VNUM lines that match known vnums."""
    if not os.path.exists(path):
        return
    with open(path) as f:
        content = f.read()

    # Match: #define SOMETHING_VNUM <number> — single-pass replacement via regex
    new_content = re.sub(
        r'(#define\s+\w*VNUM\w*\s+)(\d+)',
        lambda m: m.group(1) + str(mapping.get(int(m.group(2)), int(m.group(2)))),
        content
    )
    if new_content != content:
        with open(path, 'w') as f:
            f.write(new_content)


def _replace_literal_vnums_in_file(path, mapping):
    """Replace literal vnum values in files using a single-pass regex.

    IMPORTANT: Must be single-pass to avoid cascading replacements
    (e.g. replacing 14083→3522, then accidentally replacing 3522 again).
    """
    if not os.path.exists(path):
        return
    with open(path) as f:
        content = f.read()

    # Build pattern that matches any old vnum as a whole number
    # Sort by descending length to ensure longer numbers match first
    old_vnums = sorted(
        [v for v in mapping if mapping[v] != v],
        key=lambda v: -len(str(v))
    )
    if not old_vnums:
        return

    # Single-pass replacement using \b word boundaries.
    # \b ensures we only match standalone numbers, not numbers embedded inside
    # identifiers like sha256 or ITEM_256 etc.
    pattern = r'\b(' + '|'.join(re.escape(str(v)) for v in old_vnums) + r')\b'

    def replacer(m):
        old_v = int(m.group(1))
        return str(mapping.get(old_v, old_v))

    new_content = re.sub(pattern, replacer, content)

    if new_content != content:
        with open(path, 'w') as f:
            f.write(new_content)


def _replace_specific_vnums(path, mapping, vnum_list):
    """Replace only specific known vnums in a file (single-pass).

    Safer than _replace_literal_vnums_in_file for files that mix vnum
    literals with non-vnum numbers (e.g., HP values, buffer sizes).
    Only the explicitly listed vnums are replaced.
    """
    if not os.path.exists(path):
        return
    with open(path) as f:
        content = f.read()

    # Only process vnums that are in the mapping AND in our list
    targets = sorted(
        [v for v in vnum_list if v in mapping and mapping[v] != v],
        key=lambda v: -len(str(v))
    )
    if not targets:
        return

    pattern = r'\b(' + '|'.join(re.escape(str(v)) for v in targets) + r')\b'

    def replacer(m):
        old_v = int(m.group(1))
        return str(mapping.get(old_v, old_v))

    new_content = re.sub(pattern, replacer, content)

    if new_content != content:
        with open(path, 'w') as f:
            f.write(new_content)


# ---------------------------------------------------------------------------
# Step 6: Update docs/ files
# ---------------------------------------------------------------------------

def update_docs(mapping, area_list, area_vnums):
    """Update all documentation files with new vnum references."""

    # Compute area new ranges for area_index.md update
    area_new_ranges = {}
    for area in area_list:
        if area in SKIP_AREAS:
            continue
        vnums = area_vnums.get(area, [])
        if vnums:
            area_new_ranges[area] = (
                mapping.get(vnums[0], vnums[0]),
                mapping.get(vnums[-1], vnums[-1])
            )

    # Update area_index.md
    _update_area_index(os.path.join(DOCS_DIR, 'area_index.md'),
                       area_new_ranges, mapping)

    # Update world_links.md and all other .md files with vnum replacement
    docs_to_update = []
    for root, dirs, files in os.walk(DOCS_DIR):
        for fname in files:
            if fname.endswith('.md') or fname.endswith('.txt'):
                docs_to_update.append(os.path.join(root, fname))

    for doc_path in docs_to_update:
        _replace_vnums_in_doc(doc_path, mapping)


def _update_area_index(path, area_new_ranges, mapping):
    """Rebuild the area_index.md table with new vnum ranges."""
    if not os.path.exists(path):
        return
    with open(path) as f:
        content = f.read()

    # Replace vnum ranges in table rows like "166–265" with new ranges
    # Pattern: | `filename.are` | 166–265 | ...
    def replace_range(m):
        fname = m.group(1)
        if fname in area_new_ranges:
            new_min, new_max = area_new_ranges[fname]
            return f'`{fname}` | {new_min}–{new_max} |'
        return m.group(0)

    new_content = re.sub(
        r'`([^`]+\.are)`\s*\|\s*(\d+)–(\d+)\s*\|',
        replace_range,
        content
    )

    # Also replace any standalone vnum numbers in the document
    new_content = _replace_vnums_in_text(new_content, mapping)

    if new_content != content:
        with open(path, 'w') as f:
            f.write(new_content)


def _replace_vnums_in_doc(path, mapping):
    """Replace vnum numbers in a documentation file."""
    if not os.path.exists(path):
        return
    with open(path) as f:
        content = f.read()

    new_content = _replace_vnums_in_text(content, mapping)

    if new_content != content:
        with open(path, 'w') as f:
            f.write(new_content)


def _replace_vnums_in_text(content, mapping):
    """Replace old vnum numbers with new ones in text, single-pass to avoid cascading."""
    old_vnums = sorted(
        [v for v in mapping if mapping[v] != v],
        key=lambda v: -len(str(v))
    )
    if not old_vnums:
        return content

    pattern = r'\b(' + '|'.join(re.escape(str(v)) for v in old_vnums) + r')\b'

    def replacer(m):
        old_v = int(m.group(1))
        return str(mapping.get(old_v, old_v))

    return re.sub(pattern, replacer, content)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    print("Reading area.lst...")
    area_list = read_area_lst()
    print(f"  Found {len(area_list)} areas ({len(area_list) - len(SKIP_AREAS)} to renumber)")

    print("Collecting vnums from area files...")
    area_vnums = collect_area_vnums(area_list)
    total_vnums = sum(len(v) for v in area_vnums.values())
    print(f"  Total vnums to renumber: {total_vnums}")

    print("Building vnum mapping...")
    mapping = build_mapping(area_list, area_vnums)
    print(f"  Mapping created: {len(mapping)} vnums (1 to {max(mapping.values()) if mapping else 0})")

    # Show summary of old vs new ranges per area
    print("\nArea vnum range summary:")
    for area in area_list:
        if area in SKIP_AREAS:
            print(f"  {area}: SKIPPED (ceiling.are)")
            continue
        vnums = area_vnums.get(area, [])
        if vnums:
            old_min, old_max = vnums[0], vnums[-1]
            new_min = mapping.get(old_min, old_min)
            new_max = mapping.get(old_max, old_max)
            print(f"  {area}: {old_min}-{old_max} -> {new_min}-{new_max} ({len(vnums)} vnums)")
        else:
            print(f"  {area}: (no vnums found)")

    print("\nTransforming area files...")
    for area in area_list:
        if area in SKIP_AREAS:
            continue
        path = os.path.join(AREA_DIR, area)
        if not os.path.exists(path):
            continue
        print(f"  {area}...")
        transform_area_file(path, mapping, area_vnums.get(area, []))

    print("\nUpdating src/ hardcoded vnum constants...")
    update_src_files(mapping)

    print("\nUpdating docs/ files...")
    update_docs(mapping, area_list, area_vnums)

    print("\nDone! Summary:")
    print(f"  Remapped {len(mapping)} vnums")
    print(f"  New range: 1 to {max(mapping.values()) if mapping else 0}")
    print(f"  ceiling.are unchanged (32501-32766)")


if __name__ == '__main__':
    main()
