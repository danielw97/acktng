#!/usr/bin/env python3
"""
Fix script for /home/user/acktng/area/the_northern_oasis.are

Applies all spec violations fixes:
1. Remove blank lines in tilde-terminated strings
2. Fix mobile act flags (add stay_area=64, fix boss flags)
3. Add ITEM_BOSS to boss items 8890-8894
4. Remove ITEM_BUCKLER from items whose wear_flags don't include hold (32768)
5. Fix key_vnum=0 to -1 for exits going to rooms 8600-8609
6. Remove blank lines in #RESETS section
7. Reorder sections: #AREA, #MOBILES, #OBJECTS, #ROOMS, #RESETS, #$
8. Generate unique room descriptions for duplicate ones
9. Fix extra desc keyword from "route line~" to "banners~"
"""

import re
import sys

INPUT_FILE = '/home/user/acktng/area/the_northern_oasis.are'
OUTPUT_FILE = '/home/user/acktng/area/the_northern_oasis.are'

# ============================================================
# Room description generation data
# ============================================================

ROW_ADJECTIVES = [
    'northern', 'sun-bleached', 'eastern', 'western', 'central',
    'inner', 'outer', 'upper', 'lower', 'far'
]

COL_FEATURES = [
    'cistern ribs', 'reed windbreaks', 'sunken spillways', 'carved pillars',
    'ochre stonework', 'water troughs', 'clay-packed walls', 'carved cisterns',
    'stone benches', 'iron anchors', 'rope cordons', 'dye-stained basins',
    'weathered posts', 'grain sacks', 'salt-crusted channels',
    'cracked paving', 'wind-scored ledges', 'painted waymarks',
    'limestone blocks', 'reed-lashed posts'
]

COL_ADJECTIVES = [
    'first', 'second', 'third', 'middle', 'exposed',
    'sheltered', 'open', 'winding', 'narrow', 'wide',
    'sun-drenched', 'dust-hazed', 'wind-scored', 'grit-swept', 'heat-shimmered',
    'reed-bordered', 'stone-edged', 'sand-soft', 'hard-packed', 'salt-rimmed'
]

SKY_OBSERVATIONS = [
    'the northern sky holds a fierce @@lbright blue@@N vault,',
    'gulls track the hidden waterline somewhere ahead,',
    '@@bbrown@@N dust streams low across your boots,',
    'you can see the oasis well from this position,',
    'the skyline shows pale rock formations,',
    'a shimmer of heat distorts the far horizon,',
    'the trade road stretches visibly to the north,',
    'sand devils spiral lazily in the open air,',
    'the sun sits fierce and unblinking overhead,',
    'wind-driven haze softens distant outlines,',
]

UNIQUE_DETAILS = [
    'Tracks lead east where the next marker waits.',
    'A shadow marker stands to the east, its base worn smooth by passing feet.',
    'You can hear water somewhere to the north, hidden beneath stone.',
    'Iron stakes mark the perimeter at measured intervals.',
    'Grit-filled ruts trace the old wagon path across the square.',
    'Faded trade symbols mark the threshold between two older lanes.',
    'Stacked stones outline an old corral against the retaining wall.',
    'Bleached tent pegs cluster near the wall where merchants once camped.',
    'A carved notch in the pavement indicates a former well-head.',
    'Pale lime mortar shows where a partition wall once stood.',
    'Deep grooves in the stone mark the passage of heavy cart wheels.',
    'A litter of date pits and dried fruit rind suggests recent habitation.',
    'The wind carries the scent of mineral springs beneath the sand.',
    'Cracked plaster still clings to one face of the retaining wall.',
    'Charcoal marks on the pavement hint at old fire pits.',
    'A scattering of ceramic shards glints between the paving stones.',
    'Rope-worn channels at waist height indicate a former tether line.',
    'The eastern wall bears hand-height scratches left by livestock.',
    'A depression in the centre of the lane shows where a cistern drain once opened.',
    'Several large flat stones have been laid side by side to form a rough seat.',
]

