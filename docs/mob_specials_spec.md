# Mob Specials Specification

Source: `src/special.c` (registry, shared helpers), `src/ai/spec_*.c` (individual implementations), `src/special.h` (declarations).

## Overview

Mob specials (`spec_fun`) are C functions that run periodically on a mob — once per game pulse when the mob is randomly selected. Each function returns `TRUE` if it took a significant action, `FALSE` otherwise. The game calls them by name via the `#SPECIALS` section of an area file.

Every special lives in its own file under `src/ai/`. The registry (`spec_lookup`, `rev_spec_lookup`, `print_spec_lookup`) and the shared helper functions used by multiple specials (`dragon`, `spec_summon_cast_random`, `summon_master_heal_chance`, `spec_summon_heal_master`) live in `src/special.c`.

---

## Assignment syntax (`#SPECIALS` section)

```text
#SPECIALS
M <mob_vnum> <spec_fun_name>
...
S
```

`*` comment lines are accepted. The `<mob_vnum>` must be within the area's declared vnum range. Each mob may have at most one special function; assigning two `M` lines for the same vnum is invalid.

---

## Builder policies (summary)

| Rule | Detail |
|---|---|
| `spec_summon_*` — never set in area files | These are assigned and cleared at runtime by the summon systems only. Setting them in an area file is invalid. |
| `spec_keep_*` — human builders only | May only be assigned by a human builder. Do not use in generated area files. |
| Area-specific specials — match the area | Midgaard, Kiess, Kowloon, GNF, and Roc Road specials are written for specific areas. Using them in the wrong area produces anachronistic dialogue. |
| One special per mob | Multiple `M` lines for the same vnum are invalid. |

---

## Breath weapon specials

Fire only while the mob is fighting. Each breath delegates to the `dragon()` helper (selects a target that is fighting this mob and casts the named breath spell) or for gas uses a room-wide cast.

| Special | Source | Effect |
|---|---|---|
| `spec_breath_any` | `src/ai/spec_breath_any.c` | Randomly picks one of the five breath types below using a 3-bit random gate. Distribution: fire (1/8), lightning (2/8), gas (1/8), acid (1/8), frost (3/8). |
| `spec_breath_acid` | `src/ai/spec_breath_acid.c` | Casts `acid breath` on a fighting target. |
| `spec_breath_fire` | `src/ai/spec_breath_fire.c` | Casts `fire breath` on a fighting target. |
| `spec_breath_frost` | `src/ai/spec_breath_frost.c` | Casts `frost breath` on a fighting target. |
| `spec_breath_gas` | `src/ai/spec_breath_gas.c` | Casts `gas breath` room-wide (no single target). |
| `spec_breath_lightning` | `src/ai/spec_breath_lightning.c` | Casts `lightning breath` on a fighting target. |

---

## Combat caster specials

Fire only while the mob is fighting. A target is selected from characters in the room that are fighting this mob. Spell selection is level-gated and randomly chosen each tick.

### `spec_cast_cleric` (`src/ai/spec_cast_cleric.c`)

Announces *"The eyes of $n glow brightly!"* Level-gated spell pool (4-bit random selection; loop retries until level gate passes):

| Case | Minimum level | Spell |
|---|---:|---|
| 0 | 0 | `blindness` |
| 1 | 3 | `cause serious` |
| 2 | 7 | `earthquake` |
| 3 | 9 | `cause critical` |
| 4 | 10 | `dispel evil` |
| 5 | 12 | `curse` |
| 6 | 12 | `change sex` |
| 7 | 13 | `flamestrike` |
| 8–10 | 15 | `harm` |
| default | 16 | `dispel magic` |

### `spec_cast_judge` (`src/ai/spec_cast_judge.c`)

Casts `high explosive` on a fighting target (no level gate). Suited to powerful single-target NPCs.

### `spec_cast_mage` (`src/ai/spec_cast_mage.c`)

Announces *"An eerie sound comes from $n as $e stares at $N!!"* Level-gated spell pool:

| Case | Minimum level | Spell |
|---|---:|---|
| 0 | 0 | `blindness` |
| 1 | 3 | `chill touch` |
| 2 | 7 | `weaken` |
| 3 | 8 | `teleport` |
| 4 | 11 | `color spray` |
| 5 | 12 | `change sex` |
| 6–9 | 15 | `fireball` |
| default | 20 | `acid blast` |

### `spec_cast_undead` (`src/ai/spec_cast_undead.c`)

Announces *"$n summons forth the powers of darkness!"* Level-gated spell pool. If `summon` is selected, the mob teleports a random undead NPC from elsewhere in the world into the room and sets it fighting.

| Case | Minimum level | Spell |
|---|---:|---|
| 0 | 0 | `curse` |
| 1 | 3 | `weaken` |
| 2 | 6 | `chill touch` |
| 3 | 9 | `blindness` |
| 4 | 12 | `poison` |
| 5–6 | 18 | `harm` |
| 7 | 21 | `teleport` |
| default | 24 | `gate` (or `summon` teleport) |

### `spec_cast_cadaver` (`src/ai/spec_cast_cadaver.c`)

Out-of-combat only. On a random 6-in-8 chance, casts a short-duration `detect invis` on a visible character in the room (skips if already affected). Designed for the cadaver NPC.

### `spec_cast_bigtime` (`src/ai/spec_cast_bigtime.c`)

Announces *"$n invokes the dark powers!!"* High-level combat caster pool. The retry loop is bounded at 10 iterations to prevent infinite looping on low-level mobs.

