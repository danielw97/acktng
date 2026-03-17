# NPC LoRA Training Data

This folder contains ChatML `.jsonl` training data for NPC dialogue LoRA tuning,
aligned with `docs/proposals/ai.md` section 10.

- `npc_training.jsonl`: 1000 examples (one JSON object per line).
- Every system prompt includes explicit conditioning tags:
  - `[CITY_ACCENT:<city>]`
  - `[RACIAL_INCLINATION:<race>]`
- Every system prompt also includes both section-10 style directives:
  - `Speak with a <City> accent: ...`
  - `Your <Race> nature colors your speech: ...`
- City accent definitions follow the section-10 comments:
  - Midgaard: clipped bureaucratic phrasing, ledger idiom
  - Kowloon: terse, elliptic, proverb-laden
  - Mafdet: formal titles, trade jargon, oath references
  - Kiess: measured, watchful phrasing, frontier caution
  - Rakuen: warm but weary, garden metaphors, communal
- Racial speech inclinations cover all races listed in section 10:
  Human, Khenari, Khephari, Ashborn, Umbral, Rivennid, Deltari, Ushabti,
  Serathi, and Kethari.
- Balanced matrix: 5 cities × 10 races × 5 archetypes × 4 scenarios = 1000 rows.
- Includes both single-turn and multi-turn samples, plus in-world refusals for
  immersion-breaking prompts.
