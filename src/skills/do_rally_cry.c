#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_rally_cry(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *gch;
   AFFECT_DATA *paf, *paf_next;
   int count = 0;

   if (!can_use_pub_society_skill(ch, gsn_rally_cry))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_rally_cry))
      return;

   WAIT_STATE(ch, skill_table[gsn_rally_cry].beats);
   ch->cooldown[gsn_rally_cry] = 150; /* ~10 minutes at standard tick rate */
   raise_skill(ch, gsn_rally_cry);

   act("$n shouts a rallying cry!", ch, NULL, NULL, TO_ROOM);
   send_to_char("You shout a rallying cry!\n\r", ch);

   /* Remove fear/panic effects from group members in room */
   for (gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room)
   {
      if (!is_same_group(gch, ch))
         continue;

      for (paf = gch->first_affect; paf != NULL; paf = paf_next)
      {
         paf_next = paf->next;
         /* Remove fear-type effects (terror, panic, etc.) */
         if (paf->type > 0 && paf->type < MAX_SKILL && skill_table[paf->type].name != NULL &&
             (str_infix("fear", skill_table[paf->type].name) == FALSE ||
              str_infix("terror", skill_table[paf->type].name) == FALSE ||
              str_infix("panic", skill_table[paf->type].name) == FALSE))
         {
            affect_remove(gch, paf);
            count++;
         }
      }

      if (gch != ch)
         send_to_char("@@WThe rallying cry steadies your nerves!@@N\n\r", gch);
   }
}
