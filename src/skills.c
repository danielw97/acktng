#include "globals.h"
#include "magic.h"

bool can_hit_skill(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);
bool do_poison(CHAR_DATA *ch, char *argument, int gsn);
bool combo(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);
bool is_valid_finisher(CHAR_DATA *ch);
bool is_ready_finisher(CHAR_DATA *ch);
void reset_combo(CHAR_DATA *ch);
int max_combo(CHAR_DATA *ch);
bool subtract_energy_cost(CHAR_DATA *ch, int gsn);
int get_energy_cost(CHAR_DATA *ch, int gsn);

void do_backstab(CHAR_DATA *ch, char *argument)
{
   char arg[MSL];

   if (!IS_NPC(ch) && IS_WOLF(ch) && (IS_SHIFTED(ch) || IS_RAGED(ch)))
   {
      send_to_char("You cannot do that while in this form.\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_backstab))
      return;

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Backstab whom?\n\r", ch);
      return;
   }

   backstab(ch, get_char_room(ch, arg), TRUE);
}

void do_circle(CHAR_DATA *ch, char *argument)
{
   char arg[MSL];
   if (!IS_NPC(ch) && IS_WOLF(ch) && (IS_SHIFTED(ch) || IS_RAGED(ch)))
   {
      send_to_char("You cannot do that while in this form.\n\r", ch);
      return;
   }

   if (!can_use_skill_message(ch, gsn_backstab))
      return;

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      strcpy(arg, "enemy");
   }

   backstab(ch, get_char_room(ch, arg), FALSE);
}

