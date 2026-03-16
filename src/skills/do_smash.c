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
#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_smash(CHAR_DATA *ch, char *argument)
{
   const sh_int rev_dir[] = {2, 3, 0, 1, 5, 4};
   char arg[MAX_INPUT_LENGTH];
   int door;
   bool joke; /* Was it unlocked to start with? */

   joke = FALSE;
   one_argument(argument, arg);

   if (!can_use_skill(ch, gsn_smash))
   {
      send_to_char("You don't know of such a skill.\n\r", ch);
      return;
   }

   if (arg[0] == '\0')
   {
      send_to_char("Smash what?\n\r", ch);
      return;
   }

   if ((door = find_door(ch, arg)) >= 0)
   {
      /*
       * 'open door'
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if (!IS_SET(pexit->exit_info, EX_CLOSED))
      {
         send_to_char("It's already open.\n\r", ch);
         return;
      }

      if (IS_SET(pexit->exit_info, EX_LOCKED))
         REMOVE_BIT(pexit->exit_info, EX_LOCKED);
      else
         joke = TRUE;

      REMOVE_BIT(pexit->exit_info, EX_CLOSED);
      act("$n smashes opens the $d.", ch, NULL, pexit->keyword, TO_ROOM);
      act("You smash open the $d.", ch, NULL, pexit->keyword, TO_CHAR);

      if (joke)
      {
         act("The $d was already unlocked!!!", ch, NULL, pexit->keyword, TO_ROOM);
         act("The $d was already unlocked!!!", ch, NULL, pexit->keyword, TO_CHAR);
         act("$n tumbles through $d!!!", ch, NULL, pexit->keyword, TO_ROOM);
         act("You tumble through $d!!!", ch, NULL, pexit->keyword, TO_CHAR);
      }

      /*
       * open the other side
       */
      if ((to_room = pexit->to_room) != NULL &&
          (pexit_rev = to_room->exit[rev_dir[door]]) != NULL && pexit_rev->to_room == ch->in_room)
      {
         CHAR_DATA *rch;

         if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
            REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);

         REMOVE_BIT(pexit_rev->exit_info, EX_CLOSED);
         for (rch = to_room->first_person; rch != NULL; rch = rch->next_in_room)
            act("The $d is smashed open.", rch, NULL, pexit_rev->keyword, TO_CHAR);

         if (joke)
         {
            char_from_room(ch);
            char_to_room(ch, to_room);
            act("$n tumbles into the room, head over heels!", ch, NULL, NULL, TO_ROOM);
            do_look(ch, "");
         }
      }
   }

   return;
}
