#!/usr/bin/env python3
"""
crusade_ai_gen.py — AI-powered crusade quest generator for ACK!MUD TNG.

Usage:
    python3 crusade_ai_gen.py <mob_name> <mob_level> <tier> <area_name> <output_path>

Calls the Anthropic Claude API to generate:
- A personality index (1–8) appropriate to the NPC and tier
- A stolen item name (short_descr and long_descr) that fits the NPC and world
- 17 message pairs (m1 / m2) for the crusade timer cycle

Output is a flat JSON file at <output_path> with keys:
    personality, item_short, item_long, m1_0..m1_16, m2_0..m2_16

The server reads this file once the script exits with status 0.
On any error the script exits with non-zero status; the server falls back to
static messages.
"""

import sys
import json
import os
import re

# ---------------------------------------------------------------------------
# World lore context (baked into the prompt for grounding)
# ---------------------------------------------------------------------------
WORLD_LORE = """
WORLD: ACK!MUD TNG — gritty low-fantasy medieval world. No humor, no anachronism.

GEOGRAPHY & CITIES:
- Midgaard: administrative capital, ledger culture, bureaucratic NPCs, Keeper heritage
- Kowloon: trade port, terse mercantile culture, proverb-laden speech
- Kiess: frontier city, watchful, measured speech, wilderness nearby
- Rakuen: communal, garden metaphors, weary warmth, collective framing
- Mafdet: formal oath culture, title-heavy speech, swift declarative sentences

HISTORY:
- Keeper of Measures: ancient civilization that built the five cities; their
  artefacts include brass rods, Measure Script tablets, salt-glass fragments,
  and survey instruments. Now long collapsed but relics remain.
- Solar Court: martial empire, campaign honours, iron seals, signet rings.
  The Ash-and-Iron Decades were its peak military era. Campaign veterans carry
  ashfall shards from the Cinderteeth Mountains.
- Crusader Corps: hereditary tradition of quest-givers; crusades are public
  appeals for aid, not religious wars. A crusade is a personal matter of honour.
- Dueling commissions: issued by the Swordsman guilds; folded parchment, wax seal.

TONE RULES:
- Medieval vocabulary only. No "okay", "cool", "you guys", "basically", etc.
- Avoid meta-humor. The NPC believes in their quest sincerely.
- Short sentences. 1–2 sentences per message. Color codes (@@a, @@l, @@e) are
  acceptable but not required.
- The 17-message cycle tracks time (each message = 1 minute of a 15-min quest):
  Messages 0–6: theft is discovered, thief unknown
  Messages 7–14: thief identified, urgency escalates
  Message 15: time-expiry / giving up
  Message 16: completion / item returned (uses player name via %s)
- For messages 7–14 m1 (thief alive): use %s for thief name, then %s for item name
- For messages 7–14 m2 (thief dead): use %s for item name only
- Message 16 m1: use %s for player name, then %s for item name
- Avoid repeating the exact same phrasing across consecutive messages.
"""

# ---------------------------------------------------------------------------
# Personality descriptions (for the AI to choose from)
# ---------------------------------------------------------------------------
PERSONALITIES = {
    1: "wimpy — tearful, helpless, childlike, begging tone",
    2: "cool — detached, philosophical, resigned, mildly sardonic",
    3: "mean — wrathful, dramatic, threatening, imperious",
    4: "noble — dignified, formal, honor-bound, restrained urgency",
    5: "merchant — transactional, calculating, framing loss as a debt",
    6: "devout — fervent, theological, fate-invoking, appeals to higher powers",
    7: "tragic — mournful, poetic, world-weary, elegiac tone",
    8: "cryptic — indirect, riddle-speaking, ominous, never names things plainly",
}


