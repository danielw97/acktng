# Public Societies: Data Structures

## Constants

```c
#define MAX_PUB_SOCIETY        7   /* 0 = none, 1-6 = the six societies        */
#define MAX_PUB_SOCIETY_RANK   6   /* ranks 0-5 (0 = recruit, 5 = commander)   */
#define MAX_PUB_SOCIETY_REP  500   /* reputation cap (positive and negative)    */

#define PUB_SOCIETY_NONE              0
#define PUB_SOCIETY_GUARD_COMMAND     1   /* Midgaard — military defense         */
#define PUB_SOCIETY_HARBOR_SYNDICS    2   /* Kowloon — maritime commerce         */
#define PUB_SOCIETY_EMBER_WARDENS     3   /* Rakuen — disaster response          */
#define PUB_SOCIETY_FIRST_CLAW        4   /* Mafdet — oath enforcement           */
#define PUB_SOCIETY_WALL_COMMAND      5   /* Kiess — frontier defense            */
#define PUB_SOCIETY_ROAD_WARDENS      6   /* Intercity — trade route protection  */
```

These are defined in `typedefs.h` alongside the existing secret society
constants. The `PUB_` prefix distinguishes them from the `SOCIETY_` constants
used by the secret society system.

---

## Public Society Table (Static Definition)

The public society table follows the same pattern as `society_table[]` and
`clan_table[]` — a static array of structs.

```c
struct pub_society_type
{
   char  *name;              /* Full name: "The Guard Command"              */
   char  *abbr;              /* Five-char abbreviation: "GUARD"             */
   sh_int base_city;         /* Primary city constant (CITY_MIDGAARD, etc.) */
   sh_int hall_room;         /* Vnum of society hall main room              */
   sh_int min_level;         /* Minimum level to join                       */
   int    skill_first;       /* First skill index in pub society skill block*/
   int    skill_count;       /* Number of society-specific skills           */
};

extern const struct pub_society_type pub_society_table[MAX_PUB_SOCIETY];
```

**File:** `src/const_pub_society.c` (new file)

No passphrase or rival fields — public societies have open recruitment and no
built-in rivalries.

---

## Player Public Society Data

Public society membership data is stored on `PC_DATA`, parallel to the secret
society fields.

### New Fields on PC_DATA

```c
/* In PC_DATA (ack.h) */
sh_int pub_society;              /* Public society ID (0 = none, 1-6)       */
sh_int pub_society_rank;         /* Rank within society (0-5)               */
int    pub_society_rep;          /* Reputation (-500 to 500)                */
int    pub_society_tasks;        /* Lifetime completed task count           */
time_t pub_society_joined;       /* Timestamp of joining                    */
```

### Comparison with Secret Society Fields

Public societies require fewer fields because they have no exposure system,
no sigil mechanics, no PvP windows, and no disavowal. The fields track only
membership, rank, reputation, task count, and join date.

| Feature | Secret Society | Public Society |
|---|---|---|
| Society ID | `society` | `pub_society` |
| Rank | `society_rank` | `pub_society_rank` |
| Reputation | `society_rep` | `pub_society_rep` |
| Task/mission count | `society_missions` | `pub_society_tasks` |
| Join timestamp | `society_joined` | `pub_society_joined` |
| Flags bitfield | `society_flags` | Not needed |
| Exposure state | `society_exposed` | Not needed |
| Exposure timer | `society_exposure_timer` | Not needed |
| PvP target | `society_pvp_target` | Not needed |
| PvP timer | `society_pvp_timer` | Not needed |
| Kill count | `society_kills` | Not needed |

---

## Reputation

Public society reputation uses the same -500 to +500 range as secret
societies. The tier labels reflect institutional standing:

| Range | Label | Effect |
|---|---|---|
| -500 to -300 | Dishonorably Discharged | Expelled; barred for 30 days |
| -299 to -100 | Disciplined | Restricted task access |
| -99 to 99 | Standing | Standard access |
| 100 to 299 | Commended | Priority task access |
| 300 to 499 | Decorated | Command tasks; institutional privileges |
| 500 | Exemplary | Maximum standing; leadership tier |

Reputation changes:
- Completing tasks: +10 to +50 depending on difficulty
- Failing tasks: -5 to -20 depending on severity
- Abandoning tasks: -3 to -10
- Mentoring a recruit (rank 3+): +15
- Time-based decay: reputation above 300 decays by 1 per game day toward 300

No exposure penalties exist for public societies. The reputation system is
simpler because public society work carries no risk of identity compromise.

---

## NPC Public Society Awareness

Public society NPCs use fields on `MOB_INDEX_DATA` that parallel the secret
society NPC fields:

```c
/* In MOB_INDEX_DATA (ack.h) — already has society/society_npc_role */
sh_int pub_society;           /* Public society this NPC belongs to (0=none) */
sh_int pub_society_npc_role;  /* Role: enrollor, taskmaster, merchant, etc.  */
```

