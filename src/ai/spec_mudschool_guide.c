#include "globals.h"
#include "special.h"

#define VNUM_GEAR_BAG 4930

static const int gear_vnums[] = {4931, /* academy lantern     */
                                 4932, /* training sword      */
                                 4933, /* training buckler    */
                                 4935, /* iron helm           */
                                 4936, /* iron faceguard      */
                                 4937, /* jade earring        */
                                 4938, /* ruby earring        */
                                 4939, /* silver chain necklace */
                                 4940, /* amber amulet        */
                                 4941, /* leather shoulderpads */
                                 4942, /* tattered cloak      */
                                 4943, /* chainmail vest      */
                                 4944, /* studded belt        */
                                 4945, /* iron leggings       */
                                 4946, /* iron boots          */
                                 4947, /* padded sleeves      */
                                 4948, /* copper bracelet     */
                                 4949, /* silver bracelet     */
                                 4950, /* leather gauntlets   */
                                 0};

/*
 * speech_mudschool_guide — SPEECH_FUN for the Academy Guide (mob vnum 4910).
 *
 * Called from do_say() when a player speaks in the guide's room.  If the
 * player's message contains a gear/bag keyword, the guide creates an
 * adventurer's bag loaded with the full newbie equipment set and gives it
 * to the player.  Returns TRUE to suppress LLM dispatch when handled.
 */
bool speech_mudschool_guide(CHAR_DATA *mob, CHAR_DATA *player, const char *message)
{
   static const char *keywords[] = {"bag", "gear", "equipment", "kit", "stuff", NULL};
   OBJ_DATA *bag;
   OBJ_DATA *item;
   OBJ_INDEX_DATA *pIndex;
   int i;
   bool matched = FALSE;

   for (i = 0; keywords[i] != NULL; i++)
   {
      if (!str_infix(keywords[i], message))
      {
         matched = TRUE;
         break;
      }
   }

   if (!matched)
      return FALSE;

   /* Check if player already carries an adventurer's bag from the guide */
   for (bag = player->first_carry; bag != NULL; bag = bag->next_in_carry_list)
   {
      if (bag->pIndexData->vnum == VNUM_GEAR_BAG)
      {
         act("$n says 'You already have an adventurer's bag, $N!  Make good use of it.'", mob, NULL,
             player, TO_ROOM);
         act("$n says 'You already have an adventurer's bag, $N!  Make good use of it.'", mob, NULL,
             player, TO_VICT);
         return TRUE;
      }
   }

   /* Create the bag */
   pIndex = get_obj_index(VNUM_GEAR_BAG);
   if (pIndex == NULL)
      return FALSE;

   bag = create_object(pIndex, 1);
   if (bag == NULL)
      return FALSE;

   /* Load gear items into the bag */
   for (i = 0; gear_vnums[i] != 0; i++)
   {
      pIndex = get_obj_index(gear_vnums[i]);
      if (pIndex == NULL)
         continue;
      item = create_object(pIndex, 1);
      if (item == NULL)
         continue;
      obj_to_obj(item, bag);
   }

   obj_to_char(bag, player);

   act("$n reaches into a cabinet and produces a well-packed adventurer's bag, handing it to $N.",
       mob, NULL, player, TO_ROOM);
   act("$n reaches into a cabinet and produces a well-packed adventurer's bag, handing it to you.",
       mob, NULL, player, TO_VICT);
   act("$n says 'Here is a full set of starter gear, $N.  Type LOOK IN BAG to see what's inside,"
       " then WEAR ALL to equip everything at once.'",
       mob, NULL, player, TO_ROOM);
   act("$n says 'Here is a full set of starter gear, $N.  Type LOOK IN BAG to see what's inside,"
       " then WEAR ALL to equip everything at once.'",
       mob, NULL, player, TO_VICT);

   return TRUE;
}

/*
 * spec_mudschool_guide — SPEC_FUN for the Academy Guide (mob vnum 4910).
 *
 * Called directly from the new-character login path (login.c) when a brand-new
 * player enters the mudschool for the first time.  Delivers a one-time static
 * greeting and clears is_new_player so subsequent tick-based calls are no-ops.
 */
bool spec_mudschool_guide(CHAR_DATA *ch)
{
   CHAR_DATA *victim;

   for (victim = ch->in_room->first_person; victim != NULL; victim = victim->next_in_room)
   {
      if (!IS_NPC(victim) && victim->pcdata->is_new_player)
      {
         victim->pcdata->is_new_player = FALSE;
         act("$n says 'Welcome, $N!  I am the Academy Guide.  I am an AI-enabled NPC"
             " — feel free to ask me any questions you have about the game.'",
             ch, NULL, victim, TO_ROOM);
         act("$n says 'Welcome, $N!  I am the Academy Guide.  I am an AI-enabled NPC"
             " — feel free to ask me any questions you have about the game.'",
             ch, NULL, victim, TO_VICT);
         return TRUE;
      }
   }

   return FALSE;
}