void backstab(CHAR_DATA *ch, CHAR_DATA *victim, bool backstab)
{
   OBJ_DATA *obj;
   int dam;

   if (victim == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if ((victim == NULL) || (victim->is_free != FALSE))
      return;

   if (IS_NPC(victim) && IS_SET(victim->act, ACT_NO_BODY))
   {
      act("$N has no body to backstab!", ch, NULL, victim, TO_CHAR);
      return;
   }

   if (!backstab && !can_hit_skill(victim, ch, gsn_circle))
   {
      act("$n tries to circle $N, but misses!", ch, NULL, victim, TO_NOTVICT);
      act("You try to circle $N, but miss!", ch, NULL, victim, TO_CHAR);
      act("$N tries to circle you, but misses!", victim, NULL, ch, TO_CHAR);
      WAIT_STATE(ch, skill_table[gsn_circle].beats);
      return;
   }

   if (victim == ch)
   {
      send_to_char("How can you sneak up on yourself?\n\r", ch);
      return;
   }

   if (is_safe(ch, victim))
      return;

   if ((obj = get_eq_char(ch, WEAR_HOLD_HAND_R)) == NULL || obj->value[3] != 11)
      if ((obj = get_eq_char(ch, WEAR_HOLD_HAND_L)) == NULL || obj->value[3] != 11)
      {
         send_to_char("You need to wield a piercing weapon.\n\r", ch);
         return;
      }

   if (backstab && victim->fighting != NULL)
   {
      send_to_char("You can't backstab a fighting person.\n\r", ch);
      return;
   }

   if (backstab)
   {
      if (!subtract_energy_cost(ch, gsn_backstab))
         return;
   }
   else
   {
      if (!subtract_energy_cost(ch, gsn_circle))
         return;
   }

   /*
    * Work out damage
    */
   dam = number_range(obj->value[1], obj->value[2]);
   dam += number_range(get_psuedo_level(ch) / 2, get_psuedo_level(ch) * 2) + get_damroll(ch) / 2;
   dam += dam * ch->lvl[CLASS_THI] / 100;
   dam += dam * ch->remort[CLASS_ASS] / 100;
   dam += dam * ch->remort[CLASS_WLK] / 100 * .75;
   check_killer(ch, victim);

   if (backstab && IS_NPC(victim) && IS_AFFECTED(victim, AFF_SANCTUARY) && (ch->remort[CLASS_ASS] > 0 || ch->remort[CLASS_WLK] > 0) && (number_percent() > 50))
   {
      send_to_char("Critical Success! Your advanced training has succeeded!\n\r", ch);
      act("The white aura around $n fades.", victim, NULL, NULL, TO_ROOM);
      send_to_char("The white aura around you fades.\n\r", victim);
      REMOVE_BIT(victim->affected_by, AFF_SANCTUARY);
   }

   char actbuf[MSL];
   sprintf(actbuf, "$n places $p into the back of $N!!");
   act(actbuf, ch, obj, victim, TO_NOTVICT);
   sprintf(actbuf, "You place $p into the back of $N!!");
   act(actbuf, ch, obj, victim, TO_CHAR);
   sprintf(actbuf, "$N places $p into your back!!");
   act(actbuf, victim, obj, ch, TO_CHAR);

   if (backstab)
      raise_skill(ch, gsn_backstab);
   else
      raise_skill(ch, gsn_circle);

   if (!IS_NPC(ch) && ch->lvl[CLASS_THI] > 0 && number_percent() == 1)
   {
      send_to_room("You hear a large CRACK!\n\r", ch->in_room);
      dam *= 2;
   }

   if (backstab)
   {
      if ((obj = get_eq_char(ch, WEAR_HOLD_HAND_R)) != NULL && obj->value[3] == 11)
      {
         if ((obj = get_eq_char(ch, WEAR_HOLD_HAND_L)) != NULL && obj->value[3] == 11)
         {
            if (IS_SET(stance_app[ch->stance].specials, STANCE_DUAL_BACKSTAB))
            {
               calculate_damage(ch, victim, number_range(dam * 0.95, dam * 1.05), gsn_backstab, ELE_PHYSICAL, TRUE);
            }
         }
      }
      if (!calculate_damage(ch, victim, dam, gsn_backstab, ELE_PHYSICAL, TRUE))
         return;
   }
   else
   {
      if (!calculate_damage(ch, victim, dam * 0.8, gsn_circle, ELE_PHYSICAL, TRUE))
         return;
   }

   if (backstab)
      WAIT_STATE(ch, skill_table[gsn_backstab].beats);
   else
      WAIT_STATE(ch, skill_table[gsn_circle].beats);

   dam = dam * 0.8;

   if (is_affected(victim, skill_lookup("poison:quinine")))
   {
      dam = number_range(dam * 1.1, dam * 1.25);
      sprintf(actbuf, "$N screams as the quinine in $M veins is consumed!");
      act(actbuf, ch, obj, victim, TO_NOTVICT);
      sprintf(actbuf, "$N screams as the quinine in $M veins is consumed!");
      act(actbuf, ch, obj, victim, TO_CHAR);
      sprintf(actbuf, "You scream as the quinine in your veins is consumed!");
      act(actbuf, ch, obj, victim, TO_VICT);
      calculate_damage(ch, victim, dam, gsn_poison_quinine, ELE_PHYSICAL | ELE_POISON, TRUE);
      affect_strip(victim, skill_lookup("poison:quinine"));
   }

   if (is_affected(victim, skill_lookup("poison:arsenic")))
   {
      dam = number_range(dam * 1.1, dam * 1.25);
      sprintf(actbuf, "$N screams as the arsenic in $M veins is consumed!");
      act(actbuf, ch, obj, victim, TO_NOTVICT);
      sprintf(actbuf, "$N screams as the arsenic in $M veins is consumed!");
      act(actbuf, ch, obj, victim, TO_CHAR);
      sprintf(actbuf, "You scream as the arsenic in your veins is consumed!");
      act(actbuf, ch, obj, victim, TO_VICT);
      calculate_damage(ch, victim, dam, gsn_poison_arsenic, ELE_PHYSICAL | ELE_POISON, TRUE);
      affect_strip(victim, skill_lookup("poison:arsenic"));
   }

   if (is_affected(victim, skill_lookup("poison:nightshade")))
   {
      dam = number_range(dam * 1.25, dam * 1.4);
      sprintf(actbuf, "$N screams as the nightshade in $M veins is consumed!");
      act(actbuf, ch, obj, victim, TO_NOTVICT);
      sprintf(actbuf, "$N screams as the nightshade in $M veins is consumed!");
      act(actbuf, ch, obj, victim, TO_CHAR);
      sprintf(actbuf, "You scream as the nightshade in your veins is consumed!");
      act(actbuf, ch, obj, victim, TO_VICT);
      calculate_damage(ch, victim, dam, gsn_poison_nightshade, ELE_PHYSICAL | ELE_POISON, TRUE);
      affect_strip(victim, skill_lookup("poison:nightshade"));
   }
}

void do_poison_quinine(CHAR_DATA *ch, char *argument)
{
   do_poison(ch, argument, gsn_poison_quinine);
}

void do_poison_arsenic(CHAR_DATA *ch, char *argument)
{
   do_poison(ch, argument, gsn_poison_arsenic);
}

void do_poison_nightshade(CHAR_DATA *ch, char *argument)
{
   do_poison(ch, argument, gsn_poison_nightshade);
}

bool do_poison(CHAR_DATA *ch, char *argument, int gsn)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   AFFECT_DATA af;

   if (!IS_NPC(ch) && IS_WOLF(ch) && (IS_SHIFTED(ch) || IS_RAGED(ch)))
   {
      send_to_char("You cannot do that while in this form.\n\r", ch);
      return FALSE;
   }

   if (!can_use_skill_message(ch, gsn))
      return FALSE;

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Poison whom?\n\r", ch);
      return FALSE;
   }

   victim = get_char_room(ch, arg);

   if (victim == NULL)
   {
      send_to_char("Poison whom?\n\r", ch);
      return FALSE;
   }

   if (!subtract_energy_cost(ch, gsn))
      return FALSE;

   if (is_affected(ch, skill_table[gsn].name))
   {
      send_to_char("Your victim is already inflicted with that.\n\r", ch);
      return FALSE;
   }

   WAIT_STATE(ch, skill_table[gsn].beats);

   check_killer(ch, victim);
   set_fighting(victim, ch, TRUE);

   raise_skill(ch, gsn);

   if (!can_hit_skill(ch, victim, gsn))
   {
      sprintf(buf, "You attempt to inflict $N with %s, but fail.", skill_table[gsn].name);
      act(buf, ch, NULL, victim, TO_CHAR);
      sprintf(buf, "$n attempts to inflict you with %s, but fails.", skill_table[gsn].name);
      act(buf, ch, NULL, victim, TO_VICT);
      sprintf(buf, "$n attempts to inflict $N with %s, but fails.", skill_table[gsn].name);
      act(buf, ch, NULL, victim, TO_ROOM);
      return FALSE;
   }

   sprintf(buf, "You inflict $N with %s!", skill_table[gsn].name);
   act(buf, ch, NULL, victim, TO_CHAR);
   sprintf(buf, "$n inflictS you with %s!", skill_table[gsn].name);
   act(buf, ch, NULL, victim, TO_VICT);
   sprintf(buf, "$n inflictS $N with %s!", skill_table[gsn].name);
   act(buf, ch, NULL, victim, TO_ROOM);
   af.type = gsn;
   af.duration = 10;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_DOT;
   af.modifier = get_psuedo_level(ch) / 2;
   af.bitvector = 0;
   af.element = ELEMENT_PHYSICAL | ELEMENT_POISON;
   af.caster = ch;
   affect_to_char(victim, &af);
   return TRUE;
}

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
         if (ch->lvl[cnt] >= skill_table[gsn_rescue].skill_level[cnt] && ch->lvl[cnt] >= best)
            best = cnt;
   }
   else
      best = ch->level;

   if (best == -1)
   {
      send_to_char("You don't know how to rescue!!\n\r", ch);
      return;
   }

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
   if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_rescue])
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

