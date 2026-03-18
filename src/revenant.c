/*
 * revenant.c — Revenant (Undead Minion) System
 *
 * A revenant is raised from an NPC corpse.  The corpse's original mob
 * stats form the revenant's power budget.  Better corpse = better
 * revenant.  Tracks multiply what the corpse provides.
 *
 * Corpse level capped at caster pseudo_level, hard cap 150.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "tables.h"
#include "magic.h"
#include "revenant.h"

/* ── Track names ──────────────────────────────────────────────── */

static const char *track_names[] = {
    "none",    /* 0  */
    "dread",   /* 1  - primary: offensive melee         */
    "shadow",  /* 2  - primary: evasion/ambush           */
    "bone",    /* 3  - primary: tanking/defense           */
    "soul",    /* 4  - primary: spellcasting              */
    "plague",  /* 5  - primary: dots/debuffs              */
    "blood",   /* 6  - primary: lifesteal/sustain         */
    "wrath",   /* 7  - secondary: damage aura             */
    "ward",    /* 8  - secondary: protection aura          */
    "hunger",  /* 9  - secondary: mana siphon              */
    "spite",   /* 10 - secondary: retribution              */
    "whisper", /* 11 - secondary: buff support             */
    "decay",   /* 12 - secondary: debuff aura              */
};

const char *revenant_track_name(int track_id)
{
   if (track_id < 0 || track_id > REV_MAX_TRACK)
      return "unknown";
   return track_names[track_id];
}

/* ── Soul costs ───────────────────────────────────────────────── */

static const int primary_soul_costs[REV_MAX_PRIMARY_RANK] = {
    REV_SOUL_RANK_1, REV_SOUL_RANK_2, REV_SOUL_RANK_3, REV_SOUL_RANK_4, REV_SOUL_RANK_5,
};

static const int secondary_soul_costs[REV_MAX_SECONDARY_RANK] = {
    REV_SOUL_SEC_RANK_1,
    REV_SOUL_SEC_RANK_2,
    REV_SOUL_SEC_RANK_3,
};

int revenant_soul_cost_primary(int rank)
{
   if (rank < 1 || rank > REV_MAX_PRIMARY_RANK)
      return 0;
   return primary_soul_costs[rank - 1];
}

int revenant_soul_cost_secondary(int rank)
{
   if (rank < 1 || rank > REV_MAX_SECONDARY_RANK)
      return 0;
   return secondary_soul_costs[rank - 1];
}

/* ── Track multipliers (returned as x100) ─────────────────────── */

int revenant_primary_multiplier(int rank)
{
   /* Rank 1 = x1.0, 2 = x1.1, 3 = x1.2, 4 = x1.35, 5 = x1.5 */
   static const int mults[] = {100, 110, 120, 135, 150};

   if (rank < 1)
      return 100;
   if (rank > REV_MAX_PRIMARY_RANK)
      rank = REV_MAX_PRIMARY_RANK;
   return mults[rank - 1];
}

int revenant_secondary_multiplier(int rank)
{
   /* Rank 1 = 5%, 2 = 10%, 3 = 15% of stat shared to master */
   static const int mults[] = {5, 10, 15};

   if (rank < 1)
      return 5;
   if (rank > REV_MAX_SECONDARY_RANK)
      rank = REV_MAX_SECONDARY_RANK;
   return mults[rank - 1];
}

/* ── Helpers ──────────────────────────────────────────────────── */

bool is_revenant(CHAR_DATA *ch)
{
   return (ch != NULL && IS_NPC(ch) && ch->revenant != NULL);
}

CHAR_DATA *find_revenant(CHAR_DATA *ch)
{
   CHAR_DATA *fch;

   if (ch == NULL || ch->in_room == NULL)
      return NULL;

   for (fch = first_char; fch != NULL; fch = fch->next)
   {
      if (is_revenant(fch) && fch->master == ch)
         return fch;
   }
   return NULL;
}

static bool is_primary_track(int track_id)
{
   return (track_id >= REV_TRACK_DREAD && track_id <= REV_TRACK_BLOOD);
}

static bool is_secondary_track(int track_id)
{
   return (track_id >= REV_TRACK_WRATH && track_id <= REV_TRACK_DECAY);
}

/* Scale a base stat by track multiplier (x100). */
static int scale_stat(int base, int mult_x100)
{
   return base * mult_x100 / 100;
}

/* ── Apply track-scaled stats ─────────────────────────────────── */