| Case | Minimum level | Spell |
|---|---:|---|
| 0 | 45 | `dispel magic` |
| 1 | 46 | `acid blast` |
| 2 | 30 | `phobia` |
| 3 | 55 | `chain lightning` |
| 4–5 | 43 | `mindflame` |
| 6 | 12 | `lava burst` |
| 7 | 44 | `static` |
| default | 25 | `hellspawn` |

### `spec_cast_adept` (`src/ai/spec_cast_adept.c`)

Out-of-combat only. Each tick:
1. If the room lacks `ROOM_BV_HEAL_REGEN`, casts `healing light` at level 79.
2. If the room lacks `ROOM_BV_MANA_REGEN`, casts `mana flare` at level 79.
3. Picks a random visible character in the room and casts one of: `armor`, `bless`, `cure blindness`, `cure light`, `refresh`. Case 5 also announces the number of days until Christmas.

---

## Law enforcement specials

### `spec_executioner` (`src/ai/spec_executioner.c`)

Does not fire while fighting. Scans the room for:
- Any player with `PLR_KILLER` → crime `"KILLER"`
- Any player with `PLR_THIEF` → crime `"THIEF"`
- Any NPC with `ACT_UNDEAD` → crime `"UNDEAD"`

On detection: yells the offense, calls `multi_hit` on the target, and spawns two city guards (`MOB_VNUM_CITYGUARD`) in the room.

### `spec_policeman` (`src/ai/spec_policeman.c`)

Does not fire while fighting. Scans the room for players with `PLR_KILLER` or `PLR_THIEF`. On detection: yells the crime, then sets a hunt on the criminal (only if not already hunting something). Does not spawn guards.

### `spec_guard` (`src/ai/spec_guard.c`)

Wrapper — identical to `spec_policeman`. The area file name `spec_guard` and `spec_policeman` both resolve to the same function pointer in `spec_lookup`.

### `spec_mino_guard` (`src/ai/spec_mino_guard.c`)

Does not fire while fighting. Checks every player in the room for a minotaur pass (`OBJ_VNUM_MINO_PASS`) worn in either hand. Players with the pass receive *"Let this one pass"*. Players without it trigger *"Intruder Alert! Man the Walls!"* yell and immediate `multi_hit`.

---

## NPC behavior specials

### `spec_fido` (`src/ai/spec_fido.c`)

Fires while awake. Scans the room for NPC corpses (`ITEM_CORPSE_NPC`). When one is found (50% per tick per corpse), extracts all items onto the floor and destroys the corpse. Emits *"$n savagely devours a corpse."*

### `spec_janitor` (`src/ai/spec_janitor.c`)

Fires while awake. Scans room contents for takeable items (`ITEM_TAKE` set) that are drink containers, trash, or cost fewer than 10 gold. Picks up one such item per tick, emitting *"$n picks up some trash."*

### `spec_thief` (`src/ai/spec_thief.c`)

Fires while standing. Picks a random player in the room that can be seen. If the player is awake and the level-check fails (level gate: `number_range(0, level) != 0`), the player notices. Otherwise, steals `gold × number_range(1,20) / 80`; the mob keeps 7/8 of that.

### `spec_poison` (`src/ai/spec_poison.c`)

Fires during combat only. Fires if a random percent roll passes `2 × level`. Bites the target (one extra hit of type `TYPE_HIT+8`), announces the bite, and applies `spell_poison` at the mob's level.

### `spec_rewield` (`src/ai/spec_rewield.c`)

35% chance when not fighting, 60% when fighting. Scans the mob's inventory first, then the room, for weapons with higher `value[2]` than any currently held weapon. Picks up the best candidate from the room if needed, then calls `do_wear`. If the weapon cannot be worn, destroys it.

### `spec_mayor` (`src/ai/spec_mayor.c`)

Complex Midgaard mayor movement script. Activates at hours 6 (open path) and 20 (close path), walking fixed encoded paths that include: wake/sleep, directional moves, speech lines, open/close/lock/unlock the gate, and city proclamations. Falls back to `spec_cast_cleric` if attacked mid-route.

### `spec_sage` (`src/ai/spec_sage.c`)

Story-telling NPC script. Activates at hour 6, walking an encoded path with: wake, greet, rake fire, produce food, get/eat food, nine sequential story segments about the Sylai, gossip, and goodbye before sleeping. Falls back to `spec_cast_cleric` if attacked.

### `spec_stephen` (`src/ai/spec_stephen.c`)

Fires while standing. Each tick: teleports to a random room in the world, emitting arrival/departure messages, then picks a random player in the new room and delivers a Christmas gift. Uses a loop to guarantee a valid room is found.

### `spec_tax_man` (`src/ai/spec_tax_man.c`)

Fires while standing. Teleports to a random room. Targets a random player who has either more than 100,000 gold on hand or more than 1,000,000 in the bank. Deducts 1% of the player's total wealth (bank-first, then on hand). Logs the transaction to `MONITOR_MOB`. Calls `do_save` on the target.

### `spec_sylai_priest` (`src/ai/spec_sylai_priest.c`)

Room-specific: only activates in room 439. Fires during combat. Each tick: 30% chance to attempt, then 60% success rate. On success, shoves the fighting player off the outcrop into room 433 (30%) or 436 (70%). Announces the shove, calls `stop_fighting`, moves the player, and shows them the new room description.

### `spec_wizardofoz` (`src/ai/spec_wizardofoz.c`)

Disabled. Always returns `FALSE`. Previously used for intelligence-based mob behavior; retained as a placeholder.

### `spec_keep_physical_captain` (`src/ai/spec_keep_physical_captain.c`)

**Human-assignment only. Do not use in generated area files.**

