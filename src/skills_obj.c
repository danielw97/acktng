#include "globals.h"
#include "magic.h"

void disarm(CHAR_DATA *ch, CHAR_DATA *victim);

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

void do_disarm(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument(argument, arg);

    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch, arg);

    if (victim == NULL)
    {
        send_to_char("No such target to disarm!\n\r", ch);
        return;
    }

    disarm(ch, victim);
}

void disarm(CHAR_DATA *ch, CHAR_DATA *victim)
{
    OBJ_DATA *obj = NULL;
    AFFECT_DATA af;
    int chance;

    if (!can_use_skill_message(ch, gsn_disarm))
        return;

    if (IS_SET(victim->skills, MOB_DISARM))
    {
        act("$N cannot be disarmed!", ch, NULL, victim, TO_CHAR);
        return;
    }

    set_fighting(ch, victim, TRUE);
    if (obj == NULL)
    {
        if (((obj = get_eq_char(victim, WEAR_HOLD_HAND_L)) == NULL) || (obj->item_type != ITEM_WEAPON))
        {
            if (((obj = get_eq_char(victim, WEAR_HOLD_HAND_R)) == NULL) || (obj->item_type != ITEM_WEAPON))
            {
                if (!IS_SET(obj->extra_flags, ITEM_FIST))
                {
                    send_to_char("Your opponent is not wielding a weapon.\n\r", ch);
                    return;
                }
            }
        }
    }

    if (!IS_NPC(victim) && IS_WOLF(victim) && (IS_SHIFTED(victim) || IS_RAGED(victim)))
    {
        send_to_char("How do you expect to disarm a werewolf of their claws?!?\n\r", ch);
        return;
    }

    chance = 0;

    if (IS_SET(obj->extra_flags, ITEM_NODISARM))
        chance -= 80;

    if (can_use_skill(victim, gsn_nodisarm))
        chance -= 50;

    chance += (get_psuedo_level(ch) - get_psuedo_level(victim)) / 2;

    raise_skill(ch, gsn_disarm);

    if (!skill_success(ch, victim, gsn_disarm, chance))
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
