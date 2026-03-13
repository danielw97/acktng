# Propositions Specification

This document specifies the static proposition file format currently used by ACK and catalogs the static proposition templates that are **actually loadable** at runtime.

## Scope and loading behavior

- Static proposition templates are loaded from `../propositions/<n>.prop` at startup, where `<n>` runs from `1` through `PROP_MAX_STATIC_PROPOSITIONS` (`48`). Missing files are ignored.
- Each loaded file is assigned a zero-based static proposition ID (`file number - 1`).
- The parser ignores blank lines, comment lines beginning with `#`, and leading/trailing whitespace around each significant line.
- Repository snapshot: `54` proposition files exist in `propositions/`; `47` are in the loadable range `1-48`.
- Files above the load cap are not loaded unless `PROP_MAX_STATIC_PROPOSITIONS` is increased: `50, 51, 52, 53, 54, 55, 56`.
- Missing file numbers in the load range: `15`.

## File format (`.prop`)

Each proposition file has this structure:
1. **Title line**  
   Freeform text used as the proposition title.
2. **Numeric definition line** (10 or 11 integers):

```text
prerequisite_static_id type num_targets kill_needed min_level [max_level] offerer_vnum reward_gold reward_qp reward_item_vnum reward_item_count
```

- If `max_level` is omitted (10-integer legacy form), runtime default is `170`.
3. **Target vnum line**  
   Space-delimited integer vnums. The parser reads up to `PROP_MAX_TARGETS` (`5`) values.
4. **Accept message line** *(optional)*  
   Message shown on acceptance.
5. **Completion message line** *(optional)*  
   Message shown on completion hand-in.
6. **Custom reward-object block** *(all optional; only used when `reward_item_vnum > 0` and `reward_item_count > 0`)*:
   - reward object short description
   - reward object keywords
   - reward object long description
   - reward object wear flags (int)
   - reward object extra flags (int)
   - reward object weight (int)
   - reward object item-apply selector (int)

## Field definitions

- `prerequisite_static_id`: static proposition ID that must already be completed; `-1` means no prerequisite.
- `type`: proposition objective type.
  - `1` = kill-variety objective (kill one of each listed target).
  - `2` = collect-items objective.
  - `3` = kill-count objective against a single target (`kill_needed` required).
- `num_targets`: number of valid entries expected from the target vnum line.
- `kill_needed`: required kill count (used by type `3`; `0` for non-count objectives).
- `min_level`, `max_level`: pseudo-level gating for acceptance.
- `offerer_vnum`: NPC vnum of the mob that offers the static proposition and receives turn-in.
- `reward_gold`, `reward_qp`: fixed completion rewards for static propositions.
- `reward_item_vnum`, `reward_item_count`: optional base item reward linkage.
- Custom reward-object block fields override generated reward object identity/flags when present.

## Currently loadable static propositions (`1.prop`-`48.prop`)

