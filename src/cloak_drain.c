#include "cloak_drain.h"

int cloak_drain_damage_from_level(int pseudo_level)
{
    if (pseudo_level < 0)
        return 0;

    return pseudo_level;
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
