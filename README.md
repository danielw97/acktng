# ACK!MUD TNG 4.3.1

A MUD (Multi-User Dungeon) game server written in C, descended from the Diku → Merc → ACK! lineage. Players connect via telnet and explore a persistent world of rooms, monsters, magic, and other players.

This is the current release of Release 1 from ackmud.com:8890. The code is kept up to date with the live server, including all area files and bugfixes.

> **Note:** No player files are included. Before booting the server you must create `player/a` through `player/z` subdirectories.

**Stats at a glance:** 68 core C source files · 212 spell implementations · 41 skill files · 48 area files · 8 pulses/second game loop

---

## Table of Contents

1. [Quick Start](#quick-start)
2. [Repository Layout](#repository-layout)
3. [Architecture Overview](#architecture-overview)
4. [Startup Sequence](#startup-sequence)
5. [Main Game Loop](#main-game-loop)
6. [Command Processing](#command-processing)
7. [Update Cycle](#update-cycle)
8. [Core Data Structures](#core-data-structures)
9. [Combat System](#combat-system)
10. [Magic & Spell System](#magic--spell-system)
11. [Skills System](#skills-system)
12. [World Loading (Database)](#world-loading-database)
13. [Area File Format](#area-file-format)
14. [Save & Load System](#save--load-system)
15. [Invasion System](#invasion-system)
16. [Memory Management](#memory-management)
17. [Building & Testing](#building--testing)
18. [Open PR Validation Workflow](#open-pr-validation-workflow)

---

## Quick Start

```sh
# Build
cd src && make ack

# Create required player directories
mkdir -p area/../player/{a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z}

# Run on port 4000
cd area && ../src/ack 4000

# Connect
telnet localhost 4000
```

To run all tests (unit + integration):

```sh
cd src && make unit-tests
```

---

## Repository Layout

```
acktng/
├── src/                  # All C source code and Makefile
│   ├── comm.c            # Network I/O and main game loop
│   ├── db.c              # World/area loading at boot
│   ├── interp.c          # Command dispatch table
│   ├── fight.c           # Combat engine
│   ├── update.c          # Periodic tick/pulse updates
│   ├── save.c            # Player save and load
│   ├── handler.c         # Object/character utilities
│   ├── magic*.c          # Spell casting and effects (4 files)
│   ├── skills*.c         # Skill system (6 files)
│   ├── act_*.c           # Player command handlers (7 files)
│   ├── invasion.c        # Mob invasion event system
│   ├── keep.c            # Player housing/chest system
│   ├── ssm.c             # Shared string memory manager
│   ├── strfuns.c         # String utility functions
│   ├── spells/           # ~212 individual spell files
│   ├── skills/           # ~41 individual skill files
│   ├── ai/               # Mob AI / special functions
│   ├── tests/            # Unit test files (test_*.c)
│   ├── ack.h             # Central header (includes everything)
│   ├── typedefs.h        # All structs, types, constants, macros
│   ├── config.h          # Game configuration (name, limits)
│   └── globals.h         # Global variable declarations
├── area/                 # ~48 area data files + runtime dir
│   └── area.lst          # Master list of areas to load
├── data/                 # Runtime data (bans, clans, socials)
├── docs/                 # Documentation and specifications
├── player/               # Player save files (a-z subdirectories)
├── log/                  # Server log output
├── integration-test.sh   # Full boot + login + crash check
└── .github/workflows/    # CI: runs unit-tests on every PR
```

---

## Architecture Overview

The server is a single-process, `select()`-based event loop. All game state lives in memory as linked lists and hash tables. There is no external database — the world is loaded from text files at boot and players are saved as binary files on disk.

```
┌─────────────────────────────────────────────────────────────┐
│                        ACK!MUD TNG                          │
│                                                             │
│  ┌──────────┐    ┌───────────┐    ┌──────────────────────┐  │
│  │  Telnet  │◄──►│  comm.c   │◄──►│  Command Dispatch    │  │
│  │ Clients  │    │ (network) │    │    (interp.c)        │  │
│  └──────────┘    └─────┬─────┘    └──────────┬───────────┘  │
│                        │                     │              │
│                   ┌────▼──────────────────────▼──────────┐  │
│                   │           Game State                  │  │
│                   │  Rooms · Mobs · Objects · Players     │  │
│                   └────┬──────────────────────┬──────────┘  │
│                        │                      │             │
│                   ┌────▼─────┐          ┌─────▼──────┐      │
│                   │update.c  │          │ act_*.c    │      │
│                   │(ticks)   │          │ fight.c    │      │
│                   └────┬─────┘          │ magic*.c   │      │
│                        │                │ skills*.c  │      │
│          ┌─────────────┼──────────┐     └────────────┘      │
│          │             │          │                         │
│     ┌────▼────┐  ┌─────▼───┐  ┌──▼─────┐                   │
│     │fight.c  │  │char/obj │  │invasion│                   │
│     │(combat) │  │ updates │  │.c      │                   │
│     └─────────┘  └─────────┘  └────────┘                   │
│                                                             │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐   │
│  │    db.c      │    │   save.c     │    │    ssm.c     │   │
│  │ (boot load)  │    │ (player I/O) │    │ (string mem) │   │
│  └──────────────┘    └──────────────┘    └──────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

---

## Startup Sequence

`main()` in `comm.c` drives the entire boot:

```
main(argc, argv)
  │
  ├─ init_socket(port)          Open TCP socket, bind, listen
  │
  ├─ boot_db()                  Load entire world into memory
  │   ├─ init_string_space()    Initialize SSM string pool
  │   ├─ init_time()            Set in-game time and weather
  │   ├─ init_skills()          Assign global skill numbers (gsn_*)
  │   ├─ load_clans()           Load clan data from data/
  │   ├─ load_areas()           Parse all .are files
  │   │   ├─ #AREA sections
  │   │   ├─ #MOBILES sections
  │   │   ├─ #OBJECTS sections
  │   │   ├─ #ROOMS sections
  │   │   ├─ #RESETS sections   (how/where mobs and objects respawn)
  │   │   └─ #SPECIALS sections (assign AI functions to mobs)
  │   ├─ fix_exits()            Resolve room exit vnums to pointers
  │   ├─ reset_area()           Populate world with initial mobs/objects
  │   ├─ load_helps()           Load help files
  │   └─ load_socials()         Load social commands
  │
  └─ game_loop(control)         Run until shutdown
```

---

## Main Game Loop

`game_loop()` in `comm.c` runs at **8 pulses per second** (125 ms per iteration):

```
game_loop()
  │
  ├─ [every iteration]
  │   ├─ select() on all sockets    Wait for I/O (up to 125ms)
  │   ├─ accept new connections     → new_descriptor()
  │   ├─ read input                 → read_from_descriptor()
  │   │
  │   ├─ for each descriptor:
  │   │   ├─ if logging in:  nanny()      (login/char creation state machine)
  │   │   └─ if playing:     interpret()  (command dispatch)
  │   │
  │   ├─ update_handler()           Autonomous game updates (see Update Cycle)
  │   │
  │   ├─ for each descriptor:
  │   │   └─ process_output()       Flush output buffer → client
  │   │
  │   └─ sleep until next 125ms boundary
  │
  └─ [on shutdown] close all sockets, free memory
```

---

## Command Processing

### Login Flow (nanny)

New connections go through a state machine before entering the game:

```
Connect
  │
  ▼
Get Name ──► New player? ──Yes──► Confirm Name
  │                                    │
  No                             Create Password
  │                                    │
  ▼                               Confirm Password
Get Password                           │
  │                               Choose Sex (M/F)
  ▼                                    │
Character Menu                    Choose Race
  │                                    │
  ▼                               Choose Class
Enter Game                             │
                                  Enter Game
```

### Command Dispatch (interpret)

Once in game, every line of input goes through `interpret()` in `interp.c`:

```
interpret(ch, input)
  │
  ├─ Strip leading spaces
  ├─ Extract command word (first argument)
  ├─ Check ch->position (can't attack while sleeping, etc.)
  │
  ├─ Search cmd_table[] — first exact match, then prefix match
  │
  ├─ Check trust level (player vs. immortal commands)
  ├─ Log command (if applicable)
  │
  └─ Call cmd->do_fun(ch, argument)
       │
       ├─ act_info.c    look, score, who, inventory, ...
       ├─ act_move.c    north, south, go, flee, ...
       ├─ act_obj.c     get, drop, give, wear, remove, ...
       ├─ act_comm.c    say, tell, shout, emote, ...
       ├─ act_mob.c     buy, sell, list, practice, ...
       ├─ act_clan.c    clan management commands
       ├─ act_wiz.c     immortal / admin commands
       ├─ fight.c       kill, bash, kick, backstab, ...
       └─ magic*.c      cast, pray, ...
```

---

## Update Cycle

`update_handler()` in `update.c` fires at the end of every game loop iteration. Different subsystems run at different pulse intervals:

```
Pulse Rate          Period      Subsystem
─────────────────────────────────────────────────────────────
PULSE_VIOLENCE      0.25 sec    violence_update()   — combat rounds
PULSE_MOBILE        0.50 sec    mobile_update()     — NPC movement
                                ai_update()         — NPC special behaviors
PULSE_OBJFUN        0.50 sec    objfun_update()     — object special functions
PULSE_MESSAGE       1.50 sec    message_update()    — bulletin board messages
PULSE_ROOMS         3.75 sec    rooms_update()      — room affects
                                invasion_rooms_update()
PULSE_AUCTION       3.75 sec    auction_update()    — auction house
PULSE_TICK          7.50 sec    weather_update()    — in-game time/weather
                                char_update()       — idle timers, position
                                obj_update()        — object decay
                                quest_update()
                                invasion_update()
                                caravan_update()
PULSE_AREA         10.00 sec    area_update()       — area resets (respawn)
PULSE_GAIN        5–8 sec       gain_update()       — HP/mana/move regen
```

Visual timeline (one second = 8 pulses):

```
Pulse:  1    2    3    4    5    6    7    8
        │    │    │    │    │    │    │    │
        ▼    ▼    ▼    ▼    ▼    ▼    ▼    ▼
Combat  ■    ■    ■    ■    ■    ■    ■    ■   (every pulse)
Mobile  ■         ■         ■         ■        (every 4 pulses)
Rooms        ■                   ■             (every 30 pulses)
Tick                                  ■        (every 60 pulses)
```

---

## Core Data Structures

All major types are defined in `typedefs.h`. The central types and their relationships:

```
AREA_DATA
  │  name, author, vnum range
  └─► ROOM_INDEX_DATA[]  (all rooms in this area)
           │  vnum, name, description, sector_type, flags
           ├─► EXIT_DATA[6]        north/south/east/west/up/down
           │     └─► to_room (ROOM_INDEX_DATA*)
           ├─► EXTRA_DESCR_DATA[]  extra 'look' descriptions
           ├─► people              CHAR_DATA* linked list
           └─► first_content       OBJ_DATA* linked list


CHAR_DATA  (player or NPC)
  │  name, level, position, gold, exp
  │  hit/max_hit, mana/max_mana, move/max_move
  │  hitroll, damroll, armor
  ├─► in_room         ROOM_INDEX_DATA*
  ├─► fighting        CHAR_DATA*       (combat target)
  ├─► master          CHAR_DATA*       (follow/charm)
  ├─► first_carry     OBJ_DATA*        (inventory linked list)
  ├─► first_affect    AFFECT_DATA*     (active buffs/debuffs)
  ├─► pIndexData      MOB_INDEX_DATA*  (NULL for players)
  └─► pcdata          PC_DATA*         (NULL for NPCs)
          │  pwd (hash), learned[skill], quest_points
          │  email, prompt, aliases, ignores
          └─► saved affects, saved equipment


OBJ_DATA  (in-game object instance)
  │  name, short_descr, long_descr
  │  item_type, value[4], weight, cost
  │  wear_loc (where equipped)
  ├─► pIndexData    OBJ_INDEX_DATA*   (template/prototype)
  ├─► carried_by    CHAR_DATA*
  ├─► in_room       ROOM_INDEX_DATA*
  └─► first_contain OBJ_DATA*        (nested objects, e.g. bags)


AFFECT_DATA  (a buff or debuff on a character)
  │  type (spell/skill that caused it)
  │  duration (pulses remaining)
  │  modifier (stat amount, e.g. +5 strength)
  └─► bitvector (AFF_INVISIBLE, AFF_SANCTUARY, etc.)
```

---

## Combat System

Combat is tick-driven. Characters in combat are processed every `PULSE_VIOLENCE` (~0.25 sec).

```
Player types "kill goblin"
  │
  └─► do_kill(ch, victim)
        └─► set_fighting(ch, victim)   — links ch->fighting and victim->fighting

[PULSE_VIOLENCE fires]
  │
  └─► violence_update()
        └─► for each CHAR_DATA ch where ch->fighting != NULL:
              └─► one_hit(ch, victim, dt)
                    │
                    ├─ Compute to-hit roll vs. victim AC
                    ├─ If MISS → send miss message, return
                    │
                    ├─ Roll damage (weapon dice or bare hands)
                    ├─ Apply hitroll/damroll bonuses
                    ├─ Check for critical hit (damage multiplier)
                    │
                    └─► damage(ch, victim, dam, dt)
                          ├─ Apply elemental resistances/susceptibilities
                          ├─ Reduce victim->hit
                          ├─ Send damage messages (both sides)
                          │
                          └─ If victim->hit <= 0:
                               └─► raw_kill(victim)
                                     ├─ Create corpse OBJ_DATA with loot
                                     ├─ Award exp/quest points to killer
                                     ├─ stop_fighting(both)
                                     └─ Respawn or extract char
```

**Key combat modifiers:**

| Mechanic | Description |
|---|---|
| Dual wield | Second weapon fires extra `one_hit()` |
| Combo system | Sequences of up to 6 chained hits |
| Chi/Energy | Martial arts modifier, consumed per strike |
| Stances | Alter damage output vs. defense tradeoff |
| Elemental resist | Each damage type has per-mob resist/susceptibility |
| Shields/parry | Buckler, dodge, and parry reduce incoming damage |
| Spells in combat | Casters can cast each round instead of melee |

---

## Magic & Spell System

```
Player types "cast 'fireball' goblin"
  │
  └─► do_cast(ch, arg)           (magic.c)
        ├─ Parse spell name and target
        ├─ Lookup in skill_table[] by name → sn
        ├─ Compute mana cost (base cost modified by stats, buffs)
        ├─ Check prerequisites (level, mana available, not silenced)
        ├─ Subtract mana
        │
        └─► (*spell_table[sn].spell_fun)(sn, level, ch, victim)
              │
              │  Each spell is a separate function in spells/
              │
              ├─ Damage spells:  damage(ch, victim, dam, dt)
              ├─ Buff spells:    affect_to_char(victim, &af)
              │                    adds AFFECT_DATA to victim->first_affect
              ├─ Heal spells:    victim->hit += amount
              ├─ Utility spells: move victim, create objects, etc.
              └─ say_spell()     Announce spell to room
```

**Affect system** — duration-based effects on characters:

```
affect_to_char(ch, af)
  Adds AFFECT_DATA to ch->first_affect linked list
  Calls affect_modify(ch, af, TRUE) to apply stat modifiers
    e.g. +10 AC, +5 str, set AFF_INVISIBLE bit

affect_remove(ch, af)
  Calls affect_modify(ch, af, FALSE) to undo modifiers
  Unlinks AFFECT_DATA from ch->first_affect

char_update() — runs every PULSE_TICK
  Decrements af->duration on all active affects
  Calls affect_remove() when duration reaches 0
```

---

## Skills System

Skills are non-magical, usually instant, and often hook directly into the combat engine.

```
skill_table[]   — global registry (up to 999 entries)
  │  name, gsn (global skill number), targets, min_level[class]
  │  spell_fun / skill_fun, min_mana, beats (recovery time)
  └─► Indexed by gsn (e.g. gsn_kick, gsn_backstab, gsn_dodge)

ch->pcdata->learned[gsn]   — per-character expertise 0..100
  0   = not learned
  1+  = learned; higher value = more effective
  100 = grandmaster

Skill execution:
  do_kick(ch, arg)
    ├─ Check learned[gsn_kick] > 0
    ├─ Check position, target, cooldown
    ├─ Roll success vs. learned%
    ├─ On success: damage(ch, victim, dam, gsn_kick)
    │              + optional knockdown/stun
    └─ Improvement: small chance to raise learned[gsn_kick]++
```

**Skill categories:**

| Category | Examples |
|---|---|
| Combat | kick, bash, trip, disarm, rescue, backstab |
| Martial arts / Chi | palm strike, chi block, flurry |
| Combos | Chained multi-hit sequences (up to 6 strikes) |
| Defense | dodge, parry, shield block |
| Utility | climb, pick lock, appraise, detect hidden |
| Class-specific | Thief sneak/hide, Cleric turn undead, etc. |

---

## World Loading (Database)

At boot, `boot_db()` in `db.c` reads `area/area.lst` and processes each listed `.are` file:

```
area.lst
  ├─ midgaard.are
  ├─ newbie.are
  ├─ ... (48 total)
  └─ END

For each .are file — load_area_file(fname):
  │
  ├─ #AREA     → new AREA_DATA; parse name, author, vnum range
  ├─ #MOBILES  → new MOB_INDEX_DATA per mob; add to mob_index_hash[]
  ├─ #OBJECTS  → new OBJ_INDEX_DATA per object; add to obj_index_hash[]
  ├─ #ROOMS    → new ROOM_INDEX_DATA per room; add to room_index_hash[]
  │               parse exits (store vnum, resolve to pointer later)
  │               parse extra descriptions
  ├─ #SHOPS    → attach SHOP_DATA to a mob's pIndexData
  ├─ #RESETS   → build RESET_DATA list (rules for repopulating area)
  │               M vnum room  — place mob in room
  │               O vnum room  — place object in room
  │               E vnum slot  — equip object on last mob
  │               G vnum       — give object to last mob
  │               D dir flag   — set door state
  │               R room dir   — randomize exits
  └─ #SPECIALS → attach spec_fun pointer to mob (spec_fido, spec_guard, etc.)

After all files loaded:
  fix_exits()     — resolve exit vnums → ROOM_INDEX_DATA pointers
  reset_area()    — execute all RESET_DATA to populate the world
```

**Hash tables** provide O(1) lookup by vnum:

```
room_index_hash[vnum % MAX_KEY_HASH]   linked list per bucket
mob_index_hash[vnum % MAX_KEY_HASH]
obj_index_hash[vnum % MAX_KEY_HASH]

get_room_index(vnum)   walks the bucket list to find the exact vnum
get_mob_index(vnum)
get_obj_index(vnum)
```

---

## Area File Format

Area files use a plain-text, section-based format with `~` as the string terminator.

```
#AREA
Midgaard~
Finieous~
1 199~

#MOBILES
#3001
the town crier~                       ← keywords (for targeting)
A town crier~                         ← short description
A town crier walks about the square.~ ← long description (seen in room)
                                      ← blank line = start of full description
This is the town crier...~            ← full description (on examine)
NSBCDE 0 0                            ← act flags, affected flags, alignment
17 0 1d6+70 1d3+5 100 150            ← level act hp_dice dam_dice gold exp

#ROOMS
#3001
The Town Square~
You are in the center of town...~
0 0 5                                 ← room flags, sector type
D0                                    ← Direction 0 = north
A short path leads north.~            ← exit description
~                                     ← exit keyword (for open/close)
0 -1 3002                             ← lock flags, key vnum, to_room vnum
S                                     ← end of room

#RESETS
M 0 3001 1 3001   ← Mob vnum=3001, max=1, in room=3001
O 0 3010 0 3001   ← Object vnum=3010 in room=3001
E 0 3011 0 17    ← Equip object 3011 on last mob, wear slot 17
S

#$                ← end of file
```

Full specification: [`docs/area_file_spec.md`](docs/area_file_spec.md)

---

## Save & Load System

Player files are stored at `player/{first_letter}/{charname}` (e.g. `player/a/aragorn`).

```
Logout / crash-save / periodic save:
  save_char_obj(ch)
    ├─ Open player/{letter}/{name} for writing
    ├─ Write SAVE_REVISION header (version number for format compat)
    ├─ Serialize CHAR_DATA fields (level, stats, flags, gold, ...)
    ├─ Serialize PC_DATA (learned skills, quest points, aliases, ...)
    ├─ Serialize active AFFECT_DATA list
    ├─ Walk ch->first_carry (inventory tree):
    │   └─ fwrite_obj(ch, obj, depth)
    │         ├─ Write OBJ_DATA fields
    │         └─ Recurse into obj->first_contain (bags/containers)
    └─ Close file

Login:
  load_char_obj(ch, name)
    ├─ Open player file
    ├─ Check SAVE_REVISION (handle older formats gracefully)
    ├─ Deserialize CHAR_DATA and PC_DATA
    ├─ Restore active affects
    └─ Restore inventory tree (fread_obj, recursive)
```

---

## Invasion System

`invasion.c` runs a scripted world event when players are online.

```
Trigger: 1+ mortal players online
  │
  └─► invasion_start()
        ├─ Select random BOSS_PROFILE (15 unique boss types)
        ├─ Spawn boss mob at level = max_player_level + 20
        │   Boss has: unique spells, elemental strengths/weaknesses,
        │             special skills, random trash-talk emotes
        │
        └─► Wave spawning (every PULSE_TICK)
              ├─ Select random MOB_PROFILE (45 types with varied abilities)
              ├─ Spawn wave mobs as SENTINEL across the world
              ├─ Mobs march toward Room 3110 (Gertrude's location)
              └─ On reaching Room 3110: sacrifice mob → wound Gertrude

Resolution:
  Boss killed    → invasion_success()
                   Give 25 QP reward to participants
                   Despawn all invasion mobs
                   Schedule next invasion

  Gertrude dies  → invasion_fail()
                   World penalty event
```

---

## Memory Management

`ssm.c` implements a **Shared String Manager** that deduplicates string data across the entire server to save memory.

```
String pool layout:
  ┌──────────────────────────────────┐
  │  Chunk 0  (CHUNK_SIZE bytes)     │
  │  ┌────────────────────────────┐  │
  │  │ "The Town Square\0"        │  │
  │  │ "A goblin\0"               │  │
  │  │ "You are in...\0"          │  │
  │  │ ...                        │  │
  │  └────────────────────────────┘  │
  │  Chunk 1  ...                    │
  └──────────────────────────────────┘

str_dup(str)
  1. Hash the string
  2. Check hash table — if found, return existing pointer (no alloc)
  3. If new: copy into current chunk, add to hash table, return pointer

free_string(str)
  Decrement reference count
  If count reaches 0: mark space as free
```

**Benefits:** Area loading creates thousands of strings; deduplication saves significant memory. Pointer equality works for shared strings (fast comparison). All mob/room/object names that are identical share a single allocation.

For non-string heap memory, `alloc_mem()` / `dispose()` wrap `malloc`/`free` with optional debug modes (`DEBUG_MEM`, `DEBUG_MEM_CORRUPT`).

---

## Building & Testing

### Build Commands

All commands run from `src/`:

```sh
make ack              # Build server binary → src/ack
make clean            # Remove .o files and binary
make format           # Auto-apply .clang-format to all source
make lint             # Check formatting (fails if any files differ)
make unit-tests       # Build + run all unit tests + integration test
make all              # clean → format → lint → build → unit-tests
```

### Unit Tests

Tests live in `src/tests/test_*.c`. Each is a standalone binary using `assert()` — no test framework required.

```
Pattern:
  test_fight.c                 ← test file (contains main() and asserts)
  fight.unit-test.o            ← module under test (compiled with UNIT_TEST_FIGHT)
  test_is_fighting.o           ← shared stub (linked into every test binary)

Linker flags:
  -ffunction-sections -fdata-sections   per-symbol sections
  -Wl,--gc-sections                     dead-strip unused symbols

Run:
  cd src && make unit-tests
  → builds each binary into tests/unit-test-*
  → runs each binary; non-zero exit = failure
  → runs integration-test.sh at the end
```

### Integration Test

`integration-test.sh` (run from repo root):

1. Builds the server
2. Boots it on a random ephemeral port
3. Walks the full new-player login flow via `nc`: Name → Password → Sex → Race → Class → Enter game
4. Monitors for crashes for 8 seconds
5. Sends shutdown command, verifies clean exit

---

## Open PR Validation Workflow

`.github/workflows/validate-open-prs.yml` runs `cd src && make unit-tests` against every open PR.

- **Schedule:** Hourly + on manual dispatch
- **On pass:** Submits an automated PR approval review
- **On fail:** Submits a `REQUEST_CHANGES` review and fails the job

To enforce merge safety, configure branch protection to require this workflow check and at least one approving review before merging.

---

## Key Configuration

`src/config.h` controls tunable parameters:

| Setting | Value | Description |
|---|---|---|
| `MAX_LEVEL` | 105 | Hero at 101, immortal at 102+ |
| `MAX_CLASS` | 6 | Mage, Cleric, Warrior, Thief, Psion, Pug |
| `MAX_REMORT` | 12 | Reincarnation tiers |
| `MAX_CLAN` | 9 | Player clans |
| `MAX_SKILL` | 999 | Skill table slots |
| `PULSE_PER_SECOND` | 8 | Game loop frequency |
| `PULSE_VIOLENCE` | 2 | Combat rounds per second |
| `PULSE_TICK` | 60 | Regen/update tick (~7.5 sec) |

---

## Lineage

```
Diku MUD (1990, University of Copenhagen)
  └─► Merc MUD 2.1 (1992, Furey/Duelfer/Visoiu)
        └─► ACK! MUD (1994–2000, various)
              └─► ACK!MUD TNG 4.x (this codebase)
```

License terms from the original Diku and Merc licenses apply. See [`docs/`](docs/) for full license text and specifications.
