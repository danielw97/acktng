#include "globals.h"
#include "magic.h"

int get_chi(CHAR_DATA *ch)
{
    int max_chi = 10;

    if (ch->remort[CLASS_MON] > 0 || ch->remort[CLASS_BRA] > 0)
        max_chi = 15;

    if (ch->adept[CLASS_MAR] > 0)
        max_chi = 20;

    if (ch->chi > max_chi)
        ch->chi = max_chi;

    return ch->chi;
}

void do_chiblock(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;

    if (IS_NPC(ch))
        return;

    if (ch->fighting == NULL)
    {
        send_to_char("You can only prepare for a chi block when fightingg!\n\r", ch);
        return;
    }

    if (is_affected(ch, gsn_chiblock))
    {
        send_to_char("You already are prepared to perform a chi block!!\n\r", ch);
        return;
    }

    if (!can_use_skill_message(ch, gsn_chiblock))
        return;

    int cost = 5;
    if (ch->cooldown[gsn_chiblock] > 0)
        cost *= 2;

    if (ch->chi < cost)
    {
        send_to_char("You do not have sufficient chi to use chiblock.\n\r", ch);
        return;
    }

    ch->chi -= cost;

    raise_skill(ch, gsn_chiblock);

    WAIT_STATE(ch, skill_table[gsn_chiblock].beats);

    ch->cooldown[gsn_chiblock] = 10;

    af.type = gsn_chiblock;
    af.duration = get_max_combo(ch) - 3;
    af.duration_type = DURATION_ROUND;
    af.location = APPLY_AC;
    af.modifier = -1;
    af.bitvector = 0;
    affect_to_char(ch, &af);

    send_to_char("You prepare to block with your chi!\n\r", ch);
    act("$n prepares to block with their chi!", ch, NULL, NULL, TO_ROOM);
}

void do_chakra(CHAR_DATA *ch, char *argument)
{
    if (ch->fighting == NULL)
    {
        send_to_char("You can only prepare for a chakra when fighting!\n\r", ch);
        return;
    }

    if (!can_use_skill_message(ch, gsn_chakra))
        return;

    int cost = 5;
    if (ch->cooldown[gsn_chiblock] > 0)
        cost *= 2;

    if (ch->chi < cost)
    {
        send_to_char("You do not have sufficient chi to use chakra.\n\r", ch);
        return;
    }

    ch->chi -= cost;

    raise_skill(ch, gsn_chakra);

    WAIT_STATE(ch, skill_table[gsn_chakra].beats);

    act("$n focuses their chakra, and $n looks better.", ch, NULL, NULL, TO_ROOM);
    act("You focus your chakra, and you feel noticably better.", ch, NULL, NULL, TO_CHAR);

    ch->cooldown[gsn_chakra] = 10;

    int base_heal = 5;

    if (ch->adept[CLASS_MAR] > 0)
        base_heal = 7;

    int heal = class_heal_character(ch, ch, (ch->remort[CLASS_MON]+ch->adept[CLASS_MAR]) * base_heal, gsn_chakra, CLASS_MON, FALSE);

    heal_character(ch, ch, heal, gsn_chakra, FALSE);

    int dur = 1;
    if (ch->adept[CLASS_MAR] > 0)
       dur = 2;

    AFFECT_DATA af;

    af.type = gsn_chakra;
    af.duration = dur;
    af.duration_type = DURATION_ROUND;
    af.location = APPLY_DAMROLL;
    af.modifier = ch->remort[CLASS_MON] * 5 + ch->adept[CLASS_MAR] * 5;
    af.bitvector = 0;
    affect_to_char(ch, &af);
}

void do_phantomfist(CHAR_DATA *ch, char *argument)
{
    if (ch->fighting == NULL)
    {
        send_to_char("You can only prepare for a phantomfist when fighting!\n\r", ch);
        return;
    }

    if (!can_use_skill_message(ch, gsn_phantomfist))
        return;

    ch->chi += 2;

    raise_skill(ch, gsn_phantomfist);

    WAIT_STATE(ch, skill_table[gsn_phantomfist].beats);

    act("$n focuses their chi, and begins to move like a phantom.", ch, NULL, NULL, TO_ROOM);
    act("You focus your chi, and begin to move like a phantom.", ch, NULL, NULL, TO_CHAR);

    ch->cooldown[gsn_phantomfist] = 10;

    AFFECT_DATA af;

    int dur = 2;

    if (ch->adept[CLASS_MAR] > 0)
       dur = 3;

    af.type = gsn_phantomfist;
    af.duration = 2;
    af.duration_type = DURATION_ROUND;
    af.location = APPLY_SPEED;
    af.modifier = 3;
    af.bitvector = 0;
    affect_to_char(ch, &af);

}