def make_room_description(vnum):
    """Generate a unique description for the given room vnum."""
    idx = vnum - 8700
    row = idx // 20
    col = idx % 20

    row_adj = ROW_ADJECTIVES[row % len(ROW_ADJECTIVES)]
    col_adj = COL_ADJECTIVES[col % len(COL_ADJECTIVES)]
    feature = COL_FEATURES[col % len(COL_FEATURES)]
    sky_obs = SKY_OBSERVATIONS[(row * 3 + col) % len(SKY_OBSERVATIONS)]
    detail = UNIQUE_DETAILS[(row * 7 + col * 3) % len(UNIQUE_DETAILS)]

    desc = (
        f"The {row_adj} {col_adj} quarter of the northern oasis opens around {feature} stitched with @@lbright blue@@N banners.@@N\n"
        f"Wind drives @@yyellow@@N grit in low ribbons while @@bbrown@@N retaining walls keep the lane coherent despite branching footpaths.@@N\n"
        f"From this square, {sky_obs} every marker suggests traders once moved in disciplined columns.@@N\n"
        f"{detail}@@N\n"
    )
    return desc


# ============================================================
# String (tilde-terminated) blank line removal
# ============================================================

def strip_blank_lines_in_string(s):
    """
    Given the content of a tilde-terminated string (without the leading/trailing ~),
    remove back-to-back newlines and ensure exactly one trailing newline.
    """
    # Collapse multiple consecutive newlines into one
    s = re.sub(r'\n{2,}', '\n', s)
    # Ensure exactly one trailing newline
    s = s.rstrip('\n') + '\n'
    return s


# ============================================================
# Section parser helpers
# ============================================================

def split_into_sections(text):
    """
    Split the file into named sections. Returns list of (name, content) tuples
    where content includes the section header line.
    """
    # Find all section start positions
    section_pattern = re.compile(r'^(#AREA|#ROOMS|#MOBILES|#OBJECTS|#RESETS|#\$)\s*$', re.MULTILINE)
    matches = list(section_pattern.finditer(text))

    sections = []
    for i, m in enumerate(matches):
        name = m.group(1)
        start = m.start()
        end = matches[i+1].start() if i+1 < len(matches) else len(text)
        content = text[start:end]
        sections.append((name, content))

    return sections


# ============================================================
# Fix 1: Remove blank lines from tilde-terminated strings
# ============================================================