Boss-tier combat special. Uses a cycling counter (`ch->spec_behavior`) reset to 0 when not fighting:

| Counter mod 10 | Action |
|---:|---|
| 1 | 50%: `do_morale` / 50%: `do_leadership` |
| 4 | 50%: `do_warcry` / 50%: `do_beserk` |
| 5 | Announces upcoming massive strike to room |
| 7 | Delivers the massive strike: 9,000–11,000 physical+holy damage |

---

## Midgaard ambient specials

**Builder policy: these specials are specific to Midgaard (`area/midgaard.are`) and must not be assigned to mobs in other areas.** Their dialogue references Midgaard-specific institutions (the Violet Compact, the Lantern Reforms, the Lantern Registry, the Temple of the Resounding Heart) and their flavor is grounded in Midgaard's civic lore.

These fire out of combat only. They use `number_bits(3)` as an 8-way gate (~12.5% chance per pulse), then randomly emit either an action (`act()`) or a spoken line (`do_say()`). They always return `FALSE` — they provide atmosphere only, no game-state changes.

| Special | Source | Flavor / setting | Sample lines |
|---|---|---|---|
| `spec_lamplighter` | `src/ai/spec_lamplighter.c` | Lamp maintenance on the Kindling Watch route. Trims wicks, checks oil, refills from shoulder satchel, references the Lantern Registry. | *"No ward unlit, no traveler uncounted — that's the Oath, and that's the work."* |
| `spec_warden` | `src/ai/spec_warden.c` | Violet Compact arcane-checkpoint warden. Checks ward sigils, consults registry ledger, references the Compact's containment doctrine and the threshold to the below. | *"All arcane goods require registry notation before this threshold. The Compact is clear."* |
| `spec_vendor` | `src/ai/spec_vendor.c` | Midgaard crossroads street-market vendor. Calls out goods, discusses Roc Road supply chains, the Guild Concordat, and Midgaard-specific tariff boards. | *"Tariff board raised the western-goods levy again. Prices have to reflect it, nothing personal."* |
| `spec_lay_sister` | `src/ai/spec_lay_sister.c` | Temple of the Resounding Heart memorial keeper. Tends lamps at memorial markers, reads the civic liturgy, recites the Three Oaths. | *"The Temple maintains rolls of the uncounted. Those removed from the ledgers are still remembered here."* |
| `spec_laborer` | `src/ai/spec_laborer.c` | Midgaard day laborer. References the Midgaard-specific day-token system, cinder debt, route variance penalties, and the work-broker system. | *"Levy goes up, day-rate stays flat. Cinder debt, they call it — compounds whether you pay or not."* |

---

## Kiess ambient specials

Introduced for the Kiess area (`area/kiess.are`). **Do not assign to mobs outside Kiess.** All follow the same ambient-only pattern (8-way gate, action or speech, always return `FALSE`).

| Special | Source | Intended mob roles | Flavor / setting | Sample lines |
|---|---|---|---|---|
| `spec_kiess_shopkeeper` | `src/ai/spec_kiess_shopkeeper.c` | Grand Market merchant, Steel & Edge weaponsmith, Bastion armorer, Argent Arcana mage-merchant, Cartographer, Mercenary Board registrar | Shop floor: arranges display, greets arrivals, pitches wares, discusses frontier supply, references the Exchange's quality guarantee. | *"Most of what you need for the frontier is right here. We stock for the road, not the shelf."* |
| `spec_kiess_innkeeper` | `src/ai/spec_kiess_innkeeper.c` | Wayfarer's Inn innkeeper, inn server | Inn atmosphere: checks guest ledger, refills cups, references the memorial plaque, gives travel advice toward the forest. | *"First night in Kiess? The memorial plaque by the door has names — not to alarm you, but to remind you to come back."* |
| `spec_kiess_scout` | `src/ai/spec_kiess_scout.c` | Forest of Confusion scouts, Wall Command intelligence staff | Post-patrol debrief: marks maps, tracks mist-line movement, reports centaur sign and bell-post status, recommends sample collection for healers. | *"The mist line's moved another three hundred paces north since last week. That's fast for this season."* |
| `spec_kiess_orator` | `src/ai/spec_kiess_orator.c` | Prism Square civic orator | Prepared civic speech: always emits both an action (speaker taking the stage) and one of six full speeches about the founding compact, Evermeet's fall, and the city's design. | *"Three factions, one peace code. Not because they agree — they rarely agree — but because the alternative is Evermeet."* |
| `spec_kiess_wall_officer` | `src/ai/spec_kiess_wall_officer.c` | Wall Command duty officer | Command-room behavior: reviews duty roster, traces patrol routes on tactical map, dispatches couriers, speaks on threat assessment doctrine and the Withered Depths blight. | *"Wall Command doesn't guess at threat levels. We assess, we document, we act on evidence. That is the doctrine."* |

---

## Kowloon ambient specials

Introduced for the Kowloon area (`area/kowloon.are`). **Do not assign to mobs outside Kowloon.** Their dialogue references the Neon Covenant, the five founding institutions, Yen-Mak, jade inspection ribbons, CLO, and the delta flood-control systems. All follow the same ambient-only pattern (8-way gate, action or speech, always return `FALSE`) unless noted.