void revenant_apply_tracks(CHAR_DATA *rev)
{
   REVENANT_DATA *rd;
   int pri_mult;

   if (!is_revenant(rev))
      return;

   rd = rev->revenant;
   pri_mult = revenant_primary_multiplier(rd->primary_rank);

   /* Start from base stats */
   rev->hr_mod = rd->base_hr_mod;
   rev->dr_mod = rd->base_dr_mod;
   rev->ac_mod = rd->base_ac_mod;
   rev->hp_mod = rd->base_hp_mod;
   rev->spellpower_mod = rd->base_spellpower_mod;
   rev->healing_mod = rd->base_healing_mod;
   rev->crit_mod = rd->base_crit_mod;
   rev->crit_mult_mod = rd->base_crit_mult_mod;
   rev->spell_crit_mod = rd->base_spell_crit_mod;
   rev->spell_mult_mod = rd->base_spell_mult_mod;
   rev->parry_mod = rd->base_parry_mod;
   rev->dodge_mod = rd->base_dodge_mod;
   rev->block_mod = rd->base_block_mod;
   rev->pierce_mod = rd->base_pierce_mod;

   /* Apply primary track bonuses */
   switch (rd->primary_track)
   {
   case REV_TRACK_DREAD: /* offensive melee: hr, dr, crit */
      rev->hr_mod = scale_stat(rd->base_hr_mod, pri_mult);
      rev->dr_mod = scale_stat(rd->base_dr_mod, pri_mult);
      rev->crit_mod = scale_stat(rd->base_crit_mod, pri_mult);
      break;

   case REV_TRACK_SHADOW: /* evasion: dodge, parry, pierce */
      rev->dodge_mod = scale_stat(rd->base_dodge_mod, pri_mult);
      rev->parry_mod = scale_stat(rd->base_parry_mod, pri_mult);
      rev->pierce_mod = scale_stat(rd->base_pierce_mod, pri_mult);
      break;

   case REV_TRACK_BONE: /* tanking: ac, block, hp */
      rev->ac_mod = scale_stat(rd->base_ac_mod, pri_mult);
      rev->block_mod = scale_stat(rd->base_block_mod, pri_mult);
      rev->hp_mod = (long)rd->base_hp_mod * pri_mult / 100;
      break;

   case REV_TRACK_SOUL: /* spellcasting: spellpower, spell crit */
      rev->spellpower_mod = scale_stat(rd->base_spellpower_mod, pri_mult);
      rev->spell_crit_mod = scale_stat(rd->base_spell_crit_mod, pri_mult);
      rev->spell_mult_mod = scale_stat(rd->base_spell_mult_mod, pri_mult);
      break;

   case REV_TRACK_PLAGUE: /* dots/debuffs: level drives save DCs */
      /* Plague track doesn't boost flat stats, it enhances debuff */
      /* application chance, handled in the combat spec function.  */
      break;

   case REV_TRACK_BLOOD: /* lifesteal: healing, hr (drain scales w/ hits) */
      rev->healing_mod = scale_stat(rd->base_healing_mod, pri_mult);
      rev->hr_mod = scale_stat(rd->base_hr_mod, pri_mult);
      break;

   default:
      break;
   }

   /* Recalculate HP from hp_mod */
   rev->hit = get_max_hp(rev);
}

/* ── Create a revenant from a corpse ──────────────────────────── */

