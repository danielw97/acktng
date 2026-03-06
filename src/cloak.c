#ifndef DEC_ACK_H
#include "globals.h"
#endif
#include "magic.h"
#include "cloak.h"

bool cloak_reactive_can_trigger(int element)
{
    return !IS_SET(element, NO_REFLECT);
}

int cloak_adept_hitroll_bonus(CHAR_DATA *ch)
{
    if (IS_AFFECTED(ch, AFF_CLOAK_ADEPT))
    {
        return get_psuedo_level(ch) * 2;
    }

    return 0;
}

int cloak_adept_defense_bonus(CHAR_DATA *ch)
{
    if (IS_AFFECTED(ch, AFF_CLOAK_ADEPT))
        return 5;

    return 0;
}

int cloak_adept_scale_damage(CHAR_DATA *ch, int dam)
{
    if (IS_AFFECTED(ch, AFF_CLOAK_ADEPT))
        dam *= 1.2;

    return dam;
}

#ifndef UNIT_TEST_CLOAK
bool cloak_handle_spell_defense(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int element, int dam)
{
    if ((skill_table[dt].target == TAR_CHAR_OFFENSIVE) && (IS_AFFECTED(victim, AFF_CLOAK_REFLECTION)) && (ch != victim) && (number_percent() < (get_psuedo_level(victim) - 70)))
    {

        act("@@N$n's @@lc@@el@@ro@@ya@@ak@@N glows brightly as $Nn's spell hits it, and the spell is reflected@@N!!", ch,
            victim, NULL, TO_ROOM);
        act("@@N$N's @@lc@@el@@ro@@ya@@ak@@N glows brightly, and reflects your spell back on you@@N!!", ch, NULL, victim,
            TO_CHAR);
        act("@@NYour @@lc@@el@@ro@@ya@@ak@@N glows brightly, and reflects the spell back on $N@@N!!!", victim, NULL, ch,
            TO_CHAR);
        sp_damage(NULL, victim, ch, dam, element, dt, TRUE);
        return TRUE;
    }

    if ((skill_table[dt].target == TAR_CHAR_OFFENSIVE) && (IS_AFFECTED(victim, AFF_CLOAK_ABSORPTION)) && (ch != victim) && (number_percent() < (get_psuedo_level(victim) - 55)))
    {
        int mana;
        mana = mana_cost(ch, dt);
        victim->mana = UMIN(get_max_mana(victim), victim->mana + mana);

        act("@@N$n's @@lcloak@@N glows brightly as $N's spell hits it, then fades@@N!!", victim, NULL, ch, TO_ROOM);
        act("@@N$N's @@lcloak@@N glows brightly, and absorbs your spell@@N!!", ch, NULL, victim, TO_CHAR);
        act("@@NYour @@lcloak@@N glows brightly, and absorbs $N's spell@@N!!!", victim, NULL, ch, TO_CHAR);
        return TRUE;
    }

    return FALSE;
}

