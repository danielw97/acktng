#!/usr/bin/env python3
"""
Merge rocroad_byways.are into rocroad.are to create a single combined area.
Updates area.lst and area_index.md accordingly.
"""

import re
import os

AREA_DIR = '/home/user/acktng/area'


def parse_sections(filepath):
    """Parse area file into a dict of section_name -> content_text."""
    with open(filepath) as f:
        content = f.read()

    sections = {}
    # Split on section headers: #WORD at line start
    # Section headers: #AREA, #ROOMS, #MOBILES, #OBJECTS, #SHOPS, #RESETS,
    #                  #SPECIALS, #OBJFUNS, #$
    pattern = re.compile(r'^(#(?:AREA|ROOMS|MOBILES|OBJECTS|SHOPS|RESETS|SPECIALS|OBJFUNS|\$))\n',
                         re.MULTILINE)

    matches = list(pattern.finditer(content))
    for i, m in enumerate(matches):
        section_name = m.group(1)
        start = m.end()
        end = matches[i+1].start() if i+1 < len(matches) else len(content)
        section_content = content[start:end]
        # Some sections appear twice (e.g. duplicate #MOBILES in rocroad_byways)
        # Strip terminator (#0 or S) from previous content before concatenating
        if section_name in sections:
            prev = sections[section_name].rstrip('\n').rstrip()
            if prev.endswith('#0'):
                prev = prev[:-2].rstrip()
            elif prev.endswith('S'):
                prev = prev[:-1].rstrip()
            sections[section_name] = prev + '\n' + section_content
        else:
            sections[section_name] = section_content

    return sections


def merge_entity_section(content_a, content_b):
    """Merge two entity sections (ROOMS, MOBILES, OBJECTS).

    Each section ends with '#0\n'. Combine entities from both,
    with a single #0 at the end.
    """
    # Strip trailing #0 from content_a
    a = content_a.rstrip()
    if a.endswith('#0'):
        a = a[:-2].rstrip()
    else:
        # Remove last #0 line
        lines = a.split('\n')
        while lines and lines[-1].strip() in ('#0', ''):
            lines.pop()
        a = '\n'.join(lines)

    # Strip leading/trailing whitespace from content_b
    b = content_b.strip()

    return a.rstrip() + '\n' + b + '\n'


def merge_shops(content_a, content_b):
    """Merge SHOPS sections. Each ends with '0' on its own line."""
    lines_a = content_a.rstrip().split('\n')
    # Remove trailing '0' from a
    while lines_a and lines_a[-1].strip() == '0':
        lines_a.pop()

    lines_b = content_b.strip().split('\n')

    merged = '\n'.join(lines_a).rstrip()
    if merged:
        merged += '\n'
    merged += '\n'.join(lines_b) + '\n'
    return merged


def merge_terminated(content_a, content_b, terminator='S'):
    """Merge sections terminated by a single line containing terminator."""
    lines_a = content_a.rstrip().split('\n')
    # Remove trailing terminator from a
    while lines_a and lines_a[-1].strip() == terminator:
        lines_a.pop()

    lines_b = content_b.strip().split('\n')

    merged = '\n'.join(lines_a).rstrip()
    if merged:
        merged += '\n'
    merged += '\n'.join(lines_b) + '\n'
    return merged


def merge_area_header(header_a, header_b):
    """Create combined #AREA header using rocroad as base."""
    # Update the name in header_a to "Roc Road, Byways, and Forest Path"
    lines = header_a.split('\n')
    new_lines = []
    for i, line in enumerate(lines):
        stripped = line.strip()
        if i == 0 and '~' in stripped:
            # This is the area name line - update it
            new_lines.append('@@bRoc Road@@W, @@bByways@@W, and the @@GForest Path@@N~')
        elif stripped.startswith('K '):
            # Update keyword to include both
            new_lines.append('K rocroad rocroad_byways~')
        elif stripped.startswith('V '):
            # V range: use rocroad min, rocroad_byways max
            parts = stripped.split()
            # min from rocroad (3201), max from rocroad_byways (3649)
            new_lines.append(f'V {parts[1]} 3649')
        else:
            new_lines.append(line)
    return '\n'.join(new_lines)


