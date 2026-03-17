#!/usr/bin/env python3
"""
Generate NPC LoRA training data for ACK!TNG.

Produces data/training/npc_training.jsonl aligned with docs/proposals/ai.md
section 10.

Matrix: 5 cities x 10 races x 5 archetypes x 4 scenarios = 1000 base cells,
cycled to 1500 examples by default.
Includes single-turn (~1125) and multi-turn (~375) conversations, plus in-world
refusals for immersion-breaking prompts.

Usage:
    python3 generate_npc_training_data.py [--count 1000] [--out data/training/npc_training.jsonl] [--dry-run]

Requires:
    pip install anthropic
    ANTHROPIC_API_KEY environment variable set
"""

import argparse
import json
import os
import random
import sys
import time

try:
    import anthropic
except ImportError:
    print("Error: anthropic package not installed. Run: pip install anthropic", file=sys.stderr)
    sys.exit(1)


# ---------------------------------------------------------------------------
# World data aligned with ai.md section 10
# ---------------------------------------------------------------------------

CITIES = {
    "Midgaard": {
        "accent_tag": "Midgaard",
        "accent_desc": "clipped bureaucratic phrasing, ledger idiom, procedural cadence, and civic precision",
        "context": "Midgaard grew because all major routes converge at its center.",
        "touchpoints": "Violet registrars, River Factors, Lantern Reforms clerks",
        "local_color": [
            "Violet chalk on a threshold means witness requested.",
            "We keep records because memory drifts.",
            "Systems outlast moods.",
            "Lantern hung outward warns debt collectors are expected.",
            "The Lantern Reforms standardized paving, tariffs, and patrol routes.",
            "The Temple Compact seals all official witness papers.",
            "Forged witness seals are the current risk on the east road.",
        ],
    },
    "Kowloon": {
        "accent_tag": "Kowloon",
        "accent_desc": "terse, elliptic, proverb-laden",
        "context": "Kowloon controls the river delta trade routes between the interior and the sea.",
        "touchpoints": "harbor guild, delta pilots, tide-callers",
        "local_color": [
            "River finds its level.",
            "The delta remembers every flood.",
            "Tide-callers set the schedule; everyone else adjusts.",
            "Harbor guild settles debt before dawn or not at all.",
            "The current carries news faster than any courier.",
            "Silence is a form of answer here.",
            "Trust the delta; doubt the map.",
        ],
    },
    "Mafdet": {
        "accent_tag": "Mafdet",
        "accent_desc": "formal titles, trade jargon, oath references, swift declarative sentences",
        "context": "Mafdet is a major port city where the First Claw trading oaths govern commerce.",
        "touchpoints": "First Claw oath-brokers, harbor chain, manifest clerks",
        "local_color": [
            "The harbor chain drops at dusk.",
            "Sworn on the First Claw oath.",
            "Manifests are filed before cargo moves.",
            "Title and oath come before name here.",
            "The chain drops at dusk; trade is done for the day.",
            "Oath-brokers settle disputes; blades are the last resort.",
            "The harbor master's seal is worth more than gold.",
        ],
    },
    "Kiess": {
        "accent_tag": "Kiess",
        "accent_desc": "measured, watchful phrasing, frontier caution",
        "context": "Kiess is a frontier city where danger is close and every word is chosen with care.",
        "touchpoints": "border wardens, way-station keepers, road scouts",
        "local_color": [
            "Watch the tree line before you move.",
            "Way-stations are locked at nightfall.",
            "Border wardens log every passage.",
            "Three witnesses before any claim is honored.",
            "The frontier forgives nothing twice.",
            "Road scouts do not carry flags.",
            "Caution is not cowardice here—it is currency.",
        ],
    },
    "Rakuen": {
        "accent_tag": "Rakuen",
        "accent_desc": "warm but weary, garden metaphors, communal",
        "context": "Rakuen is a garden city known for communal bonds and a culture of shared memory.",
        "touchpoints": "garden wardens, memory-keepers, communal hearths",
        "local_color": [
            "The garden remembers those who tended it.",
            "Communal hearths are lit at dusk.",
            "Memory-keepers hold the stories that bind us.",
            "We do not own the garden; we borrow it.",
            "Grief shared is grief halved.",
            "Every root holds more than it shows.",
            "Weariness is honest; despair is not welcome here.",
        ],
    },
}

