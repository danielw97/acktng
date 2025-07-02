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

/*
 * This file should contain:
 *	o advanced damage engine
 */

struct sp_dam_str_type sp_dam_str[] = {
    /* NONE      */ {ELEMENT_NONE, "@@g", "@@g", "@@W", "", "%s%s%s",
                     "blast", "BLAST", "blasts", "BLASTS"},

    /* Fire      */ {ELEMENT_FIRE, "@@R", "@@W", "@@y", "@@2", "%s<%s^%s>", "burn", "INCINERATE", "burns", "INCINERATES"},
    /* gas       */ {ELEMENT_AIR, "@@W", "@@y", "@@c", "@@7", "%s(%sO%s)", "choke", "ENVELOP", "chokes", "ENVELOPS"},
    /* poison    */ {ELEMENT_POISON, "@@e", "@@r", "@@e", "@@4", "%s{%s^%s}", "weaken", "POISON", "weakens", "POISONS"},
    /* cold      */ {ELEMENT_WATER, "@@l", "@@W", "@@y", "@@1", "%s[%s~%s]", "chill", "FREEZE", "chills", "FREEZES"},
    /* drain     */ {ELEMENT_SHADOW, "@@d", "@@R@@i", "@@2@@e", "@@R@@i", "%s>%s-%s<", "taint", "DRAIN", "taints", "DRAINS"},
    /* impact    */ {ELEMENT_EARTH, "@@b", "@@W", "@@y", "@@4", "%s.%s^%s.", "strike", "SLAM", "strikes", "SLAMS"},
    /* mind      */ {ELEMENT_MENTAL, "@@m", "@@W", "@@p", "@@5", "%s/%sV%s\\", "zap", "BLAST", "zaps", "BLASTS"},
    /* holy      */ {ELEMENT_HOLY, "@@l", "@@W", "@@y", "@@1", "%s~%s\\/%s~", "holy", "HOLY", "holies", "HOLIES"}};

CHAR_DATA *player_summon(CHAR_DATA *ch, int level, int element)
{
   CHAR_DATA *summoned;
   char name[MAX_STRING_LENGTH], short_desc[MAX_STRING_LENGTH], long_desc[MAX_STRING_LENGTH];
   int base_penalty;

   if (element == REALM_WATER)
   {
      strcpy(name, "Water Elemental");
      strcpy(short_desc, "@@lW@@Bater @@lE@@Blemental@@N");
      strcpy(long_desc, "A @@lW@@Bater @@lE@@Blemental@@N surfs here.\n\r");
      base_penalty = 40;
   }
   else if (element == REALM_FIRE)
   {
      strcpy(name, "Fire Elemental");
      strcpy(short_desc, "@@eF@@Rire @@eE@@Rlemental@@N");
      strcpy(long_desc, "A @@eF@@Rire @@eE@@Rlemental@@N burns here.\n\r");
      base_penalty = 40;
   }
   else if (element == REALM_EARTH)
   {
      strcpy(name, "Earth Elemental");
      strcpy(short_desc, "@@yE@@barth @@yE@@blemental@@N");
      strcpy(long_desc, "A @@yE@@barth @@yE@@blemental@@N rumbles here.\n\r");
      base_penalty = 35;
   }
   else if (element == ELEMENT_SHADOW)
   {
      strcpy(name, "Skeleton");
      strcpy(short_desc, "A @@dSkeleton@@N");
      strcpy(long_desc, "A @@dSkeleton@@N goes 'Myaah!' here.\n\r");
      base_penalty = 50;
   }
   else if (element == REALM_HOLY)
   {
      strcpy(name, "Holy Avenger");
      strcpy(short_desc, "@@cHoly @@WAvenger@@N");
      strcpy(long_desc, "A majestic @@cHoly @@WAvenger@@N stands before you.\n\r");
      base_penalty = 40;
   }
   else if (element == ELEMENT_AIR)
   {
      strcpy(name, "Soul Thief");
      strcpy(short_desc, "@@dSoul @@BThief@@N");
      strcpy(long_desc, "A @@dSoul @@BThief@@N skulks about here.\n\r");
      base_penalty = 35;
   }
   else if (element == ELEMENT_PHYSICAL)
   {
      strcpy(name, "Iron Golem");
      strcpy(short_desc, "@@dIron @@WGolem@@N");
      strcpy(long_desc, "@@NA towering mass of @@dmetal@@N peers into your soul.\n\r");
      base_penalty = 25;
   }
   else if (element == ELEMENT_MENTAL)
   {
      strcpy(name, "Diamond Golem");
      strcpy(short_desc, "@@WD@@yi@@Wa@@ym@@Wo@@yn@@Wd @@WGolem@@N");
      strcpy(long_desc, "@@yA shimmering tower of @@cdiamond@@y glitters before you.\n\r");
      base_penalty = 15;
   }

   summoned = create_mobile(get_mob_index(MOB_VNUM_WATERELEM));
   char_to_room(summoned, ch->in_room);

   summoned->level = level - base_penalty;
   summoned->max_hit = summoned->level * 15 + number_range(summoned->level * summoned->level / 2, summoned->level * summoned->level / 1);
   summoned->hit = summoned->max_hit;

   summoned->exp = 0;
   summoned->intell_exp = 0;

   summoned->max_mana = summoned->level * 25;
   summoned->mana = summoned->max_mana;

   summoned->name = str_dup(name);
   summoned->short_descr = str_dup(short_desc);
   summoned->long_descr = str_dup(long_desc);

   SET_BIT(summoned->act, ACT_PET);
   SET_BIT(summoned->affected_by, AFF_CHARM);
   summoned->extract_timer = get_psuedo_level(ch) / 4;
   add_follower(summoned, ch);

   return summoned;
}

