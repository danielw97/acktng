/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

#ifndef DEC_MAGIC_H
#include "magic.h"
#endif

#include "special.h"

/*
 * Given a name, return the appropriate spec fun.
 */

SPEC_FUN *spec_lookup(const char *name)
{
   if (!str_cmp(name, "spec_breath_any"))
      return spec_breath_any;
   if (!str_cmp(name, "spec_breath_acid"))
      return spec_breath_acid;
   if (!str_cmp(name, "spec_breath_fire"))
      return spec_breath_fire;
   if (!str_cmp(name, "spec_breath_frost"))
      return spec_breath_frost;
   if (!str_cmp(name, "spec_breath_gas"))
      return spec_breath_gas;
   if (!str_cmp(name, "spec_breath_lightning"))
      return spec_breath_lightning;
   if (!str_cmp(name, "spec_cast_adept"))
      return spec_cast_adept;
   if (!str_cmp(name, "spec_cast_cleric"))
      return spec_cast_cleric;
   if (!str_cmp(name, "spec_cast_judge"))
      return spec_cast_judge;
   if (!str_cmp(name, "spec_cast_mage"))
      return spec_cast_mage;
   if (!str_cmp(name, "spec_cast_undead"))
      return spec_cast_undead;
   if (!str_cmp(name, "spec_executioner"))
      return spec_executioner;
   if (!str_cmp(name, "spec_fido"))
      return spec_fido;
   if (!str_cmp(name, "spec_guard"))
      return spec_policeman;
   if (!str_cmp(name, "spec_janitor"))
      return spec_janitor;
   if (!str_cmp(name, "spec_mayor"))
      return spec_mayor;
   if (!str_cmp(name, "spec_poison"))
      return spec_poison;
   if (!str_cmp(name, "spec_thief"))
      return spec_thief;
   if (!str_cmp(name, "spec_policeman"))
      return spec_policeman;
   if (!str_cmp(name, "spec_cast_cadaver"))
      return spec_cast_cadaver;
   if (!str_cmp(name, "spec_undead"))
      return spec_undead;
   if (!str_cmp(name, "spec_stephen"))
      return spec_stephen;
   if (!str_cmp(name, "spec_rewield"))
      return spec_rewield;
   if (!str_cmp(name, "spec_sylai_priest"))
      return spec_sylai_priest;
   if (!str_cmp(name, "spec_cast_bigtime"))
      return spec_cast_bigtime;
   if (!str_cmp(name, "spec_sage"))
      return spec_sage;
   if (!str_cmp(name, "spec_wizardofoz"))
      return spec_wizardofoz;
   if (!str_cmp(name, "spec_vamp_hunter"))
      return spec_vamp_hunter;
   if (!str_cmp(name, "spec_mino_guard"))
      return spec_mino_guard;
   if (!str_cmp(name, "spec_tax_man"))
      return spec_tax_man;
   if (!str_cmp(name, "spec_keep_physical_captain"))
      return spec_keep_physical_captain;
   if (!str_cmp(name, "spec_summon_water"))
      return spec_summon_water;
   if (!str_cmp(name, "spec_summon_fire"))
      return spec_summon_fire;
   if (!str_cmp(name, "spec_summon_earth"))
      return spec_summon_earth;
   if (!str_cmp(name, "spec_summon_undead"))
      return spec_summon_undead;
   if (!str_cmp(name, "spec_summon_holy"))
      return spec_summon_holy;
   if (!str_cmp(name, "spec_summon_shadow"))
      return spec_summon_shadow;
   if (!str_cmp(name, "spec_summon_metal"))
      return spec_summon_metal;
   if (!str_cmp(name, "spec_summon_animate"))
      return spec_summon_animate;
   if (!str_cmp(name, "spec_summon_thought"))
      return spec_summon_thought;
   if (!str_cmp(name, "spec_lamplighter"))
      return spec_lamplighter;
   if (!str_cmp(name, "spec_warden"))
      return spec_warden;
   if (!str_cmp(name, "spec_vendor"))
      return spec_vendor;
   if (!str_cmp(name, "spec_lay_sister"))
      return spec_lay_sister;
   if (!str_cmp(name, "spec_laborer"))
      return spec_laborer;
   if (!str_cmp(name, "spec_city_messenger"))
      return spec_city_messenger;
   if (!str_cmp(name, "spec_park_keeper"))
      return spec_park_keeper;
   if (!str_cmp(name, "spec_records_keeper"))
      return spec_records_keeper;
   if (!str_cmp(name, "spec_posting_clerk"))
      return spec_posting_clerk;
   if (!str_cmp(name, "spec_harbor_hand"))
      return spec_harbor_hand;
   if (!str_cmp(name, "spec_kiess_shopkeeper"))
      return spec_kiess_shopkeeper;
   if (!str_cmp(name, "spec_kiess_innkeeper"))
      return spec_kiess_innkeeper;
   if (!str_cmp(name, "spec_kiess_scout"))
      return spec_kiess_scout;
   if (!str_cmp(name, "spec_kiess_orator"))
      return spec_kiess_orator;
   if (!str_cmp(name, "spec_kiess_wall_officer"))
      return spec_kiess_wall_officer;
   if (!str_cmp(name, "spec_kowloon_shopkeeper"))
      return spec_kowloon_shopkeeper;
   if (!str_cmp(name, "spec_kowloon_gate_captain"))
      return spec_kowloon_gate_captain;
   if (!str_cmp(name, "spec_kowloon_courier"))
      return spec_kowloon_courier;
   if (!str_cmp(name, "spec_kowloon_innkeeper"))
      return spec_kowloon_innkeeper;
   if (!str_cmp(name, "spec_kowloon_corsair"))
      return spec_kowloon_corsair;
   if (!str_cmp(name, "spec_kowloon_sweeper"))
      return spec_kowloon_sweeper;
   if (!str_cmp(name, "spec_kowloon_magistracy"))
      return spec_kowloon_magistracy;
   if (!str_cmp(name, "spec_kowloon_shrine"))
      return spec_kowloon_shrine;
   if (!str_cmp(name, "spec_kowloon_vendor"))
      return spec_kowloon_vendor;
   if (!str_cmp(name, "spec_kowloon_laborer"))
      return spec_kowloon_laborer;
   if (!str_cmp(name, "spec_gnf_road_warden"))
      return spec_gnf_road_warden;
   if (!str_cmp(name, "spec_gnf_lamplighter"))
      return spec_gnf_lamplighter;
   if (!str_cmp(name, "spec_gnf_peddler"))
      return spec_gnf_peddler;
   if (!str_cmp(name, "spec_gnf_mire_speaker"))
      return spec_gnf_mire_speaker;
   if (!str_cmp(name, "spec_gnf_everkeeper"))
      return spec_gnf_everkeeper;
   if (!str_cmp(name, "spec_gnf_joint_scout"))
      return spec_gnf_joint_scout;
   if (!str_cmp(name, "spec_gnf_toll_collector"))
      return spec_gnf_toll_collector;
   if (!str_cmp(name, "spec_gnf_customs"))
      return spec_gnf_customs;
   if (!str_cmp(name, "spec_gnf_courier"))
      return spec_gnf_courier;
   if (!str_cmp(name, "spec_rr_road_clerk"))
      return spec_rr_road_clerk;
   if (!str_cmp(name, "spec_rr_warden_captain"))
      return spec_rr_warden_captain;
   if (!str_cmp(name, "spec_rr_charter_keeper"))
      return spec_rr_charter_keeper;
   if (!str_cmp(name, "spec_rr_convoy_marshal"))
      return spec_rr_convoy_marshal;
   if (!str_cmp(name, "spec_rr_peddler"))
      return spec_rr_peddler;
   if (!str_cmp(name, "spec_rr_shrine_keeper"))
      return spec_rr_shrine_keeper;
   if (!str_cmp(name, "spec_rr_ferryman"))
      return spec_rr_ferryman;
   if (!str_cmp(name, "spec_rr_camp_cook"))
      return spec_rr_camp_cook;
   if (!str_cmp(name, "spec_rr_pilgrim"))
      return spec_rr_pilgrim;
   if (!str_cmp(name, "spec_rr_ruin_scavenger"))
      return spec_rr_ruin_scavenger;
   if (!str_cmp(name, "spec_pyramid_black_sun_shard"))
      return spec_pyramid_black_sun_shard;
   if (!str_cmp(name, "spec_keep_elemental_captain"))
      return spec_keep_elemental_captain;
   if (!str_cmp(name, "spec_hermit_archon"))
      return spec_hermit_archon;

   return 0;
}

