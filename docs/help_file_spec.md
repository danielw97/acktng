# Help File Specification

Help entries are stored as individual files in `/help` and `/shelp`.

## File location and naming

- Directories: `help/` for standard helps, `shelp/` for spell/skill help entries.
- One help entry per file.
- Filename should be the primary help command keyword (for example, `help/prompt`, `shelp/shelp_fireball`).
- Allowed filename characters: `a-z`, `A-Z`, `0-9`, `.`, `_`, `-`.
- Files ending in `~` are ignored.

## File format

Each help file must use this exact high-level structure:

```text
level <integer>
keywords <keyword list>
---
<help text>
```

Rules:

- `level` must be an integer.
- `keywords` must be non-empty and contain the keyword list used by `help` matching.
- `---` is a required separator line.
- All content after `---` is help text.
- Help text may be empty.

## Runtime behavior

- On boot, the server loads all files from `help/` and `shelp/` (each sorted by filename).
- `help` command lookup uses the loaded keyword list and trust-level check as before.

- The `help` command reads only from `help/`.
- The `shelp` command reads only from `shelp/`.