def fix_blank_lines_in_strings(section_text):
    """
    Remove blank lines (consecutive newlines) from tilde-terminated multi-line strings.
    Also ensures each multi-line string ends with exactly one newline before ~.

    Uses a line-by-line state machine to track multi-line string context.

    A multi-line string is identified by a ~ that appears alone on a line (closing tilde).
    The content preceding it (from the previous tilde on its own line or the previous
    structural line) is the string content.

    Key insight: we scan lines. When we see a line that ends with ~ and the ~ is at
    the END of the line (i.e., NOT the only character - that would be a solo ~),
    it's a single-line string terminator. When ~ is alone on a line, it terminates
    a multi-line string.

    We track: "are we inside a multi-line string?" State machine:
    - NOT_IN_STRING: looking for start of multi-line string content
    - IN_MULTI_STRING: accumulating lines of string content

    A multi-line string STARTS when we've seen a line that does NOT end with ~
    (i.e., the line is content that will be terminated by a later ~ on its own line).

    Actually, the simplest approach: use the tilde-split but carefully distinguish
    between structural segments and string segments.

    A segment is PURE STRING CONTENT if and only if:
    - It does NOT contain structural indicators (D0-D9, 'S', digit-only lines, etc.)
    - AND it contains newline characters

    But this is complex. Better: use regex to find multi-line strings directly.

    REGEX APPROACH:
    A multi-line string is: content spanning multiple lines, terminated by a ~ on its own line.
    Pattern: after a newline following either a structural line or another string terminator,
    content lines not containing ~, followed by \n~

    The simplest reliable regex: find all occurrences of
    (non-~ content with \n\n) followed by \n~
    and collapse the \n\n.
    """
    # Use a targeted regex to find multi-line string content with blank lines.
    # Multi-line string content ends with \n~ (tilde on its own line).
    # We want to collapse \n\n within such content.

    # Strategy: find all tilde-terminated blocks that contain \n\n
    # and remove the blank lines within them.

    # A tilde-terminated block is: any text followed by \n~ (tilde on its own line)
    # We match these greedily from the LAST \n\n before \n~

    # Actually, let's use a proper approach:
    # Split the text into segments where each segment ends with either:
    # (a) content~ (same-line tilde) - single-line string
    # (b) content\n~ (tilde on own line) - multi-line string
    # For (b), collapse \n\n within content.

    # Regex: match content between start-of-field and closing ~ (on its own line)
    # The closing ~ is either: alone on a line (\n~\n or \n~$)
    # The content between opening and closing ~ contains no ~ characters

    count = [0]

    def collapse_blank_lines(m):
        full_match = m.group(0)
        content = m.group(1)
        # Count double newlines in the full match (before \n~ suffix)
        # The full match is content + \n~, where content = group(1)
        # Any extra \n before the final \n~ is a blank line
        # group(1) is greedy, so it ends with \n (one newline), then \n~ follows
        # But the full match could be "text\n\n~" where group(1)="text\n"
        # In that case the full match has 2 \n before ~
        trailing_newlines = len(full_match) - len(full_match.rstrip('\n~'))
        if trailing_newlines > 1:
            count[0] += trailing_newlines - 1

        # Also handle \n\n inside content (not just trailing)
        if '\n\n' in content:
            count[0] += content.count('\n\n')
            content = re.sub(r'\n{2,}', '\n', content)

        # Ensure exactly one trailing newline before ~
        content = content.rstrip('\n') + '\n'
        return content + '~'

    # Match: content (no ~) followed by \n~
    # The regex ([^~]+)\n~ matches multi-line string content ending with \n~
    # Single-line strings like "banners~" have no \n before ~ so won't match.
    # This handles:
    # - Room descriptions: multi-line text\n\n~
    # - Exit descriptions: single-line text\n\n~
    # - Mobile long_descr/desc: text\n\n~
    result = re.sub(r'([^~]+)\n~', collapse_blank_lines, section_text)

    return result, count[0]


# ============================================================
# Fix 2: Mobile act flags
# ============================================================

def fix_mobile_act_flags(mobiles_section):
    """
    Regular mobs: act=1 -> 65 (add stay_area=64)
    Boss mobs: act=33554435 -> 67108931 (add stay_area=64, add boss=67108864, remove solo=33554432)

    The act flag line looks like: "1 0 0 S" or "33554435 0 0 S"
    """
    regular_fixed = [0]
    boss_fixed = [0]

    def replace_act(m):
        act_val = int(m.group(1))
        rest = m.group(2)
        if act_val == 1:
            regular_fixed[0] += 1
            return f'65{rest}'
        elif act_val == 33554435:
            # Remove solo (33554432), add stay_area (64), add boss (67108864)
            new_val = act_val - 33554432 + 64 + 67108864
            # = 1 + 2 + 64 + 67108864 = 67108931
            boss_fixed[0] += 1
            return f'{new_val}{rest}'
        else:
            return m.group(0)

    # Match lines like "1 0 0 S" or "33554435 0 0 S"
    # The act line format is: <act> <affected_by> <alignment> S
    result = re.sub(r'^(1|33554435)( \d+ \d+ S)$', replace_act, mobiles_section, flags=re.MULTILINE)
    return result, regular_fixed[0], boss_fixed[0]


# ============================================================
# Fix 3: Add ITEM_BOSS to boss items 8890-8894
# ============================================================

ITEM_BOSS = 134217728