int class_heal_character(CHAR_DATA *ch, CHAR_DATA *victim, int base_heal, int sn, int class_index, bool hot)
{
   int heal = base_heal * 2;

   if (class_table[class_index].attr_prime == APPLY_INT)
   {
      int intel = (get_curr_int(ch) - 13) * 5;

      heal += heal * intel / 100;

      if (sn != spell_psionic_recovery)
      {
         heal += heal * ch->lvl[CLASS_MAG] / 50;
         heal += heal * ch->remort[CLASS_SOR] / 50;
         heal += heal * ch->remort[CLASS_WIZ] / 50;
      }
      else
      {
         heal += heal * ch->lvl[CLASS_PSI] / 100;
         heal += heal * ch->remort[CLASS_EGO] / 50;
      }
   }
   else if (class_table[class_index].attr_prime == APPLY_WIS)
   {
      int wis = (get_curr_wis(ch) - 13) * 5;

      heal += heal * wis / 100;
      heal += heal * ch->lvl[CLASS_CLE] / 50;
   }

   heal += heal * ch->remort[CLASS_PRI] / 50;
   heal += heal * ch->remort[CLASS_PAL] / 50 * 0.75;
   heal += heal * ch->adept[CLASS_TEM] / 25;

   if (stance_app[ch->stance].heal_mod != 0)
      heal += heal * stance_app[ch->stance].heal_mod / 10;

   if (IS_NPC(ch))
   {
      heal += heal * ch->level / 100;
      if (ch->level > 100)
         heal += heal * (ch->level - 100) / 50;
      if (ch->level > 150)
         heal += heal * (ch->level - 150) / 50;
   }

   if (hot)
      heal += get_spellpower(ch) / 4;
   else
      heal += get_spellpower(ch);

   return heal;
}

