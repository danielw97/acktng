# Area Plan: Scorching Sands (Full Rebuild)

## Overview

- **Area file:** `scorchsands.are`
- **Area title:** `@@yThe @@eScorching @@bSands@@N`
- **Keyword:** `scorching_sands`
- **Level band:** `I 65 85`
- **Fixed vnum envelope (unchanged):** `V 29900 29974`
- **Room utilization mandate:** all **75 room vnums** are used (`29900-29974`).
- **Primary design goal:** rebuild Scorching Sands as the legal-ecological hinge between the Eastern Desert corridor, oasis convoy law, and Saltglass Reach conversion routes, with lore-consistent ties to pyramid certification doctrine.

This plan draws directly from:
- `docs/lore/scorching_sands_lore.md` (core identity: Ember Dunes, Kiln Basins, Cinder Channels, Ashglass Rims, Burn Ledger history, Cinder Gate).
- `docs/lore/oasis_pyramid_lore.md` (water-law institutions, oasis administrative continuity, pyramid logistics dependency).
- `docs/lore/eastern_desert_lore.md` (charter-road collapse and ritualized administration).
- `docs/lore/great_oasis_lore.md`, `docs/lore/southern_oasis_lore.md`, `docs/lore/northern_oasis_lore.md` (ration, verification, convoy hardening).
- `docs/lore/great_pyramid_lore.md`, `docs/lore/northern_pyramid_lore.md`, `docs/lore/southern_pyramid_lore.md` (seal culture and funerary-industrial continuity).
- `docs/lore/saltglass_reach_lore.md` and `docs/lore/mafdet_lore.md` (inland-to-littoral legal transition).

---

## Spec Compliance Checklist (`docs/area_file_spec.md`, `docs/room_spec.md`, `docs/mob_spec.md`, `docs/object_spec.md`, `docs/quests_spec.md`)

- `#AREA` includes required `Q 16`.
- `O Virant~` owner policy is used.
- All authored text strings are `~` terminated during implementation and avoid double blank lines.
- No in-world string references numeric vnums.
- Mobile `long_descr` is single-line format only; mobile `description` ends with exactly one trailing newline before `~`.
- Room descriptions are unique, minimum sentence counts obeyed, and named exits are anchored in description/object/extra-description text.
- Exits are bi-directional; no directional cycles are introduced (this area is not a maze).
- Door semantics follow spec: `EX_ISDOOR` on exit; lock state set in `#RESETS` with `D` commands; locked doors have valid key objects.
- Mob and object vnums remain inside `29900-29974` envelope.
- Quest definitions use valid quest type semantics from `docs/quests_spec.md`.

---

## Color Theme

- `@@y` sun-scoured gold (charter law, old authority)
- `@@e` heat/scorch stress (active environmental danger)
- `@@b` kiln-ember glow and fused mineral flare
- `@@c` residual water-law memory and survey calibration marks
- `@@W` legal inscription clarity and tablet text
- `@@N` reset

Policy: no `@@k`, no flashing code, no background color codes.

---

## Macro Layout (Non-Grid, Multi-Link Traverse)

Scorching Sands is deliberately **braided**, not square: three parallel travel spines cross-connect at legal chokepoints while preserving dead-end risk pockets and boss-gated subroutes.

### External links (worldgraph intent)

- **West/Northwest link:** Eastern Desert late caravan spine (existing overland continuity).
- **Southwest links:** Great Oasis and Southern Oasis convoy feeders.
- **East link:** Saltglass Reach Heavy Road approach.
- **Northeast legal relay:** Mafdet-facing pre-screening function via Cinder Gate traffic.
- **Lore tie (not direct room exit):** pyramid seal-culture references through kiln certification ruins.

### Traversal structure

1. **Route Spine A (Caravan Law Track):** safest path, highest faction contact.
2. **Route Spine B (Kiln Basin Cut):** medium risk, lore-rich, certification ruins.
3. **Route Spine C (Ashglass Rim Line):** highest attrition, fastest eastbound run.
4. **Cross-Bridges:** 8 intentionally placed connectors prevent pure linearity.
5. **Chokepoint:** Cinder Gate functions as route merger and social/legal pressure node.

