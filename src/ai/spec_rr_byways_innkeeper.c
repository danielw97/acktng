#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_byways_innkeeper(CHAR_DATA *ch)
{
   static const char *acts[] = {
      "$n refills a cup from the tap without being asked, returning the serving with a practiced nod.",
      "$n reviews the room ledger at the front table, penciling an amendment to tomorrow's availability.",
      "$n wipes the common room table with a damp cloth, clearing the day's crumbs in efficient strokes.",
      "$n checks the firewood stack beside the hearth and makes a mental calculation about the evening burn.",
      "$n glances at the door as it opens, notes the new arrival, and reaches for a fresh cup."
   };
   static char *says[] = {
      "The ferry runs at dawn and dusk. Miss it and you're swimming or waiting. Ferryman doesn't adjust schedule for stragglers.",
      "We get convoy traffic from both directions here. Most of it's Kiess trade, some Midgaard official. The split's been about even this season.",
      "The north ridge wolves have been a topic lately. Travelers coming down from the Banner Hills with bite marks, mostly. Nothing fatal this season.",
      "Market day at the crossroads is twice a week. Day before market is our busiest night — traders holding over.",
      "The cave system east of here isn't safe for casual exploration. We've had guests who went in and came back roughed up. Spiders, mostly.",
      "The shrine keeps the village on a decent sort of footing. People give what they can and take what they need. It works out."
   };

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   switch (number_range(0, 2))
   {
      case 0:  /* Quest completion recognition */
      {
         CHAR_DATA *qplr[8];
         int qid[8];
         int nq = 0;

         for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
         {
            if (IS_NPC(plr) || plr->pcdata == NULL)
               continue;
            if (plr->pcdata->completed_static_quests[75]) { qplr[nq] = plr; qid[nq++] = 75; }
            if (plr->pcdata->completed_static_quests[76]) { qplr[nq] = plr; qid[nq++] = 76; }
            if (plr->pcdata->completed_static_quests[79]) { qplr[nq] = plr; qid[nq++] = 79; }
         }

         if (nq > 0)
         {
            int pick = number_range(0, nq - 1);
            plr = qplr[pick];
            switch (qid[pick])
            {
               case 75:
                  act("$n sets a better cup in front of $N without explanation.", ch, NULL, plr, TO_NOTVICT);
                  act("$n sets a better cup in front of you without explanation.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "Word got around that the road survey's been done. The wardens mentioned it. I appreciate anyone who takes the time to actually document these roads — makes the journey safer for everyone who comes after.");
                  return FALSE;
               case 76:
                  act("$n refills $N's cup with noticeably better stock.", ch, NULL, plr, TO_NOTVICT);
                  act("$n refills your cup with noticeably better stock.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "The ridge has been quieter since the wolf count dropped. Two traders came in last week saying the north approach felt different — less watched. Good for business, good for the village.");
                  return FALSE;
               case 79:
                  act("$n places a small dish of pickled road-herbs in front of $N — house compliment.", ch, NULL, plr, TO_NOTVICT);
                  act("$n places a small dish of pickled road-herbs in front of you — house compliment.", ch, NULL, plr, TO_VICT);
                  do_say(ch, "Fewer bite reports from the cave side of the road. I keep an informal tally — it's useful for advising guests. The count's been improving. Whatever you did in there helped.");
                  return FALSE;
            }
         }
         break;
      }
      case 1:  /* Area hint */
      {
         CHAR_DATA *hint_plr = NULL;
         for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
         {
            if (!IS_NPC(plr) && plr->pcdata != NULL)
            {
               hint_plr = plr;
               break;
            }
         }
         if (hint_plr != NULL)
         {
            const char *hints[4];
            int hint_count = 0;
            int lv = hint_plr->level;
            if (lv >= 10 && lv <= 40) hints[hint_count++] = "The cave entrance is to the east — past the crossroads, down from the road surface. Guests who've been in there mention spiders particularly. They're faster than you expect.";
            if (lv >= 25 && lv <= 60) hints[hint_count++] = "The north ridge is Banner Hills territory — wolves run it. The Kiess postmaster's had a notice about it for a while. Something worth looking into if you're traveling that stretch anyway.";
            if (hint_count > 0)
            {
               do_say(ch, (char *)hints[number_range(0, hint_count - 1)]);
               return FALSE;
            }
         }
         break;
      }
   }

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 5)]);

   return FALSE;
}