/* MAG Mod */

char *rev_spec_lookup(void *func)
{
   if (func == spec_breath_any)
      return "spec_breath_any";
   if (func == spec_breath_acid)
      return "spec_breath_acid";
   if (func == spec_breath_fire)
      return "spec_breath_fire";
   if (func == spec_breath_frost)
      return "spec_breath_frost";
   if (func == spec_breath_gas)
      return "spec_breath_gas";
   if (func == spec_breath_lightning)
      return "spec_breath_lightning";
   if (func == spec_cast_adept)
      return "spec_cast_adept";
   if (func == spec_cast_cleric)
      return "spec_cast_cleric";
   if (func == spec_cast_judge)
      return "spec_cast_judge";
   if (func == spec_cast_mage)
      return "spec_cast_mage";
   if (func == spec_cast_undead)
      return "spec_cast_undead";
   if (func == spec_executioner)
      return "spec_executioner";
   if (func == spec_fido)
      return "spec_fido";
   if (func == spec_guard)
      return "spec_policeman";
   if (func == spec_janitor)
      return "spec_janitor";
   if (func == spec_mayor)
      return "spec_mayor";
   if (func == spec_poison)
      return "spec_poison";
   if (func == spec_thief)
      return "spec_thief";
   if (func == spec_policeman)
      return "spec_policeman";
   if (func == spec_cast_cadaver)
      return "spec_cast_cadaver";
   if (func == spec_undead)
      return "spec_undead";
   if (func == spec_stephen)
      return "spec_stephen";
   if (func == spec_rewield)
      return "spec_rewield";
   if (func == spec_sylai_priest)
      return "spec_sylai_priest";
   if (func == spec_cast_bigtime)
      return "spec_cast_bigtime";
   if (func == spec_sage)
      return "spec_sage";
   if (func == spec_wizardofoz)
      return "spec_wizardofoz";
   if (func == spec_vamp_hunter)
      return "spec_vamp_hunter";
   if (func == spec_mino_guard)
      return "spec_mino_guard";
   if (func == spec_tax_man)
      return "spec_tax_man";
   if (func == spec_keep_physical_captain)
      return "spec_keep_physical_captain";
   if (func == spec_summon_water)
      return "spec_summon_water";
   if (func == spec_summon_fire)
      return "spec_summon_fire";
   if (func == spec_summon_earth)
      return "spec_summon_earth";
   if (func == spec_summon_undead)
      return "spec_summon_undead";
   if (func == spec_summon_holy)
      return "spec_summon_holy";
   if (func == spec_summon_shadow)
      return "spec_summon_shadow";
   if (func == spec_summon_metal)
      return "spec_summon_metal";
   if (func == spec_summon_animate)
      return "spec_summon_animate";
   if (func == spec_summon_thought)
      return "spec_summon_thought";
   if (func == spec_lamplighter)
      return "spec_lamplighter";
   if (func == spec_warden)
      return "spec_warden";
   if (func == spec_vendor)
      return "spec_vendor";
   if (func == spec_lay_sister)
      return "spec_lay_sister";
   if (func == spec_laborer)
      return "spec_laborer";
   if (func == spec_city_messenger)
      return "spec_city_messenger";
   if (func == spec_park_keeper)
      return "spec_park_keeper";
   if (func == spec_records_keeper)
      return "spec_records_keeper";
   if (func == spec_posting_clerk)
      return "spec_posting_clerk";
   if (func == spec_harbor_hand)
      return "spec_harbor_hand";
   if (func == spec_kiess_shopkeeper)
      return "spec_kiess_shopkeeper";
   if (func == spec_kiess_innkeeper)
      return "spec_kiess_innkeeper";
   if (func == spec_kiess_scout)
      return "spec_kiess_scout";
   if (func == spec_kiess_orator)
      return "spec_kiess_orator";
   if (func == spec_kiess_wall_officer)
      return "spec_kiess_wall_officer";
   if (func == spec_kowloon_shopkeeper)
      return "spec_kowloon_shopkeeper";
   if (func == spec_kowloon_gate_captain)
      return "spec_kowloon_gate_captain";
   if (func == spec_kowloon_courier)
      return "spec_kowloon_courier";
   if (func == spec_kowloon_innkeeper)
      return "spec_kowloon_innkeeper";
   if (func == spec_kowloon_corsair)
      return "spec_kowloon_corsair";
   if (func == spec_kowloon_sweeper)
      return "spec_kowloon_sweeper";
   if (func == spec_kowloon_magistracy)
      return "spec_kowloon_magistracy";
   if (func == spec_kowloon_shrine)
      return "spec_kowloon_shrine";
   if (func == spec_kowloon_vendor)
      return "spec_kowloon_vendor";
   if (func == spec_kowloon_laborer)
      return "spec_kowloon_laborer";
   if (func == spec_gnf_road_warden)
      return "spec_gnf_road_warden";
   if (func == spec_gnf_lamplighter)
      return "spec_gnf_lamplighter";
   if (func == spec_gnf_peddler)
      return "spec_gnf_peddler";
   if (func == spec_gnf_mire_speaker)
      return "spec_gnf_mire_speaker";
   if (func == spec_gnf_everkeeper)
      return "spec_gnf_everkeeper";
   if (func == spec_gnf_joint_scout)
      return "spec_gnf_joint_scout";
   if (func == spec_gnf_toll_collector)
      return "spec_gnf_toll_collector";
   if (func == spec_gnf_customs)
      return "spec_gnf_customs";
   if (func == spec_gnf_courier)
      return "spec_gnf_courier";
   if (func == spec_rr_road_clerk)
      return "spec_rr_road_clerk";
   if (func == spec_rr_warden_captain)
      return "spec_rr_warden_captain";
   if (func == spec_rr_charter_keeper)
      return "spec_rr_charter_keeper";
   if (func == spec_rr_convoy_marshal)
      return "spec_rr_convoy_marshal";
   if (func == spec_rr_peddler)
      return "spec_rr_peddler";
   if (func == spec_rr_shrine_keeper)
      return "spec_rr_shrine_keeper";
   if (func == spec_rr_ferryman)
      return "spec_rr_ferryman";
   if (func == spec_rr_camp_cook)
      return "spec_rr_camp_cook";
   if (func == spec_rr_pilgrim)
      return "spec_rr_pilgrim";
   if (func == spec_rr_ruin_scavenger)
      return "spec_rr_ruin_scavenger";
   if (func == spec_pyramid_black_sun_shard)
      return "spec_pyramid_black_sun_shard";
   if (func == spec_keep_elemental_captain)
      return "spec_keep_elemental_captain";
   if (func == spec_hermit_archon)
      return "spec_hermit_archon";

   return 0;
}

