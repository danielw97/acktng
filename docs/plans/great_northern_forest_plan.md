# Area Plan: The Great Northern Forest

## Overview
- **Area Name:** `@@GThe @@lGreat @@gNorthern @@rForest@@N`
- **File Name:** `great_northern_forest.are`
- **Keyword:** `greatnorthernforest`
- **Intended Levels:** 1-5 (all player levels)
- **Recommended Active Combat Band:** 20-100 in this area (higher-level progression continues into Kowloon and surrounding high-tier zones)
- **Vnum Range:** `3739-4238`
- **Vnum Count:** 500 rooms (all room vnums used)
- **Shape:** Irregular, meandering wilderness with loops, side pockets, and escalating danger bands away from the road
- **South Connection:** Room `3751` south exit connects to Midgaard room `925` (Granite Arcade, northern wall spine)
- **North Connection:** Room `4233` north exit connects to Kowloon room `3534` (Iron Gate, southern wall)
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
- `L @@W(@@r1 5@@W)@@N~`
- `N <allocate at implementation time>`
- `I 1 5`
- `V 3739 4238`
- `F 15`
- `U @@gmist settles between the old pines as the Great Northern Forest stirs@@N~`
- `O Virant~`
- Optional builder flags as needed (`T`, etc.)

---

## Macro Layout (Irregular by Design)
The 500 rooms are arranged as an **irregular 20x25 coordinate envelope** (x=0..19, y=0..24), but connectivity is non-grid: loops, deadwood pockets, branching tributaries, and offset subregions.

- **All vnums `3739-4238` are rooms in this area plan**.
- Coordinates are planning coordinates only; final exits intentionally avoid full cardinal mesh.
- The Lantern Road weaves across the map, never remaining straight for more than 3-4 rooms.

### Coordinate-to-Vnum Formula
`vnum = 3739 + (y * 20) + x`

Example:
- `(x=0,y=0) => 3739`
- `(x=12,y=0) => 3751` (south link to Midgaard `925`)
- `(x=14,y=24) => 4233` (north link to Kowloon `3534`)

---

## 20x25 Room Layout Grid (all 500 vnums)

| y\\x | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 00 | 3739 | 3740 | 3741 | 3742 | 3743 | 3744 | 3745 | 3746 | 3747 | 3748 | 3749 | 3750 | 3751 | 3752 | 3753 | 3754 | 3755 | 3756 | 3757 | 3758 |
| 01 | 3759 | 3760 | 3761 | 3762 | 3763 | 3764 | 3765 | 3766 | 3767 | 3768 | 3769 | 3770 | 3771 | 3772 | 3773 | 3774 | 3775 | 3776 | 3777 | 3778 |
| 02 | 3779 | 3780 | 3781 | 3782 | 3783 | 3784 | 3785 | 3786 | 3787 | 3788 | 3789 | 3790 | 3791 | 3792 | 3793 | 3794 | 3795 | 3796 | 3797 | 3798 |
| 03 | 3799 | 3800 | 3801 | 3802 | 3803 | 3804 | 3805 | 3806 | 3807 | 3808 | 3809 | 3810 | 3811 | 3812 | 3813 | 3814 | 3815 | 3816 | 3817 | 3818 |
| 04 | 3819 | 3820 | 3821 | 3822 | 3823 | 3824 | 3825 | 3826 | 3827 | 3828 | 3829 | 3830 | 3831 | 3832 | 3833 | 3834 | 3835 | 3836 | 3837 | 3838 |
| 05 | 3839 | 3840 | 3841 | 3842 | 3843 | 3844 | 3845 | 3846 | 3847 | 3848 | 3849 | 3850 | 3851 | 3852 | 3853 | 3854 | 3855 | 3856 | 3857 | 3858 |
| 06 | 3859 | 3860 | 3861 | 3862 | 3863 | 3864 | 3865 | 3866 | 3867 | 3868 | 3869 | 3870 | 3871 | 3872 | 3873 | 3874 | 3875 | 3876 | 3877 | 3878 |
| 07 | 3879 | 3880 | 3881 | 3882 | 3883 | 3884 | 3885 | 3886 | 3887 | 3888 | 3889 | 3890 | 3891 | 3892 | 3893 | 3894 | 3895 | 3896 | 3897 | 3898 |
| 08 | 3899 | 3900 | 3901 | 3902 | 3903 | 3904 | 3905 | 3906 | 3907 | 3908 | 3909 | 3910 | 3911 | 3912 | 3913 | 3914 | 3915 | 3916 | 3917 | 3918 |
| 09 | 3919 | 3920 | 3921 | 3922 | 3923 | 3924 | 3925 | 3926 | 3927 | 3928 | 3929 | 3930 | 3931 | 3932 | 3933 | 3934 | 3935 | 3936 | 3937 | 3938 |
| 10 | 3939 | 3940 | 3941 | 3942 | 3943 | 3944 | 3945 | 3946 | 3947 | 3948 | 3949 | 3950 | 3951 | 3952 | 3953 | 3954 | 3955 | 3956 | 3957 | 3958 |
| 11 | 3959 | 3960 | 3961 | 3962 | 3963 | 3964 | 3965 | 3966 | 3967 | 3968 | 3969 | 3970 | 3971 | 3972 | 3973 | 3974 | 3975 | 3976 | 3977 | 3978 |
| 12 | 3979 | 3980 | 3981 | 3982 | 3983 | 3984 | 3985 | 3986 | 3987 | 3988 | 3989 | 3990 | 3991 | 3992 | 3993 | 3994 | 3995 | 3996 | 3997 | 3998 |
| 13 | 3999 | 4000 | 4001 | 4002 | 4003 | 4004 | 4005 | 4006 | 4007 | 4008 | 4009 | 4010 | 4011 | 4012 | 4013 | 4014 | 4015 | 4016 | 4017 | 4018 |
| 14 | 4019 | 4020 | 4021 | 4022 | 4023 | 4024 | 4025 | 4026 | 4027 | 4028 | 4029 | 4030 | 4031 | 4032 | 4033 | 4034 | 4035 | 4036 | 4037 | 4038 |
| 15 | 4039 | 4040 | 4041 | 4042 | 4043 | 4044 | 4045 | 4046 | 4047 | 4048 | 4049 | 4050 | 4051 | 4052 | 4053 | 4054 | 4055 | 4056 | 4057 | 4058 |
| 16 | 4059 | 4060 | 4061 | 4062 | 4063 | 4064 | 4065 | 4066 | 4067 | 4068 | 4069 | 4070 | 4071 | 4072 | 4073 | 4074 | 4075 | 4076 | 4077 | 4078 |
| 17 | 4079 | 4080 | 4081 | 4082 | 4083 | 4084 | 4085 | 4086 | 4087 | 4088 | 4089 | 4090 | 4091 | 4092 | 4093 | 4094 | 4095 | 4096 | 4097 | 4098 |
| 18 | 4099 | 4100 | 4101 | 4102 | 4103 | 4104 | 4105 | 4106 | 4107 | 4108 | 4109 | 4110 | 4111 | 4112 | 4113 | 4114 | 4115 | 4116 | 4117 | 4118 |
| 19 | 4119 | 4120 | 4121 | 4122 | 4123 | 4124 | 4125 | 4126 | 4127 | 4128 | 4129 | 4130 | 4131 | 4132 | 4133 | 4134 | 4135 | 4136 | 4137 | 4138 |
| 20 | 4139 | 4140 | 4141 | 4142 | 4143 | 4144 | 4145 | 4146 | 4147 | 4148 | 4149 | 4150 | 4151 | 4152 | 4153 | 4154 | 4155 | 4156 | 4157 | 4158 |
| 21 | 4159 | 4160 | 4161 | 4162 | 4163 | 4164 | 4165 | 4166 | 4167 | 4168 | 4169 | 4170 | 4171 | 4172 | 4173 | 4174 | 4175 | 4176 | 4177 | 4178 |
| 22 | 4179 | 4180 | 4181 | 4182 | 4183 | 4184 | 4185 | 4186 | 4187 | 4188 | 4189 | 4190 | 4191 | 4192 | 4193 | 4194 | 4195 | 4196 | 4197 | 4198 |
| 23 | 4199 | 4200 | 4201 | 4202 | 4203 | 4204 | 4205 | 4206 | 4207 | 4208 | 4209 | 4210 | 4211 | 4212 | 4213 | 4214 | 4215 | 4216 | 4217 | 4218 |
| 24 | 4219 | 4220 | 4221 | 4222 | 4223 | 4224 | 4225 | 4226 | 4227 | 4228 | 4229 | 4230 | 4231 | 4232 | 4233 | 4234 | 4235 | 4236 | 4237 | 4238 |

---

## Irregular Region Partition and Progression Bands

### Region A: Southern Verge / Midgaard March (3739-3858, 120 rooms)
- Closest to Midgaard; patrol camps, hunter tracks, shrines, and safer edges.
- Lantern Road begins at `3751` and drifts northwest then northeast before resuming northward.
- **Road mobs:** level 18-26 (target ~20).
- **Off-road mobs:** level 24-38.

### Region B: Mosswater Bends (3859-3958, 100 rooms)
- Wet hollows, streams, and fen bridges.
- Multiple side loops with reed villages and trapped beast dens.
- **Road mobs:** level 22-32.
- **Off-road mobs:** level 35-55.

### Region C: Ironpine Rise (3959-4058, 100 rooms)
- Higher ridges, wolf-clan hunting grounds, abandoned watch cairns.
- Irregular switchbacks and cliffside connectors.
- **Road mobs:** level 28-40.
- **Off-road mobs:** level 48-72.

### Region D: Ancestor Deeps (4059-4158, 100 rooms)
- Megalith rings, spirit groves, and root-cavern mouths.
- Dense branch structure intentionally far from road.
- **Road mobs:** level 35-50.
- **Off-road mobs:** level 65-90.

### Region E: Northern Crown / Kowloon Reach (4159-4238, 80 rooms)
- Final approach toward Kowloon contact zone.
- Cold mist valleys, oath-steles, and elite hostile pockets.
- **Road mobs:** level 38-55.
- **Farthest interior apex pockets:** level 95-100 cap.

> Design rule enforced: the **highest mobs (~100)** are at the **greatest graph distance from Lantern Road**, not merely by north/south position.

---

## Lantern Road Meander Plan (primary route)

Main road sequence (south to north), deliberately winding:

`3751 -> 3750 -> 3770 -> 3769 -> 3789 -> 3790 -> 3810 -> 3811 -> 3831 -> 3851 -> 3852 -> 3872 -> 3892 -> 3891 -> 3911 -> 3912 -> 3932 -> 3952 -> 3951 -> 3971 -> 3991 -> 3992 -> 4012 -> 4013 -> 4033 -> 4053 -> 4052 -> 4072 -> 4073 -> 4093 -> 4113 -> 4112 -> 4132 -> 4152 -> 4151 -> 4171 -> 4191 -> 4192 -> 4212 -> 4213 -> 4233`

Road adjacency verification (all consecutive pairs are cardinally adjacent on the 20x25 grid):
- `3751(12,0)` west to `3750(11,0)` north to `3770(11,1)` west to `3769(10,1)` north to `3789(10,2)` east to `3790(11,2)` ... continuing with verified cardinal adjacency through each step ... north to `4213(14,23)` north to `4233(14,24)`.

- Connects south to Midgaard via `3751` south exit to Midgaard room `925` (Granite Arcade).
- Connects north to Kowloon via `4233` north exit to Kowloon room `3534` (Iron Gate).
- Never forms a straight uninterrupted cardinal strip.
- Road-adjacent service clearings branch 1-3 steps off this chain.

---

## Connection Plan (external links)