def main():
    rocroad_path = os.path.join(AREA_DIR, 'rocroad.are')
    byways_path = os.path.join(AREA_DIR, 'rocroad_byways.are')

    print("Parsing rocroad.are...")
    rr = parse_sections(rocroad_path)

    print("Parsing rocroad_byways.are...")
    rb = parse_sections(byways_path)

    print("Merging sections...")

    # Build merged content
    merged_parts = []

    # #AREA header
    merged_parts.append('#AREA\n')
    merged_parts.append(merge_area_header(rr.get('#AREA', ''), rb.get('#AREA', '')))

    # #ROOMS
    merged_parts.append('#ROOMS\n')
    merged_parts.append(merge_entity_section(
        rr.get('#ROOMS', '#0\n'),
        rb.get('#ROOMS', '#0\n')
    ))

    # #MOBILES
    merged_parts.append('#MOBILES\n')
    merged_parts.append(merge_entity_section(
        rr.get('#MOBILES', '#0\n'),
        rb.get('#MOBILES', '#0\n')
    ))

    # #OBJECTS (if either has it)
    rr_obj = rr.get('#OBJECTS', '')
    rb_obj = rb.get('#OBJECTS', '')
    if rr_obj or rb_obj:
        merged_parts.append('#OBJECTS\n')
        merged_parts.append(merge_entity_section(
            rr_obj or '#0\n',
            rb_obj or '#0\n'
        ))

    # #SHOPS (if either has it)
    rr_shops = rr.get('#SHOPS', '')
    rb_shops = rb.get('#SHOPS', '')
    if rr_shops or rb_shops:
        merged_parts.append('#SHOPS\n')
        merged_parts.append(merge_shops(
            rr_shops or '0\n',
            rb_shops or '0\n'
        ))

    # #RESETS
    merged_parts.append('#RESETS\n')
    merged_parts.append(merge_terminated(
        rr.get('#RESETS', 'S\n'),
        rb.get('#RESETS', 'S\n'),
        'S'
    ))

    # #SPECIALS
    rr_spec = rr.get('#SPECIALS', '')
    rb_spec = rb.get('#SPECIALS', '')
    if rr_spec or rb_spec:
        merged_parts.append('#SPECIALS\n')
        merged_parts.append(merge_terminated(
            rr_spec or 'S\n',
            rb_spec or 'S\n',
            'S'
        ))

    # #OBJFUNS (if either has it)
    rr_of = rr.get('#OBJFUNS', '')
    rb_of = rb.get('#OBJFUNS', '')
    if rr_of or rb_of:
        merged_parts.append('#OBJFUNS\n')
        merged_parts.append(merge_terminated(
            rr_of or 'S\n',
            rb_of or 'S\n',
            'S'
        ))

    merged_parts.append('#$\n')

    merged_content = ''.join(merged_parts)

    # Write merged file
    print(f"Writing merged rocroad.are...")
    with open(rocroad_path, 'w') as f:
        f.write(merged_content)

    # Remove rocroad_byways.are from area.lst
    lst_path = os.path.join(AREA_DIR, 'area.lst')
    with open(lst_path) as f:
        lst_lines = f.readlines()

    new_lst = []
    removed = False
    for line in lst_lines:
        if line.strip() == 'rocroad_byways.are':
            removed = True
            # Also skip blank line before it if present
        else:
            new_lst.append(line)

    if removed:
        print("Removed rocroad_byways.are from area.lst")
        # Clean up consecutive blank lines
        cleaned = []
        prev_blank = False
        for line in new_lst:
            is_blank = line.strip() == ''
            if is_blank and prev_blank:
                continue
            cleaned.append(line)
            prev_blank = is_blank

        with open(lst_path, 'w') as f:
            f.writelines(cleaned)

    print("Done. rocroad_byways.are is now merged into rocroad.are")
    print("Note: rocroad_byways.are file is left on disk but removed from area.lst")


if __name__ == '__main__':
    main()