void print_spec_lookup(char *buf)
{
   strcat(buf, "       spec_breath_any         \n\r");
   strcat(buf, "       spec_breath_acid        \n\r");
   strcat(buf, "       spec_breath_fire        \n\r");
   strcat(buf, "       spec_breath_frost       \n\r");
   strcat(buf, "       spec_breath_gas         \n\r");
   strcat(buf, "       spec_breath_lightning   \n\r");
   strcat(buf, "       spec_cast_adept         \n\r");
   strcat(buf, "       spec_cast_cleric        \n\r");
   strcat(buf, "       spec_cast_judge         \n\r");
   strcat(buf, "       spec_cast_mage          \n\r");
   strcat(buf, "       spec_cast_undead        \n\r");
   strcat(buf, "       spec_executioner        \n\r");
   strcat(buf, "       spec_fido               \n\r");
   strcat(buf, "       spec_guard              \n\r");
   strcat(buf, "       spec_janitor            \n\r");
   strcat(buf, "       spec_mayor              \n\r");
   strcat(buf, "       spec_poison             \n\r");
   strcat(buf, "       spec_thief              \n\r");
   strcat(buf, "       spec_policeman          \n\r");
   strcat(buf, "       spec_cast_cadaver       \n\r");
   strcat(buf, "       spec_undead             \n\r");
   strcat(buf, "       spec_rewield            \n\r");
   strcat(buf, "	     spec_cast_bigtime       \n\r");
   strcat(buf, "       spec_wizardofoz         \n\r");
   strcat(buf, "       spec_vamp_hunter (Int mobs only) \n\r");
   strcat(buf, "       spec_mino_guard \n\r");
   strcat(buf, "       spec_tax_man \n\r");
   strcat(buf, "       spec_keep_physical_captain\n\r");
   strcat(buf, "       spec_summon_water        \n\r");
   strcat(buf, "       spec_summon_fire         \n\r");
   strcat(buf, "       spec_summon_earth        \n\r");
   strcat(buf, "       spec_summon_undead       \n\r");
   strcat(buf, "       spec_summon_holy         \n\r");
   strcat(buf, "       spec_summon_shadow       \n\r");
   strcat(buf, "       spec_summon_metal        \n\r");
   strcat(buf, "       spec_summon_animate      \n\r");
   strcat(buf, "       spec_summon_thought      \n\r");
   strcat(buf, "       spec_lamplighter         \n\r");
   strcat(buf, "       spec_warden              \n\r");
   strcat(buf, "       spec_vendor              \n\r");
   strcat(buf, "       spec_lay_sister          \n\r");
   strcat(buf, "       spec_laborer             \n\r");
   strcat(buf, "       spec_kiess_shopkeeper   \n\r");
   strcat(buf, "       spec_kiess_innkeeper    \n\r");
   strcat(buf, "       spec_kiess_scout        \n\r");
   strcat(buf, "       spec_kiess_orator       \n\r");
   strcat(buf, "       spec_kiess_wall_officer \n\r");
   strcat(buf, "       spec_kowloon_shopkeeper \n\r");
   strcat(buf, "       spec_kowloon_gate_captain\n\r");
   strcat(buf, "       spec_kowloon_courier     \n\r");
   strcat(buf, "       spec_kowloon_innkeeper   \n\r");
   strcat(buf, "       spec_kowloon_corsair     \n\r");
   strcat(buf, "       spec_kowloon_sweeper    \n\r");
   strcat(buf, "       spec_kowloon_magistracy \n\r");
   strcat(buf, "       spec_kowloon_shrine     \n\r");
   strcat(buf, "       spec_kowloon_vendor     \n\r");
   strcat(buf, "       spec_kowloon_laborer    \n\r");
   strcat(buf, "       spec_gnf_road_warden   \n\r");
   strcat(buf, "       spec_gnf_lamplighter   \n\r");
   strcat(buf, "       spec_gnf_peddler       \n\r");
   strcat(buf, "       spec_gnf_mire_speaker  \n\r");
   strcat(buf, "       spec_gnf_everkeeper    \n\r");
   strcat(buf, "       spec_gnf_joint_scout   \n\r");
   strcat(buf, "       spec_gnf_toll_collector\n\r");
   strcat(buf, "       spec_gnf_customs       \n\r");
   strcat(buf, "       spec_gnf_courier       \n\r");
   strcat(buf, "       spec_rr_road_clerk     \n\r");
   strcat(buf, "       spec_rr_warden_captain \n\r");
   strcat(buf, "       spec_rr_charter_keeper \n\r");
   strcat(buf, "       spec_rr_convoy_marshal \n\r");
   strcat(buf, "       spec_rr_peddler        \n\r");
   strcat(buf, "       spec_rr_shrine_keeper  \n\r");
   strcat(buf, "       spec_rr_ferryman       \n\r");
   strcat(buf, "       spec_rr_camp_cook      \n\r");
   strcat(buf, "       spec_rr_pilgrim        \n\r");
   strcat(buf, "       spec_rr_ruin_scavenger \n\r");
   strcat(buf, "       spec_pyramid_black_sun_shard\n\r");
   strcat(buf, "       spec_keep_elemental_captain\n\r");
   strcat(buf, "       spec_hermit_archon\n\r");

   return;
}