CHAR_DATA *revenant_create(CHAR_DATA *caster, OBJ_DATA *corpse)
{
   CHAR_DATA *rev;
   MOB_INDEX_DATA *pMob;
   REVENANT_DATA *rd;
   int rev_level;
   char buf[MAX_STRING_LENGTH];

   if (corpse == NULL || corpse->item_type != ITEM_CORPSE_NPC)
      return NULL;

   /* value[4] stores the original mob vnum (set in make_corpse) */
   pMob = get_mob_index(corpse->value[4]);
   if (pMob == NULL)
   {
      send_to_char("The corpse is too decayed to raise.\n\r", caster);
      return NULL;
   }

   /* Level cap: min(corpse level, caster pseudo_level, 150) */
   rev_level = corpse->level;
   rev_level = UMIN(rev_level, get_psuedo_level(caster));
   rev_level = UMIN(rev_level, REV_LEVEL_CAP);
   rev_level = UMAX(rev_level, 1);

   /* Create the mob from the zombie template */
   rev = create_mobile(get_mob_index(MOB_VNUM_ZOMBIE));
   if (rev == NULL)
      return NULL;

   char_to_room(rev, caster->in_room);

   /* ── Set level and recalculate HP ── */
   rev->level = rev_level;

   /* ── Copy combat stats from the ORIGINAL mob template ── */
   rev->ac_mod = pMob->ac_mod;
   rev->hr_mod = pMob->hr_mod;
   rev->dr_mod = pMob->dr_mod;
   rev->spellpower_mod = pMob->spellpower_mod;
   rev->healing_mod = pMob->healing_mod;
   rev->crit_mod = pMob->crit_mod;
   rev->crit_mult_mod = pMob->crit_mult_mod;
   rev->spell_crit_mod = pMob->spell_crit_mod;
   rev->spell_mult_mod = pMob->spell_mult_mod;
   rev->parry_mod = pMob->parry_mod;
   rev->dodge_mod = pMob->dodge_mod;
   rev->block_mod = pMob->block_mod;
   rev->pierce_mod = pMob->pierce_mod;

   /* Copy combat flags */
   rev->skills = pMob->skills;
   rev->cast = pMob->cast;
   rev->def = pMob->def;
   rev->class = pMob->class;
   rev->strong_magic = pMob->strong_magic;
   rev->weak_magic = pMob->weak_magic;
   rev->power_skills = pMob->power_skills;
   rev->power_cast = pMob->power_cast;

   /* Inherit resistances, add undead immunities */
   rev->resist = pMob->resist | ELE_POISON;
   rev->suscept = pMob->suscept | ELE_HOLY;

   /* Calculate HP: use the mob template's hp_mod as a base,
    * then scale by the revenant's (possibly capped) level vs original. */
   if (pMob->level > 0)
   {
      long hp_base = pMob->hp_mod + rev_level * 15 +
                     number_range(rev_level * rev_level * 2, rev_level * rev_level * 4);
      rev->hp_mod = hp_base;
   }
   rev->hit = get_max_hp(rev);

   /* Mana */
   rev->max_mana = rev_level * 25;
   rev->mana = rev->max_mana;
   rev->exp = 0;
   rev->intell_exp = 0;

   /* ── Set descriptions ── */
   free_string(rev->name);
   sprintf(buf, "revenant %s", pMob->player_name);
   rev->name = str_dup(buf);

   free_string(rev->short_descr);
   sprintf(buf, "@@da revenant of %s@@N", pMob->short_descr);
   rev->short_descr = str_dup(buf);

   free_string(rev->long_descr);
   sprintf(buf, "@@dA revenant of %s@@d stands here, awaiting orders.@@N\n\r", pMob->short_descr);
   rev->long_descr = str_dup(buf);

   /* ── Set act/affect flags ── */
   SET_BIT(rev->act, ACT_UNDEAD);
   SET_BIT(rev->act, ACT_PET);
   SET_BIT(rev->affected_by, AFF_CHARM);
   rev->extract_timer = -1; /* permanent — does not expire */

   /* ── Assign spec function ── */
   rev->spec_fun = spec_lookup("spec_summon_revenant");

   /* ── Allocate and initialize revenant data ── */
   rd = (REVENANT_DATA *)calloc(1, sizeof(REVENANT_DATA));
   rd->source_vnum = pMob->vnum;
   rd->corpse_level = rev_level;
   rd->soul_points = 0;
   rd->primary_track = REV_TRACK_NONE;
   rd->primary_rank = 1;
   rd->secondary_track = REV_TRACK_NONE;
   rd->secondary_rank = 1;

   /* Store base stats for track scaling */
   rd->base_hr_mod = rev->hr_mod;
   rd->base_dr_mod = rev->dr_mod;
   rd->base_ac_mod = rev->ac_mod;
   rd->base_hp_mod = rev->hp_mod;
   rd->base_spellpower_mod = rev->spellpower_mod;
   rd->base_healing_mod = rev->healing_mod;
   rd->base_crit_mod = rev->crit_mod;
   rd->base_crit_mult_mod = rev->crit_mult_mod;
   rd->base_spell_crit_mod = rev->spell_crit_mod;
   rd->base_spell_mult_mod = rev->spell_mult_mod;
   rd->base_parry_mod = rev->parry_mod;
   rd->base_dodge_mod = rev->dodge_mod;
   rd->base_block_mod = rev->block_mod;
   rd->base_pierce_mod = rev->pierce_mod;

   rev->revenant = rd;

   /* ── Transfer corpse inventory to revenant ── */
   {
      OBJ_DATA *obj;
      for (;;)
      {
         if (corpse->first_in_carry_list == NULL)
            break;
         obj = corpse->first_in_carry_list;
         obj_from_obj(obj);
         obj_to_char(obj, rev);
      }
   }

   /* Destroy the corpse */
   extract_obj(corpse);

   /* Equip whatever was in the corpse */
   do_wear(rev, "all");

   /* Become follower */
   add_follower(rev, caster);

   return rev;
}

/* ── Infuse (feed a corpse for soul points) ───────────────────── */