---

## Room Plan (All 75 Rooms)

## District A — Ember Dunes Intake (29900-29914, 15 rooms)
Primary western intake from Eastern Desert and oasis feeder lanes.

| Vnum | Room Name | Purpose |
|---|---|---|
|29900|The Charter Heatline|Primary western entry from Eastern Desert spine.|
|29901|Surveyor's Wind Post|Measure-era wayfinding remains.|
|29902|Split of the Three Convoys|Major branch node to all three internal spines.|
|29903|Great Oasis Ration Spur|Southwest feeder route tie.|
|29904|Southern Hardening Track|Southern Oasis convoy doctrine tie.|
|29905|Witness-Stick Clearing|Northern Oasis verification custom callback.|
|29906|Leeward Dune Shelf|Lower-risk rest pocket.|
|29907|False-Cool Mirage Pan|Hazard room; path misread risk.|
|29908|Ash Scorpion Scrape|Fauna pressure lane.|
|29909|Burned Cargo Scatter|Caravan collapse evidence room.|
|29910|Cracked Shade Arch I|First surviving Seven Shade Arch marker.|
|29911|Cinder Channel Mouth|Entry to central channel routes.|
|29912|Drift Ledger Cairn|Legal marker pile with altered inscriptions.|
|29913|Dust Oath Circle|Faction social encounter point.|
|29914|Gateward Ascent|Transition room to Cinder Gate district.|

## District B — Kiln Basins and Burn Ledger Quarter (29915-29929, 15 rooms)
Industrial-legal core built around Bassek-era kiln compounds.

| Vnum | Room Name | Purpose |
|---|---|---|
|29915|Bassek Compound Forecourt|District hub; kiln quarter entry.|
|29916|Seal Clerk Arcade|Administrative ruins and record hooks.|
|29917|Furnace Hands' Barrack Ruin|Labor history room.|
|29918|Seven-Color Assay Hall|Heat-certification lore node.|
|29919|Sun-Fired Tag Kiln|Quest-object source room.|
|29920|Collapsed Firing Tunnel|Gas vent hazard lane.|
|29921|Breath-Burn Basin|Environmental hazard room.|
|29922|Dual-Marker Calibration Slab|Keeper survey method callback.|
|29923|Burn Ledger Annex|Primary lore archive chamber.|
|29924|Cauterist Graffito Wall|Synod over-write evidence.|
|29925|Kiln Master's Seal Vault|Locked room (keyed).|
|29926|Charred Amphora Yard|Object farming/reset cluster.|
|29927|Ration Dispute Court|Quest-offerer room (legal NPC).|
|29928|Ashfall Escarpment Stairs|Vertical connector to rim routes.|
|29929|Warden Relay Niche|Bridge toward Cinder Gate authority node.|

## District C — Cinder Channels and Ambush Lanes (29930-29944, 15 rooms)
Protected-from-wind paths with high predator/raider activity.

| Vnum | Room Name | Purpose |
|---|---|---|
|29930|North Channel Descent|Entry from intake and kiln districts.|
|29931|Split Channel Run|Major internal branch.|
|29932|Smuggler's Sidecut|Optional high-risk shortcut.|
|29933|Overhung Heat Shelf|Ambush overlook room.|
|29934|Collapsed Culvert Mouth|Bottleneck combat room.|
|29935|Cloudburst Scour Mark|Rare-flood evidence room.|
|29936|Rope-Bridge Slot|Non-grid cross-link to rim line.|
|29937|Cinder Kite Roost|Fauna route pressure.|
|29938|Broker Whisper Post|Faction intelligence node.|
|29939|Hijacked Toll Station|Hostile administrative holdpoint.|
|29940|Witness Pyre Turn|Moral/lore scene room.|
|29941|Channel Tri-Fork|Second major branch node.|
|29942|Basalt Marker Blind|Navigation challenge room.|
|29943|Broken Water Cart Sink|Escort/defense narrative space.|
|29944|Gate Drain Approach|Feeds directly into Cinder Gate district.|

