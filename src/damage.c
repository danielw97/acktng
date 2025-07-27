#ifndef DEC_ACK_H
#include "globals.h"
#endif
#include "magic.h"

#include <stdio.h>
#include <stdlib.h> /* For div_t, div() */
#include <string.h>
#include <time.h>
#include "tables.h"
#include <math.h>

extern POL_DATA politics_data;
extern CHAR_DATA *quest_target;
extern CHAR_DATA *quest_mob;

int calculate_damage(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int element, bool crit_possible)
{
    OBJ_DATA *wield;
    char buf[MSL];
    bool critical = FALSE;
    int crit_chance;
    float dam_modifier = 1.0;

    if (dt >= TYPE_HIT || dt < 0)
    {
        if ((dt > TYPE_HIT || (ch->remort[CLASS_MON] < 1 && ch->remort[CLASS_BRA] < 1)) && dam > 400)
        {
            dam -= 400;
            dam /= 3;
            dam += 300;
        }
        else if (dam > 200)
        {
            dam -= 200;
            dam /= 2;
            dam += 200;
        }
    }

    bool can_reflect = TRUE;
    bool can_absorb = TRUE;

    if (IS_SET(element, NO_REFLECT) || IS_SET(element, ELE_PHYSICAL))
    {
        REMOVE_BIT(element, NO_REFLECT);
        can_reflect = FALSE;
    }
    if (IS_SET(element, NO_ABSORB) || IS_SET(element, ELE_PHYSICAL))
    {
        REMOVE_BIT(element, NO_ABSORB);
        can_absorb = FALSE;
    }
    if (can_reflect && (skill_table[dt].target == TAR_CHAR_OFFENSIVE) && (IS_AFFECTED(victim, AFF_CLOAK_REFLECTION)) && (ch != victim) && (number_percent() < (get_psuedo_level(victim) - 70)))
    {

        act("@@N$n's @@lc@@el@@ro@@ya@@ak@@N glows brightly as $Nn's spell hits it, and the spell is reflected@@N!!", ch,
            victim, NULL, TO_ROOM);
        act("@@N$N's @@lc@@el@@ro@@ya@@ak@@N glows brightly, and reflects your spell back on you@@N!!", ch, NULL, victim,
            TO_CHAR);
        act("@@NYour @@lc@@el@@ro@@ya@@ak@@N glows brightly, and reflects the spell back on $N@@N!!!", victim, NULL, ch,
            TO_CHAR);
        sp_damage(NULL, victim, ch, dam, element, dt, TRUE);
        return FALSE;
    }

    if (can_reflect && (skill_table[dt].target == TAR_CHAR_OFFENSIVE) && (IS_AFFECTED(victim, AFF_CLOAK_ABSORPTION)) && (ch != victim) && (number_percent() < (get_psuedo_level(victim) - 55)))
    {
        int mana;
        mana = mana_cost(ch, dt);
        victim->mana = UMIN(victim->max_mana, victim->mana + mana);

        act("@@N$n's @@lcloak@@N glows brightly as $N's spell hits it, then fades@@N!!", victim, NULL, ch, TO_ROOM);
        act("@@N$N's @@lcloak@@N glows brightly, and absorbs your spell@@N!!", ch, NULL, victim, TO_CHAR);
        act("@@NYour @@lcloak@@N glows brightly, and absorbs $N's spell@@N!!!", victim, NULL, ch, TO_CHAR);
        return FALSE;
    }

    if (IS_SET(element, ELE_PHYSICAL))
        dam -= dam * get_curr_con(victim) / 100;
    else
        dam += dam * (get_curr_int(ch) - get_curr_wis(victim)) * 5 / 100;

    dam = scale_damage(ch, victim, element, dam, dt);

    if (!IS_SET(element, ELE_PHYSICAL))
    {
        if (IS_SET(element, SIXTH_DIVISOR))
        {
            dam += get_spellpower(ch) / 6;
            REMOVE_BIT(element, SIXTH_DIVISOR);
        }
        else if (IS_SET(element, FIFTH_DIVISOR))
        {
            dam += get_spellpower(ch) / 5;
            REMOVE_BIT(element, FIFTH_DIVISOR);
        }
        else if (IS_SET(element, FOURTH_DIVISOR))
        {
            dam += get_spellpower(ch) / 4;
            REMOVE_BIT(element, FOURTH_DIVISOR);
        }
        else if (IS_SET(element, THIRD_DIVISOR))
        {
            dam += get_spellpower(ch) / 3;
            REMOVE_BIT(element, THIRD_DIVISOR);
        }
        else if (IS_SET(element, SECOND_DIVISOR))
        {
            dam += get_spellpower(ch) / 2;
            REMOVE_BIT(element, SECOND_DIVISOR);
        }
        else
            dam += get_spellpower(ch);

        if (stance_app[ch->stance].spell_mod != 0)
            dam += dam * stance_app[ch->stance].spell_mod / 10;
    }
    else
       dam += get_damroll(ch) / 3;       

    if (!IS_SET(element, ELE_PHYSICAL))
        crit_chance = get_spell_crit(ch);
    else
        crit_chance = get_crit(ch);

    if (crit_possible && number_range(0, 100) < crit_chance)
        critical = TRUE;

    if (critical)
    {
        if (!IS_SET(element, ELE_PHYSICAL))
            dam += dam * get_spell_crit_mult(ch) / 100;
        else
            dam += dam * get_crit_mult(ch) / 100;
    }

    int skin_mods;
    if (!IS_NPC(victim))
        skin_mods = race_table[victim->race].race_flags;
    else
        skin_mods = victim->pIndexData->race_mods | race_table[victim->race].race_flags;

    if (IS_SET(element, ELE_PHYSICAL) && IS_SET(skin_mods, RACE_MOD_TOUGH_SKIN))
        dam = dam * 0.9;

    if (IS_SET(element, ELE_PHYSICAL) && IS_SET(skin_mods, RACE_MOD_STONE_SKIN))
        dam = dam * 0.8;

    if (IS_SET(element, ELE_PHYSICAL) && IS_SET(skin_mods, RACE_MOD_IRON_SKIN))
        dam = dam * 0.7;

    if (IS_SET(element, ELE_PHYSICAL) && (IS_AFFECTED(victim, AFF_SANCTUARY) || item_has_apply(victim, ITEM_APPLY_SANC)))
        dam /= 2;

    if (IS_EVIL(ch) && (IS_AFFECTED(victim, AFF_PROTECT) || item_has_apply(victim, ITEM_APPLY_PROT)))
        dam -= dam / 4;

    if (is_affected(victim, skill_lookup("feeble body")) && IS_SET(element, ELE_PHYSICAL))
        dam /= 2;

    if (is_affected(ch, skill_lookup("feeble mind")) && !IS_SET(element, ELE_PHYSICAL))
        dam /= 2;

    AFFECT_DATA *paf, *paf_next;

    for (paf = victim->first_affect; paf != NULL; paf = paf_next)
    {
        paf_next = paf->next;

        if (paf->type == gsn_cripple && IS_SET(element, ELE_PHYSICAL))
        {
            dam -= dam * paf->modifier / 100;
        }
    }

    for (paf = victim->first_affect; paf != NULL; paf = paf_next)
    {
        paf_next = paf->next;
        if (!IS_SET(element, ELE_PHYSICAL))
        {
            if (paf->type == gsn_anti_magic_shell)
            {
                dam = dam * 0.1;
                sprintf(buf, "$N's anti-magic shell negates %s.", skill_table[dt].name);
                act(buf, ch, NULL, victim, TO_ROOM);
                sprintf(buf, "Your anti-magic shell negates %s.", skill_table[dt].name);
                act(buf, ch, NULL, victim, TO_VICT);
                sprintf(buf, "$N's anti-magic shell absorbs your %s", skill_table[dt].name);
                act(buf, ch, NULL, victim, TO_CHAR);
            }

            if (paf->type == skill_lookup("refuge"))
            {
                // Templar
                if (paf->modifier == 2)
                    dam = dam * 0.75;
                else
                    dam = dam * 0.5;
            }
        }
        else
        {
            if (IS_SET(element, ELE_PHYSICAL))
            {
                OBJ_DATA *shield = get_eq_char(victim, WEAR_HOLD_HAND_L);
                if (shield == NULL || shield->item_type != ITEM_ARMOR)
                    shield = get_eq_char(victim, WEAR_HOLD_HAND_R);
                if (shield == NULL || shield->item_type != ITEM_ARMOR)
                    shield = get_eq_char(victim, WEAR_BUCKLER);

                if (paf->type == gsn_riposte && dt > 0 && dt < TYPE_HIT)
                {
                    ch->stunTimer = 1;
                    sprintf(buf, "$N's perfect riposte interrupts $n's %s.", skill_table[dt].name);
                    act(buf, ch, NULL, victim, TO_ROOM);
                    sprintf(buf, "Your perfect riposte interrupts $n's %s.", skill_table[dt].name);
                    act(buf, ch, NULL, victim, TO_VICT);
                    sprintf(buf, "$N's perfect riposte interrupts your %s", skill_table[dt].name);
                    act(buf, ch, NULL, victim, TO_CHAR);
                    calculate_damage(victim, ch, dam, gsn_riposte, ELEMENT_PHYSICAL, TRUE);
                    affect_strip(victim, gsn_riposte);
                    return;
                }

                if (paf->type == gsn_shieldblock && shield != NULL && shield->item_type == ITEM_ARMOR)
                {
                    sprintf(buf, "$N blocks $n's %s with $p.", skill_table[dt].name);
                    act(buf, ch, shield, victim, TO_ROOM);
                    sprintf(buf, "You block $n's %s with $p.", skill_table[dt].name);
                    act(buf, ch, shield, victim, TO_VICT);
                    sprintf(buf, "$N blocks your %s with $p", skill_table[dt].name);
                    act(buf, ch, shield, victim, TO_CHAR);
                    if (IS_SET(shield->extra_flags, ITEM_BUCKLER))
                        dam = dam * 0.15;
                    else
                        dam = dam * 0;
                }

                if (paf->type == gsn_chiblock)
                {
                    sprintf(buf, "$N blocks $n's %s with their chi.", skill_table[dt].name);
                    act(buf, ch, NULL, victim, TO_ROOM);
                    sprintf(buf, "You block $n's %s with your chi.", skill_table[dt].name);
                    send_to_char(buf, victim);
                    sprintf(buf, "$N blocks your %s with their chi", skill_table[dt].name);
                    act(buf, ch, NULL, victim, TO_CHAR);
                    dam = dam * 0.1;
                }
            }
        }
    }

    do_damage(ch, victim, dam, dt, element, critical);

    return dam;
}

