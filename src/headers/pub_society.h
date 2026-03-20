/***************************************************************************
 * pub_society.h
 *
 * Defines for the public society system.
 * Membership state is stored in PC_DATA fields (ack.h).
 ***************************************************************************/

#ifndef PUB_SOCIETY_H
#define PUB_SOCIETY_H

/* ------------------------------------------------------------------
 * Society index constants (stored in pcdata->pub_society)
 * ------------------------------------------------------------------ */
#define PUB_SOCIETY_NONE 0
#define PUB_SOCIETY_GUARD_COMMAND 1
#define PUB_SOCIETY_HARBOR_SYNDICS 2
#define PUB_SOCIETY_EMBER_WARDENS 3
#define PUB_SOCIETY_FIRST_CLAW 4
#define PUB_SOCIETY_WALL_COMMAND 5
#define PUB_SOCIETY_ROAD_WARDENS 6

#define MAX_PUB_SOCIETY 7 /* 0=none + 6 societies */

/* Rank score boundaries */
#define PUB_SOCIETY_RANK_MIN -500
#define PUB_SOCIETY_RANK_MAX 500
#define PUB_SOCIETY_RANK_DISCHARGE -300 /* auto-kicked below this */

/* Rank thresholds for skill unlocks */
#define PUB_RANK_THRESHOLD_0 -99 /* entry skill */
#define PUB_RANK_THRESHOLD_1 1   /* rank 1 skill */
#define PUB_RANK_THRESHOLD_2 100 /* rank 2 skill */
#define PUB_RANK_THRESHOLD_3 200 /* rank 3 skill (passive combat) */
#define PUB_RANK_THRESHOLD_4 300 /* rank 4 skill */
#define PUB_RANK_THRESHOLD_5 400 /* rank 5 skill (active combat) */

/* Number of skills per society */
#define PUB_SOCIETY_SKILLS_PER 6

/* Cooldown (in days) before rejoining after leaving */
#define PUB_SOCIETY_REJOIN_COOLDOWN_DAYS 7

/* NPC role constants for society hall NPCs */
#define PUB_SOC_NPC_NONE 0
#define PUB_SOC_NPC_RECRUITER 1
#define PUB_SOC_NPC_TASKMASTER 2
#define PUB_SOC_NPC_MERCHANT 3
#define PUB_SOC_NPC_TRAINER 4
#define PUB_SOC_NPC_COMMANDER 5

/* ------------------------------------------------------------------
 * Society table structure
 * ------------------------------------------------------------------ */
struct pub_society_type
{
   char *name;                                   /* e.g. "The Guard Command" */
   char *abbr;                                   /* e.g. "GUARD" */
   char *base_city;                              /* e.g. "Midgaard" */
   int hall_room;                                /* vnum of the main hall room */
   int min_level;                                /* minimum level to join */
   int skill_gsns[PUB_SOCIETY_SKILLS_PER];       /* filled at boot from gsn ptrs */
   int skill_thresholds[PUB_SOCIETY_SKILLS_PER]; /* rank score required */
   char *rank_titles[6];                         /* titles for each rank tier */
};

/* ------------------------------------------------------------------
 * Function prototypes
 * ------------------------------------------------------------------ */
void do_enlist args((CHAR_DATA * ch, char *argument));
void do_society args((CHAR_DATA * ch, char *argument));
bool can_join_pub_society args((CHAR_DATA * ch, int society));
bool can_use_pub_society_skill args((CHAR_DATA * ch, int gsn));
int pub_society_skills_available args((int society, int rank_score));
const char *pub_society_rank_title args((int society, int rank_score));
void pub_society_resign args((CHAR_DATA * ch));
void pub_society_boot_init args((void));

/* Passive combat hooks */
int pub_society_dodge_bonus args((CHAR_DATA * ch));
int pub_society_damage_reduction args((CHAR_DATA * ch, int dam, int element));
int pub_society_extra_attack args((CHAR_DATA * ch));
int pub_society_hp_regen args((CHAR_DATA * ch));
bool pub_society_marked_strike_check args((CHAR_DATA * ch, CHAR_DATA *victim));

/* Active skill command functions */
void do_patrol_sense args((CHAR_DATA * ch, char *argument));
void do_field_mend args((CHAR_DATA * ch, char *argument));
void do_rally_cry args((CHAR_DATA * ch, char *argument));
void do_tactical_assess args((CHAR_DATA * ch, char *argument));
void do_command_presence args((CHAR_DATA * ch, char *argument));
void do_dockside_audit args((CHAR_DATA * ch, char *argument));
void do_haggle args((CHAR_DATA * ch, char *argument));
void do_bonded_cargo args((CHAR_DATA * ch, char *argument));
void do_anchor_slam args((CHAR_DATA * ch, char *argument));
void do_forage args((CHAR_DATA * ch, char *argument));
void do_field_repair args((CHAR_DATA * ch, char *argument));
void do_ember_signal args((CHAR_DATA * ch, char *argument));
void do_containment_ward args((CHAR_DATA * ch, char *argument));
void do_controlled_burn args((CHAR_DATA * ch, char *argument));
void do_oath_mark args((CHAR_DATA * ch, char *argument));
void do_track_quarry args((CHAR_DATA * ch, char *argument));
void do_writ_of_hold args((CHAR_DATA * ch, char *argument));
void do_claws_pursuit args((CHAR_DATA * ch, char *argument));
void do_claws_judgment args((CHAR_DATA * ch, char *argument));
void do_dig_in args((CHAR_DATA * ch, char *argument));
void do_shelter args((CHAR_DATA * ch, char *argument));
void do_trailblaze args((CHAR_DATA * ch, char *argument));
void do_signal_tower args((CHAR_DATA * ch, char *argument));
void do_rampart_slam args((CHAR_DATA * ch, char *argument));
void do_wayfind args((CHAR_DATA * ch, char *argument));
void do_road_camp args((CHAR_DATA * ch, char *argument));
void do_forced_march args((CHAR_DATA * ch, char *argument));
void do_dispatch_rider args((CHAR_DATA * ch, char *argument));
void do_waylay args((CHAR_DATA * ch, char *argument));

#endif /* PUB_SOCIETY_H */
