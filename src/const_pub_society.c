/***************************************************************************
 * const_pub_society.c
 *
 * Public society table data and boot-time initialization.
 ***************************************************************************/

#include "globals.h"

/*
 * The pub_society_table is non-const because skill_gsns[] are filled
 * at boot time by pub_society_boot_init() after gsn resolution.
 */
struct pub_society_type pub_society_table[MAX_PUB_SOCIETY] = {
    /* PUB_SOCIETY_NONE (index 0) */
    {"None", "NONE", "None", 0, 0, {0}, {0}, {"", "", "", "", "", ""}},

    /* PUB_SOCIETY_GUARD_COMMAND (index 1) */
    {"The Guard Command",
     "GUARD",
     "Midgaard",
     7800,
     15,
     {0}, /* filled at boot */
     {PUB_RANK_THRESHOLD_0, PUB_RANK_THRESHOLD_1, PUB_RANK_THRESHOLD_2, PUB_RANK_THRESHOLD_3,
      PUB_RANK_THRESHOLD_4, PUB_RANK_THRESHOLD_5},
     {"Watchman", "Guard", "Sergeant", "Lieutenant", "Captain", "Marshal"}},

    /* PUB_SOCIETY_HARBOR_SYNDICS (index 2) */
    {"The Harbor Syndics",
     "SYNDX",
     "Kowloon",
     8000,
     15,
     {0},
     {PUB_RANK_THRESHOLD_0, PUB_RANK_THRESHOLD_1, PUB_RANK_THRESHOLD_2, PUB_RANK_THRESHOLD_3,
      PUB_RANK_THRESHOLD_4, PUB_RANK_THRESHOLD_5},
     {"Clerk", "Factor", "Assessor", "Arbiter", "Senior Syndic", "Port Master"}},

    /* PUB_SOCIETY_EMBER_WARDENS (index 3) */
    {"The Ember Wardens",
     "EMBER",
     "Rakuen",
     8200,
     15,
     {0},
     {PUB_RANK_THRESHOLD_0, PUB_RANK_THRESHOLD_1, PUB_RANK_THRESHOLD_2, PUB_RANK_THRESHOLD_3,
      PUB_RANK_THRESHOLD_4, PUB_RANK_THRESHOLD_5},
     {"Responder", "Warden", "Senior Warden", "Warden-Lieutenant", "Warden-Captain",
      "Warden-Commander"}},

    /* PUB_SOCIETY_FIRST_CLAW (index 4) */
    {"The Shrine of the First Claw",
     "CLAW",
     "Mafdet",
     8100,
     20,
     {0},
     {PUB_RANK_THRESHOLD_0, PUB_RANK_THRESHOLD_1, PUB_RANK_THRESHOLD_2, PUB_RANK_THRESHOLD_3,
      PUB_RANK_THRESHOLD_4, PUB_RANK_THRESHOLD_5},
     {"Petitioner", "Sworn", "Claw Agent", "Arbiter", "Senior Arbiter", "High Claw"}},

    /* PUB_SOCIETY_WALL_COMMAND (index 5) */
    {"The Wall Command",
     "WALLC",
     "Kiess",
     7900,
     15,
     {0},
     {PUB_RANK_THRESHOLD_0, PUB_RANK_THRESHOLD_1, PUB_RANK_THRESHOLD_2, PUB_RANK_THRESHOLD_3,
      PUB_RANK_THRESHOLD_4, PUB_RANK_THRESHOLD_5},
     {"Sentry", "Ranger", "Pathfinder", "Wall Sergeant", "Wall Captain", "Castellan"}},

    /* PUB_SOCIETY_ROAD_WARDENS (index 6) */
    {"The Road Wardens",
     "ROADW",
     "Midgaard",
     8300,
     20,
     {0},
     {PUB_RANK_THRESHOLD_0, PUB_RANK_THRESHOLD_1, PUB_RANK_THRESHOLD_2, PUB_RANK_THRESHOLD_3,
      PUB_RANK_THRESHOLD_4, PUB_RANK_THRESHOLD_5},
     {"Runner", "Outrider", "Trailkeeper", "Road Sergeant", "Road Captain", "Warden-Master"}},
};

/*
 * Called at boot after skill_table gsn resolution.
 * Links each society's skill_gsns[] array to the resolved gsn values.
 */