def fix_boss_items(objects_section):
    """
    Items 8890-8894 are boss drops and need ITEM_BOSS (134217728) added to extra_flags.
    We find each vnum block and OR in ITEM_BOSS to whatever the current extra_flags is.

    Object stat line format: <item_type> <extra_flags> <wear_flags> <item_apply>
    """
    boss_vnums = [8890, 8891, 8892, 8893, 8894]
    fixed_count = [0]

    def fix_boss_item_block(text, vnum):
        # Find position of this vnum
        start_marker = f'\n#{vnum}\n'
        start_pos = text.find(start_marker)
        if start_pos == -1:
            start_marker = f'#{vnum}\n'
            start_pos = text.find(start_marker)
            if start_pos == -1:
                return text, False

        # Find the next vnum or end of section
        next_vnum_pos = text.find('\n#', start_pos + 1)
        if next_vnum_pos == -1:
            block = text[start_pos:]
            prefix = text[:start_pos]
            after = ''
        else:
            block = text[start_pos:next_vnum_pos]
            prefix = text[:start_pos]
            after = text[next_vnum_pos:]

        # In the block, find the item stat line (item_type extra_flags wear_flags item_apply)
        # and add ITEM_BOSS to extra_flags
        def add_boss_flag(m):
            itype = int(m.group(1))
            eflags = int(m.group(2))
            wflags = int(m.group(3))
            iapply = int(m.group(4))
            new_eflags = eflags | ITEM_BOSS
            fixed_count[0] += 1
            return f'{itype} {new_eflags} {wflags} {iapply}'

        new_block = re.sub(r'^(\d+) (\d+) (\d+) (\d+)$', add_boss_flag, block, flags=re.MULTILINE)
        if new_block != block:
            return prefix + new_block + after, True
        return text, False

    result = objects_section
    for vnum in boss_vnums:
        result, _ = fix_boss_item_block(result, vnum)

    return result, fixed_count[0]


# ============================================================
# Fix 4: Remove ITEM_BUCKLER from items without hold wear_flag
# ============================================================

ITEM_BUCKLER = 268435456
HOLD_FLAG = 32768

def fix_buckler_items(objects_section):
    """
    For each object, check if wear_flags has HOLD_FLAG (32768).
    If not, and extra_flags has ITEM_BUCKLER (268435456), remove ITEM_BUCKLER.

    Object stat line format: <item_type> <extra_flags> <wear_flags> <item_apply>
    We process line by line.
    """
    fixed_count = [0]
    lines = objects_section.split('\n')
    result_lines = []

    for line in lines:
        # Match item stat line: integer integer integer integer
        m = re.match(r'^(\d+) (\d+) (\d+) (\d+)$', line)
        if m:
            item_type = int(m.group(1))
            extra_flags = int(m.group(2))
            wear_flags = int(m.group(3))
            item_apply = int(m.group(4))

            if (extra_flags & ITEM_BUCKLER) and not (wear_flags & HOLD_FLAG):
                new_extra = extra_flags & ~ITEM_BUCKLER
                fixed_count[0] += 1
                line = f'{item_type} {new_extra} {wear_flags} {item_apply}'

        result_lines.append(line)

    return '\n'.join(result_lines), fixed_count[0]


# ============================================================
# Fix 5: key_vnum = 0 should be -1 for exits to 8600-8609
# ============================================================

def fix_key_vnum(rooms_section):
    """
    Pattern: "0 0 860[0-9]" -> "0 -1 860[0-9]"
    These are exit lines: <locks> <key_vnum> <to_room>
    """
    original = rooms_section
    result = re.sub(r'^0 0 (860[0-9])$', r'0 -1 \1', rooms_section, flags=re.MULTILINE)
    count = sum(1 for _ in re.finditer(r'^0 -1 860[0-9]$', result, re.MULTILINE)) if result != original else 0
    # Count actual replacements
    before_count = len(re.findall(r'^0 0 860[0-9]$', original, re.MULTILINE))
    return result, before_count


# ============================================================
# Fix 6: Remove blank lines in #RESETS
# ============================================================

def fix_resets_blank_lines(resets_section):
    """Remove all blank lines within the #RESETS section."""
    lines = resets_section.split('\n')
    result_lines = []
    blank_count = 0
    for line in lines:
        if line.strip() == '':
            blank_count += 1
        else:
            result_lines.append(line)
    # Rejoin (we might need a trailing newline)
    return '\n'.join(result_lines) + '\n', blank_count