| File | Static ID | Title | Prereq ID | Type | Targets | Kill Needed | Level Range | Offerer | Rewards |
|---|---:|---|---:|---:|---|---:|---|---:|---|
| `1.prop` | 0 | Route reconnaissance: Forest of Confusion | -1 | 1 | `9601 9602 9603` | 0 | 20-39 | 13001 | 1200 gold, 2 qp |
| `2.prop` | 1 | Conclave residue containment | 0 | 3 | `1011` | 8 | 40-54 | 13001 | 2200 gold, 3 qp |
| `3.prop` | 2 | Eastern trade road interdiction | 1 | 1 | `29904 29909 29932` | 0 | 55-69 | 3015 | 3600 gold, 4 qp |
| `4.prop` | 3 | Sealed-warrant courier disruption | 2 | 1 | `8500 10159 11485` | 0 | 70-84 | 3015 | 5000 gold, 5 qp |
| `5.prop` | 4 | Blightfront quarantine sweep | 3 | 1 | `10012 1030 1050` | 0 | 85-170 | 3015 | 7000 gold, 6 qp |
| `6.prop` | 5 | Road predator cull: Banner Hills to Dustward | -1 | 1 | `3214 3215 3217` | 0 | 15-170 | 3015 | 800 gold, 2 qp |
| `7.prop` | 6 | Tollbreak crew interdiction: River Crossing | -1 | 3 | `3225` | 6 | 30-170 | 3015 | 2000 gold, 3 qp |
| `8.prop` | 7 | Shadow trade disruption: Crossroads to Greenveil | 7 | 1 | `3226 3227 3228` | 0 | 40-170 | 3015 | 3500 gold, 4 qp |
| `9.prop` | 8 | Convoy route security: Dustward predator sweep | -1 | 3 | `3217` | 8 | 20-170 | 13001 | 1000 gold, 2 qp |
| `10.prop` | 9 | Greenveil Spur reconnaissance: spirit clearance | -1 | 1 | `3224 3226` | 0 | 35-170 | 13001 | 1800 gold, 3 qp |
| `11.prop` | 10 | Inlet smuggler crackdown: cave network purge | 10 | 3 | `3228` | 10 | 50-170 | 13001 | 4000 gold, 5 qp |
| `12.prop` | 11 | Ruin custodian suppression: Weathered Causeway | -1 | 1 | `3220 3221 3222 3223` | 0 | 50-170 | 3015 | 4500 gold, 5 qp |
| `13.prop` | 12 | Coastal hazard survey: Western Shore predators | -1 | 1 | `3218 3219 3216` | 0 | 40-170 | 0 | 2500 gold, 3 qp |
| `14.prop` | 13 | Transcontinental corridor assessment: full-road interdiction | 13 | 1 | `3214 3225 3220 3228 3219` | 0 | 60-170 | 0 | 6000 gold, 6 qp |
| `16.prop` | 15 | Lantern Road wolf cull | -1 | 3 | `15505` | 10 | 20-39 | 3015 | 1400 gold, 2 qp |
| `17.prop` | 16 | Mosswater smuggler interdiction | -1 | 1 | `15530 15531 15537` | 0 | 30-54 | 13001 | 2000 gold, 3 qp |
| `18.prop` | 17 | Northern Crown predator survey | -1 | 1 | `15580 15583 15588` | 0 | 40-69 | 14021 | 2800 gold, 3 qp |
| `19.prop` | 18 | Ironpine Rise Ashfang suppression | -1 | 1 | `15545 15546 15552` | 0 | 45-69 | 3015 | 3200 gold, 4 qp |
| `20.prop` | 19 | Rootbound perimeter probe | -1 | 1 | `15562 15563` | 0 | 55-79 | 13001 | 3400 gold, 4 qp |
| `21.prop` | 20 | Rootbound lieutenant strike | 19 | 1 | `15568 15596` | 0 | 70-94 | 13001 | 5200 gold, 5 qp |
| `22.prop` | 21 | Thornmother sanctum assault | 20 | 3 | `15585` | 1 | 85-170 | 13001 | 8000 gold, 7 qp |
| `23.prop` | 22 | Cairn-Keeper exorcism | -1 | 3 | `15561` | 6 | 60-84 | 14021 | 4000 gold, 4 qp |
| `24.prop` | 23 | Ashfang war-chief elimination | 22 | 1 | `15567 15573` | 0 | 75-99 | 14021 | 6000 gold, 5 qp |
| `25.prop` | 24 | Oathbreaker wraith banishment | 23 | 1 | `15584 15595` | 0 | 80-170 | 14021 | 7500 gold, 6 qp |
| `26.prop` | 25 | Bell-Post Line Reopening | -1 | 3 | `9604` | 8 | 18-38 | 13001 | 1700 gold, 2 qp |
| `27.prop` | 26 | Mirrorbark Predator Census | -1 | 1 | `9607 9608 9610` | 0 | 28-49 | 3015 | 2400 gold, 3 qp |
| `28.prop` | 27 | Conclave Survey Slate Recovery | -1 | 2 | `9633` | 0 | 34-58 | 14021 | 2800 gold, 3 qp |
| `29.prop` | 28 | Neogi Warrant Service | -1 | 3 | `9628` | 1 | 35-60 | 13001 | 3200 gold, 4 qp |
| `30.prop` | 29 | Ashen Lattice Containment Sweep | -1 | 1 | `9620 9621 9623` | 0 | 42-68 | 3015 | 3900 gold, 4 qp |
| `31.prop` | 30 | Wardline Reconsolidation | -1 | 1 | `9622 9619` | 0 | 48-74 | 13001 | 4300 gold, 4 qp |
| `32.prop` | 31 | Yugoloth Contract Severance | 30 | 3 | `9641` | 1 | 56-82 | 13001 | 5200 gold, 5 qp |
| `33.prop` | 32 | Ymmas Lair Strike | 31 | 3 | `9642` | 1 | 60-90 | 13001 | 7000 gold, 6 qp |
| `34.prop` | 33 | Cave Route Pacification | -1 | 1 | `9636 9640` | 0 | 44-70 | 14021 | 4200 gold, 4 qp |
| `35.prop` | 34 | Chitin Crown Decapitation | 33 | 3 | `9638` | 1 | 52-80 | 14021 | 6100 gold, 5 qp |
| `36.prop` | 35 | Fen kingpin decapitation | -1 | 3 | `15537` | 1 | 46-72 | 3015 | 3600 gold, 4 qp |
| `37.prop` | 36 | Crown alpha hunt | -1 | 1 | `15587 15589` | 0 | 82-120 | 13001 | 6800 gold, 6 qp |
| `38.prop` | 37 | Briar throne breach | -1 | 3 | `15585` | 1 | 90-170 | 14021 | 9000 gold, 7 qp |
| `39.prop` | 38 | Covenant ash purge | 37 | 1 | `15567 15589` | 0 | 92-170 | 14021 | 9800 gold, 8 qp |
| `40.prop` | 39 | Oathnight final rite | 38 | 1 | `15584 15595` | 0 | 95-170 | 14021 | 11000 gold, 9 qp |
| `41.prop` | 40 | Violet archive stabilization sweep | -1 | 1 | `11206 11209 11223` | 0 | 26-34 | 3015 | 2800 gold, 3 qp |
| `42.prop` | 41 | Evermeet reliquary quieting | -1 | 3 | `11231` | 6 | 28-36 | 13001 | 3200 gold, 3 qp |
| `43.prop` | 42 | Lantern syndic penumbra audit | -1 | 1 | `11241 11244 11248` | 0 | 30-38 | 14021 | 3800 gold, 4 qp |
| `44.prop` | 43 | Mirror-Queen injunction service | 42 | 1 | `11251 11252` | 0 | 34-39 | 14021 | 5200 gold, 5 qp |
| `45.prop` | 44 | Noctivar deposition writ | 43 | 3 | `11254` | 1 | 36-40 | 3015 | 7000 gold, 6 qp |
| `46.prop` | 45 | Preserve command decapitation order | -1 | 1 | `20814 20815 20819` | 0 | 90-124 | 3015 | 8600 gold, 7 qp |
| `47.prop` | 46 | Preserve border attrition campaign | 45 | 3 | `20819` | 6 | 105-140 | 3015 | 9800 gold, 8 qp |
| `48.prop` | 47 | Warden removal final writ | -1 | 1 | `20815 20814 20819` | 0 | 120-170 | 3015 | 12000 gold, 10 qp |
