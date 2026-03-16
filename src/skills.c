#include "globals.h"
#include "magic.h"

bool can_hit_skill(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);
bool subtract_energy_cost(CHAR_DATA *ch, int gsn);
int get_energy_cost(CHAR_DATA *ch, int gsn);
void trip(CHAR_DATA *ch, CHAR_DATA *victim);

void do_rescue(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *fch;
   int best;
   int cnt;

   best = -1;

   if (!IS_NPC(ch))
   {
      for (cnt = 0; cnt < MAX_CLASS; cnt++)
         if (ch->class_level[cnt] >= skill_table[gsn_rescue].skill_level[cnt] &&
             ch->class_level[cnt] >= best)
            best = cnt;
   }
   else
      best = ch->level;

   if (!can_use_skill_message(ch, gsn_rescue))
      return;

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Rescue whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (victim == ch)
   {
      send_to_char("What about fleeing instead?\n\r", ch);
      return;
   }

   if (!IS_NPC(ch) && IS_NPC(victim))
   {
      send_to_char("Doesn't need your help!\n\r", ch);
      return;
   }

   if (ch->fighting == victim)
   {
      send_to_char("Too late.\n\r", ch);
      return;
   }

   if ((fch = victim->fighting) == NULL)
   {
      send_to_char("That person is not fighting right now.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_rescue].beats);
   if (!IS_NPC(ch) && !skill_success(ch, victim, gsn_rescue, 0))
   {
      send_to_char("You fail the rescue.\n\r", ch);
      return;
   }

   act("You rescue $N!", ch, NULL, victim, TO_CHAR);
   act("$n rescues you!", ch, NULL, victim, TO_VICT);
   act("$n rescues $N!", ch, NULL, victim, TO_NOTVICT);

   stop_fighting(fch, FALSE);
   stop_fighting(victim, FALSE);

   set_fighting(ch, fch, TRUE);
   set_fighting(fch, ch, FALSE);
   return;
}

void do_trip(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int best;
   /*    int cnt;  */

   best = -1;

   if (!IS_NPC(ch))
   {
      /*
       * for ( cnt = 0; cnt < MAX_CLASS; cnt++ )
       * if ( ch->class_level[cnt] >= skill_table[gsn_trip].skill_level[cnt]
       * && ch->class_level[cnt] >= best )
       * best = cnt;
       */
      if (ch->pcdata->learned[gsn_trip] > 75)
         best = UMAX(79, get_psuedo_level(ch));
   }
   else
      best = ch->level;

   if (best == -1)
   {
      send_to_char("You don't know of such a skill.\n\r", ch);
      return;
   }

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Trip whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (victim == ch)
   {
      send_to_char("That wouldn't even be funny.\n\r", ch);
      return;
   }

   if (!subtract_energy_cost(ch, gsn_trip))
      return;

   if (is_safe(ch, victim))
      return;

   if (victim->fighting == NULL)
   {
      send_to_char("You can't trip someone who isn't fighting.\n\r", ch);
      return;
   }

   check_killer(ch, victim);

   if (number_percent() < (IS_NPC(ch) ? 50 : 2 * ch->pcdata->learned[gsn_trip]))
   {
      check_killer(ch, victim);
      trip(ch, victim);
      WAIT_STATE(ch, skill_table[gsn_trip].beats);
   }

   return;
}

void do_beserk(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (IS_NPC(ch))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You can only go beserk when fighting!\n\r", ch);
      return;
   }

   if (is_affected(ch, gsn_beserk))
   {
      send_to_char("You are already beserk!!\n\r", ch);
      return;
   }

   if (!can_use_skill(ch, gsn_beserk))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_beserk].beats);

   if (!subtract_energy_cost(ch, gsn_beserk))
      return;

   raise_skill(ch, gsn_beserk);

   if (number_percent() < 25)
   {
      /*
       * Failure
       */
      act("$n calls on the Dark Powers, but they don't answer!", ch, NULL, NULL, TO_ROOM);
      send_to_char("You call on the Dark Powers.  They don't answer!", ch);
      return;
   }

   af.type = gsn_beserk;
   af.duration = -1;
   af.location = APPLY_AC;
   af.modifier = ch->class_level[CLASS_WAR] / 2;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   af.location = APPLY_HITROLL;
   af.modifier = ch->class_level[CLASS_WAR] / 5;
   affect_to_char(ch, &af);

   af.location = APPLY_DAMROLL;
   af.modifier = ch->class_level[CLASS_WAR] / 5;
   affect_to_char(ch, &af);

   af.location = APPLY_SAVING_SPELL;
   af.modifier = ch->class_level[CLASS_WAR] / -10;
   affect_to_char(ch, &af);

   act("$n calls on the Dark Powers, who answer!!!", ch, NULL, NULL, TO_ROOM);
   send_to_char("You call on the Dark Powers.  They answer!!!\n\r", ch);
   return;
}