void do_disarm(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int percent;
   int defense_chance = 0;

   if (!IS_NPC(ch) && IS_WOLF(ch) && (IS_SHIFTED(ch) || IS_RAGED(ch)))
   {
      send_to_char("You cannot do that while in this form.\n\r", ch);
      return;
   }

   if (!can_use_skill(ch, gsn_disarm))
   {
      send_to_char("You don't know how to disarm!\n\r", ch);
      return;
   }

   if ((!IS_WEAPON(get_eq_char(ch, WEAR_HOLD_HAND_L))) && (!IS_WEAPON(get_eq_char(ch, WEAR_HOLD_HAND_R))))
   {
      send_to_char("You must wield a weapon to disarm.\n\r", ch);
      return;
   }

   if ((victim = ch->fighting) == NULL)
   {
      send_to_char("You aren't fighting anyone.\n\r", ch);
      return;
   }
   if (!IS_NPC(victim) && IS_WOLF(victim) && (IS_SHIFTED(victim) || IS_RAGED(victim)))
   {
      send_to_char("Try slicing off their claws instead!\n\r", ch);
      return;
   }

   if (((obj = get_eq_char(victim, WEAR_HOLD_HAND_L)) == NULL) || (obj->item_type != ITEM_WEAPON))
      if (((obj = get_eq_char(victim, WEAR_HOLD_HAND_R)) == NULL) || (obj->item_type != ITEM_WEAPON))
      {
         send_to_char("Your opponent is not wielding a weapon.\n\r", ch);
         return;
      }
   if (IS_SET(obj->extra_flags, ITEM_NODISARM))
      defense_chance += 80;

   if (!IS_NPC(victim) && can_use_skill(victim, gsn_nodisarm))
      defense_chance += 50;

   if (!subtract_energy_cost(ch, gsn_disarm))
      return;

   combo(ch, victim, gsn_disarm);

   raise_skill(ch, gsn_disarm);

   WAIT_STATE(ch, skill_table[gsn_disarm].beats);
   percent = number_percent() + victim->level - ch->level;
   if (percent < defense_chance)
      disarm(ch, victim, obj);
   else
      send_to_char("You failed.\n\r", ch);
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
       * if ( ch->lvl[cnt] >= skill_table[gsn_trip].skill_level[cnt]
       * && ch->lvl[cnt] >= best )
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

void do_dirt(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   if (!can_use_skill_message(ch, gsn_dirt))
      return;

   one_argument(argument, arg);

   if (arg[0] == '\0' && ch->fighting == NULL)
   {
      send_to_char("Kick dirt at whom?\n\r", ch);
      return;
   }

   if ((victim = get_char_room(ch, arg)) == NULL && ch->fighting == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
      return;
   }

   if (!subtract_energy_cost(ch, gsn_dirt))
      return;

   if (victim == NULL)
      victim = ch->fighting;

   if (victim == ch)
   {
      send_to_char("That wouldn't be too smart, would it??.\n\r", ch);
      return;
   }

   if (is_safe(ch, victim))
   {
      send_to_char("They are safe!\n\r",ch);
      return;
   }


   if (IS_AFFECTED(victim, AFF_BLIND))
   {
      send_to_char("Your target is already blinded!\n\r",ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_dirt].beats);

   raise_skill(ch, gsn_dirt);

   if (!can_hit_skill(ch, victim, gsn_dirt))
   {
      act("You kick dirt at $M but miss!", ch, NULL, victim, TO_CHAR);
      act("$n kicks dirt at you but misses!", ch, NULL, victim, TO_VICT);
      act("$n kicks dirt at $N but misses!", ch, NULL, victim, TO_NOTVICT);
      return;
   }
   else
   {
      act("You kick dirt at $S eyes!", ch, NULL, victim, TO_CHAR);
      act("$n kicks dirt in your eyes!", ch, NULL, victim, TO_VICT);
      act("$n kicks dirt at $N's eyes!", ch, NULL, victim, TO_NOTVICT);

      af.type = skill_lookup("blindness");
      af.location = APPLY_HITROLL;
      af.modifier = -2;
      af.duration = 3;
      af.duration_type = DURATION_ROUND;
      af.bitvector = AFF_BLIND;
      affect_to_char(victim, &af);
   }

   set_fighting(victim,ch,TRUE);

   combo(ch, victim, gsn_dirt);
   return;
}

void do_beserk(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (IS_NPC(ch))
      return;

   if (ch->fighting == NULL)
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
   af.modifier = ch->lvl[CLASS_WAR] / 2;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   af.location = APPLY_HITROLL;
   af.modifier = ch->lvl[CLASS_WAR] / 5;
   affect_to_char(ch, &af);

   af.location = APPLY_DAMROLL;
   af.modifier = ch->lvl[CLASS_WAR] / 5;
   affect_to_char(ch, &af);

   af.location = APPLY_SAVING_SPELL;
   af.modifier = ch->lvl[CLASS_WAR] / -10;
   affect_to_char(ch, &af);

   act("$n calls on the Dark Powers, who answer!!!", ch, NULL, NULL, TO_ROOM);
   send_to_char("You call on the Dark Powers.  They answer!!!\n\r", ch);
   return;
}

void war_attack(CHAR_DATA *ch, char *argument, int gsn)
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   char actbuf[MAX_STRING_LENGTH];
   int dam;

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      strcpy(arg, "enemy");
   }

   if (!can_use_skill_message(ch, gsn))
      return;

   if (((victim = get_char_room(ch, argument)) == NULL) && ch->fighting == NULL)
   {
      send_to_char("No such victim!\n\r", ch);
      return;
   }

   if (victim == NULL)
      victim = ch->fighting;

   if (!subtract_energy_cost(ch, gsn))
      return;

   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 10, get_psuedo_level(ch) * get_curr_str(ch) / 5);

   if (dam < 1)
      dam = 1;

   dam += get_damroll(ch) / 2;

   if (ch->lvl[CLASS_WAR] > ch->lvl[CLASS_PUG])
      dam += dam * ch->lvl[CLASS_WAR] / 100;
   else
      dam += dam * ch->lvl[CLASS_PUG] / 100;

   if (skill_table[gsn].flag1 == REMORT || skill_table[gsn].flag1 == ADEPT)
   {
      if (ch->remort[CLASS_KNI] > 0)
         dam += dam * ch->remort[CLASS_KNI] / 100;
      else if (ch->remort[CLASS_SWO] > 0)
         dam += dam * ch->remort[CLASS_SWO] / 100;
      else if (ch->remort[CLASS_MON] > 0)
         dam += dam * ch->remort[CLASS_MON] / 100;
      else if (ch->remort[CLASS_PAL] > 0)
         dam += dam * ch->remort[CLASS_PAL] / 100 * .75;
      dam += dam * ch->adept[CLASS_CRU] / 50;
   }

   WAIT_STATE(ch, skill_table[gsn].beats);

   raise_skill(ch, gsn);

   int element = ELE_PHYSICAL;

   if (gsn == gsn_holystrike)
   {
      element = element | ELE_HOLY;
      dam *= 1.25;
   }

   if (gsn == gsn_fleche)
      dam *= 1.5;

   if (IS_NPC(ch))
      dam /= 2;

   check_killer(ch, victim);
   if (can_hit_skill(ch, victim, gsn))
   {
      char actbuf[MSL];
      sprintf(actbuf, "$n %ss $N!!", skill_table[gsn].name);
      act(actbuf, ch, NULL, victim, TO_NOTVICT);
      sprintf(actbuf, "$n %ss you really hard!!", skill_table[gsn].name);
      act(actbuf, ch, NULL, victim, TO_VICT);
      sprintf(actbuf, "You %s $N!!", skill_table[gsn].name);
      act(actbuf, ch, NULL, victim, TO_CHAR);

      calculate_damage(ch, victim, dam, gsn, element, TRUE);
      combo(ch, victim, gsn);
   }
   else
   {
      sprintf(actbuf, "$n tries to %s $N, but misses!", skill_table[gsn].name);
      act(actbuf, ch, NULL, victim, TO_NOTVICT);
      sprintf(actbuf, "$N tries to %s you, but misses!", skill_table[gsn].name);
      act(actbuf, ch, victim, NULL, TO_VICT);
      sprintf(actbuf, "You try to %s $N, but miss!", skill_table[gsn].name);
      act(actbuf, ch, NULL, victim, TO_CHAR);
      set_fighting(victim, ch, TRUE);
   }
}

