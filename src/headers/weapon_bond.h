/*
 * weapon_bond.h — Swordsman Weapon Bonding System
 *
 * A swordsman can bond to a sword, creating a soul-linked weapon that
 * scales with the wielder's level.  The weapon's power budget is anchored
 * to the original weapon's level at the time of bonding: a level 150 base
 * sword produces a far more powerful bond than a level 1 sword.
 *
 * Only one bond at a time — bonding a new sword destroys the old one and
 * inflicts a "Broken Bond" debuff.  Bonded weapons cannot be dropped,
 * given away, disarmed, or lost on death.
 *
 * Tracks provide multiplicative bonuses funded by bond points earned
 * through combat.  At max rank (5), a mastery objfun is unlocked.
 */

#ifndef DEC_WEAPON_BOND_H
#define DEC_WEAPON_BOND_H

/* ── Track IDs ────────────────────────────────────────────────── */

#define BOND_TRACK_NONE 0
#define BOND_TRACK_EDGE 1   /* Raw damage: +dr_mod, +damage dice      */
#define BOND_TRACK_GUARD 2  /* Defense: +ac_mod (static), +parry_mod  */
#define BOND_TRACK_KEEN 3   /* Precision: +hr_mod, +crit/crit_mult    */
#define BOND_TRACK_SPIRIT 4 /* Sustain: +hp bonus, +spellpower_mod    */

#define BOND_MAX_TRACK 4
#define BOND_MAX_RANK 5

/* ── Bond point costs per rank (5x revenant costs) ────────────── */

#define BOND_COST_RANK_1 0
#define BOND_COST_RANK_2 125
#define BOND_COST_RANK_3 375
#define BOND_COST_RANK_4 750
#define BOND_COST_RANK_5 1500

/* ── Track multipliers (x100) for scaled stats ────────────────── */

#define BOND_MULT_RANK_1 100 /* x1.0  */
#define BOND_MULT_RANK_2 110 /* x1.1  */
#define BOND_MULT_RANK_3 120 /* x1.2  */
#define BOND_MULT_RANK_4 135 /* x1.35 */
#define BOND_MULT_RANK_5 150 /* x1.5  */

/* ── Static bonus tables (Guard / Keen) ───────────────────────── */

/* Guard: ac_mod scales with base_level, parry_mod is flat */
/* ac_mod formula: base_level * guard_ac_scale[rank-1] / 100       */
#define GUARD_AC_SCALE_1 33  /* ~0.33x base_level */
#define GUARD_AC_SCALE_2 66  /* ~0.66x            */
#define GUARD_AC_SCALE_3 100 /* 1.0x              */
#define GUARD_AC_SCALE_4 150 /* 1.5x              */
#define GUARD_AC_SCALE_5 200 /* 2.0x              */

/* Guard: flat parry_mod per rank */
#define GUARD_PARRY_1 1
#define GUARD_PARRY_2 2
#define GUARD_PARRY_3 3
#define GUARD_PARRY_4 4
#define GUARD_PARRY_5 5

/* Keen: flat crit_mod per rank */
#define KEEN_CRIT_1 1
#define KEEN_CRIT_2 2
#define KEEN_CRIT_3 3
#define KEEN_CRIT_4 4
#define KEEN_CRIT_5 5

/* Keen: flat crit_mult_mod per rank */
#define KEEN_CRIT_MULT_1 2
#define KEEN_CRIT_MULT_2 4
#define KEEN_CRIT_MULT_3 6
#define KEEN_CRIT_MULT_4 9
#define KEEN_CRIT_MULT_5 12

/* ── Broken Bond debuff ───────────────────────────────────────── */

#define BOND_BROKEN_DURATION 10  /* ticks (~5-10 minutes real time) */
#define BOND_MASTERY_QP_COST 100 /* quest points to unlock mastery   */

/* ── Weapon type: swords are value[3] == 3 (slash) ────────────── */

#define WEAPON_TYPE_SWORD 3

/* ── Bonded weapon marker: uses ITEM_BONDED extra flag (BIT_33) ── */
/* IS_OBJ_BONDED is implemented as a static inline function in inlines.h */

/* ── Bond Data (stored on PC_DATA, persisted in player files) ─── */

struct bond_data
{
   int base_obj_vnum;  /* original weapon template vnum           */
   int base_level;     /* level of weapon when bonded (1-150)     */
   int bond_points;    /* accumulated from combat kills           */
   int track;          /* BOND_TRACK_NONE .. BOND_TRACK_SPIRIT    */
   int rank;           /* 1..5 within chosen track                */
   bool mastery;       /* TRUE if mastery objfun is active        */
   char *weapon_name;  /* custom name (or inherited from base)    */
   char *weapon_short; /* short description for display           */
};

typedef struct bond_data BOND_DATA;

/* ── Public API ───────────────────────────────────────────────── */

/* Master command handler: "bond <subcommand>" */
void do_bond(CHAR_DATA *ch, char *argument);

/* Return TRUE if obj is a bonded weapon owned by ch. */
bool is_bonded_weapon(CHAR_DATA *ch, OBJ_DATA *obj);

/* Recalculate bonded weapon stats for ch's current level.
 * Called on level-up and when equipping the bonded weapon.       */
void bond_recalculate(CHAR_DATA *ch);

/* Award bond points after a kill (called from damage.c).        */
void bond_award_kill(CHAR_DATA *ch, CHAR_DATA *victim);

/* Return the track name string for a track ID. */
const char *bond_track_name(int track_id);

/* Return the bond point cost for a given rank (1-indexed). */
int bond_rank_cost(int rank);

/* Return the track multiplier (x100) for a given rank. */
int bond_rank_multiplier(int rank);

/* Return the guard track ac_mod for a given rank and base_level. */
int bond_guard_ac(int rank, int base_level);

/* Return the guard track parry_mod for a given rank. */
int bond_guard_parry(int rank);

/* Return the keen track crit_mod for a given rank. */
int bond_keen_crit(int rank);

/* Return the keen track crit_mult_mod for a given rank. */
int bond_keen_crit_mult(int rank);

/* Free bond data (used on character cleanup). */
void bond_free(BOND_DATA *bond);

/* ── Mastery objfuns (one per track, assigned at rank 5) ──────── */

DECLARE_OBJ_FUN(objfun_bond_edge);   /* Edge mastery: bonus slash attacks    */
DECLARE_OBJ_FUN(objfun_bond_guard);  /* Guard mastery: passive damage shield */
DECLARE_OBJ_FUN(objfun_bond_keen);   /* Keen mastery: deep wound bleed       */
DECLARE_OBJ_FUN(objfun_bond_spirit); /* Spirit mastery: mana/hp regen pulse  */

#endif /* DEC_WEAPON_BOND_H */
