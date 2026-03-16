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

void do_pick(CHAR_DATA *ch, char *argument)
{
   const sh_int rev_dir[] = {2, 3, 0, 1, 5, 4};
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *gch;
   OBJ_DATA *obj;
   int door;

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Pick what?\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_pick_lock].beats);

   /*
    * look for guards
    */
   for (gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
   {
      if (IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level)
      {
         act("$N is standing too close to the lock.", ch, NULL, gch, TO_CHAR);
         return;
      }
   }

   if (!can_use_skill(ch, gsn_pick_lock))
   {
      send_to_char("You failed.\n\r", ch);
      return;
   }

   if ((obj = get_obj_here(ch, arg)) != NULL)
   {
      /*
       * 'pick object'
       */
      if (obj->item_type != ITEM_CONTAINER)
      {
         send_to_char("That's not a container.\n\r", ch);
         return;
      }
      if (!IS_SET(obj->value[1], CONT_CLOSED))
      {
         send_to_char("It's not closed.\n\r", ch);
         return;
      }
      if (obj->value[2] < 0)
      {
         send_to_char("It can't be unlocked.\n\r", ch);
         return;
      }
      if (!IS_SET(obj->value[1], CONT_LOCKED))
      {
         send_to_char("It's already unlocked.\n\r", ch);
         return;
      }
      if (IS_SET(obj->value[1], CONT_PICKPROOF))
      {
         send_to_char("You failed.\n\r", ch);
         return;
      }

      REMOVE_BIT(obj->value[1], CONT_LOCKED);
      send_to_char("*Click*\n\r", ch);
      act("$n picks $p.", ch, obj, NULL, TO_ROOM);
      return;
   }

   if ((door = find_door(ch, arg)) >= 0)
   {
      /*
       * 'pick door'
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if (!IS_SET(pexit->exit_info, EX_CLOSED))
      {
         send_to_char("It's not closed.\n\r", ch);
         return;
      }
      if (pexit->key < 0)
      {
         send_to_char("It can't be picked.\n\r", ch);
         return;
      }
      if (!IS_SET(pexit->exit_info, EX_LOCKED))
      {
         send_to_char("It's already unlocked.\n\r", ch);
         return;
      }
      if (IS_SET(pexit->exit_info, EX_PICKPROOF))
      {
         send_to_char("You failed.\n\r", ch);
         return;
      }

      REMOVE_BIT(pexit->exit_info, EX_LOCKED);
      send_to_char("*Click*\n\r", ch);
      act("$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM);

      /*
       * pick the other side
       */
      if ((to_room = pexit->to_room) != NULL &&
          (pexit_rev = to_room->exit[rev_dir[door]]) != NULL && pexit_rev->to_room == ch->in_room)
      {
         REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
      }
   }

   return;
}