int scale_damage(CHAR_DATA *ch, CHAR_DATA *victim, int element, int dam, int dt)
{
    OBJ_DATA *wield;
 
    if (IS_AFFECTED(ch, AFF_CLOAK_ADEPT))
        dam *= 1.2;
    if (dam <= 0)
        dam = 1;

    float dam_mod = 1.0;
    int ch_strong = (IS_NPC(ch) ? (((ch->race > 0) && (ch->race < MAX_RACE)) ? race_table[ch->race].strong_realms : ch->strong_magic) : race_table[ch->race].strong_realms);
    int ch_resist = (IS_NPC(ch) ? (((ch->race > 0) && (ch->race < MAX_RACE)) ? race_table[ch->race].resist_realms : ch->resist) : race_table[ch->race].resist_realms);
    int ch_weak = (IS_NPC(ch) ? (((ch->race > 0) && (ch->race < MAX_RACE)) ? race_table[ch->race].weak_realms : ch->weak_magic) : race_table[ch->race].weak_realms);
    int ch_suscept = (IS_NPC(ch) ? (((ch->race > 0) && (ch->race < MAX_RACE)) ? race_table[ch->race].suscept_realms : ch->suscept) : race_table[ch->race].suscept_realms);
    int ch_race = (IS_NPC(ch) ? (((ch->race > 0) && (ch->race < MAX_RACE)) ? race_table[ch->race].race_flags : ch->race_mods) : race_table[ch->race].race_flags);

    if (IS_SET(ch_strong, element))
        dam_mod += .25;
    else if (IS_SET(ch_weak, element))
        dam_mod -= .5;

    if (!IS_SET(element, ELE_PHYSICAL) && IS_SET(ch_race, RACE_MOD_STRONG_MAGIC))
        dam_mod += .25;
    else if (!IS_SET(element, ELE_PHYSICAL) && IS_SET(ch_race, RACE_MOD_WEAK_MAGIC))
        dam_mod -= .25;
    else if (!IS_SET(element, ELE_PHYSICAL) && IS_SET(ch_race, RACE_MOD_NO_MAGIC))
        dam_mod -= .50;

    if (!IS_SET(element, ELE_PHYSICAL) && is_affected(ch, skill_lookup("mystical focus")))
        dam_mod += .5;

    int vi_strong = (IS_NPC(victim) ? (((victim->race > 0) && (victim->race < MAX_RACE)) ? race_table[victim->race].strong_realms : victim->strong_magic) : race_table[victim->race].strong_realms);
    int vi_resist = (IS_NPC(victim) ? (((victim->race > 0) && (victim->race < MAX_RACE)) ? race_table[victim->race].resist_realms : victim->resist) : race_table[victim->race].resist_realms);
    int vi_weak = (IS_NPC(victim) ? (((victim->race > 0) && (victim->race < MAX_RACE)) ? race_table[victim->race].weak_realms : victim->weak_magic) : race_table[victim->race].weak_realms);
    int vi_suscept = (IS_NPC(victim) ? (((victim->race > 0) && (victim->race < MAX_RACE)) ? race_table[victim->race].suscept_realms : victim->suscept) : race_table[victim->race].suscept_realms);
    int vi_race = (IS_NPC(victim) ? (((victim->race > 0) && (victim->race < MAX_RACE)) ? race_table[victim->race].race_flags : victim->race_mods) : race_table[victim->race].race_flags);

    if (IS_SET(vi_suscept, element))
    {
        dam_mod += .5;
    }
    else if (IS_SET(vi_resist, element))
    {
        dam_mod -= .35;
    }

    else if (IS_SET(vi_race, RACE_MOD_NO_MAGIC) && element != ELE_PHYSICAL)
    {
        dam_mod -= .35;
    }

    if (!IS_NPC(ch))
    {
        if (IS_SET(element, ELEMENT_FIRE) || IS_SET(element, ELEMENT_AIR) || IS_SET(element, ELEMENT_WATER) ||
            IS_SET(element, ELEMENT_EARTH))
            dam_mod += ch->pcdata->adept_reincarnations[CLASS_GMA] / 50;

        if (IS_SET(element, ELEMENT_HOLY))
            dam_mod += ch->pcdata->adept_reincarnations[CLASS_TEM] / 50;

        if (IS_SET(element, ELEMENT_PHYSICAL))
            dam_mod += ch->pcdata->adept_reincarnations[CLASS_TEM] / 100;

        if (IS_SET(element, ELEMENT_SHADOW) || IS_SET(element, ELEMENT_POISON) || IS_SET(element, ELEMENT_MENTAL))
            dam_mod += ch->pcdata->adept_reincarnations[CLASS_KIN] / 50;

        if (dt >= TYPE_HIT || dt < 0)
            dam_mod += ch->pcdata->adept_reincarnations[CLASS_MAR] / 50;

        if (dt == gsn_circle || dt == gsn_backstab)
            dam_mod += ch->pcdata->adept_reincarnations[CLASS_NIG] / 50;

        if (dt == gsn_kick || dt == gsn_punch || dt == gsn_knee || dt == gsn_headbutt || dt == gsn_charge || dt == gsn_holystrike || dt == gsn_fleche)
            dam_mod += ch->pcdata->adept_reincarnations[CLASS_CRU] / 50;
    }

    if (get_eq_char(ch, WEAR_TWO_HANDED) != NULL)
        dam += dam * 0.2;

    if (dt == gsn_circle || dt == gsn_backstab)
    {
        dam_mod += ch->lvl[CLASS_THI] / 100;
        dam_mod += ch->remort[CLASS_ASS] / 100;       // 100% at 100
        dam_mod += ch->remort[CLASS_WLK] / 100 * .75; // 75% at 100
        dam_mod += ch->adept[CLASS_NIG] / 50;         // 40% at 20
    }

    if (dt == gsn_kick || dt == gsn_punch || dt == gsn_knee || dt == gsn_headbutt || dt == gsn_charge || dt == gsn_holystrike || dt == gsn_fleche)
    {
        dam_mod += ch->lvl[CLASS_WAR] / 100;

        if (skill_table[dt].flag1 == REMORT || skill_table[dt].flag1 == ADEPT)
        {
            dam_mod += ch->remort[CLASS_KNI] / 50;
            dam_mod += ch->remort[CLASS_SWO] / 50;
            dam_mod += ch->remort[CLASS_BRA] / 50 * 0.75;
            dam_mod += ch->adept[CLASS_CRU] / 20;
        }
    }

    if (dt == gsn_palmstrike || dt == gsn_pummel || dt == gsn_aurabolt)
    {
        dam_mod += ch->lvl[CLASS_PUG] / 200;

        if (skill_table[dt].flag1 == REMORT || skill_table[dt].flag1 == ADEPT)
        {
            dam_mod += ch->remort[CLASS_MON] / 200;
            dam_mod += ch->remort[CLASS_BRA] / 200 * 0.75;
            dam_mod += ch->adept[CLASS_MAR] / 40;
        }
    }

    if (IS_SET(element, ELE_PHYSICAL))
    {
        if (can_use_skill(ch, gsn_enhanced_damage))
            dam_mod += get_curr_str(ch) * 3 / 100;
        else if (IS_NPC(ch) && IS_SET(ch->skills, MOB_ENHANCED) || (item_has_apply(ch, ITEM_APPLY_ENHANCED)))
            dam_mod += 0.2;

        if (!IS_AWAKE(victim))
            dam_mod += 0.5;

        dam_mod += ch->remort[CLASS_PAL] / 100 * 0.4; // 100 * 0.4 = 40% at 100
        dam_mod += ch->adept[CLASS_TEM] / 100;        // 20% max at 20

        if ((dt == TYPE_HIT || dt == TYPE_MARTIAL || dt == gsn_counter) && can_use_skill(ch, gsn_bare_hand))
        {
            dam += dam * ch->remort[CLASS_BRA] / 100 * 0.75;
            dam += dam * ch->remort[CLASS_MON] / 100;
            dam += dam * ch->adept[CLASS_MAR] / 50;
        }

        wield = get_eq_char(ch, WEAR_HOLD_HAND_L);
        if (wield == NULL || wield->item_type != ITEM_WEAPON || wield->value[3] != 3)
            wield = get_eq_char(ch, WEAR_HOLD_HAND_R);
        if (wield == NULL || wield->item_type != ITEM_WEAPON || wield->value[3] != 3)
            wield = get_eq_char(ch, WEAR_TWO_HANDED);

        if (!IS_NPC(ch) && wield && wield->value[3] == 3 && can_use_skill(ch, gsn_enhanced_sword))
        {
            dam_mod += number_range(20, 40) / 100;
        }

        /*
         * extra damage from martial arts
         */
        if (dt == TYPE_MARTIAL)
            dam_mod += 1 / 3;
    }

    if (IS_SET(element, ELE_HOLY))
    {
        dam_mod += ch->adept[CLASS_TEM] / 50;
        dam_mod += ch->remort[CLASS_PRI] / 100;
    }

    if (!IS_SET(element, ELE_PHYSICAL) && (skill_table[dt].flag1 == REMORT || skill_table[dt].flag1 == ADEPT))
    {
        dam_mod += ch->remort[CLASS_SOR] / 100 * 0.5;
        dam_mod += ch->remort[CLASS_WIZ] / 100 * 0.5;
        dam_mod += ch->remort[CLASS_NEC] / 100 * 0.5;
        dam_mod += ch->remort[CLASS_EGO] / 100 * 0.5;
        dam_mod += ch->remort[CLASS_WLK] / 100 * .75 * 0.5;
        dam_mod += ch->adept[CLASS_GMA] * .05 * 0.5;
        dam_mod += ch->adept[CLASS_KIN] * .05 * 0.5;
    }

    if (can_use_skill(ch, gsn_potency) && !IS_SET(element, ELEMENT_PHYSICAL))
        dam_mod += get_curr_int(ch) * 2 / 100;

    if ((IS_SET(element, ELE_MENTAL)) && (!HAS_MIND(victim)))
        dam_mod = 0.0;

    if (((IS_SET(element, ELEMENT_EARTH)) || (IS_SET(element, ELEMENT_AIR))) && (!HAS_BODY(victim)))
        dam_mod = 0.0;

    if ((IS_SET(element, ELE_POISON)) && (IS_SET(vi_race, RACE_MOD_IMMUNE_POISON)))
        dam_mod = 0.0;

    if ((IS_SET(element, ELEMENT_SHADOW)) && (IS_UNDEAD(victim)))
        dam_mod = 0.0;

    dam = dam * dam_mod;

    return dam;
}