void do_palmstrike(CHAR_DATA *ch, char *argument)
{
   war_attack(ch, argument, gsn_headbutt);
}

void do_headbutt(CHAR_DATA *ch, char *argument)
{
   war_attack(ch, argument, gsn_headbutt);
}

void do_charge(CHAR_DATA *ch, char *argument)
{
   war_attack(ch, argument, gsn_charge);
}

void do_kick(CHAR_DATA *ch, char *argument)
{
   war_attack(ch, argument, gsn_kick);
}

void do_punch(CHAR_DATA *ch, char *argument)
{
   war_attack(ch, argument, gsn_punch);
}

void do_holystrike(CHAR_DATA *ch, char *argument)
{
   if (!is_valid_finisher(ch))
   {
      send_to_char("You are not prepared for a finisher!\n\r", ch);
      return;
   }

   war_attack(ch, argument, gsn_holystrike);
}

void do_fleche(CHAR_DATA *ch, char *argument)
{
   if (!is_valid_finisher(ch))
   {
      send_to_char("You are not prepared for a finisher!n\r", ch);
      return;
   }

   war_attack(ch, argument, gsn_fleche);
}

void do_riposte(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (IS_NPC(ch))
      return;

   if (ch->fighting == NULL)
   {
      send_to_char("You can only prepare for a riposte when fighting!\n\r", ch);
      return;
   }

   if (is_affected(ch, gsn_riposte))
   {
      send_to_char("You already are prepared to perform a riposte!!\n\r", ch);
      return;
   }

   if (!can_use_skill(ch, gsn_riposte))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!is_valid_finisher(ch))
   {
      send_to_char("You are not prepared for a finisher!n\r", ch);
      return;
   }

   reset_combo(ch);

   raise_skill(ch, gsn_riposte);

   WAIT_STATE(ch, skill_table[gsn_riposte].beats);

   af.type = gsn_riposte;
   af.duration = 2;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_HITROLL;
   af.modifier = 1;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   send_to_char("You prepare for a riposte!\n\r", ch);
   act("$n prepares for a riposte!", ch, NULL, NULL, TO_ROOM);
}