### South Connection: Midgaard
- **Forest room:** `3751` (South Lantern Gate, coordinate x=12, y=0)
- **Midgaard room:** `925` (Granite Arcade, northernmost point of Midgaard's main north-south spine)
- **Exit:** `3751` has `D2` (south) exit to `925`. Midgaard `925` gets `D0` (north) exit to `3751`.
- **Door:** Door flag (`EX_ISDOOR`) set on the `3751` south exit. Reverse exit on `925` north must also have door flag set.
- **Reset:** `D 0 3751 2 1` (south door, state closed). `D 0 925 0 1` on Midgaard's side (added when forest is implemented).
- **Key:** `-1` (no key; closed but not locked).
- **Lore:** The South Lantern Gate marks the formal boundary where Midgaard's cleared approaches end and the forest canopy begins. A pair of iron-banded lamp posts flanks the road, their bases carved with Midgaard runes and the split-pine motif of Ilren Voss.

### North Connection: Kowloon
- **Forest room:** `4233` (North Lantern Gate, coordinate x=14, y=24)
- **Kowloon room:** `3534` (Iron Gate, southern wall of Kowloon, per `docs/kowloon_plan.md`)
- **Exit:** `4233` has `D0` (north) exit to `3534`. Kowloon `3534` has `D2` (south) exit to `4233`.
- **Door:** Door flag (`EX_ISDOOR`) set on the `4233` north exit. Reverse exit on `3534` south already has door flag set per Kowloon plan.
- **Reset:** `D 0 4233 0 1` (north door, state closed). Kowloon's `D 0 3534 2 1` handles the reverse.
- **Key:** `-1` (no key; closed but not locked).
- **Lore:** The North Lantern Gate stands where the forest's last ironpines give way to Kowloon's cleared customs approach. A Kowloon-standard chalkboard lists current tariff rates and caravan inspection hours. The gate posts bear both the lantern-and-scale motif and Kowloon's Wardens of Iron insignia.

### West Connections: Cinderteeth Mountains (3 border links — Planned)

The GNF's northwestern high-altitude edge (x=0 column, y=22–24, Northern Crown region) directly borders the Cinderteeth Mountains' eastern high-altitude approaches (Zone 7, Ember Crown Approaches). These three connections form a high-altitude border traverse. All are undoored open-air ridgeline passages (no door flag). Both sides must be authored simultaneously when `cinderteeth_mountains.are` is built.

| GNF Room | Coord | Dir | Cinderteeth Room | Character |
|---|---|---|---|---|
| **4179** (Northwestern Crown Ridgeline South) | x=0, y=22 | `west` | **6627** Northeastern Ridge Approach — Lower | Lower altitude ridgeline traverse |
| **4199** (Northwestern Crown Ridgeline Mid) | x=0, y=23 | `west` | **6633** Northeastern Ridge Approach — Mid | Mid-altitude ridgeline traverse |
| **4219** (Northwestern Crown — Westernmost Point) | x=0, y=24 | `west` | **6639** Northeastern Ridge Approach — Upper | Highest altitude; most exposed link |

- **Exit format (each):** GNF room has `D3` (west) exit, `0 -1 <cinderteeth_vnum>`. Cinderteeth room has `D1` (east) exit, `0 -1 <gnf_vnum>`. No door bits, no key.
- **Lore anchor:** These rooms sit above the treeline on the rocky northwestern Crown shoulder. The volcanic ash coating on the eastern face (visible from all three rooms) marks the Ashfall Monsoon's reach into the GNF ecosystem. Ranger-writ expedition markers driven into the western-face rocks by GNF patrol teams are discoverable in extra descriptions.

---

## Door Plan (explicit, spec-compliant)
Only major political boundary gates and select road checkpoints use doors; interior forest paths are undoored natural movement.

### Perimeter Gates

1. **South Lantern Gate** (`3751` south exit to Midgaard `925`)
  - `D2` (south) exit: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
  - `#RESETS` command `D`: `D 0 3751 2 1` (door south, state `closed`).
  - Reverse exit on Midgaard `925` (north) must also have `door` bit set.

2. **North Lantern Gate** (`4233` north exit to Kowloon `3534`)
  - `D0` (north) exit: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
  - `#RESETS` command `D`: `D 0 4233 0 1` (door north, state `closed`).
  - Reverse exit on Kowloon `3534` (south) already has `door` bit set per Kowloon plan.

### Optional Internal Checkpoint Doors (if desired for pacing, max 4)

3. **Fen Toll Bridge Gate** (`3912` east exit `<->` `3913` west exit)
  - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
  - `#RESETS`: `D 0 3912 1 1` (east door, state closed).
  - Note: `3912` is a road room (x=13, y=8); `3913` (x=14, y=8) is adjacent east.

4. **Ridge Pass Barricade** (`4033` east exit `<->` `4034` west exit)
  - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
  - `#RESETS`: `D 0 4033 1 1` (east door, state closed).
  - Note: `4033` is a road room (x=14, y=14); `4034` (x=15, y=14) is adjacent east.

5. **Ancestor Ring Ward Gate** (`4132` east exit `<->` `4133` west exit)
  - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
  - `#RESETS`: `D 0 4132 1 1` (east door, state closed).
  - Note: `4132` is a road room (x=13, y=19); `4133` (x=14, y=19) is adjacent east.

6. **Northern Customs Post** (`4191` east exit `<->` `4192` west exit)
  - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
  - `#RESETS`: `D 0 4191 1 1` (east door, state closed).
  - Note: `4191` and `4192` are consecutive road rooms (x=12/13, y=22). This checkpoint gates passage along the road itself.

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
- **Elite apex mobs (95-100):** sentinel or tiny no_mob cages in farthest nodes (e.g., deep branches off `4205-4208`, `4229-4232` interior pockets).
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
- `3751` **South Lantern Gate** (Midgaard link to `925`)
- `3912` **Mosswater Toll Bridge**
- `3991` **Ilren's Bent Mile Marker** (historic roadbuilder monument)
- `4132` **Oath-Crossing Stones** (Midgaard-Kowloon covenant marker)
- `4171` **North Warden Camp**
- `4233` **North Lantern Gate** (Kowloon link to `3534`)
- `4236-4238` **Crown of Antlers** (farthest apex wilderness)

These anchors tie lore directly to traversal and level escalation.

---

## Mobile Definitions (80 mobs)

All mobs in this area use vnums `3739-3818` (mob index). Cross-type overlap with room vnums is allowed per spec. All mobs carry `is_npc` (`1`) + `stay_area` (`64`) in act flags. Alignment ranges: good `500-291`, neutral `-35 to 35`, evil `-291 to -500`. Combat archetype (melee/hybrid/caster) determines which skill floor table applies per `area_file_spec.md` section 4.1.a.

### Region A: Southern Verge Mobs (3739-3758)

#### Non-Hostile / Civic

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3739 | warden road southern | the southern road warden | 30 | 1 (M) | 228 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms, Region A |
| 3740 | lampkeeper lamp keeper | the lamp-keeper | 25 | 2 (F) | 600 | is_npc, sentinel, stay_area | melee | Lamp maintenance | Road bend rooms |
| 3741 | surveyor timber woodwrights | the Woodwrights Guild surveyor | 28 | 1 (M) | 500 | is_npc, sentinel, stay_area | melee | Timber assessment | Near-road rooms |
| 3742 | guard caravan southern | a southern caravan guard | 32 | 1 (M) | 600 | is_npc, stay_area | melee | Caravan escort | Road rooms |
| 3743 | peddler forest traveling | a traveling forest peddler | 20 | 2 (F) | 400 | is_npc, sentinel, stay_area | — | Flavor NPC | Roadside clearings |

#### Hostile / Wildlife

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3744 | wolf timber grey | a grey timber wolf | 22 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Off-road, 1-2 rooms from path |
| 3745 | boar wild forest | a wild forest boar | 20 | 0 | 0 | is_npc, aggressive, stay_area | melee | Territorial beast | Off-road thickets |
| 3746 | spider forest giant | a giant forest spider | 24 | 0 | -35 | is_npc, aggressive, stay_area | melee | Ambush predator | Dense canopy rooms |
| 3747 | poacher forest | a forest poacher | 26 | 1 (M) | -350 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Off-road, 2-3 rooms out |
| 3748 | highwayman masked | a masked highwayman | 30 | 1 (M) | -500 | is_npc, aggressive, stay_area, remember | melee | Road bandit | Near-road ambush spots |
| 3749 | deer stag young | a young stag | 18 | 1 (M) | 0 | is_npc, stay_area | — | Wildlife flavor | Forest glades |
| 3750 | fox red forest | a red forest fox | 18 | 0 | 0 | is_npc, stay_area, wimpy | — | Wildlife flavor | Undergrowth rooms |
| 3751 | snake copperhead | a copperhead snake | 20 | 0 | 0 | is_npc, aggressive, stay_area | melee | Minor predator | Rocky off-road |
| 3752 | bandit woodland | a woodland bandit | 28 | 2 (F) | -450 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Off-road camps |
| 3753 | bear black young | a young black bear | 26 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Deep off-road |
| 3754 | wolf pack runner | a wolf pack runner | 24 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fast predator | Wolf territory rooms |
| 3755 | patrol sergeant southern | the southern patrol sergeant | 38 | 1 (M) | 178 | is_npc, sentinel, stay_area | melee | Road authority | South gate area |
| 3756 | hunter trapper verge | a verge trapper | 22 | 1 (M) | 35 | is_npc, sentinel, stay_area | — | Flavor NPC | Trapping clearings |
| 3757 | owl horned great | a great horned owl | 20 | 0 | 0 | is_npc, stay_area | — | Wildlife flavor | Canopy rooms |
| 3758 | badger forest | a forest badger | 22 | 0 | 0 | is_npc, aggressive, stay_area | melee | Minor predator | Burrow rooms |

### Region B: Mosswater Bends Mobs (3759-3778)

#### Non-Hostile / Civic

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3759 | fisher mire clan | a mire-clan fisher | 25 | 1 (M) | 300 | is_npc, sentinel, stay_area | — | Flavor NPC | Reed-platform rooms |
| 3760 | herbalist mire clan | a mire-clan herbalist | 28 | 2 (F) | 400 | is_npc, sentinel, stay_area | — | Flavor/trade NPC | Fen village rooms |
| 3761 | collector toll fen | the fen toll collector | 35 | 1 (M) | 500 | is_npc, sentinel, stay_area | melee | Checkpoint NPC | Mosswater Toll Bridge |
| 3762 | trader reed platform | a reed-platform trader | 30 | 2 (F) | 300 | is_npc, sentinel, stay_area | — | Flavor/trade NPC | Fen village rooms |
| 3763 | warden road mosswater | the Mosswater road warden | 35 | 2 (F) | 178 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms, Region B |

#### Hostile / Wildlife

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3764 | snake marsh water | a marsh water snake | 28 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Waterway rooms |
| 3765 | frog giant fen | a giant fen frog | 30 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Shallow water rooms |
| 3766 | heron territorial giant | a giant territorial heron | 32 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Hummock rooms |
| 3767 | crocodile mosswater | a Mosswater crocodile | 38 | 0 | 0 | is_npc, aggressive, stay_area | melee | Apex fen predator | Deep channel rooms |
| 3768 | lurker bog | a bog lurker | 42 | 0 | -300 | is_npc, aggressive, stay_area | melee | Ambush predator | Deep fen rooms |
| 3769 | smuggler lookout | a smuggler lookout | 35 | 1 (M) | -500 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Off-road fen edges |
| 3770 | smuggler runner fen | a fen smuggler runner | 38 | 2 (F) | -500 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Smuggler camps |
| 3771 | leech swarm giant | a giant leech swarm | 30 | 0 | 0 | is_npc, aggressive, stay_area, no_body | melee | Fen hazard | Stagnant water rooms |
| 3772 | turtle snapping moss | a moss-backed snapping turtle | 34 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Stream crossing rooms |
| 3773 | wisp will marsh | a marsh will-o-wisp | 36 | 0 | -400 | is_npc, aggressive, stay_area, no_body | caster | Fen hazard | Deep fog rooms |
| 3774 | eel electric mosswater | an electric mosswater eel | 32 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen predator | Deep water rooms |
| 3775 | wolf fen grey | a fen-grey wolf | 28 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Fen-edge hunting grounds |
| 3776 | smuggler boss fen | the fen smuggler boss | 48 | 1 (M) | -178 | is_npc, aggressive, stay_area, solo, remember | hybrid | Strong hostile | Deep smuggler camp |
| 3777 | asp mud venom | a venomous mud asp | 30 | 0 | 0 | is_npc, aggressive, stay_area | melee | Fen hazard | Mudbank rooms |
| 3778 | crane mist fisher | a mist-fisher crane | 26 | 0 | 0 | is_npc, stay_area | — | Wildlife flavor | Open water rooms |

### Region C: Ironpine Rise Mobs (3779-3798)

#### Non-Hostile / Civic

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3779 | warden ridge ironpine | the Ironpine ridge warden | 45 | 1 (M) | 178 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms, Region C |
| 3780 | hermit everkeeper old | an old Everkeeper hermit | 40 | 1 (M) | 278 | is_npc, sentinel, stay_area | hybrid | Lore NPC | Planting-ring rooms |
| 3781 | scout patrol joint | a joint-commission patrol scout | 42 | 2 (F) | 600 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms |

#### Hostile / Wildlife / Faction

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3782 | wolf mountain grey | a grey mountain wolf | 38 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Ridge corridors |
| 3783 | cat mountain stalker | a mountain stalker cat | 42 | 0 | 0 | is_npc, aggressive, stay_area | melee | Ambush predator | Cliffside rooms |
| 3784 | scout ashfang | an Ashfang scout | 50 | 1 (M) | -600 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Off-road, 3-4 rooms out |
| 3785 | raider ashfang | an Ashfang raider | 55 | 2 (F) | -128 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Ashfang camp rooms |
| 3786 | shade everkeeper | an Everkeeper's shade | 52 | 0 | -300 | is_npc, aggressive, stay_area, undead | caster | Undead encounter | Abandoned planting-rings |
| 3787 | sentinel stone antler | a stone-antler sentinel | 58 | 1 (M) | 100 | is_npc, aggressive, stay_area | melee | Sacred site guard | Near megalith rooms |
| 3788 | bear ironpine brown | an ironpine brown bear | 48 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Deep ridge rooms |
| 3789 | alpha wolf pack | the wolf pack alpha | 60 | 1 (M) | 0 | is_npc, aggressive, stay_area, solo | melee | Strong predator | Wolf den rooms |
| 3790 | hawk ridge hunter | a ridge-hunter hawk | 35 | 0 | 0 | is_npc, aggressive, stay_area | melee | Aerial predator | Exposed ridge rooms |
| 3791 | ashfang trapper | an Ashfang trapper | 52 | 1 (M) | -600 | is_npc, aggressive, stay_area, remember | melee | Hostile humanoid | Trap-line rooms |
| 3792 | boar ridge tusked | a tusked ridge boar | 40 | 0 | 0 | is_npc, aggressive, stay_area | melee | Territorial beast | Rocky clearings |
| 3793 | viper rock ironpine | an ironpine rock viper | 36 | 0 | 0 | is_npc, aggressive, stay_area | melee | Ambush predator | Boulder-field rooms |
| 3794 | archer stone antler | a stone-antler archer | 62 | 2 (F) | 100 | is_npc, aggressive, stay_area | melee | Sacred site guard | Outer megalith rooms |
| 3795 | wolverine ridge | a ridge wolverine | 44 | 0 | 0 | is_npc, aggressive, stay_area, no_flee | melee | Fearless predator | Deep ridge rooms |
| 3796 | spider ironpine trap | an ironpine trap-door spider | 38 | 0 | -35 | is_npc, aggressive, stay_area | melee | Ambush predator | Root-hollow rooms |
| 3797 | lynx shadow ironpine | an ironpine shadow lynx | 46 | 0 | 0 | is_npc, aggressive, stay_area | melee | Stealth predator | Dense canopy rooms |
| 3798 | ashfang shaman ridge | an Ashfang ridge shaman | 58 | 1 (M) | -178 | is_npc, aggressive, stay_area, remember | caster | Hostile caster | Ashfang ritual sites |

### Region D: Ancestor Deeps Mobs (3799-3813)

#### Non-Hostile

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3799 | warden ancestor road deep | the Ancestor Deeps road warden | 50 | 1 (M) | 178 | is_npc, sentinel, stay_area | hybrid | Road patrol | Road rooms, Region D |

#### Hostile / Wildlife / Faction

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3800 | ghost cairnkeeper | a Cairn-Keeper ghost | 70 | 0 | -500 | is_npc, aggressive, stay_area, undead, no_body | caster | Undead encounter | Standing stone rooms |
| 3801 | acolyte rootbound | a Rootbound acolyte | 65 | 2 (F) | -278 | is_npc, aggressive, stay_area, remember | hybrid | Hostile cultist | Deep branch rooms |
| 3802 | ritualist rootbound | a Rootbound ritualist | 72 | 1 (M) | -282 | is_npc, aggressive, stay_area, remember | caster | Hostile caster | Ritual clearing rooms |
| 3803 | wolf dire ancestor | an ancestor dire wolf | 68 | 0 | 0 | is_npc, aggressive, stay_area | melee | Apex predator | Deep interior rooms |
| 3804 | bear cave ancient | an ancient cave bear | 75 | 0 | 0 | is_npc, aggressive, stay_area | melee | Apex predator | Cavern-mouth rooms |
| 3805 | guardian spirit megalith | a megalith spirit guardian | 72 | 0 | 35 | is_npc, aggressive, stay_area | caster | Sacred site defender | Megalith ring rooms |
| 3806 | warchief ashfang | the Ashfang war-chief | 82 | 1 (M) | -278 | is_npc, aggressive, stay_area, solo, remember, no_flee | hybrid | Strong hostile | Deep Ashfang camp |
| 3807 | lieutenant thornmother | a Thornmother's lieutenant | 80 | 2 (F) | -286 | is_npc, aggressive, stay_area, solo, remember | hybrid | Strong hostile | Rootbound outer grove |
| 3808 | stag corrupted red | a red-corrupted stag | 70 | 1 (M) | -400 | is_npc, aggressive, stay_area | melee | Corrupted wildlife | Red-moss bloom rooms |
| 3809 | watcher megalith stone | a megalith stone watcher | 78 | 0 | 0 | is_npc, aggressive, stay_area, no_body | caster | Construct guardian | Inner megalith rooms |
| 3810 | treant ironpine ancient | an ancient ironpine treant | 85 | 0 | 300 | is_npc, aggressive, stay_area, solo, no_flee | melee | Strong predator | Deepest ironpine groves |
| 3811 | serpent root coiled | a coiled root serpent | 65 | 0 | -300 | is_npc, aggressive, stay_area | melee | Corrupted wildlife | Root-cavern rooms |
| 3812 | ashfang berserker deep | a deep Ashfang berserker | 75 | 1 (M) | -178 | is_npc, aggressive, stay_area, remember, no_flee | melee | Hostile humanoid | Deep Ashfang territory |
| 3813 | moth ghost pale | a pale ghost moth | 62 | 0 | -35 | is_npc, aggressive, stay_area | caster | Deep-forest hazard | Luminescent moss rooms |

### Region E: Northern Crown / Kowloon Reach Mobs (3814-3838)

#### Non-Hostile / Civic

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3814 | officer customs northern | the northern customs officer | 65 | 1 (M) | 178 | is_npc, sentinel, stay_area | melee | Checkpoint NPC | Northern Customs Post |
| 3815 | warden patrol kowloon | a Kowloon patrol warden | 60 | 2 (F) | 178 | is_npc, sentinel, stay_area | melee | Road patrol | Road rooms, Region E |
| 3816 | runner courier lantern | a Courier Lantern runner | 45 | 1 (M) | 600 | is_npc, stay_area | melee | Message runner | Road rooms, Region E |
| 3817 | captain warden north | the North Warden Camp captain | 75 | 1 (M) | 278 | is_npc, sentinel, stay_area | hybrid | Camp commander | North Warden Camp |
| 3818 | guard warden north camp | a North Warden Camp guard | 55 | 2 (F) | 178 | is_npc, sentinel, stay_area | melee | Camp guard | North Warden Camp |

#### Hostile / Wildlife / Faction

| Vnum | player_name | short_descr | Level | Sex | Alignment | Act Flags | Archetype | Role | Spawn Concept |
|---|---|---|---:|---|---:|---|---|---|---|
| 3819 | viper mist northern | a northern mist viper | 45 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Mist-valley rooms |
| 3820 | berserker ashfang northern | a northern Ashfang berserker | 70 | 1 (M) | -178 | is_npc, aggressive, stay_area, remember, no_flee | melee | Hostile humanoid | Northern Ashfang camps |
| 3821 | elk crown bull | a crown bull elk | 55 | 1 (M) | 0 | is_npc, aggressive, stay_area | melee | Territorial beast | Northern ridgeline rooms |
| 3822 | wolf frost touched | a frost-touched wolf | 50 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Cold-valley rooms |
| 3823 | wraith oathbreaker | an oathbreaker wraith | 80 | 0 | -286 | is_npc, aggressive, stay_area, undead, no_body | caster | Undead encounter | Ruined oath-stele rooms |
| 3824 | thornmother the | @@Rthe Thornmother@@N | 98 | 2 (F) | -291 | is_npc, aggressive, sentinel, stay_area, boss, no_flee | caster | **BOSS** | Hidden grove (`no_mob` room) |
| 3825 | stag white the | @@Wthe White Stag@@N | 90 | 1 (M) | 291 | is_npc, sentinel, stay_area, solo | — | Rare encounter | Crown of Antlers apex |
| 3826 | lord bear primal | @@Rthe Primal Bear Lord@@N | 95 | 1 (M) | 0 | is_npc, aggressive, sentinel, stay_area, solo, no_flee | melee | Apex predator | Crown of Antlers apex |
| 3827 | cat mist shadow | a mist-shadow cat | 48 | 0 | 0 | is_npc, aggressive, stay_area | melee | Ambush predator | Mist-valley rooms |
| 3828 | ashfang warmaster northern | the northern Ashfang warmaster | 88 | 1 (M) | -278 | is_npc, aggressive, stay_area, solo, remember, no_flee | hybrid | Strong hostile | Deepest Ashfang camp |
| 3829 | cultist rootbound crown | a Crown Rootbound cultist | 78 | 2 (F) | -282 | is_npc, aggressive, stay_area, remember | hybrid | Hostile cultist | Northern cult rooms |
| 3830 | elk cow crown | a crown cow elk | 40 | 2 (F) | 0 | is_npc, aggressive, stay_area | melee | Territorial beast | Elk grazing rooms |
| 3831 | serpent crown ice | an ice crown serpent | 55 | 0 | 0 | is_npc, aggressive, stay_area | melee | Predator | Cold-stream rooms |
| 3832 | sentinel antler stone elder | an elder stone-antler sentinel | 82 | 1 (M) | 35 | is_npc, aggressive, stay_area, solo | melee | Strong guardian | Northern megalith rooms |
| 3833 | hawk bone crowned | a bone-crowned hawk | 42 | 0 | 0 | is_npc, aggressive, stay_area | melee | Aerial predator | Exposed peak rooms |
| 3834 | spirit ancestor cold | a cold ancestor spirit | 85 | 0 | -600 | is_npc, aggressive, stay_area, undead, no_body | caster | Undead encounter | Northern standing stones |
| 3835 | rootbound briar knight | a Rootbound briar knight | 82 | 1 (M) | -286 | is_npc, aggressive, stay_area, remember, no_flee | melee | Elite cultist | Thornmother approach |
| 3836 | boar ghost white | a white ghost boar | 60 | 0 | 0 | is_npc, aggressive, stay_area | melee | Spectral beast | Northern ghost-birch rooms |
| 3837 | bear mother ironpine | an ironpine mother bear | 65 | 2 (F) | 0 | is_npc, aggressive, stay_area, no_flee | melee | Protective predator | Northern den rooms |
| 3838 | warden ironhand kowloon | a Kowloon Warden of Iron | 70 | 1 (M) | 178 | is_npc, sentinel, stay_area | hybrid | Kowloon patrol | Near north gate rooms |

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
| 3824 | the Thornmother | `spec_cast_undead` | Boss caster; undead/nature hybrid with dark casting |
| 3798 | Ashfang ridge shaman | `spec_cast_cleric` | Hostile caster with healing and offense |
| 3802 | Rootbound ritualist | `spec_cast_mage` | Cult caster with arcane offense |
| 3773 | marsh will-o-wisp | `spec_poison` | Fen hazard with poison attacks |
| 3786 | Everkeeper's shade | `spec_cast_undead` | Undead caster |
| 3800 | Cairn-Keeper ghost | `spec_cast_undead` | Undead caster |
| 3823 | oathbreaker wraith | `spec_cast_undead` | Undead caster |
| 3834 | cold ancestor spirit | `spec_cast_undead` | Undead caster |
| 3805 | megalith spirit guardian | `spec_cast_mage` | Arcane guardian caster |
| 3809 | megalith stone watcher | `spec_cast_mage` | Arcane construct caster |
| 3826 | the Primal Bear Lord | `spec_breath_frost` | Apex beast with frost breath |

---

## Object Definitions (90 objects)

All objects in this area use vnums `3739-3828` (object index). Cross-type overlap with room and mob vnums is allowed per spec. All objects must include `ITEM_TAKE` (`8388608`) in wear flags. No object may set `ITEM_GENERATED` or `ITEM_WEAR_CLAN_COLORS`. Item stats are generated at runtime; only identity/behavior data is authored. Object names must be unique within the area file.

Weight archetype ranges: `1-5` = caster item, `6-10` = melee item, `11-15` = tank item.

### Weapons (item_type = 5, wear_flags include hold + take)

All weapons require `hold` (`32768`) + `take` (`8388608`) in wear_flags. `value3` must be thematically consistent with weapon concept. Two-handed weapons include `ITEM_TWO_HANDED` (`2147483648`) in extra_flags. Fist weapons include `ITEM_FIST` (`1073741824`) and use `value3 = 0` (hit).

| Vnum | name (keywords) | short_descr | Lv | Weight | value3 | Extra Flags | Loot? | Notes |
|---|---|---|---:|---:|---:|---|---|---|
| 3739 | sword short warden | a warden's short sword | 20 | 7 | 3 (slash) | — | — | Road warden equipment |
| 3740 | knife hunting poacher | a poacher's hunting knife | 22 | 6 | 2 (stab) | ITEM_LOOT | loot | Poacher drop |
| 3741 | spear bone mire clan | a mire-clan bone spear | 30 | 8 | 11 (pierce) | ITEM_LOOT | loot | Mire-clan themed |
| 3742 | javelin fang tipped | a fang-tipped javelin | 35 | 7 | 11 (pierce) | ITEM_LOOT | loot | Wildlife-themed |
| 3743 | axe blackened ashfang | an Ashfang blackened axe | 50 | 9 | 3 (slash) | ITEM_LOOT | loot | Ashfang drop |
| 3744 | blade ritual stone antler | a stone-antler ritual blade | 55 | 6 | 1 (slice) | ITEM_LOOT | loot | Stone-antler drop |
| 3745 | mace heavy ironpine | an ironpine heavy mace | 60 | 10 | 7 (pound) | ITEM_LOOT | loot | Ironpine Rise themed |
| 3746 | halberd ridge warden | a ridge warden's halberd | 65 | 12 | 3 (slash) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Two-handed |
| 3747 | glaive thorn rootbound | a Rootbound thorn glaive | 70 | 9 | 1 (slice) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Two-handed, cult drop |
| 3748 | cleaver warchief ashfang | an Ashfang war-chief's cleaver | 80 | 10 | 3 (slash) | ITEM_LOOT | loot | War-chief drop |
| 3749 | blade ghost cairnkeeper | a Cairn-Keeper ghost blade | 85 | 5 | 1 (slice) | ITEM_LOOT | loot | Undead drop |
| 3750 | scourge briar thornmother | a Thornmother's briar scourge | 90 | 7 | 4 (whip) | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |
| 3751 | greataxe crown antlers | a Crown of Antlers great axe | 95 | 13 | 3 (slash) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Apex drop |
| 3752 | sword cursed oathbreaker | an oathbreaker's cursed sword | 95 | 8 | 3 (slash) | ITEM_LOOT | loot | Wraith drop |
| 3753 | fist bearclaw primal | a primal bear-claw fist weapon | 100 | 9 | 0 (hit) | ITEM_LOOT, ITEM_FIST | loot | Apex beast drop |
| 3754 | spear lantern road warden | a Lantern Road warden's spear | 40 | 8 | 11 (pierce) | — | — | Warden equipment |
| 3755 | dagger smuggler concealed | a smuggler's concealed dagger | 35 | 6 | 2 (stab) | ITEM_LOOT | loot | Smuggler drop |
| 3756 | club ironwood heavy | a heavy ironwood club | 45 | 10 | 8 (crush) | ITEM_LOOT | loot | Bandit/raider drop |
| 3757 | whip vine thornmother | a Thornmother's vine whip | 92 | 5 | 4 (whip) | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |
| 3758 | greatsword ashfang warmaster | a warmaster's ashfang greatsword | 88 | 12 | 3 (slash) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Warmaster drop |
| 3759 | spear antler elder stone | an elder stone-antler spear | 82 | 9 | 11 (pierce) | ITEM_LOOT | loot | Elder sentinel drop |
| 3760 | pike briar rootbound knight | a briar knight's thorn pike | 82 | 11 | 11 (pierce) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Briar knight drop |
| 3761 | staff gnarled treant | a gnarled treant staff | 85 | 12 | 8 (crush) | ITEM_LOOT, ITEM_TWO_HANDED | loot | Treant drop |
| 3762 | wand fen wisp | a fen wisp wand | 36 | 3 | 6 (blast) | ITEM_LOOT, ITEM_EXTRA_WAND | loot | Will-o-wisp drop |

### Armor (item_type = 9)

All armor requires `take` (`8388608`) plus the appropriate wear slot flag. Shields use `hold` (`32768`). Bucklers add `ITEM_BUCKLER` (`268435456`).

| Vnum | name (keywords) | short_descr | Lv | Weight | Wear Slot | Extra Flags | Loot? | Notes |
|---|---|---|---:|---:|---|---|---|---|
| 3763 | jerkin leather warden | a warden's leather jerkin | 20 | 12 | body | — | — | Road warden equipment |
| 3764 | hood fur lined woodsman | a woodsman's fur-lined hood | 22 | 11 | head | ITEM_LOOT | loot | Poacher/trapper drop |
| 3765 | mantle woven mire clan | a mire-clan woven mantle | 30 | 4 | about | ITEM_LOOT | loot | Mire-clan themed |
| 3766 | boots marsh treader | a pair of marsh-treader boots | 35 | 11 | feet | ITEM_LOOT | loot | Fen themed |
| 3767 | pauldrons spiked ashfang | a pair of Ashfang spiked pauldrons | 50 | 13 | shoulders | ITEM_LOOT | loot | Ashfang drop |
| 3768 | helm bone stone antler | a stone-antler bone helm | 55 | 12 | head | ITEM_LOOT | loot | Stone-antler drop |
| 3769 | shield bark ironpine | an ironpine bark shield | 60 | 14 | hold | ITEM_LOOT | loot | Ironpine Rise themed |
| 3770 | vest chain ridge patrol | a ridge patrol chain vest | 65 | 14 | body | ITEM_LOOT | loot | Ridge warden drop |
| 3771 | armor living rootbound | a suit of Rootbound living armor | 70 | 12 | body | ITEM_LOOT | loot | Rootbound cultist drop |
| 3772 | plate warchief ashfang | an Ashfang war-chief's plate | 80 | 15 | body | ITEM_LOOT | loot | War-chief drop |
| 3773 | shield ghost cairnkeeper | a Cairn-Keeper ghost shield | 85 | 4 | hold | ITEM_LOOT | loot | Ghost drop |
| 3774 | crown briar thornmother | a Thornmother's briar crown | 90 | 4 | head | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |
| 3775 | greatshield bearhide primal | a primal bearhide greatshield | 95 | 15 | hold | ITEM_LOOT | loot | Apex drop |
| 3776 | gauntlets cursed oathbreaker | a pair of oathbreaker's cursed gauntlets | 95 | 9 | hands | ITEM_LOOT | loot | Wraith drop |
| 3777 | buckler stag crown | a crown stag-horn buckler | 100 | 13 | hold | ITEM_LOOT, ITEM_BUCKLER | loot | Apex drop |
| 3778 | greaves lantern road patrol | a pair of Lantern Road patrol greaves | 40 | 13 | legs | — | — | Warden equipment |
| 3779 | bracers ashfang iron | a pair of Ashfang iron bracers | 55 | 10 | wrist | ITEM_LOOT | loot | Ashfang drop |
| 3780 | coat mosswater oiled | a mosswater oiled coat | 32 | 11 | about | ITEM_LOOT | loot | Fen themed |
| 3781 | helm ironpine bark ridge | an ironpine bark ridge helm | 60 | 13 | head | ITEM_LOOT | loot | Ironpine Rise themed |
| 3782 | boots ridge patrol heavy | a pair of heavy ridge patrol boots | 65 | 14 | feet | ITEM_LOOT | loot | Ridge patrol drop |
| 3783 | cloak treant moss | a treant moss cloak | 85 | 5 | about | ITEM_LOOT | loot | Treant drop |
| 3784 | plate briar knight rootbound | a briar knight's thorn plate | 82 | 14 | body | ITEM_LOOT | loot | Briar knight drop |
| 3785 | vambraces warmaster ashfang | a pair of warmaster's ashfang vambraces | 88 | 12 | arms | ITEM_LOOT | loot | Warmaster drop |
| 3786 | skirt chain ashfang | an Ashfang chain skirt | 52 | 12 | legs | ITEM_LOOT | loot | Ashfang raider drop |
| 3787 | cowl rootbound vine | a Rootbound vine cowl | 65 | 4 | head | ITEM_LOOT | loot | Rootbound acolyte drop |
| 3788 | shield elder stone antler | an elder stone-antler shield | 82 | 15 | hold | ITEM_LOOT | loot | Elder sentinel drop |
| 3789 | robe thornmother thorn woven | a Thornmother's thorn-woven robe | 95 | 3 | body | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |

### Accessories (item_type = 9 for worn, or item_type = 8 for treasure)

| Vnum | name (keywords) | short_descr | Lv | Weight | Item Type | Wear Slot | Extra Flags | Loot? | Notes |
|---|---|---|---:|---:|---:|---|---|---|---|
| 3790 | pendant lantern warden | a warden's lantern pendant | 20 | 3 | 9 | neck | — | — | Warden equipment |
| 3791 | ring reed mire speaker | a mire-speaker's reed ring | 30 | 2 | 9 | finger | ITEM_LOOT | loot | Mire-clan themed |
| 3792 | necklace tooth ashfang | an Ashfang tooth necklace | 50 | 8 | 9 | neck | ITEM_LOOT | loot | Ashfang drop |
| 3793 | torc stone antler circle | a stone-antler circle torc | 55 | 7 | 9 | neck | ITEM_LOOT | loot | Stone-antler drop |
| 3794 | ring planting everkeeper | an Everkeeper's planting-ring | 60 | 3 | 9 | finger | ITEM_LOOT | loot | Everkeeper shade drop |
| 3795 | amulet bone cairnkeeper | a Cairn-Keeper's bone amulet | 75 | 4 | 9 | neck | ITEM_LOOT | loot | Ghost drop |
| 3796 | bracelet vine rootbound | a Rootbound vine bracelet | 70 | 3 | 9 | wrist | ITEM_LOOT | loot | Rootbound drop |
| 3797 | ring signet oathbreaker | an oathbreaker's signet ring | 85 | 4 | 9 | finger | ITEM_LOOT | loot | Wraith drop |
| 3798 | circlet thorn thornmother | a Thornmother's thorn circlet | 90 | 2 | 9 | horns | ITEM_LOOT, ITEM_BOSS | loot | Boss drop |
| 3799 | crown antler white stag | a white stag antler crown | 100 | 4 | 9 | head | ITEM_LOOT | loot | Apex rare drop |
| 3800 | earring fang wolf | a wolf-fang earring | 25 | 6 | 9 | ear | ITEM_LOOT | loot | Wolf territory drop |
| 3801 | belt smuggler hidden | a smuggler's hidden-pocket belt | 35 | 7 | 9 | waist | ITEM_LOOT | loot | Smuggler drop |
| 3802 | cloak patrol warden forest | a forest warden's patrol cloak | 40 | 11 | 9 | about | — | — | Warden equipment |
| 3803 | sash joint commission | a joint-commission warden's sash | 42 | 3 | 9 | waist | — | — | Scout equipment |
| 3804 | ring ironpine bark | an ironpine bark ring | 60 | 4 | 9 | finger | ITEM_LOOT | loot | Ironpine Rise themed |
| 3805 | torque ancestor bone cold | a cold ancestor-bone torque | 85 | 5 | 9 | neck | ITEM_LOOT | loot | Ancestor spirit drop |
| 3806 | crown iron warmaster | a warmaster's iron crown | 88 | 9 | 9 | head | ITEM_LOOT | loot | Warmaster drop |
| 3807 | amulet bear tooth primal | a primal bear-tooth amulet | 95 | 8 | 9 | neck | ITEM_LOOT | loot | Bear Lord drop |

### Consumables and Flavor Objects

| Vnum | name (keywords) | short_descr | Lv | Weight | Item Type | Wear Flags | Extra Flags | Notes |
|---|---|---|---:|---:|---:|---|---|---|
| 3808 | rations trail warden | a pack of warden's trail rations | 1 | 6 | 19 (food) | take | — | Road warden supply |
| 3809 | flask resin mire clan | a mire-clan resin flask | 1 | 4 | 17 (drink_con) | take | — | Fen village flavor |
| 3810 | mushroom forest edible | an edible forest mushroom | 1 | 2 | 19 (food) | take | — | Forage item |
| 3811 | tea pine northern | a pouch of northern pine tea | 1 | 2 | 19 (food) | take | — | Northern Crown flavor |
| 3812 | bandage herb poultice | a herbal poultice bandage | 1 | 3 | 19 (food) | take | — | Fen herbalist supply |
| 3813 | ledger passage tithe relic | a relic passage tithe ledger | 1 | 4 | 13 (trash) | take | — | Room flavor object |
| 3814 | seal stamp midgaard wax | a Midgaard wax seal stamp | 1 | 3 | 13 (trash) | take | — | Room flavor object |
| 3815 | seal knot kowloon thread | a Kowloon thread-knot seal | 1 | 3 | 13 (trash) | take | — | Room flavor object |
| 3816 | lantern road iron banded | an iron-banded road lantern | 1 | 8 | 1 (light) | take | — | Road atmosphere |
| 3817 | map forest worn trail | a worn forest trail map | 1 | 2 | 13 (trash) | take | — | Flavor loot |
| 3818 | hide wolf grey tanned | a tanned grey wolf hide | 1 | 7 | 13 (trash) | take | — | Wildlife loot |
| 3819 | fang boar tusked | a tusked boar fang | 1 | 5 | 13 (trash) | take | — | Wildlife loot |
| 3820 | venom sac snake marsh | a marsh snake venom sac | 1 | 2 | 13 (trash) | take | — | Fen wildlife loot |
| 3821 | skin frog giant fen | a giant fen frog skin | 1 | 4 | 13 (trash) | take | — | Fen wildlife loot |
| 3822 | claw bear cave ancient | an ancient cave bear claw | 1 | 6 | 13 (trash) | take | — | Trophy drop |
| 3823 | antler stag white | a white stag antler | 1 | 5 | 8 (treasure) | take | — | Rare trophy |
| 3824 | bark ironpine petrified | a chunk of petrified ironpine bark | 1 | 8 | 13 (trash) | take | — | Ironpine Rise flavor |
| 3825 | moss luminescent jar | a jar of luminescent moss | 1 | 3 | 1 (light) | take | — | Ancestor Deeps flavor |
| 3826 | token ashfang black | an Ashfang black token | 1 | 4 | 13 (trash) | take | — | Ashfang faction drop |
| 3827 | charm rootbound vine | a Rootbound vine charm | 1 | 2 | 13 (trash) | take | — | Rootbound cult drop |
| 3828 | stone cairn carved | a carved cairn stone | 1 | 6 | 13 (trash) | take | — | Ancestor Deeps flavor |

---

## Loot Table Plan

Loot tables connect specific mobs to their item drops. The `l` extension defines loot amount and item vnums; the `L` extension defines per-slot selection weights. Sum of all `L` values must be `<= 100`. All items referenced by loot tables must have `ITEM_LOOT` set. Boss mob items must also have `ITEM_BOSS` set.

### Region A: Southern Verge Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 3744 | grey timber wolf | 50 | 3818 (wolf hide), 3800 (wolf-fang earring) | 70, 30 |
| 3745 | wild forest boar | 50 | 3819 (boar fang) | 100 |
| 3746 | giant forest spider | 30 | 3817 (trail map) | 100 |
| 3747 | forest poacher | 80 | 3740 (hunting knife), 3764 (fur-lined hood), 3817 (trail map) | 40, 35, 25 |
| 3748 | masked highwayman | 100 | 3740 (hunting knife), 3755 (concealed dagger), 3801 (hidden belt) | 30, 40, 30 |
| 3752 | woodland bandit | 80 | 3755 (concealed dagger), 3764 (fur-lined hood) | 55, 45 |
| 3753 | young black bear | 50 | 3822 (bear claw) | 100 |
| 3754 | wolf pack runner | 40 | 3818 (wolf hide), 3800 (wolf-fang earring) | 75, 25 |

### Region B: Mosswater Bends Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 3764 | marsh water snake | 40 | 3820 (venom sac) | 100 |
| 3765 | giant fen frog | 50 | 3821 (frog skin) | 100 |
| 3767 | Mosswater crocodile | 80 | 3742 (fang javelin), 3766 (marsh boots) | 50, 50 |
| 3768 | bog lurker | 80 | 3742 (fang javelin), 3780 (oiled coat) | 55, 45 |
| 3769 | smuggler lookout | 100 | 3755 (concealed dagger), 3801 (hidden belt), 3826 (ashfang token) | 40, 35, 25 |
| 3770 | fen smuggler runner | 100 | 3755 (concealed dagger), 3741 (bone spear), 3801 (hidden belt) | 35, 35, 30 |
| 3773 | marsh will-o-wisp | 60 | 3762 (fen wisp wand) | 100 |
| 3776 | fen smuggler boss | 150 | 3755 (concealed dagger), 3741 (bone spear), 3801 (hidden belt), 3780 (oiled coat) | 25, 30, 20, 25 |

### Region C: Ironpine Rise Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 3782 | grey mountain wolf | 60 | 3818 (wolf hide), 3800 (wolf-fang earring) | 65, 35 |
| 3783 | mountain stalker cat | 60 | 3824 (ironpine bark) | 100 |
| 3784 | Ashfang scout | 100 | 3743 (blackened axe), 3767 (spiked pauldrons), 3792 (tooth necklace), 3826 (ashfang token) | 25, 25, 25, 25 |
| 3785 | Ashfang raider | 120 | 3743 (blackened axe), 3767 (spiked pauldrons), 3786 (chain skirt), 3792 (tooth necklace) | 25, 25, 25, 25 |
| 3786 | Everkeeper's shade | 80 | 3794 (planting-ring), 3749 (ghost blade) | 60, 40 |
| 3787 | stone-antler sentinel | 100 | 3744 (ritual blade), 3768 (bone helm), 3793 (circle torc) | 35, 35, 30 |
| 3788 | ironpine brown bear | 60 | 3822 (bear claw), 3824 (ironpine bark) | 60, 40 |
| 3789 | wolf pack alpha | 120 | 3818 (wolf hide), 3800 (wolf-fang earring), 3804 (bark ring) | 35, 35, 30 |
| 3791 | Ashfang trapper | 100 | 3743 (blackened axe), 3779 (iron bracers), 3826 (ashfang token) | 35, 35, 30 |
| 3794 | stone-antler archer | 100 | 3744 (ritual blade), 3768 (bone helm), 3793 (circle torc) | 30, 35, 35 |
| 3798 | Ashfang ridge shaman | 120 | 3743 (blackened axe), 3792 (tooth necklace), 3779 (iron bracers), 3756 (ironwood club) | 25, 25, 25, 25 |

### Region D: Ancestor Deeps Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 3800 | Cairn-Keeper ghost | 100 | 3749 (ghost blade), 3773 (ghost shield), 3795 (bone amulet), 3828 (cairn stone) | 25, 25, 25, 25 |
| 3801 | Rootbound acolyte | 100 | 3747 (thorn glaive), 3796 (vine bracelet), 3787 (vine cowl), 3827 (vine charm) | 25, 25, 25, 25 |
| 3802 | Rootbound ritualist | 120 | 3747 (thorn glaive), 3771 (living armor), 3796 (vine bracelet), 3787 (vine cowl) | 25, 25, 25, 25 |
| 3803 | ancestor dire wolf | 80 | 3818 (wolf hide), 3800 (wolf-fang earring) | 55, 45 |
| 3804 | ancient cave bear | 100 | 3822 (bear claw) | 100 |
| 3806 | Ashfang war-chief | 35 | 3748 (cleaver), 3772 (plate), 3792 (tooth necklace), 3756 (ironwood club) | 30, 30, 20, 20 |
| 3807 | Thornmother's lieutenant | 35 | 3747 (thorn glaive), 3771 (living armor), 3796 (vine bracelet), 3787 (vine cowl) | 30, 25, 25, 20 |
| 3808 | red-corrupted stag | 80 | 3827 (vine charm), 3824 (ironpine bark) | 55, 45 |
| 3810 | ancient ironpine treant | 150 | 3761 (treant staff), 3783 (moss cloak), 3824 (ironpine bark) | 35, 30, 35 |
| 3812 | deep Ashfang berserker | 120 | 3743 (blackened axe), 3772 (plate), 3779 (iron bracers) | 35, 35, 30 |

### Region E: Northern Crown Loot

| Mob Vnum | Mob Name | loot_amount | Loot Slots (obj vnums) | Loot Chances (%) |
|---|---|---:|---|---|
| 3820 | northern Ashfang berserker | 120 | 3743 (blackened axe), 3767 (spiked pauldrons), 3792 (tooth necklace), 3826 (ashfang token) | 25, 25, 25, 25 |
| 3823 | oathbreaker wraith | 150 | 3752 (cursed sword), 3776 (cursed gauntlets), 3797 (signet ring) | 35, 35, 30 |
| 3824 | **the Thornmother** | 300 | 3750 (briar scourge), 3757 (vine whip), 3774 (briar crown), 3798 (thorn circlet), 3789 (thorn robe) | 20, 20, 20, 20, 20 |
| 3825 | the White Stag | 100 | 3799 (antler crown), 3823 (white antler) | 50, 50 |
| 3826 | **the Primal Bear Lord** | 35 | 3753 (bear-claw fist), 3775 (bearhide greatshield), 3807 (bear-tooth amulet), 3822 (bear claw) | 30, 25, 25, 20 |
| 3828 | northern Ashfang warmaster | 35 | 3758 (warmaster greatsword), 3785 (warmaster vambraces), 3806 (iron crown), 3792 (tooth necklace) | 30, 25, 25, 20 |
| 3829 | Crown Rootbound cultist | 120 | 3747 (thorn glaive), 3796 (vine bracelet), 3787 (vine cowl), 3827 (vine charm) | 25, 25, 25, 25 |
| 3832 | elder stone-antler sentinel | 150 | 3759 (elder spear), 3788 (elder shield), 3793 (circle torc) | 35, 35, 30 |
| 3834 | cold ancestor spirit | 120 | 3805 (ancestor torque), 3795 (bone amulet), 3828 (cairn stone) | 35, 35, 30 |
| 3835 | Rootbound briar knight | 150 | 3760 (thorn pike), 3784 (thorn plate), 3796 (vine bracelet) | 35, 35, 30 |

---

## Reset Plan

### Mob Resets (M)

Mob resets place mobiles in specific rooms. Format: `M 0 <mob_vnum> <limit> <room_vnum>`. Limits control maximum concurrent instances.

#### Region A: Southern Verge
- `3739` (southern road warden): 3 resets across southern road rooms, limit 1 each
- `3740` (lamp-keeper): 2 resets at road bend rooms, limit 1 each
- `3741` (Woodwrights surveyor): 1 reset near-road, limit 1
- `3742` (caravan guard): 2 resets on road rooms, limit 1 each
- `3743` (traveling peddler): 1 reset roadside clearing, limit 1
- `3744` (timber wolf): 4 resets off-road, limit 2 each
- `3745` (wild boar): 3 resets off-road thickets, limit 2 each
- `3746` (giant spider): 2 resets dense canopy, limit 1 each
- `3747` (poacher): 2 resets off-road, limit 1 each
- `3748` (highwayman): 2 resets near-road ambush spots, limit 1 each
- `3749` (young stag): 2 resets forest glades, limit 1 each
- `3750` (red fox): 2 resets undergrowth, limit 1 each
- `3751` (copperhead): 2 resets rocky off-road, limit 1 each
- `3752` (woodland bandit): 2 resets off-road camps, limit 1 each
- `3753` (young black bear): 2 resets deep off-road, limit 1 each
- `3754` (wolf pack runner): 3 resets wolf territory, limit 2 each
- `3755` (southern patrol sergeant): 1 reset south gate area, limit 1
- `3756` (verge trapper): 1 reset trapping clearing, limit 1
- `3757` (great horned owl): 2 resets canopy rooms, limit 1 each
- `3758` (forest badger): 2 resets burrow rooms, limit 1 each

#### Region B: Mosswater Bends
- `3759` (mire-clan fisher): 2 resets reed-platform rooms, limit 1 each
- `3760` (mire-clan herbalist): 1 reset fen village, limit 1
- `3761` (fen toll collector): 1 reset Mosswater Toll Bridge, limit 1
- `3762` (reed-platform trader): 1 reset fen village, limit 1
- `3763` (Mosswater road warden): 2 resets road rooms Region B, limit 1 each
- `3764` (marsh water snake): 4 resets waterway rooms, limit 2 each
- `3765` (giant fen frog): 3 resets shallow water rooms, limit 2 each
- `3766` (territorial heron): 2 resets hummock rooms, limit 1 each
- `3767` (Mosswater crocodile): 2 resets deep channel rooms, limit 1 each
- `3768` (bog lurker): 2 resets deep fen rooms, limit 1 each
- `3769` (smuggler lookout): 2 resets off-road fen edges, limit 1 each
- `3770` (fen smuggler runner): 2 resets smuggler camps, limit 1 each
- `3771` (giant leech swarm): 2 resets stagnant water, limit 1 each
- `3772` (moss-backed snapping turtle): 2 resets stream crossings, limit 1 each
- `3773` (marsh will-o-wisp): 2 resets deep fog rooms, limit 1 each
- `3774` (electric eel): 1 reset deep water, limit 1
- `3775` (fen-grey wolf): 3 resets fen-edge hunting grounds, limit 2 each
- `3776` (fen smuggler boss): 1 reset deep smuggler camp, limit 1
- `3777` (venomous mud asp): 2 resets mudbank rooms, limit 1 each
- `3778` (mist-fisher crane): 2 resets open water, limit 1 each

#### Region C: Ironpine Rise
- `3779` (Ironpine ridge warden): 2 resets road rooms Region C, limit 1 each
- `3780` (Everkeeper hermit): 1 reset planting-ring room, limit 1
- `3781` (joint-commission scout): 2 resets road rooms, limit 1 each
- `3782` (grey mountain wolf): 4 resets ridge corridors, limit 2 each
- `3783` (mountain stalker cat): 2 resets cliffside rooms, limit 1 each
- `3784` (Ashfang scout): 3 resets off-road 3-4 rooms out, limit 1 each
- `3785` (Ashfang raider): 3 resets Ashfang camp rooms, limit 1 each
- `3786` (Everkeeper's shade): 2 resets abandoned planting-rings, limit 1 each
- `3787` (stone-antler sentinel): 2 resets near megalith rooms, limit 1 each
- `3788` (ironpine brown bear): 2 resets deep ridge rooms, limit 1 each
- `3789` (wolf pack alpha): 1 reset wolf den room, limit 1
- `3790` (ridge-hunter hawk): 2 resets exposed ridge rooms, limit 1 each
- `3791` (Ashfang trapper): 2 resets trap-line rooms, limit 1 each
- `3792` (tusked ridge boar): 2 resets rocky clearings, limit 1 each
- `3793` (ironpine rock viper): 2 resets boulder-field rooms, limit 1 each
- `3794` (stone-antler archer): 2 resets outer megalith rooms, limit 1 each
- `3795` (ridge wolverine): 2 resets deep ridge rooms, limit 1 each
- `3796` (ironpine trap-door spider): 2 resets root-hollow rooms, limit 1 each
- `3797` (ironpine shadow lynx): 2 resets dense canopy rooms, limit 1 each
- `3798` (Ashfang ridge shaman): 1 reset Ashfang ritual site, limit 1

#### Region D: Ancestor Deeps
- `3799` (Ancestor Deeps road warden): 2 resets road rooms Region D, limit 1 each
- `3800` (Cairn-Keeper ghost): 3 resets standing stone rooms, limit 1 each
- `3801` (Rootbound acolyte): 3 resets deep branch rooms, limit 1 each
- `3802` (Rootbound ritualist): 2 resets ritual clearing rooms, limit 1 each
- `3803` (ancestor dire wolf): 3 resets deep interior rooms, limit 1 each
- `3804` (ancient cave bear): 2 resets cavern-mouth rooms, limit 1 each
- `3805` (megalith spirit guardian): 2 resets megalith ring rooms, limit 1 each
- `3806` (Ashfang war-chief): 1 reset deep Ashfang camp, limit 1
- `3807` (Thornmother's lieutenant): 1 reset Rootbound outer grove, limit 1
- `3808` (red-corrupted stag): 2 resets red-moss bloom rooms, limit 1 each
- `3809` (megalith stone watcher): 2 resets inner megalith rooms, limit 1 each
- `3810` (ancient ironpine treant): 1 reset deepest ironpine grove, limit 1
- `3811` (coiled root serpent): 2 resets root-cavern rooms, limit 1 each
- `3812` (deep Ashfang berserker): 2 resets deep Ashfang territory, limit 1 each
- `3813` (pale ghost moth): 2 resets luminescent moss rooms, limit 1 each

#### Region E: Northern Crown / Kowloon Reach
- `3814` (northern customs officer): 1 reset Northern Customs Post, limit 1
- `3815` (Kowloon patrol warden): 3 resets road rooms Region E, limit 1 each
- `3816` (Courier Lantern runner): 2 resets road rooms Region E, limit 1 each
- `3817` (North Warden Camp captain): 1 reset North Warden Camp, limit 1
- `3818` (North Warden Camp guard): 2 resets North Warden Camp, limit 1 each
- `3819` (northern mist viper): 3 resets mist-valley rooms, limit 1 each
- `3820` (northern Ashfang berserker): 3 resets northern Ashfang camps, limit 1 each
- `3821` (crown bull elk): 2 resets northern ridgeline rooms, limit 1 each
- `3822` (frost-touched wolf): 3 resets cold-valley rooms, limit 2 each
- `3823` (oathbreaker wraith): 2 resets ruined oath-stele rooms, limit 1 each
- `3824` (the Thornmother): 1 reset hidden grove (no_mob room), limit 1
- `3825` (the White Stag): 1 reset Crown of Antlers apex, limit 1
- `3826` (the Primal Bear Lord): 1 reset Crown of Antlers apex, limit 1
- `3827` (mist-shadow cat): 2 resets mist-valley rooms, limit 1 each
- `3828` (northern Ashfang warmaster): 1 reset deepest Ashfang camp, limit 1
- `3829` (Crown Rootbound cultist): 3 resets northern cult rooms, limit 1 each
- `3830` (crown cow elk): 2 resets elk grazing rooms, limit 2 each
- `3831` (ice crown serpent): 2 resets cold-stream rooms, limit 1 each
- `3832` (elder stone-antler sentinel): 1 reset northern megalith room, limit 1
- `3833` (bone-crowned hawk): 2 resets exposed peak rooms, limit 1 each
- `3834` (cold ancestor spirit): 2 resets northern standing stones, limit 1 each
- `3835` (Rootbound briar knight): 2 resets Thornmother approach, limit 1 each
- `3836` (white ghost boar): 2 resets ghost-birch rooms, limit 1 each
- `3837` (ironpine mother bear): 1 reset northern den room, limit 1
- `3838` (Kowloon Warden of Iron): 2 resets near north gate rooms, limit 1 each

### Equipment Resets (G/E)

Non-hostile civic/patrol NPCs receive thematic equipment via `G` (give to inventory) or `E` (equip on wear slot) resets placed immediately after their `M` reset:

- **Road wardens** (`3739`, `3763`, `3779`, `3799`): `E` warden's short sword (`3739`) on hold, `E` warden's leather jerkin (`3763`) on body, `E` warden's lantern pendant (`3790`) on neck
- **Southern patrol sergeant** (`3755`): `E` Lantern Road warden's spear (`3754`) on hold, `E` Lantern Road patrol greaves (`3778`) on legs
- **Fen toll collector** (`3761`): `E` Lantern Road warden's spear (`3754`) on hold
- **Joint-commission scout** (`3781`): `E` warden's short sword (`3739`) on hold, `E` joint-commission sash (`3803`) on waist
- **North Warden Camp captain** (`3817`): `E` Lantern Road warden's spear (`3754`) on hold, `E` forest warden's patrol cloak (`3802`) on about
- **North Warden Camp guards** (`3818`): `E` warden's short sword (`3739`) on hold, `E` warden's leather jerkin (`3763`) on body
- **Kowloon patrol warden** (`3815`): `E` warden's short sword (`3739`) on hold, `E` warden's leather jerkin (`3763`) on body
- **Kowloon Warden of Iron** (`3838`): `E` Lantern Road warden's spear (`3754`) on hold, `E` Lantern Road patrol greaves (`3778`) on legs, `E` forest warden's patrol cloak (`3802`) on about

### Object Room Resets (O)

Flavor objects placed in specific rooms for atmosphere:

- `3813` (relic tithe ledger): 1-2 resets in ruined guard-hut rooms (Mosswater Bends)
- `3814` (Midgaard wax seal): 1 reset at South Lantern Gate (`3751`)
- `3815` (Kowloon thread-knot seal): 1 reset at North Lantern Gate (`4233`)
- `3816` (iron-banded road lantern): 3-4 resets at road bend/waystation rooms
- `3825` (luminescent moss jar): 2 resets in Ancestor Deeps luminescent rooms
- `3824` (petrified ironpine bark): 2 resets in Ironpine Rise rooms
- `3828` (carved cairn stone): 2 resets in Ancestor Deeps standing-stone rooms

### Door Resets (D)

As specified in **Door Plan**:
- `D 0 3751 2 1` — South Lantern Gate, south door, closed
- `D 0 4233 0 1` — North Lantern Gate, north door, closed
- `D 0 3912 1 1` — Fen Toll Bridge Gate, east door, closed (optional)
- `D 0 4033 1 1` — Ridge Pass Barricade, east door, closed (optional)
- `D 0 4132 1 1` — Ancestor Ring Ward Gate, east door, closed (optional)
- `D 0 4191 1 1` — Northern Customs Post, east door, closed (optional)

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
- Kowloon's Iron Gate (`3534`) faces "the caravan roads and the lawless wetland frontier" per the Kowloon plan. The forest's northern approach should reflect this: the last few rooms before Kowloon should show increasing signs of Kowloon's administrative presence (customs markers, inspection posts, Wardens of Iron patrol signs).
- Midgaard's Granite Arcade (`925`) is the administrative hub for northern trade. The forest's southern approach should show Midgaard's practical civic character: ranger notice boards, guild contract postings, and warden supply caches.

---

## Quests

Static quests offered through the three regional postmasters for quests set in or motivated by the Great Northern Forest. Quest IDs 0-4 are already defined in `quests/1-5.prop`. The baseline set below uses IDs 15-24 (files `16.prop` through `25.prop`), with an additional boss-focused extension using IDs 35-39 (files `36.prop` through `40.prop`). `PROP_MAX_STATIC_QUESTS` is assumed to be increased to `64`.

Postmaster vnums:
- **Midgaard:** `931` (the Postmaster)
- **Kiess:** `3340` (the Postmaster of Kiess)
- **Kowloon:** `3460` (Postmaster of Kowloon)

### Non-Chain Quests

These standalone quests have no prerequisites (`prerequisite_static_id = -1`) and can be accepted independently.

#### Quest 15 — Lantern Road wolf cull (Midgaard)

| Field | Value |
|---|---|
| **File** | `16.prop` |
| **Static ID** | 15 |
| **Title** | Lantern Road wolf cull |
| **prerequisite_static_id** | -1 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 10 |
| **min_level** | 20 |
| **max_level** | 39 |
| **offerer_vnum** | 931 |
| **reward_gold** | 591 |
| **reward_qp** | 2 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3744` (grey timber wolf) |
| **Accept message** | Midgaard's post office has received a stack of delayed delivery reports citing wolf packs along the southern Lantern Road approach. Rangers estimate the pack has split into multiple hunting parties that rotate ambush positions between road bends. Thin the population enough that courier runners can resume single-escort schedules instead of the current armed-convoy requirement. |
| **Completion message** | Wolf sightings along the southern approach have dropped to seasonal norms, and the post office has downgraded the Lantern Road threat assessment from convoy-only to standard escort. Courier runners are already making faster turnarounds, and the backlog of delayed parcels from the Granite Arcade sorting office is clearing for the first time in weeks. |

#### Quest 16 — Mosswater smuggler interdiction (Kiess)

| Field | Value |
|---|---|
| **File** | `17.prop` |
| **Static ID** | 16 |
| **Title** | Mosswater smuggler interdiction |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 3 |
| **kill_needed** | 0 |
| **min_level** | 30 |
| **max_level** | 54 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 792 |
| **reward_qp** | 3 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3769 3770 3776` (smuggler lookout, fen smuggler runner, fen smuggler boss) |
| **Accept message** | Kiess dispatch has intercepted falsified manifests traced to a smuggling ring operating out of the Mosswater Bends. They use reed-platform camps to stage contraband before running it north along fen channels that bypass the Lantern Road checkpoints entirely. Kiess route clerks need the ring broken at three levels: their lookouts, their runners, and whoever is directing operations from the deep fen. |
| **Completion message** | The smuggling ring's command structure is shattered, and Kiess customs officers are already recovering cached contraband from the reed camps you identified. Fen toll collectors report that legitimate mire-clan traders are moving goods openly again instead of avoiding the channels out of fear of smuggler reprisal. The Kiess post has reopened direct fen-route courier service for the first time this season. |

#### Quest 17 — Northern Crown predator survey (Kowloon)

| Field | Value |
|---|---|
| **File** | `18.prop` |
| **Static ID** | 17 |
| **Title** | Northern Crown predator survey |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 3 |
| **kill_needed** | 0 |
| **min_level** | 40 |
| **max_level** | 69 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 2800 |
| **reward_qp** | 3 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3819 3822 3827` (northern mist viper, frost-touched wolf, mist-shadow cat) |
| **Accept message** | Kowloon's Wardens of Iron need current threat data from the Northern Crown approaches before they can adjust seasonal patrol rotations. Three predator types have been reported in densities that exceed safe caravan transit thresholds: mist vipers in the cold valleys, frost-touched wolves along the ridge corridors, and mist-shadow cats near the tree line. Engage one of each and bring back field assessment notes so the wardens can calibrate their next deployment. |
| **Completion message** | Your field reports give Kowloon's patrol command the data they needed to redraw the Northern Crown escort map. The Wardens of Iron have shifted two squads to cover the mist-valley corridors you flagged, and caravan masters are requesting the updated route charts from the Kowloon post office. Seasonal rotation planning can proceed on schedule instead of defaulting to the expensive all-hands deployment. |

#### Quest 18 — Ironpine Rise Ashfang suppression (Midgaard)

| Field | Value |
|---|---|
| **File** | `19.prop` |
| **Static ID** | 18 |
| **Title** | Ironpine Rise Ashfang suppression |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 3 |
| **kill_needed** | 0 |
| **min_level** | 45 |
| **max_level** | 69 |
| **offerer_vnum** | 931 |
| **reward_gold** | 1116 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3784 3785 3791` (Ashfang scout, Ashfang raider, Ashfang trapper) |
| **Accept message** | Midgaard's ledgerhouse has flagged a sharp increase in cargo losses through the Ironpine Rise, and ranger reports point to Ashfang warbands operating in coordinated cells: scouts mark targets, trappers slow caravans with deadfalls and snares, and raiders hit the stalled columns. The postmaster needs each cell type disrupted so ridge-route dispatches can resume without triple escort surcharges. |
| **Completion message** | Ashfang activity on the Ironpine Rise has fragmented into scattered opportunists now that their scout-trapper-raider coordination is broken. Ridge patrol wardens report clean passage windows on routes that had been effectively closed for weeks. Midgaard's post office has already re-listed the Ironpine route as standard-risk, and convoy insurance rates are dropping back to seasonal norms. |

### Chain Quests

#### Rootbound Purge Chain (Kiess, 3 quests)

A three-part chain investigating and dismantling Rootbound cult operations in the deep forest. Kiess route clerks drive this chain because Rootbound sabotage of road infrastructure directly threatens Kiess-Midgaard courier schedules.

##### Quest 19 — Rootbound perimeter probe (chain start)

| Field | Value |
|---|---|
| **File** | `20.prop` |
| **Static ID** | 19 |
| **Title** | Rootbound perimeter probe |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 55 |
| **max_level** | 79 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 3400 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3801 3802` (Rootbound acolyte, Rootbound ritualist) |
| **Accept message** | Kiess compact archives have correlated a pattern: every time Rootbound cult activity spikes in the Ancestor Deeps, bridge pilings and lamp-post footings along the Lantern Road show fresh sabotage within days. The clerks believe the cult's outer ring of acolytes and ritualists are the ones carrying out infrastructure attacks between their rites. Engage both tiers and recover any materials that confirm the sabotage-to-ritual pipeline. |
| **Completion message** | The materials you recovered confirm what Kiess archivists suspected: the Rootbound outer circle treats road sabotage as a devotional act, carving rune-marks into bridge timbers before weakening them. With their acolyte and ritualist cadres disrupted, the cult's ability to project sabotage beyond their deep-forest sanctuaries is severely diminished. Kiess road maintenance crews can repair the damage faster than the cult can replace its losses. |

##### Quest 20 — Rootbound lieutenant strike (chain middle)

| Field | Value |
|---|---|
| **File** | `21.prop` |
| **Static ID** | 20 |
| **Title** | Rootbound lieutenant strike |
| **prerequisite_static_id** | 19 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 70 |
| **max_level** | 94 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 5200 |
| **reward_qp** | 5 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3807 3835` (Thornmother's lieutenant, Rootbound briar knight) |
| **Accept message** | Your earlier probe forced the Rootbound cult to consolidate around its inner circle. Kiess intelligence now has positions on two key figures: a lieutenant who coordinates the Thornmother's field operations, and a briar knight who commands the cult's armed enforcers in the northern groves. Removing them will isolate the Thornmother from her remaining followers and open a path for the final strike. |
| **Completion message** | With the lieutenant and briar knight eliminated, the Rootbound cult's command chain between the Thornmother and her remaining followers is severed. Kiess field agents report that scattered cultists are retreating deeper into the forest instead of holding their former positions. The path to the Thornmother's inner sanctum is now exposed, though the cult's remaining defenses will not be trivial. |

##### Quest 21 — Thornmother sanctum assault (chain end)

| Field | Value |
|---|---|
| **File** | `22.prop` |
| **Static ID** | 21 |
| **Title** | Thornmother sanctum assault |
| **prerequisite_static_id** | 20 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 1 |
| **min_level** | 85 |
| **max_level** | 5 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 8000 |
| **reward_qp** | 7 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3824` (the Thornmother) |
| **Accept message** | The outer ring is broken and the inner circle is scattered. Kiess compact clerks have authorized the final warrant: engage the Thornmother herself in her hidden grove deep in the Ancestor Deeps. She is the living heart of the Rootbound cult, and Kowloon's Wardens of Iron have matched the Kiess bounty with their own standing order. End this threat before the cult can rebuild its chain of command. |
| **Completion message** | The Thornmother has fallen, and with her the Rootbound cult's claim to the deep forest. Kiess and Kowloon officials are already sending joint survey teams into the former cult territory to assess the damage and begin reclaiming sabotaged road sections. For the first time in years, the Ancestor Deeps are open to legitimate passage without cult interference. Both post offices have issued commendations naming your work as the decisive action. |

#### Ancestor Deeps Reclamation Chain (Kowloon, 3 quests)

A three-part chain clearing the Ancestor Deeps of hostile factions threatening Kowloon's southern trade security. Kowloon's postmaster drives this chain because the deep-forest threats directly endanger the northern approach to the Iron Gate.

##### Quest 22 — Cairn-Keeper exorcism (chain start)

| Field | Value |
|---|---|
| **File** | `23.prop` |
| **Static ID** | 22 |
| **Title** | Cairn-Keeper exorcism |
| **prerequisite_static_id** | -1 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 6 |
| **min_level** | 60 |
| **max_level** | 84 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 4000 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3800` (Cairn-Keeper ghost) |
| **Accept message** | Kowloon's Jade Magistracy has received formal complaints from caravan masters who report that Cairn-Keeper ghosts in the Ancestor Deeps have become actively hostile to all traffic, not merely trespassers at the standing stones. The magistracy believes the recent Rootbound cult activity has disturbed the burial sites and agitated the spirits beyond their traditional territorial behavior. Reduce the ghost population to levels the wardens can manage through standard containment patrols. |
| **Completion message** | The Cairn-Keeper ghost activity has subsided to levels consistent with historical norms. Kowloon's wardens report that the remaining spirits have retreated to their standing stone sites instead of ranging across trade corridors. The Jade Magistracy has formally noted the action in their seasonal security ledger and adjusted the Ancestor Deeps threat classification downward. Caravan masters are accepting contracts through the deep route again. |

##### Quest 23 — Ashfang war-chief elimination (chain middle)

| Field | Value |
|---|---|
| **File** | `24.prop` |
| **Static ID** | 23 |
| **Title** | Ashfang war-chief elimination |
| **prerequisite_static_id** | 22 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 75 |
| **max_level** | 99 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 6000 |
| **reward_qp** | 5 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3806 3812` (Ashfang war-chief, deep Ashfang berserker) |
| **Accept message** | With the Cairn-Keeper spirits contained, Kowloon intelligence has confirmed that the Ashfang warbands exploited the ghost disruption to expand their deep-forest camps. Their war-chief has established a fortified position in the Ancestor Deeps, supported by berserker enforcers who control the approaches. Kowloon's Wardens of Iron cannot commit a full garrison sortie without leaving the Iron Gate undermanned. Remove the war-chief and his berserker guard to collapse the Ashfang deep-forest presence. |
| **Completion message** | The Ashfang war-chief's deep camp has been broken, and surviving warband members are scattering toward the Ironpine Rise where ridge patrol wardens can contain them. Kowloon's southern approach is measurably safer, and the Wardens of Iron have redeployed the emergency reserve back to standard rotation. The postmaster has flagged your action in the next dispatch to Kowloon's Harbor Syndics as evidence that the forest route remains commercially viable. |

##### Quest 24 — Oathbreaker wraith banishment (chain end)

| Field | Value |
|---|---|
| **File** | `25.prop` |
| **Static ID** | 24 |
| **Title** | Oathbreaker wraith banishment |
| **prerequisite_static_id** | 23 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 80 |
| **max_level** | 5 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 1632 |
| **reward_qp** | 6 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3823 3834` (oathbreaker wraith, cold ancestor spirit) |
| **Accept message** | The living threats are contained, but the deepest corruption remains: oathbreaker wraiths and cold ancestor spirits that haunt the Northern Crown's ruined oath-steles. Kowloon's covenant tradition considers these entities an affront to the compact that binds both cities. The wraiths are the spirits of those who violated the original forest charter, and their continued presence poisons the ground where Midgaard and Kowloon first swore mutual obligation. Banish them so the oath-stele sites can be reconsecrated. |
| **Completion message** | The oathbreaker wraiths have been banished and the ancestor spirits have been laid to rest. Kowloon's covenant scholars report that the oath-stele sites are already showing signs of recovery, with the oppressive cold lifting and the corruption receding from the surrounding stones. The Jade Magistracy has authorized a formal reconsecration ceremony, and both cities' wardens have been invited to witness the renewal of the original forest charter marks. The northern forest passage is as secure as it has been in living memory. |

### Quest Summary Table

| File | Static ID | Title | Prereq ID | Type | Targets | Kill Needed | Level Range | Offerer | Rewards | Chain |
|---|---:|---|---:|---:|---|---:|---|---:|---|---|
| `16.prop` | 15 | Lantern Road wolf cull | -1 | 3 | `3744` | 10 | 20-39 | 931 | 591 gold, 2 qp | — |
| `17.prop` | 16 | Mosswater smuggler interdiction | -1 | 1 | `3769 3770 3776` | 0 | 30-54 | 3340 | 792 gold, 3 qp | — |
| `18.prop` | 17 | Northern Crown predator survey | -1 | 1 | `3819 3822 3827` | 0 | 40-69 | 3460 | 2800 gold, 3 qp | — |
| `19.prop` | 18 | Ironpine Rise Ashfang suppression | -1 | 1 | `3784 3785 3791` | 0 | 45-69 | 931 | 1116 gold, 4 qp | — |
| `20.prop` | 19 | Rootbound perimeter probe | -1 | 1 | `3801 3802` | 0 | 55-79 | 3340 | 3400 gold, 4 qp | Rootbound 1/3 |
| `21.prop` | 20 | Rootbound lieutenant strike | 19 | 1 | `3807 3835` | 0 | 70-94 | 3340 | 5200 gold, 5 qp | Rootbound 2/3 |
| `22.prop` | 21 | Thornmother sanctum assault | 20 | 3 | `3824` | 1 | 85-5 | 3340 | 8000 gold, 7 qp | Rootbound 3/3 |
| `23.prop` | 22 | Cairn-Keeper exorcism | -1 | 3 | `3800` | 6 | 60-84 | 3460 | 4000 gold, 4 qp | Reclamation 1/3 |
| `24.prop` | 23 | Ashfang war-chief elimination | 22 | 1 | `3806 3812` | 0 | 75-99 | 3460 | 6000 gold, 5 qp | Reclamation 2/3 |
| `25.prop` | 24 | Oathbreaker wraith banishment | 23 | 1 | `3823 3834` | 0 | 80-5 | 3460 | 1632 gold, 6 qp | Reclamation 3/3 |
| `36.prop` | 35 | Fen kingpin decapitation | -1 | 3 | `3776` | 1 | 46-72 | 931 | 1366 gold, 4 qp | — |
| `37.prop` | 36 | Crown alpha hunt | -1 | 1 | `3826 3828` | 0 | 82-120 | 3340 | 6800 gold, 6 qp | — |
| `38.prop` | 37 | Briar throne breach | -1 | 3 | `3824` | 1 | 90-5 | 3460 | 9000 gold, 7 qp | Oathnight 1/3 |
| `39.prop` | 38 | Covenant ash purge | 37 | 1 | `3806 3828` | 0 | 92-5 | 3460 | 2539 gold, 8 qp | Oathnight 2/3 |
| `40.prop` | 39 | Oathnight final rite | 38 | 1 | `3823 3834` | 0 | 95-5 | 3460 | 11000 gold, 9 qp | Oathnight 3/3 |

### Quest Design Notes

- **Postmaster distribution (including IDs 35-39):** 3 Midgaard (IDs 15, 18, 35), 5 Kiess (IDs 16, 19, 20, 21, 36), 7 Kowloon (IDs 17, 22, 23, 24, 37, 38, 39).
- **Kiess dominance in Rootbound chain:** Kiess route clerks and compact archives drive the cult investigation because Rootbound sabotage targets road infrastructure that Kiess's courier network depends on. The chain escalates from outer-ring disruption through inner-circle strikes to the boss encounter.
- **Kowloon dominance in Reclamation chain:** Kowloon's covenant tradition and Jade Magistracy security concerns drive the deep-forest clearing because the Ancestor Deeps threats directly endanger Kowloon's southern trade approaches. The chain escalates from undead containment through Ashfang power removal to spiritual purification.
- **Midgaard practical-threat focus:** Midgaard quests prioritize direct commerce disruption issues (wolves, Ashfang raiding, and fen smuggler leadership) consistent with Midgaard's pragmatic civic character.
- **Level band coverage:** The 15 quests span levels 20-5, with non-chain quests covering lower-to-mid bands and chains escalating through high-end forest and border content.
- **Type variety:** 5 kill-count quests (IDs 15, 21, 22, 35, 37) and 10 kill-variety quests (IDs 16, 17, 18, 19, 20, 23, 24, 36, 38, 39).
- **All target vnums are mobs defined in this area plan** (3739-3838 mob range).
- **Reward scaling** follows the pattern established by existing quests 0-4: gold and qp increase with level range and chain depth.
- **`PROP_MAX_STATIC_QUESTS`** must be increased from `16` to `64` to support IDs beyond 15 in future areas.

### Additional Boss-Focused Quests (IDs 35-39)

The following five quests extend the forest board with boss-target contracts only. They mix standalone and chained progression while staying anchored to The Forest of Confusion's corruption, oath-fracture, and border-security themes.

#### Quest 35 — Fen kingpin decapitation (Midgaard)

| Field | Value |
|---|---|
| **File** | `36.prop` |
| **Static ID** | 35 |
| **Title** | Fen kingpin decapitation |
| **prerequisite_static_id** | -1 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 1 |
| **min_level** | 46 |
| **max_level** | 72 |
| **offerer_vnum** | 931 |
| **reward_gold** | 1366 |
| **reward_qp** | 4 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3776` (fen smuggler boss) |
| **Accept message** | Midgaard's Granite Arcade has traced another burst of forged courier seals to the Mosswater ring, proving the old smuggler command never truly collapsed. The same fen kingpin has resumed bribing river guides and diverting sealed post bags into the Confusion thickets where records vanish. Eliminate the smuggler boss outright so Midgaard can reassert lawful carriage over the southern lantern route. |
| **Completion message** | With the fen smuggler boss dead, Midgaard auditors have recovered stolen satchels, broken counterfeit stamp blocks, and restored chain-of-custody logs for the southern dispatch lanes. Rangers report that smuggler lookouts are scattering without central command, and convoy captains are filing normal route plans again. |

#### Quest 36 — Crown alpha hunt (Kiess)

| Field | Value |
|---|---|
| **File** | `37.prop` |
| **Static ID** | 36 |
| **Title** | Crown alpha hunt |
| **prerequisite_static_id** | -1 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 82 |
| **max_level** | 120 |
| **offerer_vnum** | 3340 |
| **reward_gold** | 6800 |
| **reward_qp** | 6 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3826 3828` (the Primal Bear Lord, northern Ashfang warmaster) |
| **Accept message** | Kiess route-maps show that two apex predators now dominate the same northern lanes: the Primal Bear Lord and an Ashfang warmaster driving raiders behind it. That pairing has turned old escape paths into kill-boxes where patrols disappear before signal flares can be lit. Break both alpha threats so Kiess can reopen courier scouting beyond the Crown ridges. |
| **Completion message** | Both Crown alphas are down, and Kiess scouts confirm the pressure front around the northern ridges has collapsed. Surviving Ashfang cells have withdrawn to fragmented camps while the great bear's roaming territory is no longer forcing caravans into dead-end gullies. Relay routes marked abandoned since the last ash winter are being reinstated. |

#### Quest 37 — Briar throne breach (Kowloon)

| Field | Value |
|---|---|
| **File** | `38.prop` |
| **Static ID** | 37 |
| **Title** | Briar throne breach |
| **prerequisite_static_id** | -1 |
| **type** | 3 (KILL_COUNT) |
| **num_targets** | 1 |
| **kill_needed** | 1 |
| **min_level** | 90 |
| **max_level** | 5 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 9000 |
| **reward_qp** | 7 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3824` (the Thornmother) |
| **Accept message** | Kowloon's Courier Lantern Office has elevated the Rootbound crisis to covenant emergency status after fresh briar growth sealed two ward-stone paths overnight. Scribes believe the Thornmother is reweaving confusion wards around the old oath corridor, preventing lawful passage and feeding corruption into the north gate approaches. Enter the sanctum and sever the Briar Throne by slaying the Thornmother. |
| **Completion message** | The Thornmother has fallen, and Kowloon wardens report immediate weakening of the briar wards strangling the covenant lanes. Inspectors from the Iron Gate confirm that navigational glyphs once scrambled by Confusion haze are stable again. The Jade Magistracy has authorized full-scale reclamation teams to move into the cleared sanctum perimeter. |

#### Quest 38 — Covenant ash purge (Kowloon, chain 2/3)

| Field | Value |
|---|---|
| **File** | `39.prop` |
| **Static ID** | 38 |
| **Title** | Covenant ash purge |
| **prerequisite_static_id** | 37 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 92 |
| **max_level** | 5 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 2539 |
| **reward_qp** | 8 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3806 3828` (Ashfang war-chief, northern Ashfang warmaster) |
| **Accept message** | Thornmother's fall exposed the Ashfang command lattice that had been using her confusion mists as battlefield cover. Kowloon covenant marshals now require a coordinated purge of both remaining Ashfang war leaders before they can regroup and rearm the border camps. Cut down the war-chief and the warmaster to end organized ashfang pressure on the northern charter road. |
| **Completion message** | Both Ashfang commanders are dead, and intercepted battle-calls indicate no surviving officer has authority to unify the splintered warbands. Kowloon customs patrols report that ambush banners have been stripped from the old ridge chokepoints. The covenant road is open for the final spiritual cleansing phase. |

#### Quest 39 — Oathnight final rite (Kowloon, chain 3/3)

| Field | Value |
|---|---|
| **File** | `40.prop` |
| **Static ID** | 39 |
| **Title** | Oathnight final rite |
| **prerequisite_static_id** | 38 |
| **type** | 1 (KILL_VARIETY) |
| **num_targets** | 2 |
| **kill_needed** | 0 |
| **min_level** | 95 |
| **max_level** | 5 |
| **offerer_vnum** | 3460 |
| **reward_gold** | 11000 |
| **reward_qp** | 9 |
| **reward_item_vnum** | 0 |
| **reward_item_count** | 0 |
| **Target vnums** | `3823 3834` (oathbreaker wraith, cold ancestor spirit) |
| **Accept message** | With Rootbound briars severed and Ashfang command broken, Kowloon can finally attempt the Oathnight rite that restores the original Midgaard-Kowloon covenant markers. The rite cannot hold while oathbreaker wraiths and cold ancestor spirits still infest the stele ring, twisting every vow into static and frost. Destroy both spiritual bosses so the charter stones can be bound in living law once more. |
| **Completion message** | The final rite succeeded after the wraith and ancestor spirit were destroyed, and the stele ring now carries a stable covenant resonance recognized by both cities' heralds. Couriers from Midgaard and Kowloon completed the first dual-sealed run through the forest without losing route bearings to Confusion haze. By decree of the Jade Magistracy, the Oathnight corridor is restored to lawful passage. |

---

## Compliance Checklist
- [x] Plan only (no area file generation)
- [x] Uses `docs/area_file_spec.md` constraints in planning
- [x] 500-room plan with all vnums used (`3739-4238`)
- [x] Irregular overall layout with explicit meandering road
- [x] Multi-color theme defined (no `@@k`, no background codes, no flashing)
- [x] Extensive lore connecting Midgaard (south) and Kowloon (north)
- [x] Doors explicitly specified with `EX_ISDOOR` bit, reset states, and key vnums
- [x] Room layout included (full 20x25 vnum grid)
- [x] Road mobs around level 20 in south approach; stronger farther away
- [x] Farthest pockets cap around level 100
- [x] Mob containment strategy enforced (`sentinel` and/or `no_mob` boundaries; all mobs get `stay_area`)
- [x] South connection identified: `3751` south to Midgaard `925` (Granite Arcade)
- [x] North connection identified: `4233` north to Kowloon `3534` (Iron Gate)
- [ ] West connections (Planned — requires `cinderteeth_mountains.are`): `4179` west → 6627; `4199` west → 6633; `4219` west → 6639. Add `D3` exits on all three rooms when Cinderteeth is built.
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
- [x] Quests section with 15 static quests (IDs 15-24 and 35-39) across three postmasters
- [x] Mix of non-chain and chain quests, including a boss-focused Oathnight chain (37->38->39)
- [x] All quest target vnums reference mobs defined in this area plan