| Special | Source | Intended mob roles | Flavor / setting | Sample lines |
|---|---|---|---|---|
| `spec_kowloon_shopkeeper` | `src/ai/spec_kowloon_shopkeeper.c` | All Kowloon shops: weaponsmith, armorsmith, reagent keeper, provisioner, fletcher, jeweler, pawn broker, chandler | Market floor: hangs goods on hooks (old floating-stall tradition), checks jade ribbons, discusses Ledgerhouse dues, Guild Concordat minimums, tariff-cleared stock. | *"Everything on this counter is Ledgerhouse-cleared. Tariff paid, weights checked, ribbon current."* |
| `spec_kowloon_gate_captain` | `src/ai/spec_kowloon_gate_captain.c` | The four gate captains (Jade, Iron, Lantern, Tide), Harbor Syndic inspector | Gate inspection: reviews manifests against the Syndic ledger, scans approaches, signals guards, checks courier permits, references the seasonal rotation policy and Covenant. | *"Seasonal rotation isn't tradition — it's policy. No one builds loyalty at a chokepoint. The Covenant was specific."* |
| `spec_kowloon_courier` | `src/ai/spec_kowloon_courier.c` | Lantern-runners and CLO messengers | Dispatch and route service: checks seal integrity, consults route cards, polishes brass lantern badge, records deliveries, scans junction bulletin boards. | *"The badge gets you through any gate at any hour. You'd be surprised how useful that is at second bell."* |
| `spec_kowloon_innkeeper` | `src/ai/spec_kowloon_innkeeper.c` | Copper Tide innkeeper | Inn hospitality with frontier character: checks guest ledger, adjusts lamp, sets tea for arrivals, references the city's founding promise to receive battered travelers. | *"If you're recall-safe and breathing, Kowloon will receive you. That's the city's oldest promise. We're just where you start."* |
| `spec_kowloon_corsair` | `src/ai/spec_kowloon_corsair.c` | Hostile river corsairs of Blacktide Shen's crews | Reconnaissance menace: watches patrol timing, marks dock patterns, references Shen's three-year surveillance campaign. Fires **out of combat only**; combat is initiated by the mob's `aggressive` flag, not this special. | *"Shen knows these channels better than the Syndic charts do. Every gate timing, every patrol gap. Patience is strategy."* |
| `spec_kowloon_sweeper` | `src/ai/spec_kowloon_sweeper.c` | Street sweepers | Municipal maintenance with flood-control context: sweeps debris into storm-drain grates, marks sector tallies, explains the drainage importance to flood survival, references monsoon history. | *"The drainage has to stay clear. Flood season, one blocked grate backs up a whole district quadrant."* |
| `spec_kowloon_magistracy` | `src/ai/spec_kowloon_magistracy.c` | Jade Magistracy plaza inspectors, civic scribes, customs clerks | Civic authority: stamps permit filings, checks merchant display ribbons against the audit register, cites the Covenant Schedule, explains the color-coded licensing system and gold-lacquer bonding. | *"The Covenant established civic law above clan retaliation. That's not an opinion, it's the first article."* |
| `spec_kowloon_shrine` | `src/ai/spec_kowloon_shrine.c` | Temple Circle shrine keeper | Shrine maintenance: sweeps offering bowls, relights incense, refills lamp oil, recites the Three Oaths, references Yen-Mak and the Temple's civic-exemption in the Covenant. | *"First Oath: No ward unlit, no traveler uncounted. The Temple observes this at every shrine, whether or not the city does."* |
| `spec_kowloon_vendor` | `src/ai/spec_kowloon_vendor.c` | Street food vendors, caravan callers | Delta street commerce: stirs rice porridge on charcoal brazier, calls out to foot traffic, tracks daily cart route through district junctions, discusses caravan tariffs, Syndic clerks, and dock haulers. | *"I follow the foot traffic. Courier crossing in the morning, provisioner junction at midday, harbor spine before dusk."* |
| `spec_kowloon_laborer` | `src/ai/spec_kowloon_laborer.c` | Harbor dockhands, harbor porters, pump attendants, delta fishermen | Coppersalt-route dock labor: shifts heavy cargo, checks day-token assignments, explains the Syndic's weight-based pay scale, references flood-season hazard rates and pump rotation. | *"Harbor work feeds the city. The cargo comes in, the city eats. Simple as that. They just don't pay us like it matters."* |

### Kowloon builder notes

- `spec_kowloon_shopkeeper` is the standard ambient special for all Kowloon service shops. Assign to any merchant NPC with a `#SHOPS` entry.
- `spec_kowloon_gate_captain` doubles for Harbor Syndic inspectors — the manifest-checking flavor applies equally.
- `spec_kowloon_corsair` is assigned to hostile mob types (negative alignment, `aggressive` flag set). Combat is initiated by the act flag; this special provides out-of-combat atmosphere only.
- `spec_thief` (functional gold-stealing mechanic) is appropriate for Kowloon pickpockets — it is not area-specific flavor.
- Do **not** use Midgaard specials (`spec_lamplighter`, `spec_warden`, `spec_vendor`, `spec_lay_sister`, `spec_laborer`) for Kowloon mobs.

---

## Great Northern Forest ambient specials

Introduced for the Great Northern Forest area (`area/gnf.are`). **Do not assign to mobs outside the GNF.** Dialogue references Ilren Voss's road, the joint commission (dual Midgaard / Kowloon authority), Mire-Speaker passage tithe, the Everkeeper grove law, the Rootbound corruption, and the Ironpine Rise. All follow the ambient-only pattern (8-way gate, action or speech, always return `FALSE`).