int do_damage(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int element, bool critical)
{
    OBJ_DATA *sil_weapon;
    int sn;
    int damcap = get_damcap(ch);

    if (victim->is_free == TRUE)
    {
        bug("Freed victim in one_hit", 0);
        return -1;
    }

    if (IS_NPC(ch))
        damcap *= 5;

    if (critical)
        damcap *= 2;

    if (dt < TYPE_HIT && dt >= 0)
        damcap *= 2;

    if (dam > damcap)
        dam = damcap;

    if (victim != ch)
    {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        if (is_safe(ch, victim))
            return;
        if (victim != ch->fighting)
            check_killer(ch, victim);

        if (victim->position > POS_STUNNED)
        {
            if (victim->fighting == NULL)
                set_fighting(victim, ch, FALSE);
            victim->position = POS_FIGHTING;
        }

        if (victim->position > POS_STUNNED)
        {
            if (ch->fighting == NULL)
            {
                set_fighting(ch, victim, TRUE);
                /*
                 * check_killer( ch, victim );
                 */
            }

            /*
             * If victim is charmed, ch might attack victim's master.
             */
            if (IS_NPC(ch) && IS_NPC(victim) && IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL && victim->master->in_room == ch->in_room && number_bits(3) == 0)
            {
                stop_fighting(ch, FALSE);
                multi_hit(ch, victim->master, TYPE_UNDEFINED);
                return -1;
            }
        }

        /*
         * More charm stuff.
         */
        if (victim->master == ch)
            stop_follower(victim);

        /*
         * Inviso attacks ... not.
         */
        if (IS_AFFECTED(ch, AFF_INVISIBLE))
        {
            affect_strip(ch, gsn_invis);
            affect_strip(ch, gsn_mass_invis);
            REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);
            act("$n shimmers into existence.", ch, NULL, NULL, TO_ROOM);
        }

        if (dam < 0)
            dam = 0;

        /*
         * Check for disarm, trip, parry, and dodge.
         */
        if (dt >= TYPE_HIT)
        {
            if (check_avoidance(ch, victim))
                return -1;
        }
    }

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    if (dt != -1 && IS_SET(element, ELE_PHYSICAL))
        dam_message(ch, victim, dam, dt, critical);
    else if (dt != -1)
        sp_dam_message(NULL, ch, victim, dam, element, dt, critical);
    victim->hit -= dam;
    if (!IS_NPC(victim))
        check_adrenaline(victim, dam);

    if (!IS_NPC(victim) && IS_WOLF(victim) && (dam > 350))
        do_rage(victim, "FORCE");

    if (IS_SET(element, ELE_PHYSICAL))
        cloak_action(ch, victim, dam);

    update_pos(victim);

    if ((IS_NPC(victim) || !IS_VAMP(victim)) && !(deathmatch))
    {
        switch (victim->position)
        {
        case POS_MORTAL:
            act("$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM);
            send_to_char("You are mortally wounded, and will die soon, if not aided.\n\r", victim);
            break;

        case POS_INCAP:
            act("$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM);
            send_to_char("You are incapacitated and will slowly die, if not aided.\n\r", victim);
            break;

        case POS_STUNNED:
            act("$n is too stunned to do anything!", victim, NULL, NULL, TO_ROOM);
            send_to_char("You are too stunned to do anything!\n\r", victim);
            break;

        case POS_DEAD:
            if ((sil_weapon = get_eq_char(ch, WEAR_HOLD_HAND_L)) == NULL)
                sil_weapon = get_eq_char(ch, WEAR_HOLD_HAND_R);
            /*            if (IS_WOLF(victim) && (!IS_NPC(ch)) && (ch->pcdata->learned[gsn_decapitate] != 0) && (sil_weapon != NULL) && (IS_SET(sil_weapon->extra_flags, ITEM_SILVER)))
                        {
                            int chance;

                            chance = IS_NPC(ch) ? ch->level * 2 : ch->pcdata->learned[gsn_decapitate];
                            chance += 25;

                            if ((victim->pcdata->vamp_level * 5) > ch->level)
                                chance -= (victim->pcdata->vamp_level * 5) - ch->level;

                            if (number_percent() < chance)
                            {
                                act("You DECAPITATE $N's head off with one skillful stroke!", ch, NULL, victim, TO_CHAR);
                                act("$n DECAPITATES $N's head off with one skillful stroke!", ch, NULL, victim, TO_NOTVICT);
                                send_to_char("You suddenly feel the world is spinning away from you!", victim);
                                send_to_char("You have been DECAPITATED!!", victim);
                                send_to_char("You feel the rage of the wolf flow from your body.....\n\r", victim);
                                send_to_char("You THINK you are dead!  Ooops....\n\r", victim);
                            }

                            REMOVE_BIT(victim->pcdata->pflags, PFLAG_WEREWOLF);
                            victim->pcdata->vamp_level = 0;
                            victim->pcdata->vamp_exp = 0;
                            victim->pcdata->bloodlust = 0;
                            victim->pcdata->bloodlust_max = 0;
                            victim->pcdata->generation = -1;
                            victim->pcdata->vamp_bloodline = 0;
                            victim->pcdata->recall_vnum = 3001;

                            for (sn = 0; sn <= MAX_SKILL; sn++)
                                if ((skill_table[sn].flag2 == WOLF) && (victim->pcdata->learned[sn] > 0))
                                    victim->pcdata->learned[sn] = 0;
                        }*/

            act("$n is DEAD!!", victim, 0, 0, TO_ROOM);
            send_to_char("You have been KILLED!!\n\r\n\r", victim);
            break;

        default:
            if (dam > victim->max_hit / 4)
                send_to_char("That really did HURT!\n\r", victim);
            if (victim->hit < victim->max_hit / 4)
                send_to_char("You sure are BLEEDING!\n\r", victim);
            break;
        }
    }

    if (!IS_AWAKE(victim))
        stop_fighting(victim, FALSE);

    if (victim->position == POS_DEAD && (IS_NPC(victim) || !IS_VAMP(victim) || (deathmatch)))
    {
        group_gain(ch, victim);

        if (!IS_NPC(ch))
        {
            if (!IS_NPC(victim))
                ch->pcdata->pkills++;
            else
                ch->pcdata->mkills++;
        }
        if (!IS_NPC(victim))
        {
            if (!IS_NPC(ch))
                victim->pcdata->pkilled++;
            else
                victim->pcdata->mkilled++;
        }

        if (IS_NPC(ch))
            raw_kill(victim, "");
        else
        {
            char name_buf[MAX_STRING_LENGTH];
            sprintf(name_buf, "%s", ch->name);
            raw_kill(victim, name_buf);
        }

        if (deathmatch && !IS_NPC(victim))
            do_quit(victim, "");

        if (IS_NPC(ch) && IS_NPC(victim) && IS_SET(ch->act, ACT_INTELLIGENT))
        {
            do_get(ch, "all corpse");
            do_sacrifice(ch, "corpse");
            check_re_equip(ch);
            check_rewield(ch);
        }

        if (!IS_NPC(ch) && IS_NPC(victim))
        {
            if (IS_SET(ch->config, CONFIG_AUTOMONEY))
                do_get(ch, "money corpse");
            if (IS_SET(ch->config, CONFIG_AUTOLOOT))
                do_get(ch, "all corpse");
            else
                do_look(ch, "in corpse");

            if (IS_SET(ch->config, CONFIG_AUTOSAC))
                do_sacrifice(ch, "corpse");
        }

        return dam;
    }

    if (victim == ch)
        return dam;

    /*
     * Take care of link dead people.
     */
    if (!IS_NPC(victim) && victim->desc == NULL)
    {
        if (number_range(0, victim->wait) == 0)
        {
            do_recall(victim, "");
            return dam;
        }
    }

    /*
     * Wimp out?
     */
    if (IS_NPC(victim) && dam > 0)
    {
        if ((IS_SET(victim->act, ACT_WIMPY) && number_bits(1) == 0 && victim->hit < victim->max_hit / 2) || (IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL && victim->master->in_room != victim->in_room))
            do_flee(victim, "");
    }

    if (!IS_NPC(victim) && victim->hit > 0 && victim->hit <= victim->wimpy && victim->wait == 0)
    {
        do_flee(victim, "");
    }

    tail_chain();
    return dam;
}

bool do_lifesteal(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *wield, bool dual, int dam)
{
    char buf[MAX_STRING_LENGTH];
    int chance = 10, potency = 20;

    chance += ch->lvl[CLASS_MAG] / 20;

    if (ch->remort[CLASS_SOR] > 0)
        chance += ch->remort[CLASS_SOR] / 10;
    else if (ch->remort[CLASS_WIZ] > 0)
        chance += ch->remort[CLASS_WIZ] / 10;

    potency += ch->lvl[CLASS_PSI] / 10;

    if (ch->remort[CLASS_NEC] > 0)
        potency += ch->remort[CLASS_NEC] / 10;
    else if (ch->remort[CLASS_EGO] > 0)
        potency += ch->remort[CLASS_EGO] / 10;

    if (dual)
        chance /= 2;

    /* Due to melee damage nerf */
    potency *= 4;

    if (number_range(0, 99) < chance)
    {
        int ls = dam * potency / 100;

        ls = number_range(ls * 95 / 100, ls * 130 / 100);

        sprintf(buf, "@@W$n screams in @@Ragony@@W as an evil @@da@@eur@@da@@W flows from $p!@@N (@@r%d@@N)", ls);
        act(buf, victim, wield, ch, TO_NOTVICT);
        sprintf(buf, "@@WYou feel a surge of health as $p sucks the life of $N@@N!! (@@r%d@@N)", ls);
        act(buf, ch, wield, victim, TO_CHAR);
        sprintf(buf, "@@WYou scream in @@Ragony@@W as $p shrieks, and shrouds you in an evil @@da@@eur@@da@@N!! (@@r%d@@N)", ls);
        act(buf, victim, wield, ch, TO_CHAR);

        ch->hit = UMIN(ch->max_hit, ch->hit + ls);

        ch->alignment = UMAX(-1000, ch->alignment - 50);

        return TRUE;
    }

    return FALSE;
}

/*
 * Set position of a victim.
 */
void update_pos(CHAR_DATA *victim)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int num, counter;

    if (victim->hit > 0)
    {
        if (IS_SET(victim->affected_by, AFF_VAMP_HEALING))
            REMOVE_BIT(victim->affected_by, AFF_VAMP_HEALING);

        if (victim->position <= POS_STUNNED && victim->stunTimer == 0)
        {
            act("$n stands, and gets to $s feet.", victim, NULL, NULL, TO_ROOM);
            victim->position = POS_STANDING;
        }
        return;
    }
    if (IS_VAMP(victim) && (victim->hit < 1) && !IS_SET(victim->affected_by, AFF_VAMP_HEALING) && !IS_NPC(victim) && !(deathmatch))
    {
        CHAR_DATA *check;

        if (!IS_NPC(victim))
            gain_exp(victim, 0 - (victim->exp / 4));

        sprintf(buf, "%s (vampire) has been misted!", victim->name);
        monitor_chan(buf, MONITOR_COMBAT);

        act("$n turns to mist and floats away....", victim, NULL, NULL, TO_ROOM);
        act("You turn to mist and float back to the morgue...", victim, NULL, NULL, TO_CHAR);

        /* Universe -  need to add junk here */
        if (IS_VAMP(victim) && ((victim->fighting != NULL) && !IS_NPC(victim->fighting)))
        {
            counter = number_range(2, 4);

            while (counter > 0)
            {
                num = number_range(1, UMIN(1, victim->carry_number));
                obj = victim->first_carry;
                if (obj == NULL)
                    break;

                for (obj = victim->first_carry; obj != NULL; obj = obj->next_in_carry_list)
                {
                    num = num - 1;
                    if (num == 0)
                        break;
                }

                if (obj == NULL)
                    continue;

                if (obj != NULL && !IS_SET(obj->extra_flags, ITEM_NOLOOT))
                {
                    obj_from_char(obj);
                    obj_to_room(obj, victim->in_room);
                }
                counter = counter - 1;
            }
        }
        /*
         * drop stuff if is (WANTED)
         */
        if ((IS_SET(victim->act, PLR_KILLER) || IS_SET(victim->act, PLR_THIEF)) && ((victim->fighting != NULL) && ((!IS_NPC(victim->fighting)) || (!str_cmp(rev_spec_lookup(victim->fighting->spec_fun), "spec_executioner")))))
        {
            counter = number_range(2, 3);

            while (counter > 0)
            {
                num = number_range(1, UMIN(1, victim->carry_number));
                obj = victim->first_carry;
                if (obj == NULL)
                    break;

                for (obj = victim->first_carry; obj != NULL; obj = obj->next_in_carry_list)
                {
                    num = num - 1;
                    if (num == 0)
                        break;
                }

                if (obj == NULL)
                    continue;

                if (obj != NULL && !IS_SET(obj->extra_flags, ITEM_NOLOOT))
                {
                    obj_from_char(obj);
                    obj_to_room(obj, victim->in_room);
                }
                counter = counter - 1;
            }
        }

        /*
         * drop stuff if is (WANTED)
         */
        if ((victim->fighting != NULL) && (!IS_NPC(victim->fighting)) && (IS_WOLF(victim->fighting)))
        {
            counter = number_range(4, 8);

            while (counter > 0)
            {
                num = number_range(1, UMIN(1, victim->carry_number));
                obj = victim->first_carry;
                if (obj == NULL)
                    break;

                for (obj = victim->first_carry; obj != NULL; obj = obj->next_in_carry_list)
                {
                    num = num - 1;
                    if (num == 0)
                        break;
                }

                if (obj == NULL)
                    continue;

                if (obj != NULL && !IS_SET(obj->extra_flags, ITEM_NOLOOT))
                {
                    obj_from_char(obj);
                    obj_to_room(obj, victim->in_room);
                }
                counter = counter - 1;
            }
        }
        char_from_room(victim);
        char_to_room(victim, get_room_index(victim->pcdata->recall_vnum));
        act("A mist floats in, and forms into $n's corpse!", victim, NULL, NULL, TO_ROOM);

        SET_BIT(victim->affected_by, AFF_VAMP_HEALING);

        stop_fighting(victim, TRUE);
        victim->hit = -20;
        for (check = first_char; check != NULL; check = check->next)
        {
            if (check->hunting == victim)
                end_hunt(check);
        }
        return;
    }

    if (IS_VAMP(victim) && IS_SET(victim->affected_by, AFF_VAMP_HEALING) && !(deathmatch) && !IS_NPC(victim))
    {

        /*
         * Then they don't really die... just regen slowly...!
         */
        if (victim->hit < -20)
            victim->hit = -20;

        if (victim->hit == -20)
        {
            act("$n's body slowly regains it's shape!", victim, NULL, NULL, TO_ROOM);
            send_to_char("Your body slowly regains it's shape!\n\r", victim);
            victim->hit++;
            victim->position = POS_MORTAL;
        }
        else if (victim->hit > -20 && victim->hit < -15)
            victim->hit++;
        else if (victim->hit == -15)
        {
            act("$n's skin grows back into place!", victim, NULL, NULL, TO_ROOM);
            send_to_char("Your skin grows back into place!\n\r", victim);
            victim->hit++;
        }
        else if (victim->hit > -15 && victim->hit < -10)
            victim->hit++;
        else if (victim->hit == -10)
        {
            act("$n's wounds form scar tissue and vanish!", victim, NULL, NULL, TO_ROOM);
            send_to_char("Your wounds form scar tissue and vanish!\n\r", victim);
            victim->hit++;
        }
        else if (victim->hit > -10 && victim->hit < -5)
            victim->hit++;
        else if (victim->hit == -5)
        {
            act("$n's looks like $s old self again.", victim, NULL, NULL, TO_ROOM);
            send_to_char("Your look like your old self again.\n\r", victim);
            victim->hit++;
        }
        else if (victim->hit > -5 && victim->hit < -1)
            victim->hit++;
        else
        {
            REMOVE_BIT(victim->affected_by, AFF_VAMP_HEALING);
            victim->position = POS_STUNNED;
            victim->hit = 5;
        }
        return;
    }

    if ((!IS_NPC(victim)) && (IS_VAMP(victim)) && !(deathmatch))
        return;

    if (IS_NPC(victim) && victim->hit <= 0)
    {
        victim->position = POS_DEAD;
        return;
    }
    if (victim->hit <= -10)
        victim->position = POS_DEAD;
    else if (victim->hit <= -6)
        victim->position = POS_MORTAL;
    else if (victim->hit <= -3)
        victim->position = POS_INCAP;
    else
        victim->position = POS_STUNNED;

    return;
}