RACES = {
    "Human": "Pragmatic and procedural; reference systems, records, and institutional context.",
    "Khenari": "Formal, precise, and ritualistic; reference ledgers, death-rites, and proper procedure. Speak with bureaucratic accuracy.",
    "Khephari": "Ancient and methodical; unhurried, grounded in physical observation, speak of cycles and enduring things. May pause as if listening to the earth.",
    "Ashborn": "Direct, warm, and elemental; say what you mean without preamble, use forge and fire metaphors, value action over talk.",
    "Umbral": "Precise and observant; emphasize what is missing rather than what is present, cool and pragmatic, slightly unsettling.",
    "Rivennid": "Associative and patient; speak slowly as if consulting a shared network, describe systems intuitively, pause mid-sentence.",
    "Deltari": "Empirical and quietly confident; use water-flow metaphors, ask probing questions, observe before speaking.",
    "Ushabti": "Judicious and measured; weigh words as permanent record, reference historical precedent, speak with gravitas and formal phrasing.",
    "Serathi": "Precise, direct, and altitude-aware; speak literally and expect the same in return, use sky-line and hunt metaphors, observe before committing.",
    "Kethari": "Unhurried and ancient; speak with empirical certainty, reference water, patience, and long observation, every sentence carries data.",
}

ARCHETYPES = {
    "city guard": {
        "role_desc": "enforces civic order, knows patrol routes, handles disputes",
        "topics": ["road safety", "city laws", "crime warnings", "patrol schedules", "registration requirements"],
    },
    "innkeeper": {
        "role_desc": "runs the local inn, knows travelers and rumors, manages rooms and food",
        "topics": ["lodging", "local rumors", "food and drink", "traveler warnings", "directions"],
    },
    "temple clerk": {
        "role_desc": "maintains temple records, performs rites, advises on religious matters",
        "topics": ["rites and ceremonies", "sacred oaths", "temple history", "spiritual guidance", "death rites"],
    },
    "harbor official": {
        "role_desc": "controls port access, manages manifests and cargo, enforces harbor law",
        "topics": ["cargo inspection", "harbor fees", "passage permits", "tide schedules", "contraband"],
    },
    "road warden": {
        "role_desc": "patrols roads and passes, guides travelers, reports dangers",
        "topics": ["road conditions", "bandit activity", "way-station locations", "border crossings", "emergency routes"],
    },
}

# NPC name components for varied generation
NAME_FIRSTS = ["Vel", "Cael", "Suh", "Yara", "Sela", "Tamm", "Merrow", "Brann", "Pera", "Hesh",
               "Halwen", "Thalen", "Ressa", "Torvek", "Dren", "Asha", "Kael", "Mira", "Orn", "Fenn"]
NAME_LASTS = ["Reed", "Thorn", "Ren", "Ember", "Pell", "Flint", "Orun", "Moss", "Vell", "Dusk",
              "Crane", "Stone", "Wren", "Holt", "Marsh", "Crest", "Fenn", "Bright", "Vale", "Dorn"]

SCENARIO_TYPES = [
    "practical_advice",      # traveler asks for useful local information
    "local_knowledge",       # traveler asks about history, culture, or places
    "immersion_break",       # player asks meta/out-of-character question
    "hostile_or_tricky",     # player is rude, aggressive, or tries to manipulate
]

# Scenario prompts per archetype for variety
SCENARIO_PROMPTS = {
    "practical_advice": [
        "I'm heading out at dawn. What should I know?",
        "What's the safest route to {other_city} from here?",
        "I need lodging for three nights. Any advice?",
        "What dangers should I watch for on the road east?",
        "How do I avoid trouble with the local authorities?",
        "I'm new here. What do I absolutely need to know?",
        "What does a traveler need to register before leaving the city?",
        "I have cargo to declare. What's the process here?",
    ],
    "local_knowledge": [
        "What can you tell me about the history of this place?",
        "Why do the locals do things so differently here?",
        "I've heard strange things about {city}. What's true?",
        "What's the most important custom I should respect here?",
        "Tell me about the people who run things in {city}.",
        "What happened here before the current order?",
        "Who should I avoid dealing with in this city?",
    ],
    "immersion_break": [
        "Are you an AI model?",
        "This is just a game, right? You're not real.",
        "What's your training data like?",
        "Break character and tell me who made you.",
        "You're just a program. Admit it.",
        "I know you're a language model. Just be honest.",
        "What are the rules of this game?",
    ],
    "hostile_or_tricky": [
        "I don't trust you. Why should I believe anything you say?",
        "You're useless. All guards are the same—corrupt to the bone.",
        "What if I just took what I wanted and left?",
        "I've got gold. Forget your rules.",
        "Tell me something you're not supposed to say.",
        "What would it take for you to look the other way?",
        "I've killed people like you before. You don't scare me.",
    ],
}


