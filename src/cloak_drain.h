#ifndef CLOAK_DRAIN_H
#define CLOAK_DRAIN_H

int cloak_drain_damage_from_level(int pseudo_level);
long cloak_drain_attacker_hp_after_hit(long attacker_hp, int drain_damage);
long cloak_drain_victim_hp_after_hit(long victim_hp, long victim_max_hp, int drain_damage);

#endif