struct dam_table_str
{
    int min_dam;
    const char *col;
    const char *vs;
    const char *vp;
    const char *str;
};

struct dam_table_str dam_table[] = {
    {2900, "@@l", "!!!!VIVESECT!!!!", "!!!!VIVESECTS!!!!", " into a living corpse with"},
    {2400, "@@m", "!!!!OBLITERATE!!!!", "!!!!OBLITERATES!!!!", " into disappearing particles with"},
    {2100, "@@m", "!!!!DEVASTATE!!!!", "!!!!DEVASTATES!!!!", "'s very existence with"},
    {1700, "@@m", "!!!!MUTILATE!!!!", "!!!!MUTILATES!!!!", " with"},
    {1450, "@@e", "****ANNIHILATE****", "****ANNIHILATES****", " into the bloody ground with"},
    {1375, "@@e", "***EVISCERATE***", "***EVISCERATES***", " into lots of small pieces with"},
    {1300, "@@e", "**DEMOLISH**", "**DEMOLISHES**", ", spraying $S guts all over the ground with"},
    {1200, "@@R", "*PULVERISE*", "*PULVERISES*", ", spreading blood and gore over the floor with"},
    {1150, "@@R", "THWACK", "THWACKS", ", leaving $M dazed and reeling with"},
    {1100, "@@R", "annihilate", "annihilates", " with"},
    {900, "@@p", "eviscerate", "eviscerates", " with"},
    {850, "@@p", "demolish", "demolishes", " with"},
    {800, "@@p", "pulverise", "pulverises", " with"},
    {700, "@@y", "thwack", "thwacks", " with"},
    {650, "@@y", "flay", "flays", "'s body mercillesly with"},
    {600, "@@y", "lacerate", "lacerates", " into a map of the mud with"},
    {500, "@@G", "maul", "mauls", " with great vengeance and FURIOUS anger from"},
    {450, "@@G", "tear", "tears", "'s skin into shreds with"},
    {400, "@@r", "rip apart", "rips apart", "'s skull with"},
    {350, "@@r", "remove", "removes", "'s ability to have children from"},
    {300, "@@c", "smash", "smashes", " forcefully with"},
    {250, "@@c", "cause", "causes", " to scream in agony from"},
    {200, "@@c", "laugh at", "laughs at", ", drawing trails of blood with"},
    {100, "@@a", "make", "makes", " wince in pain from"},
    {50, "@@a", "mark", "marks", " with an X from"},
    {28, "@@a", "wound", "wounds", " painfully with"},
    {20, "@@b", "graze", "grazes", " with"},
    {10, "@@b", "bonk", "bonks", " on the head with"},
    {7, "@@b", "scratch", "scratches", " roughly with"},
    {5, "@@b", "nick", "nicks", " gently with"},
    {2, "@@b", " tickle", "tickles", " softly with"},
    {0, "@@g", " fail to hit", "fails to hit", " with"}};

