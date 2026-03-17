# NPC LoRA Training Data

This folder contains ChatML `.jsonl` training data for NPC dialogue LoRA tuning,
aligned with `docs/proposals/ai.md` section 10.

## Files

- `npc_training.jsonl`: Training examples (one JSON object per line).

## Generating / Regenerating Data

Use `generate_npc_training_data.py` at the repo root:

```sh
# Requires anthropic package and API key
pip install anthropic
export ANTHROPIC_API_KEY=your-key-here

# Generate 1500 examples (default) into data/training/npc_training.jsonl
python3 generate_npc_training_data.py

# Custom count or output path
python3 generate_npc_training_data.py --count 1500 --out data/training/npc_training.jsonl

# Dry run (no API calls, writes placeholders to verify structure)
python3 generate_npc_training_data.py --dry-run

# Use a specific model (default: claude-haiku-4-5-20251001)
python3 generate_npc_training_data.py --model claude-haiku-4-5-20251001

# Reproducible output with fixed seed (default: 42)
python3 generate_npc_training_data.py --seed 42
```

## Dataset Spec

- **Target size:** 1500 examples
- **Matrix:** 5 cities × 10 races × 5 archetypes × 4 scenarios = 1000 base cells,
  cycled to 1500 with shuffled variety
- **Multi-turn:** ~25% of examples (≈375) are 2-turn conversations
- **Format:** ChatML `.jsonl` — one JSON object per line with a `messages` array

## System Prompt Format

Every system prompt includes:
- Explicit conditioning tags: `[CITY_ACCENT:<city>]` and `[RACIAL_INCLINATION:<race>]`
- Both section-10 style directives:
  - `Speak with a <City> accent: ...`
  - `Your <Race> nature colors your speech: ...`

## City Accents

| City | Speech Style |
|------|-------------|
| Midgaard | Clipped bureaucratic phrasing, ledger idiom |
| Kowloon | Terse, elliptic, proverb-laden |
| Mafdet | Formal titles, trade jargon, oath references |
| Kiess | Measured, watchful phrasing, frontier caution |
| Rakuen | Warm but weary, garden metaphors, communal |

## Racial Speech Inclinations

Covers all 10 races: Human, Khenari, Khephari, Ashborn, Umbral, Rivennid,
Deltari, Ushabti, Serathi, Kethari.

## Archetypes

City guard, innkeeper, temple clerk, harbor official, road warden.

## Scenario Types

1. `practical_advice` — traveler asks for useful local information
2. `local_knowledge` — traveler asks about history, culture, or places
3. `immersion_break` — player asks meta/out-of-character question (NPC deflects in-character)
4. `hostile_or_tricky` — player is rude, aggressive, or tries to manipulate