int cloak_apply_reactive_effects(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int element, bool shortfight_round)
{
    int reactive_damage = 0;

    if (!IS_AFFECTED(victim, AFF_CLOAK_FLAMING) && !is_affected(victim, skill_lookup("cloak:elements"))
        && !is_affected(victim, skill_lookup("cloak:drain"))
        && !is_affected(victim, skill_lookup("cloak:misery")))
        return reactive_damage;

    if ((IS_AFFECTED(victim, AFF_CLOAK_FLAMING) || is_affected(victim, skill_lookup("cloak:elements"))) && (ch != victim))
    {
        int reflected_damage = 0;
        bool has_flaming = IS_AFFECTED(victim, AFF_CLOAK_FLAMING) ? TRUE : FALSE;
        bool has_elements = is_affected(victim, skill_lookup("cloak:elements"));

        if (has_flaming && IS_SET(element, ELE_PHYSICAL))
        {
            if (!shortfight_round)
            {
                act("@@N$n's @@ecloak@@N flares and engulfs $N in @@eflames@@N!!", victim, NULL, ch, TO_NOTVICT);
            }
            else
            {
                CHAR_DATA *rch;
                for (rch = ch->in_room->first_person; rch != NULL; rch = rch->next_in_room)
                {
                    char buf[MSL];
                    if (rch == ch || rch == victim)
                        continue;
                    if (!IS_NPC(rch) && IS_SET(rch->config, CONFIG_SHORT_FIGHT))
                        continue;
                    sprintf(buf, "@@N%s's @@ecloak@@N flares and engulfs %s in @@eflames@@N!!@@N\n\r", PERS(victim, rch), PERS(ch, rch));
                    send_to_char(buf, rch);
                }
            }

            if (!(shortfight_round && !IS_NPC(ch) && IS_SET(ch->config, CONFIG_SHORT_FIGHT)))
            {
                if (!IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_BLIND_PLAYER))
                    act("Flame cloak on $K ouch", ch, NULL, victim, TO_CHAR);
                else
                    act("@@N$N's @@ecloak@@N erupts and burns you in retaliation!!", ch, NULL, victim, TO_CHAR);
            }

            if (!(shortfight_round && !IS_NPC(victim) && IS_SET(victim->config, CONFIG_SHORT_FIGHT)))
            {
                if (!IS_NPC(victim) && IS_SET(victim->pcdata->pflags, PFLAG_BLIND_PLAYER))
                    act("Your Flame cloak flares", victim, NULL, ch, TO_CHAR);
                else
                    act("@@NYour @@ecloak@@N erupts and scorches $N!!!", victim, NULL, ch, TO_CHAR);
            }

            reflected_damage = UMAX(1, dam * 15 / 100);
        }
        else if (has_elements && !IS_SET(element, ELE_PHYSICAL))
        {
            if (!shortfight_round)
            {
                act("@@N$n's @@lELEMENTS @@Ncloak crackles and lashes $N with raw power!!", victim, NULL, ch, TO_NOTVICT);
            }
            else
            {
                CHAR_DATA *rch;
                for (rch = ch->in_room->first_person; rch != NULL; rch = rch->next_in_room)
                {
                    char buf[MSL];
                    if (rch == ch || rch == victim)
                        continue;
                    if (!IS_NPC(rch) && IS_SET(rch->config, CONFIG_SHORT_FIGHT))
                        continue;
                    sprintf(buf, "@@N%s's @@lELEMENTS @@Ncloak crackles and lashes %s with raw power!!@@N\n\r", PERS(victim, rch), PERS(ch, rch));
                    send_to_char(buf, rch);
                }
            }

            if (!(shortfight_round && !IS_NPC(ch) && IS_SET(ch->config, CONFIG_SHORT_FIGHT)))
            {
                if (!IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_BLIND_PLAYER))
                    act("Element cloak on $K ouch", ch, NULL, victim, TO_CHAR);
                else
                    act("@@N$N's @@lELEMENTS @@Ncloak crackles and shocks you in retaliation!!", ch, NULL, victim, TO_CHAR);
            }

            if (!(shortfight_round && !IS_NPC(victim) && IS_SET(victim->config, CONFIG_SHORT_FIGHT)))
            {
                if (!IS_NPC(victim) && IS_SET(victim->pcdata->pflags, PFLAG_BLIND_PLAYER))
                    act("Your Elements cloak flares", victim, NULL, ch, TO_CHAR);
                else
                    act("@@NYour @@lELEMENTS @@Ncloak crackles and lashes back at $N!!!", victim, NULL, ch, TO_CHAR);
            }

            reflected_damage = UMAX(1, dam * 15 / 100);
        }

        if (reflected_damage > 0)
        {
            int reactive_element = has_flaming ? ELE_FIRE : ELEMENT_WATER;
            int applied_damage = do_damage(victim, ch, reflected_damage, -1,
                                           reactive_element | NO_REFLECT | NO_ABSORB, FALSE);
            if (applied_damage > 0)
                reactive_damage += applied_damage;
        }
    }

    if ((is_affected(victim, skill_lookup("cloak:misery"))) && (ch != victim) && (number_range(0, 99) < 35))
    {
        AFFECT_DATA af;
        if (!shortfight_round)
        {
            act("@@N$n's @@dcloak@@N flares and shrouds $N in @@dmisery@@N!!", victim, NULL, ch, TO_NOTVICT);
        }
        else
        {
            CHAR_DATA *rch;
            for (rch = ch->in_room->first_person; rch != NULL; rch = rch->next_in_room)
            {
                char buf[MSL];
                if (rch == ch || rch == victim)
                    continue;
                if (!IS_NPC(rch) && IS_SET(rch->config, CONFIG_SHORT_FIGHT))
                    continue;
                sprintf(buf, "@@N%s's @@dcloak@@N flares and shrouds %s in @@dmisery@@N!!\n\r", PERS(victim, rch), PERS(ch, rch));
                send_to_char(buf, rch);
            }
        }

        if (!(shortfight_round && !IS_NPC(ch) && IS_SET(ch->config, CONFIG_SHORT_FIGHT)))
        {
            if (!IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_BLIND_PLAYER))
                act("Flame cloak on $K ouch", ch, NULL, victim, TO_CHAR);
            else
                act("@@N$N's @@dcloak@@N flares, and shrouds you with @@dmisery@@N!!", ch, NULL, victim, TO_CHAR);
        }

        if (!(shortfight_round && !IS_NPC(victim) && IS_SET(victim->config, CONFIG_SHORT_FIGHT)))
        {
            if (!IS_NPC(victim) && IS_SET(victim->pcdata->pflags, PFLAG_BLIND_PLAYER))
                act("Your Death cloak flares", victim, NULL, ch, TO_CHAR);
            else
                act("@@NYour @@dcloak@@N flares, and shrouds $N with @@dmisery@@N!!!", victim, NULL, ch, TO_CHAR);
        }
        af.type = skill_lookup("curse");
        af.duration = 1;
        af.location = APPLY_HITROLL;
        af.modifier = -1;
        af.bitvector = AFF_CURSE;
        affect_join(ch, &af);

        af.location = APPLY_DAMROLL;
        af.modifier = -1;
        affect_join(ch, &af);

        af.location = APPLY_AC;
        af.modifier = -5;
        affect_join(ch, &af);
    }

    if ((is_affected(victim, skill_lookup("cloak:drain"))) && (ch != victim) && (number_range(0, 99) < 35))
    {
        int drain_damage;

        if (!shortfight_round)
        {
            act("@@N$n's @@rcloak@@N flares and drains life from $N!!", victim, NULL, ch, TO_NOTVICT);
        }
        else
        {
            CHAR_DATA *rch;
            for (rch = ch->in_room->first_person; rch != NULL; rch = rch->next_in_room)
            {
                char buf[MSL];
                if (rch == ch || rch == victim)
                    continue;
                if (!IS_NPC(rch) && IS_SET(rch->config, CONFIG_SHORT_FIGHT))
                    continue;
                sprintf(buf, "@@N%s's @@rcloak@@N flares and drains life from %s!!@@N\n\r", PERS(victim, rch), PERS(ch, rch));
                send_to_char(buf, rch);
            }
        }

        if (!(shortfight_round && !IS_NPC(ch) && IS_SET(ch->config, CONFIG_SHORT_FIGHT)))
        {
            if (!IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_BLIND_PLAYER))
                act("Flame cloak on $K ouch", ch, NULL, victim, TO_CHAR);
            else
                act("@@N$N's @@rcloak@@N flares, and drains your life!!", ch, NULL, victim, TO_CHAR);
        }

        if (!(shortfight_round && !IS_NPC(victim) && IS_SET(victim->config, CONFIG_SHORT_FIGHT)))
        {
            if (!IS_NPC(victim) && IS_SET(victim->pcdata->pflags, PFLAG_BLIND_PLAYER))
                act("Your Death cloak flares", victim, NULL, ch, TO_CHAR);
            else
                act("@@NYour @@rcloak@@N flares, draining life from $N!!!", victim, NULL, ch, TO_CHAR);
        }

        drain_damage = cloak_drain_damage_from_level(get_psuedo_level(victim));
        ch->hit = cloak_drain_attacker_hp_after_hit(ch->hit, drain_damage);
        victim->hit = cloak_drain_victim_hp_after_hit(victim->hit, get_max_hp(victim), drain_damage);
        reactive_damage += drain_damage;
    }


    return reactive_damage;
}
#endif

