# ACK MUD Core Data Structure Specifications

This document describes the four primary data structures used in ACK MUD:
`CHAR_DATA`, `PC_DATA`, `OBJ_DATA`, and `ROOM_INDEX_DATA` (the in-code name for
what is commonly called "ROOM_DATA").  All definitions live in
`src/ack.h` unless noted otherwise.

---

## Table of Contents

1. [CHAR_DATA](#char_data)
2. [PC_DATA](#pc_data)
3. [OBJ_DATA](#obj_data)
4. [ROOM_INDEX_DATA (ROOM_DATA)](#room_index_data)

---

## CHAR_DATA

`struct char_data` — defined at `src/ack.h:551`

Represents a single character, whether a player character (PC) or a
non-player character (NPC/mob).  Every creature in the world is allocated as
a `CHAR_DATA`.  PC-only information is stored in the linked `PC_DATA` block
pointed to by `pcdata`; that pointer is `NULL` for mobs.

### Linked-list / housekeeping

| Field | Type | Description |
|---|---|---|
| `is_free` | `bool` | Guard flag used by LINK/UNLINK macros to catch double-free bugs (Ramias) |
| `next` | `CHAR_DATA *` | Next character in the global character list |
| `prev` | `CHAR_DATA *` | Previous character in the global character list |
| `next_in_room` | `CHAR_DATA *` | Next character in the same room |
| `prev_in_room` | `CHAR_DATA *` | Previous character in the same room |

### Relationships

| Field | Type | Description |
|---|---|---|
| `master` | `CHAR_DATA *` | The character this one follows or is charmed by |
| `leader` | `CHAR_DATA *` | Group leader |
| `fighting` | `CHAR_DATA *` | Current combat target |
| `reply` | `CHAR_DATA *` | Last character to send a tell (used by `reply` command) |
| `hunting` | `CHAR_DATA *` | PC or mob this character is actively hunting |
| `hunt_obj` | `OBJ_DATA *` | Object this character is hunting for |
| `hunt_for` | `CHAR_DATA *` | Employer (for mercenaries / critters) |
| `hunt_home` | `ROOM_INDEX_DATA *` | Room to return to after a hunt concludes |
| `searching` | `char *` | Hunt target name preserved across a quit |
| `hunt_flags` | `int` | Bitfield controlling hunting behaviour |
| `old_body` | `CHAR_DATA *` | Original body when a wizard has `switch`ed into an NPC |
| `num_followers` | `sh_int` | Number of active followers / charmies |
| `extract_timer` | `sh_int` | Countdown before auto-extracting charmed mobs |
| `ngroup` | `NPC_GROUP_DATA *` | AI grouping data for mobs |

### Identity & description

| Field | Type | Description |
|---|---|---|
| `name` | `char *` | Character name (keyword for targeting) |
| `short_descr` | `char *` | Short description shown in room character lists |
| `long_descr` | `char *` | Description printed when a character enters the room |
| `long_descr_orig` | `char *` | Unmodified copy of `long_descr` |
| `description` | `char *` | Full description shown on `look <char>` |
| `prompt` | `char *` | Custom command prompt string |
| `old_prompt` | `char *` | Saved prompt used while the player is in an editor |
| `sex` | `sh_int` | Sex: male, female, or neutral |
| `login_sex` | `sh_int` | Sex selected at character creation (temporary) |

### Pointers to related structures

| Field | Type | Description |
|---|---|---|
| `pIndexData` | `MOB_INDEX_DATA *` | Mob template (prototype); `NULL` for PCs |
| `desc` | `DESCRIPTOR_DATA *` | Network connection descriptor; `NULL` for mobs |
| `pcdata` | `PC_DATA *` | Player-only data block; `NULL` for mobs |
| `spec_fun` | `SPEC_FUN *` | Special-function callback for mob AI |
| `switched` | `bool` | `true` when this descriptor is controlling another body |

### Affects & notes

| Field | Type | Description |
|---|---|---|
| `first_affect` | `AFFECT_DATA *` | Head of applied magical affects list |
| `last_affect` | `AFFECT_DATA *` | Tail of applied magical affects list |
| `first_saved_aff` | `AFFECT_DATA *` | Head of saved (persistent) affects list |
| `last_saved_aff` | `AFFECT_DATA *` | Tail of saved affects list |
| `pnote` | `NOTE_DATA *` | Note currently being read or composed |
| `first_shield` | `MAGIC_SHIELD *` | Head of active magic shields list |
| `last_shield` | `MAGIC_SHIELD *` | Tail of active magic shields list |
| `current_brand` | `BRAND_DATA *` | Active brand / enchantment on the character |

### Location & inventory

| Field | Type | Description |
|---|---|---|
| `in_room` | `ROOM_INDEX_DATA *` | Room the character is currently in |
| `was_in_room` | `ROOM_INDEX_DATA *` | Previous room (saved across death, link-death, etc.) |
| `first_carry` | `OBJ_DATA *` | Head of carried inventory list |
| `last_carry` | `OBJ_DATA *` | Tail of carried inventory list |
| `sitting` | `OBJ_DATA *` | Furniture object the character is resting or sleeping on |

### Class, race & levels

| Field | Type | Description |
|---|---|---|
| `class` | `sh_int` | Primary character class index |
| `race` | `sh_int` | Character race index |
| `clan` | `sh_int` | Clan membership index |
| `level` | `sh_int` | Effective level (maximum of all class levels) |
| `lvl[MAX_CLASS]` | `int []` | Per-class levels for multi-class characters |
| `remort[MAX_REMORT]` | `int []` | Remort (reincarnation) level data |
| `adept[MAX_CLASS]` | `int []` | Adept level per class |
| `adept_level` | `int` | Current adept tier |
| `poly_level` | `int` | Polymorphism level |

### Vital statistics

| Field | Type | Description |
|---|---|---|
| `hit` | `long` | Current hit points |
| `max_hit` | `long` | Maximum hit points |
| `mana` | `long` | Current mana |
| `max_mana` | `long` | Maximum mana |
| `move` | `long` | Current movement points |
| `max_move` | `long` | Maximum movement points |
| `gold` | `int` | Gold coins carried |
| `balance` | `int` | Bank account balance |
| `exp` | `long` | Experience points |
| `intell_exp` | `int` | Intelligence-scaled experience |
| `chi` | `int` | Chi / Ki energy |
| `holy_power` | `int` | Holy power resource |
| `arcane_power` | `int` | Arcane power resource |
| `psionic_power` | `int` | Psionic power resource |
| `quest_points` | `int` | Quest points |
| `money` | `MONEY_TYPE *` | Multi-currency wallet |
| `bank_money` | `MONEY_TYPE *` | Multi-currency bank balance |

### Combat & modifiers

| Field | Type | Description |
|---|---|---|
| `alignment` | `sh_int` | Alignment score (−1000 to +1000) |
| `hitroll` | `int` | Base bonus to attack rolls |
| `damroll` | `int` | Base bonus to damage rolls |
| `armor` | `int` | Armour class value |
| `saving_throw` | `sh_int` | Saving throw modifier |
| `hp_mod` | `long` | HP modifier from equipment / spells |
| `ac_mod` | `int` | AC modifier |
| `hr_mod` | `int` | Hit-roll modifier |
| `dr_mod` | `int` | Damage-roll modifier |
| `spellpower_mod` | `int` | Spell power modifier |
| `healing_mod` | `int` | Healing effectiveness modifier |
| `crit_mod` | `int` | Melee critical-hit chance modifier |
| `crit_mult_mod` | `int` | Melee critical-hit multiplier modifier |
| `spell_crit_mod` | `int` | Spell critical-hit chance modifier |
| `spell_mult_mod` | `int` | Spell critical-hit multiplier modifier |
| `parry_mod` | `int` | Parry modifier |
| `dodge_mod` | `int` | Dodge modifier |
| `block_mod` | `int` | Block modifier |
| `pierce_mod` | `int` | Armour-pierce modifier |
| `resist` | `int` | Damage-type resistance flags |
| `suscept` | `int` | Damage-type susceptibility flags |
| `stance` | `int` | Combat stance index |
| `target` | `char *` | Name of the last attacker (used for auto-retaliation) |
| `wimpy` | `sh_int` | HP threshold for automatic fleeing |
| `stunTimer` | `int` | Remaining stun duration (ticks) |
| `combo[MAX_COMBO]` | `int []` | In-progress combo attack state |

### Mob combat stats

| Field | Type | Description |
|---|---|---|
| `skills` | `int` | Mob melee skill level |
| `cast` | `int` | Mob spellcasting ability |
| `def` | `int` | Mob defence rating |
| `strong_magic` | `int` | Bonus to certain magic schools |
| `weak_magic` | `int` | Penalty to certain magic schools |
| `race_mods` | `int` | Race-derived modifiers |
| `power_skills` | `int` | Power-skill points |
| `power_cast` | `int` | Power-casting points |
| `loot_amount` | `int` | Number of loot items dropped on death |
| `loot_chance[MAX_LOOT]` | `int []` | Drop-chance per loot slot |
| `loot[MAX_LOOT]` | `int []` | VNUM per loot slot |

### State & flags

| Field | Type | Description |
|---|---|---|
| `act` | `int` | Action / behaviour flags (ACT_*) |
| `config` | `int` | Player configuration flags |
| `affected_by` | `int` | Currently active affect flags |
| `position` | `sh_int` | Current position (standing, sitting, sleeping, …) |
| `trust` | `sh_int` | Immortal trust level |
| `wizbit` | `bool` | Wizard (immortal) flag |
| `invis` | `sh_int` | Wizard-invisibility level |
| `incog` | `sh_int` | Incognito level |
| `deaf` | `int` | Deafness channel bitfield |
| `spec_behavior` | `int` | Mob special-behaviour flags |
| `sentence` | `int` | Prison-sentence duration (legal system) |
| `act_build` | `int` | Active OLC / building action |
| `build_vnum` | `int` | VNUM currently being edited (−1 = none) |
| `using_named_door` | `bool` | Set while the character is using a named-door command |

### Time & tracking

| Field | Type | Description |
|---|---|---|
| `played` | `int` | Total play time in minutes |
| `logon` | `time_t` | Unix timestamp of last login |
| `save_time` | `time_t` | Unix timestamp of last save |
| `last_note` | `time_t` | Unix timestamp of last note interaction |
| `timer` | `sh_int` | Heartbeat idle timer |
| `wait` | `sh_int` | Command lag counter (ticks remaining) |
| `cooldown[MAX_SKILL]` | `sh_int []` | Per-skill cooldown timers |
| `is_quitting` | `bool` | Set to `true` while the character is in the quit process |

### Carry limits

| Field | Type | Description |
|---|---|---|
| `practice` | `int` | Practice sessions available |
| `carry_weight` | `float` | Current carried weight |
| `carry_number` | `sh_int` | Current number of carried items |

### program scripting

| Field | Type | Description |
|---|---|---|
| `first_mpact` | `MPROG_ACT_LIST *` | Head of pending program action list |
| `last_mpact` | `MPROG_ACT_LIST *` | Tail of pending program action list |
| `mpactnum` | `int` | Count of pending program actions |

---

## PC_DATA

`struct pc_data` — defined at `src/ack.h:711`

Player-character-only data.  Every logged-in PC has exactly one `PC_DATA`
block; it is accessed via `ch->pcdata`.  NPCs always have `pcdata == NULL`.

### Linked-list / housekeeping

| Field | Type | Description |
|---|---|---|
| `is_free` | `bool` | Guard flag used by LINK/UNLINK macros |
| `next` | `PC_DATA *` | Next `PC_DATA` in the global player list |
| `prev` | `PC_DATA *` | Previous `PC_DATA` in the global player list |

### Account & login

| Field | Type | Description |
|---|---|---|
| `pwd` | `char *` | Encrypted (crypt'd) password |
| `host` | `char *` | Hostname or IP address of the last login |
| `failures` | `sh_int` | Consecutive failed login attempts |
| `lastlogin` | `char *` | Human-readable date/time of last successful login |
| `email_address` | `char *` | Player's registered email address |
| `valid_email` | `bool` | Whether the email address has been verified |

### Cosmetic / display

| Field | Type | Description |
|---|---|---|
| `color[MAX_COLOR]` | `int []` | ANSI color preferences by category |
| `hicol` | `char` | High-brightness colour code preference |
| `dimcol` | `char` | Dim colour code preference |
| `bamfin` | `char *` | Custom arrival message (replaces default "arrives") |
| `bamfout` | `char *` | Custom departure message |
| `room_enter` | `char *` | Custom message sent to the room on entry |
| `room_exit` | `char *` | Custom message sent to the room on exit |
| `title` | `char *` | Title string shown after the character name |
| `who_name` | `char *` | Alternate name shown on the `who` list |
| `load_msg` | `char *` | Message displayed to the player on login |
| `pagelen` | `sh_int` | Lines per page for paged output |
| `term_rows` | `sh_int` | Terminal height in rows |
| `term_columns` | `sh_int` | Terminal width in columns |

### Attributes (permanent, maximum, modifier)

| Field | Type | Description |
|---|---|---|
| `perm_str` | `sh_int` | Permanent (base) strength |
| `perm_int` | `sh_int` | Permanent intelligence |
| `perm_wis` | `sh_int` | Permanent wisdom |
| `perm_dex` | `sh_int` | Permanent dexterity |
| `perm_con` | `sh_int` | Permanent constitution |
| `max_str` | `sh_int` | Racial / class maximum strength |
| `max_int` | `sh_int` | Racial / class maximum intelligence |
| `max_wis` | `sh_int` | Racial / class maximum wisdom |
| `max_dex` | `sh_int` | Racial / class maximum dexterity |
| `max_con` | `sh_int` | Racial / class maximum constitution |
| `mod_str` | `sh_int` | Temporary strength modifier (spells, equipment) |
| `mod_int` | `sh_int` | Temporary intelligence modifier |
| `mod_wis` | `sh_int` | Temporary wisdom modifier |
| `mod_dex` | `sh_int` | Temporary dexterity modifier |
| `mod_con` | `sh_int` | Temporary constitution modifier |

### Survival needs

| Field | Type | Description |
|---|---|---|
| `condition[3]` | `sh_int []` | Hunger, thirst, and drunk levels |

### Skills

| Field | Type | Description |
|---|---|---|
| `learned[MAX_SKILL]` | `unsigned int []` | Proficiency percentage (0–100) per skill slot |
| `immskll` | `char *` | Immortal-granted skill list string |

### Multi-class ordering

| Field | Type | Description |
|---|---|---|
| `order[MAX_CLASS]` | `sh_int []` | Class preference order |
| `index[MAX_CLASS]` | `sh_int []` | Ordering index per class |

### Notes / boards

| Field | Type | Description |
|---|---|---|
| `header` | `char *` | Subject header for a note being composed |
| `message` | `char *` | Body of the note being composed |

### Aliases

| Field | Type | Description |
|---|---|---|
| `alias_name[MAX_ALIASES]` | `char *[]` | Alias trigger words |
| `alias[MAX_ALIASES]` | `char *[]` | Expansion strings for each alias |

### PvP & kill tracking

| Field | Type | Description |
|---|---|---|
| `pkills` | `int` | Player-kill count |
| `pkilled` | `int` | Times killed by a player |
| `mkills` | `int` | Mob-kill count |
| `mkilled` | `int` | Times killed by a mob |
| `pflags` | `int` | Player-specific flags (PK status, etc.) |

### Social / clan

| Field | Type | Description |
|---|---|---|
| `clan` | `sh_int` | Clan membership ID |
| `ruler_rank` | `sh_int` | Rank in the influence / ruler system |
| `generation` | `int` | Vampire generation or Werewolf age |
| `monitor` | `int` | Channel bitfield for immortal monitoring |

### Ignore list

| Field | Type | Description |
|---|---|---|
| `ignore_list[MAX_IGNORES]` | `char *[]` | Names of players being ignored |

### Player keep / housing

| Field | Type | Description |
|---|---|---|
| `recall_vnum` | `sh_int` | VNUM of the character's recall room |
| `keep_vnum` | `int` | VNUM of the player's keep |
| `keep_healer_bought` | `int` | Whether a keep healer has been purchased |
| `keep_healer_vnum` | `int` | Room VNUM of the keep healer |

### Reincarnation / remort

| Field | Type | Description |
|---|---|---|
| `reincarnations[MAX_CLASS]` | `int []` | Reincarnation count per class |
| `remort_reincarnations[MAX_REMORT]` | `int []` | Reincarnation count per remort tier |
| `adept_reincarnations[MAX_CLASS]` | `int []` | Adept reincarnation count |
| `reincarnation_data[MAX_REINCARNATE]` | `int []` | General reincarnation tracking data |
| `reincarnate_race` | `int` | Race selected for next reincarnation |
| `reincarnate_order[MAX_CLASS]` | `int []` | Class order for next reincarnation |
| `reincarnate_confirm` | `int` | Confirmation flag for pending reincarnation |
| `has_exp_fix` | `sh_int` | Whether an experience-fix migration has been applied |

### Stat gains from levelling (non-item)

| Field | Type | Description |
|---|---|---|
| `mana_from_gain` | `int` | Mana gained through levelling (excluding items) |
| `hp_from_gain` | `int` | HP gained through levelling |
| `move_from_gain` | `int` | Movement gained through levelling |

### Quest system

| Field | Type | Description |
|---|---|---|
| `quest_points` | `sh_int` | Quest points earned |
| `invasion_points` | `int` | Points earned during invasion events |
| `invasion_rewards[3]` | `int []` | Invasion reward slots |
| `quest_points` | `int` | Quest quest points |
| `prop_type` | `int` | Active quest type (`PROP_TYPE_*`; 0 = none) |
| `prop_completed` | `bool` | `true` when the quest is ready to hand in |
| `prop_num_targets` | `int` | Number of active target slots (1–5) |
| `prop_target_vnum[PROP_MAX_TARGETS]` | `int []` | Mob or object VNUM per target slot |
| `prop_target_done[PROP_MAX_TARGETS]` | `bool []` | Completion flag per target slot |
| `prop_kill_needed` | `int` | Type-3 quest: total kills required |
| `prop_kill_count` | `int` | Type-3 quest: kills completed so far |

### Vampire / Werewolf specifics

| Field | Type | Description |
|---|---|---|
| `bloodlust` | `int` | Werewolf rage points |
| `bloodlust_max` | `int` | Maximum werewolf rage |
| `vamp_level` | `int` | Vampire / werewolf form level |
| `vamp_bloodlust_max` | `int` | Maximum vampire bloodlust |
| `vamp_skill_num` | `int` | Active vampire skill slot |
| `vamp_skill_max` | `int` | Maximum vampire skills available |
| `vamp_bloodline` | `int` | Vampire bloodline type |
| `vamp_exp` | `int` | Vampire-specific experience |
| `vamp_pracs` | `int` | Vampire practice sessions |

### OLC / property editing

| Field | Type | Description |
|---|---|---|
| `pedit_state` | `char *` | Current OLC editor state string |
| `pedit_string[5]` | `char *[]` | Strings held during property editing |

---

## OBJ_DATA

`struct obj_data` — defined at `src/ack.h:911`

Represents a single instance of an object in the game world.  Permanent
template information (name, base stats) lives in `OBJ_INDEX_DATA`; `OBJ_DATA`
holds the per-instance mutable state.

### Linked-list / housekeeping

| Field | Type | Description |
|---|---|---|
| `is_free` | `bool` | Guard flag used by LINK/UNLINK macros |
| `next` | `OBJ_DATA *` | Next object in the global object list |
| `prev` | `OBJ_DATA *` | Previous object in the global object list |

### Location chains

| Field | Type | Description |
|---|---|---|
| `next_in_carry_list` | `OBJ_DATA *` | Next item in the carrier's inventory |
| `prev_in_carry_list` | `OBJ_DATA *` | Previous item in the carrier's inventory |
| `first_in_carry_list` | `OBJ_DATA *` | Head of this object's sub-inventory (when it is a container) |
| `last_in_carry_list` | `OBJ_DATA *` | Tail of this object's sub-inventory |
| `next_in_room` | `OBJ_DATA *` | Next object lying in the same room |
| `prev_in_room` | `OBJ_DATA *` | Previous object lying in the same room |
| `first_in_room` | `OBJ_DATA *` | First object nested at the room level (alternate list) |
| `last_in_room` | `OBJ_DATA *` | Last object nested at the room level |
| `first_content` | `OBJ_DATA *` | Head of contents list (for containers) |
| `last_content` | `OBJ_DATA *` | Tail of contents list |
| `next_content` | `OBJ_DATA *` | Next sibling in a container's contents list |
| `prev_content` | `OBJ_DATA *` | Previous sibling in a container's contents list |
| `in_obj` | `OBJ_DATA *` | Container this object is inside; `NULL` if not contained |
| `carried_by` | `CHAR_DATA *` | Character carrying this object; `NULL` if in room or container |
| `in_room` | `ROOM_INDEX_DATA *` | Room this object is in; `NULL` if carried or contained |

### Pointers to related structures

| Field | Type | Description |
|---|---|---|
| `pIndexData` | `OBJ_INDEX_DATA *` | Prototype / template this instance was created from |
| `obj_fun` | `OBJ_FUN *` | Object special-function callback |
| `first_exdesc` | `EXTRA_DESCR_DATA *` | Head of extra-description list (keyword → text pairs) |
| `last_exdesc` | `EXTRA_DESCR_DATA *` | Tail of extra-description list |
| `first_apply` | `AFFECT_DATA *` | Head of affects applied when the object is worn/wielded |
| `last_apply` | `AFFECT_DATA *` | Tail of applies list |

### Identity & description

| Field | Type | Description |
|---|---|---|
| `name` | `char *` | Space-separated targeting keywords |
| `short_descr` | `char *` | Brief description used in room and inventory listings |
| `description` | `char *` | Full description shown when the object is examined |
| `owner` | `char *` | Name of the character who owns this object |

### Item type & flags

| Field | Type | Description |
|---|---|---|
| `item_type` | `int` | Item category (weapon, armour, food, container, …) |
| `extra_flags` | `int` | Extra-flag bitfield (glow, magic, cursed, no-drop, …) |
| `wear_flags` | `int` | Where the item may be worn (head, body, wield, …) |
| `item_apply` | `int` | Class / restriction flags for the item |
| `wear_loc` | `int` | Current wear-location on its wearer; `WEAR_NONE` if not worn |

### Physical properties

| Field | Type | Description |
|---|---|---|
| `weight` | `sh_int` | Weight in game units |
| `cost` | `int` | Base purchase cost in gold |
| `level` | `sh_int` | Minimum character level required to use |
| `timer` | `sh_int` | Decay / expiry countdown (ticks); −1 = permanent |
| `condition` | `sh_int` | Condition percentage (100 = new, 0 = destroyed) |

### Type-specific values

| Field | Type | Description |
|---|---|---|
| `value[10]` | `int []` | Ten general-purpose integer slots whose meaning depends on `item_type`.  Examples: for weapons — damage dice; for containers — capacity, key VNUM, lock flags; for food/drink — nutrition, liquid type, poison flag |

### Currency

| Field | Type | Description |
|---|---|---|
| `money` | `MONEY_TYPE *` | Multi-currency amount if this object contains money |

---

## ROOM_INDEX_DATA

`struct room_index_data` — defined at `src/ack.h:1153`

> **Note:** The common shorthand "ROOM_DATA" does not exist as a separate
> typedef in this codebase.  The correct type name is `ROOM_INDEX_DATA`.
> All room pointers (e.g., `ch->in_room`) use this type.

Each `ROOM_INDEX_DATA` represents one room in the world.  Rooms are permanent
(not allocated per-visit); the same record holds both static template data
(description, flags) and live state (current occupants, objects, affects).

### Linked-list / housekeeping

| Field | Type | Description |
|---|---|---|
| `is_free` | `bool` | Guard flag used by LINK/UNLINK macros |
| `next` | `ROOM_INDEX_DATA *` | Next room in the global room list |

### Occupants & contents

| Field | Type | Description |
|---|---|---|
| `first_person` | `CHAR_DATA *` | Head of the list of characters currently in this room |
| `last_person` | `CHAR_DATA *` | Tail of that list |
| `first_content` | `OBJ_DATA *` | Head of the list of objects lying in this room |
| `last_content` | `OBJ_DATA *` | Tail of that list |

### Descriptions

| Field | Type | Description |
|---|---|---|
| `name` | `char *` | Room title (shown at the top of the room description) |
| `description` | `char *` | Full room description text |
| `auto_message` | `char *` | Message broadcast to the room every tick; `NULL` if none |
| `first_exdesc` | `EXTRA_DESCR_DATA *` | Head of keyword → text extra-description list |
| `last_exdesc` | `EXTRA_DESCR_DATA *` | Tail of extra-description list |

### Identity

| Field | Type | Description |
|---|---|---|
| `vnum` | `sh_int` | Virtual number — unique identifier for this room |
| `area` | `AREA_DATA *` | Area this room belongs to |

### Exits

| Field | Type | Description |
|---|---|---|
| `exit[6]` | `EXIT_DATA *[]` | Exits indexed 0–5: north, east, south, west, up, down |

### Flags & environment

| Field | Type | Description |
|---|---|---|
| `room_flags` | `int` | Room-flag bitfield (dark, no-magic, no-recall, peaceful, …) |
| `sector_type` | `sh_int` | Terrain type (city, forest, mountain, water, …) affects movement cost |
| `light` | `sh_int` | Dynamic light level in the room |
| `affected_by` | `int` | Currently active room-affect flags |
| `block_timer` | `sh_int` | Timer for blocking / area-effect room spells |

### Affects

| Field | Type | Description |
|---|---|---|
| `first_room_affect` | `ROOM_AFFECT_DATA *` | Head of active room-affect list |
| `last_room_affect` | `ROOM_AFFECT_DATA *` | Tail of active room-affect list |

### OLC resets

| Field | Type | Description |
|---|---|---|
| `first_room_reset` | `BUILD_DATA_LIST *` | Head of reset-command list (mob/obj spawns) |
| `last_room_reset` | `BUILD_DATA_LIST *` | Tail of reset-command list |

### Miscellaneous

| Field | Type | Description |
|---|---|---|
| `first_mark_list` | `MARK_LIST_MEMBER *` | Head of mark / sign list in room |
| `last_mark_list` | `MARK_LIST_MEMBER *` | Tail of mark / sign list |
| `treasure` | `MONEY_TYPE *` | Multi-currency treasure sitting in the room |
