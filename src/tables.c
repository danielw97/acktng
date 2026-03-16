#include "globals.h"
#include "tables.h"

/* #define RACE_MOD_NONE BIT_1
#define RACE_MOD_MIRACLE_HEAL BIT_2
#define RACE_MOD_EXTRAORDINARY_HEAL BIT_3
#define RACE_MOD_FAST_HEAL BIT_4
#define RACE_MOD_SLOW_HEAL BIT_5
#define RACE_MOD_STRONG_MAGIC BIT_6
#define RACE_MOD_MAGIC_ADEPT BIT_7
#define RACE_MOD_MAGIC_GENIUS BIT_8
#define RACE_MOD_WEAK_MAGIC BIT_9
#define RACE_MOD_NO_MAGIC BIT_10
#define RACE_MOD_IMMUNE_POISON BIT_11
#define RACE_MOD_RESIST_MAGIC BIT_12
#define RACE_MOD_THWART_MAGIC BIT_13
#define RACE_MOD_MAGIC_FOIL BIT_14
#define RACE_MOD_WOODLAND BIT_15
#define RACE_MOD_DARKNESS BIT_16
#define RACE_MOD_HUGE BIT_17
#define RACE_MOD_LARGE BIT_18
#define RACE_MOD_TINY BIT_19
#define RACE_MOD_SMALL BIT_20
#define RACE_MOD_TAIL BIT_21
#define RACE_MOD_TOUGH_SKIN BIT_22
#define RACE_MOD_STONE_SKIN BIT_23
#define RACE_MOD_IRON_SKIN BIT_24
#define RACE_MOD_STRONG BIT_25
#define RACE_MOD_HERCULES BIT_26
#define RACE_MOD_GOLIATH BIT_27 */

const char *lookup_race_bit_value(int bit)
{
   for (int i = 0; tab_race_mod[i].text != NULL; i++)
   {
      if (tab_race_mod[i].value == bit)
         return tab_race_mod[i].text;
   }

   return "";
}

const struct lookup_extended_type tab_race_mod[] = {
    {"none", RACE_MOD_NONE, -1, 0, ""},
    {"miracle heal", RACE_MOD_MIRACLE_HEAL, RACE_MOD_EXTRAORDINARY_HEAL, 1,
     "Increases your regeneration by 4x"},
    {"extraordinary heal", RACE_MOD_EXTRAORDINARY_HEAL, RACE_MOD_FAST_HEAL, 1,
     "Increase your regeneration by 3x"},
    {"fast heal", RACE_MOD_FAST_HEAL, RACE_MOD_NONE, 1, "Increases your regeneration by 2x"},
    {"slow heal", RACE_MOD_SLOW_HEAL, RACE_MOD_NONE, -1, "Halves your regeneration"},
    {"strong magic", RACE_MOD_STRONG_MAGIC, RACE_MOD_NONE, 1,
     "Increases all of your magical damage dealt by 10%"},
    {"magic adept", RACE_MOD_MAGIC_ADEPT, RACE_MOD_STRONG_MAGIC, 3,
     "Increases all of your magical damage dealt by 20%"},
    {"magic genius", RACE_MOD_MAGIC_GENIUS, RACE_MOD_MAGIC_ADEPT, 5,
     "Increases all of your magical damage dealt by 30%"},
    {"weak magic", RACE_MOD_WEAK_MAGIC, RACE_MOD_NONE, -1,
     "Reduces all magical damage you deal by 25%"},
    {"no magic", RACE_MOD_NO_MAGIC, RACE_MOD_NONE, -1, "Makes you extremely bad at magic"},
    {"immune poison", RACE_MOD_IMMUNE_POISON, RACE_MOD_NONE, -1, "Grants poison immunity"},
    {"resist magic", RACE_MOD_RESIST_MAGIC, RACE_MOD_NONE, 1,
     "Reduces magical damage you take by 10%"},
    {"thwart magic", RACE_MOD_THWART_MAGIC, RACE_MOD_RESIST_MAGIC, 2,
     "Reduces magical damage you take by 20%"},
    {"magic foil", RACE_MOD_MAGIC_FOIL, RACE_MOD_THWART_MAGIC, 3,
     "Reduces magical damage you take by 30%"},
    {"woodland", RACE_MOD_WOODLAND, RACE_MOD_NONE, 1, "Adds woodland"},
    {"darkness", RACE_MOD_DARKNESS, RACE_MOD_NONE, 1, "Adds darkness"},
    {"huge", RACE_MOD_HUGE, RACE_MOD_LARGE, 1, "Adds huge"},
    {"large", RACE_MOD_LARGE, RACE_MOD_NONE, 1, "Adds large"},
    {"small", RACE_MOD_SMALL, RACE_MOD_NONE, 1, "Adds small"},
    {"tiny", RACE_MOD_TINY, RACE_MOD_SMALL, 1, "Adds tiny"},
    {"tail", RACE_MOD_TAIL, RACE_MOD_NONE, 3, "Adds tail equip racial and tail extra attack."},
    {"tough skin", RACE_MOD_TOUGH_SKIN, RACE_MOD_NONE, 1,
     "Reduces physical damage you take by 10%"},
    {"stone skin", RACE_MOD_STONE_SKIN, RACE_MOD_TOUGH_SKIN, 2,
     "Reduces physical damage you take by 20%"},
    {"iron skin", RACE_MOD_IRON_SKIN, RACE_MOD_IRON_SKIN, 3,
     "Reduces physical damage you take by 30%"},
    {"strong", RACE_MOD_STRONG, RACE_MOD_NONE, 1, "Increases physical damage you deal by 10%"},
    {"hercules", RACE_MOD_HERCULES, RACE_MOD_STRONG, 3,
     "Increases physical damage you deal by 20%"},
    {"goliath", RACE_MOD_GOLIATH, RACE_MOD_HERCULES, 5,
     "Increases physical damage you deal by 30%"},
    {NULL, -1, 0, -1, NULL}};