void pub_society_boot_init(void)
{
   /* Guard Command */
   pub_society_table[PUB_SOCIETY_GUARD_COMMAND].skill_gsns[0] = gsn_patrol_sense;
   pub_society_table[PUB_SOCIETY_GUARD_COMMAND].skill_gsns[1] = gsn_field_mend;
   pub_society_table[PUB_SOCIETY_GUARD_COMMAND].skill_gsns[2] = gsn_rally_cry;
   pub_society_table[PUB_SOCIETY_GUARD_COMMAND].skill_gsns[3] = gsn_shield_wall;
   pub_society_table[PUB_SOCIETY_GUARD_COMMAND].skill_gsns[4] = gsn_tactical_assess;
   pub_society_table[PUB_SOCIETY_GUARD_COMMAND].skill_gsns[5] = gsn_command_presence;

   /* Harbor Syndics */
   pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[0] = gsn_dockside_audit;
   pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[1] = gsn_haggle;
   pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[2] = gsn_longshoreman;
   pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[3] = gsn_bonded_cargo;
   pub_society_table[PUB_SOCIETY_HARBOR_SYNDICS].skill_gsns[4] = gsn_anchor_slam;

   /* Ember Wardens */
   pub_society_table[PUB_SOCIETY_EMBER_WARDENS].skill_gsns[0] = gsn_forage;
   pub_society_table[PUB_SOCIETY_EMBER_WARDENS].skill_gsns[1] = gsn_field_repair;
   pub_society_table[PUB_SOCIETY_EMBER_WARDENS].skill_gsns[2] = gsn_ember_signal;
   pub_society_table[PUB_SOCIETY_EMBER_WARDENS].skill_gsns[3] = gsn_ember_vitality;
   pub_society_table[PUB_SOCIETY_EMBER_WARDENS].skill_gsns[4] = gsn_containment_ward;
   pub_society_table[PUB_SOCIETY_EMBER_WARDENS].skill_gsns[5] = gsn_controlled_burn;

   /* Shrine of the First Claw */
   pub_society_table[PUB_SOCIETY_FIRST_CLAW].skill_gsns[0] = gsn_oath_mark;
   pub_society_table[PUB_SOCIETY_FIRST_CLAW].skill_gsns[1] = gsn_track_quarry;
   pub_society_table[PUB_SOCIETY_FIRST_CLAW].skill_gsns[2] = gsn_writ_of_hold;
   pub_society_table[PUB_SOCIETY_FIRST_CLAW].skill_gsns[3] = gsn_marked_strike;
   pub_society_table[PUB_SOCIETY_FIRST_CLAW].skill_gsns[4] = gsn_claws_pursuit;
   pub_society_table[PUB_SOCIETY_FIRST_CLAW].skill_gsns[5] = gsn_claws_judgment;

   /* Wall Command */
   pub_society_table[PUB_SOCIETY_WALL_COMMAND].skill_gsns[0] = gsn_dig_in;
   pub_society_table[PUB_SOCIETY_WALL_COMMAND].skill_gsns[1] = gsn_shelter;
   pub_society_table[PUB_SOCIETY_WALL_COMMAND].skill_gsns[2] = gsn_trailblaze;
   pub_society_table[PUB_SOCIETY_WALL_COMMAND].skill_gsns[3] = gsn_bulwark;
   pub_society_table[PUB_SOCIETY_WALL_COMMAND].skill_gsns[4] = gsn_signal_tower;
   pub_society_table[PUB_SOCIETY_WALL_COMMAND].skill_gsns[5] = gsn_rampart_slam;

   /* Road Wardens */
   pub_society_table[PUB_SOCIETY_ROAD_WARDENS].skill_gsns[0] = gsn_wayfind;
   pub_society_table[PUB_SOCIETY_ROAD_WARDENS].skill_gsns[1] = gsn_road_camp;
   pub_society_table[PUB_SOCIETY_ROAD_WARDENS].skill_gsns[2] = gsn_forced_march;
   pub_society_table[PUB_SOCIETY_ROAD_WARDENS].skill_gsns[3] = gsn_road_grit;
   pub_society_table[PUB_SOCIETY_ROAD_WARDENS].skill_gsns[4] = gsn_dispatch_rider;
   pub_society_table[PUB_SOCIETY_ROAD_WARDENS].skill_gsns[5] = gsn_waylay;
}