## District D — Ashglass Rim Transition (29945-29959, 15 rooms)
Geological transition toward Saltglass Reach mirror-flat ecology.

| Vnum | Room Name | Purpose |
|---|---|---|
|29945|First Fused Crust|Initial ashglass visual shift.|
|29946|Slag Sheet Traverse|Brittle terrain hazard.|
|29947|Glass-Sing Dune Crest|Wind resonance room.|
|29948|Rim Survey Needle|Cartography anchor room.|
|29949|Heatquake Scar Fold|Verticality and route split.|
|29950|Shoreward Table Fragment|Legal conversion precursor lore.|
|29951|Vitrified Caravan Ruts|Historic route fossilization room.|
|29952|Mafdet Pre-Stamp Cairn|Manifest culture bridge room.|
|29953|Warden Ember Watch|Neutral checkpoint reset room.|
|29954|Rim Bandit Drift|Hostile pressure node.|
|29955|Saltglass Foretaste Flats|Environmental transition room.|
|29956|Cairnline Echo Post|Directional recall landmark.|
|29957|Fused Pier Shadow|Cinder Gate foreshadow silhouette.|
|29958|Jurisdiction Windbreak|Pre-gate queue room.|
|29959|Outer Cinder Queue|Converges to final district.|

## District E — Cinder Gate and Ember Crucible (29960-29974, 15 rooms)
Jurisdictional choke + endgame confrontation around heat technology.

| Vnum | Room Name | Purpose |
|---|---|---|
|29960|The Cinder Gate West Pier|Primary merge from inland routes.|
|29961|The Cinder Gate East Pier|Primary merge toward Reach approach.|
|29962|Manifest Shade Court|NPC/legal interaction hub.|
|29963|Token Ledger Stand|Transit-ration dispute room.|
|29964|Broker Rope Canopy|Information market room.|
|29965|Column of Striated Stone|Landmark inspection room.|
|29966|Gate Undershadow Passage|Alternative flanking route.|
|29967|Crucible Access Ramp|Boss wing entry.|
|29968|Ember Crucible Intake|Pre-boss hazard room.|
|29969|Thermal Valve Gallery|Mechanism/lore room with keyed door.|
|29970|Seal Re-Firing Chamber|High-danger elite room.|
|29971|The Crucible Heart|Boss room (`no_mob`, indoors, hot).|
|29972|Post-Burn Registry|Resolution room with quest turn-in fallback.|
|29973|Eastbound Heavy Road Join|Exit toward Saltglass Reach.|
|29974|Mafdet Relay Spur|Northeast outbound legal relay exit.|

---

## Connectivity Blueprint (Creative, Non-Square)

- **Hub 1:** 29902 (Three Convoys Split)
- **Hub 2:** 29915 (Bassek Forecourt)
- **Hub 3:** 29941 (Channel Tri-Fork)
- **Hub 4:** 29960/29961 (Cinder Gate dual-pier merge)

Cross-bridges (examples):
- 29910 <-> 29922 (arch marker to calibration slab)
- 29911 <-> 29930 (intake to channel descent)
- 29928 <-> 29946 (kiln escalator to rim traverse)
- 29936 <-> 29948 (rope bridge to survey needle)
- 29944 <-> 29960 (channel drain to west pier)
- 29957 <-> 29965 (rim foreshadow to gate landmark)
- 29959 <-> 29962 (queue to shade court)
- 29971 <-> 29972 (boss to resolution)

Design rule: branch-and-backtrack with converging spines; no circular directional loop chains.

---

## Mob Plan (within `29900-29974`, sequential and contiguous)

**Mob block:** `29900-29924` (25 templates)

### Factions/Ecology

1. **Charter Remnant Staff** (neutral/defensive)
   - seal clerk remnant, ration adjudicator, gate scrivener, witness elder.
2. **Cinder Brokers / Raider affiliates** (hostile opportunists)
   - cinder broker enforcer, toll hijacker, channel cutthroat.
