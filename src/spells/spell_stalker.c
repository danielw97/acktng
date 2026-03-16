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

bool spell_stalker(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   /*
    * Fixed problem of stalker not finding victim, and attacking caster,
    * * thus giving an easy source of xp -S-
    */

   CHAR_DATA *victim;
   CHAR_DATA *stalker;

   if (target_name[0] == '\0')
   {
      send_to_char("Summon a stalker to hunt who?\n\r", ch);
      return FALSE;
   }

   victim = get_char_world(ch, target_name);

   if (victim == NULL)
   {
      send_to_char("Target can't be found!\n\r", ch);
      return FALSE;
   }

   if (ch == victim)
   {
      send_to_char("That WOULDN'T be wise!\n\r", ch);
      return FALSE;
   }
   act("$n calls upon the dark powers to summon forth a Stalker!", ch, NULL, NULL, TO_ROOM);
   send_to_char("You call upon the dark powers to summon forth a Stalker!\n\r", ch);

   stalker = create_mobile(get_mob_index(MOB_VNUM_STALKER));

   char_to_room(stalker, ch->in_room);
   act("$n appears before you suddenly.", stalker, NULL, NULL, TO_ROOM);

   stalker->level = victim->level;
   stalker->max_hit = victim->max_hit;
   stalker->hit = stalker->max_hit;
   stalker->exp = victim->level * 10; /* not much exp :P */

   if (set_hunt(stalker, ch, victim, NULL, HUNT_MERC, HUNT_CR))
      act("$n sniffs the air in search of $s prey.", stalker, NULL, NULL, TO_ROOM);
   else
   {
      int sn;

      sn = skill_lookup("hellspawn");
      if (sn != 0) /* Check to be sure... should never == 0 */
         (*skill_table[sn].spell_fun)(sn, stalker->level, stalker, ch, NULL);

      do_say(stalker, "How dare you waste my time!!");
      act("$n returns to the dark planes, vanishing suddenly!", stalker, NULL, NULL, TO_ROOM);
      /*
       * char_from_room( stalker );
       */
      extract_char(stalker, TRUE);
   }
   return TRUE;
}
