#include "globals.h"
#include "magic.h"
#include "skills.h"

void do_command_presence(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   CHAR_DATA *gch;
   AFFECT_DATA af;
   char arg[MAX_INPUT_LENGTH];
   int dam;

   if (!can_use_pub_society_skill(ch, gsn_command_presence))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_command_presence))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to use command presence!\n\r", ch);
      return;
   }

   one_argument(argument, arg);
   if (arg[0] == '\0')
      victim = ch->fighting;
   else
   {
      if ((victim = get_char_room(ch, arg)) == NULL)
      {
         send_to_char("They aren't here.\n\r", ch);
         return;
      }
   }

   if (!subtract_energy_cost(ch, gsn_command_presence))
      return;

   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 5,
                      get_psuedo_level(ch) * get_curr_str(ch) / 4);
   if (dam < 1)
      dam = 1;

   WAIT_STATE(ch, skill_table[gsn_command_presence].beats);
   ch->cooldown[gsn_command_presence] = 20;
   raise_skill(ch, gsn_command_presence);

   check_killer(ch, victim);

   if (can_hit_skill(ch, victim, gsn_command_presence))
   {
      act("$n delivers a commanding strike and rallies $s allies!", ch, NULL, victim, TO_NOTVICT);
      act("$n delivers a commanding strike against you!", ch, NULL, victim, TO_VICT);
      send_to_char("You deliver a commanding strike and rally your allies!\n\r", ch);

      calculate_damage(ch, victim, dam, gsn_command_presence, ELE_PHYSICAL, TRUE);

      /* Apply hitroll buff to all group members in room */
      for (gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room)
      {
         if (!is_same_group(gch, ch))
            continue;
         if (is_affected(gch, gsn_command_presence))
            continue;

         af.type = gsn_command_presence;
         af.duration = 3;
         af.duration_type = DURATION_ROUND;
         af.location = APPLY_HITROLL;
         af.modifier = 5;
         af.bitvector = 0;
         af.caster = ch;
         affect_to_char(gch, &af);

         if (gch != ch)
            send_to_char("@@WThe Marshal's presence steels your resolve!@@N\n\r", gch);
      }

      combo(ch, victim, gsn_command_presence);
   }
   else
   {
      act("$n swings with authority but misses!", ch, NULL, victim, TO_NOTVICT);
      act("$n swings with authority but misses you!", ch, NULL, victim, TO_VICT);
      send_to_char("Your commanding strike misses!\n\r", ch);
      set_fighting(victim, ch, TRUE);
   }
}
