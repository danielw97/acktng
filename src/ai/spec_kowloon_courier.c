#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_courier(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n checks a dispatch pouch's seal for tampering before shouldering it — ingrained "
       "professional habit.",
       "$n consults a route card, noting a shortcut through the registry quarter with a quick "
       "finger-trace.",
       "$n polishes the brass lantern badge on $s vest, the mark of Courier Lantern Office passage "
       "rights.",
       "$n records a delivery confirmation in a small field ledger, then stows it without pausing "
       "stride.",
       "$n scans a bulletin board at the junction corner, checking for priority rerouting notices.",
       "$n updates a relay station log from memory, filling in the handoff times without "
       "consulting notes.",
       "$n reroutes a dispatch through the Jade Gate after checking the Iron Gate's current queue "
       "length."};
   static char *says[] = {
       "The badge gets you through any gate at any hour. You'd be surprised how useful that is at "
       "second bell.",
       "CLO runs the census, the post, the recall registry, and the relay stations. We count "
       "everyone inside these walls.",
       "I know every shortcut between here and the Jade Gate. You need fast delivery anywhere in "
       "this city, come to us.",
       "Sealed dispatch means sealed. I don't read them, I don't hold them. I deliver, I confirm, "
       "I move.",
       "Last week I ran the Iron Gate route four times before Bell Watch. That's corsair intel — "
       "frequent updates.",
       "Mosswater route has been showing contraband traffic. If someone cleared those smugglers "
       "out, relay times would improve considerably on the northern circuit.",
       "The Gloamvault runs northwest off the Kiess circuit — our field couriers bypass it. "
       "Something worth checking if you're operating at that capability level."};

   CHAR_DATA *plr;

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   switch (number_range(0, 2))
   {
   case 0: /* Quest completion recognition */
   {
      CHAR_DATA *qplr[8];
      int qid[8];
      int nq = 0;

      for (plr = ch->in_room->first_person; plr != NULL; plr = plr->next_in_room)
      {
         if (IS_NPC(plr) || plr->pcdata == NULL)
            continue;
         if (plr->pcdata->completed_static_quests[16])
         {
            qplr[nq] = plr;
            qid[nq++] = 16;
         }
         if (plr->pcdata->completed_static_quests[17])
         {
            qplr[nq] = plr;
            qid[nq++] = 17;
         }
         if (plr->pcdata->completed_static_quests[0])
         {
            qplr[nq] = plr;
            qid[nq++] = 0;
         }
      }

      if (nq > 0)
      {
         int pick = number_range(0, nq - 1);
         plr = qplr[pick];
         switch (qid[pick])
         {
         case 16:
            act("$n glances toward $N and touches the brim of $s cap in a courier's brief salute.",
                ch, NULL, plr, TO_NOTVICT);
            act("$n glances toward you and touches the brim of $s cap in a courier's brief salute.",
                ch, NULL, plr, TO_VICT);
            do_say(ch, "Mosswater route is running clean now. Relay times on the northern circuit "
                       "came down half a bell after those smugglers were cleared. That's "
                       "operationally significant for the CLO schedule.");
            return FALSE;
         case 17:
            act("$n pauses $s route card work and acknowledges $N with a brief nod.", ch, NULL, plr,
                TO_NOTVICT);
            act("$n pauses $s route card work and acknowledges you with a brief nod.", ch, NULL,
                plr, TO_VICT);
            do_say(ch, "Northern Crown zone assessment is in the registry now. We updated the "
                       "field courier routing to avoid the predator corridors your survey "
                       "confirmed. The map is more accurate for it.");
            return FALSE;
         case 0:
            act("$n catches $N's eye with the quick recognition of someone who reads the route "
                "reports.",
                ch, NULL, plr, TO_NOTVICT);
            act("$n catches your eye with the quick recognition of someone who reads the route "
                "reports.",
                ch, NULL, plr, TO_VICT);
            do_say(ch, "Forest of Confusion approach data you filed came through the CLO relay. "
                       "Courier dispatch rescheduled two northern handoffs based on your threat "
                       "marks. Good data.");
            return FALSE;
         }
      }
      break;
   }
   case 1: /* Area hint */
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
         const char *hints[5];
         int hint_count = 0;
         int lv = hint_plr->level;
         if (lv >= 5 && lv <= 20)
            hints[hint_count++] =
                "If you're still building your field experience, the Gloamvault northwest of Kiess "
                "runs on our secondary circuit. Cult structure — CLO doesn't post couriers inside, "
                "but we know the perimeter. Worth your time at that level.";
         if (lv >= 10 && lv <= 25)
            hints[hint_count++] =
                "The Nightfall Catacombs show up in our routing notes as a restricted zone — CLO "
                "routes around it. Undead concentration, multi-level vault. If you're capable of "
                "clearing such things, it's worth charting.";
         if (lv >= 15 && lv <= 30)
            hints[hint_count++] =
                "Sepulcher Pasture is east of the crossroads, on the far side of the banner hills. "
                "CLO routes couriers around it — burial cult territory, bone-remnant activity. "
                "Worth charting if you're capable of handling what's in there.";
         if (lv >= 20 && lv <= 35)
            hints[hint_count++] =
                "The Cathedral of the Violet Eclipse is marked as a relay detour on the western "
                "circuit — eclipse cult site, relic guardians on every approach. CLO avoids it, "
                "but field operatives at your level might want to investigate.";
         if (lv >= 25 && lv <= 40)
            hints[hint_count++] =
                "Umbra Heartspire sits at the edge of the CLO routing zone — shadow construct "
                "territory, too unpredictable for courier paths. High-capability field work if "
                "you're ready for that kind of engagement.";
         if (hint_count > 0)
         {
            do_say(ch, (char *)hints[number_range(0, hint_count - 1)]);
            return FALSE;
         }
      }
      break;
   }
   }

   /* Normal random dialogue */
   if (number_bits(1) == 0)
      act(acts[number_range(0, 6)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 6)]);

   return FALSE;
}