def build_prompt(mob_name: str, mob_level: int, tier: int, area_name: str) -> str:
    tier_guidance = {
        1: "low-power NPC (level 1–100), likely a common folk person or minor figure",
        2: "mid-tier NPC (level 101–149), likely a skilled professional or notable figure",
        3: "high-power NPC (level 150–170), likely a dangerous or significant figure",
    }.get(tier, "unknown tier NPC")

    personality_list = "\n".join(f"  {k}: {v}" for k, v in PERSONALITIES.items())

    return f"""You are generating crusade quest content for the MUD game ACK!TNG.

WORLD LORE:
{WORLD_LORE}

NPC CONTEXT:
- Name: {mob_name}
- Level: {mob_level} ({tier_guidance})
- Area: {area_name}

TASK:
Generate crusade quest content for this NPC. The NPC has just had an item stolen
and is appealing to adventurers on the crusade channel to recover it.

Choose the most fitting personality from this list:
{personality_list}

Then generate:
1. A stolen item appropriate to the NPC and world lore (e.g., a Keeper relic for
   a scholar in Midgaard, a campaign seal for a Solar Court veteran, etc.)
2. 17 message pairs following the timer structure described in the lore above.

OUTPUT FORMAT — respond with ONLY valid JSON, no commentary:
{{
  "personality": <integer 1-8>,
  "item_short": "<short name, e.g. 'a keeper's survey rod'>",
  "item_long": "<long desc, e.g. 'A brass survey rod etched with Measure Script lies here.'>",
  "m1_0": "<message 0 m1>", "m2_0": "",
  "m1_1": "<message 1 m1>", "m2_1": "",
  "m1_2": "<message 2 m1>", "m2_2": "",
  "m1_3": "<message 3 m1>", "m2_3": "",
  "m1_4": "<message 4 m1>", "m2_4": "",
  "m1_5": "<message 5 m1>", "m2_5": "",
  "m1_6": "<message 6 m1>", "m2_6": "",
  "m1_7": "<thief-named m1, use %s for thief then %s for item>",
  "m2_7": "<thief-dead m2, use %s for item>",
  "m1_8": "<thief-named m1>", "m2_8": "<thief-dead m2>",
  "m1_9": "<thief-named m1>", "m2_9": "<thief-dead m2>",
  "m1_10": "<thief-named m1>", "m2_10": "<thief-dead m2>",
  "m1_11": "<thief-named m1>", "m2_11": "<thief-dead m2>",
  "m1_12": "<thief-named m1>", "m2_12": "<thief-dead m2>",
  "m1_13": "<thief-named m1>", "m2_13": "<thief-dead m2>",
  "m1_14": "<give-up m1, use %s for thief, %s for item>",
  "m2_14": "<give-up m2 if thief dead, use %s for item>",
  "m1_15": "<expiry farewell, no %s substitutions>", "m2_15": "",
  "m1_16": "<completion, use %s for player name then %s for item name>", "m2_16": ""
}}"""


def extract_json(text: str) -> dict:
    """Extract JSON from the model response, handling markdown code fences."""
    # Strip markdown fences
    text = re.sub(r"```(?:json)?\s*", "", text).strip()

    # Find the outermost { ... }
    start = text.find("{")
    if start == -1:
        raise ValueError("No JSON object found in response")

    depth = 0
    end = start
    for i, ch in enumerate(text[start:], start):
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                end = i
                break

    return json.loads(text[start : end + 1])


def validate_output(data: dict) -> dict:
    """Validate required fields and return cleaned dict."""
    personality = int(data.get("personality", 1))
    if personality < 1 or personality > 8:
        personality = 1

    item_short = str(data.get("item_short", "a quest token")).strip()
    item_long = str(data.get("item_long", "A quest token lies here.")).strip()

    result = {
        "personality": personality,
        "item_short": item_short,
        "item_long": item_long,
    }

    for i in range(17):
        result[f"m1_{i}"] = str(data.get(f"m1_{i}", "")).strip()
        result[f"m2_{i}"] = str(data.get(f"m2_{i}", "")).strip()

    return result


def main():
    if len(sys.argv) < 6:
        print(
            "Usage: crusade_ai_gen.py <mob_name> <mob_level> <tier> <area_name> <output_path>",
            file=sys.stderr,
        )
        sys.exit(1)

    mob_name = sys.argv[1]
    mob_level = int(sys.argv[2])
    tier = int(sys.argv[3])
    area_name = sys.argv[4]
    output_path = sys.argv[5]

    try:
        import anthropic
    except ImportError:
        print("crusade_ai_gen: anthropic package not installed", file=sys.stderr)
        sys.exit(1)

    client = anthropic.Anthropic()  # uses ANTHROPIC_API_KEY from environment

    prompt = build_prompt(mob_name, mob_level, tier, area_name)

    try:
        message = client.messages.create(
            model="claude-opus-4-5",
            max_tokens=2048,
            messages=[{"role": "user", "content": prompt}],
        )
        response_text = message.content[0].text
    except Exception as e:
        print(f"crusade_ai_gen: API call failed: {e}", file=sys.stderr)
        sys.exit(1)

    try:
        raw = extract_json(response_text)
        result = validate_output(raw)
    except Exception as e:
        print(f"crusade_ai_gen: JSON parse failed: {e}", file=sys.stderr)
        print(f"Response was: {response_text[:500]}", file=sys.stderr)
        sys.exit(1)

    try:
        with open(output_path, "w") as f:
            json.dump(result, f, ensure_ascii=True, indent=2)
    except Exception as e:
        print(f"crusade_ai_gen: failed to write output: {e}", file=sys.stderr)
        sys.exit(1)

    sys.exit(0)


if __name__ == "__main__":
    main()
