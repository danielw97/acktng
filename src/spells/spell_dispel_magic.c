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
#include "tables.h"
#include "magic.h"

bool spell_dispel_magic(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   /*
    * Remove certain affects from victim.  Chance will be 100% if
    * * ch == victim.  Otherwise, variable chance of success, depending
    * * on if a) victim is NPC b) Difference in levels.
    * * Rewritten yet AGAIN to check chance for each dispel... also works
    * * on objects as well :)
    * * Stephen
    */
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   CHAR_DATA *victim;
   AFFECT_DATA *paf;
   AFFECT_DATA *paf_next;
   int chance;

   if (target_name[0] == '\0' && !is_fighting(ch))
   {
      send_to_char("Dispel who or what??\n\r", ch);
      return FALSE;
   }
   if (target_name[0] == '\0' && is_fighting(ch))
      target_name = "enemy";

   if ((victim = get_char_room(ch, target_name)) != NULL)
   {
      if (IS_NPC(victim))
         chance = 100;
      else
         chance = 75;

      if (ch == victim)
         chance = 100;

      chance += get_psuedo_level(ch) - get_psuedo_level(victim);
      /*
       * Bonus/penalty for difference in levels.
       */
      if (obj == NULL)
      {
         if (ch == victim)
         {
            send_to_char("You gesture towards yourself.\n\r", ch);
            act("$n gestures towards $mself.", ch, NULL, NULL, TO_ROOM);
         }
         else
         {
            act("You gesture towards $N.", ch, NULL, victim, TO_CHAR);
            act("$n gestures towards $N.", ch, NULL, victim, TO_NOTVICT);
            act("$N gestures towards you.", victim, NULL, ch, TO_CHAR);
         }
      }
      else
      {
         act("$p glows brightly at $n.", victim, obj, NULL, TO_ROOM);
         act("$p glows brightly towards you.", victim, obj, NULL, TO_CHAR);
      }

      int cloak_chance = 40;

      cloak_chance += (get_psuedo_level(ch) - get_psuedo_level(victim));

      if (IS_NPC(ch) && IS_SET(ch->act, ACT_SOLO))
         cloak_chance += 15;
      if (IS_NPC(ch) && IS_SET(ch->act, ACT_BOSS))
         cloak_chance += 25;

      if ((IS_AFFECTED(victim, AFF_CLOAK_REFLECTION)) && (ch != victim) &&
          (number_percent() < cloak_chance))
      {

         act("@@N$n's @@lc@@el@@ro@@ya@@ak@@N glows brightly as $N's spell hits it@@N!!", victim,
             NULL, ch, TO_ROOM);
         act("@@N$N's @@lc@@el@@ro@@ya@@ak@@N glows brightly@@N!!", ch, NULL, victim, TO_CHAR);
         act("@@NYour @@lc@@el@@ro@@ya@@ak@@N glows brightly@@N!!!", victim, NULL, ch, TO_CHAR);
         CREF(vch_next, CHAR_NEXTROOM);
         for (vch = ch->in_room->first_person; vch; vch = vch_next)
         {
            vch_next = vch->next_in_room;
            if (victim == vch && victim->fighting == NULL)
            {
               check_killer(ch, victim);
               multi_hit(victim, ch, TYPE_UNDEFINED);
               break;
            }
         }
         char_unreference(&vch_next);
         return TRUE;
      }

      if ((IS_AFFECTED(victim, AFF_CLOAK_ABSORPTION)) && (ch != victim) &&
          (number_percent() < cloak_chance))
      {

         act("@@N$n's @@lcloak@@N glows brightly as $N's spell hits it, then fades@@N!!", victim,
             NULL, ch, TO_ROOM);
         act("@@N$N's @@lcloak@@N glows brightly, and absorbs your spell@@N!!", ch, NULL, victim,
             TO_CHAR);
         act("@@NYour @@lcloak@@N glows brightly, and absorbs $N's spell@@N!!!", victim, NULL, ch,
             TO_CHAR);
         CREF(vch_next, CHAR_NEXTROOM);

         for (vch = ch->in_room->first_person; vch; vch = vch_next)
         {
            vch_next = vch->next_in_room;
            if (victim == vch && victim->fighting == NULL)
            {
               check_killer(ch, victim);
               multi_hit(victim, ch, TYPE_UNDEFINED);
               break;
            }
         }
         char_unreference(&vch_next);

         return TRUE;
      }

      for (paf = victim->first_affect; paf != NULL; paf = paf_next)
      {
         paf_next = paf->next;
         if ((obj) || (victim == ch->fighting))
         {
            switch (paf->bitvector)
            {
            case AFF_BLIND:
            case AFF_FAERIE_FIRE:
            case AFF_CURSE:
            case AFF_POISON:
            case AFF_SLEEP:
            case AFF_CHARM:
            case AFF_HOLD:
            case AFF_PARALYSIS:
               continue;
               break;
            default:
               break;
            }
         }
         if (number_percent() < chance)
         {
            affect_remove(victim, paf);
            chance = (2 * chance) / 3;
         }
         else
            break;
      }

      if (IS_NPC(victim))
      {
         if (IS_AFFECTED(victim, AFF_CLOAK_ABSORPTION) && (number_percent() < chance))
         {
            chance = (chance) / 3;
            act("@@NThe @@lcloak@@N around $n is ripped to shreds!", victim, NULL, victim, TO_ROOM);
            send_to_char("@@NYour @@lcloak@@N is ripped from your body!\n\r", victim);
            REMOVE_BIT(victim->affected_by, AFF_CLOAK_ABSORPTION);
         }

         if (IS_AFFECTED(victim, AFF_CLOAK_REFLECTION) && (number_percent() < chance))
         {
            chance = (chance) / 3;
            act("@@NThe @@lc@@el@@ro@@ya@@ak@@N around $n is ripped to shreds!", victim, NULL, NULL,
                TO_ROOM);
            send_to_char("@@NYour @@lc@@el@@ro@@ya@@ak@@N is ripped from your body!\n\r", victim);
            REMOVE_BIT(victim->affected_by, AFF_CLOAK_REFLECTION);
         }

         if (IS_AFFECTED(victim, AFF_SANCTUARY) && (number_percent() < chance))
         {
            chance = (2 * chance) / 3;
            act("The white aura around $n fades.", victim, NULL, NULL, TO_ROOM);
            send_to_char("The white aura around you fades.\n\r", victim);
            REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
         }
         if (IS_AFFECTED(victim, AFF_FLYING) && (number_percent() < chance))
         {
            act("$n suddenly drops to the ground!", victim, NULL, NULL, TO_ROOM);
            send_to_char("You suddenly drop to the ground!\n\r", victim);
            REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
         }
         if (IS_AFFECTED(victim, AFF_INVISIBLE) && (number_percent() < chance))
         {
            act("$n flickers, and becomes visible.", victim, NULL, NULL, TO_ROOM);
            send_to_char("You flicker, and become visible.\n\r", victim);
            REMOVE_BIT(victim->affected_by, AFF_INVISIBLE);
         }
         if (IS_AFFECTED(victim, AFF_FAERIE_FIRE) && (number_percent() < chance))
         {
            act("The pink glow around $n suddenly fades.", victim, NULL, NULL, TO_ROOM);
            send_to_char("The pink glow around you suddenly fades.\n\r", ch);
            REMOVE_BIT(victim->affected_by, AFF_FAERIE_FIRE);
         }
         if (IS_AFFECTED(victim, AFF_DETECT_INVIS) && (number_percent() < chance))
            REMOVE_BIT(victim->affected_by, AFF_DETECT_INVIS);

         if (IS_AFFECTED(victim, AFF_DETECT_EVIL) && (number_percent() < chance))
            REMOVE_BIT(victim->affected_by, AFF_DETECT_EVIL);

         if (IS_AFFECTED(victim, AFF_DETECT_MAGIC) && (number_percent() < chance))
            REMOVE_BIT(victim->affected_by, AFF_DETECT_MAGIC);

         if (IS_AFFECTED(victim, AFF_DETECT_HIDDEN) && (number_percent() < chance))
            REMOVE_BIT(victim->affected_by, AFF_DETECT_HIDDEN);

         if (IS_AFFECTED(victim, AFF_CLOAK_FLAMING) && (number_percent() < chance))
         {
            chance = (chance) / 3;

            act("@@NThe @@ecloak@@N around $n is ripped to shreds!", victim, NULL, NULL, TO_ROOM);
            send_to_char("@@NYour @@ecloak@@N is ripped from your body!\n\r", victim);
            REMOVE_BIT(victim->affected_by, AFF_CLOAK_FLAMING);
         }
      }

      CREF(vch_next, CHAR_NEXTROOM);
      for (vch = ch->in_room->first_person; vch; vch = vch_next)
      {
         vch_next = vch->next_in_room;
         if (victim == vch && victim->fighting == NULL)
         {
            check_killer(ch, victim);
            multi_hit(victim, ch, TYPE_UNDEFINED);
            break;
         }
      }
      char_unreference(&vch_next);

      return TRUE;
   }

   return TRUE;
}