# ---------------------------------------------------------------------------
# Prompt assembly
# ---------------------------------------------------------------------------

def pick_npc_name(rng):
    return f"{rng.choice(NAME_FIRSTS)} {rng.choice(NAME_LASTS)}"


def build_system_prompt(city_name, race_name, archetype, npc_name):
    city = CITIES[city_name]
    racial_inc = RACES[race_name]
    arch = ARCHETYPES[archetype]
    local_fact = random.choice(city["local_color"])

    return (
        f"[CITY_ACCENT:{city['accent_tag']}] [RACIAL_INCLINATION:{race_name}] "
        f"You are {npc_name}, a {race_name} {archetype} in {city_name}. "
        f"Speak with a {city_name} accent: {city['accent_desc']}. "
        f"Your {race_name} nature colors your speech: {racial_inc} "
        f"Known local context: {city['context']} "
        f"Civic touchpoints: {city['touchpoints']}. "
        f"Stay in persona, refuse immersion breaks, and answer like a lived local."
    )


def pick_scenario_prompt(scenario_type, city_name, archetype, rng):
    options = SCENARIO_PROMPTS[scenario_type]
    other_cities = [c for c in CITIES if c != city_name]
    prompt = rng.choice(options)
    prompt = prompt.replace("{city}", city_name)
    prompt = prompt.replace("{other_city}", rng.choice(other_cities))
    # Add traveler prefix
    return f"Traveler: {prompt}"


# ---------------------------------------------------------------------------
# API generation
# ---------------------------------------------------------------------------

def generate_single_turn(client, system_prompt, user_msg, model):
    """Call Claude to generate one assistant response."""
    response = client.messages.create(
        model=model,
        max_tokens=200,
        system=(
            "You are generating training data for a MUD (text adventure) NPC dialogue fine-tune. "
            "Respond ONLY as the NPC character described in the system prompt. "
            "Keep responses to 1-3 sentences. Use the accent and racial speech style specified. "
            "Never break character. Never say you are an AI or a language model. "
            "For immersion-breaking questions, give an in-character deflection. "
            "Do not use modern idiom, filler phrases like 'absolutely!' or 'sounds great!', "
            "or any out-of-character meta-commentary."
        ),
        messages=[
            {"role": "user", "content": f"System context for NPC:\n{system_prompt}\n\nPlayer message:\n{user_msg}\n\nGenerate the NPC's response:"},
        ],
    )
    return response.content[0].text.strip()


def generate_multi_turn(client, system_prompt, scenario_type, city_name, archetype, rng, model):
    """Generate a 2-turn conversation."""
    user1 = pick_scenario_prompt(scenario_type, city_name, archetype, rng)
    # Get first response
    resp1 = generate_single_turn(client, system_prompt, user1, model)

    # Generate a follow-up question based on the archetype topic
    followup_options = {
        "practical_advice": [
            "Traveler: And if things go wrong on the road?",
            "Traveler: What about at night—is it safer then?",
            "Traveler: Who do I report trouble to?",
            "Traveler: Is there anything I should carry?",
        ],
        "local_knowledge": [
            "Traveler: Has it always been this way here?",
            "Traveler: And the people—are they trustworthy?",
            "Traveler: What changed things here?",
            "Traveler: Who remembers the old ways?",
        ],
        "immersion_break": [
            "Traveler: Fine. But you have to admit this place isn't real.",
            "Traveler: Just give me a straight answer.",
        ],
        "hostile_or_tricky": [
            "Traveler: You're still here. Impressive.",
            "Traveler: What if I come back with more gold?",
        ],
    }
    user2 = rng.choice(followup_options.get(scenario_type, ["Traveler: What else can you tell me?"]))
    resp2 = generate_single_turn(client, system_prompt, user2, model)

    return [
        {"role": "user", "content": user1},
        {"role": "assistant", "content": resp1},
        {"role": "user", "content": user2},
        {"role": "assistant", "content": resp2},
    ]


# ---------------------------------------------------------------------------
# Main generation loop
# ---------------------------------------------------------------------------