/*
 * spec_handle_damage — called from do_damage() whenever an NPC with a
 * spec_fun takes a hit.  Dispatches to the per-spec damage callback so
 * each special can react to incoming element damage without coupling
 * the core damage code to individual boss implementations.
 */
void spec_handle_damage(CHAR_DATA *victim, int element, int dam)
{
   if (victim->spec_fun == spec_keep_elemental_captain)
      spec_keep_elemental_captain_damage(victim, element, dam);
   if (victim->spec_fun == spec_hermit_archon)
      spec_hermit_archon_damage(victim, element, dam);
}

/*
 * spec_superboss_index — return the superboss tracking index for a mob,
 * or -1 if the mob is not a superboss.
 */
int spec_superboss_index(CHAR_DATA *mob)
{
   if (mob->spec_fun == spec_pyramid_black_sun_shard)
      return SUPERBOSS_PYRAMID_BLACK_SUN_SHARD;
   if (mob->spec_fun == spec_keep_elemental_captain)
      return SUPERBOSS_KEEP_ELEMENTAL_CAPTAIN;
   if (mob->spec_fun == spec_keep_physical_captain)
      return SUPERBOSS_KEEP_PHYSICAL_CAPTAIN;
   if (mob->spec_fun == spec_hermit_archon)
      return SUPERBOSS_HERMIT_ARCHON;
   return -1;
}

