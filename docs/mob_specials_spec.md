# Mob Specials Specification

Extracted from `docs/area_file_spec.md` section 10 and `src/special.c`.

## Overview

Mob specials (`spec_fun`) are C functions in `src/special.c` that run periodically on a mob (typically once per game pulse when the mob is chosen). Each function returns `TRUE` if it performed a significant action, `FALSE` otherwise. The game calls them by name via the `#SPECIALS` section of an area file.

## Assignment syntax (`#SPECIALS` section)

```text
#SPECIALS
M <mob_vnum> <spec_fun_name>
...
S
```

`*` comment lines are accepted. The `<mob_vnum>` must be within the area's declared vnum range.

## Builder policies

- Mob specials beginning with `spec_summon_` must **never** be set in area files. These are assigned and cleared at runtime by the summon systems only.
- Any mob special beginning with `spec_keep_` may only be assigned manually by a human builder (not by automation or generation tools).
- Each mob may have at most one special function. Assigning two `M` lines for the same vnum in `#SPECIALS` is invalid.

---

## Breath weapon specials

These fire only while the mob is fighting and check level requirements from the
breath attack tables.

| Special | Effect |
|---|---|
| `spec_breath_any` | Breathes a randomly chosen breath weapon (acid, fire, frost, gas, or lightning). |
| `spec_breath_acid` | Breathes acid. |
| `spec_breath_fire` | Breathes fire. |
| `spec_breath_frost` | Breathes frost. |
| `spec_breath_gas` | Breathes gas (room-wide). |
| `spec_breath_lightning` | Breathes lightning. |

---

## Combat caster specials

These fire only while the mob is fighting. They select a spell from a weighted
random table gated by mob level and cast it on a fighting target.

| Special | Spell pool |
|---|---|
| `spec_cast_cleric` | Cleric combat spells: curse, weaken, chill touch, blindness, poison, harm, teleport, gate. |
| `spec_cast_judge` | Dark judgment spells (undead-flavored cleric pool). |
| `spec_cast_mage` | Mage combat spells (magic missile up through acid blast and gate). |
| `spec_cast_undead` | Undead dark magic: blindness, chill touch, weaken, earthquake, dispel magic, curse, gate, fireball, summon. Also can teleport other undead into the fight. |
| `spec_cast_cadaver` | Casts `detect invis` briefly on a target in the room (non-combat; skips if already affected). |
| `spec_cast_bigtime` | High-level combat spell pool (level 30+): dispel magic, acid blast, phobia, chain lightning, mindflame, lava burst, static, hellspawn. Announces "invokes the dark powers." |
| `spec_cast_adept` | Out-of-combat support: maintains `healing light` and `mana flare` on the room, then randomly buffs a visible character with armor, bless, cure blindness, cure light, or refresh. |

---

## Law enforcement specials

| Special | Behavior |
|---|---|
| `spec_executioner` | Scans the room for characters with `PLR_KILLER` or `PLR_THIEF` flags, or for undead NPCs. On detection: yells the offense, calls `multi_hit` on the criminal, and spawns two city guards (`MOB_VNUM_CITYGUARD`). Does not fire while already fighting. |
| `spec_policeman` | Scans the room for `PLR_KILLER` or `PLR_THIEF` characters. On detection: yells the crime and sets a hunt on the criminal. Does not fire while already fighting. |
| `spec_guard` | Alias for `spec_policeman` (identical lookup return). |
| `spec_mino_guard` | Minotaur gate guard. Checks each player in the room for a specific minotaur pass item (`OBJ_VNUM_MINO_PASS`) held in either hand. Players with the pass are let through; players without it trigger an "Intruder Alert" yell and combat. |

---

## NPC behavior specials