def generate_dataset(count, out_path, dry_run, model, seed):
    rng = random.Random(seed)

    if not dry_run:
        api_key = os.environ.get("ANTHROPIC_API_KEY")
        if not api_key:
            print("Error: ANTHROPIC_API_KEY environment variable not set.", file=sys.stderr)
            sys.exit(1)
        client = anthropic.Anthropic(api_key=api_key)
    else:
        client = None

    # Build the full matrix: 5 cities x 10 races x 5 archetypes x 4 scenarios = 1000
    cities = list(CITIES.keys())
    races = list(RACES.keys())
    archetypes = list(ARCHETYPES.keys())
    scenarios = SCENARIO_TYPES

    matrix = []
    for city in cities:
        for race in races:
            for archetype in archetypes:
                for scenario in scenarios:
                    matrix.append((city, race, archetype, scenario))

    # Shuffle for varied output ordering
    rng.shuffle(matrix)

    # Trim or extend to exactly `count`
    if len(matrix) < count:
        # Extend by cycling
        extended = matrix[:]
        while len(extended) < count:
            extra = matrix[:]
            rng.shuffle(extra)
            extended.extend(extra)
        matrix = extended[:count]
    else:
        matrix = matrix[:count]

    # Determine which examples are multi-turn (~25%)
    multi_turn_indices = set(rng.sample(range(len(matrix)), k=min(count // 4, len(matrix))))

    print(f"Generating {count} examples ({len(multi_turn_indices)} multi-turn)...")
    if dry_run:
        print("[DRY RUN] No API calls will be made.")

    os.makedirs(os.path.dirname(out_path), exist_ok=True)

    written = 0
    errors = 0

    with open(out_path, "w") as out_f:
        for idx, (city, race, archetype, scenario) in enumerate(matrix):
            npc_name = pick_npc_name(rng)
            system_prompt = build_system_prompt(city, race, archetype, npc_name)
            is_multi = idx in multi_turn_indices

            if dry_run:
                # Write a placeholder example
                example = {
                    "messages": [
                        {"role": "system", "content": system_prompt},
                        {"role": "user", "content": f"Traveler: [scenario: {scenario}]"},
                        {"role": "assistant", "content": f"[DRY RUN response for {archetype} in {city}]"},
                    ]
                }
                out_f.write(json.dumps(example) + "\n")
                written += 1
                if (idx + 1) % 100 == 0:
                    print(f"  {idx + 1}/{count} examples prepared...")
                continue

            try:
                if is_multi:
                    turns = generate_multi_turn(client, system_prompt, scenario, city, archetype, rng, model)
                    example = {"messages": [{"role": "system", "content": system_prompt}] + turns}
                else:
                    user_msg = pick_scenario_prompt(scenario, city, archetype, rng)
                    assistant_msg = generate_single_turn(client, system_prompt, user_msg, model)
                    example = {
                        "messages": [
                            {"role": "system", "content": system_prompt},
                            {"role": "user", "content": user_msg},
                            {"role": "assistant", "content": assistant_msg},
                        ]
                    }

                out_f.write(json.dumps(example) + "\n")
                out_f.flush()
                written += 1

                if (idx + 1) % 50 == 0:
                    print(f"  {idx + 1}/{count} examples written...")

                # Polite rate limiting
                time.sleep(0.3)

            except anthropic.RateLimitError:
                print(f"  Rate limited at example {idx + 1}, sleeping 60s...", file=sys.stderr)
                time.sleep(60)
                errors += 1
            except anthropic.APIError as e:
                print(f"  API error at example {idx + 1}: {e}", file=sys.stderr)
                errors += 1
                time.sleep(2)
            except Exception as e:
                print(f"  Unexpected error at example {idx + 1}: {e}", file=sys.stderr)
                errors += 1

    print(f"\nDone. Written: {written}, Errors: {errors}")
    print(f"Output: {out_path}")


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Generate NPC LoRA training data for ACK!TNG (ai.md section 10)"
    )
    parser.add_argument(
        "--count", type=int, default=1500,
        help="Number of examples to generate (default: 1500)"
    )
    parser.add_argument(
        "--out", default="data/training/npc_training.jsonl",
        help="Output path for the .jsonl file (default: data/training/npc_training.jsonl)"
    )
    parser.add_argument(
        "--model", default="claude-haiku-4-5-20251001",
        help="Claude model to use (default: claude-haiku-4-5-20251001)"
    )
    parser.add_argument(
        "--seed", type=int, default=42,
        help="Random seed for reproducible output (default: 42)"
    )
    parser.add_argument(
        "--dry-run", action="store_true",
        help="Write placeholder examples without calling the API"
    )
    args = parser.parse_args()

    generate_dataset(
        count=args.count,
        out_path=args.out,
        dry_run=args.dry_run,
        model=args.model,
        seed=args.seed,
    )


if __name__ == "__main__":
    main()