void do_anti_magic_shell(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

   if (IS_NPC(ch))
      return;

   if (ch->fighting == NULL)
   {
      send_to_char("You can only create an anti-magic shell when fighting!\n\r", ch);
      return;
   }

   if (is_affected(ch, gsn_anti_magic_shell))
   {
      send_to_char("You already have an anti-magic shell!!\n\r", ch);
      return;
   }

   if (!can_use_skill(ch, gsn_anti_magic_shell))
   {
      send_to_char("You don't know how to use this skill!\n\r", ch);
      return;
   }

   if (!is_valid_finisher(ch))
   {
      send_to_char("You are not prepared for a finisher!n\r", ch);
      return;
   }

   reset_combo(ch);

   raise_skill(ch, gsn_anti_magic_shell);

   WAIT_STATE(ch, skill_table[gsn_anti_magic_shell].beats);

   af.type = gsn_anti_magic_shell;
   af.duration = 2;
   af.duration_type = DURATION_ROUND;
   af.location = APPLY_HITROLL;
   af.modifier = 1;
   af.bitvector = 0;
   affect_to_char(ch, &af);

   send_to_char("You surround yourself with an anti-magic shell!\n\r", ch);
   act("$n creates an anti-magic shell!", ch, NULL, NULL, TO_ROOM);
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

   level = ch->lvl[CLASS_WAR];

   if (ch->fighting == NULL)
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

void do_knee(CHAR_DATA *ch, char *argument)
{
   war_attack(ch, argument, gsn_knee);
}

void do_bash(CHAR_DATA *ch, char *argument)
{
   war_attack(ch, argument, gsn_bash);
}

void do_detox(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;

   if (!can_use_skill_message(ch, gsn_detox))
      return;

   if (((victim = get_char_room(ch, argument)) == NULL) && ch->fighting == NULL)
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

   if (!can_use_skill_message(ch, gsn_morale))
      return;

   raise_skill(ch, gsn_morale);

   for (gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room)
   {
      if (is_affected(gch, gsn_morale) || !is_same_group(ch, gch))
         continue;

      act("$n seems much more willing to fight.", gch, NULL, NULL, TO_ROOM);
      send_to_char("You are inspired to fight better!\n\r", gch);
      af.type = gsn_morale;
      if (ch == gch)
         af.duration = -1;
      else
         af.duration = 4 + (get_psuedo_level(ch) / 5);
      af.location = APPLY_DAMROLL;
      af.modifier = get_psuedo_level(ch) / 10;
      af.bitvector = 0;
      affect_to_char(gch, &af);
   }
   send_to_char("You inspire the troops!\n\r", ch);
}

void do_leadership(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;
   CHAR_DATA *gch;

   if (!can_use_skill_message(ch, gsn_leadership))
      return;

   raise_skill(ch, gsn_leadership);

   for (gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room)
   {
      if (is_affected(gch, gsn_leadership) || !is_same_group(ch, gch))
         continue;

      act("$n looks more courageous!", gch, NULL, NULL, TO_ROOM);
      send_to_char("You feel courage wash over you!\n\r", gch);
      af.type = gsn_leadership;
      if (ch == gch)
         af.duration = -1;
      else
         af.duration = 4 + (get_psuedo_level(ch) / 5);
      af.location = APPLY_HITROLL;
      af.modifier = get_psuedo_level(ch) / 10;
      af.bitvector = 0;
      affect_to_char(gch, &af);
   }
   send_to_char("You inspire the troops!\n\r", ch);
}

void disarm(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj)
{
   AFFECT_DATA af;
   int chance;

   if (!can_use_skill_message(ch, gsn_disarm))
      return;

   set_fighting(ch, victim, TRUE);
   if (obj == NULL)
   {
      if (((obj = get_eq_char(victim, WEAR_HOLD_HAND_L)) == NULL) || (obj->item_type != ITEM_WEAPON))
         if (((obj = get_eq_char(victim, WEAR_HOLD_HAND_R)) == NULL) || (obj->item_type != ITEM_WEAPON))
         {
            send_to_char("Your opponent is not wielding a weapon.\n\r", ch);
            return;
         }
   }
   if (IS_SET(obj->extra_flags, ITEM_NODISARM))
      return;

   if (!IS_NPC(victim) && IS_WOLF(victim) && (IS_SHIFTED(victim) || IS_RAGED(victim)))
      return;

   /*
    * Check to see if victim is warrior, and if they deflect the disarm
    * In part, this idea comes from HiddenWorlds,
    * but it's also pretty common sense really ;)
    * Stephen
    */

   chance = IS_NPC(victim) ? IS_SET(victim->skills, MOB_NODISARM) ? 90 : 0 : victim->pcdata->learned[gsn_nodisarm];

   raise_skill(ch, gsn_disarm);

   if (number_percent() < chance)
   {
      act("You dodge $n's disarm attempt!", ch, NULL, victim, TO_VICT);
      act("You fail to disarm $N!", ch, NULL, victim, TO_CHAR);
      act("$N dodges $n's disarm attempt!", ch, NULL, victim, TO_NOTVICT);
      return;
   }

   act("$n DISARMS you!", ch, NULL, victim, TO_VICT);
   act("You disarm $N!", ch, NULL, victim, TO_CHAR);
   act("$n DISARMS $N!", ch, NULL, victim, TO_NOTVICT);

   unequip_char(ch, obj);

   af.type = skill_lookup("disarm");
   af.location = APPLY_NONE;
   af.modifier = 0;
   af.duration_type = DURATION_ROUND;
   af.duration = 2;
   af.bitvector = 0;
   affect_to_char(victim, &af);
}

void trip(CHAR_DATA *ch, CHAR_DATA *victim)
{
   if (victim->wait == 0)
   {
      int chance;

      chance = IS_NPC(victim) ? IS_SET(victim->skills, MOB_NOTRIP) ? 75 : 0 : victim->pcdata->learned[gsn_notrip];

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

void do_frenzy(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *vch;
   CHAR_DATA *vch_next;
   int moves = 0;
   int damage = 0;

   if (IS_NPC(ch))
      return;

   if (!IS_NPC(ch) && ch->pcdata->learned[gsn_frenzy] == 0)
   {
      send_to_char("You are not trained in this skill!\n\r", ch);
      return;
   }

   if (!IS_NPC(ch) && ch->move < 100 && ch->position == POS_FIGHTING)
   {
      send_to_char("You're too tired to go into a frenzy!\n\r", ch);
      return;
   }

   if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_frenzy])
   {
      send_to_char("You try to go into a frenzy, but nearly cut your leg off!\n\r", ch);
      return;
   }

   if (((ch->move / 10) < 75) && ch->move >= 75)
      moves = 75;
   else
      moves = ch->move / 10;

   if (ch->hit > 200)
      damage = 20;

   WAIT_STATE(ch, skill_table[gsn_frenzy].beats);

   if (!IS_NPC(ch) && ch->position == POS_FIGHTING)
      ch->move -= moves;
   ch->hit -= damage;
   CREF(vch_next, CHAR_NEXTROOM);
   for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
   {
      vch_next = vch->next_in_room;
      if (vch->in_room == NULL)
         continue;

      if (vch->in_room == ch->in_room)
      {
         if (vch != ch && (vch->in_room == ch->in_room) && (IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch)) && (vch->master != ch) && (!is_same_group(ch, vch)))
         {
            act("$N is @@Wgored@@N by your FRENZY!!!\n\r", ch, NULL, vch, TO_CHAR);
            act("$n goes into a FRENZY, @@Wgoring@@N $N!!!", ch, NULL, vch, TO_ROOM);
            check_killer(ch, vch);
            one_hit(ch, vch, -1);
            continue;
         }
      }
   }
   CUREF(vch_next);

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

   if (ch->position == POS_FIGHTING)
   {
      send_to_char("@@rTracking, tracking, tracking...@@eGOT HIM!!!@@N\n\r", ch);
      stop_fighting(ch, FALSE);
   }

   WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
   check_killer(ch, victim);
   one_hit(ch, victim, TYPE_UNDEFINED);
   return;
}