void revenant_infuse(CHAR_DATA *master, CHAR_DATA *rev, OBJ_DATA *corpse)
{
   REVENANT_DATA *rd;
   int souls_gained;
   char buf[MAX_STRING_LENGTH];

   if (!is_revenant(rev) || corpse == NULL)
      return;

   rd = rev->revenant;

   if (corpse->item_type != ITEM_CORPSE_NPC)
   {
      send_to_char("Your revenant can only consume NPC corpses.\n\r", master);
      return;
   }

   /* Soul points gained = corpse level (better corpses = more souls) */
   souls_gained = UMAX(1, corpse->level / 2);
   rd->soul_points += souls_gained;

   sprintf(buf,
           "@@dYour revenant consumes the corpse, absorbing @@W%d@@d soul point%s. "
           "(Total: @@W%d@@d)@@N\n\r",
           souls_gained, souls_gained == 1 ? "" : "s", rd->soul_points);
   send_to_char(buf, master);

   act("@@d$N shudders as dark energy flows from $p into its form!@@N", master, corpse, rev,
       TO_ROOM);

   extract_obj(corpse);
}

/* ── Set track ────────────────────────────────────────────────── */

bool revenant_set_track(CHAR_DATA *master, CHAR_DATA *rev, int track_id)
{
   REVENANT_DATA *rd;
   char buf[MAX_STRING_LENGTH];

   if (!is_revenant(rev))
      return FALSE;

   rd = rev->revenant;

   if (is_primary_track(track_id))
   {
      if (rd->primary_track == track_id)
      {
         send_to_char("Your revenant is already on that primary track.\n\r", master);
         return FALSE;
      }
      if (rd->primary_track != REV_TRACK_NONE)
      {
         sprintf(buf,
                 "@@WWARNING:@@N Switching primary track from @@W%s@@N will @@Rreset@@N "
                 "rank to 1 and @@Rforfeit@@N invested soul points.\n\r",
                 revenant_track_name(rd->primary_track));
         send_to_char(buf, master);
      }
      rd->primary_track = track_id;
      rd->primary_rank = 1;
      revenant_apply_tracks(rev);
      sprintf(buf, "@@dYour revenant is now on the @@W%s@@d primary track (rank 1).@@N\n\r",
              revenant_track_name(track_id));
      send_to_char(buf, master);
      return TRUE;
   }
   else if (is_secondary_track(track_id))
   {
      if (rd->secondary_track == track_id)
      {
         send_to_char("Your revenant is already on that secondary track.\n\r", master);
         return FALSE;
      }
      if (rd->secondary_track != REV_TRACK_NONE)
      {
         sprintf(buf,
                 "@@WWARNING:@@N Switching secondary track from @@W%s@@N will @@Rreset@@N "
                 "rank to 1 and @@Rforfeit@@N invested soul points.\n\r",
                 revenant_track_name(rd->secondary_track));
         send_to_char(buf, master);
      }
      rd->secondary_track = track_id;
      rd->secondary_rank = 1;
      sprintf(buf, "@@dYour revenant is now on the @@W%s@@d secondary track (rank 1).@@N\n\r",
              revenant_track_name(track_id));
      send_to_char(buf, master);
      return TRUE;
   }

   send_to_char("That is not a valid track.\n\r", master);
   return FALSE;
}

/* ── Rank up ──────────────────────────────────────────────────── */