| Special | Behavior |
|---|---|
| `spec_janitor` | Scans the room for takeable items that are drink containers, trash, or cost fewer than 10 gold. Picks one up per tick. Emits "picks up some trash." |
| `spec_fido` | Scans the room for NPC corpses. When found, extracts all contents into the room and then destroys the corpse ("savagely devours a corpse"). |
| `spec_thief` | Steals a percentage of a random awake player's gold each tick. If the player is awake and the roll fails, they notice ("discover $n's hands in your wallet"). |
| `spec_rewield` | Checks inventory and room for weapons with higher damage than the mob's current wield. Picks up and wields the best available weapon. Runs at 35% chance when not fighting, 60% when fighting. |
| `spec_poison` | During combat: bites the target for extra damage and applies the poison spell. Fires proportional to mob level. |
| `spec_mayor` | Complex movement/speech script for a city mayor: wakes at hour 6, walks a fixed path opening the city gate, greets citizens, and returns to sleep at hour 20 to close the gate. Falls back to `spec_cast_cleric` if attacked. |
| `spec_sage` | Story-telling NPC script. Wakes at hour 6, walks through a fixed sequence of speeches about the Sylai, produces food, and returns to sleep. Tells a multi-part narrative about a destroyed race. |
| `spec_stephen` | Teleports to a random room in the world each tick, then delivers a Christmas gift to a random player in that room. |
| `spec_tax_man` | Teleports to a random room, then taxes a random wealthy player (over 100,000 gold on hand or 1,000,000 banked) by deducting 1% of total wealth. Documents the event to the monitor channel. |
| `spec_vamp_hunter` | When actively hunting a vampire player: periodically tells the target threatening messages or yells to the room about the hunt. Does not act when not hunting. |
| `spec_sylai_priest` | Room-specific special (room 439 only). During combat, attempts to shove the fighting player off the outcrop into a lower room (433 or 436). 30% chance to attempt per tick; 40% success when attempted. |
| `spec_wizardofoz` | Disabled. Always returns `FALSE`. |
| `spec_keep_physical_captain` | Boss-tier combat special for keep captains. On a cycling counter: uses morale or leadership at step 1, warcry or berserk at step 4, announces a massive strike at step 5, and delivers the massive strike (9000–11000 physical/holy damage) at step 7. **Human-assignment only; do not use in generated area files.** |

---

## Ambient city specials

These fire out of combat only. They use `number_bits(3)` as an 8-way gate (approximately 12.5% chance per pulse that anything happens), then randomly emit an action (`act()`) or a spoken line (`do_say()`). They always return `FALSE` — they provide atmosphere, not game-state changes.

| Special | Flavor / setting | Sample lines |
|---|---|---|
| `spec_lamplighter` | Lamp maintenance worker. Trims wicks, checks oil levels, consults route cards. | *"No ward unlit, no traveler uncounted — that's the Oath, and that's the work."* |
| `spec_warden` | Arcane-compact checkpoint warden. Checks sigils, consults registry ledger, references containment doctrine. | *"All arcane goods require registry notation before this threshold. The Compact is clear."* |
| `spec_vendor` | Street market vendor. Calls out goods, discusses tariffs and supply chains. | *"Tariff board raised the western-goods levy again. Prices have to reflect it, nothing personal."* |
| `spec_lay_sister` | Temple memorial keeper. Tends lamps at markers, reads liturgy, recites the Three Oaths. | *"The Temple maintains rolls of the uncounted. Those removed from the ledgers are still remembered here."* |
| `spec_laborer` | Day laborer. Handles heavy loads, checks day-tokens, talks about working conditions. | *"Levy goes up, day-rate stays flat. Cinder debt, they call it — compounds whether you pay or not."* |

---

## Kiess ambient specials

Specials introduced for the Kiess area (`area/kiess.are`) to give civic NPCs
distinctive voice and behavior. All follow the same ambient-only pattern
(8-way gate, action or speech, always return `FALSE`).

| Special | Intended mob roles | Flavor / setting | Sample lines |
|---|---|---|---|
| `spec_kiess_shopkeeper` | Grand Market merchant, Steel & Edge weaponsmith, Bastion armorer, Argent Arcana mage-merchant, Cartographer, Mercenary Board registrar | Shop-floor presence: arranges display, greets arrivals, pitches wares, discusses supply. | *"Most of what you need for the frontier is right here. We stock for the road, not the shelf."* |
| `spec_kiess_innkeeper` | Wayfarer's Inn innkeeper, inn server | Inn atmosphere: checks guest ledger, refills cups, references memorial plaque, gives travel advice. | *"First night in Kiess? The memorial plaque by the door has names — not to alarm you, but to remind you to come back."* |
| `spec_kiess_scout` | Forest Confusion scout, Wall Command intelligence feed | Post-patrol debrief behavior: marks maps, discusses mist-line movement, shares centaur sign reports and bell-post status. | *"The mist line's moved another three hundred paces north since last week. That's fast for this season."* |
| `spec_kiess_orator` | Prism Square orator | Civic speech delivery: addresses an audience with a prepared speech on founding compact history, Evermeet's fall, and the city's design principles. Always emits both an action and a speech. | *"Three factions, one peace code. Not because they agree — they rarely agree — but because the alternative is Evermeet."* |
| `spec_kiess_wall_officer` | Wall Command duty officer | Military command room behavior: reviews duty roster, traces patrol routes on tactical map, dispatches couriers, speaks on doctrine and threat posture. | *"Wall Command doesn't guess at threat levels. We assess, we document, we act on evidence. That is the doctrine."* |