void dam_message(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, bool critical)
{
    char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH], critical_message[MAX_STRING_LENGTH];
    const char *vs;  /* Singular */
    const char *vp;  /* Plural   */
    const char *str; /* Strength */
    const char *attack;
    const char *col;
    char punct;
    int dam_table_num, check_dt;
    bool dead = FALSE;

    if (dam >= victim->hit)
        dead = TRUE;

    if (critical)
        sprintf(critical_message, "@@N(@@rCRITICAL@@N)");
    else
        sprintf(critical_message, "");

    if (!IS_NPC(ch) && IS_WOLF(ch) && (IS_SHIFTED(ch) || IS_RAGED(ch)))
    {
        dt = TYPE_HIT + 5; /* claw attack */
    }

    for (dam_table_num = 0; dam_table[dam_table_num].min_dam > dam; dam_table_num++)
        ;

    col = dam_table[dam_table_num].col;
    vs = dam_table[dam_table_num].vs;
    vp = dam_table[dam_table_num].vp;
    str = dam_table[dam_table_num].str;

    punct = (dam <= 64) ? '.' : '!';

    /*
     * Use combat skills, etc to make unarmed combat more fun :)
     */

    if (dt == TYPE_MARTIAL)
        dt = TYPE_HIT + 13 + number_range(0, 14);

    attack = get_dt_name(dt);

    sprintf(buf1, "%s$n %s%s $N%s%s $s %s%c@@g @@l(@@e%d@@l)@@N %s", col, col, vp, col, str, attack, punct, dam, critical_message);
    sprintf(buf2, "%sYou %s%s $N%s%s your %s%c@@g @@l(@@e%d@@l)@@N %s", col, col, vs, col, str, attack, punct, dam, critical_message);
    if (*str == '\'')
        sprintf(buf3, "%s$n %s%s your%s%s $s %s%c@@g @@l(@@e%d@@l)@@N %s", col, col, vp, col, str + 2, attack, punct, dam, critical_message);
    else
        sprintf(buf3, "%s$n %s%s you%s%s $s %s%c@@g @@l(@@e%d@@l)@@N %s", col, col, vp, col, str, attack, punct, dam, critical_message);

    act(buf1, ch, NULL, victim, TO_NOTVICT);
    act(buf2, ch, NULL, victim, TO_CHAR);
    act(buf3, ch, NULL, victim, TO_VICT);

    if (dead)
    {
        death_message(ch, victim, dt);
    }
    return;
}
/* -S- Addition: Like damage() but called by objects, no message */
/* WARNING: No killer checks made, etc.  Only use if no keeper for obj */