static void revenant_try_rankup(CHAR_DATA *master, CHAR_DATA *rev)
{
   REVENANT_DATA *rd;
   int cost;
   char buf[MAX_STRING_LENGTH];

   if (!is_revenant(rev))
      return;

   rd = rev->revenant;

   /* Try primary track first */
   if (rd->primary_track != REV_TRACK_NONE && rd->primary_rank < REV_MAX_PRIMARY_RANK)
   {
      cost = revenant_soul_cost_primary(rd->primary_rank + 1);
      if (rd->soul_points >= cost)
      {
         rd->soul_points -= cost;
         rd->primary_rank++;
         revenant_apply_tracks(rev);
         sprintf(buf,
                 "@@dYour revenant's @@W%s@@d track advances to @@Wrank %d@@d! "
                 "(@@W%d@@d soul points remaining)@@N\n\r",
                 revenant_track_name(rd->primary_track), rd->primary_rank, rd->soul_points);
         send_to_char(buf, master);
         return;
      }
      else
      {
         sprintf(buf,
                 "@@dNot enough soul points. @@W%s@@d rank %d requires @@W%d@@d "
                 "soul points (you have @@W%d@@d).@@N\n\r",
                 revenant_track_name(rd->primary_track), rd->primary_rank + 1, cost,
                 rd->soul_points);
         send_to_char(buf, master);
         return;
      }
   }

   /* Try secondary track */
   if (rd->secondary_track != REV_TRACK_NONE && rd->secondary_rank < REV_MAX_SECONDARY_RANK)
   {
      cost = revenant_soul_cost_secondary(rd->secondary_rank + 1);
      if (rd->soul_points >= cost)
      {
         rd->soul_points -= cost;
         rd->secondary_rank++;
         sprintf(buf,
                 "@@dYour revenant's @@W%s@@d track advances to @@Wrank %d@@d! "
                 "(@@W%d@@d soul points remaining)@@N\n\r",
                 revenant_track_name(rd->secondary_track), rd->secondary_rank, rd->soul_points);
         send_to_char(buf, master);
         return;
      }
      else
      {
         sprintf(buf,
                 "@@dNot enough soul points. @@W%s@@d rank %d requires @@W%d@@d "
                 "soul points (you have @@W%d@@d).@@N\n\r",
                 revenant_track_name(rd->secondary_track), rd->secondary_rank + 1, cost,
                 rd->soul_points);
         send_to_char(buf, master);
         return;
      }
   }

   if (rd->primary_track == REV_TRACK_NONE && rd->secondary_track == REV_TRACK_NONE)
      send_to_char("Your revenant has no tracks set. Use 'revenant track <name>' first.\n\r",
                   master);
   else
      send_to_char("Your revenant's tracks are already at maximum rank.\n\r", master);
}

/* ── Status display ───────────────────────────────────────────── */

