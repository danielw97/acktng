# Area Plan: The Great Northern Forest

## Overview
- **Area Name:** `@@GThe @@lGreat @@gNorthern @@rForest@@N`
- **File Name:** `great_northern_forest.are`
- **Keyword:** `greatnorthernforest`
- **Intended Levels:** 1-170 (all player levels)
- **Recommended Active Combat Band:** 20-100 in this area (higher-level progression continues into Kowloon and surrounding high-tier zones)
- **Vnum Range:** `15500-15999`
- **Vnum Count:** 500 rooms (all room vnums used)
- **Shape:** Irregular, meandering wilderness with loops, side pockets, and escalating danger bands away from the road
- **South Connection:** Room `15512` south exit connects to Midgaard room `3009` (Granite Arcade, northern wall spine)
- **North Connection:** Room `15994` north exit connects to Kowloon room `14095` (Iron Gate, southern wall)
- **Primary Travel Spine:** The Lantern Road (winding north-south road that repeatedly bends east/west)
- **Reset Rate:** 15
- **Reset Message:** `@@gmist settles between the old pines as the Great Northern Forest stirs@@N`


## Lore
See `great_northern_forest_lore.md` for extracted lore.

## Color Theme (multi-color, spec-safe)
Intentional in-string palette, avoiding `@@k` black, flashing, and background codes:

- **Canopy / life:** `@@G` green
- **Fog / age / stone:** `@@g` gray
- **Cold streams / moonlight:** `@@l` light blue
- **Mossfire / will-o-wisps:** `@@r` light green
- **Hazard / blood sign:** `@@R` red
- **Sacred sites / old oaths:** `@@W` white
- **Reset:** always `@@N`

Theme concept: a cold northern old-growth forest where civilization thins into ancestral wilderness. Near the road, colors are calmer (`@@g`, `@@l`, `@@G`). Deeper interior shifts into heightened `@@r` and danger-marking `@@R` for corrupted groves and apex territories.

## Area Header Plan (`#AREA` compliance)
- `Q 16` (mandatory)
- `K greatnorthernforest~`
- `L @@W(@@r1 170@@W)@@N~`
- `N <allocate at implementation time>`
- `I 1 170`
- `V 15500 15999`
- `F 15`
- `U @@gmist settles between the old pines as the Great Northern Forest stirs@@N~`
- `O Virant~`
- Optional builder flags as needed (`T`, etc.)

---

## Macro Layout (Irregular by Design)
The 500 rooms are arranged as an **irregular 20x25 coordinate envelope** (x=0..19, y=0..24), but connectivity is non-grid: loops, deadwood pockets, branching tributaries, and offset subregions.

- **All vnums `15500-15999` are rooms in this area plan**.
- Coordinates are planning coordinates only; final exits intentionally avoid full cardinal mesh.
- The Lantern Road weaves across the map, never remaining straight for more than 3-4 rooms.

### Coordinate-to-Vnum Formula
`vnum = 15500 + (y * 20) + x`

Example:
- `(x=0,y=0) => 15500`
- `(x=12,y=0) => 15512` (south link to Midgaard `3009`)
- `(x=14,y=24) => 15994` (north link to Kowloon `14095`)

---

## 20x25 Room Layout Grid (all 500 vnums)

| y\\x | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 00 | 15500 | 15501 | 15502 | 15503 | 15504 | 15505 | 15506 | 15507 | 15508 | 15509 | 15510 | 15511 | 15512 | 15513 | 15514 | 15515 | 15516 | 15517 | 15518 | 15519 |
| 01 | 15520 | 15521 | 15522 | 15523 | 15524 | 15525 | 15526 | 15527 | 15528 | 15529 | 15530 | 15531 | 15532 | 15533 | 15534 | 15535 | 15536 | 15537 | 15538 | 15539 |
| 02 | 15540 | 15541 | 15542 | 15543 | 15544 | 15545 | 15546 | 15547 | 15548 | 15549 | 15550 | 15551 | 15552 | 15553 | 15554 | 15555 | 15556 | 15557 | 15558 | 15559 |
| 03 | 15560 | 15561 | 15562 | 15563 | 15564 | 15565 | 15566 | 15567 | 15568 | 15569 | 15570 | 15571 | 15572 | 15573 | 15574 | 15575 | 15576 | 15577 | 15578 | 15579 |
| 04 | 15580 | 15581 | 15582 | 15583 | 15584 | 15585 | 15586 | 15587 | 15588 | 15589 | 15590 | 15591 | 15592 | 15593 | 15594 | 15595 | 15596 | 15597 | 15598 | 15599 |
| 05 | 15600 | 15601 | 15602 | 15603 | 15604 | 15605 | 15606 | 15607 | 15608 | 15609 | 15610 | 15611 | 15612 | 15613 | 15614 | 15615 | 15616 | 15617 | 15618 | 15619 |
| 06 | 15620 | 15621 | 15622 | 15623 | 15624 | 15625 | 15626 | 15627 | 15628 | 15629 | 15630 | 15631 | 15632 | 15633 | 15634 | 15635 | 15636 | 15637 | 15638 | 15639 |
| 07 | 15640 | 15641 | 15642 | 15643 | 15644 | 15645 | 15646 | 15647 | 15648 | 15649 | 15650 | 15651 | 15652 | 15653 | 15654 | 15655 | 15656 | 15657 | 15658 | 15659 |
| 08 | 15660 | 15661 | 15662 | 15663 | 15664 | 15665 | 15666 | 15667 | 15668 | 15669 | 15670 | 15671 | 15672 | 15673 | 15674 | 15675 | 15676 | 15677 | 15678 | 15679 |
| 09 | 15680 | 15681 | 15682 | 15683 | 15684 | 15685 | 15686 | 15687 | 15688 | 15689 | 15690 | 15691 | 15692 | 15693 | 15694 | 15695 | 15696 | 15697 | 15698 | 15699 |
| 10 | 15700 | 15701 | 15702 | 15703 | 15704 | 15705 | 15706 | 15707 | 15708 | 15709 | 15710 | 15711 | 15712 | 15713 | 15714 | 15715 | 15716 | 15717 | 15718 | 15719 |
| 11 | 15720 | 15721 | 15722 | 15723 | 15724 | 15725 | 15726 | 15727 | 15728 | 15729 | 15730 | 15731 | 15732 | 15733 | 15734 | 15735 | 15736 | 15737 | 15738 | 15739 |
| 12 | 15740 | 15741 | 15742 | 15743 | 15744 | 15745 | 15746 | 15747 | 15748 | 15749 | 15750 | 15751 | 15752 | 15753 | 15754 | 15755 | 15756 | 15757 | 15758 | 15759 |
| 13 | 15760 | 15761 | 15762 | 15763 | 15764 | 15765 | 15766 | 15767 | 15768 | 15769 | 15770 | 15771 | 15772 | 15773 | 15774 | 15775 | 15776 | 15777 | 15778 | 15779 |
| 14 | 15780 | 15781 | 15782 | 15783 | 15784 | 15785 | 15786 | 15787 | 15788 | 15789 | 15790 | 15791 | 15792 | 15793 | 15794 | 15795 | 15796 | 15797 | 15798 | 15799 |
| 15 | 15800 | 15801 | 15802 | 15803 | 15804 | 15805 | 15806 | 15807 | 15808 | 15809 | 15810 | 15811 | 15812 | 15813 | 15814 | 15815 | 15816 | 15817 | 15818 | 15819 |
| 16 | 15820 | 15821 | 15822 | 15823 | 15824 | 15825 | 15826 | 15827 | 15828 | 15829 | 15830 | 15831 | 15832 | 15833 | 15834 | 15835 | 15836 | 15837 | 15838 | 15839 |
| 17 | 15840 | 15841 | 15842 | 15843 | 15844 | 15845 | 15846 | 15847 | 15848 | 15849 | 15850 | 15851 | 15852 | 15853 | 15854 | 15855 | 15856 | 15857 | 15858 | 15859 |
| 18 | 15860 | 15861 | 15862 | 15863 | 15864 | 15865 | 15866 | 15867 | 15868 | 15869 | 15870 | 15871 | 15872 | 15873 | 15874 | 15875 | 15876 | 15877 | 15878 | 15879 |
| 19 | 15880 | 15881 | 15882 | 15883 | 15884 | 15885 | 15886 | 15887 | 15888 | 15889 | 15890 | 15891 | 15892 | 15893 | 15894 | 15895 | 15896 | 15897 | 15898 | 15899 |
| 20 | 15900 | 15901 | 15902 | 15903 | 15904 | 15905 | 15906 | 15907 | 15908 | 15909 | 15910 | 15911 | 15912 | 15913 | 15914 | 15915 | 15916 | 15917 | 15918 | 15919 |
| 21 | 15920 | 15921 | 15922 | 15923 | 15924 | 15925 | 15926 | 15927 | 15928 | 15929 | 15930 | 15931 | 15932 | 15933 | 15934 | 15935 | 15936 | 15937 | 15938 | 15939 |
| 22 | 15940 | 15941 | 15942 | 15943 | 15944 | 15945 | 15946 | 15947 | 15948 | 15949 | 15950 | 15951 | 15952 | 15953 | 15954 | 15955 | 15956 | 15957 | 15958 | 15959 |
| 23 | 15960 | 15961 | 15962 | 15963 | 15964 | 15965 | 15966 | 15967 | 15968 | 15969 | 15970 | 15971 | 15972 | 15973 | 15974 | 15975 | 15976 | 15977 | 15978 | 15979 |
| 24 | 15980 | 15981 | 15982 | 15983 | 15984 | 15985 | 15986 | 15987 | 15988 | 15989 | 15990 | 15991 | 15992 | 15993 | 15994 | 15995 | 15996 | 15997 | 15998 | 15999 |

---

## Irregular Region Partition and Progression Bands

### Region A: Southern Verge / Midgaard March (15500-15619, 120 rooms)
- Closest to Midgaard; patrol camps, hunter tracks, shrines, and safer edges.
- Lantern Road begins at `15512` and drifts northwest then northeast before resuming northward.
- **Road mobs:** level 18-26 (target ~20).
- **Off-road mobs:** level 24-38.

### Region B: Mosswater Bends (15620-15719, 100 rooms)
- Wet hollows, streams, and fen bridges.
- Multiple side loops with reed villages and trapped beast dens.
- **Road mobs:** level 22-32.
- **Off-road mobs:** level 35-55.

### Region C: Ironpine Rise (15720-15819, 100 rooms)
- Higher ridges, wolf-clan hunting grounds, abandoned watch cairns.
- Irregular switchbacks and cliffside connectors.
- **Road mobs:** level 28-40.
- **Off-road mobs:** level 48-72.

### Region D: Ancestor Deeps (15820-15919, 100 rooms)
- Megalith rings, spirit groves, and root-cavern mouths.
- Dense branch structure intentionally far from road.
- **Road mobs:** level 35-50.
- **Off-road mobs:** level 65-90.

### Region E: Northern Crown / Kowloon Reach (15920-15999, 80 rooms)
- Final approach toward Kowloon contact zone.
- Cold mist valleys, oath-steles, and elite hostile pockets.
- **Road mobs:** level 38-55.
- **Farthest interior apex pockets:** level 95-100 cap.

> Design rule enforced: the **highest mobs (~100)** are at the **greatest graph distance from Lantern Road**, not merely by north/south position.

---

## Lantern Road Meander Plan (primary route)

Main road sequence (south to north), deliberately winding:

`15512 -> 15511 -> 15531 -> 15530 -> 15550 -> 15551 -> 15571 -> 15572 -> 15592 -> 15612 -> 15613 -> 15633 -> 15653 -> 15652 -> 15672 -> 15673 -> 15693 -> 15713 -> 15712 -> 15732 -> 15752 -> 15753 -> 15773 -> 15774 -> 15794 -> 15814 -> 15813 -> 15833 -> 15834 -> 15854 -> 15874 -> 15873 -> 15893 -> 15913 -> 15912 -> 15932 -> 15952 -> 15953 -> 15973 -> 15974 -> 15994`

Road adjacency verification (all consecutive pairs are cardinally adjacent on the 20x25 grid):
- `15512(12,0)` west to `15511(11,0)` north to `15531(11,1)` west to `15530(10,1)` north to `15550(10,2)` east to `15551(11,2)` ... continuing with verified cardinal adjacency through each step ... north to `15974(14,23)` north to `15994(14,24)`.