### NPC Roles

```c
#define PUB_SOC_NPC_NONE        0
#define PUB_SOC_NPC_ENROLLOR    1  /* Processes enrollment                     */
#define PUB_SOC_NPC_TASKMASTER  2  /* Assigns tasks, reports status            */
#define PUB_SOC_NPC_MERCHANT    3  /* Sells society-specific items to members  */
#define PUB_SOC_NPC_TRAINER     4  /* Teaches society skills                   */
#define PUB_SOC_NPC_COMMANDER   5  /* Handles promotions and intelligence      */
```

Public society NPCs do not need a guard role (no hidden HQ to protect) or a
covert contact role (nothing is covert).

---

## Save Format

Public society data is persisted in the player save file. This uses the same
save revision as the secret society system (revision 16) if both are
implemented together, or revision 17 if public societies are added after
secret societies.

### Save Block

```
PubSociety   2
PubSocRank   3
PubSocRep    225
PubSocTasks  28
PubSocJoined 1742400000
```

### Save/Load Functions

In `save_players.c` (`fwrite_char`):

```c
if (ch->pcdata->pub_society > 0)
{
   fprintf(fp, "PubSociety   %d\n", ch->pcdata->pub_society);
   fprintf(fp, "PubSocRank   %d\n", ch->pcdata->pub_society_rank);
   fprintf(fp, "PubSocRep    %d\n", ch->pcdata->pub_society_rep);
   fprintf(fp, "PubSocTasks  %d\n", ch->pcdata->pub_society_tasks);
   fprintf(fp, "PubSocJoined %ld\n", (long)ch->pcdata->pub_society_joined);
}
```

In `save_players.c` (`fread_char`):

Unknown `PubSoc*` keys are skipped. Players from earlier revisions default
all public society fields to 0.

---

## Task System

Public society tasks use the existing `QUEST_DATA` infrastructure with a new
quest type constant:

```c
#define QUEST_TYPE_PUB_SOCIETY  4  /* public society task */
```

This is distinct from `QUEST_TYPE_SOCIETY` (3) used by secret society missions.
A player can have one secret society mission, one public society task, and up
to 3 normal quests active simultaneously.

### Task Templates

```c
struct pub_society_task_type
{
   int    id;                      /* unique task ID                         */
   sh_int pub_society;             /* which society offers this              */
   sh_int min_rank;                /* minimum rank to accept                 */
   sh_int min_rep;                 /* minimum reputation to accept           */
   sh_int difficulty;              /* 1-5 scale                              */
   int    rep_reward;              /* reputation gained on success           */
   int    rep_penalty;             /* reputation lost on failure             */
   int    gold_reward;             /* gold paid on success                   */
   int    time_limit;              /* ticks before auto-failure (0=none)     */
   char  *name;                    /* task name shown to player              */
   char  *desc;                    /* brief description                      */
   sh_int objective_type;          /* reuses MISSION_OBJ_* constants         */
   int    target_vnum;             /* mob, obj, or room vnum for target      */
   int    delivery_vnum;           /* room or mob vnum for delivery          */
};
```

Note the absence of `exposure_on_fail` — public society tasks never trigger
exposure because there is nothing to expose.

---

## Skill Slots

Public society skills occupy a separate block from secret society skills:

```c
/* Secret society skills: 900-949 */
/* Public society skills: 950-999 */
#define SKILL_PUB_SOCIETY_START  950
#define SKILL_PUB_SOCIETY_END    999  /* 50 slots: 6 per society + spare */
```

Skills are registered in `skill_table[]` with `skill_level[*]` set to
`NO_USE` for all classes — they are unlocked by public society rank, not
class level.

---

## Communication Channel

Public society members share a communication channel:

```c
#define CHANNEL_PUB_SOCIETY  (1 << 19)  /* public society channel */
```

The channel is used via:

```
society say <message>
```

Since a player can be in both a public and secret society, the `society`
command dispatches to the appropriate system based on context. The `say`
subcommand always sends to the public society channel. Secret society
communication uses the separate secret `society` command with a different
prefix (or the `covert` subcommand — TBD at implementation).

---

## Memory Layout Summary

| Location | New Fields | Size Impact |
|---|---|---|
| `PC_DATA` | 5 fields (pub_society through pub_society_joined) | ~20 bytes per player |
| `MOB_INDEX_DATA` | 2 fields (pub_society, pub_society_npc_role) | ~4 bytes per mob template |
| `pub_society_table[]` | Static array of 7 entries | ~300 bytes total (constant) |
| Player save file | 5 new lines per society member | ~80 bytes per save |

Combined with the secret society fields, total per-player overhead for both
systems is ~48 bytes — negligible.