/*
 * spec_death_handler — called from group_gain() when an NPC dies.
 * Checks if the dead mob is a superboss and awards a reincarnation
 * point to each group member who has not killed this superboss before.
 */
void spec_death_handler(CHAR_DATA *victim, CHAR_DATA *killer)
{
   int sb_index;
   CHAR_DATA *gch;
   char buf[MAX_STRING_LENGTH];

   if (victim == NULL || victim->spec_fun == NULL)
      return;

   sb_index = spec_superboss_index(victim);
   if (sb_index < 0)
      return;

   for (gch = victim->in_room->first_person; gch != NULL; gch = gch->next_in_room)
   {
      if (IS_NPC(gch) || gch->pcdata == NULL)
         continue;

      if (!is_same_group(gch, killer))
         continue;

      if (gch->pcdata->superboss_kills[sb_index])
         continue;

      gch->pcdata->superboss_kills[sb_index] = TRUE;
      gch->pcdata->reincarnation_data[REINCARNATION_POINTS]++;

      sprintf(buf,
         "@@a*** You have slain @@e%s@@a for the first time! ***\n\r"
         "@@a*** You have been awarded @@W1 reincarnation point@@a! ***@@N\n\r",
         victim->short_descr);
      send_to_char(buf, gch);
   }
}