void do_stun(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;

   if (IS_NPC(ch)) /* for now */
      return;

   if (can_use_skill_message(ch, gsn_stun))
   {
      return;
   }

   if (((victim = ch->fighting) == NULL) || (victim->in_room == NULL))
   {
      send_to_char("You must be fighting someone first!\n\r", ch);
      return;
   }
   stun(ch, victim);
}

void stun(CHAR_DATA *ch, CHAR_DATA *victim)
{
   if (ch == NULL || victim == NULL)
      return;

   /*if( ( number_percent(  ) + number_percent(  ) ) < ( chance + chance2 ) )
   {*/
   /*
    * Success
    */
   act("You slam into $N, leaving $M stunned.", ch, NULL, victim, TO_CHAR);
   act("$n slams into you, leaving you stunned.", ch, NULL, victim, TO_VICT);
   act("$n slams into $N, leaving $M stunned.", ch, NULL, victim, TO_NOTVICT);

   victim->stunTimer += number_range(2, 4);

   /*}
   else
   {
      *
       * Ooops!
       *
      act( "You try to slam into $N, but miss and fall onto your face!", ch, NULL, victim, TO_CHAR );
      act( "$n tries to slam into you, but misses and falls onto $s face!", ch, NULL, victim, TO_VICT );
      act( "$n tries to slam into $N, but misses and falls onto $s face!", ch, NULL, victim, TO_NOTVICT );
      return;
   }*/
   return;
}

bool can_hit_skill(CHAR_DATA *ch, CHAR_DATA *victim, int gsn)
{
   int chance = 0;

   if (gsn == gsn_poison_quinine || gsn == gsn_poison_arsenic)
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

   if (IS_NPC(ch))
      chance -= 20;

   if (!skill_success(ch, victim, gsn, chance))
      return FALSE;

   return TRUE;
}