static void revenant_status(CHAR_DATA *master, CHAR_DATA *rev)
{
   REVENANT_DATA *rd;
   MOB_INDEX_DATA *pMob;
   char buf[MAX_STRING_LENGTH];

   if (!is_revenant(rev))
      return;

   rd = rev->revenant;
   pMob = get_mob_index(rd->source_vnum);

   send_to_char("@@d╔══════════════════════════════════════════╗@@N\n\r", master);
   send_to_char("@@d║         @@WREVENANT STATUS@@d                 ║@@N\n\r", master);
   send_to_char("@@d╠══════════════════════════════════════════╣@@N\n\r", master);

   sprintf(buf, "@@d║ @@WSource:@@N  %-30s@@d║@@N\n\r", pMob ? pMob->short_descr : "unknown");
   send_to_char(buf, master);

   sprintf(buf, "@@d║ @@WLevel:@@N   %-3d  @@d(corpse: %d, cap: %d)     @@d║@@N\n\r", rev->level,
           rd->corpse_level, REV_LEVEL_CAP);
   send_to_char(buf, master);

   sprintf(buf, "@@d║ @@WSouls:@@N   %-34d@@d║@@N\n\r", rd->soul_points);
   send_to_char(buf, master);

   sprintf(buf, "@@d║ @@WHP:@@N      %ld / %ld%-23s@@d║@@N\n\r", rev->hit, get_max_hp(rev), "");
   send_to_char(buf, master);

   send_to_char("@@d╠══════════════════════════════════════════╣@@N\n\r", master);
   send_to_char("@@d║ @@WCOMBAT STATS (base / effective)@@d         ║@@N\n\r", master);
   send_to_char("@@d╠══════════════════════════════════════════╣@@N\n\r", master);

   sprintf(buf, "@@d║ @@WHitroll:@@N  %4d / %-4d   @@WDamroll:@@N %4d/%-4d@@d║@@N\n\r",
           rd->base_hr_mod, rev->hr_mod, rd->base_dr_mod, rev->dr_mod);
   send_to_char(buf, master);

   sprintf(buf, "@@d║ @@WAC Mod:@@N   %4d / %-4d   @@WSpPow:@@N   %4d/%-4d@@d║@@N\n\r",
           rd->base_ac_mod, rev->ac_mod, rd->base_spellpower_mod, rev->spellpower_mod);
   send_to_char(buf, master);

   sprintf(buf, "@@d║ @@WCrit:@@N     %4d / %-4d   @@WDodge:@@N   %4d/%-4d@@d║@@N\n\r",
           rd->base_crit_mod, rev->crit_mod, rd->base_dodge_mod, rev->dodge_mod);
   send_to_char(buf, master);

   sprintf(buf, "@@d║ @@WParry:@@N    %4d / %-4d   @@WBlock:@@N   %4d/%-4d@@d║@@N\n\r",
           rd->base_parry_mod, rev->parry_mod, rd->base_block_mod, rev->block_mod);
   send_to_char(buf, master);

   sprintf(buf, "@@d║ @@WPierce:@@N   %4d / %-4d   @@WHeal:@@N    %4d/%-4d@@d║@@N\n\r",
           rd->base_pierce_mod, rev->pierce_mod, rd->base_healing_mod, rev->healing_mod);
   send_to_char(buf, master);

   send_to_char("@@d╠══════════════════════════════════════════╣@@N\n\r", master);
   send_to_char("@@d║ @@WTRACKS@@d                                  ║@@N\n\r", master);
   send_to_char("@@d╠══════════════════════════════════════════╣@@N\n\r", master);

   if (rd->primary_track != REV_TRACK_NONE)
   {
      sprintf(buf, "@@d║ @@WPrimary:@@N  %-10s @@Wrank %d@@N / %d       @@d║@@N\n\r",
              revenant_track_name(rd->primary_track), rd->primary_rank, REV_MAX_PRIMARY_RANK);
      send_to_char(buf, master);
      sprintf(buf, "@@d║           @@Wmultiplier: x%.2f@@N             @@d║@@N\n\r",
              revenant_primary_multiplier(rd->primary_rank) / 100.0);
      send_to_char(buf, master);
   }
   else
      send_to_char("@@d║ @@WPrimary:@@N  @@Rnone@@N                          @@d║@@N\n\r", master);

   if (rd->secondary_track != REV_TRACK_NONE)
   {
      sprintf(buf, "@@d║ @@WSecondary:@@N%-10s @@Wrank %d@@N / %d       @@d║@@N\n\r",
              revenant_track_name(rd->secondary_track), rd->secondary_rank, REV_MAX_SECONDARY_RANK);
      send_to_char(buf, master);
      sprintf(buf, "@@d║           @@Wshares %d%%%%@@N of stat to master   @@d║@@N\n\r",
              revenant_secondary_multiplier(rd->secondary_rank));
      send_to_char(buf, master);
   }
   else
      send_to_char("@@d║ @@WSecondary:@@N@@Rnone@@N                          @@d║@@N\n\r", master);

   /* Next rank costs */
   if (rd->primary_track != REV_TRACK_NONE && rd->primary_rank < REV_MAX_PRIMARY_RANK)
   {
      sprintf(buf, "@@d║ @@WNext primary rank:@@N  %d soul points      @@d║@@N\n\r",
              revenant_soul_cost_primary(rd->primary_rank + 1));
      send_to_char(buf, master);
   }
   if (rd->secondary_track != REV_TRACK_NONE && rd->secondary_rank < REV_MAX_SECONDARY_RANK)
   {
      sprintf(buf, "@@d║ @@WNext secondary rank:@@N%d soul points      @@d║@@N\n\r",
              revenant_soul_cost_secondary(rd->secondary_rank + 1));
      send_to_char(buf, master);
   }

   send_to_char("@@d╚══════════════════════════════════════════╝@@N\n\r", master);

   /* Resistances */
   if (rev->resist)
   {
      sprintf(buf, "@@d  Resists:@@N ");
      if (IS_SET(rev->resist, ELE_PHYSICAL))
         strcat(buf, "physical ");
      if (IS_SET(rev->resist, ELE_MENTAL))
         strcat(buf, "mental ");
      if (IS_SET(rev->resist, ELE_HOLY))
         strcat(buf, "holy ");
      if (IS_SET(rev->resist, ELE_AIR))
         strcat(buf, "air ");
      if (IS_SET(rev->resist, ELE_EARTH))
         strcat(buf, "earth ");
      if (IS_SET(rev->resist, ELE_WATER))
         strcat(buf, "water ");
      if (IS_SET(rev->resist, ELE_FIRE))
         strcat(buf, "fire ");
      if (IS_SET(rev->resist, ELE_SHADOW))
         strcat(buf, "shadow ");
      if (IS_SET(rev->resist, ELE_POISON))
         strcat(buf, "poison ");
      strcat(buf, "\n\r");
      send_to_char(buf, master);
   }
   if (rev->suscept)
   {
      sprintf(buf, "@@d  Suscept:@@R ");
      if (IS_SET(rev->suscept, ELE_PHYSICAL))
         strcat(buf, "physical ");
      if (IS_SET(rev->suscept, ELE_MENTAL))
         strcat(buf, "mental ");
      if (IS_SET(rev->suscept, ELE_HOLY))
         strcat(buf, "holy ");
      if (IS_SET(rev->suscept, ELE_AIR))
         strcat(buf, "air ");
      if (IS_SET(rev->suscept, ELE_EARTH))
         strcat(buf, "earth ");
      if (IS_SET(rev->suscept, ELE_WATER))
         strcat(buf, "water ");
      if (IS_SET(rev->suscept, ELE_FIRE))
         strcat(buf, "fire ");
      if (IS_SET(rev->suscept, ELE_SHADOW))
         strcat(buf, "shadow ");
      if (IS_SET(rev->suscept, ELE_POISON))
         strcat(buf, "poison ");
      strcat(buf, "@@N\n\r");
      send_to_char(buf, master);
   }
}

