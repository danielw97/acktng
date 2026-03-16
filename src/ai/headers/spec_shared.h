#ifndef DEC_SPEC_SHARED_H
#define DEC_SPEC_SHARED_H 1

/*
 * Shared helper functions used by multiple mob special procedures.
 * Implementations live in special.c.
 */
bool dragon(CHAR_DATA *ch, char *spell_name);
bool spec_summon_cast_random(CHAR_DATA *ch, CHAR_DATA *target, const char *const *spells,
                             int spell_count);
int summon_master_heal_chance(int master_hit, int master_max_hp, int thematic_bonus);
bool spec_summon_heal_master(CHAR_DATA *ch, int thematic_bonus);

#endif