bool combo(CHAR_DATA *ch, CHAR_DATA *victim, int gsn)
{
   int i;
   int max = max_combo(ch);
   int punch_cnt = 0, knee_cnt = 0, headbutt_cnt = 0, kick_cnt = 0, disarm_cnt = 0, dirt_cnt = 0, bash_cnt = 0, charge_cnt = 0, fleche_cnt = 0, holystrike_cnt = 0;
   int mult = 23;

   for (i = 0; i < max; i++)
   {
      /* Reverse index */
      ch->combo[max - i] = ch->combo[max - i - 1];
   }

   ch->combo[0] = gsn;

   if (!is_ready_finisher(ch))
      return FALSE;

   if (max == 5)
   {
      mult = 18;
   }

   if (max == 6)
   {
      mult = 15;
   }

   // Calc our chances
   for (int i = 0; i < max; i++)
   {
      if (ch->combo[i] == gsn_punch)
         punch_cnt += mult;

      if (ch->combo[i] == gsn_knee)
         knee_cnt += mult;

      if (ch->combo[i] == gsn_headbutt)
         headbutt_cnt += mult;

      if (ch->combo[i] == gsn_kick)
         kick_cnt += mult;

      if (ch->combo[i] == gsn_disarm)
         disarm_cnt += mult;

      if (ch->combo[i] == gsn_dirt)
         dirt_cnt += mult;

      if (ch->combo[i] == gsn_bash)
         bash_cnt += mult;

      if (ch->combo[i] == gsn_charge)
         charge_cnt += mult;

      if (ch->combo[i] == gsn_fleche)
         fleche_cnt += mult;

      if (ch->combo[i] == gsn_holystrike)
         holystrike_cnt += mult;
   }

   {
      send_to_char("@@yCombo triggered!@@N\n\r", ch);
      act("You begin a combo attack!", ch, NULL, victim, TO_CHAR);
      act("$n begins a combo attack!", ch, NULL, victim, TO_ROOM);

      int max_attacks = max - 3;

      if (ch->combo[0] == gsn_fleche)
         max_attacks++;

      int max_combo = 6;

      if (ch->remort[CLASS_KNI] > 0 || ch->remort[CLASS_SWO] > 0)
         max_combo += 2;

      if (ch->remort[CLASS_CRU] > 0)
         max_combo += 2;

      int combo_chance = 25;
      if (gsn == gsn_holystrike || gsn == gsn_fleche)
         combo_chance += 10;

      for (int i = 0; i < max_attacks; i++)
      {
         reset_combo(ch);

         if (ch->fighting == NULL)
            break;

         if (i > max_combo)
            break;

         if (i == 0 && number_percent() < combo_chance)
            max_attacks++;
         if (number_percent() < combo_chance)
            max_attacks++;

         int roll = number_percent();
         int chance = 0;

         if (roll < chance + punch_cnt)
         {
            do_punch(ch, victim->name);
            continue;
         }

         chance += punch_cnt;

         if (roll < chance + kick_cnt)
         {
            do_kick(ch, victim->name);
            continue;
         }

         chance += kick_cnt;

         if (roll < chance + knee_cnt)
         {
            do_knee(ch, victim->name);
            continue;
         }

         chance += knee_cnt;

         if (roll < chance + headbutt_cnt)
         {
            do_headbutt(ch, victim->name);
            continue;
         }

         chance += headbutt_cnt;

         if (roll < chance + disarm_cnt)
         {
            do_disarm(ch, victim->name);
            continue;
         }

         chance += disarm_cnt;

         if (roll < chance + dirt_cnt)
         {
            do_dirt(ch, victim->name);
            continue;
         }

         chance += dirt_cnt;

         if (roll < chance + bash_cnt)
         {
            do_bash(ch, victim->name);
            continue;
         }

         chance += bash_cnt;

         if (roll < chance + charge_cnt)
         {
            do_charge(ch, victim->name);
            continue;
         }

         chance += charge_cnt;

         if (roll < chance + fleche_cnt)
         {
            war_attack(ch, victim->name, gsn_fleche);
            continue;
         }

         chance += fleche_cnt;

         if (roll < chance + holystrike_cnt)
         {
            war_attack(ch, victim->name, gsn_holystrike);
            continue;
         }

         if (roll < 95)
         {
            stun(ch, ch->fighting);
            continue;
         }

         // 95+
         max_attacks += 2;
      }
   }

   reset_combo(ch);

   return TRUE;
}

bool is_valid_finisher(CHAR_DATA *ch)
{
   for (int i = 0; i < max_combo(ch) - 1; i++)
   {
      if (ch->combo[i] == -1)
         return FALSE;

      if (i > 0 && ch->combo[i - 1] == ch->combo[i])
         return FALSE;
   }

   return TRUE;
}

bool is_ready_finisher(CHAR_DATA *ch)
{
   for (int i = 0; i < max_combo(ch); i++)
   {
      if (ch->combo[i] == -1)
         return FALSE;

      if (i > 0 && ch->combo[i - 1] == ch->combo[i])
         return FALSE;
   }

   if (get_combo_count(ch) >= max_combo(ch)-1)
      return TRUE;

   return FALSE;
}

void reset_combo(CHAR_DATA *ch)
{
   for (int i = 0; i < max_combo(ch); i++)
      ch->combo[i] = -1;
}

