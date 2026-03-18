/*
 * revenant.h — Revenant (Undead Minion) System
 *
 * A revenant is a permanent undead follower raised from an NPC corpse.
 * The corpse's original mob stats form the revenant's power budget:
 * better corpse = better revenant.  Tracks then multiply the chassis.
 *
 * Corpse level is capped at the caster's pseudo_level (hard cap 150).
 */

#ifndef DEC_REVENANT_H
#define DEC_REVENANT_H

/* ── Track IDs ────────────────────────────────────────────────── */

/* Primary tracks (active combat abilities, 5 ranks each) */
#define REV_TRACK_NONE 0
#define REV_TRACK_DREAD 1  /* Offensive melee: hr, dr, crit        */
#define REV_TRACK_SHADOW 2 /* Evasion/ambush: dodge, parry, pierce */
#define REV_TRACK_BONE 3   /* Tanking/defense: ac, block, hp       */
#define REV_TRACK_SOUL 4   /* Spellcasting: spellpower, spell_crit */
#define REV_TRACK_PLAGUE 5 /* DoTs/debuffs: skills flags, level DC */
#define REV_TRACK_BLOOD 6  /* Lifesteal/sustain: healing, hr       */

/* Secondary tracks (passive auras on master, 3 ranks each) */
#define REV_TRACK_WRATH 7    /* Damage aura: master +dr             */
#define REV_TRACK_WARD 8     /* Protection aura: master +ac         */
#define REV_TRACK_HUNGER 9   /* Mana siphon: master mana regen      */
#define REV_TRACK_SPITE 10   /* Retribution: revenant retaliates    */
#define REV_TRACK_WHISPER 11 /* Buff support: periodic master buffs */
#define REV_TRACK_DECAY 12   /* Debuff aura: enemies weakened       */

#define REV_MAX_PRIMARY_TRACK 6
#define REV_MAX_SECONDARY_TRACK 6
#define REV_MAX_TRACK 12

#define REV_MAX_PRIMARY_RANK 5
#define REV_MAX_SECONDARY_RANK 3

#define REV_LEVEL_CAP 150

/* Soul point costs per rank (primary: 5 ranks, secondary: 3 ranks) */
#define REV_SOUL_RANK_1 0
#define REV_SOUL_RANK_2 25
#define REV_SOUL_RANK_3 75
#define REV_SOUL_RANK_4 150
#define REV_SOUL_RANK_5 300

#define REV_SOUL_SEC_RANK_1 0
#define REV_SOUL_SEC_RANK_2 50
#define REV_SOUL_SEC_RANK_3 150

/* ── Revenant Data (stored on CHAR_DATA for NPC revenants) ──── */

/*
 * This struct is allocated for a revenant mob and pointed to
 * by CHAR_DATA.revenant_data.  NULL for non-revenants.
 */
struct revenant_data
{
   int source_vnum;     /* original mob template vnum                 */
   int corpse_level;    /* level of the corpse when raised            */
   int soul_points;     /* accumulated soul points (from feeding)     */
   int primary_track;   /* REV_TRACK_DREAD .. REV_TRACK_BLOOD, or 0  */
   int primary_rank;    /* 1..5                                       */
   int secondary_track; /* REV_TRACK_WRATH .. REV_TRACK_DECAY, or 0 */
   int secondary_rank;  /* 1..3                                      */

   /* Base stats inherited from the corpse mob (before track scaling) */
   int base_hr_mod;
   int base_dr_mod;
   int base_ac_mod;
   long base_hp_mod;
   int base_spellpower_mod;
   int base_healing_mod;
   int base_crit_mod;
   int base_crit_mult_mod;
   int base_spell_crit_mod;
   int base_spell_mult_mod;
   int base_parry_mod;
   int base_dodge_mod;
   int base_block_mod;
   int base_pierce_mod;
};

typedef struct revenant_data REVENANT_DATA;

/* ── Public API ───────────────────────────────────────────────── */

/* Create a revenant from a corpse object.  Returns the mob, or NULL. */
CHAR_DATA *revenant_create(CHAR_DATA *caster, OBJ_DATA *corpse);

/* Apply track-scaled stats to the revenant mob. */
void revenant_apply_tracks(CHAR_DATA *rev);

/* Feed a corpse to a revenant for soul points. */
void revenant_infuse(CHAR_DATA *master, CHAR_DATA *rev, OBJ_DATA *corpse);

/* Set primary or secondary track. */
bool revenant_set_track(CHAR_DATA *master, CHAR_DATA *rev, int track_id);

/* Get the track multiplier (x100) for a given rank.
 * e.g. rank 1 = 100 (×1.0), rank 5 = 150 (×1.5). */
int revenant_primary_multiplier(int rank);
int revenant_secondary_multiplier(int rank);

/* Return TRUE if ch is a revenant. */
bool is_revenant(CHAR_DATA *ch);

/* Return the revenant follower of ch, or NULL. */
CHAR_DATA *find_revenant(CHAR_DATA *ch);

/* Master command handler: "revenant <subcommand>" */
void do_revenant(CHAR_DATA *ch, char *argument);

/* Soul cost for a given primary rank (0-indexed: rank 1 = index 0). */
int revenant_soul_cost_primary(int rank);
int revenant_soul_cost_secondary(int rank);

/* Return the track name string for a track ID. */
const char *revenant_track_name(int track_id);

/* Apply secondary track aura effects during tick update. */
void revenant_aura_tick(CHAR_DATA *rev);

/* Secondary track: spite retribution (called when master takes damage). */
int revenant_spite_retaliate(CHAR_DATA *rev, CHAR_DATA *attacker, int damage_taken);

#endif /* DEC_REVENANT_H */