void do_spinfist(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    if (!can_use_skill_message(ch, gsn_spinfist))
        return;

    int cost = 5;
    if (ch->cooldown[gsn_spinfist] > 0)
        cost *= 2;

    if (ch->chi < cost)
    {
        send_to_char("You do not have sufficient chi to use spinfist.\n\r", ch);
        return;
    }

    ch->cooldown[gsn_spinfist] = 10;

    act("You go into a FRENZY!!!\n\r", ch, NULL, NULL, TO_CHAR);
    act("$n goes into a FRENZY!!!", ch, NULL, NULL, TO_ROOM);
    for (vch = ch->in_room->first_person; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (vch->in_room == NULL)
            continue;

        if (vch->in_room == ch->in_room)
        {
            if (vch != ch && (vch->in_room == ch->in_room) && (IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch)) && (vch->master != ch) && (!is_same_group(ch, vch)))
            {
                if (can_hit_skill(ch, vch, gsn_spinfist))
                    war_attack(ch, vch->name, gsn_spinfist);
            }
        }
    }

    return;
}

void do_pummel(CHAR_DATA *ch, char *argument)
{
    pug_attack(ch, argument, gsn_pummel);
}

void do_aurabolt(CHAR_DATA *ch, char *argument)
{
    pug_attack(ch, argument, gsn_aurabolt);
}

void do_palmstrike(CHAR_DATA *ch, char *argument)
{
    pug_attack(ch, argument, gsn_palmstrike);
}

void pug_attack(CHAR_DATA *ch, char *argument, int gsn)
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

    dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 30, get_psuedo_level(ch) * get_curr_str(ch) / 20);

    if (dam < 1)
        dam = 1;

    if (gsn != gsn_spinfist)
       ch->chi++;

    float dam_mod = 0.0;

    dam_mod += ch->lvl[CLASS_PUG] / 200;

    if (skill_table[gsn].flag1 == REMORT || skill_table[gsn].flag1 == ADEPT)
    {
        dam_mod += ch->remort[CLASS_MON] / 200;
        dam_mod += ch->remort[CLASS_BRA] / 200 * 0.75;
        dam_mod += ch->adept[CLASS_MAR] / 100;
    }

    dam += dam * dam_mod;

    if (gsn == gsn_palmstrike)
    {
       dam *= 2;
       ch->cooldown[gsn_palmstrike] = 5;
    }

    WAIT_STATE(ch, skill_table[gsn].beats);

    raise_skill(ch, gsn);

    int element = ELE_PHYSICAL;

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

void do_mindoverbody(CHAR_DATA *ch, char *argument)
{
   AFFECT_DATA af;

    if (ch->fighting == NULL)
    {
        send_to_char("You can only perform mindoverbody when fighting!\n\r", ch);
        return;
    }

    if (!can_use_skill_message(ch, gsn_mindoverbody))
        return;

    if (!subtract_energy_cost(ch, gsn_mindoverbody))
        return;

   ch->chi++;

   ch->cooldown[gsn_mindoverbody] = 15;

   int base_heal = ch->lvl[CLASS_PUG];

   base_heal += ch->remort[CLASS_MON] * 2;
   base_heal += ch->remort[CLASS_BRA] * 2;
   base_heal += ch->adept[CLASS_MAR] * 5;

   if (is_affected(ch, gsn_mindoverbody) || is_affected(ch, skill_lookup("mindoverbody")))
      return FALSE;
   af.type = gsn_mindoverbody;
   af.duration = 3;
   af.location = APPLY_HOT;
   af.duration_type = DURATION_ROUND;
   af.modifier = base_heal;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char(ch, &af);
   act("$N begins to focus on mind over body.", ch, NULL, NULL, TO_ROOM);
   send_to_char("You begin to focus on mind over body.\n\r", ch);
   return TRUE;
}

void do_flurry(CHAR_DATA *ch, char *argument)
{
    char buf[MSL];
    if (ch->fighting == NULL)
    {
        send_to_char("You can only perform a flurry when fighting!\n\r", ch);
        return;
    }

    if (!can_use_skill_message(ch, gsn_flurry))
        return;

    if(ch->chi < 5)
    {
       sprintf(buf, "You must have at least 5 chi to initiate a flurry, you only have %s!\n\r", ch->chi);
       send_to_char(buf,ch);
       return;
    }

    raise_skill(ch, gsn_flurry);

    WAIT_STATE(ch, skill_table[gsn_flurry].beats);

    act("$n focuses their chi, and prepares for a flurry!", ch, NULL, NULL, TO_ROOM);
    act("You focus your chi, and prepare for a flurry!", ch, NULL, NULL, TO_CHAR);

    ch->cooldown[gsn_flurry] = 30;

    for(int i = 0; ch->chi > 0; ch->chi--)
    {
       one_hit(ch, ch->fighting, TYPE_HIT);
    }
}