| Special | Source | Intended mob roles | Flavor / setting | Sample lines |
|---|---|---|---|---|
| `spec_gnf_road_warden` | `src/ai/spec_gnf_road_warden.c` | Joint-commission wardens patrolling the Lantern Road | Road security: scans treeline, checks waystones, marks field ledger, adjusts joint-commission sash, taps two-tap passage-clear signal. References dual-city authority and ashfang threats. | *"Split-pine badge means joint authority. Midgaard law south, Kowloon compact north, forest charter in between."* |
| `spec_gnf_lamplighter` | `src/ai/spec_gnf_lamplighter.c` | Lantern Road lamp-keepers | Post maintenance: refills waystone lanterns, trims wicks, tests iron brackets for rust, patches cracked lamp-glass, marks maintenance log. References the Midgaard-funded oil and Kowloon-supplied glass compact. | *"The Lantern Road gets its name because the posts stay lit. My job is making sure that's true every night."* |
| `spec_gnf_peddler` | `src/ai/spec_gnf_peddler.c` | Independent road peddlers and itinerant traders | Road commerce: reorganizes pack, holds up northern resin vials, notes trade ledger, shares road gossip about ashfang raids, Mire-clan trade terms, and caravan traffic. | *"Northern resin from the Ironpine Rise is what moves. Wardens need it for sealing dispatches, healers for wounds."* |
| `spec_gnf_mire_speaker` | `src/ai/spec_gnf_mire_speaker.c` | Mire-Speaker clan representatives at fen crossings | Fen navigation and traditional knowledge: reads fish-trap tension, bundles fen herbs, reads water-surface color, watches herons for ford indicators, splices reed cord. References the passage tithe (salt, iron, lamp oil) and the Mire-Speakers' prior claim. | *"We were here before the road. We'll be here after. The Mire-Speakers gave permission for the bridge. That permission can be withdrawn."* |
| `spec_gnf_everkeeper` | `src/ai/spec_gnf_everkeeper.c` | Everkeeper grove wardens | Grove maintenance: plants seedlings in seven-circle patterns, reads bark ring patterns, marks seven-stone circles, monitors Rootbound corruption. References the seven-seedlings law and Ilren Voss's deference to existing groves. | *"Seven seedlings for every ironpine felled. That is the law of the grove. The Everkeepers made it the law of the sky."* |
| `spec_gnf_joint_scout` | `src/ai/spec_gnf_joint_scout.c` | Joint-commission patrol scouts | Dual-authority field work: marks maps in joint-commission cipher, scans treeline, confirms commission writ, counts boot-prints, exchanges hand-signals. Reports to both cities; references forest charter's army-march restriction. | *"Joint commission patrol means dual authority. I write to Midgaard's rangers and Kowloon's wardens both. One incident, two ledgers."* |
| `spec_gnf_toll_collector` | `src/ai/spec_gnf_toll_collector.c` | Fen bridge toll collectors | Passage-tithe administration: reviews tithe ledger, weighs salt measures, marks entries, checks cargo seals against manifest, stamps travel permits. References the relic tithe rate (salt / iron nails / lamp oil) set at road founding. | *"I maintain ledgers going back forty years at this post. The tithe records are the road's memory — everything passes, everything's counted."* |
| `spec_gnf_customs` | `src/ai/spec_gnf_customs.c` | Northern customs officers at the forest boundary | Dual-mark customs: checks manifests for both Midgaard wax and Kowloon thread-knot, examines cargo seals, updates tariff chalkboard, stamps with the northern customs seal. References the fifth article of the forest charter. | *"All northbound manifests require dual marks: Midgaard wax and Kowloon thread-knot. That's the fifth article of the forest charter."* |
| `spec_gnf_courier` | `src/ai/spec_gnf_courier.c` | CLO relay runners on the northern road half | Dispatch relay: checks pouch seal, marks waystation signatures, scans road, polishes bronze CLO badge, adjusts pack balance. References the eight-waystation relay and the fourteen-hour urgent-dispatch record. | *"CLO relay runs from the Granite Arcade to the Iron Gate. Eight waystation stops. I run the northern half."* |

### GNF builder notes

- `spec_gnf_road_warden` and `spec_gnf_joint_scout` are distinct roles: road wardens are permanent assigned staff; joint scouts are paired patrol units on specific assignments. Use them on separate mob types.
- `spec_gnf_mire_speaker` is appropriate for Mire-Speaker clan NPCs at fen-crossing camps. They are neutral (not hostile) by default.
- `spec_gnf_everkeeper` mobs should be placed only inside or adjacent to ironpine groves, never on the road itself.
- `spec_gnf_toll_collector` and `spec_gnf_customs` are administratively distinct: toll collectors administer the Mire-Speaker passage tithe; customs officers enforce the forest-charter dual-mark requirement on cargo. Do not confuse them.

---

## Roc Road ambient specials

Introduced for the Roc Road area (`area/rocroad.are`). **Do not assign to mobs outside Roc Road.** Dialogue references the Roc Road charter, the Ledger Watch, the Three Civic Oaths, the compass-and-wall Trade Syndic mark, Banner Hills, and Midgaard's Magistrate Ledgerhouse. All follow the same ambient-only pattern (8-way gate, action or speech, always return `FALSE`).

