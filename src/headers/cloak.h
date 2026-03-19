#ifndef DEC_CLOAK_H
#define DEC_CLOAK_H

#include "config.h"

bool cloak_reactive_can_trigger(int element);
bool cloak_handle_spell_defense(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int element, int dam);
int cloak_apply_reactive_effects(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int element,
                                 bool shortfight_round);
int cloak_adept_hitroll_bonus(CHAR_DATA *ch);
int cloak_adept_defense_bonus(CHAR_DATA *ch);
int cloak_adept_scale_damage(CHAR_DATA *ch, int dam);

int cloak_drain_damage_from_level(int pseudo_level);
int cloak_level_damage_reduction(int pseudo_level);
long cloak_drain_attacker_hp_after_hit(long attacker_hp, int drain_damage);
long cloak_drain_victim_hp_after_hit(long victim_hp, long victim_max_hp, int drain_damage);
int cloak_oathbreaker_avoidance_chance(int pseudo_level);
int cloak_transcendence_avoidance_chance(int pseudo_level);
bool cloak_oathbreaker_avoids_spell_damage(int dam, int element, bool has_cloak, int pseudo_level,
                                           int roll_percent);
bool cloak_transcendence_avoids_physical_hit(int dam, int element, int dt, bool has_cloak,
                                             int pseudo_level, int roll_percent);

int cloak_precision_reduce_crit_chance(CHAR_DATA *victim, int crit_chance);
int cloak_precision_reduce_crit_damage(CHAR_DATA *victim, int dam);
int cloak_precision_crit_chance_reduction(int pseudo_level);
int cloak_precision_crit_damage_reduction(int dam, int pseudo_level);
bool cloak_valor_consume_guaranteed_crit(CHAR_DATA *ch);
int cloak_valor_proc_chance(int pseudo_level);

#endif