3. **Cauterist Synod cell** (hostile ritual-administrative)
   - cauterist acolyte, brand cantor, re-firing deacon.
4. **Heat-adapted fauna**
   - ash scorpion, cinder kite, dune stalk cat, kiln adder.
5. **Undead/construct legacy**
   - fired-clay sentinel, ledger mummy, crucible warden.

### Key named mobs

- `29920` **Orulan Bassek's Bound Assessor** (mini-boss, kiln quarter)
- `29923` **Warden of the Striated Piers** (gate enforcer mini-boss)
- `29924` **The Ember Crucible Regent** (final boss, room 29971)

Policy notes:
- All mobs include `is_npc` and `stay_area`.
- Hostiles include `aggressive` where appropriate.
- Boss templates include `boss` + `sentinel`; placed only in `no_mob` boss room via resets policy.
- Strong elites marked `solo`.

---

## Object Plan (within `29900-29974`, sequential after mobs)

**Object block:** `29925-29949` (25 templates)

### Equipment themes

- Heat-resistant convoy gear (sand veils, kiln-hide wraps, ashglass gaiters).
- Legal-inscription weapons (charter-edged khopesh, witness-scribed spear).
- Hybrid inland-littoral transitional gear (seal-clasped harness, conversion-stamped buckler).

### Key objects

- `29925` burnt witness-stick bundle (quest/lore item)
- `29926` seven-color assay shard set (quest item)
- `29927` falsified scorch-clause tablet (quest item)
- `29928` cinder gate transit token (quest/legal item)
- `29929` Bassek vault key (unlocks room 29925 equivalent keyed chamber)
- `29930` crucible valve key (unlocks 29969 access door)
- `29931` registrar's heatproof satchel (rare utility container)
- `29932` ashglass survey slate (cartography quest turn-in item)
- `29933-29948` mixed armor/weapon loot tables
- `29949` ember-regent sigil core (`ITEM_BOSS` + `ITEM_LOOT`)

Object spec notes:
- All loot-table entries use `ITEM_LOOT`.
- Boss drop(s) include `ITEM_BOSS`.
- Key objects have valid take/wear flags and exist before keyed door resets.

---

## Quest Plan (single cartography quest included)

Quest IDs/file numbers are placeholders to allocate against global quest index during implementation.

### Quest design constraints

- Exactly **one cartography quest** exists in this area plan.
- Added side content focuses on **non-chain standalone quests** so players can engage Scorching Sands without mandatory linear gating.
- Only two quests are chained into the boss route (`143 -> 144`); all others are explicitly non-chain (`prereq -1`).

| File | Static ID | Title | Prereq | Type | Description |
|---|---:|---|---:|---:|---|
| `scorching_01.prop` | 140 | **Scorching Sands cartography survey: Three Spines to Cinder Gate** | -1 | 1 (kill-variety) | **Only cartography quest in this plan.** Offerer: Gate cartographer at 29962. Survey completion requires clearing one threat at each spine landmark: ash scorpion alpha (District A), toll hijacker captain (District C), rim bandit harrier (District D), and filing at Cinder Gate. Reward: 7000 gold, 6 qp. Level 65-80. |
| `scorching_02.prop` | 141 | Burn Ledger recovery writ | -1 | 2 (collect-items) | Recover Burn Ledger fragments from kiln quarter and return to ration adjudicator. Collect 3 ledger fragments (from District B). Reward: 6200 gold, 5 qp. Level 66-82. |
| `scorching_03.prop` | 142 | Seal fraud interdiction | -1 | 3 (kill-count) | Remove fraudulent seal brokers operating in channels. Kill 10 cinder broker operatives. Reward: 6400 gold, 5 qp. Level 67-83. |
| `scorching_04.prop` | 143 | Cauterist re-firing disruption | 141 | 1 (kill-variety) | Kill one each of cauterist acolyte, brand cantor, and re-firing deacon in Districts B/E. Reward: 7200 gold, 6 qp. Level 70-84. |
| `scorching_05.prop` | 144 | The Crucible Regent dismantled | 143 | 3 (kill-count) | Final sanction writ: kill 1 Ember Crucible Regent in room 29971. Reward: 9800 gold, 8 qp. Level 75-85. |
| `scorching_06.prop` | 145 | Channel casualty triage drill | -1 | 2 (collect-items) | Southern Oasis hardening crews request practical support: recover 4 intact emergency vent wraps from wrecked convoy packs in Districts A/C. Offerer: convoy triage marshal at 29904. Reward: 6100 gold, 5 qp. Level 66-81. |
| `scorching_07.prop` | 146 | Witness-stick cohort verification | -1 | 1 (kill-variety) | A Northern-style witness elder needs route integrity proof after forged crossing marks were reported. Eliminate one each of forged witness thief, channel falsifier, and broker intimidator. Offerer: witness elder at 29905. Reward: 6500 gold, 5 qp. Level 67-82. |
| `scorching_08.prop` | 147 | Mafdet pre-screen manifest seizure | -1 | 2 (collect-items) | Mafdet relay factors suspect illicit relic entries through the Cinder Gate queue. Seize 3 fraudulent pre-stamp manifests from gate runners in Districts D/E. Offerer: relay factor at 29974. Reward: 7000 gold, 6 qp. Level 70-84. |