| Special | Source | Intended mob roles | Flavor / setting | Sample lines |
|---|---|---|---|---|
| `spec_rr_road_clerk` | `src/ai/spec_rr_road_clerk.c` | Ledger Watch census clerk | Traffic counting and record-keeping at relay stations: makes tally marks, stamps travel records, checks dispatch boxes, rewrites wax-tablet errors. References road-funding arithmetic and Ledgerhouse monthly summaries. Uses 6-entry act pool. | *"A blank entry on the census roll is a gap in the civic record. I don't leave gaps."* |
| `spec_rr_warden_captain` | `src/ai/spec_rr_warden_captain.c` | Road warden post captain | Command presence: studies patrol board, paces perimeter for sight-lines, files dispatch reports, reviews wanted notices, briefs junior wardens. References the Three Oaths recited at every shift change. Uses 6-entry act pool. | *"The warden who doesn't know every face on their stretch doesn't know their stretch. Walk it until you do."* |
| `spec_rr_charter_keeper` | `src/ai/spec_rr_charter_keeper.c` | Charter marker maintainer | Slab maintenance: brushes lichen, repaints Midgaard-blue inscriptions, traces oldest text layers, notes condition in maintenance register. References three eras of script and the legal defensibility of legible inscriptions. | *"The inscriptions are the road's legal memory. As long as they're legible, the charter is defensible in arbitration."* |
| `spec_rr_convoy_marshal` | `src/ai/spec_rr_convoy_marshal.c` | Convoy marshal at staging post | Convoy operations: blows signal horn, checks manifests against route schedule, calls spacing instructions, marks departure times, references Emergency Compact 12 authorizing marshal override of warden orders. Uses 6-entry act pool. | *"I have wagons staged and escorts short. Until the escorts arrive, nothing moves. That is not negotiable."* |
| `spec_rr_peddler` | `src/ai/spec_rr_peddler.c` | Road peddlers on the Roc Road route | Road commerce: reorganizes cart, checks vial contents, adjusts prices in ledger, steps aside for convoys, counts coins. References the three-day Midgaard run, waterproofing wax, and wolf territory on the Banner Hills. | *"Best thing I carry? Waterproofing wax and spare lamp wicks. Every patrol warden needs them. Regular customers."* |
| `spec_rr_shrine_keeper` | `src/ai/spec_rr_shrine_keeper.c` | Waystone shrine keeper | Shrine maintenance: fills lamp hollow, traces blessing text, places wildflowers, sweeps leaves, checks carved lettering. References the pre-road blessing and the keeper's independence from both cities. | *"I serve the road, not either city. Midgaard wardens leave coin. Kiess marshals leave rations. I thank both and serve neither."* |
| `spec_rr_ferryman` | `src/ai/spec_rr_ferryman.c` | River ferryman at road crossing | Crossing operations: tests mooring rope, checks punt bottom for splits, reads river current, straightens fare board, checks balance. References upstream run-time addition, fare pricing in dual currency, and undercut bank danger. | *"I've crossed this river thousands of times. The ford stones haven't shifted in forty years. The crossing is sound."* |
| `spec_rr_camp_cook` | `src/ai/spec_rr_camp_cook.c` | Road camp cook for warden posts and convoy crews | Camp cooking: stirs stew with ladle, sets clay bowls, fans cookfire, tastes and adjusts salt, breaks root vegetables into pot. References payment in dual currencies and the low-banked night fire. | *"The fire's banked low at night. No need to announce yourself to every ridge wolf on the Banner Hills."* |
| `spec_rr_pilgrim` | `src/ai/spec_rr_pilgrim.c` | Travelers and road pilgrims | Walking traveler presence: sets down pack with relief, takes measured water sip, reties sandal knot, reads a worn slip of paper, pauses to look west. Reflects on road conditions, warden reliability, and road grace. | *"I'm not lost. I know exactly where I'm going. I'm just not certain I'll make it."* |
| `spec_rr_ruin_scavenger` | `src/ai/spec_rr_ruin_scavenger.c` | Archaeological ruin scavengers near the road | Ruin investigation: taps stone with hammer, makes charcoal rubbings, sifts rubble for worked edges, compares rubbings to field notebook, sweeps flagstones. References the Granite Arcade authentication process and the three road layers. | *"This road bed has three layers underneath. The deepest layer predates everything with a name. I've found script nobody can read."* |

### Roc Road builder notes

- `spec_rr_road_clerk` is for fixed census-post NPCs, not wandering ones. Pair with `act sentinel`.
- `spec_rr_convoy_marshal` implies a convoy staging post. The mob should be placed at a defined staging area, not a generic roadside room.
- `spec_rr_shrine_keeper` is distinct from `spec_rr_charter_keeper`: shrine keepers tend the waystone blessing sites; charter keepers maintain the carved legal inscriptions. Each has a specific physical location type.
- `spec_rr_pilgrim` is suitable for generic traveler mobs that add road-life ambiance without civic institutional voice.

---

## Summon specials (runtime-only — do not set in area files)

**Builder policy: `spec_summon_*` must never be set in `#SPECIALS` in any area file.** These are assigned to player-summoned familiars at runtime by the summon system and cleared when the summon is dismissed or dies.

Each summon special implements two behaviors per tick (first one that fires wins):

1. **Heal master**: if the master is in the same room and below max HP, there is a level-scaled chance to cast `heal` on them. The chance is `thematic_bonus + (100 − master_hp_pct)`, clamped to [5, 95].
2. **Combat cast**: if the summon is fighting, selects a random spell from the type's spell pool and casts it on the enemy.

The `is_player_summon_special()` function in `src/special.c` returns TRUE for any of these nine spec pointers; this is used by the summon system to identify player-owned familiars.

| Special | Source | Thematic heal bonus | Combat spell pool |
|---|---|---:|---|
| `spec_summon_water` | `src/ai/spec_summon_water.c` | 30 | `chill touch`, `acid blast`, `weaken` |
| `spec_summon_fire` | `src/ai/spec_summon_fire.c` | 12 | `fireball`, `high explosive`, `curse` |
| `spec_summon_earth` | `src/ai/spec_summon_earth.c` | 20 | `earthquake`, `acid blast`, `weaken` |
| `spec_summon_undead` | `src/ai/spec_summon_undead.c` | 8 | `harm`, `chill touch`, `poison` |
| `spec_summon_holy` | `src/ai/spec_summon_holy.c` | 40 | `holy wrath`, `dispel evil`, `curse` |
| `spec_summon_shadow` | `src/ai/spec_summon_shadow.c` | 10 | `energy drain`, `curse`, `weaken` |
| `spec_summon_metal` | `src/ai/spec_summon_metal.c` | 15 | `acid blast`, `lightning bolt`, `high explosive` |
| `spec_summon_animate` | `src/ai/spec_summon_animate.c` | 8 | `harm`, `poison`, `weaken` |
| `spec_summon_thought` | `src/ai/spec_summon_thought.c` | 12 | `thought vise`, `mind bolt`, `feeble mind` |