# ============================================================
# Fix 8 & 9: Room descriptions and extra desc fix
# ============================================================

def fix_rooms_section(rooms_section):
    """
    Process each room in the #ROOMS section:
    1. Ensure unique descriptions (generate new ones for duplicates)
    2. Fix extra desc keyword from "route line~" to "banners~"
    3. Fix extra desc content
    """
    seen_descriptions = {}
    desc_fixed = [0]
    extra_desc_fixed = [0]

    # Split rooms section into individual room blocks
    # Each room starts with #<vnum>
    room_blocks = re.split(r'(?=^#\d+\n)', rooms_section, flags=re.MULTILINE)

    result_blocks = []
    for block in room_blocks:
        if not block.strip():
            result_blocks.append(block)
            continue

        # Check if this is a room block (starts with #vnum)
        vnum_match = re.match(r'^#(\d+)\n', block)
        if not vnum_match:
            result_blocks.append(block)
            continue

        vnum = int(vnum_match.group(1))

        # Only process rooms in our range
        if not (8700 <= vnum <= 8899):
            result_blocks.append(block)
            continue

        # Fix extra desc: "route line~" -> "banners~"
        if 'route line~' in block:
            old_extra = (
                'E\nroute line~\n'
                'Painted arrows and knot-marks imply the intended progression: west-to-east across a row, then south to the next line.~'
            )
            new_extra = (
                'E\nbanners~\n'
                'Close inspection of the nearest @@lbright blue@@N banner reveals route-marks and painted arrows indicating the intended progression: west-to-east across a row, then south to the next.\n~'
            )
            if old_extra in block:
                block = block.replace(old_extra, new_extra)
                extra_desc_fixed[0] += 1
            else:
                # Try a more flexible replacement
                block = re.sub(
                    r'E\nroute line~\n.*?~',
                    new_extra,
                    block,
                    flags=re.DOTALL
                )
                extra_desc_fixed[0] += 1

        # Extract room description (4th field: after name~, then the description ends with ~)
        # Room format:
        # #vnum
        # name~
        # description~   (or multi-line description ending with ~)
        # flags sector
        # exits...
        # E
        # keyword~
        # desc~
        # S

        # Find the description: it's after the name line (which ends with ~)
        # The name is the first tilde-terminated string
        # The description is the second tilde-terminated string

        # Split by tildes to find the description segment
        tilde_parts = block.split('~')
        # tilde_parts[0] = "#vnum\nname"
        # tilde_parts[1] = "\ndescription content\n"  (the room desc)
        # tilde_parts[2] = "\n..." (room flags/exits etc or single-line desc)

        if len(tilde_parts) < 3:
            result_blocks.append(block)
            continue

        # The description is tilde_parts[1]
        desc_content = tilde_parts[1]

        # Check if this description is a duplicate
        # Normalize for comparison
        desc_normalized = desc_content.strip()

        if desc_normalized in seen_descriptions and seen_descriptions[desc_normalized] != vnum:
            # This is a duplicate - generate a new description
            new_desc = '\n' + make_room_description(vnum)
            tilde_parts[1] = new_desc
            block = '~'.join(tilde_parts)
            desc_fixed[0] += 1
        else:
            seen_descriptions[desc_normalized] = vnum

        result_blocks.append(block)

    result = ''.join(result_blocks)
    return result, desc_fixed[0], extra_desc_fixed[0]


# ============================================================
# Main processing
# ============================================================