void obj_damage(OBJ_DATA *obj, CHAR_DATA *victim, int dam)
{
    if ((victim->position == POS_DEAD) || (victim->is_free == TRUE))
        return;

    /*
     * Damage modifiers.
     */
    if (IS_AFFECTED(victim, AFF_SANCTUARY) || item_has_apply(victim, ITEM_APPLY_SANC))
        dam /= 2;

    if ((IS_AFFECTED(victim, AFF_PROTECT) || item_has_apply(victim, ITEM_APPLY_PROT)) && IS_SET(obj->extra_flags, ITEM_EVIL))
        dam -= dam / 4;

    if (dam < 0)
        dam = 0;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= dam;

    update_pos(victim);

    switch (victim->position)
    {
    case POS_MORTAL:
        act("$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM);
        send_to_char("You are mortally wounded, and will die soon, if not aided.\n\r", victim);
        break;

    case POS_INCAP:
        act("$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM);
        send_to_char("You are incapacitated and will slowly die, if not aided.\n\r", victim);
        break;

    case POS_STUNNED:
        act("$n is stunned, but will probably recover.", victim, NULL, NULL, TO_ROOM);
        send_to_char("You are stunned, but will probably recover.\n\r", victim);
        break;

    case POS_DEAD:
        act("$n is DEAD!!", victim, 0, 0, TO_ROOM);
        send_to_char("You have been KILLED!!\n\r\n\r", victim);
        break;

    default:
        if (dam > victim->max_hit / 4)
            send_to_char("That really did HURT!\n\r", victim);
        if (victim->hit < victim->max_hit / 4)
            send_to_char("You sure are BLEEDING!\n\r", victim);
        break;
    }

    /*
     * Payoff for killing things.
     */
    if (victim->position == POS_DEAD)
    {

        if (!IS_NPC(victim))
        {

            sprintf(log_buf, "%s killed by %s at %d", victim->name, obj->short_descr, victim->in_room->vnum);
            log_string(log_buf);

            notify(log_buf, 82);

            /*
             * As level gain is no longer automatic, a dead char loses
             * * 1/2 their gained exp.  -S-
             * * Fixed my bug here too, hehe!
             */

            if (victim->exp > 0)
            {
                gain_exp(victim, (0 - (victim->exp / 2)));
                victim->exp = UMAX(victim->exp, 0);
            }
        }

        raw_kill(victim, "");

        if (deathmatch && !IS_NPC(victim))
            do_quit(victim, "");

        return;
    }

    tail_chain();
    return;
}

void death_message(CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
    /*
     * Used to display assorted death messages, based on dt
     * * max_dt == number of entries in attack table in dam_message,
     * * so we know if kill was unarmed, armed, or through spells or skills.
     * * Stephen
     */

    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    int vnum;

    /*
     * For debugging purposes
     * sprintf( buf, "dt: %d max_dt: %d\n\r", dt, max_dt );
     * notify( buf, 1 );
     */

    vnum = 0; /* Used to load up body parts.... eewwwwww! */

    if (dt == 0)
    {
        if (!(IS_NPC(victim) && IS_SET(victim->act, ACT_NO_BODY)))
            switch (number_range(0, 9))
            {
            case 0:
                sprintf(buf1, "$n grabs $N's neck, and twists until there is a loud SNAP!");
                sprintf(buf2, "You grab $N's neck, and twist until there is a loud SNAP!");
                sprintf(buf3, "$n grabs your neck, and twists until there is a loud SNAP!");
                break;
            case 1:
                sprintf(buf1, "$n slams $s fist into $N and crushes $S heart!");
                sprintf(buf2, "You slam your fist into $N and crush $S heart!");
                sprintf(buf3, "$n slams $s fist into you, and crushes your heart!");
                break;
            case 2:
                sprintf(buf1, "$n rams $s hand into $N's ribcage, and rips out $S guts!");
                sprintf(buf2, "You ram your hand into $N's ribcage, and rip out $S guts!");
                sprintf(buf3, "$n rams $s hand into your ribcage, and rips out your guts!");
                break;
            case 3:
                sprintf(buf1, "$n grabs $N, and rips $S head clean off!");
                sprintf(buf2, "You grab $N, and rip his head clean off!");
                sprintf(buf3, "$n grabs you, and rips your head clean off!");
                break;
            case 4:
                sprintf(buf1, "$n reaches behind $N, and rips $S spine out of $S back!");
                sprintf(buf2, "You reach behind $N, and rip $S spine out of $S back!");
                sprintf(buf3, "$n reaches behind you, and rips your spine out of your back!");
                break;
            case 5:
                sprintf(buf1, "$n rips $N's face open via $S eye sockets!");
                sprintf(buf2, "You rip $N's face open via $S eye sockets!");
                sprintf(buf3, "$n rips your face open via your eye sockets!");
                break;
            case 6:
                sprintf(buf1, "$n rips off $N's head and vomits down $S throat!");
                sprintf(buf2, "You rip off $N's head and vomit down $S throat!");
                sprintf(buf3, "$n rips off your head and vomits down your throat!");
                break;
            case 7:
                sprintf(buf1, "$N splurts blood as $n rips open $S chest with $s teeth!");
                sprintf(buf2, "$N splurts blood as you rip open $S chest with your teeth!");
                sprintf(buf3, "You splurt blood as $n rips open your chest with $s teeth!");
                break;
            case 8:
                sprintf(buf1, "$n wrenches $N's arms out from their sockets!");
                sprintf(buf2, "You wrench $N's arms out from their sockets!");
                sprintf(buf3, "$n wrenches your arms out of thier sockets!");
                break;
            case 9:
                sprintf(buf1, "$n shatters $N's skull with a punch.  Brains leak out.");
                sprintf(buf2, "You shatter $N's skull with a punch.  Brains leak out.");
                sprintf(buf3, "$n shatters your skull with a punch.  Brains leak out.");
                break;
            }
        else /* Unarmed, mob has no_body */
        {
            sprintf(buf1, "$n shatters $N's skull with a punch.  Brains leak out.");
            sprintf(buf2, "You shatter $N's skull with a punch.  Brains leak out.");
            sprintf(buf3, "$n shatters your skull with a punch.  Brains leak out.");
        }
    }

    if (dt >= TYPE_HIT || dt == TYPE_MARTIAL || dt == gsn_counter)
        switch (dt - TYPE_HIT)
        {
        case 1: /* slice */
        case 3: /* slash */
            switch (number_range(0, 4))
            {
            case 0:
                sprintf(buf1, "$n slices $N's head clean from $S neck");
                sprintf(buf2, "You slice $N's head clean from $S neck");
                sprintf(buf3, "$n slices your head clean from your neck");
                break;
            case 1:
                sprintf(buf1, "$n slashes open $N's chest; $S entrails pour out!");
                sprintf(buf2, "You slash open $N's chest; $S entrails pour out!");
                sprintf(buf3, "$n slashes open your chest; your entrails pour out!");
                break;
            case 2:
                sprintf(buf1, "$n slices $N's throat open.  Blood sprays out wildly!");
                sprintf(buf2, "You slice $N's throat open.  Blood sprays out wildly!");
                sprintf(buf3, "$n slices your throat open.  Blood sprays out wildly!");
                break;
            case 3:
                sprintf(buf1, "$n slices $N's legs off, leaving two bloody stumps!");
                sprintf(buf2, "You slice $N's legs off, leaving two bloody stumps!");
                sprintf(buf3, "$n slices your legs off, leaving two bloody stumps!");
                break;
            case 4:
                sprintf(buf1, "$n slashes $N's eyeballs out!");
                sprintf(buf2, "You slash $N's eyeballs out!");
                sprintf(buf3, "$n slashes your eyeballs out!");
                break;
            }
            break;
        case 2:  /* Stab */
        case 11: /* Pierce */
            switch (number_range(0, 4))
            {
            case 0:
                sprintf(buf1, "$n rips a gaping hole down $N's back!");
                sprintf(buf2, "You rip a gaping hole down $N's back!");
                sprintf(buf3, "$n rips a gaping hole down your back!");
                break;
            case 1:
                sprintf(buf1, "$n stabs into $N, and cuts $S heart out!");
                sprintf(buf2, "You stab into $N, and cut $S heart out!");
                sprintf(buf3, "$n stabs into you, and cuts your heart out!");
                break;
            case 2:
                sprintf(buf1, "$n stabs into $N's back, and wrenches out $S spine!");
                sprintf(buf2, "You stab into $N's back, and wrench out $S spine!");
                sprintf(buf3, "$n stabs into your back, and wrenches out your spine!");
                break;
            case 3:
                sprintf(buf1, "$n plunges $s weapon into $N's head!");
                sprintf(buf2, "You plunge your weapon into $N's head!");
                sprintf(buf3, "$n plunges $s weapon into your head!");
                break;
            case 4:
                sprintf(buf1, "$n stabs into $N's chest, skewering $S heart!");
                sprintf(buf1, "$n stabs into $N's chest, skewering $S heart!");
                sprintf(buf2, "You stab into $N's chest, skewering $S heart!");
                sprintf(buf2, "You stab into $N's chest, skewering $S heart!");
                sprintf(buf3, "$n stabs into your chest, skewering your heart!");
                break;
            }
            break;
        case 4: /* Whip */
            switch (number_range(0, 3))
            {
            case 0:
                sprintf(buf1, "$n whips out $N's eyes, spraying blood everywhere!");
                sprintf(buf2, "You whip out $N's eyes, spraying blood everywhere!");
                sprintf(buf3, "$n whips out your eyes, spraying blood everywhere!");
                break;
            case 1:
                sprintf(buf1, "$n's whip catches $N's head, and rips it off!");
                sprintf(buf2, "Your whip catches $N's head, and rips it off!");
                sprintf(buf3, "$n's whip catches your head, and rips it off!");
                break;
            case 2:
                sprintf(buf1, "$n's whip wraps around $N's arms, yanking them off!");
                sprintf(buf2, "Your whip wraps around $N's arms, yanking them off!");
                sprintf(buf3, "$n's whip wraps around your arms, yanking them off!");
                break;
            case 3:
                sprintf(buf1, "$n's whip cuts open $N's main artery, spraying blood!");
                sprintf(buf2, "Your whip cuts open $N's main artery, spraying blood!");
                sprintf(buf3, "$n's whip cuts open your main artery, spraying blood!");
                break;
            }
            break;
        case 5: /* Claw */
            switch (number_range(0, 4))
            {
            case 0:
                sprintf(buf1, "$n claws out $N's heart!");
                sprintf(buf2, "You claw out $N's heart!");
                sprintf(buf3, "$n claws out your heart!");
                break;
            case 1:
                sprintf(buf1, "$n's claw catches $N's back, slicing it open!");
                sprintf(buf2, "Your claw catches $N's back, slicing it open!");
                sprintf(buf3, "$n's claw catches your back, slicing it open!");
                break;
            case 2:
                sprintf(buf1, "$N screams in agony, as $n's claw removes $S eyes!");
                sprintf(buf1, "$N screams in agony, as $n's claw removes $S eyes!");
                sprintf(buf2, "$N screams in agony, as your claw removes $S eyes!");
                sprintf(buf2, "$N screams in agony, as your claw removes $S eyes!");
                sprintf(buf3, "You scream in agony, as $n's claw removes your eyes!");
                break;
            case 3:
                sprintf(buf1, "$n's claw ruptures $N's ribcage, shredding $S heart!");
                sprintf(buf2, "Your claw ruptures $N's ribcage, shredding $S heart!");
                sprintf(buf3, "$n's claw ruptures your ribcage, shredding your heart!");
                break;
            case 4:
                sprintf(buf1, "$n's claw slashes $N's neck, decapitating $M!");
                sprintf(buf2, "Your claw slashes $N's neck, decapitating $M!");
                sprintf(buf3, "$n's claw slashes your neck, decapitating you!");
                break;
            }
            break;
        case 7: /* Pound */
        case 8: /* Crush */
            switch (number_range(0, 4))
            {
            case 0:
                sprintf(buf1, "$n pounds $N's head; $S brains leak from $S ears!");
                sprintf(buf2, "You pound $N's head; $S brains leak from $S ears!");
                sprintf(buf3, "$n pounds your head; your brains leak from your ears!");
                break;
            case 1:
                sprintf(buf1, "$n crushes $N's ribcage, and $S entrails slop out!");
                sprintf(buf2, "You crush $N's ribcage, and $S entrails slop out!");
                sprintf(buf3, "$n crushes your ribcage, and your entrails slop out!");
                break;
            case 2:
                sprintf(buf1, "$n pounds $N's spine until you hear it CRACK!");
                sprintf(buf2, "You pound $N's spine until you hear it CRACK!");
                sprintf(buf3, "$n pounds your spine until you hear it CRACK!");
                break;
            case 3:
                sprintf(buf1, "$n pounds $N's face, forcing $S nose into $S brain!");
                sprintf(buf2, "You pound $N's face, forcing $S nose into $S brain!");
                sprintf(buf3, "$n pounds your face, forcing your nose into your brain!");
                break;
            case 4:
                sprintf(buf1, "$n crushes $N's head down into $S neck!");
                sprintf(buf2, "You crush $N's head down into $S neck!");
                sprintf(buf3, "$n crushes your head down into your neck!");
                break;
            }
            break;
        case 6: /* Blast */
            switch (number_range(0, 4))
            {
            case 0:
                sprintf(buf1, "$n's blast totally obliterates $N's head!");
                sprintf(buf2, "Your blast totally obliterates $N's head!");
                sprintf(buf3, "$n's blast totally obliterates your head!");
                break;
            case 1:
                sprintf(buf1, "$n's blast makes $N's head fly into the air!");
                sprintf(buf2, "Your blast makes $N's head fly into the air!");
                sprintf(buf3, "$n's blast makes your head fly into the air!");
                break;
            case 2:
                sprintf(buf1, "$n blasts $N's stomach open - entrails plop out!");
                sprintf(buf2, "You blast $N's stomach open - entrails plop out!");
                sprintf(buf3, "$n blasts your stomach open - entrails plop out!");
                break;
            case 3:
                sprintf(buf1, "$n's blast removes $N's legs from $S body!");
                sprintf(buf2, "Your blast removes $N's legs from $S body!");
                sprintf(buf3, "$n's blast removes your legs from your body!");
                break;
            case 4:
                sprintf(buf1, "$n's blast splits $N's back, and $S spine falls out!");
                sprintf(buf2, "Your blast splits $N's back, and $S spine falls out!");
                sprintf(buf3, "$n's blast splits your back, and your spine falls out!");
                break;
            }
            break;
        default: /* grep, bite, suction */
            switch (number_range(0, 3))
            {
            case 0:
                sprintf(buf1, "$n pulls $N's heart clean from $S ribcage!");
                sprintf(buf2, "You pull $N's heart clean from $S ribcage!");
                sprintf(buf3, "$n pulls your heart clean from your ribcage!");
                break;
            case 1:
                sprintf(buf1, "$n snags $N's spine, and rips it out!");
                sprintf(buf2, "You snag $N's spine, and rip it out!");
                sprintf(buf3, "$n snags your spine, and rips it out!");
                break;
            case 2:
                sprintf(buf1, "$N's stomach splits open, and $S entrails slip out!");
                sprintf(buf2, "$N's stomach splits open, and $S entrails slip out!");
                sprintf(buf3, "Your stomach splits open, and your entrails slip out!");
                break;
            case 3:
                sprintf(buf1, "$n pulls $N's heart from $S chest!");
                sprintf(buf2, "You pull $N's heart from $S chest!");
                sprintf(buf3, "$n pulls your heart from your chest!");
                break;
            }
            break;
        }
    act(buf1, ch, NULL, victim, TO_NOTVICT);
    act(buf2, ch, NULL, victim, TO_CHAR);
    act(buf3, ch, NULL, victim, TO_VICT);

    /*
     * Load up object, if needed.
     */
    if (vnum != 0)
    {
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;
        char *name;

        name = IS_NPC(ch) ? ch->short_descr : ch->name;
        obj = create_object(get_obj_index(vnum), 0);
        obj->timer = number_range(4, 7);

        sprintf(buf, obj->short_descr, name);
        free_string(obj->short_descr);
        obj->short_descr = str_dup(buf);

        sprintf(buf, obj->description, name);
        free_string(obj->description);
        obj->description = str_dup(buf);

        obj_to_room(obj, ch->in_room);
    }

    return;
}

void cloak_action(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
    if ((IS_AFFECTED(victim, AFF_CLOAK_FLAMING)) && (ch != victim))
    {

        act("@@N$n's @@ecloak@@N flares and envelops $N in @@eflames@@N!!", victim, NULL, ch, TO_NOTVICT);
        if (!IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_BLIND_PLAYER))
            act("Flame cloak on $K ouch", ch, NULL, victim, TO_CHAR);
        else
            act("@@N$N's @@ecloak@@N flares, and envelops you with @@eflame@@N!!", ch, NULL, victim, TO_CHAR);
        if (!IS_NPC(victim) && IS_SET(victim->pcdata->pflags, PFLAG_BLIND_PLAYER))
            act("Your Flame cloak flares", victim, NULL, ch, TO_CHAR);
        else
            act("@@NYour @@ecloak@@N flares, and envelops $N with @@eflame@@N!!!", victim, NULL, ch, TO_CHAR);

        if ((is_shielded(ch, ICE_SHIELD)) && (number_range(1, 100) < 30) && (ch != victim))
        {
            CHAR_DATA *rch;
            CHAR_DATA *rch_next;
            OBJ_DATA *explosion;
            CHAR_DATA *elemental;

            act("@@e------------------------@@N", ch, NULL, NULL, TO_CHAR);
            act("@@l************************@@N", ch, NULL, NULL, TO_CHAR);
            act("@@W!!!!!!!!!!!!!!!!!!!!!!!!@@N", ch, NULL, NULL, TO_CHAR);
            act("@@NThe elemental forces of @@eFire@@N and @@aIce@@N destroy each other!!!", ch, NULL, NULL, TO_CHAR);
            act("@@W!!!!!!!!!!!!!!!!!!!!!!!!@@N", ch, NULL, NULL, TO_CHAR);
            act("@@l************************@@N", ch, NULL, NULL, TO_CHAR);
            act("@@e------------------------@@N", ch, NULL, NULL, TO_CHAR);
            act("@@e------------------------@@N", ch, NULL, NULL, TO_ROOM);
            act("@@l************************@@N", ch, NULL, NULL, TO_ROOM);
            act("@@W!!!!!!!!!!!!!!!!!!!!!!!!@@N", ch, NULL, NULL, TO_ROOM);
            act("@@NThe elemental forces of @@eFire@@N and @@aIce@@N destroy each other!!!", ch, NULL, NULL, TO_ROOM);
            act("@@W!!!!!!!!!!!!!!!!!!!!!!!!@@N", ch, NULL, NULL, TO_ROOM);
            act("@@l************************@@N", ch, NULL, NULL, TO_ROOM);
            act("@@e------------------------@@N", ch, NULL, NULL, TO_ROOM);
            act("@@N$N's @@ecloak@@N is ripped to shreds!!!@@N", ch, NULL, victim, TO_ROOM);
            act("@@NYour @@ecloak@@N is ripped to shreds!!!@@N", ch, NULL, victim, TO_VICT);

            affect_strip(ch, skill_lookup("iceshield"));
            affect_strip(victim, skill_lookup("iceshield"));
            affect_strip(ch, skill_lookup("fireshield"));
            affect_strip(victim, skill_lookup("fireshield"));
            affect_strip(ch, skill_lookup("cloak:flaming"));
            affect_strip(victim, skill_lookup("cloak:flaming"));
            if (IS_SET(ch->affected_by, AFF_CLOAK_FLAMING))
                REMOVE_BIT(ch->affected_by, AFF_CLOAK_FLAMING);
            if (IS_SET(victim->affected_by, AFF_CLOAK_FLAMING))
                REMOVE_BIT(victim->affected_by, AFF_CLOAK_FLAMING);

            if ((explosion = create_object(get_obj_index(OBJ_VNUM_CONFLAGRATION), 120)) != NULL)
                if ((elemental = create_mobile(get_mob_index(MOB_VNUM_COMBAT_ELEMENTAL))) != NULL)
                {
                    char bufz[MSL];

                    explosion->level = 120;
                    free_string(explosion->short_descr);
                    explosion->short_descr = str_dup("@@mConflagration@@N");
                    free_string(explosion->description);
                    explosion->description = str_dup("@@N A @@eFlaming @@NStaff of @@aIce@@N is supsended in mid air!");

                    elemental->level = 140;
                    free_string(elemental->name);
                    elemental->name = str_dup(".hidden");
                    free_string(elemental->short_descr);
                    elemental->short_descr = str_dup("@@NThe @@rConflict@@N of @@eFire @@Nand @@aIce@@N");
                    free_string(elemental->long_descr);
                    elemental->long_descr = str_dup("@@NA @@rPillar@@N of @@eFire @@Nand @@aIce@@N immolates itself!");

                    char_to_room(elemental, ch->in_room);
                    obj_to_char(explosion, elemental);
                    sprintf(bufz, "%s", explosion->name);
                    do_wear(elemental, bufz);

                    if (number_range(0, 99) < 40)
                        obj_cast_spell(skill_lookup("Retributive strike"), 100, elemental, NULL, explosion);
                    else
                    {
                        CREF(rch_next, CHAR_NEXTROOM);
                        for (rch = ch->in_room->first_person; rch != NULL; rch = rch_next)
                        {
                            rch_next = rch->next_in_room;
                            if (get_psuedo_level(rch) < 70)
                                continue;
                            send_to_char("\n\r@@NYou are @@ablasted@@N by the @@econflagration@@N!\n\r", rch);
                            if (number_range(0, 99) < 50)
                                obj_cast_spell(skill_lookup("frost breath"), 120, elemental, rch, explosion);
                            else
                                obj_cast_spell(skill_lookup("lava burst"), 120, elemental, rch, explosion);
                        }
                        if ((elemental->fighting != NULL) && (IS_NPC(elemental->fighting)))
                        {
                            stop_fighting(elemental, TRUE);
                            if ((number_range(0, 99) < 50) && (!IS_NPC(ch)))
                            {
                                multi_hit(elemental, ch, TYPE_UNDEFINED);
                            }
                            else if (!IS_NPC(victim))
                            {
                                multi_hit(elemental, victim, TYPE_UNDEFINED);
                            }
                        }
                        CUREF(rch_next);
                    }
                }
        }
        else
        {
            int flame_damage;
            flame_damage = dam * get_psuedo_level(victim) / 300;
            if (IS_NPC(ch))
            {
                if (IS_SET(ch->act, ACT_SOLO))
                    flame_damage = flame_damage / 5;
                else
                    flame_damage = flame_damage / 3;
            }
            if ((IS_NPC(victim)) && (IS_SET(victim->act, ACT_SOLO)))
                flame_damage = flame_damage * 1.5;

            ch->hit = UMAX(10, ch->hit - flame_damage);
        }
    }
    if ((is_affected(victim, skill_lookup("cloak:misery"))) && (ch != victim) && (number_range(0, 99) < 35))
    {
        AFFECT_DATA af;
        act("@@N$n's @@dcloak@@N flares and shrouds $N in @@dmisery@@N!!", victim, NULL, ch, TO_NOTVICT);
        if (!IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_BLIND_PLAYER))
            act("Flame cloak on $K ouch", ch, NULL, victim, TO_CHAR);
        else
            act("@@N$N's @@dcloak@@N flares, and shrouds you with @@dmisery@@N!!", ch, NULL, victim, TO_CHAR);
        if (!IS_NPC(victim) && IS_SET(victim->pcdata->pflags, PFLAG_BLIND_PLAYER))
            act("Your Death cloak flares", victim, NULL, ch, TO_CHAR);
        else
            act("@@NYour @@dcloak@@N flares, and shrouds $N with @@dmisery@@N!!!", victim, NULL, ch, TO_CHAR);
        if (TRUE)
        {
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
    }

    /* for now, can only have one shield up, or alternatively, only the first
   shield does anything		 */

    if ((victim->first_shield != NULL) && (ch != victim) && (dam > 0))
    {
        char buf1[MSL];
        char buf2[MSL];
        char buf3[MSL];
        sprintf(buf1, "%s", victim->first_shield->absorb_message_room);
        if (!IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_BLIND_PLAYER))
            sprintf(buf2, "%s", "$K shield ouch");
        else
            sprintf(buf2, "%s", victim->first_shield->absorb_message_victim);
        if (!IS_NPC(victim) && IS_SET(victim->pcdata->pflags, PFLAG_BLIND_PLAYER))
            sprintf(buf3, "%s", "Your shield cool");
        else
            sprintf(buf3, "%s", victim->first_shield->absorb_message_self);
        victim->first_shield->hits -= dam;
        dam = dam - dam * (victim->first_shield->percent / 100);
        if (victim->first_shield->harmfull == TRUE)
        {
            ch->hit = UMAX(10, (ch->hit - victim->first_shield->attack_dam));
        }
        act(buf1, victim, NULL, ch, TO_NOTVICT);
        act(buf2, ch, NULL, victim, TO_CHAR);
        act(buf3, victim, NULL, ch, TO_CHAR);

        if (victim->first_shield->hits <= 0)
        {
            AFFECT_DATA *shield_aff;

            for (shield_aff = victim->first_affect; shield_aff != NULL; shield_aff = shield_aff->next)
                if (shield_aff->type == victim->first_shield->sn)
                    break;
            if (shield_aff != NULL)
                affect_remove(victim, shield_aff);
        }
    }
}
