/*
 * spell_revenant.c — Raise a revenant from an NPC corpse.
 *
 * Replaces spell_animate.  The revenant inherits the corpse mob's
 * full combat stats, capped at the caster's pseudo_level (max 150).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "tables.h"
#include "magic.h"
#include "revenant.h"

bool spell_revenant(int sn, int level, CHAR_DATA *ch, void *vo, OBJ_DATA *obj)
{
   OBJ_DATA *corpse;
   CHAR_DATA *rev;

   (void)sn;
   (void)level;
   (void)vo;
   (void)obj;

   if (IS_NPC(ch))
      return FALSE;

   /* Check if player already has a revenant */
   if (find_revenant(ch) != NULL)
   {
      send_to_char("@@dYou already command a revenant. Release it first.@@N\n\r", ch);
      return FALSE;
   }

   corpse = get_obj_here(ch, target_name);

   if (corpse == NULL)
   {
      send_to_char("You don't see that here.\n\r", ch);
      return FALSE;
   }

   if (corpse->item_type != ITEM_CORPSE_NPC)
   {
      send_to_char("@@dYou can only raise a revenant from an NPC corpse.@@N\n\r", ch);
      return FALSE;
   }

   if (corpse->value[4] == 0)
   {
      send_to_char("@@dThis corpse is too decayed to raise as a revenant.@@N\n\r", ch);
      return FALSE;
   }

   /* Dramatic casting messages */
   act("@@d$n plunges $s hands into $p, channeling dark energy!@@N", ch, corpse, NULL, TO_ROOM);
   act("@@dYou plunge your hands into $p, channeling dark energy!@@N", ch, corpse, NULL, TO_CHAR);

   act("@@eNecrotic lightning arcs from $p as the flesh knits back together!@@N", ch, corpse, NULL,
       TO_ROOM);
   act("@@eNecrotic lightning arcs from $p as the flesh knits back together!@@N", ch, corpse, NULL,
       TO_CHAR);

   /* Create the revenant */
   rev = revenant_create(ch, corpse);
   if (rev == NULL)
   {
      send_to_char("@@dThe corpse crumbles to dust. The raising fails.@@N\n\r", ch);
      return FALSE;
   }

   act("@@d$N rises from the dead, eyes burning with @@eunholy fire@@d!@@N", ch, NULL, rev,
       TO_ROOM);
   act("@@d$N rises from the dead, eyes burning with @@eunholy fire@@d!@@N", ch, NULL, rev,
       TO_CHAR);

   {
      char buf[MAX_STRING_LENGTH];

      sprintf(buf,
              "\n\r@@dYou have raised a @@Wlevel %d@@d revenant from a level %d corpse.@@N\n\r"
              "@@dUse '@@Wrevenant status@@d' to view stats, '@@Wrevenant tracks@@d' to see "
              "upgrade paths.@@N\n\r",
              rev->level, rev->revenant->corpse_level);
      send_to_char(buf, ch);
   }

   return TRUE;
}
