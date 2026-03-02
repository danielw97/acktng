#ifndef DEC_CLOAK_H
#define DEC_CLOAK_H

#include "config.h"

bool cloak_reactive_can_trigger(int element);
bool cloak_handle_spell_defense(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int element, int dam);
int cloak_apply_reactive_effects(CHAR_DATA *ch, CHAR_DATA *victim, int dam, bool shortfight_round);
int cloak_adept_hitroll_bonus(CHAR_DATA *ch);
int cloak_adept_defense_bonus(CHAR_DATA *ch);
int cloak_adept_scale_damage(CHAR_DATA *ch, int dam);

int cloak_drain_damage_from_level(int pseudo_level);
long cloak_drain_attacker_hp_after_hit(long attacker_hp, int drain_damage);
long cloak_drain_victim_hp_after_hit(long victim_hp, long victim_max_hp, int drain_damage);

#endif
