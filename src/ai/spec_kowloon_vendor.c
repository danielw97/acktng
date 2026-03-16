#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif
#include "special.h"

bool spec_kowloon_vendor(CHAR_DATA *ch)
{
   static const char *acts[] = {
       "$n stirs a pot of spiced rice porridge over the charcoal brazier, releasing a fragrant "
       "cloud of steam.",
       "$n calls out to passing foot traffic, holding up a small wrapped portion in invitation.",
       "$n ladles a serving into a clay bowl and accepts a coin with the practiced ease of long "
       "repetition.",
       "$n adjusts the charcoal bed with a long iron fork, managing the heat for the midday rush.",
       "$n wipes down the handcart's folding counter between orders, keeping the workspace clear."};
   static char *says[] = {"Hot rice porridge, dried fish, pickled greens — delta standard since "
                          "before the walls were built. Fills you up, won't slow you down.",
                          "Bell Watch is the best time to buy. I knock down what's left before I "
                          "move the cart east for the afternoon crowd.",
                          "Market season's lighter this quarter. Caravan tariffs went up again — "
                          "everything from the inland roads is priced higher now.",
                          "I follow the foot traffic. Courier crossing in the morning, provisioner "
                          "junction at midday, harbor spine before dusk.",
                          "Salt-rice and black tea — that's the delta working meal. Syndic clerks, "
                          "dock haulers, Warden patrols, they all eat here."};

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
