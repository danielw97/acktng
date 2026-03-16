#include "globals.h"
#include "magic.h"

bool can_hit_skill(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);
bool subtract_energy_cost(CHAR_DATA *ch, int gsn);
int get_energy_cost(CHAR_DATA *ch, int gsn);
void trip(CHAR_DATA *ch, CHAR_DATA *victim);

void trip(CHAR_DATA *ch, CHAR_DATA *victim)
{
   if (victim->wait == 0)
   {
      int chance;

      chance = IS_NPC(victim) ? IS_SET(victim->skills, MOB_NOTRIP) ? 75 : 0
                              : victim->pcdata->learned[gsn_notrip];

      /*
       * Check for no-trip
       */
      if (number_percent() < chance)
      {
         act("You sidestep $n's attempt to trip you!", ch, NULL, victim, TO_VICT);
         act("$N sidesteps your attempt to trip $M!", ch, NULL, victim, TO_CHAR);
         act("$N sidesteps $n's attempt to trip $m!", ch, NULL, victim, TO_NOTVICT);
         return;
      }

      act("$n trips you and you go down!", ch, NULL, victim, TO_VICT);
      act("You trip $N and $N goes down!", ch, NULL, victim, TO_CHAR);
      act("$n trips $N and $N goes down!", ch, NULL, victim, TO_NOTVICT);

      WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
      WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
      victim->position = POS_RESTING;
   }

   return;
}

bool can_hit_skill(CHAR_DATA *ch, CHAR_DATA *victim, int gsn)
{
   int chance = 0;

   if (gsn == gsn_poison_quinine || gsn == gsn_poison_arsenic || gsn == gsn_poison_nightshade)
   {
      // Can never hit immune poison with poison
      if (IS_SET(race_table[victim->race].race_flags, RACE_MOD_IMMUNE_POISON))
         return FALSE;
   }

   if (!IS_AWAKE(victim))
      chance += 75;

   if (IS_AFFECTED(ch, AFF_SNEAK) || item_has_apply(ch, ITEM_APPLY_SNEAK))
      chance += 10;

   if (!can_see(victim, ch))
      chance += 20;

   if (!can_see(ch, victim))
      chance -= 30;

   if (IS_NPC(ch))
      chance -= 20;

   if (!skill_success(ch, victim, gsn, chance))
      return FALSE;

   return TRUE;
}

int get_energy_cost(CHAR_DATA *ch, int gsn)
{
   int base_cost = 50;

   if (skill_table[gsn].min_mana == 0)
      return 0;

   if (skill_table[gsn].min_mana > base_cost)
      base_cost = skill_table[gsn].min_mana;

   base_cost -= base_cost * (get_curr_str(ch) + get_curr_dex(ch)) / 100;

   return base_cost;
}

bool subtract_energy_cost(CHAR_DATA *ch, int gsn)
{
   int cost = get_energy_cost(ch, gsn);

   if (ch == NULL)
      return FALSE;

   if (gsn < 1 || gsn >= TYPE_HIT)
      return FALSE;

   if (cost > ch->move)
   {
      send_to_char("You don't have enough move to do this.\n\r", ch);
      return FALSE;
   }

   ch->move -= cost;

   return TRUE;
}