def main():
    print(f"Reading {INPUT_FILE}...")
    with open(INPUT_FILE, 'r') as f:
        text = f.read()

    print(f"File size: {len(text)} bytes, {text.count(chr(10))} lines")

    # Split into sections
    sections = split_into_sections(text)
    section_names = [s[0] for s in sections]
    print(f"Sections found: {section_names}")

    section_map = {name: content for name, content in sections}

    stats = {}

    # --------------------------------------------------------
    # Fix 1 & 9: Process #ROOMS - fix blank lines, descriptions, extra desc
    # --------------------------------------------------------
    print("\n[Fix 1+8+9] Processing #ROOMS section...")
    rooms = section_map.get('#ROOMS', '')

    # First fix extra desc and generate unique descriptions
    rooms, desc_fixed, extra_desc_fixed = fix_rooms_section(rooms)
    stats['descriptions_fixed'] = desc_fixed
    stats['extra_desc_fixed'] = extra_desc_fixed
    print(f"  Duplicate descriptions replaced: {desc_fixed}")
    print(f"  Extra desc keywords fixed: {extra_desc_fixed}")

    # Then fix blank lines in strings
    rooms, blank_count_rooms = fix_blank_lines_in_strings(rooms)
    stats['blank_lines_rooms'] = blank_count_rooms
    print(f"  Blank line groups removed from room strings: {blank_count_rooms}")

    # Fix 5: key_vnum
    rooms, key_vnum_fixed = fix_key_vnum(rooms)
    stats['key_vnum_fixed'] = key_vnum_fixed
    print(f"  key_vnum=0 fixed to -1: {key_vnum_fixed}")

    section_map['#ROOMS'] = rooms

    # --------------------------------------------------------
    # Fix 1: Process #MOBILES - fix blank lines in strings
    # --------------------------------------------------------
    print("\n[Fix 1+2] Processing #MOBILES section...")
    mobiles = section_map.get('#MOBILES', '')

    # Fix blank lines first
    mobiles, blank_count_mobs = fix_blank_lines_in_strings(mobiles)
    stats['blank_lines_mobiles'] = blank_count_mobs
    print(f"  Blank line groups removed from mobile strings: {blank_count_mobs}")

    # Fix act flags
    mobiles, regular_fixed, boss_fixed = fix_mobile_act_flags(mobiles)
    stats['regular_mobs_fixed'] = regular_fixed
    stats['boss_mobs_fixed'] = boss_fixed
    print(f"  Regular mob act flags fixed (1->65): {regular_fixed}")
    print(f"  Boss mob act flags fixed (33554435->67108931): {boss_fixed}")

    section_map['#MOBILES'] = mobiles

    # --------------------------------------------------------
    # Fix 3 & 4: Process #OBJECTS - fix boss items and buckler items
    # --------------------------------------------------------
    print("\n[Fix 3+4] Processing #OBJECTS section...")
    objects = section_map.get('#OBJECTS', '')

    # Fix boss items FIRST (add ITEM_BOSS) before buckler removal can affect them
    objects, boss_items_fixed = fix_boss_items(objects)
    stats['boss_items_fixed'] = boss_items_fixed
    print(f"  Boss items with ITEM_BOSS added: {boss_items_fixed}")

    # Fix buckler items (remove ITEM_BUCKLER from non-hold items)
    objects, buckler_fixed = fix_buckler_items(objects)
    stats['buckler_fixed'] = buckler_fixed
    print(f"  ITEM_BUCKLER removed from non-hold items: {buckler_fixed}")

    section_map['#OBJECTS'] = objects

    # --------------------------------------------------------
    # Fix 6: Process #RESETS - remove blank lines
    # --------------------------------------------------------
    print("\n[Fix 6] Processing #RESETS section...")
    resets = section_map.get('#RESETS', '')
    resets, blank_count_resets = fix_resets_blank_lines(resets)
    stats['blank_lines_resets'] = blank_count_resets
    print(f"  Blank lines removed from #RESETS: {blank_count_resets}")
    section_map['#RESETS'] = resets

    # --------------------------------------------------------
    # Fix 7: Reorder sections
    # --------------------------------------------------------
    print("\n[Fix 7] Reordering sections...")
    # Target order: #AREA, #MOBILES, #OBJECTS, #ROOMS, #RESETS, #$
    desired_order = ['#AREA', '#MOBILES', '#OBJECTS', '#ROOMS', '#RESETS', '#$']

    result_parts = []
    for name in desired_order:
        if name in section_map:
            result_parts.append(section_map[name])
        else:
            print(f"  WARNING: Section {name} not found!")

    print(f"  Original section order: {section_names}")
    print(f"  New section order: {desired_order}")

    result_text = ''.join(result_parts)

    # --------------------------------------------------------
    # Write output
    # --------------------------------------------------------
    print(f"\nWriting {OUTPUT_FILE}...")
    with open(OUTPUT_FILE, 'w') as f:
        f.write(result_text)

    print(f"Output file size: {len(result_text)} bytes, {result_text.count(chr(10))} lines")

    print("\n=== SUMMARY ===")
    for key, val in stats.items():
        print(f"  {key}: {val}")

    # Verify some fixes
    print("\n=== VERIFICATION ===")

    # Check no \n\n in strings (sample check)
    double_newline_count = 0
    in_string = False
    parts = result_text.split('~')
    for i, part in enumerate(parts):
        if i % 2 == 1 and '\n\n' in part:
            double_newline_count += 1
    print(f"  Remaining \\n\\n in string content: {double_newline_count}")

    # Check act flags
    remaining_old_act = len(re.findall(r'^1 0 0 S$', result_text, re.MULTILINE))
    remaining_boss_act = len(re.findall(r'^33554435 0 0 S$', result_text, re.MULTILINE))
    print(f"  Remaining old act=1 (should be 0): {remaining_old_act}")
    print(f"  Remaining old boss act=33554435 (should be 0): {remaining_boss_act}")
    new_act = len(re.findall(r'^65 0 0 S$', result_text, re.MULTILINE))
    new_boss_act = len(re.findall(r'^67108931 0 0 S$', result_text, re.MULTILINE))
    print(f"  New act=65 (should be 80): {new_act}")
    print(f"  New boss act=67108931 (should be 5): {new_boss_act}")

    # Check route line keyword gone
    route_line_remaining = result_text.count('route line~')
    print(f"  Remaining 'route line~' keywords (should be 0): {route_line_remaining}")

    # Check key_vnum
    old_key = len(re.findall(r'^0 0 860[0-9]$', result_text, re.MULTILINE))
    new_key = len(re.findall(r'^0 -1 860[0-9]$', result_text, re.MULTILINE))
    print(f"  Remaining 0 0 860x exits (should be 0): {old_key}")
    print(f"  Fixed 0 -1 860x exits (should be 10): {new_key}")

    # Check section order
    area_pos = result_text.find('#AREA')
    mobiles_pos = result_text.find('#MOBILES')
    objects_pos = result_text.find('#OBJECTS')
    rooms_pos = result_text.find('#ROOMS')
    resets_pos = result_text.find('#RESETS')
    print(f"  Section order positions - AREA:{area_pos} MOBILES:{mobiles_pos} OBJECTS:{objects_pos} ROOMS:{rooms_pos} RESETS:{resets_pos}")
    order_ok = area_pos < mobiles_pos < objects_pos < rooms_pos < resets_pos
    print(f"  Section order correct: {order_ok}")

    # Check boss items
    # After ITEM_BOSS added (134217728) and ITEM_BUCKLER removed (268435456) from 268435648:
    # 268435648 | 134217728 = 402653376, then 402653376 & ~268435456 = 134217920
    boss_item_final = len(re.findall(r'\b9 134217920 ', result_text))
    # Also count how many boss items have ITEM_BOSS bit set
    import re as re2
    boss_with_item_boss = 0
    for m in re2.finditer(r'^9 (\d+) ', result_text, re.MULTILINE):
        ef = int(m.group(1))
        if ef & ITEM_BOSS:
            boss_with_item_boss += 1
    print(f"  Boss items with final extra_flags 134217920 (should be 5): {boss_item_final}")
    print(f"  Items with ITEM_BOSS bit set (should be 5): {boss_with_item_boss}")

    # Check blank lines in resets
    resets_section_start = result_text.find('#RESETS')
    resets_section_end = result_text.find('#$', resets_section_start)
    resets_content = result_text[resets_section_start:resets_section_end]
    blank_in_resets = len(re.findall(r'\n\n', resets_content))
    print(f"  Blank lines remaining in #RESETS (should be 0): {blank_in_resets}")

    print("\nDone!")


if __name__ == '__main__':
    main()
