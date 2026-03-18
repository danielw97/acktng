#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_sage(CHAR_DATA *ch)
{

   /*
    * Give the Sage some life.  He should chat with player, heal too, etc
    */

   /*
    * What the options are:
    * * W - Wake;   H - Greet;       C - make some food
    * * 1 - First Part of Story;      2-Second Part;    3-3rd part;
    * * 4- 4th part    5- 5th part;
    * * 6- 6th part    7-7th part;       8-8th part;
    * * F - rake the fire I - cast influx;
    * * G - Gossip     S - sleep.
    * * D - Do nothing.          X - Smile.    Y - Sigh.
    * * B - say byebye
    * * U - get food V - Eat food
    */

   static const char open_path[] = "WDDDHDFDCUVD1XDD2YDD3DD4DFD5DGDD6D7XDD7YDD8DFD9XDDDBDFD.";

   static const char *path;
   static int pos;
   static bool move;

   if (!move)
   {
      if (time_info.hour == 6)
      {
         path = open_path;
         move = TRUE;
         pos = 0;
      }
   }

   if (is_fighting(ch))
      return spec_cast_cleric(ch);
   if (!move || ch->position < POS_SLEEPING)
      return FALSE;

   switch (path[pos])
   {
   case 'D':
      /*
       * Do Nothing.
       */
      break;
   case 'W':
      ch->position = POS_RESTING;
      act("$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM);
      break;

   case 'S':
      ch->position = POS_SLEEPING;
      act("$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM);
      break;

   case 'H':
      do_say(ch, "Hello There!  Welcome to my humble cave.");
      break;
   case 'F':
      act("$n leans forward, and rakes the fire.", ch, NULL, NULL, TO_ROOM);
      break;
   case 'C':
   case 'U':
   case 'V':
      /* food-related actions removed */
      break;
   case 'X':
      act("$n smiles sadly, remembering better days.", ch, NULL, NULL, TO_ROOM);
      break;
   case 'Y':
      act("$n sighes sadly.", ch, NULL, NULL, TO_ROOM);
      break;
   case '1':
      do_say(ch, "Rest yourselves, and listen to what i have to tell you about the Sylai beasts.");
      break;
   case '2':
      do_say(ch, "Many decades ago, my race lived in these tunnels, living in peace.");
      do_say(ch, "Then, one day the Sylai came.  No one knows where they originated from.");
      do_say(ch, "But when they came, they brought the Dark Powers with them.");
      break;
   case '3':
      do_say(ch, "My race did not believe in killing.  We tried to reason with the Sylai.");
      do_say(ch, "but they retaliated with violence, killing us one by one.");
      break;
   case '4':
      do_say(ch, "The Sylai had no mercy.  They killed to satisfy the Dark Powers.");
      do_say(ch, "They enjoyed killing us.  My kind were enslaved to the Sylai.");
      do_say(ch, "They used us for their evil experiments, but we started to fight back.");
      break;
   case '5':
      do_say(ch, "But we were weak.  The Dark Powers within the Sylai was too much.");
      do_say(ch, "Slowly, they butchered my kind.  Some of us fled, and hid here.");
      break;
   case '6':
      do_say(ch, "There were originally five.  Now there is just me.");
      do_say(ch, "I grow weak now - i know i don't have long left.");
      break;
   case '7':
      do_say(ch, "I just wish that we had struck back when stronger in number.");
      do_say(ch, "But now there is just me.  And what can one old fool do?");
      break;
   case '8':
      do_say(ch, "If you have come this far, you must be strong.");
      do_say(ch, "Seek revenge for my race.  Search out their leader, Sylar.");
      do_say(ch, "And wipe him from this plane of existence.");
      break;
   case '9':
      do_say(ch, "Sylar stole my race's greatest power.  Our artifacts.");
      do_say(ch, "Without those, we were powerless.  But they would help you greatly.");
      do_say(ch, "Take them, and remember my race.");
      break;
   case 'G':
      do_say(ch, "I remember when old Caryl was still with me.  Oh, the fun we had.");
      do_say(ch, "But that was years ago.  And now she's gone, like all the rest.");
      break;
   case 'B':
      do_say(ch, "I grow tired.  It is time for me to sleep.");
      do_say(ch, "Go now, and strike back at Sylar");
      act("$n waves goodbye to you.", ch, NULL, NULL, TO_ROOM);
      break;
   case '.':
      act("$n lies down slowly, and falls into a troubled sleep.", ch, NULL, NULL, TO_ROOM);
      ch->position = POS_SLEEPING;
      move = FALSE;
      break;
   }

   pos++;
   return FALSE;
}