- Connects south to Midgaard via `15512` south exit to Midgaard room `3009` (Granite Arcade).
- Connects north to Kowloon via `15994` north exit to Kowloon room `14095` (Iron Gate).
- Never forms a straight uninterrupted cardinal strip.
- Road-adjacent service clearings branch 1-3 steps off this chain.

---

## Connection Plan (external links)

### South Connection: Midgaard
- **Forest room:** `15512` (South Lantern Gate, coordinate x=12, y=0)
- **Midgaard room:** `3009` (Granite Arcade, northernmost point of Midgaard's main north-south spine)
- **Exit:** `15512` has `D2` (south) exit to `3009`. Midgaard `3009` gets `D0` (north) exit to `15512`.
- **Door:** Door flag (`EX_ISDOOR`) set on the `15512` south exit. Reverse exit on `3009` north must also have door flag set.
- **Reset:** `D 0 15512 2 1` (south door, state closed). `D 0 3009 0 1` on Midgaard's side (added when forest is implemented).
- **Key:** `-1` (no key; closed but not locked).
- **Lore:** The South Lantern Gate marks the formal boundary where Midgaard's cleared approaches end and the forest canopy begins. A pair of iron-banded lamp posts flanks the road, their bases carved with Midgaard runes and the split-pine motif of Ilren Voss.

### North Connection: Kowloon
- **Forest room:** `15994` (North Lantern Gate, coordinate x=14, y=24)
- **Kowloon room:** `14095` (Iron Gate, southern wall of Kowloon, per `docs/kowloon_plan.md`)
- **Exit:** `15994` has `D0` (north) exit to `14095`. Kowloon `14095` has `D2` (south) exit to `15994`.
- **Door:** Door flag (`EX_ISDOOR`) set on the `15994` north exit. Reverse exit on `14095` south already has door flag set per Kowloon plan.
- **Reset:** `D 0 15994 0 1` (north door, state closed). Kowloon's `D 0 14095 2 1` handles the reverse.
- **Key:** `-1` (no key; closed but not locked).
- **Lore:** The North Lantern Gate stands where the forest's last ironpines give way to Kowloon's cleared customs approach. A Kowloon-standard chalkboard lists current tariff rates and caravan inspection hours. The gate posts bear both the lantern-and-scale motif and Kowloon's Wardens of Iron insignia.

---

## Door Plan (explicit, spec-compliant)
Only major political boundary gates and select road checkpoints use doors; interior forest paths are undoored natural movement.

### Perimeter Gates

1. **South Lantern Gate** (`15512` south exit to Midgaard `3009`)
   - `D2` (south) exit: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS` command `D`: `D 0 15512 2 1` (door south, state `closed`).
   - Reverse exit on Midgaard `3009` (north) must also have `door` bit set.

2. **North Lantern Gate** (`15994` north exit to Kowloon `14095`)
   - `D0` (north) exit: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS` command `D`: `D 0 15994 0 1` (door north, state `closed`).
   - Reverse exit on Kowloon `14095` (south) already has `door` bit set per Kowloon plan.

### Optional Internal Checkpoint Doors (if desired for pacing, max 4)

3. **Fen Toll Bridge Gate** (`15673` east exit `<->` `15674` west exit)
   - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS`: `D 0 15673 1 1` (east door, state closed).
   - Note: `15673` is a road room (x=13, y=8); `15674` (x=14, y=8) is adjacent east.

4. **Ridge Pass Barricade** (`15794` east exit `<->` `15795` west exit)
   - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS`: `D 0 15794 1 1` (east door, state closed).
   - Note: `15794` is a road room (x=14, y=14); `15795` (x=15, y=14) is adjacent east.

5. **Ancestor Ring Ward Gate** (`15893` east exit `<->` `15894` west exit)
   - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS`: `D 0 15893 1 1` (east door, state closed).
   - Note: `15893` is a road room (x=13, y=19); `15894` (x=14, y=19) is adjacent east.

6. **Northern Customs Post** (`15952` east exit `<->` `15953` west exit)
   - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS`: `D 0 15952 1 1` (east door, state closed).
   - Note: `15952` and `15953` are consecutive road rooms (x=12/13, y=22). This checkpoint gates passage along the road itself.

All checkpoint doors reset to **closed** (state `1`), not locked. Since they are not locked on reset, no key objects are required (spec: key objects are only mandatory when `D` state = `2` locked). `<key_vnum>` is set to `-1` per spec for exits that cannot be locked/unlocked with a key.

If optional checkpoint doors are omitted, only the two perimeter gates remain.

---

## Mob Containment Strategy (strict anti-wander)
All mobs must be local to their intended micro-region. Use one of two containment methods per spawn group:

1. **Sentinel method:**
   - Use `sentinel` + `stay_area` for stationary defenders, camp NPCs, shrine guardians, and named encounters.

2. **No-mob boundary method:**
   - Build small subregion perimeters where all exits out of a mob pocket pass through `ROOM_NO_MOB` choke rooms.
   - Mobs can roam within their pocket but cannot cross perimeter exits.
   - Roaming mobs must still have `stay_area` set.

### Placement rules
- **Roadside patrol/traffic mobs** (target ~20 near south road, scaling modestly north): mostly sentinel or 2-4 room no_mob-bounded patrol loops.
- **Interior predator packs:** no_mob-bounded subgraphs with 6-20 rooms each.
- **Elite apex mobs (95-100):** sentinel or tiny no_mob cages in farthest nodes (e.g., deep branches off `15966-15969`, `15990-15993` interior pockets).
- No free-roaming area-wide wanderers.

---

## Difficulty Gradient by Distance-from-Road
Use shortest-path distance in rooms from nearest Lantern Road node:

- **Distance 0-1:** level 18-30 (road guards, merchants, poachers)
- **Distance 2-3:** level 28-45 (wolves, bandits, mire reptiles)
- **Distance 4-6:** level 42-70 (clan hunters, stone-antler zealots, corrupted beasts)
- **Distance 7-9:** level 68-90 (spirit champions, ancient predators)
- **Distance 10+:** level 90-100 (apex entities, oathbreaker warlords)

This ensures progression is spatially intuitive and compliant with the requested cap behavior.

---

## Room Layout Rules (implementation guidance)
- All 500 vnums are rooms in this area.
- Exits are primarily cardinal; vertical exits used sparingly for caverns/ridges.
- Bi-directional exits required unless one-way is explicitly justified.
- Irregularity enforced via:
  - nonuniform branch lengths,
  - asymmetric loops,
  - cul-de-sacs with rewards/lore,
  - off-axis reconnectors between regions.
- Avoid perfect rectangles of connected rooms in final exit graph.

### Suggested structural percentages
- 40% road + near-road rooms
- 35% mid-depth wilderness loops
- 20% deep interior danger pockets
- 5% special sites (shrines, waystations, megalith rings, diplomatic markers)

---

## Key Story Sites (anchor rooms)
- `15512` **South Lantern Gate** (Midgaard link to `3009`)
- `15673` **Mosswater Toll Bridge**
- `15752` **Ilren's Bent Mile Marker** (historic roadbuilder monument)
- `15893` **Oath-Crossing Stones** (Midgaard-Kowloon covenant marker)
- `15932` **North Warden Camp**
- `15994` **North Lantern Gate** (Kowloon link to `14095`)
- `15997-15999` **Crown of Antlers** (farthest apex wilderness)

These anchors tie lore directly to traversal and level escalation.

---

## Mobile Definitions (80 mobs)

All mobs in this area use vnums `15500-15579` (mob index). Cross-type overlap with room vnums is allowed per spec. All mobs carry `is_npc` (`1`) + `stay_area` (`64`) in act flags. Alignment ranges: good `500-1000`, neutral `-200 to 200`, evil `-1000 to -500`. Combat archetype (melee/hybrid/caster) determines which skill floor table applies per `area_file_spec.md` section 4.1.a.

### Region A: Southern Verge Mobs (15500-15519)

#### Non-Hostile / Civic

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15500 | warden road southern | the southern road warden | 30 | 1 (M) | 750 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms, Region A |
| 15501 | lampkeeper lamp keeper | the lamp-keeper | 25 | 2 (F) | 600 | is_npc, sentinel, stay_area | melee | Lamp maintenance | Road bend rooms |
| 15502 | surveyor timber woodwrights | the Woodwrights Guild surveyor | 28 | 1 (M) | 500 | is_npc, sentinel, stay_area | melee | Timber assessment | Near-road rooms |
| 15503 | guard caravan southern | a southern caravan guard | 32 | 1 (M) | 600 | is_npc, stay_area | melee | Caravan escort | Road rooms |
| 15504 | peddler forest traveling | a traveling forest peddler | 20 | 2 (F) | 400 | is_npc, sentinel, stay_area | — | Flavor NPC | Roadside clearings |

#### Hostile / Wildlife

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15505 | wolf timber grey | a grey timber wolf | 22 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Off-road, 1-2 rooms from path |
| 15506 | boar wild forest | a wild forest boar | 20 | 0 | 0 | is_npc, aggressive, stay_area | melee | Territorial beast | Off-road thickets |
| 15507 | spider forest giant | a giant forest spider | 24 | 0 | -200 | is_npc, aggressive, stay_area | melee | Ambush predator | Dense canopy rooms |
| 15508 | poacher forest | a forest poacher | 26 | 1 (M) | -350 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Off-road, 2-3 rooms out |
| 15509 | highwayman masked | a masked highwayman | 30 | 1 (M) | -500 | is_npc, aggressive, stay_area, remember | melee | Road bandit | Near-road ambush spots |
| 15510 | deer stag young | a young stag | 18 | 1 (M) | 0 | is_npc, stay_area | — | Wildlife flavor | Forest glades |
| 15511 | fox red forest | a red forest fox | 18 | 0 | 0 | is_npc, stay_area, wimpy | — | Wildlife flavor | Undergrowth rooms |
| 15512 | snake copperhead | a copperhead snake | 20 | 0 | 0 | is_npc, aggressive, stay_area | melee | Minor predator | Rocky off-road |
| 15513 | bandit woodland | a woodland bandit | 28 | 2 (F) | -450 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Off-road camps |
| 15514 | bear black young | a young black bear | 26 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Deep off-road |
| 15515 | wolf pack runner | a wolf pack runner | 24 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fast predator | Wolf territory rooms |
| 15516 | patrol sergeant southern | the southern patrol sergeant | 38 | 1 (M) | 700 | is_npc, sentinel, stay_area | melee | Road authority | South gate area |
| 15517 | hunter trapper verge | a verge trapper | 22 | 1 (M) | 200 | is_npc, sentinel, stay_area | — | Flavor NPC | Trapping clearings |
| 15518 | owl horned great | a great horned owl | 20 | 0 | 0 | is_npc, stay_area | — | Wildlife flavor | Canopy rooms |
| 15519 | badger forest | a forest badger | 22 | 0 | 0 | is_npc, aggressive, stay_area | melee | Minor predator | Burrow rooms |

### Region B: Mosswater Bends Mobs (15520-15539)

#### Non-Hostile / Civic

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15520 | fisher mire clan | a mire-clan fisher | 25 | 1 (M) | 300 | is_npc, sentinel, stay_area | — | Flavor NPC | Reed-platform rooms |
| 15521 | herbalist mire clan | a mire-clan herbalist | 28 | 2 (F) | 400 | is_npc, sentinel, stay_area | — | Flavor/trade NPC | Fen village rooms |
| 15522 | collector toll fen | the fen toll collector | 35 | 1 (M) | 500 | is_npc, sentinel, stay_area | melee | Checkpoint NPC | Mosswater Toll Bridge |
| 15523 | trader reed platform | a reed-platform trader | 30 | 2 (F) | 300 | is_npc, sentinel, stay_area | — | Flavor/trade NPC | Fen village rooms |
| 15524 | warden road mosswater | the Mosswater road warden | 35 | 2 (F) | 700 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms, Region B |

#### Hostile / Wildlife

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15525 | snake marsh water | a marsh water snake | 28 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Waterway rooms |
| 15526 | frog giant fen | a giant fen frog | 30 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Shallow water rooms |
| 15527 | heron territorial giant | a giant territorial heron | 32 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Hummock rooms |
| 15528 | crocodile mosswater | a Mosswater crocodile | 38 | 0 | 0 | is_npc, aggressive, stay_area | melee | Apex fen predator | Deep channel rooms |
| 15529 | lurker bog | a bog lurker | 42 | 0 | -300 | is_npc, aggressive, stay_area | melee | Ambush predator | Deep fen rooms |
| 15530 | smuggler lookout | a smuggler lookout | 35 | 1 (M) | -500 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Off-road fen edges |
| 15531 | smuggler runner fen | a fen smuggler runner | 38 | 2 (F) | -500 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Smuggler camps |
| 15532 | leech swarm giant | a giant leech swarm | 30 | 0 | 0 | is_npc, aggressive, stay_area, no_body | melee | Fen hazard | Stagnant water rooms |
| 15533 | turtle snapping moss | a moss-backed snapping turtle | 34 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Stream crossing rooms |
| 15534 | wisp will marsh | a marsh will-o-wisp | 36 | 0 | -400 | is_npc, aggressive, stay_area, no_body | caster | Fen hazard | Deep fog rooms |
| 15535 | eel electric mosswater | an electric mosswater eel | 32 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Deep water rooms |
| 15536 | wolf fen grey | a fen-grey wolf | 28 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Fen-edge hunting grounds |
| 15537 | smuggler boss fen | the fen smuggler boss | 48 | 1 (M) | -700 | is_npc, aggressive, stay_area, solo, remember | hybrid | Strong hostile | Deep smuggler camp |
| 15538 | asp mud venom | a venomous mud asp | 30 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen hazard | Mudbank rooms |
| 15539 | crane mist fisher | a mist-fisher crane | 26 | 0 | 0 | is_npc, stay_area | — | Wildlife flavor | Open water rooms |

### Region C: Ironpine Rise Mobs (15540-15559)

#### Non-Hostile / Civic

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15540 | warden ridge ironpine | the Ironpine ridge warden | 45 | 1 (M) | 700 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms, Region C |
| 15541 | hermit everkeeper old | an old Everkeeper hermit | 40 | 1 (M) | 800 | is_npc, sentinel, stay_area | hybrid | Lore NPC | Planting-ring rooms |
| 15542 | scout patrol joint | a joint-commission patrol scout | 42 | 2 (F) | 600 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms |

#### Hostile / Wildlife / Faction

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15543 | wolf mountain grey | a grey mountain wolf | 38 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Ridge corridors |
| 15544 | cat mountain stalker | a mountain stalker cat | 42 | 0 | 0 | is_npc, aggressive, stay_area | melee | Ambush predator | Cliffside rooms |
| 15545 | scout ashfang | an Ashfang scout | 50 | 1 (M) | -600 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Off-road, 3-4 rooms out |
| 15546 | raider ashfang | an Ashfang raider | 55 | 2 (F) | -650 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Ashfang camp rooms |
| 15547 | shade everkeeper | an Everkeeper's shade | 52 | 0 | -300 | is_npc, aggressive, stay_area, undead | caster | Undead encounter | Abandoned planting-rings |
| 15548 | sentinel stone antler | a stone-antler sentinel | 58 | 1 (M) | 100 | is_npc, aggressive, stay_area | melee | Sacred site guard | Near megalith rooms |
| 15549 | bear ironpine brown | an ironpine brown bear | 48 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Deep ridge rooms |
| 15550 | alpha wolf pack | the wolf pack alpha | 60 | 1 (M) | 0 | is_npc, aggressive, stay_area, solo | melee | Strong predator | Wolf den rooms |
| 15551 | hawk ridge hunter | a ridge-hunter hawk | 35 | 0 | 0 | is_npc, aggressive, stay_area | melee | Aerial predator | Exposed ridge rooms |
| 15552 | ashfang trapper | an Ashfang trapper | 52 | 1 (M) | -600 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Trap-line rooms |
| 15553 | boar ridge tusked | a tusked ridge boar | 40 | 0 | 0 | is_npc, aggressive, stay_area | melee | Territorial beast | Rocky clearings |
| 15554 | viper rock ironpine | an ironpine rock viper | 36 | 0 | 0 | is_npc, aggressive, stay_area | melee | Ambush predator | Boulder-field rooms |
| 15555 | archer stone antler | a stone-antler archer | 62 | 2 (F) | 100 | is_npc, aggressive, stay_area | melee | Sacred site guard | Outer megalith rooms |
| 15556 | wolverine ridge | a ridge wolverine | 44 | 0 | 0 | is_npc, aggressive, stay_area, no_flee | melee | Fearless predator | Deep ridge rooms |
| 15557 | spider ironpine trap | an ironpine trap-door spider | 38 | 0 | -200 | is_npc, aggressive, stay_area | melee | Ambush predator | Root-hollow rooms |
| 15558 | lynx shadow ironpine | an ironpine shadow lynx | 46 | 0 | 0 | is_npc, aggressive, stay_area | melee | Stealth predator | Dense canopy rooms |
| 15559 | ashfang shaman ridge | an Ashfang ridge shaman | 58 | 1 (M) | -700 | is_npc, aggressive, stay_area, remember | caster | Hostile caster | Ashfang ritual sites |

### Region D: Ancestor Deeps Mobs (15560-15574)

#### Non-Hostile

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15560 | warden ancestor road deep | the Ancestor Deeps road warden | 50 | 1 (M) | 700 | is_npc, sentinel, stay_area | hybrid | Road patrol | Road rooms, Region D |

#### Hostile / Wildlife / Faction

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15561 | ghost cairnkeeper | a Cairn-Keeper ghost | 70 | 0 | -500 | is_npc, aggressive, stay_area, undead, no_body | caster | Undead encounter | Standing stone rooms |
| 15562 | acolyte rootbound | a Rootbound acolyte | 65 | 2 (F) | -800 | is_npc, aggressive, stay_area, remember | hybrid | Hostile cultist | Deep branch rooms |
| 15563 | ritualist rootbound | a Rootbound ritualist | 72 | 1 (M) | -850 | is_npc, aggressive, stay_area, remember | caster | Hostile caster | Ritual clearing rooms |
| 15564 | wolf dire ancestor | an ancestor dire wolf | 68 | 0 | 0 | is_npc, aggressive, stay_area | melee | Apex predator | Deep interior rooms |
| 15565 | bear cave ancient | an ancient cave bear | 75 | 0 | 0 | is_npc, aggressive, stay_area | melee | Apex predator | Cavern-mouth rooms |
| 15566 | guardian spirit megalith | a megalith spirit guardian | 72 | 0 | 200 | is_npc, aggressive, stay_area | caster | Sacred site defender | Megalith ring rooms |
| 15567 | warchief ashfang | the Ashfang war-chief | 82 | 1 (M) | -800 | is_npc, aggressive, stay_area, solo, remember, no_flee | hybrid | Strong hostile | Deep Ashfang camp |
| 15568 | lieutenant thornmother | a Thornmother's lieutenant | 80 | 2 (F) | -900 | is_npc, aggressive, stay_area, solo, remember | hybrid | Strong hostile | Rootbound outer grove |
| 15569 | stag corrupted red | a red-corrupted stag | 70 | 1 (M) | -400 | is_npc, aggressive, stay_area | melee | Corrupted wildlife | Red-moss bloom rooms |
| 15570 | watcher megalith stone | a megalith stone watcher | 78 | 0 | 0 | is_npc, aggressive, stay_area, no_body | caster | Construct guardian | Inner megalith rooms |
| 15571 | treant ironpine ancient | an ancient ironpine treant | 85 | 0 | 300 | is_npc, aggressive, stay_area, solo, no_flee | melee | Strong predator | Deepest ironpine groves |
| 15572 | serpent root coiled | a coiled root serpent | 65 | 0 | -300 | is_npc, aggressive, stay_area | melee | Corrupted wildlife | Root-cavern rooms |
| 15573 | ashfang berserker deep | a deep Ashfang berserker | 75 | 1 (M) | -700 | is_npc, aggressive, stay_area, remember, no_flee | melee | Hostile humanoid | Deep Ashfang territory |
| 15574 | moth ghost pale | a pale ghost moth | 62 | 0 | -200 | is_npc, aggressive, stay_area | caster | Deep-forest hazard | Luminescent moss rooms |

### Region E: Northern Crown / Kowloon Reach Mobs (15575-15599)

#### Non-Hostile / Civic

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15575 | officer customs northern | the northern customs officer | 65 | 1 (M) | 700 | is_npc, sentinel, stay_area | melee | Checkpoint NPC | Northern Customs Post |
| 15576 | warden patrol kowloon | a Kowloon patrol warden | 60 | 2 (F) | 700 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms, Region E |
| 15577 | runner courier lantern | a Courier Lantern runner | 45 | 1 (M) | 600 | is_npc, stay_area | melee | Message runner | Road rooms, Region E |
| 15578 | captain warden north | the North Warden Camp captain | 75 | 1 (M) | 800 | is_npc, sentinel, stay_area | hybrid | Camp commander | North Warden Camp |
| 15579 | guard warden north camp | a North Warden Camp guard | 55 | 2 (F) | 700 | is_npc, sentinel, stay_area | melee | Camp guard | North Warden Camp |

#### Hostile / Wildlife / Faction

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 15580 | viper mist northern | a northern mist viper | 45 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Mist-valley rooms |
| 15581 | berserker ashfang northern | a northern Ashfang berserker | 70 | 1 (M) | -700 | is_npc, aggressive, stay_area, remember, no_flee | melee | Hostile humanoid | Northern Ashfang camps |
| 15582 | elk crown bull | a crown bull elk | 55 | 1 (M) | 0 | is_npc, aggressive, stay_area | melee | Territorial beast | Northern ridgeline rooms |
| 15583 | wolf frost touched | a frost-touched wolf | 50 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Cold-valley rooms |
| 15584 | wraith oathbreaker | an oathbreaker wraith | 80 | 0 | -900 | is_npc, aggressive, stay_area, undead, no_body | caster | Undead encounter | Ruined oath-stele rooms |
| 15585 | thornmother the | @@Rthe Thornmother@@N | 98 | 2 (F) | -1000 | is_npc, aggressive, sentinel, stay_area, boss, no_flee | caster | **BOSS** | Hidden grove (`no_mob` room) |
| 15586 | stag white the | @@Wthe White Stag@@N | 90 | 1 (M) | 1000 | is_npc, sentinel, stay_area, solo | — | Rare encounter | Crown of Antlers apex |
| 15587 | lord bear primal | @@Rthe Primal Bear Lord@@N | 95 | 1 (M) | 0 | is_npc, aggressive, sentinel, stay_area, solo, no_flee | melee | Apex predator | Crown of Antlers apex |
| 15588 | cat mist shadow | a mist-shadow cat | 48 | 0 | 0 | is_npc, aggressive, stay_area | melee | Ambush predator | Mist-valley rooms |
| 15589 | ashfang warmaster northern | the northern Ashfang warmaster | 88 | 1 (M) | -800 | is_npc, aggressive, stay_area, solo, remember, no_flee | hybrid | Strong hostile | Deepest Ashfang camp |
| 15590 | cultist rootbound crown | a Crown Rootbound cultist | 78 | 2 (F) | -850 | is_npc, aggressive, stay_area, remember | hybrid | Hostile cultist | Northern cult rooms |
| 15591 | elk cow crown | a crown cow elk | 40 | 2 (F) | 0 | is_npc, aggressive, stay_area | melee | Territorial beast | Elk grazing rooms |
| 15592 | serpent crown ice | an ice crown serpent | 55 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Cold-stream rooms |
| 15593 | sentinel antler stone elder | an elder stone-antler sentinel | 82 | 1 (M) | 200 | is_npc, aggressive, stay_area, solo | melee | Strong guardian | Northern megalith rooms |
| 15594 | hawk bone crowned | a bone-crowned hawk | 42 | 0 | 0 | is_npc, aggressive, stay_area | melee | Aerial predator | Exposed peak rooms |
| 15595 | spirit ancestor cold | a cold ancestor spirit | 85 | 0 | -600 | is_npc, aggressive, stay_area, undead, no_body | caster | Undead encounter | Northern standing stones |
| 15596 | rootbound briar knight | a Rootbound briar knight | 82 | 1 (M) | -900 | is_npc, aggressive, stay_area, remember, no_flee | melee | Elite cultist | Thornmother approach |
| 15597 | boar ghost white | a white ghost boar | 60 | 0 | 0 | is_npc, aggressive, stay_area | melee | Spectral beast | Northern ghost-birch rooms |
| 15598 | bear mother ironpine | an ironpine mother bear | 65 | 2 (F) | 0 | is_npc, aggressive, stay_area, no_flee | melee | Protective predator | Northern den rooms |
| 15599 | warden ironhand kowloon | a Kowloon Warden of Iron | 70 | 1 (M) | 700 | is_npc, sentinel, stay_area | hybrid | Kowloon patrol | Near north gate rooms |

### Mobile Extension Notes

For the `!` extension line, combat archetypes determine skill/cast/def assignment using the floor tables in `area_file_spec.md` section 4.1.a:

- **Melee profile mobs** (wildlife, warriors, raiders, guards): Use melee floor table. `cast` = `0`, `def` = `0` (or minimal self-heal for named humanoids).
- **Hybrid profile mobs** (war-chiefs, shamans, lieutenants, camp captains): Use hybrid floor table. `cast` includes 1-3 offensive spells appropriate to level. `def` includes `cure_light`/`cure_serious`/`heal` as level permits.
- **Caster profile mobs** (ritualists, ghosts, shades, spirit guardians, the Thornmother): Use caster floor table. `cast` includes multiple offensive spells. `def` includes healing and shielding.

Suggested `cast` assignments by faction:
- **Rootbound cultists/Thornmother:** `flamestrike`, `earthquake`, `faerie_fire`, `suffocate` (nature/corruption theme)
- **Ashfang shamans:** `fireball`, `burn_hands`, `flare`, `heat_armor` (fire/ash theme)
- **Cairn-Keeper ghosts/oathbreaker wraiths:** `mind_flail`, `phobia`, `ego_whip`, `mind_bolt` (psychic/spirit theme)
- **Everkeeper shades:** `earthquake`, `faerie_fire`, `static` (nature remnant theme)
- **Megalith spirit guardians/stone watchers:** `planergy`, `light_bolt`, `chain_light` (arcane/earth theme)
- **Will-o-wisps/ghost moths:** `col_spray`, `static`, `mind_flail` (eldritch theme)

Suggested `def` assignments:
- **Named humanoid leaders (war-chief, warmaster, Thornmother):** `heal` + `fireshield` or `iceshield`
- **Mid-tier hostile humanoids:** `cure_critic` or `cure_serious`
- **Undead casters:** `cure_light` only (weakened healing)
- **Non-caster wildlife:** `def` = `0`

### Specials Plan

| Mob Vnum | Mob Name | Special Function | Rationale |
|---|---|---|---|
| 15585 | the Thornmother | `spec_cast_undead` | Boss caster; undead/nature hybrid with dark casting |
| 15559 | Ashfang ridge shaman | `spec_cast_cleric` | Hostile caster with healing and offense |
| 15563 | Rootbound ritualist | `spec_cast_mage` | Cult caster with arcane offense |
| 15534 | marsh will-o-wisp | `spec_poison` | Fen hazard with poison attacks |
| 15547 | Everkeeper's shade | `spec_cast_undead` | Undead caster |
| 15561 | Cairn-Keeper ghost | `spec_cast_undead` | Undead caster |
| 15584 | oathbreaker wraith | `spec_cast_undead` | Undead caster |
| 15595 | cold ancestor spirit | `spec_cast_undead` | Undead caster |
| 15566 | megalith spirit guardian | `spec_cast_mage` | Arcane guardian caster |
| 15570 | megalith stone watcher | `spec_cast_mage` | Arcane construct caster |
| 15587 | the Primal Bear Lord | `spec_breath_frost` | Apex beast with frost breath |

---

## Object Definitions (90 objects)

All objects in this area use vnums `15500-15589` (object index). Cross-type overlap with room and mob vnums is allowed per spec. All objects must include `ITEM_TAKE` (`8388608`) in wear flags. No object may set `ITEM_GENERATED` or `ITEM_WEAR_CLAN_COLORS`. Item stats are generated at runtime; only identity/behavior data is authored. Object names must be unique within the area file.

Weight archetype ranges: `1-5` = caster item, `6-10` = melee item, `11-15` = tank item.

### Weapons (item_type = 5, wear_flags include hold + take)

All weapons require `hold` (`32768`) + `take` (`8388608`) in wear_flags. `value3` must be thematically consistent with weapon concept. Two-handed weapons include `ITEM_TWO_HANDED` (`2147483648`) in extra_flags. Fist weapons include `ITEM_FIST` (`1073741824`) and use `value3 = 0` (hit).

| Vnum | name (keywords) | short_descr | Lv | Weight | value3 | Extra Flags | Loot? | Notes |
|---|---|---|---:|---:|---:|---|---|---|
| 15500 | sword short warden | a warden's short sword | 20 | 7 | 3 (slash) | — | — | Road warden equipment |
| 15501 | knife hunting poacher | a poacher's hunting knife | 22 | 6 | 2 (stab) | ITEM_LOOT | loot | Poacher drop |
| 15502 | spear bone mire clan | a mire-clan bone spear | 30 | 8 | 11 (pierce) | ITEM_LOOT | loot | Mire-clan themed |
| 15503 | javelin fang tipped | a fang-tipped javelin | 35 | 7 | 11 (pierce) | ITEM_LOOT | loot | Wildlife-themed |
| 15504 | axe blackened ashfang | an Ashfang blackened axe | 50 | 9 | 3 (slash) | ITEM_LOOT | loot | Ashfang drop |
| 15505 | blade ritual stone antler | a stone-antler ritual blade | 55 | 6 | 1 (slice) | ITEM_LOOT | loot | Stone-antler drop |
| 15506 | mace heavy ironpine | an ironpine heavy mace | 60 | 10 | 7 (pound) | ITEM_LOOT | loot | Ironpine Rise themed |
| 15507 | halberd ridge warden | a ridge warden's halberd | 65 | 12 | 3 (slash) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Two-handed |
| 15508 | glaive thorn rootbound | a Rootbound thorn glaive | 70 | 9 | 1 (slice) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Two-handed, cult drop |
| 15509 | cleaver warchief ashfang | an Ashfang war-chief's cleaver | 80 | 10 | 3 (slash) | ITEM_LOOT | loot | War-chief drop |
| 15510 | blade ghost cairnkeeper | a Cairn-Keeper ghost blade | 85 | 5 | 1 (slice) | ITEM_LOOT | loot | Undead drop |
| 15511 | scourge briar thornmother | a Thornmother's briar scourge | 90 | 7 | 4 (whip) | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |
| 15512 | greataxe crown antlers | a Crown of Antlers great axe | 95 | 13 | 3 (slash) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Apex drop |
| 15513 | sword cursed oathbreaker | an oathbreaker's cursed sword | 95 | 8 | 3 (slash) | ITEM_LOOT | loot | Wraith drop |
| 15514 | fist bearclaw primal | a primal bear-claw fist weapon | 100 | 9 | 0 (hit) | ITEM_LOOT, ITEM_FIST | loot | Apex beast drop |
| 15515 | spear lantern road warden | a Lantern Road warden's spear | 40 | 8 | 11 (pierce) | — | — | Warden equipment |
| 15516 | dagger smuggler concealed | a smuggler's concealed dagger | 35 | 6 | 2 (stab) | ITEM_LOOT | loot | Smuggler drop |
| 15517 | club ironwood heavy | a heavy ironwood club | 45 | 10 | 8 (crush) | ITEM_LOOT | loot | Bandit/raider drop |
| 15518 | whip vine thornmother | a Thornmother's vine whip | 92 | 5 | 4 (whip) | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |
| 15519 | greatsword ashfang warmaster | a warmaster's ashfang greatsword | 88 | 12 | 3 (slash) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Warmaster drop |
| 15520 | spear antler elder stone | an elder stone-antler spear | 82 | 9 | 11 (pierce) | ITEM_LOOT | loot | Elder sentinel drop |
| 15521 | pike briar rootbound knight | a briar knight's thorn pike | 82 | 11 | 11 (pierce) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Briar knight drop |
| 15522 | staff gnarled treant | a gnarled treant staff | 85 | 12 | 8 (crush) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Treant drop |
| 15523 | wand fen wisp | a fen wisp wand | 36 | 3 | 6 (blast) | ITEM_LOOT, ITEM_EXTRA_WAND | loot | Will-o-wisp drop |

### Armor (item_type = 9)

All armor requires `take` (`8388608`) plus the appropriate wear slot flag. Shields use `hold` (`32768`). Bucklers add `ITEM_BUCKLER` (`268435456`).

| Vnum | name (keywords) | short_descr | Lv | Weight | Wear Slot | Extra Flags | Loot? | Notes |
|---|---|---|---:|---:|---|---|---|---|
| 15524 | jerkin leather warden | a warden's leather jerkin | 20 | 12 | body | — | — | Road warden equipment |
| 15525 | hood fur lined woodsman | a woodsman's fur-lined hood | 22 | 11 | head | ITEM_LOOT | loot | Poacher/trapper drop |
| 15526 | mantle woven mire clan | a mire-clan woven mantle | 30 | 4 | about | ITEM_LOOT | loot | Mire-clan themed |
| 15527 | boots marsh treader | a pair of marsh-treader boots | 35 | 11 | feet | ITEM_LOOT | loot | Fen themed |
| 15528 | pauldrons spiked ashfang | a pair of Ashfang spiked pauldrons | 50 | 13 | shoulders | ITEM_LOOT | loot | Ashfang drop |
| 15529 | helm bone stone antler | a stone-antler bone helm | 55 | 12 | head | ITEM_LOOT | loot | Stone-antler drop |
| 15530 | shield bark ironpine | an ironpine bark shield | 60 | 14 | hold | ITEM_LOOT | loot | Ironpine Rise themed |
| 15531 | vest chain ridge patrol | a ridge patrol chain vest | 65 | 14 | body | ITEM_LOOT | loot | Ridge warden drop |
| 15532 | armor living rootbound | a suit of Rootbound living armor | 70 | 12 | body | ITEM_LOOT | loot | Rootbound cultist drop |
| 15533 | plate warchief ashfang | an Ashfang war-chief's plate | 80 | 15 | body | ITEM_LOOT | loot | War-chief drop |
| 15534 | shield ghost cairnkeeper | a Cairn-Keeper ghost shield | 85 | 4 | hold | ITEM_LOOT | loot | Ghost drop |
| 15535 | crown briar thornmother | a Thornmother's briar crown | 90 | 4 | head | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |
| 15536 | greatshield bearhide primal | a primal bearhide greatshield | 95 | 15 | hold | ITEM_LOOT | loot | Apex drop |
| 15537 | gauntlets cursed oathbreaker | a pair of oathbreaker's cursed gauntlets | 95 | 9 | hands | ITEM_LOOT | loot | Wraith drop |
| 15538 | buckler stag crown | a crown stag-horn buckler | 100 | 13 | hold | ITEM_LOOT, ITEM_BUCKLER | loot | Apex drop |
| 15539 | greaves lantern road patrol | a pair of Lantern Road patrol greaves | 40 | 13 | legs | — | — | Warden equipment |
| 15540 | bracers ashfang iron | a pair of Ashfang iron bracers | 55 | 10 | wrist | ITEM_LOOT | loot | Ashfang drop |
| 15541 | coat mosswater oiled | a mosswater oiled coat | 32 | 11 | about | ITEM_LOOT | loot | Fen themed |
| 15542 | helm ironpine bark ridge | an ironpine bark ridge helm | 60 | 13 | head | ITEM_LOOT | loot | Ironpine Rise themed |
| 15543 | boots ridge patrol heavy | a pair of heavy ridge patrol boots | 65 | 14 | feet | ITEM_LOOT | loot | Ridge patrol drop |
| 15544 | cloak treant moss | a treant moss cloak | 85 | 5 | about | ITEM_LOOT | loot | Treant drop |
| 15545 | plate briar knight rootbound | a briar knight's thorn plate | 82 | 14 | body | ITEM_LOOT | loot | Briar knight drop |
| 15546 | vambraces warmaster ashfang | a pair of warmaster's ashfang vambraces | 88 | 12 | arms | ITEM_LOOT | loot | Warmaster drop |
| 15547 | skirt chain ashfang | an Ashfang chain skirt | 52 | 12 | legs | ITEM_LOOT | loot | Ashfang raider drop |
| 15548 | cowl rootbound vine | a Rootbound vine cowl | 65 | 4 | head | ITEM_LOOT | loot | Rootbound acolyte drop |
| 15549 | shield elder stone antler | an elder stone-antler shield | 82 | 15 | hold | ITEM_LOOT | loot | Elder sentinel drop |
| 15550 | robe thornmother thorn woven | a Thornmother's thorn-woven robe | 95 | 3 | body | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |

### Accessories (item_type = 9 for worn, or item_type = 8 for treasure)

| Vnum | name (keywords) | short_descr | Lv | Weight | Item Type | Wear Slot | Extra Flags | Loot? | Notes |
|---|---|---|---:|---:|---:|---|---|---|---|
| 15551 | pendant lantern warden | a warden's lantern pendant | 20 | 3 | 9 | neck | — | — | Warden equipment |
| 15552 | ring reed mire speaker | a mire-speaker's reed ring | 30 | 2 | 9 | finger | ITEM_LOOT | loot | Mire-clan themed |
| 15553 | necklace tooth ashfang | an Ashfang tooth necklace | 50 | 8 | 9 | neck | ITEM_LOOT | loot | Ashfang drop |
| 15554 | torc stone antler circle | a stone-antler circle torc | 55 | 7 | 9 | neck | ITEM_LOOT | loot | Stone-antler drop |
| 15555 | ring planting everkeeper | an Everkeeper's planting-ring | 60 | 3 | 9 | finger | ITEM_LOOT | loot | Everkeeper shade drop |
| 15556 | amulet bone cairnkeeper | a Cairn-Keeper's bone amulet | 75 | 4 | 9 | neck | ITEM_LOOT | loot | Ghost drop |
| 15557 | bracelet vine rootbound | a Rootbound vine bracelet | 70 | 3 | 9 | wrist | ITEM_LOOT | loot | Rootbound drop |
| 15558 | ring signet oathbreaker | an oathbreaker's signet ring | 85 | 4 | 9 | finger | ITEM_LOOT | loot | Wraith drop |
| 15559 | circlet thorn thornmother | a Thornmother's thorn circlet | 90 | 2 | 9 | horns | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |
| 15560 | crown antler white stag | a white stag antler crown | 100 | 4 | 9 | head | ITEM_LOOT | loot | Apex rare drop |
| 15561 | earring fang wolf | a wolf-fang earring | 25 | 6 | 9 | ear | ITEM_LOOT | loot | Wolf territory drop |
| 15562 | belt smuggler hidden | a smuggler's hidden-pocket belt | 35 | 7 | 9 | waist | ITEM_LOOT | loot | Smuggler drop |
| 15563 | cloak patrol warden forest | a forest warden's patrol cloak | 40 | 11 | 9 | about | — | — | Warden equipment |
| 15564 | sash joint commission | a joint-commission warden's sash | 42 | 3 | 9 | waist | — | — | Scout equipment |
| 15565 | ring ironpine bark | an ironpine bark ring | 60 | 4 | 9 | finger | ITEM_LOOT | loot | Ironpine Rise themed |
| 15566 | torque ancestor bone cold | a cold ancestor-bone torque | 85 | 5 | 9 | neck | ITEM_LOOT | loot | Ancestor spirit drop |
| 15567 | crown iron warmaster | a warmaster's iron crown | 88 | 9 | 9 | head | ITEM_LOOT | loot | Warmaster drop |
| 15568 | amulet bear tooth primal | a primal bear-tooth amulet | 95 | 8 | 9 | neck | ITEM_LOOT | loot | Bear Lord drop |

### Consumables and Flavor Objects

| Vnum | name (keywords) | short_descr | Lv | Weight | Item Type | Wear Flags | Extra Flags | Notes |
|---|---|---|---:|---:|---:|---|---|---|
| 15569 | rations trail warden | a pack of warden's trail rations | 1 | 6 | 19 (food) | take | — | Road warden supply |
| 15570 | flask resin mire clan | a mire-clan resin flask | 1 | 4 | 17 (drink_con) | take | — | Fen village flavor |
| 15571 | mushroom forest edible | an edible forest mushroom | 1 | 2 | 19 (food) | take | — | Forage item |
| 15572 | tea pine northern | a pouch of northern pine tea | 1 | 2 | 19 (food) | take | — | Northern Crown flavor |
| 15573 | bandage herb poultice | a herbal poultice bandage | 1 | 3 | 19 (food) | take | — | Fen herbalist supply |
| 15574 | ledger passage tithe relic | a relic passage tithe ledger | 1 | 4 | 13 (trash) | take | — | Room flavor object |
| 15575 | seal stamp midgaard wax | a Midgaard wax seal stamp | 1 | 3 | 13 (trash) | take | — | Room flavor object |
| 15576 | seal knot kowloon thread | a Kowloon thread-knot seal | 1 | 3 | 13 (trash) | take | — | Room flavor object |
| 15577 | lantern road iron banded | an iron-banded road lantern | 1 | 8 | 1 (light) | take | — | Road atmosphere |
| 15578 | map forest worn trail | a worn forest trail map | 1 | 2 | 13 (trash) | take | — | Flavor loot |
| 15579 | hide wolf grey tanned | a tanned grey wolf hide | 1 | 7 | 13 (trash) | take | — | Wildlife loot |
| 15580 | fang boar tusked | a tusked boar fang | 1 | 5 | 13 (trash) | take | — | Wildlife loot |
| 15581 | venom sac snake marsh | a marsh snake venom sac | 1 | 2 | 13 (trash) | take | — | Fen wildlife loot |
| 15582 | skin frog giant fen | a giant fen frog skin | 1 | 4 | 13 (trash) | take | — | Fen wildlife loot |
| 15583 | claw bear cave ancient | an ancient cave bear claw | 1 | 6 | 13 (trash) | take | — | Trophy drop |
| 15584 | antler stag white | a white stag antler | 1 | 5 | 8 (treasure) | take | — | Rare trophy |
| 15585 | bark ironpine petrified | a chunk of petrified ironpine bark | 1 | 8 | 13 (trash) | take | — | Ironpine Rise flavor |
| 15586 | moss luminescent jar | a jar of luminescent moss | 1 | 3 | 1 (light) | take | — | Ancestor Deeps flavor |
| 15587 | token ashfang black | an Ashfang black token | 1 | 4 | 13 (trash) | take | — | Ashfang faction drop |
| 15588 | charm rootbound vine | a Rootbound vine charm | 1 | 2 | 13 (trash) | take | — | Rootbound cult drop |
| 15589 | stone cairn carved | a carved cairn stone | 1 | 6 | 13 (trash) | take | — | Ancestor Deeps flavor |

---

## Loot Table Plan

Loot tables connect specific mobs to their item drops. The `l` extension defines loot amount and item vnums; the `L` extension defines per-slot selection weights. Sum of all `L` values must be `<= 100`. All items referenced by loot tables must have `ITEM_LOOT` set. Boss mob items must also have `ITEM_BOSS` set.

### Region A: Southern Verge Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 15505 | grey timber wolf | 50 | 15579 (wolf hide), 15561 (wolf-fang earring) | 70, 30 |
| 15506 | wild forest boar | 50 | 15580 (boar fang) | 100 |
| 15507 | giant forest spider | 30 | 15578 (trail map) | 100 |
| 15508 | forest poacher | 80 | 15501 (hunting knife), 15525 (fur-lined hood), 15578 (trail map) | 40, 35, 25 |
| 15509 | masked highwayman | 100 | 15501 (hunting knife), 15516 (concealed dagger), 15562 (hidden belt) | 30, 40, 30 |
| 15513 | woodland bandit | 80 | 15516 (concealed dagger), 15525 (fur-lined hood) | 55, 45 |
| 15514 | young black bear | 50 | 15583 (bear claw) | 100 |
| 15515 | wolf pack runner | 40 | 15579 (wolf hide), 15561 (wolf-fang earring) | 75, 25 |

### Region B: Mosswater Bends Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 15525 | marsh water snake | 40 | 15581 (venom sac) | 100 |
| 15526 | giant fen frog | 50 | 15582 (frog skin) | 100 |
| 15528 | Mosswater crocodile | 80 | 15503 (fang javelin), 15527 (marsh boots) | 50, 50 |
| 15529 | bog lurker | 80 | 15503 (fang javelin), 15541 (oiled coat) | 55, 45 |
| 15530 | smuggler lookout | 100 | 15516 (concealed dagger), 15562 (hidden belt), 15587 (ashfang token) | 40, 35, 25 |
| 15531 | fen smuggler runner | 100 | 15516 (concealed dagger), 15502 (bone spear), 15562 (hidden belt) | 35, 35, 30 |
| 15534 | marsh will-o-wisp | 60 | 15523 (fen wisp wand) | 100 |
| 15537 | fen smuggler boss | 150 | 15516 (concealed dagger), 15502 (bone spear), 15562 (hidden belt), 15541 (oiled coat) | 25, 30, 20, 25 |

### Region C: Ironpine Rise Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 15543 | grey mountain wolf | 60 | 15579 (wolf hide), 15561 (wolf-fang earring) | 65, 35 |
| 15544 | mountain stalker cat | 60 | 15585 (ironpine bark) | 100 |
| 15545 | Ashfang scout | 100 | 15504 (blackened axe), 15528 (spiked pauldrons), 15553 (tooth necklace), 15587 (ashfang token) | 25, 25, 25, 25 |
| 15546 | Ashfang raider | 120 | 15504 (blackened axe), 15528 (spiked pauldrons), 15547 (chain skirt), 15553 (tooth necklace) | 25, 25, 25, 25 |
| 15547 | Everkeeper's shade | 80 | 15555 (planting-ring), 15510 (ghost blade) | 60, 40 |
| 15548 | stone-antler sentinel | 100 | 15505 (ritual blade), 15529 (bone helm), 15554 (circle torc) | 35, 35, 30 |
| 15549 | ironpine brown bear | 60 | 15583 (bear claw), 15585 (ironpine bark) | 60, 40 |
| 15550 | wolf pack alpha | 120 | 15579 (wolf hide), 15561 (wolf-fang earring), 15565 (bark ring) | 35, 35, 30 |
| 15552 | Ashfang trapper | 100 | 15504 (blackened axe), 15540 (iron bracers), 15587 (ashfang token) | 35, 35, 30 |
| 15555 | stone-antler archer | 100 | 15505 (ritual blade), 15529 (bone helm), 15554 (circle torc) | 30, 35, 35 |
| 15559 | Ashfang ridge shaman | 120 | 15504 (blackened axe), 15553 (tooth necklace), 15540 (iron bracers), 15517 (ironwood club) | 25, 25, 25, 25 |

### Region D: Ancestor Deeps Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 15561 | Cairn-Keeper ghost | 100 | 15510 (ghost blade), 15534 (ghost shield), 15556 (bone amulet), 15589 (cairn stone) | 25, 25, 25, 25 |
| 15562 | Rootbound acolyte | 100 | 15508 (thorn glaive), 15557 (vine bracelet), 15548 (vine cowl), 15588 (vine charm) | 25, 25, 25, 25 |
| 15563 | Rootbound ritualist | 120 | 15508 (thorn glaive), 15532 (living armor), 15557 (vine bracelet), 15548 (vine cowl) | 25, 25, 25, 25 |
| 15564 | ancestor dire wolf | 80 | 15579 (wolf hide), 15561 (wolf-fang earring) | 55, 45 |
| 15565 | ancient cave bear | 100 | 15583 (bear claw) | 100 |
| 15567 | Ashfang war-chief | 200 | 15509 (cleaver), 15533 (plate), 15553 (tooth necklace), 15517 (ironwood club) | 30, 30, 20, 20 |
| 15568 | Thornmother's lieutenant | 200 | 15508 (thorn glaive), 15532 (living armor), 15557 (vine bracelet), 15548 (vine cowl) | 30, 25, 25, 20 |
| 15569 | red-corrupted stag | 80 | 15588 (vine charm), 15585 (ironpine bark) | 55, 45 |
| 15571 | ancient ironpine treant | 150 | 15522 (treant staff), 15544 (moss cloak), 15585 (ironpine bark) | 35, 30, 35 |
| 15573 | deep Ashfang berserker | 120 | 15504 (blackened axe), 15533 (plate), 15540 (iron bracers) | 35, 35, 30 |

### Region E: Northern Crown Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 15581 | northern Ashfang berserker | 120 | 15504 (blackened axe), 15528 (spiked pauldrons), 15553 (tooth necklace), 15587 (ashfang token) | 25, 25, 25, 25 |
| 15584 | oathbreaker wraith | 150 | 15513 (cursed sword), 15537 (cursed gauntlets), 15558 (signet ring) | 35, 35, 30 |
| 15585 | **the Thornmother** | 300 | 15511 (briar scourge), 15518 (vine whip), 15535 (briar crown), 15559 (thorn circlet), 15550 (thorn robe) | 20, 20, 20, 20, 20 |
| 15586 | the White Stag | 100 | 15560 (antler crown), 15584 (white antler) | 50, 50 |
| 15587 | **the Primal Bear Lord** | 200 | 15514 (bear-claw fist), 15536 (bearhide greatshield), 15568 (bear-tooth amulet), 15583 (bear claw) | 30, 25, 25, 20 |
| 15589 | northern Ashfang warmaster | 200 | 15519 (warmaster greatsword), 15546 (warmaster vambraces), 15567 (iron crown), 15553 (tooth necklace) | 30, 25, 25, 20 |
| 15590 | Crown Rootbound cultist | 120 | 15508 (thorn glaive), 15557 (vine bracelet), 15548 (vine cowl), 15588 (vine charm) | 25, 25, 25, 25 |
| 15593 | elder stone-antler sentinel | 150 | 15520 (elder spear), 15549 (elder shield), 15554 (circle torc) | 35, 35, 30 |
| 15595 | cold ancestor spirit | 120 | 15566 (ancestor torque), 15556 (bone amulet), 15589 (cairn stone) | 35, 35, 30 |
| 15596 | Rootbound briar knight | 150 | 15521 (thorn pike), 15545 (thorn plate), 15557 (vine bracelet) | 35, 35, 30 |

---

## Reset Plan

### Mob Resets (M)

Mob resets place mobiles in specific rooms. Format: `M 0 <mob_vnum> <limit> <room_vnum>`. Limits control maximum concurrent instances.

#### Region A: Southern Verge
- `15500` (southern road warden): 3 resets across southern road rooms, limit 1 each
- `15501` (lamp-keeper): 2 resets at road bend rooms, limit 1 each
- `15502` (Woodwrights surveyor): 1 reset near-road, limit 1
- `15503` (caravan guard): 2 resets on road rooms, limit 1 each
- `15504` (traveling peddler): 1 reset roadside clearing, limit 1
- `15505` (timber wolf): 4 resets off-road, limit 2 each
- `15506` (wild boar): 3 resets off-road thickets, limit 2 each
- `15507` (giant spider): 2 resets dense canopy, limit 1 each
- `15508` (poacher): 2 resets off-road, limit 1 each
- `15509` (highwayman): 2 resets near-road ambush spots, limit 1 each
- `15510` (young stag): 2 resets forest glades, limit 1 each
- `15511` (red fox): 2 resets undergrowth, limit 1 each
- `15512` (copperhead): 2 resets rocky off-road, limit 1 each
- `15513` (woodland bandit): 2 resets off-road camps, limit 1 each
- `15514` (young black bear): 2 resets deep off-road, limit 1 each
- `15515` (wolf pack runner): 3 resets wolf territory, limit 2 each
- `15516` (southern patrol sergeant): 1 reset south gate area, limit 1
- `15517` (verge trapper): 1 reset trapping clearing, limit 1
- `15518` (great horned owl): 2 resets canopy rooms, limit 1 each
- `15519` (forest badger): 2 resets burrow rooms, limit 1 each

#### Region B: Mosswater Bends
- `15520` (mire-clan fisher): 2 resets reed-platform rooms, limit 1 each
- `15521` (mire-clan herbalist): 1 reset fen village, limit 1
- `15522` (fen toll collector): 1 reset Mosswater Toll Bridge, limit 1
- `15523` (reed-platform trader): 1 reset fen village, limit 1
- `15524` (Mosswater road warden): 2 resets road rooms Region B, limit 1 each
- `15525` (marsh water snake): 4 resets waterway rooms, limit 2 each
- `15526` (giant fen frog): 3 resets shallow water rooms, limit 2 each
- `15527` (territorial heron): 2 resets hummock rooms, limit 1 each
- `15528` (Mosswater crocodile): 2 resets deep channel rooms, limit 1 each
- `15529` (bog lurker): 2 resets deep fen rooms, limit 1 each
- `15530` (smuggler lookout): 2 resets off-road fen edges, limit 1 each
- `15531` (fen smuggler runner): 2 resets smuggler camps, limit 1 each
- `15532` (giant leech swarm): 2 resets stagnant water, limit 1 each
- `15533` (moss-backed snapping turtle): 2 resets stream crossings, limit 1 each
- `15534` (marsh will-o-wisp): 2 resets deep fog rooms, limit 1 each
- `15535` (electric eel): 1 reset deep water, limit 1
- `15536` (fen-grey wolf): 3 resets fen-edge hunting grounds, limit 2 each
- `15537` (fen smuggler boss): 1 reset deep smuggler camp, limit 1
- `15538` (venomous mud asp): 2 resets mudbank rooms, limit 1 each
- `15539` (mist-fisher crane): 2 resets open water, limit 1 each

#### Region C: Ironpine Rise
- `15540` (Ironpine ridge warden): 2 resets road rooms Region C, limit 1 each
- `15541` (Everkeeper hermit): 1 reset planting-ring room, limit 1
- `15542` (joint-commission scout): 2 resets road rooms, limit 1 each
- `15543` (grey mountain wolf): 4 resets ridge corridors, limit 2 each
- `15544` (mountain stalker cat): 2 resets cliffside rooms, limit 1 each
- `15545` (Ashfang scout): 3 resets off-road 3-4 rooms out, limit 1 each
- `15546` (Ashfang raider): 3 resets Ashfang camp rooms, limit 1 each
- `15547` (Everkeeper's shade): 2 resets abandoned planting-rings, limit 1 each
- `15548` (stone-antler sentinel): 2 resets near megalith rooms, limit 1 each
- `15549` (ironpine brown bear): 2 resets deep ridge rooms, limit 1 each
- `15550` (wolf pack alpha): 1 reset wolf den room, limit 1
- `15551` (ridge-hunter hawk): 2 resets exposed ridge rooms, limit 1 each
- `15552` (Ashfang trapper): 2 resets trap-line rooms, limit 1 each
- `15553` (tusked ridge boar): 2 resets rocky clearings, limit 1 each
- `15554` (ironpine rock viper): 2 resets boulder-field rooms, limit 1 each
- `15555` (stone-antler archer): 2 resets outer megalith rooms, limit 1 each
- `15556` (ridge wolverine): 2 resets deep ridge rooms, limit 1 each
- `15557` (ironpine trap-door spider): 2 resets root-hollow rooms, limit 1 each
- `15558` (ironpine shadow lynx): 2 resets dense canopy rooms, limit 1 each
- `15559` (Ashfang ridge shaman): 1 reset Ashfang ritual site, limit 1

#### Region D: Ancestor Deeps
- `15560` (Ancestor Deeps road warden): 2 resets road rooms Region D, limit 1 each
- `15561` (Cairn-Keeper ghost): 3 resets standing stone rooms, limit 1 each
- `15562` (Rootbound acolyte): 3 resets deep branch rooms, limit 1 each
- `15563` (Rootbound ritualist): 2 resets ritual clearing rooms, limit 1 each
- `15564` (ancestor dire wolf): 3 resets deep interior rooms, limit 1 each
- `15565` (ancient cave bear): 2 resets cavern-mouth rooms, limit 1 each
- `15566` (megalith spirit guardian): 2 resets megalith ring rooms, limit 1 each
- `15567` (Ashfang war-chief): 1 reset deep Ashfang camp, limit 1
- `15568` (Thornmother's lieutenant): 1 reset Rootbound outer grove, limit 1
- `15569` (red-corrupted stag): 2 resets red-moss bloom rooms, limit 1 each
- `15570` (megalith stone watcher): 2 resets inner megalith rooms, limit 1 each
- `15571` (ancient ironpine treant): 1 reset deepest ironpine grove, limit 1
- `15572` (coiled root serpent): 2 resets root-cavern rooms, limit 1 each
- `15573` (deep Ashfang berserker): 2 resets deep Ashfang territory, limit 1 each
- `15574` (pale ghost moth): 2 resets luminescent moss rooms, limit 1 each

#### Region E: Northern Crown / Kowloon Reach
- `15575` (northern customs officer): 1 reset Northern Customs Post, limit 1
- `15576` (Kowloon patrol warden): 3 resets road rooms Region E, limit 1 each
- `15577` (Courier Lantern runner): 2 resets road rooms Region E, limit 1 each
- `15578` (North Warden Camp captain): 1 reset North Warden Camp, limit 1
- `15579` (North Warden Camp guard): 2 resets North Warden Camp, limit 1 each
- `15580` (northern mist viper): 3 resets mist-valley rooms, limit 1 each
- `15581` (northern Ashfang berserker): 3 resets northern Ashfang camps, limit 1 each
- `15582` (crown bull elk): 2 resets northern ridgeline rooms, limit 1 each
- `15583` (frost-touched wolf): 3 resets cold-valley rooms, limit 2 each
- `15584` (oathbreaker wraith): 2 resets ruined oath-stele rooms, limit 1 each
- `15585` (the Thornmother): 1 reset hidden grove (no_mob room), limit 1
- `15586` (the White Stag): 1 reset Crown of Antlers apex, limit 1
- `15587` (the Primal Bear Lord): 1 reset Crown of Antlers apex, limit 1
- `15588` (mist-shadow cat): 2 resets mist-valley rooms, limit 1 each
- `15589` (northern Ashfang warmaster): 1 reset deepest Ashfang camp, limit 1
- `15590` (Crown Rootbound cultist): 3 resets northern cult rooms, limit 1 each
- `15591` (crown cow elk): 2 resets elk grazing rooms, limit 2 each
- `15592` (ice crown serpent): 2 resets cold-stream rooms, limit 1 each
- `15593` (elder stone-antler sentinel): 1 reset northern megalith room, limit 1
- `15594` (bone-crowned hawk): 2 resets exposed peak rooms, limit 1 each
- `15595` (cold ancestor spirit): 2 resets northern standing stones, limit 1 each
- `15596` (Rootbound briar knight): 2 resets Thornmother approach, limit 1 each
- `15597` (white ghost boar): 2 resets ghost-birch rooms, limit 1 each
- `15598` (ironpine mother bear): 1 reset northern den room, limit 1
- `15599` (Kowloon Warden of Iron): 2 resets near north gate rooms, limit 1 each

### Equipment Resets (G/E)

Non-hostile civic/patrol NPCs receive thematic equipment via `G` (give to inventory) or `E` (equip on wear slot) resets placed immediately after their `M` reset:

- **Road wardens** (`15500`, `15524`, `15540`, `15560`): `E` warden's short sword (`15500`) on hold, `E` warden's leather jerkin (`15524`) on body, `E` warden's lantern pendant (`15551`) on neck
- **Southern patrol sergeant** (`15516`): `E` Lantern Road warden's spear (`15515`) on hold, `E` Lantern Road patrol greaves (`15539`) on legs
- **Fen toll collector** (`15522`): `E` Lantern Road warden's spear (`15515`) on hold
- **Joint-commission scout** (`15542`): `E` warden's short sword (`15500`) on hold, `E` joint-commission sash (`15564`) on waist
- **North Warden Camp captain** (`15578`): `E` Lantern Road warden's spear (`15515`) on hold, `E` forest warden's patrol cloak (`15563`) on about
- **North Warden Camp guards** (`15579`): `E` warden's short sword (`15500`) on hold, `E` warden's leather jerkin (`15524`) on body
- **Kowloon patrol warden** (`15576`): `E` warden's short sword (`15500`) on hold, `E` warden's leather jerkin (`15524`) on body
- **Kowloon Warden of Iron** (`15599`): `E` Lantern Road warden's spear (`15515`) on hold, `E` Lantern Road patrol greaves (`15539`) on legs, `E` forest warden's patrol cloak (`15563`) on about

### Object Room Resets (O)

Flavor objects placed in specific rooms for atmosphere:

- `15574` (relic tithe ledger): 1-2 resets in ruined guard-hut rooms (Mosswater Bends)
- `15575` (Midgaard wax seal): 1 reset at South Lantern Gate (`15512`)
- `15576` (Kowloon thread-knot seal): 1 reset at North Lantern Gate (`15994`)
- `15577` (iron-banded road lantern): 3-4 resets at road bend/waystation rooms
- `15586` (luminescent moss jar): 2 resets in Ancestor Deeps luminescent rooms
- `15585` (petrified ironpine bark): 2 resets in Ironpine Rise rooms
- `15589` (carved cairn stone): 2 resets in Ancestor Deeps standing-stone rooms

### Door Resets (D)

As specified in **Door Plan**:
- `D 0 15512 2 1` — South Lantern Gate, south door, closed
- `D 0 15994 0 1` — North Lantern Gate, north door, closed
- `D 0 15673 1 1` — Fen Toll Bridge Gate, east door, closed (optional)
- `D 0 15794 1 1` — Ridge Pass Barricade, east door, closed (optional)
- `D 0 15893 1 1` — Ancestor Ring Ward Gate, east door, closed (optional)
- `D 0 15952 1 1` — Northern Customs Post, east door, closed (optional)

---

## NPC Spec Compliance Notes
All mobile definitions must follow these spec requirements:

- **All mobs** must have `stay_area` (`64`) set in `act` flags. No exceptions.
- **All mobs** must have `is_npc` (`1`) set in `act` flags.
- **Sentinel mobs** (roadwardens, lamp-keepers, shrine guardians, named NPCs): `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`).
- **Boss mobs** (if any): must have `sentinel` (`2`) + `boss` (`67108864`) + `stay_area` (`64`) and be placed only in rooms flagged `no_mob` (`4`).
- **Strong non-boss mobs** (e.g., Ashfang war-chief, Thornmother lieutenant, apex predators): must have `solo` (`33554432`) + `stay_area` (`64`).
- **Aggressive mobs** (feral beasts, hostile undead, Rootbound cultists, Ashfang raiders): set `aggressive` (`32`) in `act` flags.
- **Non-hostile mobs** (roadwardens, merchants, lamp-keepers, mire-clan traders): leave `aggressive` unset.
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. No multi-line `long_descr` is permitted.
- Mobile `description` must end with exactly one trailing newline before `~`.
- Vnums must never appear in any mobile description text (`long_descr` or `description`).

### NPC Level Targets
- **Road wardens and lamp-keepers:** level 30-50 (sentinel, scales with road region).
- **Checkpoint guards and patrol captains:** level 50-80 (sentinel at gates, stronger near Kowloon approach).
- **Mire-clan traders and forest workers:** level 20-35 (non-aggressive, sentinel).
- **Wildlife (wolves, boar, bears, snakes):** level 20-90 (aggressive, scaled by distance from road).
- **Ashfang warbands:** level 50-90 (aggressive, scaled by depth).
- **Rootbound cultists:** level 60-95 (aggressive, deep interior).
- **Stone-antler circle guardians:** level 55-85 (aggressive only in sacred sites, sentinel at megaliths).
- **Apex entities (Crown of Antlers):** level 95-100 (solo or boss, sentinel, farthest from road).

---

## Room Description Requirements (spec compliance)
- Every room `<description>~` must contain at least 3 sentences of text.
- Important rooms (South Lantern Gate, North Lantern Gate, Oath-Crossing Stones, Bent Mile Marker, Mosswater Toll Bridge, North Warden Camp) must contain at least 5 sentences.
- Every room description must be unique (no reused descriptions across rooms).
- Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
- When a room uses a named exit (non-empty `<exit_keyword>`), that exit name must appear in at least one of: the room's main `<description>~`, an object that spawns in the room, or an `E` extra description in the room.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main `<description>~`.
- Room descriptions must use the area's color theme intentionally and consistently.
- Each room's main `<description>~` must end with exactly one trailing newline immediately before the terminating `~`.

---

## Integration Notes (Midgaard south / Kowloon north)
- Southward narrative hooks reference Midgaard guild contracts, caravan escorts, ranger writs, and Granite Arcade administrative postings.
- Northward hooks reference Kowloon covenant law, Iron Gate customs processing, Jade Magistracy tariff checks, and Courier Lantern Office relay dispatches.
- Forest NPC dialogue should reflect dual influence: Midgaard practicality and Kowloon civic formalism.
- The area functions as both frontier adventure and geopolitical corridor.
- Road wardens carry identification recognized by both cities; their dual loyalty creates role-playing opportunities and faction tension.
- Lore references should cross-link: Kowloon's Ashfall Monsoon and the forest's Ash Winters share the same volcanic cause (Cinderteeth eruptions), binding the two areas' histories together.
- Kowloon's Iron Gate (`14095`) faces "the caravan roads and the lawless wetland frontier" per the Kowloon plan. The forest's northern approach should reflect this: the last few rooms before Kowloon should show increasing signs of Kowloon's administrative presence (customs markers, inspection posts, Wardens of Iron patrol signs).
- Midgaard's Granite Arcade (`3009`) is the administrative hub for northern trade. The forest's southern approach should show Midgaard's practical civic character: ranger notice boards, guild contract postings, and warden supply caches.

---

## Propositions

Static propositions offered through the three regional postmasters for quests set in or motivated by the Great Northern Forest. Proposition IDs 0-4 are already defined in `propositions/1-5.prop`. The propositions below use IDs 5-14 (files `6.prop` through `15.prop`). `PROP_MAX_STATIC_PROPOSITIONS` is assumed to be increased to `64`.

Postmaster vnums:
- **Midgaard:** `3015` (the Postmaster)
- **Kiess:** `13001` (the Postmaster of Kiess)
- **Kowloon:** `14021` (Postmaster of Kowloon)

### Non-Chain Propositions

These standalone propositions have no prerequisites (`prerequisite_static_id = -1`) and can be accepted independently.

#### Proposition 5 — Lantern Road wolf cull (Midgaard)

| Field | Value |
|---|---|
| **File** | `6.prop` |
| **Static ID** | 5 |
| **Title** | Lantern Road wolf cull |
| **prerequisite_static_id** | -1 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 10 |
| **min_level** | 20 |
| **max_level** | 39 |
| **offerer_vnum** | 3015 |
| **reward_gold** | 1400 |
| **reward_qp** | 2 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15505` (grey timber wolf) |
| **Accept message** | Midgaard's post office has received a stack of delayed delivery reports citing wolf packs along the southern Lantern Road approach. Rangers estimate the pack has split into multiple hunting parties that rotate ambush positions between road bends. Thin the population enough that courier runners can resume single-escort schedules instead of the current armed-convoy requirement. |
| **Completion message** | Wolf sightings along the southern approach have dropped to seasonal norms, and the post office has downgraded the Lantern Road threat assessment from convoy-only to standard escort. Courier runners are already making faster turnarounds, and the backlog of delayed parcels from the Granite Arcade sorting office is clearing for the first time in weeks. |

#### Proposition 6 — Mosswater smuggler interdiction (Kiess)

| Field | Value |
|---|---|
| **File** | `7.prop` |
| **Static ID** | 6 |
| **Title** | Mosswater smuggler interdiction |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 3 |
| **kill_needed** | 0 |
| **min_level** | 30 |
| **max_level** | 54 |
| **offerer_vnum** | 13001 |
| **reward_gold** | 2000 |
| **reward_qp** | 3 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15530 15531 15537` (smuggler lookout, fen smuggler runner, fen smuggler boss) |
| **Accept message** | Kiess dispatch has intercepted falsified manifests traced to a smuggling ring operating out of the Mosswater Bends. They use reed-platform camps to stage contraband before running it north along fen channels that bypass the Lantern Road checkpoints entirely. Kiess route clerks need the ring broken at three levels: their lookouts, their runners, and whoever is directing operations from the deep fen. |
| **Completion message** | The smuggling ring's command structure is shattered, and Kiess customs officers are already recovering cached contraband from the reed camps you identified. Fen toll collectors report that legitimate mire-clan traders are moving goods openly again instead of avoiding the channels out of fear of smuggler reprisal. The Kiess post has reopened direct fen-route courier service for the first time this season. |

#### Proposition 7 — Northern Crown predator survey (Kowloon)

| Field | Value |
|---|---|
| **File** | `8.prop` |
| **Static ID** | 7 |
| **Title** | Northern Crown predator survey |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 3 |
| **kill_needed** | 0 |
| **min_level** | 40 |
| **max_level** | 69 |
| **offerer_vnum** | 14021 |
| **reward_gold** | 2800 |
| **reward_qp** | 3 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15580 15583 15588` (northern mist viper, frost-touched wolf, mist-shadow cat) |
| **Accept message** | Kowloon's Wardens of Iron need current threat data from the Northern Crown approaches before they can adjust seasonal patrol rotations. Three predator types have been reported in densities that exceed safe caravan transit thresholds: mist vipers in the cold valleys, frost-touched wolves along the ridge corridors, and mist-shadow cats near the tree line. Engage one of each and bring back field assessment notes so the wardens can calibrate their next deployment. |
| **Completion message** | Your field reports give Kowloon's patrol command the data they needed to redraw the Northern Crown escort map. The Wardens of Iron have shifted two squads to cover the mist-valley corridors you flagged, and caravan masters are requesting the updated route charts from the Kowloon post office. Seasonal rotation planning can proceed on schedule instead of defaulting to the expensive all-hands deployment. |

#### Proposition 8 — Ironpine Rise Ashfang suppression (Midgaard)

| Field | Value |
|---|---|
| **File** | `9.prop` |
| **Static ID** | 8 |
| **Title** | Ironpine Rise Ashfang suppression |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 3 |
| **kill_needed** | 0 |
| **min_level** | 45 |
| **max_level** | 69 |
| **offerer_vnum** | 3015 |
| **reward_gold** | 3200 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15545 15546 15552` (Ashfang scout, Ashfang raider, Ashfang trapper) |
| **Accept message** | Midgaard's ledgerhouse has flagged a sharp increase in cargo losses through the Ironpine Rise, and ranger reports point to Ashfang warbands operating in coordinated cells: scouts mark targets, trappers slow caravans with deadfalls and snares, and raiders hit the stalled columns. The postmaster needs each cell type disrupted so ridge-route dispatches can resume without triple escort surcharges. |
| **Completion message** | Ashfang activity on the Ironpine Rise has fragmented into scattered opportunists now that their scout-trapper-raider coordination is broken. Ridge patrol wardens report clean passage windows on routes that had been effectively closed for weeks. Midgaard's post office has already re-listed the Ironpine route as standard-risk, and convoy insurance rates are dropping back to seasonal norms. |

### Chain Propositions

#### Rootbound Purge Chain (Kiess, 3 propositions)

A three-part chain investigating and dismantling Rootbound cult operations in the deep forest. Kiess route clerks drive this chain because Rootbound sabotage of road infrastructure directly threatens Kiess-Midgaard courier schedules.

##### Proposition 9 — Rootbound perimeter probe (chain start)

| Field | Value |
|---|---|
| **File** | `10.prop` |
| **Static ID** | 9 |
| **Title** | Rootbound perimeter probe |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 55 |
| **max_level** | 79 |
| **offerer_vnum** | 13001 |
| **reward_gold** | 3400 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15562 15563` (Rootbound acolyte, Rootbound ritualist) |
| **Accept message** | Kiess compact archives have correlated a pattern: every time Rootbound cult activity spikes in the Ancestor Deeps, bridge pilings and lamp-post footings along the Lantern Road show fresh sabotage within days. The clerks believe the cult's outer ring of acolytes and ritualists are the ones carrying out infrastructure attacks between their rites. Engage both tiers and recover any materials that confirm the sabotage-to-ritual pipeline. |
| **Completion message** | The materials you recovered confirm what Kiess archivists suspected: the Rootbound outer circle treats road sabotage as a devotional act, carving rune-marks into bridge timbers before weakening them. With their acolyte and ritualist cadres disrupted, the cult's ability to project sabotage beyond their deep-forest sanctuaries is severely diminished. Kiess road maintenance crews can repair the damage faster than the cult can replace its losses. |

##### Proposition 10 — Rootbound lieutenant strike (chain middle)

| Field | Value |
|---|---|
| **File** | `11.prop` |
| **Static ID** | 10 |
| **Title** | Rootbound lieutenant strike |
| **prerequisite_static_id** | 9 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 70 |
| **max_level** | 94 |
| **offerer_vnum** | 13001 |
| **reward_gold** | 5200 |
| **reward_qp** | 5 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15568 15596` (Thornmother's lieutenant, Rootbound briar knight) |
| **Accept message** | Your earlier probe forced the Rootbound cult to consolidate around its inner circle. Kiess intelligence now has positions on two key figures: a lieutenant who coordinates the Thornmother's field operations, and a briar knight who commands the cult's armed enforcers in the northern groves. Removing them will isolate the Thornmother from her remaining followers and open a path for the final strike. |
| **Completion message** | With the lieutenant and briar knight eliminated, the Rootbound cult's command chain between the Thornmother and her remaining followers is severed. Kiess field agents report that scattered cultists are retreating deeper into the forest instead of holding their former positions. The path to the Thornmother's inner sanctum is now exposed, though the cult's remaining defenses will not be trivial. |

##### Proposition 11 — Thornmother sanctum assault (chain end)

| Field | Value |
|---|---|
| **File** | `12.prop` |
| **Static ID** | 11 |
| **Title** | Thornmother sanctum assault |
| **prerequisite_static_id** | 10 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 1 |
| **min_level** | 85 |
| **max_level** | 170 |
| **offerer_vnum** | 13001 |
| **reward_gold** | 8000 |
| **reward_qp** | 7 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15585` (the Thornmother) |
| **Accept message** | The outer ring is broken and the inner circle is scattered. Kiess compact clerks have authorized the final warrant: engage the Thornmother herself in her hidden grove deep in the Ancestor Deeps. She is the living heart of the Rootbound cult, and Kowloon's Wardens of Iron have matched the Kiess bounty with their own standing order. End this threat before the cult can rebuild its chain of command. |
| **Completion message** | The Thornmother has fallen, and with her the Rootbound cult's claim to the deep forest. Kiess and Kowloon officials are already sending joint survey teams into the former cult territory to assess the damage and begin reclaiming sabotaged road sections. For the first time in years, the Ancestor Deeps are open to legitimate passage without cult interference. Both post offices have issued commendations naming your work as the decisive action. |

#### Ancestor Deeps Reclamation Chain (Kowloon, 3 propositions)

A three-part chain clearing the Ancestor Deeps of hostile factions threatening Kowloon's southern trade security. Kowloon's postmaster drives this chain because the deep-forest threats directly endanger the northern approach to the Iron Gate.

##### Proposition 12 — Cairn-Keeper exorcism (chain start)

| Field | Value |
|---|---|
| **File** | `13.prop` |
| **Static ID** | 12 |
| **Title** | Cairn-Keeper exorcism |
| **prerequisite_static_id** | -1 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 6 |
| **min_level** | 60 |
| **max_level** | 84 |
| **offerer_vnum** | 14021 |
| **reward_gold** | 4000 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15561` (Cairn-Keeper ghost) |
| **Accept message** | Kowloon's Jade Magistracy has received formal complaints from caravan masters who report that Cairn-Keeper ghosts in the Ancestor Deeps have become actively hostile to all traffic, not merely trespassers at the standing stones. The magistracy believes the recent Rootbound cult activity has disturbed the burial sites and agitated the spirits beyond their traditional territorial behavior. Reduce the ghost population to levels the wardens can manage through standard containment patrols. |
| **Completion message** | The Cairn-Keeper ghost activity has subsided to levels consistent with historical norms. Kowloon's wardens report that the remaining spirits have retreated to their standing stone sites instead of ranging across trade corridors. The Jade Magistracy has formally noted the action in their seasonal security ledger and adjusted the Ancestor Deeps threat classification downward. Caravan masters are accepting contracts through the deep route again. |

##### Proposition 13 — Ashfang war-chief elimination (chain middle)

| Field | Value |
|---|---|
| **File** | `14.prop` |
| **Static ID** | 13 |
| **Title** | Ashfang war-chief elimination |
| **prerequisite_static_id** | 12 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 75 |
| **max_level** | 99 |
| **offerer_vnum** | 14021 |
| **reward_gold** | 6000 |
| **reward_qp** | 5 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15567 15573` (Ashfang war-chief, deep Ashfang berserker) |
| **Accept message** | With the Cairn-Keeper spirits contained, Kowloon intelligence has confirmed that the Ashfang warbands exploited the ghost disruption to expand their deep-forest camps. Their war-chief has established a fortified position in the Ancestor Deeps, supported by berserker enforcers who control the approaches. Kowloon's Wardens of Iron cannot commit a full garrison sortie without leaving the Iron Gate undermanned. Remove the war-chief and his berserker guard to collapse the Ashfang deep-forest presence. |
| **Completion message** | The Ashfang war-chief's deep camp has been broken, and surviving warband members are scattering toward the Ironpine Rise where ridge patrol wardens can contain them. Kowloon's southern approach is measurably safer, and the Wardens of Iron have redeployed the emergency reserve back to standard rotation. The postmaster has flagged your action in the next dispatch to Kowloon's Harbor Syndics as evidence that the forest route remains commercially viable. |

##### Proposition 14 — Oathbreaker wraith banishment (chain end)

| Field | Value |
|---|---|
| **File** | `15.prop` |
| **Static ID** | 14 |
| **Title** | Oathbreaker wraith banishment |
| **prerequisite_static_id** | 13 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 80 |
| **max_level** | 170 |
| **offerer_vnum** | 14021 |
| **reward_gold** | 7500 |
| **reward_qp** | 6 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `15584 15595` (oathbreaker wraith, cold ancestor spirit) |
| **Accept message** | The living threats are contained, but the deepest corruption remains: oathbreaker wraiths and cold ancestor spirits that haunt the Northern Crown's ruined oath-steles. Kowloon's covenant tradition considers these entities an affront to the compact that binds both cities. The wraiths are the spirits of those who violated the original forest charter, and their continued presence poisons the ground where Midgaard and Kowloon first swore mutual obligation. Banish them so the oath-stele sites can be reconsecrated. |
| **Completion message** | The oathbreaker wraiths have been banished and the ancestor spirits have been laid to rest. Kowloon's covenant scholars report that the oath-stele sites are already showing signs of recovery, with the oppressive cold lifting and the corruption receding from the surrounding stones. The Jade Magistracy has authorized a formal reconsecration ceremony, and both cities' wardens have been invited to witness the renewal of the original forest charter marks. The northern forest passage is as secure as it has been in living memory. |

### Proposition Summary Table

| File | Static ID | Title | Prereq ID | Type | Targets | Kill Needed | Level Range | Offerer | Rewards | Chain |
|---|---:|---|---:|---:|---|---:|---|---:|---|---|
| `6.prop` | 5 | Lantern Road wolf cull | -1 | 3 | `15505` | 10 | 20-39 | 3015 | 1400 gold, 2 qp | — |
| `7.prop` | 6 | Mosswater smuggler interdiction | -1 | 1 | `15530 15531 15537` | 0 | 30-54 | 13001 | 2000 gold, 3 qp | — |
| `8.prop` | 7 | Northern Crown predator survey | -1 | 1 | `15580 15583 15588` | 0 | 40-69 | 14021 | 2800 gold, 3 qp | — |
| `9.prop` | 8 | Ironpine Rise Ashfang suppression | -1 | 1 | `15545 15546 15552` | 0 | 45-69 | 3015 | 3200 gold, 4 qp | — |
| `10.prop` | 9 | Rootbound perimeter probe | -1 | 1 | `15562 15563` | 0 | 55-79 | 13001 | 3400 gold, 4 qp | Rootbound 1/3 |
| `11.prop` | 10 | Rootbound lieutenant strike | 9 | 1 | `15568 15596` | 0 | 70-94 | 13001 | 5200 gold, 5 qp | Rootbound 2/3 |
| `12.prop` | 11 | Thornmother sanctum assault | 10 | 3 | `15585` | 1 | 85-170 | 13001 | 8000 gold, 7 qp | Rootbound 3/3 |
| `13.prop` | 12 | Cairn-Keeper exorcism | -1 | 3 | `15561` | 6 | 60-84 | 14021 | 4000 gold, 4 qp | Reclamation 1/3 |
| `14.prop` | 13 | Ashfang war-chief elimination | 12 | 1 | `15567 15573` | 0 | 75-99 | 14021 | 6000 gold, 5 qp | Reclamation 2/3 |
| `15.prop` | 14 | Oathbreaker wraith banishment | 13 | 1 | `15584 15595` | 0 | 80-170 | 14021 | 7500 gold, 6 qp | Reclamation 3/3 |

### Proposition Design Notes

- **Postmaster distribution:** 2 Midgaard (IDs 5, 8), 4 Kiess (IDs 6, 9, 10, 11), 4 Kowloon (IDs 7, 12, 13, 14).
- **Kiess dominance in Rootbound chain:** Kiess route clerks and compact archives drive the cult investigation because Rootbound sabotage targets road infrastructure that Kiess's courier network depends on. The chain escalates from outer-ring disruption through inner-circle strikes to the boss encounter.
- **Kowloon dominance in Reclamation chain:** Kowloon's covenant tradition and Jade Magistracy security concerns drive the deep-forest clearing because the Ancestor Deeps threats directly endanger Kowloon's southern trade approaches. The chain escalates from undead containment through Ashfang power removal to spiritual purification.
- **Midgaard standalone focus:** Midgaard's two propositions address immediate practical threats (wolf overpopulation, Ashfang raiding) that affect road commerce, consistent with Midgaard's pragmatic civic character.
- **Level band coverage:** The 10 propositions span levels 20-170, with non-chain propositions covering the lower bands (20-69) and chain propositions escalating through mid-to-high bands (55-170).
- **Type variety:** 3 kill-count propositions (IDs 5, 11, 12), 7 kill-variety propositions (IDs 6, 7, 8, 9, 10, 13, 14).
- **All target vnums are mobs defined in this area plan** (15500-15599 mob range).
- **Reward scaling** follows the pattern established by existing propositions 0-4: gold and qp increase with level range and chain depth.
- **`PROP_MAX_STATIC_PROPOSITIONS`** must be increased from `16` to `64` to support IDs beyond 15 in future areas.

---

## Compliance Checklist
- [x] Plan only (no area file generation)
- [x] Uses `docs/area_file_spec.md` constraints in planning
- [x] 500-room plan with all vnums used (`15500-15999`)
- [x] Irregular overall layout with explicit meandering road
- [x] Multi-color theme defined (no `@@k`, no background codes, no flashing)
- [x] Extensive lore connecting Midgaard (south) and Kowloon (north)
- [x] Doors explicitly specified with `EX_ISDOOR` bit, reset states, and key vnums
- [x] Room layout included (full 20x25 vnum grid)
- [x] Road mobs around level 20 in south approach; stronger farther away
- [x] Farthest pockets cap around level 100
- [x] Mob containment strategy enforced (`sentinel` and/or `no_mob` boundaries; all mobs get `stay_area`)
- [x] South connection identified: `15512` south to Midgaard `3009` (Granite Arcade)
- [x] North connection identified: `15994` north to Kowloon `14095` (Iron Gate)
- [x] Road adjacency verified: all consecutive road rooms are cardinally adjacent
- [x] NPC spec compliance notes included (act flags, long_descr/description format, boss/solo policy)
- [x] Room description requirements section included
- [x] Area header fully spec-compliant (`Q 16`, `O Virant~`, vnum bounds, level band)
- [x] 80 mob definitions with act flags, levels, alignments, archetypes, and spawn concepts
- [x] 90 object definitions with item types, wear flags, extra flags, weights, and levels
- [x] Loot tables for all combat mobs with valid slot weights (sum <= 100)
- [x] All loot items flagged `ITEM_LOOT`; boss items also flagged `ITEM_BOSS`
- [x] Specials plan with valid spec_fun assignments (no spec_summon_, no spec_keep)
- [x] Equipment resets for civic/patrol NPCs
- [x] Object room resets for atmosphere items
- [x] Unique object names within area (no duplicates)
- [x] Weapon value3 thematically consistent with weapon concept
- [x] Two-handed weapons flagged `ITEM_TWO_HANDED`; fist weapon flagged `ITEM_FIST` with value3=0
- [x] Object weight ranges match archetype (1-5 caster, 6-10 melee, 11-15 tank)
- [x] Propositions section with 10 static propositions (IDs 5-14) across three postmasters
- [x] Mix of non-chain (4) and chain (6, in two 3-part chains) propositions
- [x] All proposition target vnums reference mobs defined in this area plan
