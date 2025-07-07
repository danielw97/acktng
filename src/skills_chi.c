#include "globals.h"
#include "magic.h"

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

    if (!can_use_skill(ch, gsn_chiblock))
    {
        send_to_char("You don't know how to use this skill!\n\r", ch);
        return;
    }

    if (!is_valid_finisher(ch))
    {
        send_to_char("You are not prepared for a finisher!\n\r", ch);
        return;
    }

    reset_combo(ch);

    raise_skill(ch, gsn_chiblock);

    WAIT_STATE(ch, skill_table[gsn_chiblock].beats);

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

    if (!can_use_skill(ch, gsn_chakra))
    {
        send_to_char("You don't know how to use this skill!\n\r", ch);
        return;
    }

    if (ch->cooldown[gsn_chakra] > 0)
    {
        send_to_char("Chakra is on cooldown!\n\r", ch);
        return;
    }

    if (!is_valid_finisher(ch))
    {
        send_to_char("You are not prepared for a finisher!\n\r", ch);
        return;
    }

    reset_combo(ch);

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