**Heal bonus interpretation**: `spec_summon_holy` (bonus 40) will attempt to heal its master at near-full HP; `spec_summon_undead` (bonus 8) will only reliably attempt when the master is badly hurt. At 50% HP the effective chances are roughly: holy 90%, water 80%, earth 70%, metal 65%, fire/thought 62%, shadow 60%, undead/animate 58%.

---

## Cinderteeth Mountains boss specials

Eight boss-specific specials written for the Cinderteeth Mountains zone (vnums 6124-6753). Each uses `ch->spec_behavior` as a round counter reset to 0 when the mob stops fighting, and several also use `ch->spec_behavior2` for persistent state (frenzy flags, element tracking).

### `spec_cinderteeth_warlord` — Ashfang Remnant Warlord (6130, level 148)

**File:** `src/ai/spec_cinderteeth_warlord.c`
**Pattern:** Cycling `spec_behavior % 8`; no damage callback.

| Round | Action |
|---|---|
| 1 | Warcry flavor (`do_warcry`) + blood-oath emote |
| 3 | 3000–5000 PHYSICAL + `high explosive` spell on target |
| 5 | Extra physical hit (`one_hit`) + `spell_poison` |
| 7 | Two rapid physical hits |

Mechanically a melee-plus-poison bandit boss. The warcry and occasional explosive give it tactical variety without relying on magic.

**Sample output (round 3):**
```
The ashfang remnant warlord seals the blood-oath and detonates a war-charge
directly at you — concussive force and explosive compound at once!
```

---

### `spec_cinderteeth_warden_cmd` — Ironpost Warden-Commander (6151, level 152)

**File:** `src/ai/spec_cinderteeth_warden_cmd.c`
**Pattern:** Random 35% tick activation; HP-threshold emergency (once at <40% HP).
**`spec_behavior2`:** 0 = normal, 1 = emergency triggered.

| Trigger | Action |
|---|---|
| HP < 40% (once) | Emergency Compact emote + `high explosive` spell on target |
| 35% chance per tick | Random from: `dispel magic`, `chain lightning`, `flamestrike`, or `harm` |

An undead commander with an unpredictable caster pool. The emergency burst makes the last phase dangerous even if the first half was easy.

**Sample output (emergency):**
```
The ironpost warden-commander invokes the Emergency Compact of the Ironpost —
calling the full force of the archive's wards against you in concentrated fire!
```

---

### `spec_cinderteeth_sulfur_colossus` — Glasswash Sulfur Fan Colossus (6172, level 156)

**File:** `src/ai/spec_cinderteeth_sulfur_colossus.c`
**Pattern:** Cycling `spec_behavior % 6`; no damage callback.

| Round | Action |
|---|---|
| 1 | 5000–8000 EARTH + `earthquake` spell |
| 3 | Room-wide `gas breath` (all players) |
| 5 | 5500–8500 FIRE+POISON primary on target; 2000–3500 POISON AOE on all other players |

Alternates between ground control (earth/quake), atmospheric denial (gas), and a split-target fire-and-poison combination. Punishes grouped players who cluster.

**Sample output (round 5):**
```
The glasswash sulfur fan colossus drives both fists into the caldera floor and
erupts — volcanic fire and concentrated sulfur blast outward!  The target takes
the full column while a spray of toxic vapor catches everyone else in the room!
```

---

### `spec_cinderteeth_anchor` — Bellspine Resonance Anchor Construct (6194, level 160)

**File:** `src/ai/spec_cinderteeth_anchor.c`
**Pattern:** Cycling `spec_behavior % 10`; **has damage callback** (`spec_cinderteeth_anchor_damage`).
**`spec_behavior2`:** Bitmask of element flags received since round 1; cleared at round 1.

| Round | Action |
|---|---|
| 1 | Clear `spec_behavior2`; resonance charge begins (flavor) |
| 2 | Warn players: **strike with EARTH or PHYSICAL to ground the conduit** |
| 3 | If `spec_behavior2` has `ELEMENT_EARTH` or `ELEMENT_PHYSICAL`: fizzle; else 4000–7000 MENTAL+AIR room AOE |
| 5 | 7000–10000 MENTAL+AIR single-target resonance lance |
| 7 | 8000–11000 EARTH single-target structural discharge |
| 9 | 6000–9000 MENTAL room AOE (anchor overload) |

The interrupt mechanic: the damage callback fires on every hit against the Anchor. During the charge window (rounds 1–2), any EARTH or PHYSICAL hit sets the corresponding flag in `spec_behavior2`. The round 3 check reads those flags.

**Sample output (round 2 → 3 interrupted):**
```
The bellspine resonance anchor construct's resonance charge intensifies — the hum becomes painful!
Strike with EARTH or PHYSICAL force to ground the conduit!
...
Your earthen/physical blow disrupts the construct's crystalline conduit!
The resonance charge scatters harmlessly — the construct resets!
```

---

### `spec_cinderteeth_ventspeaker` — Ghost of the Last Ventspeaker (6212, level 163)