### Non-chain quest summary

Standalone non-chain quests (`prereq -1`): `140`, `141`, `142`, `145`, `146`, `147`.

Boss chain quests: `143 -> 144`.

### Quest offerer map

- **29962** Gate cartographer: cartography survey intake/turn-in.
- **29927** ration adjudicator: Burn Ledger recovery.
- **29938** channel marshal contact: seal-fraud interdiction support handoff.
- **29904** convoy triage marshal: casualty drill supplies.
- **29905** witness elder: cohort verification writ.
- **29974** Mafdet relay factor: manifest seizure mandate.

---

## District Encounter Scripting Notes (expanded)

- **District A** should feel administratively busy even under decay: mixed neutral caravan remnants and predators create "safe enough" onboarding pressure.
- **District B** should foreground legal-industrial archaeology: extra descriptions on tablets, kiln tags, and assay colors should communicate Seven-Color protocol history.
- **District C** should run high combat tempo: ambush rooms (29933, 29939) and narrow movement funnels emphasize attrition and escort risk.
- **District D** should convey transition anxiety: ashglass visuals and pre-screen legal markers foreshadow Saltglass Reach mechanics.
- **District E** should pivot from social/legal conflict to boss escalation: visible queue and broker negotiation at 29962-29966, then hard lock-in via 29967-29971.

---

## Reset and Door Plan

### Keyed doors

1. **Kiln Master's Seal Vault access** (District B)
   - Door flagged `EX_ISDOOR`; reset state locked (`D ... 2`)
   - Key: `29929` Bassek vault key
2. **Thermal Valve Gallery lock** (District E, 29969)
   - Door flagged `EX_ISDOOR`; reset state locked (`D ... 2`)
   - Key: `29930` crucible valve key

### Spawn density by district

- **A:** moderate fauna + patrol neutrals
- **B:** heavy social/lore NPC density + medium hostility
- **C:** heavy hostile density
- **D:** medium hostility + environmental pressure
- **E:** heavy elite density near 29967-29971, then light at exits

---

## Difficulty and Progression

- **29900-29914:** 65-70 onboarding and route selection.
- **29915-29929:** 68-74 administrative-ruin pressure.
- **29930-29944:** 70-78 sustained combat corridor.
- **29945-29959:** 72-80 environmental + ambush blend.
- **29960-29974:** 75-85 gate politics, elite gauntlet, final boss.

---

## Implementation Checklist

- [ ] Rebuild all 75 rooms with unique spec-compliant descriptions.
- [ ] Rework `#MOBILES` to 25 coherent templates and assign reset ecology.
- [ ] Rework `#OBJECTS` to 25 coherent templates including keys and quest items.
- [ ] Add 8 static quests, with exactly **one** cartography quest and 3 additional non-chain side quests.
- [ ] Validate door/key/reset consistency and bi-directional exits.
- [ ] Run area loader validation and quest parser checks.