/* ── Aura tick (called from update.c) ─────────────────────────── */

void revenant_aura_tick(CHAR_DATA *rev)
{
   REVENANT_DATA *rd;
   CHAR_DATA *master;
   int share;
   char buf[MAX_STRING_LENGTH];

   if (!is_revenant(rev))
      return;

   rd = rev->revenant;
   master = rev->master;

   if (master == NULL || master->in_room != rev->in_room)
      return;

   share = revenant_secondary_multiplier(rd->secondary_rank);

   switch (rd->secondary_track)
   {
   case REV_TRACK_WRATH: /* master gains bonus damroll */
   {
      int bonus = rd->base_dr_mod * share / 100;

      if (bonus > 0)
      {
         sprintf(buf,
                 "@@dYour revenant's @@Rwrath@@d aura empowers you. "
                 "(@@W+%d@@d damroll)@@N\n\r",
                 bonus);
         send_to_char(buf, master);
      }
      break;
   }

   case REV_TRACK_WARD: /* master gains AC bonus */
   {
      int bonus = rd->base_ac_mod * share / 100;

      if (bonus != 0)
      {
         sprintf(buf, "@@dYour revenant's @@lward@@d aura shields you. (@@W%+d@@d AC)@@N\n\r",
                 bonus);
         send_to_char(buf, master);
      }
      break;
   }

   case REV_TRACK_HUNGER: /* master regens mana */
   {
      int bonus = UMAX(1, rd->base_spellpower_mod * share / 100);
      long max_mana = get_max_mana(master);

      if (master->mana < max_mana)
      {
         master->mana = UMIN(master->mana + bonus, max_mana);
         sprintf(buf,
                 "@@dYour revenant's @@ehunger@@d aura siphons mana to you. "
                 "(@@W+%d@@d mana)@@N\n\r",
                 bonus);
         send_to_char(buf, master);
      }
      break;
   }

   case REV_TRACK_WHISPER: /* periodic minor buff message */
   {
      int bonus = UMAX(1, rev->level * share / 100);

      sprintf(buf,
              "@@dYour revenant @@ewhispers@@d dark power into your mind. "
              "(@@W+%d@@d to saves)@@N\n\r",
              bonus);
      send_to_char(buf, master);
      break;
   }

   case REV_TRACK_DECAY: /* enemies in room are weakened */
   {
      CHAR_DATA *vch;
      int penalty = UMAX(1, rev->level * share / 100);

      for (vch = rev->in_room->first_person; vch != NULL; vch = vch->next_in_room)
      {
         if (vch == rev || vch == master || vch->master == master)
            continue;
         if (!is_fighting(vch))
            continue;

         sprintf(buf, "@@dThe @@eaura of decay@@d from %s@@d weakens you!@@N\n\r",
                 rev->short_descr);
         send_to_char(buf, vch);
         (void)penalty; /* debuff applied in combat calculations */
      }
      break;
   }

   default:
      break;
   }
}

/* ── Spite retribution (called when master takes damage) ──────── */

int revenant_spite_retaliate(CHAR_DATA *rev, CHAR_DATA *attacker, int damage_taken)
{
   REVENANT_DATA *rd;
   int share;
   int retaliation;

   if (!is_revenant(rev) || attacker == NULL)
      return 0;

   rd = rev->revenant;
   if (rd->secondary_track != REV_TRACK_SPITE)
      return 0;

   if (rev->in_room != attacker->in_room)
      return 0;

   share = revenant_secondary_multiplier(rd->secondary_rank);
   retaliation = UMAX(1, rd->base_hr_mod * share / 100);

   /* Cap retaliation at a fraction of damage taken */
   retaliation = UMIN(retaliation, damage_taken / 2);

   if (retaliation > 0)
   {
      act("@@d$N lashes out in @@Rspite@@d, retaliating against $n!@@N", attacker, NULL, rev,
          TO_ROOM);
   }

   return retaliation;
}

/* ── Track lookup by name ─────────────────────────────────────── */

static int track_lookup(const char *name)
{
   int i;

   for (i = 1; i <= REV_MAX_TRACK; i++)
   {
      if (!str_prefix(name, track_names[i]))
         return i;
   }
   return REV_TRACK_NONE;
}

/* ── Master command: "revenant <subcommand>" ──────────────────── */