---

## Kowloon ambient specials

Specials introduced for the Kowloon area (`area/kowloon.are`) to give the
city's NPCs distinctive civic voice grounded in the Neon Covenant's history
and the five founding institutions. All follow the same ambient-only pattern
(8-way gate, action or speech, always return `FALSE`) unless noted otherwise.

| Special | Intended mob roles | Flavor / setting | Sample lines |
|---|---|---|---|
| `spec_kowloon_shopkeeper` | All Kowloon shop NPCs: weaponsmith, armorsmith, reagent keeper, provisioner, fletcher, jeweler, pawn broker, chandler, general merchant | Western market floor: hangs wares on hooks (old floating-stall tradition), checks jade inspection ribbons, greets arrivals, discusses Ledgerhouse dues, Guild Concordat minimums, and tariff-cleared goods. | *"Everything on this counter is Ledgerhouse-cleared. Tariff paid, weights checked, ribbon current."* |
| `spec_kowloon_gate_captain` | The four gate captains (Jade, Iron, Lantern, Tide) and the Harbor Syndic inspector | Gate inspection and security doctrine: reviews manifests against the Syndic ledger, scans approaches, signals subordinate guards, checks courier permits, references the seasonal rotation policy. | *"Seasonal rotation isn't tradition — it's policy. No one builds loyalty at a chokepoint. The Covenant was specific."* |
| `spec_kowloon_courier` | Lantern-runners and courier messengers of the Courier Lantern Office | Dispatch and route service: checks seal integrity, consults route cards, polishes the brass lantern badge, records delivery confirmations, scans junction bulletin boards. | *"The badge gets you through any gate at any hour. You'd be surprised how useful that is at second bell."* |
| `spec_kowloon_innkeeper` | Copper Tide innkeeper at the Inn | Inn hospitality with southern-quarter frontier character: checks guest ledger, adjusts counter lamp, sets tea for arrivals, references the city's promise to receive battered travelers. | *"If you're recall-safe and breathing, Kowloon will receive you. That's the city's oldest promise. We're just where you start."* |
| `spec_kowloon_corsair` | River corsairs of Captain Blacktide Shen's probing crews (hostile mobs at harbor/dock areas) | Reconnaissance and threat awareness: watches patrol timing, marks dock patterns, references Blacktide Shen's three-year surveillance campaign. Fires out of combat only; returns `FALSE`. | *"Shen knows these channels better than the Syndic charts do. Every gate timing, every patrol gap. Patience is strategy."* |

### Kowloon builder notes

- `spec_kowloon_shopkeeper` is the standard ambient special for all Kowloon service shops. Assign to any merchant NPC with a `#SHOPS` entry.
- `spec_kowloon_gate_captain` doubles for Harbor Syndic inspectors at the docks — the manifest-checking flavor applies equally to both roles.
- `spec_kowloon_corsair` is assigned to hostile mob types (alignment negative, `aggressive` flag set). It fires **only out of combat**, providing ambient menace before engagement. It does not initiate combat; that is handled by the `aggressive` act flag.
- Existing general specials that fit Kowloon mobs: `spec_laborer` for dockhands/porters/pump attendants, `spec_vendor` for street food vendors and caravan callers, `spec_lay_sister` for shrine keepers, `spec_lamplighter` for street sweepers, `spec_warden` for plaza inspectors and customs clerks, `spec_thief` for pickpockets.

---

## Summon specials (runtime-only, do not set in area files)

These are assigned and cleared by the game's summon systems at runtime. Setting
them in area files is invalid builder practice.

- `spec_summon_water`
- `spec_summon_fire`
- `spec_summon_earth`
- `spec_summon_undead`
- `spec_summon_holy`
- `spec_summon_shadow`
- `spec_summon_metal`
- `spec_summon_animate`
- `spec_summon_thought`