void do_warcry(CHAR_DATA *ch, char *argument)
/* --Stephen
 * FIXME: make this a warrior skill, check for bless.
 */
{
   AFFECT_DATA af;
   int level;

   if (IS_NPC(ch))
      return;

   level = ch->class_level[CLASS_WAR];

   if (!is_fighting(ch))
   {
      send_to_char("You can only give a warcry when fighting!\n\r", ch);
      return;
   }

   if (is_affected(ch, gsn_warcry))
   {
      send_to_char("You already have given a warcry!!\n\r", ch);
      return;
   }

   if (!can_use_skill(ch, gsn_warcry))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_warcry].beats);

   raise_skill(ch, gsn_warcry);

   af.type = gsn_warcry;
   af.duration = -1;
   af.location = APPLY_HITROLL;
   af.modifier = level / 16;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   af.location = APPLY_SAVING_SPELL;
   af.modifier = 0 - level / 16;
   affect_to_char(ch, &af);

   send_to_char("You scream a great warcry!\n\r", ch);
   act("$n screams a great warcry!", ch, NULL, NULL, TO_ROOM);
}

void do_detox(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;

   if (!can_use_skill_message(ch, gsn_detox))
      return;

   if (((victim = get_char_room(ch, argument)) == NULL) && !is_fighting(ch))
   {
      send_to_char("No such victim!\n\r", ch);
      return;
   }

   if (IS_NPC(victim) && IS_SET(victim->act, ACT_NO_BODY))
   {
      act("$N doesn't have a definable body to detox!", ch, NULL, victim, TO_CHAR);
      return;
   }

   if (is_affected(victim, gsn_poison))
   {
      affect_strip(victim, gsn_poison);
      act("$n detoxes $N, and $N looks better.", ch, NULL, victim, TO_NOTVICT);
      act("$n detoxes you, and you feel better.", ch, NULL, victim, TO_VICT);
      act("You detox $N, and $N looks noticably better.", ch, NULL, victim, TO_CHAR);
   }
   else
      act("$N is not poisoned!", ch, NULL, victim, TO_CHAR);
}

void do_morale(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;
   CHAR_DATA *gch;

   int stat = ch->class_level[CLASS_KNI];

   if (ch->class_level[CLASS_PAL] > stat)
      stat = ch->class_level[CLASS_PAL];

   if (!can_use_skill_message(ch, gsn_morale))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to do this!\n\r", ch);
      return;
   }

   raise_skill(ch, gsn_morale);

   for (gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room)
   {
      if (is_affected(gch, gsn_morale) || !is_same_group(ch, gch))
         continue;

      act("$n seems much more willing to fight.", gch, NULL, NULL, TO_ROOM);
      send_to_char("You are inspired to fight better!\n\r", gch);
      af.type = gsn_morale;
      af.duration_type = DURATION_ROUND;
      af.duration = 50;
      af.location = APPLY_DAMROLL;
      af.modifier = stat / 2;
      af.bitvector = 0;
      affect_to_char(gch, &af);
   }
   send_to_char("You inspire the troops!\n\r", ch);
}

void do_leadership(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;
   CHAR_DATA *gch;

   int stat = ch->class_level[CLASS_KNI];

   if (ch->class_level[CLASS_PAL] > stat)
      stat = ch->class_level[CLASS_PAL];

   if (!can_use_skill_message(ch, gsn_leadership))
      return;

   if (!is_fighting(ch))
   {
      send_to_char("You must be fighting to do this!\n\r", ch);
      return;
   }

   raise_skill(ch, gsn_leadership);

   for (gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room)
   {
      if (is_affected(gch, gsn_leadership) || !is_same_group(ch, gch))
         continue;

      act("$n looks more courageous!", gch, NULL, NULL, TO_ROOM);
      send_to_char("You feel courage wash over you!\n\r", gch);
      af.type = gsn_leadership;
      af.duration_type = DURATION_ROUND;
      af.duration = 50;
      af.location = APPLY_HITROLL;
      af.modifier = stat / 2;
      af.bitvector = 0;
      affect_to_char(gch, &af);
   }
   send_to_char("You inspire the troops!\n\r", ch);
}

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

void do_target(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   if ((!IS_NPC(ch)) && (ch->pcdata->learned[gsn_target] < 65))
   {
      send_to_char("You are not trained enough in this skill!!\n\r", ch);
      return;
   }
   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Target whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (!IS_NPC(victim) && !(deathmatch))
   {
      if (!IS_SET(victim->act, PLR_KILLER) && !IS_SET(victim->act, PLR_THIEF))
      {
         send_to_char("You must MURDER a player.\n\r", ch);
         return;
      }
   }
   else
   {
      if (IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL)
      {
         send_to_char("You must MURDER a charmed creature.\n\r", ch);
         return;
      }
   }

   if (victim == ch)
   {
      send_to_char("You hit yourself.  Ouch!\n\r", ch);
      one_hit(ch, ch, TYPE_UNDEFINED);
      return;
   }

   if (is_safe(ch, victim))
      return;

   if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
   {
      act("$N is your beloved master.", ch, NULL, victim, TO_CHAR);
      return;
   }

   if (is_fighting(ch))
   {
      send_to_char("@@rTracking, tracking, tracking...@@eGOT HIM!!!@@N\n\r", ch);
      stop_fighting(ch, FALSE);
   }

   WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
   check_killer(ch, victim);
   one_hit(ch, victim, TYPE_UNDEFINED);
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