**File:** `src/ai/spec_cinderteeth_ventspeaker.c`
**Pattern:** Cycling `spec_behavior % 9`; no damage callback.

| Round | Action |
|---|---|
| 1 | `weaken` + `curse` spells on target |
| 3 | 7000–10000 MENTAL+SHADOW single-target (name-stone binding) |
| 5 | 3000–5500 PHYSICAL+AIR room AOE on all players (Ash Monsoon memory) |
| 7 | `earthquake` spell + 6000–9000 FIRE single-target (volcanic sympathy) |
| 8 | 7500–10500 MENTAL single-target (hollow prophecy) |

A spiritual boss mixing debuffs, esoteric damage types, and a room-wide physical AOE. The consecutive rounds 7 and 8 create a brutal double-hit window.

**Sample output (round 5):**
```
Cairn-Mother Retha An exhales the memory of the Ashfall Monsoon — three years
of volcanic winter press down on EVERYONE in the chamber!
```

---

### `spec_cinderteeth_oracle` — Grand Vent-Oracle of the Ash Cult (6221, level 166)

**File:** `src/ai/spec_cinderteeth_oracle.c`
**Pattern:** Cycling `spec_behavior % 10`; no damage callback.

| Round | Action |
|---|---|
| 1 | `lava burst` spell on target (vent interpretation) |
| 3 | Room-wide `gas breath` (Speaking Vent communion) |
| 5 | 9000–12000 FIRE+EARTH single-target (theological verdict) |
| 7 | `dispel magic` + `flamestrike` on target (doctrinal dispensation) |
| 9 | 10000–13000 FIRE single-target (mountain's ruling) |

A theological fire caster with the ability to strip buffs before landing a flamestrike. Culminates in a concentrated direct-damage fire column.

**Sample output (round 7):**
```
The grand vent-oracle strips you of false protections before the speaking vent —
then delivers doctrinal dispensation in fire!
```

---

### `spec_cinderteeth_caldera_watcher` — Secondtooth Caldera Watcher (6238, level 168)

**File:** `src/ai/spec_cinderteeth_caldera_watcher.c`
**Pattern:** Cycling `spec_behavior % 8` + HP-threshold frenzy.
**`spec_behavior2`:** 0 = normal, 1 = frenzy active.

| Round | Normal | Frenzy |
|---|---|---|
| 0 | Territorial circuit (flavor) | 7000–10000 PHYSICAL bonus strike |
| 2 | 10000–13000 PHYSICAL caldera charge | 11000–15000 PHYSICAL (boosted) |
| 4 | 9000–12000 FIRE heat-blast | same |
| 6 | 9000–12000 EARTH + `earthquake` | same |

Frenzy triggers once when HP drops below 25%, announced to the room. The `case 0` slot converts from flavor to an attack; the charge (case 2) gains a damage bonus.

**Sample output (frenzy trigger):**
```
The secondtooth caldera watcher's territorial fury erupts — the caldera watcher enters
a predator's last frenzy, moving with terrifying speed!
```

---

### `spec_cinderteeth_patriarch` — The Ember Throne Patriarch (6242, level 170)

**File:** `src/ai/spec_cinderteeth_patriarch.c`
**Pattern:** Cycling `spec_behavior % 12`; no damage callback. Most complex special in the zone.

| Round | Action |
|---|---|
| 1 | `do_dispel_all`: `dispel magic` on every player in room |
| 2 | Magma surge building (flavor) |
| 3 | Magma surge building, targeting indicator (flavor) |
| 4 | 12000–16000 FIRE+EARTH single-target magma eruption |
| 5 | Tectonic pressure flavor + `earthquake` spell |
| 6 | `do_tectonic_release`: 6000–9000 EARTH room AOE on all players |
| 7 | Room-wide `gas breath` (volcanic gas venting) |
| 8 | 10000–14000 MENTAL single-target (mountain's accumulated centuries of will) |
| 9 | Geological verdict building (flavor) |
| 10 | Geological verdict building, all-elements warning (flavor) |
| 11 | Final verdict warning (flavor) |
| 0 | `do_ember_verdict`: 8000–12000 FIRE+EARTH+AIR room AOE on all players |

The Ember Verdict (round 0) is the culmination: a three-element AOE preceded by a three-round warning sequence. The dispel strip at round 1 ensures each cycle begins with players exposed.

**Sample output (round 0 — EMBER VERDICT):**
```
The ember throne patriarch pronounces the EMBER VERDICT — fire, stone, and volcanic wind
combine in a cataclysmic eruption that engulfs the entire chamber!
The mountain has rendered its judgment on ALL present!
```

---

## Adding a new special function

1. Create `src/ai/spec_yourname.c` with:
   ```c
   #include "globals.h"
   #ifndef DEC_MAGIC_H
   #include "magic.h"
   #endif
   #include "special.h"

   bool spec_yourname(CHAR_DATA *ch)
   {
       /* implementation */
       return FALSE;
   }
   ```
2. Add `DECLARE_SPEC_FUN(spec_yourname);` to `src/special.h`.
3. Add entries for the new name to `spec_lookup()`, `rev_spec_lookup()`, and `print_spec_lookup()` in `src/special.c`.
4. Add `ai/spec_yourname.o \` to the `AI_SPEC_OBJS` block in `src/Makefile`.
5. Document the new special in this file under the appropriate group.

If the new special is ambient-only (returns `FALSE`, uses the 8-way gate pattern), it should follow the established template: `!IS_AWAKE(ch) || is_fighting(ch)` guard first, then `number_bits(3) != 0` gate, then action-or-say split.