int max_combo(CHAR_DATA *ch)
{
   int max = 4;

   if (ch->remort[CLASS_KNI] > 0 || ch->remort[CLASS_SWO] > 0)
      max = 5;

   if (ch->adept[CLASS_CRU] > 0)
      max = 6;

   return max;
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

void do_smash(CHAR_DATA *ch, char *argument)
{
   const sh_int rev_dir[] = {
       2, 3, 0, 1, 5, 4};
   char arg[MAX_INPUT_LENGTH];
   int door;
   bool joke; /* Was it unlocked to start with? */

   joke = FALSE;
   one_argument(argument, arg);

   if (!can_use_skill(ch, gsn_smash))
   {
      send_to_char("You don't know of such a skill.\n\r", ch);
      return;
   }

   if (arg[0] == '\0')
   {
      send_to_char("Smash what?\n\r", ch);
      return;
   }

   if ((door = find_door(ch, arg)) >= 0)
   {
      /*
       * 'open door'
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if (!IS_SET(pexit->exit_info, EX_CLOSED))
      {
         send_to_char("It's already open.\n\r", ch);
         return;
      }

      if (IS_SET(pexit->exit_info, EX_LOCKED))
         REMOVE_BIT(pexit->exit_info, EX_LOCKED);
      else
         joke = TRUE;

      REMOVE_BIT(pexit->exit_info, EX_CLOSED);
      act("$n smashes opens the $d.", ch, NULL, pexit->keyword, TO_ROOM);
      act("You smash open the $d.", ch, NULL, pexit->keyword, TO_CHAR);

      if (joke)
      {
         act("The $d was already unlocked!!!", ch, NULL, pexit->keyword, TO_ROOM);
         act("The $d was already unlocked!!!", ch, NULL, pexit->keyword, TO_CHAR);
         act("$n tumbles through $d!!!", ch, NULL, pexit->keyword, TO_ROOM);
         act("You tumble through $d!!!", ch, NULL, pexit->keyword, TO_CHAR);
      }

      /*
       * open the other side
       */
      if ((to_room = pexit->to_room) != NULL && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL && pexit_rev->to_room == ch->in_room)
      {
         CHAR_DATA *rch;

         if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
            REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);

         REMOVE_BIT(pexit_rev->exit_info, EX_CLOSED);
         for (rch = to_room->first_person; rch != NULL; rch = rch->next_in_room)
            act("The $d is smashed open.", rch, NULL, pexit_rev->keyword, TO_CHAR);

         if (joke)
         {
            char_from_room(ch);
            char_to_room(ch, to_room);
            act("$n tumbles into the room, head over heels!", ch, NULL, NULL, TO_ROOM);
            do_look(ch, "");
         }
      }
   }

   return;
}

void do_pick(CHAR_DATA *ch, char *argument)
{
   const sh_int rev_dir[] = {
       2, 3, 0, 1, 5, 4};
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *gch;
   OBJ_DATA *obj;
   int door;

   one_argument(argument, arg);

   if (arg[0] == '\0')
   {
      send_to_char("Pick what?\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn_pick_lock].beats);

   /*
    * look for guards
    */
   for (gch = ch->in_room->first_person; gch; gch = gch->next_in_room)
   {
      if (IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level)
      {
         act("$N is standing too close to the lock.", ch, NULL, gch, TO_CHAR);
         return;
      }
   }

   if (!can_use_skill(ch, gsn_pick_lock))
   {
      send_to_char("You failed.\n\r", ch);
      return;
   }

   if ((obj = get_obj_here(ch, arg)) != NULL)
   {
      /*
       * 'pick object'
       */
      if (obj->item_type != ITEM_CONTAINER)
      {
         send_to_char("That's not a container.\n\r", ch);
         return;
      }
      if (!IS_SET(obj->value[1], CONT_CLOSED))
      {
         send_to_char("It's not closed.\n\r", ch);
         return;
      }
      if (obj->value[2] < 0)
      {
         send_to_char("It can't be unlocked.\n\r", ch);
         return;
      }
      if (!IS_SET(obj->value[1], CONT_LOCKED))
      {
         send_to_char("It's already unlocked.\n\r", ch);
         return;
      }
      if (IS_SET(obj->value[1], CONT_PICKPROOF))
      {
         send_to_char("You failed.\n\r", ch);
         return;
      }

      REMOVE_BIT(obj->value[1], CONT_LOCKED);
      send_to_char("*Click*\n\r", ch);
      act("$n picks $p.", ch, obj, NULL, TO_ROOM);
      return;
   }

   if ((door = find_door(ch, arg)) >= 0)
   {
      /*
       * 'pick door'
       */
      ROOM_INDEX_DATA *to_room;
      EXIT_DATA *pexit;
      EXIT_DATA *pexit_rev;

      pexit = ch->in_room->exit[door];
      if (!IS_SET(pexit->exit_info, EX_CLOSED))
      {
         send_to_char("It's not closed.\n\r", ch);
         return;
      }
      if (pexit->key < 0)
      {
         send_to_char("It can't be picked.\n\r", ch);
         return;
      }
      if (!IS_SET(pexit->exit_info, EX_LOCKED))
      {
         send_to_char("It's already unlocked.\n\r", ch);
         return;
      }
      if (IS_SET(pexit->exit_info, EX_PICKPROOF))
      {
         send_to_char("You failed.\n\r", ch);
         return;
      }

      REMOVE_BIT(pexit->exit_info, EX_LOCKED);
      send_to_char("*Click*\n\r", ch);
      act("$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM);

      /*
       * pick the other side
       */
      if ((to_room = pexit->to_room) != NULL && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL && pexit_rev->to_room == ch->in_room)
      {
         REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
      }
   }

   return;
}

int get_combo_count(CHAR_DATA *ch)
{
   int cnt = 0;
   for(int i = 0; i < 6; i++)
   {
      if (i > 0 && ch->combo[i] == ch->combo[i-1])
         return cnt;
      if (ch->combo[i] > 0)
         cnt++;
   }

   return cnt;
}