int cloak_drain_damage_from_level(int pseudo_level)
{
    if (pseudo_level < 0)
        return 0;

    return pseudo_level;
}

int cloak_level_damage_reduction(int pseudo_level)
{
    if (pseudo_level < 0)
        return 0;

    return pseudo_level / 2;
}

int cloak_oathbreaker_avoidance_chance(int pseudo_level)
{
    if (pseudo_level < 0)
        return 0;

    return pseudo_level / 10;
}

int cloak_transcendence_avoidance_chance(int pseudo_level)
{
    if (pseudo_level < 0)
        return 0;

    return pseudo_level / 15;
}

bool cloak_oathbreaker_avoids_spell_damage(int dam, int element, bool has_cloak, int pseudo_level, int roll_percent)
{
    if (!has_cloak || dam <= 0 || IS_SET(element, ELE_PHYSICAL))
        return FALSE;

    return roll_percent < cloak_oathbreaker_avoidance_chance(pseudo_level);
}

bool cloak_transcendence_avoids_physical_hit(int dam, int element, int dt, bool has_cloak, int pseudo_level, int roll_percent)
{
    if (!has_cloak || dam <= 0 || !IS_SET(element, ELE_PHYSICAL))
        return FALSE;

    return roll_percent < cloak_transcendence_avoidance_chance(pseudo_level);
}

long cloak_drain_attacker_hp_after_hit(long attacker_hp, int drain_damage)
{
    long next_hp;

    next_hp = attacker_hp - drain_damage;
    if (next_hp < 10)
        return 10;

    return next_hp;
}

long cloak_drain_victim_hp_after_hit(long victim_hp, long victim_max_hp, int drain_damage)
{
    long heal_amount;
    long next_hp;

    heal_amount = drain_damage / 2;
    next_hp = victim_hp + heal_amount;

    if (next_hp > victim_max_hp)
        return victim_max_hp;

    return next_hp;
}
