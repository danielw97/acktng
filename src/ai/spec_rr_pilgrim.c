#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_rr_pilgrim(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n lowers $s pack to the ground, rolling $s shoulders with a wince of relief.",
       "$n takes a careful drink from the clay water jar, allowing only a measured sip.",
       "$n adjusts the cord holding $s sandals together, retying a knot that has slipped loose.",
       "$n reads a worn slip of paper, folds it carefully, and tucks it back into a pocket.",
       "$n stands at the road's edge and looks west for a long moment before resuming the walk."};
   static char *says[] = {
       "How far to the next waystone shrine? I've been walking since the gate and I haven't seen "
       "water.",
       "I know how I look. I've been on this road for eight days. I know how long it takes.",
       "The wardens at the gate told me the road west was clear. They said that three days ago.",
       "There's a shrine keeper a few miles back who gave me oil for my lamp without being asked. "
       "The road still has grace in it.",
       "I'm not lost. I know exactly where I'm going. I'm just not certain I'll make it."};

   if (!IS_AWAKE(ch) || is_fighting(ch))
      return FALSE;

   if (number_bits(3) != 0)
      return FALSE;

   if (number_bits(1) == 0)
      act(acts[number_range(0, 4)], ch, NULL, NULL, TO_ROOM);
   else
      do_say(ch, says[number_range(0, 4)]);

   return FALSE;
}
