#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_vamp_hunter(CHAR_DATA *ch)
{

   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];

   if (ch->hunting == NULL)
      return FALSE;

   if ((!IS_VAMP(ch->hunting)) || (IS_NPC(ch->hunting)))
      return FALSE;

   switch (number_range(1, 21))
   {

   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
      break;
   case 6:
      sprintf(buf, "%s", ch->hunting->name);
      sprintf(buf1, "@@eI know that you are a Vampyre, and I shall not rest until your are destroyed!!!@@N\n\r");
      safe_strcat(MAX_STRING_LENGTH, buf, buf1);
      do_tell(ch, buf);
      break;
   case 7:
   case 8:
   case 9:
   case 10:
   case 11:
      break;
   case 12:
   case 13:
      sprintf(buf, "My informants have told me that %s is a Vampyre, and I have vowed to destroy him!!\n\r",
              ch->hunting->name);
      do_yell(ch, buf);
      break;

   case 14:
   case 15:
   case 16:
      break;
   case 17:
      sprintf(buf, " %s ", ch->hunting->name);
      sprintf(buf1, "@@Do you finally know fear? I shall not rest until ALL of your kind are destroyed!!!@@N\n\r");
      safe_strcat(MAX_STRING_LENGTH, buf, buf1);
      do_tell(ch, buf);
      break;

   case 18:
   case 19:
      break;
   case 20:
      sprintf(buf,
              "All the realm shall know that %s is a Vampyre, and I shall eridicate all of these vile creatures!!\n\r",
              ch->hunting->name);
      do_yell(ch, buf);
      break;
   default:
      break;
   }

   return TRUE;
}