void heal_character(CHAR_DATA *ch, CHAR_DATA *victim, int base_heal, int sn, bool hot)
{
   char buf1[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char buf3[MAX_STRING_LENGTH];
   int heal;

   heal = base_heal;

   if (!hot)
      heal = heal * number_range(75, 125) / 100;

   victim->hit = UMIN(victim->hit + heal, victim->max_hit);
   update_pos(victim);

   sprintf(buf1, "@@N$n's %s heals $N! (@@r%d@@N)", skill_table[sn].name, heal);
   sprintf(buf2, "@@NYour %s heals $N! (@@r%d@@N)\n\r", skill_table[sn].name, heal);
   sprintf(buf3, "@@N$n's %s heals you! (@@r%d@@N)\n\r", skill_table[sn].name, heal);

   if (ch != victim)
      act(buf3, ch, NULL, victim, TO_VICT);
   if (ch != NULL)
      act(buf2, ch, NULL, victim, TO_CHAR);
   act(buf1, ch, NULL, victim, TO_NOTVICT);
}

void sp_death_message(CHAR_DATA *ch, CHAR_DATA *victim, int realm)
{
   /*
    * Used to display assorted death messages, based on dt
    * * max_dt == number of entries in attack table in dam_message,
    * * so we know if kill was unarmed, armed, or through spells or skills.
    * * Stephen
    */

   char buf1[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char buf3[MAX_STRING_LENGTH];
   int vnum;
   int looper;
   /*
    * For debugging purposes
    * sprintf( buf, "dt: %d max_dt: %d\n\r", dt, max_dt );
    * notify( buf, 1 );
    */

   vnum = 0; /* Used to load up body parts.... eewwwwww! */

   for (looper = 1; looper < 32; looper++)
   {
      if (IS_SET(realm, (1 << looper)))
         break;
   }
   switch (looper + 1)
   {
   default: /* bad realm, use non-specific */
      sprintf(buf1, "$n destroy $N!");
      sprintf(buf2, "You destroy $N!");
      sprintf(buf3, "$n destroys you.");
      break;
   case ELEMENT_FIRE:
   {
      if (HAS_BODY(victim))
      {
         switch (number_range(0, 4))
         {
         case 0:
            sprintf(buf1, "$n cremates $N's body to a crisp!");
            sprintf(buf2, "You cremate $N's body to a crispy @@eRED@@N cinder!");
            sprintf(buf3, "$n cremates your remains.");
            break;
         case 1:
            sprintf(buf1, "$n fries $N's guts; smoke envelops $S corpse!");
            sprintf(buf2, "You fry $N's guts; smoke bites your eyes!");
            sprintf(buf3, "$n slowly fries your entrails; smoke covers your remains!");
            break;
         case 2:
            sprintf(buf1, "$n leaves gigantic burns on $N's remains. Blood and pus spray out wildly!");
            sprintf(buf2, "You leave gigantic burns on $N's remains. Blood and pus spray out wildly!");
            sprintf(buf3, "$n leaves gigantic burns on your body. Blood and pus spray out wildly!");
            break;
         case 3:
            sprintf(buf1, "$n burns $N's skin off, leaving a bloody mess!");
            sprintf(buf2, "You burn $N's skin off, leaving a bloody mess!");
            sprintf(buf3, "$n burns your skin off, leaving a bloody mess!");
            break;
         case 4:
            sprintf(buf1, "$n leaves $N burning in flames. Blood spills all over the floor!");
            sprintf(buf2, "You leave $N burning in flames. Blood spills all over the floor!");
            sprintf(buf3, "$n leaves you burning in flames. Your skin breaks and blood spills on the floor!");
            break;
         }
      }
      else
      {
         sprintf(buf1, "$n sets $N aura on fire!");
         sprintf(buf2, "You set $N on fire!");
         sprintf(buf3, "$n sets you on fire!");
         break;
      }
      break;
   }
   case ELEMENT_POISON:
   {
      if (HAS_BODY(victim))
      {
         switch (number_range(0, 1))
         {
         case 0:
            sprintf(buf1, "$n poisons $N ;$S body begins to twist in agony!");
            sprintf(buf2, "You poison $N and watch $S body begin twisting in agony!");
            sprintf(buf3, "$n poisons you -- you twist in agony.");
            break;

         case 1:
            sprintf(buf1, "$n sends a poisonous cloud into $N ;$S mouth starts dripping with blood!");
            sprintf(buf2, "You send a poisonous cloud into $N ;$S mouth starts dripping with blood!");
            sprintf(buf3, "$n bursts your lungs with his poisonous cloud, blood begins flowing from inside.");
            break;
         }
      }
      else
      {
         sprintf(buf1, "$n poisons $N ;$S soul begins to twist in agony!");
         sprintf(buf2, "You poison $N and watch $S soul begin twisting in agony!");
         sprintf(buf3, "$n poisons you -- you twist in agony.");
         break;
      }
      break;
   }
   case ELEMENT_WATER:
   {
      if (HAS_BODY(victim))
      {
         switch (number_range(0, 0))
         {
         case 0:
            sprintf(buf1, "$n turns $N into a block of ice.");
            sprintf(buf2, "You turn $N into a block of ice.");
            sprintf(buf3, "$n turns you into ice!!");
            break;
         }
      }
      else
      {
         sprintf(buf1, "$n freezes $N to death!");
         sprintf(buf2, "You freeze $N to death!");
         sprintf(buf3, "$n freezes you to death!");
         break;
      }
      break;
   }
   case ELEMENT_SHADOW:
   {
      if (HAS_BODY(victim))
      {
         sprintf(buf1, "$n drains $N of $S life energy.");
         sprintf(buf2, "You drains $N of $S life energy.");
         sprintf(buf3, "$n drains you of your life energy.");
         break;
      }
      else
      {
         sprintf(buf1, "$n darkens $N soul!");
         sprintf(buf2, "You darken $N soul!");
         sprintf(buf3, "$n darkens your soul!");
         break;
      }
      break;
   }
   case ELEMENT_PHYSICAL:
   {
      if (HAS_BODY(victim))
      {
         sprintf(buf1, "$n smashes $N's body into messy flesh.");
         sprintf(buf2, "You smash $N into messy flesh.");
         sprintf(buf3, "$n smashes you into messy flesh!");
         break;
      }
      else
      {
         sprintf(buf1, "$n crushes $N to death!");
         sprintf(buf2, "You crush $N to death!");
         sprintf(buf3, "$n crushes you to death!");
         break;
      }
      break;
   }
   case ELEMENT_MENTAL:
   {
      if (HAS_MIND(victim))
      {
         sprintf(buf1, "$n blasts $N with psionic power!");
         sprintf(buf2, "You blast $N with psionic power!");
         sprintf(buf3, "$n blasts you with psionic power!");
         break;
      }
      else
      {
         sprintf(buf1, "$n blasts $N with psionic power!");
         sprintf(buf2, "You blast $N with psionic power!");
         sprintf(buf3, "$n blasts you with psionic power!");
         break;
      }
      break;
   }
   case ELEMENT_HOLY:
   {
      if (HAS_MIND(victim))
      {
         sprintf(buf1, "$n steals $N's soul with divine power!");
         sprintf(buf2, "You steal $N's soul with divine power!");
         sprintf(buf3, "$n steals your soul with divine power!");
         break;
      }
      else
      {
         sprintf(buf1, "$n steals $N's soul with divine power!");
         sprintf(buf2, "You steal $N's soul with divine power!");
         sprintf(buf3, "$n steals your soul with divine power!");
         break;
      }
      break;
   }
   } /* end switch */

   act(buf1, ch, NULL, victim, TO_NOTVICT);
   act(buf2, ch, NULL, victim, TO_CHAR);
   act(buf3, ch, NULL, victim, TO_VICT);

   /*
    * Load up object, if needed.
    */
   if (vnum != 0)
   {
      char buf[MAX_STRING_LENGTH];
      OBJ_DATA *obj;
      char *name;

      name = IS_NPC(ch) ? ch->short_descr : ch->name;
      obj = create_object(get_obj_index(vnum), 0);
      obj->timer = number_range(4, 7);

      sprintf(buf, obj->short_descr, name);
      free_string(obj->short_descr);
      obj->short_descr = str_dup(buf);

      sprintf(buf, obj->description, name);
      free_string(obj->description);
      obj->description = str_dup(buf);

      obj_to_room(obj, ch->in_room);
   }

   return;
}

/*
 * assumes a base damage, after initial save checks.
 * will modify damage as it sees fit for weaknesses and strengths.
 *
 */
void sp_dam_message(OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int dam, int realm, int dt, bool critical)
{

   char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];
   const char *attack;
   char punct;
   int rtype, dam_range;
   bool dead = FALSE;
   char symbuf[MSL];
   char catsymbuf[MSL];
   char outercol[MSL];
   char innercol[MSL];
   bool stress = FALSE;

   if (dam >= victim->hit)
      dead = TRUE;

   if (critical)
      sprintf(testerbuf, " @@l(@@e%i@@l)@@N (@@rCRITICAL@@N) ", dam);
   else
      sprintf(testerbuf, " @@l(@@e%i@@l)@@N", dam);

   for (rtype = 0; rtype < MAX_REALM; rtype++)
   {
      if (sp_dam_str[rtype].realm == realm)
         break;
   }
   if (rtype == MAX_REALM)
      rtype = 0;

   /* notes:
     2000+ stress color
     1000+ normal color
     0+ no backcolor
     500+ stress verb
     0+ normal verb
     each 50 is a symbol
   */

   if (dam > 2000)
   {
      dam_range = dam - 2000;
   }
   else if (dam > 1000)
   {
      dam_range = dam - 1000;
   }
   else
   {
      dam_range = dam;
   }
   dam_range /= 2;
   if (dam_range > 250)
   {
      stress = TRUE;
      dam_range -= 250;
   }
   punct = (!stress) ? '.' : '!';
   sprintf(outercol, "%s%s",
           (dam > 250 ? sp_dam_str[rtype].backcol : ""),
           (dam > 250 ? sp_dam_str[rtype].invertcol : sp_dam_str[rtype].basecol));
   sprintf(innercol, "%s%s",
           (dam > 250 ? sp_dam_str[rtype].backcol : ""),
           (dam > 500 ? sp_dam_str[rtype].stresscol : (dam > 250 ? sp_dam_str[rtype].invertcol : sp_dam_str[rtype].basecol)));

   catsymbuf[0] = '\0';
   sprintf(symbuf, "%s", " ");
   for (; dam_range > 0; dam_range -= 50)
   {
      sprintf(catsymbuf, sp_dam_str[rtype].formatter, outercol, innercol, outercol);
      safe_strcat(MSL, symbuf, catsymbuf);
   }
   safe_strcat(MSL, symbuf, " ");

   if (dt >= 0 && dt < MAX_SKILL)
      attack = skill_table[dt].noun_damage;
   else
   {
      bug("Dam_message: bad dt %d.", dt);
      rtype = 0;
      attack = "!!!!!";
   }
   if (obj == NULL)
   {
      sprintf(buf1, "%s$n %s%s%s@@N $N %swith $s %s%c@@N%s",
              sp_dam_str[rtype].basecol,
              symbuf,
              (stress ? sp_dam_str[rtype].vp_stress : sp_dam_str[rtype].vp),
              symbuf, sp_dam_str[rtype].basecol, attack, punct, testerbuf);
      sprintf(buf2, "%sYou %s%s%s@@N $N %swith your %s%c@@N%s",
              sp_dam_str[rtype].basecol,
              symbuf,
              (stress ? sp_dam_str[rtype].vs_stress : sp_dam_str[rtype].vs),
              symbuf, sp_dam_str[rtype].basecol, attack, punct, testerbuf);
      /*	   if ( *str == '\'' )
             sprintf( buf3, "%s$n %s%s your%s%s $s %s%c@@N",col,col, vp,col, str+2, attack, punct);
            else
      */
      sprintf(buf3, "%s$n %s%s%s %syou with $s %s%c@@N%s",
              sp_dam_str[rtype].basecol,
              symbuf,
              (stress ? sp_dam_str[rtype].vp_stress : sp_dam_str[rtype].vp),
              symbuf, sp_dam_str[rtype].basecol, attack, punct, testerbuf);
   }
   else if (obj != NULL)
   {
      sprintf(buf1, "%s$p %s%s%s@@N $N %swith a %s%c@@N",
              sp_dam_str[rtype].basecol,
              symbuf,
              (stress ? sp_dam_str[rtype].vp_stress : sp_dam_str[rtype].vp),
              symbuf, sp_dam_str[rtype].basecol, attack, punct);
#if 0
      sprintf( buf2, "%sYou %s%s%s@@N $N %swith your %s%c@@N%s",
               sp_dam_str[rtype].basecol,
               symbuf,
               ( stress ? sp_dam_str[rtype].vs_stress : sp_dam_str[rtype].vs ),
               symbuf, sp_dam_str[rtype].basecol, attack, punct, testerbuf );
#endif
      /*	   if ( *str == '\'' )
             sprintf( buf3, "%s$n %s%s your%s%s $s %s%c@@N",col,col, vp,col, str+2, attack, punct);
            else
      */
      sprintf(buf3, "%s$p %s%s%s %syou with $s %s%c@@N",
              sp_dam_str[rtype].basecol,
              symbuf,
              (stress ? sp_dam_str[rtype].vp_stress : sp_dam_str[rtype].vp),
              symbuf, sp_dam_str[rtype].basecol, attack, punct);
   }
   else
   {
      sprintf(log_buf, "%s", "Error in casting spell , sp_dam called with NULL obj and ch.");
      monitor_chan(log_buf, MONITOR_DEBUG);
      return;
   }
   if (obj == NULL)
   {
      act(buf1, ch, NULL, victim, TO_NOTVICT);
   }
   else
   {
      act(buf1, NULL, (void *)obj, victim, TO_NOTVICT);
   }
   if (!IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_BLIND_PLAYER))
   {
      if (dam < victim->max_hit / 30)
         act("You glance $K", ch, NULL, victim, TO_CHAR);
      else if (dam < victim->max_hit / 20)
         act("You hit $K", ch, NULL, victim, TO_CHAR);
      else if (dam < victim->max_hit / 10)
         act("You nail $K", ch, NULL, victim, TO_CHAR);
      else
         act("You thwack $K", ch, NULL, victim, TO_CHAR);
   }
   else if (obj == NULL)
   {
      act(buf2, ch, NULL, victim, TO_CHAR);
   }
   if (!IS_NPC(victim) && IS_SET(victim->pcdata->pflags, PFLAG_BLIND_PLAYER))
   {
      if (dam < victim->max_hit / 30)
         act("$k glances you", ch, NULL, victim, TO_VICT);
      else if (dam < victim->max_hit / 20)
         act("$k hits you", ch, NULL, victim, TO_VICT);
      else if (dam < victim->max_hit / 10)
         act("$k nails you", ch, NULL, victim, TO_VICT);
      else
         act("$k thwacks you", ch, NULL, victim, TO_VICT);
   }
   else if (obj == NULL)
   {
      act(buf3, ch, NULL, victim, TO_VICT);
   }
   else
   {
      act(buf3, NULL, (void *)obj, victim, TO_VICT);
   }
   if (dead)
   {
      sp_death_message(ch, victim, realm);
   }
   return;
}

bool sp_damage(OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *victim, int dam, int type, int sn, bool show_msg)
{
   calculate_damage(ch, victim, dam, sn, type, TRUE);
}
