#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_tactical_assess(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int hp_pct;

   if (!can_use_pub_society_skill(ch, gsn_tactical_assess))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_tactical_assess))
      return;

   one_argument(argument, arg);
   if (arg[0] == '\0')
   {
      send_to_char("Assess whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (!IS_NPC(victim))
   {
      send_to_char("You can only tactically assess NPCs.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_tactical_assess].beats);
   raise_skill(ch, gsn_tactical_assess);

   hp_pct = victim->max_hit > 0 ? (victim->hit * 100) / victim->max_hit : 0;

   sprintf(buf, "\n\r@@yTactical Assessment: %s@@N\n\r", victim->short_descr);
   send_to_char(buf, ch);
   sprintf(buf, "  Level: %d    HP: %d%% (%ld/%ld)\n\r", victim->level, hp_pct, victim->hit,
           victim->max_hit);
   send_to_char(buf, ch);

   if (victim->pIndexData->strong_magic)
   {
      send_to_char("  @@RResistances:@@N", ch);
      if (IS_SET(victim->pIndexData->strong_magic, ELE_FIRE))
         send_to_char(" fire", ch);
      if (IS_SET(victim->pIndexData->strong_magic, ELE_WATER))
         send_to_char(" water", ch);
      if (IS_SET(victim->pIndexData->strong_magic, ELE_AIR))
         send_to_char(" air", ch);
      if (IS_SET(victim->pIndexData->strong_magic, ELE_EARTH))
         send_to_char(" earth", ch);
      if (IS_SET(victim->pIndexData->strong_magic, ELE_HOLY))
         send_to_char(" holy", ch);
      if (IS_SET(victim->pIndexData->strong_magic, ELE_SHADOW))
         send_to_char(" shadow", ch);
      send_to_char("\n\r", ch);
   }

   if (victim->pIndexData->weak_magic)
   {
      send_to_char("  @@gVulnerabilities:@@N", ch);
      if (IS_SET(victim->pIndexData->weak_magic, ELE_FIRE))
         send_to_char(" fire", ch);
      if (IS_SET(victim->pIndexData->weak_magic, ELE_WATER))
         send_to_char(" water", ch);
      if (IS_SET(victim->pIndexData->weak_magic, ELE_AIR))
         send_to_char(" air", ch);
      if (IS_SET(victim->pIndexData->weak_magic, ELE_EARTH))
         send_to_char(" earth", ch);
      if (IS_SET(victim->pIndexData->weak_magic, ELE_HOLY))
         send_to_char(" holy", ch);
      if (IS_SET(victim->pIndexData->weak_magic, ELE_SHADOW))
         send_to_char(" shadow", ch);
      send_to_char("\n\r", ch);
   }

   if (victim->spec_fun != NULL)
      send_to_char("  @@RThis creature has special attacks.@@N\n\r", ch);

   if (IS_SET(victim->act, ACT_BOSS))
      send_to_char("  @@R** BOSS **@@N\n\r", ch);
}
