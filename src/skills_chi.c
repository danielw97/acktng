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

    int heal = class_heal_character(ch, ch, ch->remort[CLASS_MON] * base_heal, gsn_chakra, CLASS_MON, FALSE);

    heal_character(ch, ch, heal, gsn_chakra, FALSE);

    AFFECT_DATA af;

    af.type = gsn_chakra;
    af.duration = get_max_combo(ch) - 3;
    af.duration_type = DURATION_ROUND;
    af.location = APPLY_DAMROLL;
    af.modifier = ch->remort[CLASS_MON] * 5 + ch->adept[CLASS_MAR] * 5;
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