void do_revenant(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *rev;

   if (IS_NPC(ch))
      return;

   argument = one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("@@dRevenant commands:@@N\n\r", ch);
      send_to_char("  @@Wrevenant status@@N        - View your revenant's stats and tracks\n\r",
                   ch);
      send_to_char("  @@Wrevenant infuse <corpse>@@N- Feed a corpse for soul points\n\r", ch);
      send_to_char("  @@Wrevenant track <name>@@N   - Set primary or secondary track\n\r", ch);
      send_to_char("  @@Wrevenant rankup@@N         - Spend soul points to advance a track\n\r",
                   ch);
      send_to_char("  @@Wrevenant tracks@@N         - List all available tracks\n\r", ch);
      send_to_char("\n\rSee @@Whelp revenant@@N for full details.\n\r", ch);
      return;
   }

   /* Find the player's revenant */
   rev = find_revenant(ch);

   if (!str_prefix(arg, "tracks"))
   {
      send_to_char("@@d══════════════════════════════════════════@@N\n\r", ch);
      send_to_char("@@W  PRIMARY TRACKS@@N (5 ranks, active combat)\n\r", ch);
      send_to_char("@@d══════════════════════════════════════════@@N\n\r", ch);
      send_to_char("  @@Wdread@@N   - Offensive melee (hitroll, damroll, crit)\n\r", ch);
      send_to_char("  @@Wshadow@@N  - Evasion/ambush (dodge, parry, pierce)\n\r", ch);
      send_to_char("  @@Wbone@@N    - Tanking/defense (AC, block, HP)\n\r", ch);
      send_to_char("  @@Wsoul@@N    - Spellcasting (spellpower, spell crit)\n\r", ch);
      send_to_char("  @@Wplague@@N  - DoTs/debuffs (level-based save DCs)\n\r", ch);
      send_to_char("  @@Wblood@@N   - Lifesteal/sustain (healing, hitroll)\n\r", ch);
      send_to_char("\n\r", ch);
      send_to_char("@@W  SECONDARY TRACKS@@N (3 ranks, passive aura on master)\n\r", ch);
      send_to_char("@@d══════════════════════════════════════════@@N\n\r", ch);
      send_to_char("  @@Wwrath@@N   - Damage aura (master +damroll)\n\r", ch);
      send_to_char("  @@Wward@@N    - Protection aura (master +AC)\n\r", ch);
      send_to_char("  @@Whunger@@N  - Mana siphon (master mana regen)\n\r", ch);
      send_to_char("  @@Wspite@@N   - Retribution (revenant retaliates)\n\r", ch);
      send_to_char("  @@Wwhisper@@N - Buff support (periodic master buffs)\n\r", ch);
      send_to_char("  @@Wdecay@@N   - Debuff aura (enemies weakened)\n\r", ch);
      send_to_char("\n\rTrack multipliers amplify the @@Wcorpse's@@N base stats.\n\r", ch);
      send_to_char("Better corpse = bigger gains from track upgrades.\n\r", ch);
      return;
   }

   if (rev == NULL)
   {
      send_to_char("You don't have a revenant follower.\n\r", ch);
      return;
   }

   if (!str_prefix(arg, "status"))
   {
      revenant_status(ch, rev);
      return;
   }

   if (!str_prefix(arg, "infuse"))
   {
      char corpse_arg[MAX_INPUT_LENGTH];
      OBJ_DATA *corpse;

      argument = one_argument(argument, corpse_arg);
      if (corpse_arg[0] == '\0')
      {
         send_to_char("Feed which corpse to your revenant?\n\r", ch);
         return;
      }

      corpse = get_obj_here(ch, corpse_arg);
      if (corpse == NULL)
      {
         send_to_char("You don't see that here.\n\r", ch);
         return;
      }

      if (corpse->item_type != ITEM_CORPSE_NPC)
      {
         send_to_char("That isn't an NPC corpse.\n\r", ch);
         return;
      }

      revenant_infuse(ch, rev, corpse);
      return;
   }

   if (!str_prefix(arg, "track"))
   {
      char track_arg[MAX_INPUT_LENGTH];
      int track_id;

      argument = one_argument(argument, track_arg);
      if (track_arg[0] == '\0')
      {
         send_to_char("Set which track? See 'revenant tracks' for the list.\n\r", ch);
         return;
      }

      track_id = track_lookup(track_arg);
      if (track_id == REV_TRACK_NONE)
      {
         send_to_char("Unknown track. See 'revenant tracks' for the list.\n\r", ch);
         return;
      }

      revenant_set_track(ch, rev, track_id);
      return;
   }

   if (!str_prefix(arg, "rankup"))
   {
      revenant_try_rankup(ch, rev);
      return;
   }

   /* Unknown subcommand */
   do_revenant(ch, "");
}