/*
 * Core procedure for dragons.
 */
bool dragon(CHAR_DATA *ch, char *spell_name)
{
   CHAR_DATA *victim;
   int sn;

   if (!is_fighting(ch))
      return FALSE;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
      if (victim->fighting == ch && number_bits(2) == 0)
         break;

   if (victim == NULL)
      return FALSE;

   if ((sn = skill_lookup(spell_name)) < 0)
      return FALSE;
   (*skill_table[sn].spell_fun)(sn, ch->level, ch, victim, NULL);
   return TRUE;
}

bool is_player_summon_special(SPEC_FUN *spec_fun)
{
   return spec_fun == spec_summon_water
      || spec_fun == spec_summon_fire
      || spec_fun == spec_summon_earth
      || spec_fun == spec_summon_undead
      || spec_fun == spec_summon_holy
      || spec_fun == spec_summon_shadow
      || spec_fun == spec_summon_metal
      || spec_fun == spec_summon_animate
      || spec_fun == spec_summon_thought;
}

bool spec_summon_cast_random(CHAR_DATA *ch, CHAR_DATA *target, const char *const *spells, int spell_count)
{
   int i;
   int start;

   if (!is_fighting(ch) || target == NULL || spell_count <= 0)
      return FALSE;

   start = number_range(0, spell_count - 1);

   for (i = 0; i < spell_count; i++)
   {
      int index = (start + i) % spell_count;
      int sn = skill_lookup(spells[index]);

      if (sn < 0)
         continue;

      (*skill_table[sn].spell_fun)(sn, ch->level, ch, target, NULL);
      return TRUE;
   }

   return FALSE;
}

int summon_master_heal_chance(int master_hit, int master_max_hp, int thematic_bonus)
{
   int master_hp_pct;
   int chance_to_heal;

   if (master_max_hp <= 0 || master_hit >= master_max_hp)
      return 0;

   master_hp_pct = UMAX(0, (master_hit * 100) / master_max_hp);
   chance_to_heal = thematic_bonus + (100 - master_hp_pct);

   return URANGE(5, chance_to_heal, 95);
}

bool spec_summon_heal_master(CHAR_DATA *ch, int thematic_bonus)
{
   int sn;
   int max_hp;
   int chance_to_heal;

   if (ch->master == NULL || ch->master->in_room != ch->in_room)
      return FALSE;

   max_hp = get_max_hp(ch->master);
   if (max_hp <= 0)
      return FALSE;

   if (ch->master->hit >= max_hp)
      return FALSE;

   chance_to_heal = summon_master_heal_chance(ch->master->hit, max_hp, thematic_bonus);

   if (chance_to_heal <= 0 || number_range(1, 100) > chance_to_heal)
      return FALSE;

   sn = skill_lookup("heal");
   if (sn < 0)
      return FALSE;

   (*skill_table[sn].spell_fun)(sn, ch->level, ch, ch->master, NULL);
   return TRUE;
}
