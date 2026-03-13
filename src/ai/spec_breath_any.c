#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_breath_any(CHAR_DATA *ch)
{
   if (!is_fighting(ch))
      return FALSE;

   switch (number_bits(3))
   {
   case 0:
      return spec_breath_fire(ch);
   case 1:
   case 2:
      return spec_breath_lightning(ch);
   case 3:
      return spec_breath_gas(ch);
   case 4:
      return spec_breath_acid(ch);
   case 5:
   case 6:
   case 7:
      return spec_breath_frost(ch);
   }

   return FALSE;
}
