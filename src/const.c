/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/


#include <stdio.h>
#include <time.h>
#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif



/*
 * color table for say, gossip, shout, etc.
 * Put here for ease of editing. -S-
 */

const struct color_type color_table[MAX_COLOR] = {
   {"say", 0},
   {"tell", 1},
   {"gossip", 2},
   {"auction", 3},
   {"music", 4},
   {"shout", 5},
   {"yell", 6},
   {"clan", 7},
   {"race", 8},
   {"flame", 9},
   {"info", 10},
   {"stats", 11},
   {"rooms", 12},
   {"objects", 13},
   {"mobiles", 14},
   {"OOC", 15}

};

const struct ansi_type ansi_table[MAX_ANSI] = {
   {"gray", "\033[0;37m", 0, 'g', 7},
   {"red", "\033[0;31m", 1, 'R', 7},
   {"green", "\033[0;32m", 2, 'G', 7},
   {"brown", "\033[0;33m", 3, 'b', 7},
   {"blue", "\033[0;34m", 4, 'B', 7},
   {"magenta", "\033[0;35m", 5, 'm', 7},
   {"cyan", "\033[0;36m", 6, 'c', 7},
   {"black", "\033[0;30m", 7, 'k', 7}, /* was 0;33 */
   {"yellow", "\033[1;33m", 8, 'y', 7},
   {"white", "\033[1;37m", 9, 'W', 7},
   {"normal", "\033[0;0m\033[0;40m", 10, 'N', 13},
   {"purple", "\033[1;35m", 11, 'p', 7},
   {"dark_grey", "\033[1;30m", 12, 'd', 7},
   {"light_blue", "\033[1;34m", 13, 'l', 7},
   {"light_green", "\033[1;32m", 14, 'r', 7},
   {"light_cyan", "\033[1;36m", 15, 'a', 7},
   {"light_red", "\033[1;31m", 16, 'e', 7},
   {"bold", "\033[1m", 17, 'x', 4},
   {"flashing", "\033[5m", 18, 'f', 4},
   {"inverse", "\033[7m", 19, 'i', 4},
   {"back_red", "\033[0;41m", 20, '2', 7},
   {"back_green", "\033[0;42m", 21, '3', 7},
   {"back_yellow", "\033[0;43m", 22, '4', 7},
   {"back_blue", "\033[0;44m", 23, '1', 7},
   {"back_magenta", "\033[0;45m", 24, '5', 7},
   {"back_cyan", "\033[0;46m", 25, '6', 7},
   {"back_black", "\033[0;40m", 26, '0', 7},
   {"back_white", "\033[1;47m", 27, '7', 7},

};



/*
 * Class table.
 */
const struct class_type class_table[MAX_CLASS] = 
{
   { "Mag", "Mage", APPLY_INT, 2, 5 },

   { "Cle", "Cleric", APPLY_WIS, 3, 4},

   { "Thi", "Thief", APPLY_DEX, 6, 0},

   { "War", "Warrior", APPLY_STR, 7, 0},

   { "Psi", "Psionicist", APPLY_INT, 3, 4},

   { "Pug", "Pugilist", APPLY_CON, 7, 0}
};




/* Table for remort classes.... same format as class_table 
 * Note that alot of stuff is not needed... 
 */
const struct class_type remort_table[MAX_REMORT] = 
{
   {"Sor", "Sorcerer", APPLY_INT, 4, 10},

   {"Pal", "Paladin", APPLY_WIS, 8, 6},

   {"Ass", "Assassin", APPLY_DEX, 12, 0},

   {"Kni", "Knight", APPLY_STR, 14, 0},

   {"Nec", "Necromancer", APPLY_INT, 6, 8},

   {"Mon", "Monk", APPLY_CON, 14, 0},

   {"Wiz", "Wizard", APPLY_INT, 4, 10},

   {"Pri", "Priest", APPLY_WIS, 6, 8},

   {"Wlk", "Warlock", APPLY_DEX, 8, 6},

   {"Swo", "Swordsman", APPLY_STR, 14, 0},

   {"Ego", "Egomancer", APPLY_INT, 6, 8},

   {"Bra", "Brawler", APPLY_CON, 14, 0}
};

const struct class_type adept_table[MAX_CLASS] = 
{
   {"Gma", "Grand Magi", APPLY_INT, 8, 20,},

   {"Tem", "Templar", APPLY_WIS, 16, 12},

   {"Nig", "Nightblade", APPLY_DEX, 20, 8},

   {"Cru", "Crusader", APPLY_STR, 28, 0},

   {"Kin", "Kinetimancer", APPLY_INT, 12, 16},

   {"Mar", "Martial Artist", APPLY_CON, 28, 0}
};

const struct race_type race_table[MAX_RACE] = {
   {
    "Hmn", "Human", 101, 0,
    18, 18, 18, 18, 18,
    RACE_MOD_NONE,
    /*
     * racial wear slots  
     */
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {3, 4, 5, 2, 0, 1},
    "War, Thi, Pug, Psi, Mag, Cle",
    "",
    ELE_NONE,   /* strong magic realms for race */
    ELE_NONE,   /* weak */
    ELE_NONE,   /* resist */
    ELE_NONE,   /* suscept */
    TRUE /* player race */
    },

   {
    "Hlf", "Halfling", 3001, 0,
    13, 16, 17, 21, 18,
    RACE_MOD_SMALL | RACE_MOD_WOODLAND | RACE_MOD_RESIST_SPELL,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {1, 2, 0, 3, 4, 5},
    "Thi, Mag, Cle, War, Psi, Pug",
    "steal \'pick lock\'",
    ELE_NONE,
    ELE_NONE,
    ELE_MENTAL | ELE_FIRE | ELE_HOLY,
    ELE_NONE,
    TRUE},

   {
    "Dwf", "Dwarf", 7536, 0,
    21, 12, 19, 12, 21,
    RACE_MOD_RESIST_SPELL | RACE_MOD_SMALL | RACE_MOD_IMMUNE_POISON,

    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {4, 1, 5, 3, 0, 2},
    "War, Cle, Pug, Psi, Thi, Mag",
    "smash",
    ELE_NONE,
    ELE_NONE,
    ELE_FIRE | ELE_HOLY | ELE_POISON,
    ELE_NONE,
    TRUE},

   {
    "Elf", "Elf", 9201, 0,
    14, 20, 16, 21, 14,
    RACE_MOD_SMALL | RACE_MOD_WOODLAND | RACE_MOD_STRONG_MAGIC,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {0, 4, 1, 2, 3, 5},
    "Mag, Thi, War, Psi, Cle, Pug",
    "\'find doors\'",
    ELE_MENTAL,
    ELE_NONE,
    ELE_MENTAL,
    ELE_NONE,
    TRUE},

   {
    "Gno", "Gnome", 6015, 0,
    12, 17, 21, 20, 15,
    RACE_MOD_TINY | RACE_MOD_IMMUNE_POISON | RACE_MOD_DARKNESS,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {2, 0, 3, 4, 1, 5},
    "Cle, Psi, Mag, Thi, War, Pug",
    "\'faerie fire\' sneak",
    ELE_POISON | ELE_MENTAL | ELE_FIRE,
    ELE_NONE,
    ELE_MENTAL,
    ELE_NONE,
    TRUE},

   {
    "Ogr", "Ogre", 3430, 0,
    21, 11, 12, 20, 21,
    RACE_MOD_WEAK_MAGIC | RACE_MOD_RESIST_SPELL | RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {5, 1, 3, 2, 0, 4},
    "Pug, War, Mag, Thi, Cle, Psi",
    "\'shield block\'",
    ELE_POISON,
    ELE_MENTAL,
    ELE_POISON,
    ELE_MENTAL,
    TRUE},
   {
    "Drw", "Drow", 9201, 0,
    14, 21, 14, 21, 14,
    RACE_MOD_STRONG_MAGIC | RACE_MOD_SLOW_HEAL | RACE_MOD_DARKNESS,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {0, 4, 3, 2, 1, 5},
    "Psi, Mag, War, Thi, Cle, Pug",
    "\'find doors\' \'mind bolt\'",
    ELE_MENTAL | ELE_FIRE,
    ELE_HOLY,
    ELEMENT_SHADOW,
    ELE_HOLY,
    TRUE},

   {
    "Lam", "Lamia", 3001, 0,
    21, 12, 12, 21, 21,
    RACE_MOD_LARGE | RACE_MOD_IMMUNE_POISON | RACE_MOD_TAIL | RACE_MOD_NO_MAGIC | RACE_MOD_IRON_SKIN,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {3, 5, 4, 0, 1, 2},
    "Thi, Pug, War, Psi, Mag, Cle",
    "dodge",
    ELE_POISON,
    ELE_MENTAL | ELE_FIRE,
    ELE_POISON,
    ELE_MENTAL | ELE_FIRE,
    TRUE},

   {
    "Drc", "Dracon", 3001, 0,
    21, 21, 11, 11, 21,
    RACE_MOD_STRONG_MAGIC | RACE_MOD_TOUGH_SKIN | RACE_MOD_SLOW_HEAL | RACE_MOD_LARGE | RACE_MOD_TAIL |
    RACE_MOD_RESIST_SPELL,
    {TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {0, 1, 4, 2, 5, 3},
    "Mag, Cle, War, Psi, Pug, Thi",
    "fly \'fire breath\'",
    ELE_FIRE | ELE_POISON,
    ELE_MENTAL,
    ELE_FIRE,
    ELE_MENTAL,
    TRUE},

   {
    "Cen", "Centaur", 3001, 0,
    18, 15, 21, 12, 19,
    RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN | RACE_MOD_WOODLAND,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE},
    MAX_CLASS, {1, 5, 3, 2, 0, 4},
    "War, Pug, Cle, Thi, Mag, Psi",
    "hunt",
    ELE_MENTAL,
    ELE_FIRE,
    ELE_MENTAL,
    ELE_FIRE,
    TRUE},

   {
    "Ttn", "Titan", 3001, 0,
    21, 13, 12, 18, 21,
    RACE_MOD_NO_MAGIC | RACE_MOD_HUGE | RACE_MOD_RESIST_SPELL,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {5, 2, 3, 4, 0, 1},
    "Pug, War, Psi, Mag, Cle, Thi",
    "\'enhanced damage\'",
    ELE_MENTAL,
    ELE_NONE,
    ELE_MENTAL,
    ELE_NONE,
    TRUE},

   {
    "Sde", "Sidhe", 3001, 0,
    13, 21, 17, 21, 13,
    RACE_MOD_STRONG_MAGIC | RACE_MOD_TINY | RACE_MOD_RESIST_SPELL,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {2, 3, 1, 4, 5, 0},
    "Psi, Thi, Mag, Cle, Pug, War",
    "fly",
    ELE_MENTAL | ELE_SHADOW,
    ELE_FIRE,
    ELE_MENTAL,
    ELE_FIRE,
    TRUE},

   {
    "Min", "Minotaur", 3001, 0,
    21, 12, 11, 20, 21,
    RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN | RACE_MOD_DARKNESS,
    {TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE},
    MAX_CLASS, {1, 5, 2, 4, 0, 3},
    "War, Pug, Mag, Cle, Psi, Thi",
    "\'enhanced damage\'",
    ELE_POISON,
    ELE_NONE,
    ELE_NONE,
    ELE_NONE,
    TRUE},
   {
    "Trl", "Troll", 3001, 0,
    21, 11, 11, 21, 21,
    RACE_MOD_FAST_HEAL | RACE_MOD_LARGE | RACE_MOD_DARKNESS | RACE_MOD_STONE_SKIN,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {5, 1, 2, 4, 0, 3},
    "Pug, War, Cle, Thi, Psi, Mag",
    "\'stone skin\'",
    ELE_NONE,
    ELE_FIRE | ELE_HOLY,
    ELE_NONE,
    ELE_FIRE,
    TRUE},
   {
    "Und", "Undead", 3001, 0,
    20, 11, 11, 15, 21,
    RACE_MOD_DARKNESS | RACE_MOD_IMMUNE_POISON,
    {TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE,
     TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {5, 1, 2, 4, 0, 3},
    "Pug, War, Cle, Thi, Psi, Mag",
    "",
    ELE_POISON,
    ELE_HOLY,
    ELE_POISON | ELE_PHYSICAL,
    ELE_HOLY | ELE_FIRE,
    FALSE},

   {
    "Gar", "Gargoyle", 3001, 0,
    21, 21, 11, 12, 20,
    RACE_MOD_IRON_SKIN | RACE_MOD_SLOW_HEAL | RACE_MOD_LARGE,
    {TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE, TRUE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE},
    MAX_CLASS, {0, 1, 4, 5, 2, 3},
    "Mag, War, Psi, Pug, Cle, Thi",
    "fly \'stone skin\'",
    ELE_FIRE | ELE_POISON,
    ELE_MENTAL,
    ELE_FIRE | ELE_POISON,
    ELE_MENTAL,
    TRUE},

};

const struct clan_type clan_table[MAX_CLAN] = {
   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },
   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },
   {
    "None", "None ", 0, 0,
    "N/A", " ", {-1, -1, -1, -1}

    },

};




const struct exp_type exp_table[MAX_MOB_LEVEL] = {
   {0, {1000, 1000, 1000, 1000, 1000}},   /*  0 */
   {100, {3050, 3023, 3022, 3021, 3020}}, /*  1 */
   {200, {8081, 8080, 8084, 8083, 8082}}, /*  2 */
   {300, {11183, 11182, 11181, 11184, 11185}},  /*  3 */
   {450, {20326, 20322, 20323, 20324, 20325}},  /*  4 */
   {600, {35504, 35505, 35506, 35507, 35503}},  /*  5 */
   {850, {38350, 38725, 38726, 38727, 38728}},  /*  6 */
   {1080, {54000, 53000, 54500, 53500, 53800}}, /*  7 */
   {1300, {65000, 65500, 64500, 65900, 64300}}, /*  8 */
   {1550, {77500, 77600, 77000, 78000, 77200}}, /*  9 */
   {1800, {108000, 108500, 109000, 107500, 107000}},  /* 10 */
   {2100, {126000, 126500, 125500, 125000, 127000}},  /* 11 */
   {2450, {147000, 148000, 146000, 146500, 147500}},  /* 12 */
   {2800, {168000, 167500, 168500, 169000, 167000}},  /* 13 */
   {3150, {189000, 188000, 190000, 188500, 189500}},  /* 14 */
   {3500, {210000, 211000, 205000, 210500, 210000}},  /* 15 */
   {3900, {234000, 233000, 234500, 235000, 239000}},  /* 16 */
   {4350, {261000, 262000, 261500, 260000, 260500}},  /* 17 */
   {4800, {288000, 287000, 287500, 289000, 288000}},  /* 18 */
   {5300, {318000, 317000, 317500, 318500, 319000}},  /* 19 */
   {5800, {348000, 347000, 349000, 348500, 347500}},  /* 20 */
   {6350, {381000, 382000, 380000, 380500, 381500}},  /* 21 */
   {6950, {417000, 416500, 417500, 418000, 418500}},  /* 22 */
   {7550, {453000, 452000, 454000, 453500, 452500}},  /* 23 */
   {8200, {492000, 491000, 493000, 492500, 491500}},  /* 24 */
   {8800, {528000, 527000, 528500, 529000, 527500}},  /* 25 */
   {9500, {570000, 575000, 565000, 560000, 580000}},  /* 26 */
   {10200, {612000, 611000, 613000, 612500, 611500}}, /* 27 */
   {11000, {660000, 655000, 665000, 670000, 650000}}, /* 28 */
   {11900, {714000, 715000, 714500, 713500, 714000}}, /* 29 */
   {13000, {780000, 785000, 790000, 770000, 775000}}, /* 30 */
   {14000, {840000, 841000, 840500, 841500, 840500}}, /* 31 */
   {15300, {918000, 917500, 918500, 919000, 918200}}, /* 32 */
   {16500, {990000, 985000, 987000, 995000, 993000}}, /* 33 */
   {18000, {1080000, 1090000, 1075000, 1085000, 1087000}},  /* 34 */
   {19500, {1170000, 1165000, 1175000, 1180000, 1160000}},  /* 35 */
   {22000, {1320000, 1315000, 1330000, 1325000, 1320000}},  /* 36 */
   {24500, {1470000, 1465000, 1475000, 1477000, 1460000}},  /* 37 */
   {27500, {1650000, 1660000, 1655000, 1657000, 1645000}},  /* 38 */
   {30000, {1800000, 1790000, 1810000, 1815000, 1795000}},  /* 39 */
   {33000, {1980000, 1975000, 1985000, 1983000, 1977000}},  /* 40 */
   {34000, {2040000, 2035000, 2045000, 2039000, 2050000}},  /* 41 */
   {35500, {2130000, 2135000, 2140000, 2125000, 2133000}},  /* 42 */
   {37500, {2250000, 2240000, 2255000, 2260000, 2245000}},  /* 43 */
   {40000, {2400000, 2450000, 2480000, 2350000, 2390000}},  /* 44 */
   {43000, {2580000, 2570000, 2585000, 2589000, 2575000}},  /* 45 */
   {46000, {2760000, 2755000, 2765000, 2770000, 2763000}},  /* 46 */
   {49500, {2970000, 2975000, 2980000, 2965000, 2973000}},  /* 47 */
   {52000, {3120000, 3115000, 3125000, 3200000, 3128000}},  /* 48 */
   {55000, {3300000, 3290000, 3310000, 3295000, 3300000}},  /* 49 */
   {56000, {3360000, 3355000, 3365000, 3370000, 3368000}},  /* 50 */
   {58000, {3480000, 3470000, 3485000, 3488000, 3475000}},  /* 51 */
   {60000, {3600000, 3610000, 3590000, 3605000, 3640000}},  /* 52 */
   {63000, {3780000, 3790000, 3770000, 3775000, 3785000}},  /* 53 */
   {66000, {3960000, 3940000, 3970000, 3965000, 3962000}},  /* 54 */
   {68000, {4080000, 4095000, 4085000, 4083000, 4073000}},  /* 55 */
   {71000, {4260000, 4250000, 4265000, 4255000, 4300000}},  /* 56 */
   {73000, {4380000, 4390000, 4385000, 4375000, 4387000}},  /* 57 */
   {76000, {4560000, 4570000, 4550000, 4555000, 4565000}},  /* 58 */
   {79000, {4740000, 4750000, 4745000, 4755000, 4760000}},  /* 59 */
   {81000, {4860000, 4840000, 4850000, 4855000, 4870000}},  /* 60 */
   {83000, {4980000, 4990000, 4975000, 4985000, 4990000}},  /* 61 */
   {85000, {5100000, 5110000, 5150000, 5090000, 5095000}},  /* 62 */
   {88000, {5280000, 5270000, 5285000, 5290000, 5275000}},  /* 63 */
   {89000, {5345000, 5350000, 5360000, 5350000, 5343000}},  /* 64 */
   {91000, {5460000, 5470000, 5450000, 5465000, 5456000}},  /* 65 */
   {93000, {5580000, 5560000, 5590000, 5575000, 5590000}},  /* 66 */
   {94000, {5640000, 5650000, 5630000, 5640000, 5620000}},  /* 67 */
   {95000, {5700000, 5710000, 5690000, 5720000, 5680000}},  /* 68 */
   {97000, {5820000, 5800000, 5830000, 5820000, 5810000}},  /* 69 */
   {98000, {5880000, 5820000, 5900000, 5860000, 5870000}},  /* 70 */
   {99000, {5940000, 5920000, 5980000, 5960000, 5930000}},  /* 71 */
   {99500, {5970000, 5980000, 6000000, 5800000, 5970000}},  /* 72 */
   {100000, {6000000, 6100000, 5950000, 6150000, 6050000}}, /* 73 */
   {103000, {6180000, 6190000, 6170000, 6190000, 6185000}}, /* 74 */
   {106000, {6360000, 6350000, 6400000, 6340000, 6360000}}, /* 75 */
   {110000, {6600000, 6610000, 6620000, 6590000, 6600000}}, /* 76 */
   {115000, {6910000, 6920000, 6900000, 6850000, 6900000}}, /* 77 */
   {120000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 78 */
   {130000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 79 */
   {140000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 80 */
   {150000, {6600000, 6610000, 6620000, 6590000, 6600000}}, /* 81 */
   {175000, {6910000, 6920000, 6900000, 6850000, 6900000}}, /* 82 */
   {190000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 83 */
   {200000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 84 */
   {210000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 85 */
   {220000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 86 */
   {235000, {6600000, 6610000, 6620000, 6590000, 6600000}}, /* 87 */
   {255000, {6910000, 6920000, 6900000, 6850000, 6900000}}, /* 88 */
   {260000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 89 */
   {270000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 90 */
   {280000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 91 */
   {300000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 92 */
   {310000, {6600000, 6610000, 6620000, 6590000, 6600000}}, /* 93 */
   {315000, {6910000, 6920000, 6900000, 6850000, 6900000}}, /* 94 */
   {320000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 95 */
   {330000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 96 */
   {340000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 97 */
   {340000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 98 */
   {350000, {6600000, 6610000, 6620000, 6590000, 6600000}}, /* 99 */
   {355000, {6910000, 6920000, 6900000, 6850000, 6900000}}, /* 100 */
   {350000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 101 */
   {350000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 102 */
   {350000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 103 */
   {365000, {6910000, 6920000, 6900000, 6850000, 6900000}}, /* 104 */
   {360000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 105 */
   {360000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 106 */
   {360000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 107 */
   {360000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 108 */
   {360000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 109 */
   {360000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 110 */
   {370000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 111 */
   {370000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 112 */
   {370000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 113 */
   {375000, {6910000, 6920000, 6900000, 6850000, 6900000}}, /* 114 */
   {388000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 115 */
   {380000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 116 */
   {380000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 117 */
   {380000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 118 */
   {380000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 119 */
   {380000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 120 */
   {385000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 121 */
   {385000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 122 */
   {386000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 123 */
   {387000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 124 */
   {389000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 125 */
   {390000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 126 */
   {410000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 127 */
   {421000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 128 */
   {432000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 129 */
   {444000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 130 */
   {460000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 131 */
   {500000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 132 */
   {510000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 133 */
   {520000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 134 */
   {525000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 135 */
   {530000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 136 */
   {530000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 137 */
   {535000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 138 */
   {540000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 139 */
   {550000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 140 */
   {560000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 141 */
   {570000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 142 */
   {580000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 143 */
   {590000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 144 */
   {600000, {6910000, 6920000, 6900000, 6850000, 6900000}}, /* 145 */
   {610000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 146 */
   {620000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 147 */
   {630000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 148 */
   {640000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 149 */
   {650000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 150 */
   {660000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 151 */
   {670000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 152 */
   {680000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 153 */
   {690000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 154 */
   {700000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 155 */
   {710000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 156 */
   {720000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 157 */
   {730000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 158 */
   {740000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 159 */
   {750000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 160 */
   {760000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 161 */
   {770000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 162 */
   {780000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 163 */
   {790000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 164 */
   {800000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 165 */
   {810000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 166 */
   {820000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 167 */
   {830000, {8000000, 8000000, 8000000, 8000000, 8000000}}, /* 168 */
   {850000, {7200000, 7250000, 7230000, 7190000, 7200000}}, /* 169 */
   {1000000, {7800000, 7700000, 7750000, 7780000, 7850000}}, /* 170 */
};

#define CLASS_MAGE       0
#define CLASS_CLERIC     1
#define CLASS_THIEF      2
#define CLASS_WARRIOR    3
#define CLASS_PSI        4
#define CLASS_PUGILIST   5

/*
 * Liquid properties.
 * Used in world.obj.
 */
const struct liq_type liq_table[LIQ_MAX] = {
   {"water", "clear", {0, 1, 10}},  /*  0 */
   {"beer", "amber", {3, 2, 5}},
   {"wine", "rose", {5, 2, 5}},
   {"ale", "brown", {2, 2, 5}},
   {"dark ale", "dark", {1, 2, 5}},

   {"whisky", "golden", {6, 1, 4}}, /*  5 */
   {"lemonade", "pink", {0, 1, 8}},
   {"firebreather", "boiling", {10, 0, 0}},
   {"local specialty", "everclear", {3, 3, 3}},
   {"slime mold juice", "green", {0, 4, -8}},

   {"milk", "white", {0, 3, 6}}, /* 10 */
   {"tea", "tan", {0, 1, 6}},
   {"coffee", "black", {0, 1, 6}},
   {"blood", "red", {0, 2, -1}},
   {"salt water", "clear", {0, 1, -2}},

   {"@@bchocolate milk@@N", "creamy", {0, 3, 6}},  /* 15 */
   {"mountain dew", "bubbly", {0, 1, 5}}
};

const float hr_damTable[121] = {
   0.311,
   0.325,
   0.341,
   0.358,
   0.378,
   0.399,
   0.423,
   0.449,
   0.478,
   0.509,
   0.543,
   0.579,
   0.618,
   0.659,
   0.703,
   0.749,
   0.797,
   0.846,
   0.897,
   0.948,
   1.000,
   1.012,
   1.024,
   1.036,
   1.048,
   1.060,
   1.071,
   1.083,
   1.094,
   1.105,
   1.117,
   1.127,
   1.138,
   1.149,
   1.159,
   1.169,
   1.178,
   1.188,
   1.197,
   1.206,
   1.215,
   1.223,
   1.231,
   1.239,
   1.247,
   1.254,
   1.261,
   1.268,
   1.274,
   1.281,
   1.287,
   1.292,
   1.298,
   1.303,
   1.308,
   1.313,
   1.317,
   1.322,
   1.326,
   1.330,
   1.333,
   1.337,
   1.340,
   1.344,
   1.347,
   1.350,
   1.352,
   1.355,
   1.357,
   1.360,
   1.362,
   1.364,
   1.366,
   1.368,
   1.370,
   1.372,
   1.373,
   1.375,
   1.376,
   1.377,
   1.379,
   1.380,
   1.381,
   1.382,
   1.383,
   1.384,
   1.385,
   1.386,
   1.387,
   1.387,
   1.388,
   1.389,
   1.389,
   1.390,
   1.391,
   1.391,
   1.392,
   1.392,
   1.393,
   1.393,
   1.393,
   1.394,
   1.394,
   1.395,
   1.395,
   1.395,
   1.395,
   1.396,
   1.396,
   1.396,
   1.396,
   1.397,
   1.397,
   1.397,
   1.397,
   1.397,
   1.397,
   1.398,
   1.398,
   1.398,
   1.398
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n) n

const struct skill_type skill_table[MAX_SKILL] = {

/*
 * Magic spells.
 */

   {
    MORTAL, NORM,
    "reserved", {199, 199, 199, 199, 199, 199},
    0, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 0 ), 0, 0, FALSE,
    "", "",
    ""},

   {
    MORTAL, NORM,
    "acid blast", {50, 182, 182, 182, 182, 182},
    spell_acid_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 70 ), 20, 12, TRUE,
    "acid blast", "!Acid Blast!",
    ""},

   {
    MORTAL, NORM,
    "armor", {3, 182, 182, 182, 182, 182},
    spell_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 1 ), 5, 12, FALSE,
    "", "You feel less protected.",
    ""},

   {
    REMORT, NORM,
    "animate", {182, 182, 182, 182, 70, 182,
                182, 182, 182, 182, 70, 182},
    spell_animate, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 525 ), 50, 24, FALSE,
    "", "!Animate!",
    ""},


   {
    MORTAL, NORM,
    "bad breath", {4, 182, 182, 182, 182, 182},
    spell_badbreath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 502 ), 10, 12, TRUE,
    "bad breath", "!Bad Breath!",
    ""},

   {
    MORTAL, NORM,
    "bark skin", {182, 20, 182, 182, 182, 182},
    spell_bark_skin, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 514 ), 20, 24, FALSE,
    "", "Your skin feels softer.",
    "$n's skin looks less wooden."},

   {
    MORTAL, NORM,
    "bless", {182, 8, 182, 182, 182, 182},
    spell_bless, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 3 ), 5, 12, FALSE,
    "", "You feel less righteous.",
    "$n looks less Holy."},

   {
    MORTAL, NORM,
    "blindness", {16, 182, 182, 182, 182, 182},
    spell_blindness, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_blindness, SLOT( 4 ), 5, 12, FALSE,
    "", "You can see again.",
    "$n's vision returns."},

   {
    MORTAL, NORM,
    "bloody tears", {182, 182, 182, 182, 26, 182},
    spell_bloody_tears, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 518 ), 12, 12, FALSE,
    "Bloody Tears", "!Bloody Tears!",
    ""},

   {
    MORTAL, NORM,
    "burning hands", {6, 182, 182, 182, 182, 182},
    spell_burning_hands, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 5 ), 15, 12, TRUE,
    "burning hands", "!Burning Hands!",
    ""},

   {
    MORTAL, NORM,
    "call lightning", {182, 36, 182, 182, 182, 182},
    spell_call_lightning, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 6 ), 15, 12, TRUE,
    "lightning bolt", "!Call Lightning!",
    ""},

   {
    MORTAL, NORM,
    "calm", {182, 182, 182, 182, 52, 182},
    spell_calm, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 531 ), 30, 24, FALSE,
    "", "!Calm!",
    ""},

   {
    MORTAL, NORM,
    "cause critical", {182, 58, 182, 182, 182, 182},
    spell_cause_critical, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 63 ), 20, 12, TRUE,
    "spell", "!Cause Critical!",
    ""},

   {
    MORTAL, NORM,
    "cause light", {182, 4, 182, 182, 182, 182},
    spell_cause_light, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 62 ), 15, 12, TRUE,
    "spell", "!Cause Light!",
    ""},

   {
    MORTAL, NORM,
    "cause serious", {182, 40, 182, 182, 182, 182},
    spell_cause_serious, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 64 ), 17, 12, TRUE,
    "spell", "!Cause Serious!",
    ""},

   {
    MORTAL, NORM,
    "change sex", {66, 182, 182, 182, 182, 182},
    spell_change_sex, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 82 ), 15, 12, FALSE,
    "", "Your body feels familiar again.",
    "$n looks $mself again."},

   {
    MORTAL, NORM,
    "charm person", {49, 60, 182, 182, 182, 182},
    spell_charm_person, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_charm_person, SLOT( 7 ), 5, 12, FALSE,
    "", "You feel more self-confident.",
    "The glazed look in $n's eyes fades."},

   {
    MORTAL, NORM,
    "chill touch", {8, 182, 182, 182, 182, 182},
    spell_chill_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 8 ), 15, 12, TRUE,
    "chilling touch", "You feel less cold.",
    ""},

   {
    MORTAL, NORM,
    "color spray", {7, 182, 182, 182, 182, 182},
    spell_color_spray, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 10 ), 15, 12, TRUE,
    "color spray", "!color Spray!",
    ""},

   {
    MORTAL, NORM,
    "continual light", {11, 27, 182, 182, 182, 182},
    spell_continual_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 57 ), 7, 12, FALSE,
    "", "!Continual Light!",
    ""},

   {
    MORTAL, NORM,
    "control weather", {58, 79, 182, 182, 182, 182},
    spell_control_weather, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 11 ), 25, 12, FALSE,
    "", "!Control Weather!",
    ""},

   {
    MORTAL, NORM,
    "create food", {182, 26, 182, 182, 182, 182},
    spell_create_food, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 12 ), 5, 12, FALSE,
    "", "!Create Food!",
    ""},

   {
    MORTAL, NORM,
    "create spring", {182, 65, 182, 182, 182, 182},
    spell_create_spring, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 80 ), 20, 12, FALSE,
    "", "!Create Spring!",
    ""},

   {
    MORTAL, NORM,
    "create water", {182, 9, 182, 182, 182, 182},
    spell_create_water, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 13 ), 5, 12, FALSE,
    "", "!Create Water!",
    ""},

   {
    MORTAL, NORM,
    "cure blindness", {182, 28, 182, 182, 182, 182},
    spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 14 ), 5, 12, FALSE,
    "", "!Cure Blindness!",
    ""},

   {
    MORTAL, NORM,
    "cure critical", {182, 182, 182, 182, 182, 182},
    spell_cure_critical, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 15 ), 20, 12, FALSE,
    "", "!Cure Critical!",
    ""},

   {
    MORTAL, NORM,
    "cure light", {182, 182, 182, 182, 182, 182},
    spell_cure_light, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 16 ), 10, 12, FALSE,
    "", "!Cure Light!",
    ""},

   {
    MORTAL, NORM,
    "cure poison", {182, 32, 182, 182, 182, 182},
    spell_cure_poison, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 43 ), 5, 12, FALSE,
    "", "!Cure Poison!",
    ""},

   {
    MORTAL, NORM,
    "purify", {182, 182, 182, 182, 19, 182},
    spell_purify, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 231 ), 5, 12, FALSE,
    "Purify", "!Purify!",
    ""},

   {
    MORTAL, NORM,
    "cure serious", {182, 182, 182, 182, 182, 182},
    spell_cure_serious, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 61 ), 15, 12, TRUE,
    "", "!Cure Serious!",
    ""},

   {
    MORTAL, NORM,
    "curse", {35, 26, 182, 182, 182, 182},
    spell_curse, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_curse, SLOT( 17 ), 20, 12, FALSE,
    "curse", "The curse wears off.",
    "$n starts to look more Holy."},

   {
    MORTAL, NORM,
    "detect evil", {12, 8, 182, 182, 182, 182},
    spell_detect_evil, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 18 ), 5, 12, FALSE,
    "", "The red in your vision disappears.",
    ""},

   {
    MORTAL, NORM,
    "detect hidden", {17, 16, 182, 182, 182, 182},
    spell_detect_hidden, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 44 ), 5, 12, FALSE,
    "", "You feel less aware of your suroundings.",
    ""},

   {
    MORTAL, NORM,
    "detect invis", {19, 182, 182, 182, 182, 182},
    spell_detect_invis, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 19 ), 5, 12, FALSE,
    "", "You no longer see invisible objects.",
    ""},

   {
    MORTAL, NORM,
    "detect magic", {7, 7, 182, 182, 182, 182},
    spell_detect_magic, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 20 ), 5, 12, FALSE,
    "", "The detect magic wears off.",
    ""},

   {
    MORTAL, NORM,
    "detect poison", {182, 10, 182, 182, 182, 182},
    spell_detect_poison, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 21 ), 5, 12, FALSE,
    "", "!Detect Poison!",
    ""},

   {
    MORTAL, NORM,
    "detect undead", {8, 182, 182, 182, 182, 182},
    spell_detect_undead, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 513 ), 8, 12, FALSE,
    "", "You no longer sense undead beings.",
    ""},

   {
    MORTAL, NORM,
    "detection", {182, 182, 182, 182, 22, 182},
    spell_detection, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 527 ), 12, 24, FALSE,
    "", "!Detection!",
    ""},

   {
    MORTAL, NORM,
    "dimension blade", {186, 186, 186, 186, 186, 186},
    spell_dimension_blade, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 523 ), 100, 41, FALSE,
    "", "!Dimension Blade!",
    ""},

   {
    MORTAL, NORM,
    "dispel evil", {182, 182, 182, 182, 182, 182},  /* was 82, 63, ... */
    spell_dispel_evil, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 22 ), 15, 12, FALSE,
    "dispel evil", "!Dispel Evil!",
    ""},

   {
    MORTAL, NORM,
    "dispel object", {60, 70, 185, 185, 80, 185},
    spell_dispel_object, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 0 ), 15, 12, FALSE,
    "", "!Dispel Object!",
    ""},

   {
    MORTAL, NORM,
    "dispel magic", {68, 89, 185, 185, 185, 185},
    spell_dispel_magic, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 59 ), 15, 12, FALSE,
    "", "!Dispel Magic!",
    ""},

   {
    MORTAL, NORM,
    "purge", {185, 185, 185, 185, 50, 185},
    spell_dispel_magic, TAR_CHAR_NOTSELF, POS_FIGHTING,
    NULL, SLOT( 0 ), 15, 12, FALSE,
    "", "!Purge!",
    ""},

   {
    MORTAL, NORM,
    "earthquake", {182, 78, 182, 182, 182, 182},
    spell_earthquake, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 23 ), 15, 12, TRUE,
    "earthquake", "!Earthquake!",
    ""},


   {
    MORTAL, NORM,
    "ego whip", {182, 182, 182, 182, 25, 182},
    spell_ego_whip, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 535 ), 35, 24, TRUE,
    "ego whip", "!Ego Whip!",
    ""},


   {
    MORTAL, NORM,
    "enchant weapon", {39, 182, 182, 182, 182, 182},
    spell_enchant_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 24 ), 100, 24, FALSE,
    "", "!Enchant Weapon!",
    ""},

   {
    MORTAL, VAMP,
    "energy drain", {15, 7, 8, 11, 9, 13},
    spell_energy_drain, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 25 ), 12, 12, TRUE,
    "energy drain", "!Energy Drain!",
    ""},

   {
    MORTAL, NORM,
    "enhance weapon", {186, 186, 186, 186, 186, 186},
    spell_enhance_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 517 ), 50, 24, FALSE,
    "", "!Enhance Weapon!",
    ""},

   {
    MORTAL, NORM,
    "faerie fire", {3, 182, 182, 182, 182, 182},
    spell_faerie_fire, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 72 ), 5, 12, TRUE,
    "faerie fire", "The pink aura around you fades away.",
    "The pink aura around $n fades away."},

   {
    MORTAL, NORM,
    "faerie fog", {182, 14, 182, 182, 182, 182},
    spell_faerie_fog, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 73 ), 12, 12, FALSE,
    "faerie fog", "!Faerie Fog!",
    ""},

   {
    MORTAL, NORM,
    "fighting trance", {182, 182, 182, 182, 60, 182},
    spell_fighting_trance, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 521 ), 60, 24, FALSE,
    "", "Your fighting trance fades.",
    "$n's fighting trance fades."},

   {
    MORTAL, NORM,
    "fireball", {28, 182, 182, 182, 182, 182},
    spell_fireball, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 26 ), 15, 12, TRUE,
    "fireball", "!Fireball!",
    ""},

   {
    MORTAL, NORM,
    "flamestrike", {182, 54, 182, 182, 182, 182},
    spell_flamestrike, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 65 ), 20, 12, TRUE,
    "flamestrike", "!Flamestrike!",
    ""},

   {
    MORTAL, NORM,
    "flare", {182, 20, 182, 182, 182, 182},
    spell_flare, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 591 ), 18, 12, FALSE,
    "flare", "Your vision returns.",
    "$n blinks, and starts to see again."},


   {
    MORTAL, NORM,
    "fly", {24, 182, 182, 182, 182, 182},
    spell_fly, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 56 ), 10, 18, FALSE,
    "", "You slowly float to the ground.",
    "$n slowly floats to the ground."},

   {
    MORTAL, NORM,
    "gate", {182, 182, 182, 182, 182, 182},
    spell_gate, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 83 ), 50, 12, FALSE,
    "", "!Gate!",
    ""},

   {
    REMORT, NORM,
    "giant strength", {18, 182, 182, 182, 182, 182,
                       18, 182, 182, 182, 182, 182},
    spell_giant_strength, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 39 ), 20, 12, FALSE,
    "", "You feel weaker.",
    "$n looks weaker."},

   {
    MORTAL, NORM,
    "harm", {182, 74, 182, 182, 182, 182},
    spell_harm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 27 ), 35, 12, TRUE,
    "harm spell", "!Harm!",
    ""},

   {
    MORTAL, NORM,
    "heal", {182, 5, 182, 182, 182, 182},
    spell_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 28 ), 50, 12, TRUE,
    "", "!Heal!",
    ""},

   {
    REMORT, NORM,
    "group heal", {182, 5, 182, 182, 182, 182,
                   182, 25, 182, 182, 182, 182},
    spell_group_heal, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 169 ), 50, 12, TRUE,
    "", "!Group Heal!",
    ""},

   {
    MORTAL, NORM,
    "hypnosis", {182, 182, 182, 182, 14, 182},
    spell_hypnosis, TAR_CHAR_OFFENSIVE, POS_STANDING,
    NULL, SLOT( 532 ), 30, 24, FALSE,
    "", "!Hypnosis!",
    ""},

   {
    MORTAL, NORM,
    "identify", {9, 182, 182, 182, 182, 182},
    spell_identify, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 53 ), 12, 24, FALSE,
    "", "!Identify!",
    ""},

   {
    MORTAL, NORM,
    "infravision", {20, 182, 182, 182, 182, 182},
    spell_infravision, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 77 ), 5, 18, FALSE,
    "", "You no longer see in the dark.",
    ""},

   {
    MORTAL, NORM,
    "influx", {182, 65, 182, 182, 182, 182},
    spell_influx, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 503 ), 75, 24, TRUE,
    "", "!Influx!",
    ""},

   {
    MORTAL, NORM,
    "invis", {17, 182, 182, 182, 182, 182},
    spell_invis, TAR_IGNORE, POS_STANDING,
    &gsn_invis, SLOT( 29 ), 5, 12, FALSE,
    "", "You are no longer invisible.",
    "$n's invisibilty fades."},

   {
    MORTAL, NORM,
    "know alignment", {13, 22, 182, 182, 182, 182},
    spell_know_alignment, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 58 ), 9, 12, FALSE,
    "", "!Know Alignment!",
    ""},



   {
    MORTAL, NORM,
    "know weakness", {182, 182, 182, 182, 18, 182},
    spell_know_weakness, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 530 ), 15, 12, TRUE,
    "", "You are less aware of your enemy's weaknesses.",
    ""},


   {
    MORTAL, NORM,
    "know critical", {182, 182, 182, 182, 28, 182},
    spell_know_weakness, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 530 ), 15, 12, TRUE,
    "", "You are less aware of critical damage points.",
    ""},


   {
    MORTAL, NORM,
    "know item", {182, 182, 182, 182, 9, 182},
    spell_identify, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 533 ), 20, 24, FALSE,
    "", "!Know Item!",
    ""},

   {
    MORTAL, NORM,
    "laser bolt", {182, 182, 182, 182, 182, 182},
    spell_laserbolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 511 ), 35, 12, FALSE,
    "laserbolt", "!laserbolt!",
    ""},


   {
    MORTAL, NORM,
    "lightning bolt", {24, 182, 182, 182, 182, 182},
    spell_lightning_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 30 ), 15, 12, TRUE,
    "lightning bolt", "!Lightning Bolt!",
    ""},

   {
    MORTAL, NORM,
    "locate object", {16, 30, 182, 182, 182, 182},
    spell_locate_object, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 31 ), 20, 18, FALSE,
    "", "!Locate Object!",
    ""},

   {
    MORTAL, NORM,
    "magic missile", {3, 182, 182, 182, 182, 182},
    spell_magic_missile, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 32 ), 15, 12, TRUE,
    "magic missile", "!Magic Missile!",
    ""},

   {
    MORTAL, NORM,
    "mass invis", {60, 182, 182, 182, 182, 182},
    spell_mass_invis, TAR_IGNORE, POS_STANDING,
    &gsn_mass_invis, SLOT( 69 ), 20, 24, FALSE,
    "", "!Mass Invis!",
    ""},


   {
    MORTAL, NORM,
    "mind flail", {182, 182, 182, 182, 4, 182},
    spell_mind_flail, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 536 ), 12, 24, TRUE,
    "mind flail", "!Mind Flail!",
    ""},

   {
    MORTAL, NORM,
    "mystic armour", {10, 182, 182, 182, 182, 182},
    spell_mystic_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 590 ), 18, 24, FALSE,
    "" "Your Mystic Armour fades away",
    "The Mystic Armour around someone in the room fades away."},


   {
    MORTAL, NORM,
    "nerve fire", {182, 182, 182, 182, 60, 182},
    spell_nerve_fire, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 520 ), 50, 24, TRUE,
    "nerve fire", "!Nerve Fire!",
    ""},


   {
    MORTAL, NORM,
    "night vision", {182, 182, 182, 182, 19, 182},
    spell_night_vision, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 538 ), 17, 24, FALSE,
    "", "Your eyes feel weaker.",
    ""},


   {
    MORTAL, NORM,
    "pass door", {46, 182, 182, 182, 182, 182},
    spell_pass_door, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 74 ), 20, 12, FALSE,
    "", "You feel solid again.",
    "$n's body becomes more solid."},

   {
    MORTAL, NORM,
    "phase", {182, 182, 182, 182, 45, 182},
    spell_phase, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 522 ), 20, 12, FALSE,
    "", "You feel solid again.",
    "$n's body becomes more solid."},



   {
    MORTAL, NORM,
    "psychic crush", {182, 182, 182, 182, 19, 182},
    spell_physic_thrust, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 537 ), 33, 24, TRUE,
    "psychic crush", "!Psychic Crush!",
    ""},

   {
    MORTAL, NORM,
    "psychic thrust", {182, 182, 182, 182, 7, 182},
    spell_physic_thrust, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 537 ), 22, 24, TRUE,
    "psychic thrust", "!Physic Thrust!",
    ""},


   {
    MORTAL, NORM,
    "poison", {182, 31, 182, 182, 182, 182},
    spell_poison, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_poison, SLOT( 33 ), 10, 12, FALSE,
    "poison", "You feel less sick.",
    "$n looks less sick."},

   {
    MORTAL, NORM,
    "produce food", {182, 182, 182, 182, 24, 182},
    spell_produce_food, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 524 ), 16, 24, FALSE,
    "", "!Produce Food!",
    ""},

   {
    MORTAL, NORM,
    "protection", {9, 12, 182, 182, 182, 182},
    spell_protection, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 34 ), 5, 12, FALSE,
    "", "You feel less protected.",
    ""},

   {
    MORTAL, NORM,
    "psionic recovery", {182, 182, 182, 182, 5, 182},
    spell_psionic_recovery, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 78 ), 5, 12, TRUE,
    "", "You feel less protected.",
    ""},

   {
    MORTAL, NORM,
    "refresh", {182, 11, 182, 182, 182, 182},
    spell_refresh, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 81 ), 12, 18, FALSE,
    "refresh", "!Refresh!",
    ""},

   {
    MORTAL, NORM,
    "regen", {5, 182, 182, 182, 182, 182},
    spell_regen, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 271 ), 12, 18, TRUE,
    "", "You are no longer regenerating quickly.",
    "$n is no longer regenerating quickly."},

   {
    REMORT, NORM,
    "haste", {182, 182, 182, 182, 182, 182,
              84, 182, 182, 182, 182, 182},
    spell_haste, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 271 ), 12, 18, TRUE,
    "", "You are no longer moving quickly.",
    "$n is no longer moving quickly."},

   {
    REMORT, NORM,
    "overdrive", {182, 182, 182, 182, 182, 182,
                 182, 182, 182, 182, 57, 182},
    spell_overdrive, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 271 ), 12, 18, TRUE,
    "", "You are no longer moving quickly.",
    "$n is no longer moving quickly."},   {

    REMORT, NORM,
    "magical supremacy", {182, 182, 182, 182, 182, 182,
                 42, 182, 182, 182, 13, 182},
    spell_magical_supremacy, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 271 ), 12, 18, TRUE,
    "", "You no longer have magical supremacy.",
    "$n no longer has magical supremacy."},


   {
    REMORT, NORM,
    "slow", {182, 182, 182, 182, 182, 182,
              92, 182, 182, 182, 182, 182},
    spell_slow, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 271 ), 12, 18, TRUE,
    "", "You are now moving quickly.",
    "$n is now moving quickly."},

   {
    MORTAL, NORM,
    "remove curse", {36, 27, 182, 182, 182, 182},
    spell_remove_curse, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 35 ), 5, 12, FALSE,
    "", "!Remove Curse!",
    ""},

   {
    MORTAL, NORM,
    "sanctuary", {182, 70, 182, 182, 182, 182},
    spell_sanctuary, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 36 ), 75, 12, FALSE,
    "", "The white aura around your body fades.",
    "The white aura around $n's body fades."},

   {
    MORTAL, NORM,
    "seal room", {70, 50, 182, 182, 50, 182},
    spell_seal_room, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 541 ), 75, 12, FALSE,
    "", "The Energy web around this room fades.",
    ""},

   {
    MORTAL, NORM,
    "see magic", {182, 182, 182, 182, 7, 182},
    spell_see_magic, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 526 ), 8, 12, FALSE,
    "", "You no longer see magical auras.",
    ""},

   {
    MORTAL, NORM,
    "sense evil", {182, 182, 182, 182, 10, 182},
    spell_sense_evil, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 515 ), 12, 12, FALSE,
    "", "You no longer sense evil.",
    ""},

   {
    MORTAL, VAMP,
    "shade", {12, 7, 6, 8, 7, 13},
    spell_shade, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 542 ), 5, 12, FALSE,
    "", "The room gets lighter.",
    ""},

   {
    MORTAL, NORM,
    "shield", {9, 182, 182, 182, 182, 182},
    spell_shield, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 67 ), 12, 18, TRUE,
    "", "Your force shield shimmers, then fades away.",
    "$n's force field shimmers, then fades away."},

   {
    MORTAL, NORM,
    "shocking grasp", {5, 182, 182, 182, 182, 182},
    spell_shocking_grasp, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 82 ), 15, 12, TRUE,
    "shocking grasp", "!Shocking Grasp!",
    ""},


   {
    MORTAL, NORM,
    "sight", {30, 182, 182, 182, 182, 182},
    spell_cure_blindness, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 592 ), 20, 24, FALSE,
    "", "!Sight!",
    ""},


   {
    MORTAL, NORM,
    "sleep", {9, 182, 182, 182, 182, 182},
    spell_sleep, TAR_CHAR_OFFENSIVE, POS_STANDING,
    &gsn_sleep, SLOT( 38 ), 15, 12, FALSE,
    "", "You feel less tired.",
    "$n starts looking much more awake."},


   {
    MORTAL, NORM,
    "stalker", {182, 182, 182, 182, 182, 182},
    spell_stalker, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 401 ), 100, 24, FALSE,
    "", "!Stalker!",
    ""},


   {
    MORTAL, NORM,
    "stone skin", {36, 182, 182, 182, 182, 182},
    spell_stone_skin, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 66 ), 12, 18, TRUE,
    "", "Your skin feels soft again.",
    "$n's skin loses it's stone-like look."},

   {
    MORTAL, NORM,
    "suffocate", {182, 182, 182, 182, 46, 182},
    spell_suffocate, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 516 ), 30, 24, TRUE,
    "a choking hand", "!Suffocate!",
    ""},

   {
    MORTAL, NORM,
    "summon", {35, 182, 182, 182, 182, 182},
    spell_summon, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 40 ), 50, 12, FALSE,
    "", "!Summon!",
    ""},

   {
    MORTAL, NORM,
    "teleport", {40, 182, 182, 182, 182, 182},
    spell_teleport, TAR_CHAR_SELF, POS_FIGHTING,
    NULL, SLOT( 2 ), 35, 12, FALSE,
    "", "!Teleport!",
    ""},

   {
    MORTAL, NORM,
    "travel", {182, 182, 182, 182, 182, 182},
    spell_travel, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 599 ), 50, 24, FALSE,
    "", "!Travel!",
    ""},

   {
    MORTAL, NORM,
    "ventriloquate", {3, 182, 182, 182, 182, 182},
    spell_ventriloquate, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 41 ), 5, 12, FALSE,
    "", "!Ventriloquate!",
    ""},

   {
    MORTAL, NORM,
    "weaken", {182, 20, 182, 182, 182, 182},
    spell_weaken, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 68 ), 20, 12, FALSE,
    "spell", "You feel stronger.",
    "$n looks stronger."},

   {
    MORTAL, NORM,
    "beacon", {70, 182, 182, 182, 70, 182},
    spell_beacon, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 399 ), 25, 12, FALSE,
    "", "!Beacon!",
    ""},

   {
    MORTAL, NORM,
    "portal", {80, 182, 182, 182, 182, 182},
    spell_portal, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 334 ), 100, 12, FALSE,
    "", "!Portal!",
    ""},


   {
    MORTAL, NORM,
    "window", {70, 182, 182, 182, 70, 182},
    spell_window, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 333 ), 100, 12, FALSE,
    "", "!Window!",
    ""},


   {
    MORTAL, NORM,
    "word of recall", {182, 66, 182, 182, 182, 182},
    spell_word_of_recall, TAR_CHAR_SELF, POS_RESTING,
    NULL, SLOT( 42 ), 5, 12, FALSE,
    "", "!Word of Recall!",
    ""},


   {
    MORTAL, NORM,
    "hellspawn", {43, 182, 182, 182, 182, 182},
    spell_hellspawn, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 512 ), 50, 12, TRUE,
    "HellSpawn", "!Hellspawn!",
    ""},


   {
    MORTAL, NORM,
    "planergy", {182, 182, 182, 182, 5, 182},
    spell_planergy, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 504 ), 25, 24, TRUE,
    "energy", "!planergy!",
    ""},

   {
    MORTAL, NORM,
    "visit", {182, 182, 182, 182, 32, 182},
    spell_visit, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 505 ), 50, 48, FALSE,
    "", "!visit!",
    ""},

   {
    MORTAL, NORM,
    "barrier", {182, 182, 182, 182, 8, 182},
    spell_barrier, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 506 ), 30, 24, TRUE,
    "", "Your barrier slowly fades.",
    "The barrier around $n fades."},

   {
    MORTAL, NORM,
    "phobia", {182, 182, 182, 182, 10, 182},
    spell_phobia, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 507 ), 32, 24, FALSE,
    "phobia attack", "!phobia!",
    ""},

   {
    MORTAL, NORM,
    "mind bolt", {182, 182, 182, 182, 12, 182},
    spell_mind_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 519 ), 40, 12, TRUE,
    "Mind Bolt", "!MindBolt!",
    ""},

   {
    MORTAL, NORM,
    "mindflame", {182, 182, 182, 182, 35, 182},
    spell_mindflame, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 508 ), 40, 24, TRUE,
    "Mind Flame", "!mindflame!",
    ""},



   {
    MORTAL, NORM,
    "chain lightning", {65, 182, 182, 182, 182, 182},
    spell_chain_lightning, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 509 ), 25, 24, TRUE,
    "lightening bolt", "!chain-light!",
    ""},

   {
    MORTAL, NORM,
    "static", {182, 182, 182, 182, 20, 182},
    spell_static, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 510 ), 40, 24, TRUE,
    "discharge", "!static!",
    ""},



   {
    REMORT, NORM,
    "cloak:absorption", {60, 182, 182, 182, 182, 43,
                         60, 182, 182, 182, 182, 43},
    spell_cloak_absorb, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 221 ), 500, 12, FALSE,
    "", "@@NThe @@lcloak@@N around your body fades.",
    "@@NThe @@lcloak@@N around $n's body fades."},



   {
    REMORT, NORM,
    "cloak:reflection", {48, 182, 182, 182, 182, 182,
                         48, 182, 182, 182, 182, 182},
    spell_cloak_reflect, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 222 ), 500, 12, FALSE,
    "", "@@NThe @@lc@@el@@ro@@ya@@ak@@N around your body fades.",
    "@@NThe @@lc@@el@@ro@@ya@@ak@@N around $n's body fades."},



   {
    REMORT, NORM,
    "cloak:flaming", {70, 182, 182, 182, 60, 78,
                      70, 182, 182, 182, 60, 78},
    spell_cloak_flaming, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 223 ), 750, 12, FALSE,
    "", "@@NThe @@ecloak@@N around your body fades.",
    "@@NThe @@ecloak@@N around $n's body fades."},



   {
    MORTAL, VAMP,
    "cloak:darkness", {18, 13, 14, 14, 16, 15},
    spell_cloak_darkness, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 224 ), 45, 12, FALSE,
    "", "The @@dcloak@@N around your body fades.",
    "@@NThe @@dcloak@@N around $n's body fades."},





   {
    ADEPT, NORM,
    "cloak:adept", {21, 21, 21, 21, 21, 21},
    spell_cloak_adept, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 226 ), 500, 12, FALSE,
    "", "@@NThe @@Wcloak@@N around your body fades.",
    "@@NThe @@Wcloak@@N around $n's body fades."},

   {
    MORTAL, NORM,
    "acid breath", {182, 182, 182, 182, 182, 182},
    spell_acid_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 200 ), 0, 4, FALSE,
    "blast of acid", "!Acid Breath!",
    ""},

   {
    MORTAL, NORM,
    "fire breath", {182, 182, 182, 182, 182, 182},
    spell_fire_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 201 ), 0, 4, FALSE,
    "blast of flame", "!Fire Breath!",
    ""},

   {
    MORTAL, NORM,
    "frost breath", {182, 182, 182, 182, 182, 182},
    spell_frost_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 202 ), 0, 4, FALSE,
    "blast of frost", "!Frost Breath!",
    ""},

   {
    MORTAL, NORM,
    "gas breath", {182, 182, 182, 182, 182, 182},
    spell_gas_breath, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 203 ), 0, 4, FALSE,
    "blast of gas", "!Gas Breath!",
    ""},

   {
    MORTAL, NORM,
    "lightning breath", {182, 182, 182, 182, 182, 182},
    spell_lightning_breath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 204 ), 0, 4, FALSE,
    "blast of lightning", "!Lightning Breath!",
    ""},

   {
    MORTAL, VAMP,
    "blood leach", {8, 4, 3, 4, 4, 5},
    spell_blood_leach, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 540 ), 6, 12, FALSE,
    "", "Your body feels stronger again.",
    ""},


/*
 * Fighter and thief skills.
 */


   {
    MORTAL, NORM,
    "appraise", {182, 182, 12, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_appraise, SLOT( 0 ), 0, 24, FALSE,
    "", "!Appraise!",
    ""},


   {
    MORTAL, NORM,
    "bash", {182, 182, 182, 28, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_bash, SLOT( 0 ), 0, 24, TRUE,
    "bash", "!bash!",
    ""},


   {
    MORTAL, NORM,
    "berserk", {182, 182, 182, 34, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_beserk, SLOT( 0 ), 0, 24, TRUE,
    "berserk", "You calm down!",
    "$n looks calmer!"},


   {
    MORTAL, NORM,
    "climb", {182, 182, 25, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_climb, SLOT( 0 ), 0, 24, FALSE,
    "", "!Climb!",
    ""},

   {
    MORTAL, NORM,
    "dirt", {182, 182, 36, 35, 182, 47},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dirt, SLOT( 0 ), 1, 24, TRUE,
    "", "!dirt!",
    ""},

   {
    MORTAL, NORM,
    "nodisarm", {182, 182, 40, 30, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_nodisarm, SLOT( 0 ), 0, 0, FALSE,
    "", "!nodisarm!",
    ""},

   {
    MORTAL, NORM,
    "smash", {182, 182, 182, 30, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_smash, SLOT( 0 ), 0, 0, FALSE,
    "", "!smash!",
    ""},

   {
    MORTAL, NORM,
    "circle", {182, 182, 19, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_circle, SLOT( 0 ), 1, 18, TRUE,
    "circle", "!circle!",
    ""},

   {
    MORTAL, NORM,
    "backstab", {182, 182, 10, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_backstab, SLOT( 0 ), 1, 18, TRUE,
    "backstab", "!Backstab!",
    ""},

   {
    MORTAL, NORM,
    "disarm", {182, 182, 53, 12, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_disarm, SLOT( 0 ), 1, 18, TRUE,
    "", "!Disarm!",
    ""},

   {
    MORTAL, NORM,
    "dodge", {182, 182, 7, 182, 182, 27},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dodge, SLOT( 0 ), 0, 0, FALSE,
    "", "!Dodge!",
    ""},

   {
    REMORT, NORM,
    "dualwield", {182, 182, 5, 182, 182, 182,
                  182, 182, 5, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dualwield, SLOT( 0 ), 0, 0, FALSE,
    "", "!DualWield!",
    ""},

   {
    MORTAL, NORM,
    "enhanced damage", {182, 182, 182, 42, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_enhanced_damage, SLOT( 0 ), 0, 0, FALSE,
    "", "!Enhanced Damage!",
    ""},

   {
    REMORT, NORM,
    "equip buckler", {182, 1, 182, 1, 182, 182,
                      182, 182, 182, 1, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_equip_buckler, SLOT( 0 ), 0, 0, FALSE,
    "", "!Equip Buckler!",
    ""},

   {
    REMORT, NORM,
    "equip fist", {182, 182, 182, 182, 182, 1,
                   182, 182, 182, 182, 182, 1},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_equip_fist, SLOT( 0 ), 0, 0, FALSE,
    "", "!Equip Fist!",
    ""},

   {
    REMORT, NORM,
    "dual fist", {182, 182, 182, 182, 182, 35,
                   182, 182, 182, 182, 182, 35},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_dual_fist, SLOT( 0 ), 0, 0, FALSE,
    "", "!Dual Fist!",
    ""},

   {
    REMORT, NORM,
    "equip wand", {1, 182, 182, 182, 1, 182,
                   1, 1, 182, 182, 1, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_equip_wand, SLOT( 0 ), 0, 0, FALSE,
    "", "!Equip Wand!",
    ""},

   {
    MORTAL, VAMP,
    "feed", {1, 1, 1, 1, 1, 1},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_feed, SLOT( 0 ), 0, 24, FALSE,
    "", "!feed!",
    ""},

   {
    MORTAL, NORM,
    "flee timer", {182, 182, 182, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_flee_timer, SLOT( 0 ), 0, 24, FALSE,
    "", "Your anti-flee timer has expired!",
    ""},

   {
    MORTAL, NORM,
    "find doors", {182, 182, 16, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_find_doors, SLOT( 0 ), 0, 0, FALSE,
    "", "!Find Doors!",
    ""},
   {
    MORTAL, NORM,
    "counter", {182, 182, 182, 182, 182, 22},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_counter, SLOT( 0 ), 0, 0, FALSE,
    "counter", "!Counter!",
    ""},

   {
    MORTAL, NORM,
    "enhanced critical", {182, 182, 21, 182, 182, 122},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_enhanced_critical, SLOT( 0 ), 0, 0, FALSE,
    "", "!Enhanced Crit!",
    ""},

   {
    MORTAL, NORM,
    "spell critical", {182, 182, 182, 182, 31, 122},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_spell_critical, SLOT( 0 ), 0, 0, FALSE,
    "", "!Spell Crit!",
    ""},

   {
    MORTAL, NORM,
    "spell critical damage", {182, 42, 182, 182, 182, 122},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_spell_critical_damage, SLOT( 0 ), 0, 0, FALSE,
    "", "!Spell Critical Damage!",
    ""},

   {
    MORTAL, NORM,
    "headbutt", {182, 182, 182, 6, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_headbutt, SLOT( 0 ), 1, 18, TRUE,
    "headbutt", "!HeadButt!",
    ""},


   {
    MORTAL, NORM,
    "hide", {182, 182, 12, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_RESTING,
    &gsn_hide, SLOT( 0 ), 0, 12, FALSE,
    "", "!Hide!",
    ""},

   {
    MORTAL, NORM,
    "hunt", {182, 182, 70, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_RESTING,
    &gsn_hunt, SLOT( 0 ), 0, 12, FALSE,
    "", "!Hunt!",
    ""},

   {
    MORTAL, NORM,
    "kick", {182, 182, 182, 14, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_kick, SLOT( 0 ), 1, 18, TRUE,
    "kick", "!Kick!",
    ""},

   {
    MORTAL, NORM,
    "knee", {182, 182, 182, 8, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_knee, SLOT( 0 ), 1, 18, TRUE,
    "knee", "!Knee",
    ""},

   {
    MORTAL, NORM,
    "detox", {182, 182, 27, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_detox, SLOT( 0 ), 0, 24, FALSE,
    "detox", "!Detox",
    ""},

   {
    MORTAL, NORM,
    "martial arts", {182, 182, 182, 182, 182, 10},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_martial_arts, SLOT( 0 ), 0, 0, FALSE,
    "", "!Martial Arts!",
    ""},

   {
    MORTAL, NORM,
    "parry", {182, 182, 52, 10, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_parry, SLOT( 0 ), 0, 0, FALSE,
    "", "!Parry!",
    ""},

   {
    MORTAL, NORM,
    "peek", {182, 182, 20, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_peek, SLOT( 0 ), 0, 0, FALSE,
    "", "!Peek!",
    ""},

   {
    MORTAL, NORM,
    "pick lock", {182, 182, 25, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_pick_lock, SLOT( 0 ), 0, 12, FALSE,
    "", "!Pick!",
    ""},


   {
    MORTAL, NORM,
    "punch", {182, 182, 182, 5, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_punch, SLOT( 0 ), 1, 18, TRUE,
    "punch", "!Punch!",
    ""},

   {
    MORTAL, NORM,
    "rescue", {182, 182, 182, 15, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_rescue, SLOT( 0 ), 0, 12, FALSE,
    "", "!Rescue!",
    ""},

   {
    MORTAL, NORM,
    "shadowform", {182, 182, 182, 182, 9, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_shadow, SLOT( 0 ), 0, 0, FALSE,
    "", NULL,
    ""},


   {
    MORTAL, NORM,
    "shield block", {182, 182, 182, 40, 182, 18},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_shield_block, SLOT( 0 ), 0, 12, FALSE,
    "", "!Shield Block!",
    ""},


   {
    MORTAL, NORM,
    "sneak", {182, 182, 5, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_sneak, SLOT( 0 ), 0, 12, FALSE,
    "", NULL,
    ""},

   {
    MORTAL, NORM,
    "stake", {185, 45, 185, 60, 185, 185},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_stake, SLOT( 0 ), 0, 24, FALSE,
    "", NULL,
    ""},

   {
    REMORT, NORM,
    "stun", {182, 182, 182, 182, 182, 182,
             182, 182, 182, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_stun, SLOT( 0 ), 0, 24, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "disguise", {182, 182, 25, 182, 182, 182,
                 182, 182, 25, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_disguise, SLOT( 0 ), 0, 0, FALSE,
    "", "!Disguise!",
    ""},

   {
    MORTAL, VAMP,
    "instruct", {10, 5, 5, 5, 5, 5},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_instruct, SLOT( 0 ), 0, 0, FALSE,
    "", "!INSTRUCT!",
    ""},

   {
    REMORT, NORM,
    "frenzy", {182, 182, 182, 182, 182, 182,
               182, 182, 182, 182, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_frenzy, SLOT( 0 ), 0, 24, FALSE,
    "frenzy", "!FRENZY!",
    ""},

   {
    MORTAL, WOLF,
    "imprint", {20, 10, 10, 10, 10, 10},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_imprint, SLOT( 0 ), 0, 0, FALSE,
    "", "!INSTRUCT!",
    ""},

   {
    MORTAL, NORM,
    "warcry", {182, 182, 182, 10, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_warcry, SLOT( 0 ), 0, 24, TRUE,
    "warcry", "!Warcry!",
    ""},

   {
    MORTAL, NORM,
    "emotion control", {182, 182, 182, 182, 75, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_emotion_control, SLOT( 0 ), 0, 24, FALSE,
    "emotion control", "!EMOTION CONTROL!",
    ""},



/*
 *  Spells for mega1.are from Glop/Erkenbrand.
*/
   {
    MORTAL, NORM,
    "general purpose", {182, 182, 182, 182, 182, 182},
    spell_general_purpose, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 205 ), 0, 12, FALSE,
    "general purpose ammo", "!General Purpose Ammo!",
    ""},

   {
    MORTAL, NORM,
    "high explosive", {182, 182, 182, 182, 182, 182},
    spell_high_explosive, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 206 ), 0, 12, FALSE,
    "high explosive ammo", "!High Explosive Ammo!",
    ""},

   {
    REMORT, NORM,
    "deflect weapon", {182, 182, 182, 182, 22, 182,
                       182, 182, 182, 182, 22, 182},
    spell_deflect_weapon, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 600 ), 25, 12, TRUE,
    "", "Your mind shield melts away.",
    ""},

   {
    REMORT, NORM,
    "black hand", {182, 182, 182, 182, 7, 182,
                   182, 182, 182, 182, 7, 182},
    spell_black_hand, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 601 ), 50, 12, TRUE,
    "Black Hand", "The hand dissolves from around your throat into nothingness.",
    ""},
   {
    REMORT, NORM,
    "throwing needle", {182, 182, 182, 182, 182, 182,
                        182, 182, 7, 182, 182, 182},
    spell_throw_needle, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 602 ), 40, 12, TRUE,
    "Throwing Needle", "!Throwing Needle!",
    ""},


   {
    REMORT, NORM,
    "morale", {182, 21, 182, 10, 182, 182,
               182, 21, 182, 10, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_morale, SLOT( 0 ), 75, 24, TRUE,
    "Morale", "!Morale!",
    ""},

   {
    REMORT, NORM,
    "leadership", {182, 24, 182, 12, 182, 182,
                   182, 24, 182, 12, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_leadership, SLOT( 0 ), 75, 24, TRUE,
    "Leadership", "!Leadership!",
    ""},

   {
    REMORT, NORM,
    "two handed", {182, 8, 182, 6, 182, 182,
                   182, 182, 182, 6, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_two_handed, SLOT( 0 ), 0, 0, FALSE,
    "", "!Two Handed!",
    ""},

   {
    REMORT, NORM,
    "palmstrike", {182, 182, 182, 182, 182, 182,
                   182, 182, 182, 182, 182, 19},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_palmstrike, SLOT( 0 ), 0, 0, TRUE,
    "", "!Palm Strike!",
    ""},


   {
    REMORT, NORM,
    "bare hand", {182, 182, 182, 182, 182, 6,
                   182, 182, 182, 182, 182, 6},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_bare_hand, SLOT( 0 ), 0, 0, FALSE,
    "", "!Enhanced Sword!",
    ""},

   {
    REMORT, NORM,
    "enhanced sword damage", {182, 182, 182, 182, 182, 182,
                   182, 182, 182, 20, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_enhanced_sword, SLOT( 0 ), 0, 0, FALSE,
    "", "!Enhanced Sword!",
    ""},

   {
    REMORT, NORM,
    "enhanced sword critical", {182, 182, 182, 182, 182, 182,
                   182, 182, 182, 40, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_enhanced_sword_critical, SLOT( 0 ), 0, 0, FALSE,
    "", "!Enhanced Sword Critical!",
    ""},


   {
    REMORT, NORM,
    "Ice Bolt", {3, 182, 182, 182, 182, 182,
                 3, 182, 182, 182, 182, 182},
    spell_ice_bolt, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 605 ), 20, 12, TRUE,
    "Ice Bolt", "!Ice Bolt!",
    ""},
   {
    REMORT, NORM,
    "water elemental", {6, 182, 182, 182, 182, 182,
                        6, 182, 182, 182, 182, 182},
    spell_waterelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 606 ), 80, 24, FALSE,
    "", "!Stalker!",
    ""},
   {
    REMORT, NORM,
    "skeleton", {182, 182, 182, 182, 9, 182,
                 182, 182, 182, 182, 9, 182},
    spell_skeleton, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 607 ), 80, 24, FALSE,
    "", "!Stalker!",
    ""},
   {
    REMORT, NORM,
    "poison weapon", {182, 182, 25, 182, 182, 182,
                      182, 182, 5, 182, 182, 182},
    spell_poison_weapon, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 608 ), 100, 24, FALSE,
    "", "!Enchant Weapon!",
    ""},



   {
    MORTAL, VAMP,
    "embrace", {86, 15, 15, 15, 15, 15},
    spell_embrace, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 609 ), 15, 24, FALSE,
    "", "!EMBRACE!",
    ""},





   {
    MORTAL, VAMP,
    "mesmerise", {5, 2, 2, 2, 2, 2},
    spell_mesmerise, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 610 ), 4, 8, FALSE,
    "", "!MESMERISE!",
    ""},




   {
    REMORT, NORM,
    "ethereal travel", {30, 182, 182, 182, 24, 182,
                        30, 182, 182, 182, 24, 182},
    spell_ethereal, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 611 ), 250, 48, FALSE,
    "", "ETHEREAL TRAVEL!",
    ""},



   {
    REMORT, NORM,
    "adrenaline", {182, 42, 182, 21, 182, 28,
                   182, 42, 182, 21, 182, 28},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_adrenaline, SLOT( 0 ), 0, 0, FALSE,
    "", "!ADRENALINE!",
    ""},


   {
    REMORT, NORM,
    "throwing star", {182, 182, 43, 182, 182, 182,
                      182, 182, 12, 182, 182, 182},
    spell_throw_needle, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 628 ), 85, 12, TRUE,
    "Throwing Star", "!Throwing Star!",
    ""},



/* THIS IS A SPELL AFFECT HOLDER FOR THE SKILL ADRENALINE..DO NOT CAST! */

   {
    REMORT, NORM,
    "adrenaline bonus", {182, 182, 182, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 612 ), 0, 0, FALSE,
    "", "!ADRENALINE BONUS!",
    ""},

   {
    REMORT, NORM,
    "fire elemental", {11, 182, 182, 182, 182, 182,
                       11, 182, 182, 182, 182, 182},
    spell_fireelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 612 ), 120, 24, FALSE,
    "", "!FIRE ELEMENTAL!",
    ""},
   {
    REMORT, NORM,
    "rune:fire", {40, 182, 182, 182, 182, 182,
                  40, 182, 182, 182, 182, 182},
    spell_rune_fire, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 613 ), 150, 12, FALSE,
    "", "The rune dissipates.",
    ""},

   {
    REMORT, NORM,
    "rune:shock", {56, 182, 182, 182, 15, 182,
                   56, 182, 182, 182, 15, 182},
    spell_rune_shock, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 614 ), 150, 12, FALSE,
    "", "The rune dissipates.",
    ""},

   {
    REMORT, NORM,
    "rune:poison", {182, 182, 38, 182, 40, 182,
                    182, 182, 8, 182, 182, 182},
    spell_rune_poison, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 615 ), 80, 12, FALSE,
    "", "The rune dissipates.",
    ""},



   {
    REMORT, NORM,
    "healing light", {182, 42, 182, 182, 182, 182,
                      182, 11, 182, 182, 182, 182},
    spell_healing_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 616 ), 150, 12, FALSE,
    "", "@@NThe @@mHealing Light@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "withering shadow", {182, 182, 182, 17, 182, 182,
                         182, 182, 182, 17, 182, 182},
    spell_wither_shadow, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 617 ), 150, 12, FALSE,
    "", "@@NThe @@dWithering Shadow@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "mana flare", {18, 182, 182, 182, 182, 182,
                   18, 182, 182, 182, 182, 182},
    spell_mana_flare, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 618 ), 80, 12, FALSE,
    "", "@@NThe @@eMana Flare@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "mana drain", {11, 182, 182, 182, 15, 182,
                   11, 182, 182, 182, 15, 182},
    spell_mana_drain, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 619 ), 150, 12, FALSE,
    "", "@@NThe @@dMana Drain@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "cage", {18, 31, 40, 19, 13, 182,
             18, 31, 40, 19, 13, 182},
    spell_cage, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 620 ), 150, 12, FALSE,
    "", "@@NThe @@rCage@@N dissipates.",
    ""},


   {
    REMORT, NORM,
    "room dispel", {22, 182, 182, 182, 34, 182,
                    22, 182, 182, 182, 34, 182},
    spell_room_dispel, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 625 ), 120, 12, FALSE,
    "", "",
    ""},



   {
    REMORT, NORM,
    "soul net", {182, 182, 182, 182, 58, 182,
                 182, 182, 182, 182, 58, 182},
    spell_soul_net, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 629 ), 350, 12, FALSE,
    "", "@@NThe @@dSoul Net@@N dissipates.",
    ""},

   {
    REMORT, NORM,
    "condense soul", {182, 182, 182, 182, 78, 182,
                      182, 182, 182, 182, 78, 182},
    spell_condense_soul, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 630 ), 800, 24, FALSE,
    "", "!Soul Potion!",
    ""},

   {
    MORTAL, VAMP,
    "blood walk", {20, 19, 20, 18, 19, 20},
    spell_blood_walk, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 631 ), 40, 48, FALSE,
    "", "!blood walk!!",
    ""},

   {
    REMORT, NORM,
    "restoration", {182, 182, 182, 182, 182, 182,
                    182, 182, 182, 182, 182, 182},
    spell_restoration, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 632 ), 50, 12, FALSE,
    "", "!restoration!",
    ""},

   {
    REMORT, NORM,
    "infuse", {182, 182, 182, 182, 71, 182,
               182, 182, 182, 182, 71, 182},
    spell_infuse, TAR_OBJ_INV, POS_STANDING,
    NULL, SLOT( 633 ), 1000, 24, FALSE,
    "", "!Infuse Soul!",
    ""},

   {
    MORTAL, WOLF,
    "scent", {5, 1, 1, 1, 1, 1},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_scent, SLOT( 0 ), 0, 24, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "holy light", {182, 43, 182, 182, 182, 182,
                   82, 14, 182, 182, 182, 182},
    spell_holy_light, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 634 ), 75, 24, FALSE,
    "Holy Light", "!Holy Light!",
    ""},

   {
    REMORT, NORM,
    "target", {182, 32, 182, 20, 182, 18,
               182, 32, 182, 20, 182, 18},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_target, SLOT( 0 ), 0, 24, FALSE,
    "target", "!target",
    ""},

   {
    REMORT, NORM,
    "charge", {182, 89, 182, 30, 182, 182,
               182, 89, 182, 30, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_charge, SLOT( 0 ), 1, 24, TRUE,
    "Charge", "!Charge",
    ""},

   {
    REMORT, NORM,
    "scout", {182, 45, 26, 30, 182, 43,
              182, 45, 26, 30, 182, 43},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_scout, SLOT( 0 ), 0, 0, FALSE,
    "", "",
    ""},
   {
    MORTAL, NORM,
    "mount", {182, 182, 20, 20, 182, 20},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_mount, SLOT( 0 ), 0, 0, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "divine intervention", {182, 67, 182, 182, 182, 182,
                            182, 12, 182, 182, 182, 182},
    spell_divine_intervention, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 635 ), 150, 12, TRUE,
    "", "!Intervention!",
    ""},

   {
    REMORT, NORM,
    "holy armor", {182, 30, 182, 182, 182, 182,
                   182, 19, 182, 182, 182, 182},
    spell_holy_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 636 ), 100, 12, TRUE,
    "", "Your armor is no longer blessed.",
    ""},

   {
    REMORT, NORM,
    "unit tactics", {182, 182, 182, 16, 182, 182,
                     182, 182, 182, 16, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_unit_tactics, SLOT( 0 ), 0, 24, FALSE,
    "Unit Tactics", "!Unit Tactics!",
    ""},

   {
    REMORT, NORM,
    "earth elemental", {52, 182, 182, 182, 182, 182,
                        52, 182, 182, 182, 182, 182},
    spell_earthelem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 637 ), 500, 24, FALSE,
    "", "!EARTH ELEMENTAL!",
    ""},

   {
    REMORT, NORM,
    "iron golem", {63, 182, 182, 182, 182, 182,
                   63, 182, 182, 182, 182, 182},
    spell_iron_golem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 638 ), 800, 24, FALSE,
    "", "!IRON GOLEM!",
    ""},

   {
    REMORT, NORM,
    "diamond golem", {77, 182, 182, 182, 182, 182,
                      77, 182, 182, 182, 182, 182},
    spell_diamond_golem, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 639 ), 1100, 24, FALSE,
    "", "!DIAMOND GOLEM!",
    ""},
   {

    REMORT, NORM,
    "soul thief", {182, 182, 182, 182, 67, 182,
                   182, 182, 182, 182, 67, 182},
    spell_soul_thief, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 640 ), 900, 24, FALSE,
    "", "!SOUL THIEF!",
    ""},
   {

    REMORT, NORM,
    "holy avenger", {182, 76, 182, 182, 182, 182,
                     182, 76, 182, 182, 182, 182},
    spell_holy_avenger, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 641 ), 1200, 24, FALSE,
    "", "!HOLY AVENGER!",
    ""},
   {
    REMORT, NORM,
    "heat armor", {57, 182, 182, 182, 78, 182,
                   57, 182, 182, 182, 78, 182},
    spell_heat_armor, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 642 ), 350, 12, TRUE,
    "Heat Armor", "!Heat Armor!",
    ""},

   {
    REMORT, NORM,
    "Retributive Strike", {72, 182, 182, 182, 78, 182,
                           72, 182, 182, 182, 78, 182},
    spell_retri_strike, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 643 ), 800, 12, TRUE,
    "Retributive strike", "!Retributive Strike!",
    ""},

   {
    REMORT, NORM,
    "Lava Burst", {40, 182, 182, 182, 182, 182,
                   40, 182, 182, 182, 182, 182},
    spell_lava_burst, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 644 ), 350, 12, TRUE,
    "Lava Burst", "!Lava Burst!",
    ""},

   {
    REMORT, NORM,
    "fireshield", {80, 182, 182, 182, 182, 182,
                   80, 182, 182, 182, 182, 182},
    spell_fireshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 645 ), 400, 12, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "iceshield", {65, 182, 182, 182, 182, 182,
                  65, 182, 182, 182, 182, 182},
    spell_iceshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 646 ), 350, 12, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "shockshield", {55, 182, 182, 182, 182, 182,
                    55, 182, 182, 182, 182, 182},
    spell_shockshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 647 ), 400, 12, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "shadowshield", {182, 182, 182, 182, 182, 182,
                     182, 182, 182, 182, 182, 182},
    spell_shadowshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 648 ), 400, 12, FALSE,
    "", "@@NThe @@rshield@@N shatters!!",
    "@@NThe @@rshield@@N protecting $n shatters!!"},
   {
    REMORT, NORM,
    "thoughtshield", {182, 182, 182, 182, 182, 182,
                      182, 182, 182, 182, 182, 182},
    spell_thoughtshield, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 649 ), 400, 12, FALSE,
    "", "@@NThe @@rshield@@N shatters!!",
    "@@NThe @@rshield@@N protecting $n shatters!!"},

   {
    REMORT, WOLF,
    "Rage:wolven strength", {182, 182, 182, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 650 ), 0, 0, FALSE,
    "", "",
    ""},

   {
    REMORT, WOLF,
    "Rage:sharpened claws", {182, 182, 182, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 651 ), 0, 0, FALSE,
    "", "",
    ""},

   {
    REMORT, WOLF,
    "Rage:disregard for pain", {182, 182, 182, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 652 ), 0, 0, FALSE,
    "", "",
    ""},

   {
    REMORT, WOLF,
    "Enraged", {182, 182, 182, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    NULL, SLOT( 653 ), 0, 0, FALSE,
    "", "",
    ""},

   {
    MORTAL, WOLF,
    "tribal claw", {186, 15, 15, 15, 15, 15},
    spell_tribe_claw, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 655 ), 50, 24, FALSE,
    "", "",
    ""},

   {
    MORTAL, WOLF,
    "mark", {10, 5, 5, 5, 5, 5},
    spell_wolf_mark, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 656 ), 5, 24, FALSE,
    "", "",
    ""},

   {
    MORTAL, VAMP,
    "bloodsign", {12, 8, 10, 9, 7, 8},
    spell_blood_sign, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 657 ), 5, 24, FALSE,
    "", "",
    ""},

   {
    MORTAL, VAMP,
    "bloodsense", {10, 3, 3, 3, 3, 3},
    spell_blood_sense, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 658 ), 1, 24, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "summon pegasus", {182, 60, 182, 60, 182, 182, 
                       182, 60, 182, 60, 182, 182},
    spell_summon_pegasus, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 659 ), 1200, 24, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "summon nightmare", {182, 182, 182, 182, 60, 182, 
                         182, 182, 182, 182, 60, 182},
    spell_summon_nightmare, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 660 ), 1200, 24, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "summon beast", {60, 182, 182, 182, 182, 182,
                     60, 182, 182, 182, 182, 182},
    spell_summon_beast, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 661 ), 1200, 24, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "summon devourer", {182, 182, 182, 182, 182, 60, 
                        182, 182, 182, 182, 182, 60},
    spell_summon_devourer, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 662 ), 1200, 24, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "summon shadow", {182, 182, 60, 182, 182, 182, 
                      182, 182, 60, 182, 182, 182},
    spell_summon_shadow, TAR_IGNORE, POS_STANDING,
    NULL, SLOT( 663 ), 1200, 24, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "creature bond", {77, 182, 182, 182, 55, 182,
                      60, 182, 182, 182, 182, 182},
    spell_creature_bond, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 664 ), 100, 12, FALSE,
    "", "",
    ""},

   {
    REMORT, NORM,
    "corrupt bond", {182, 182, 182, 182, 33, 182,
                     182, 182, 182, 182, 50, 182},
    spell_corrupt_bond, TAR_CHAR_DEFENSIVE, POS_FIGHTING,
    NULL, SLOT( 665 ), 100, 12, FALSE,
    "", "",
    ""},

   {
    MORTAL, NORM,
    "decapitate", {185, 45, 185, 60, 185, 185},
    spell_null, TAR_IGNORE, POS_FIGHTING,
    &gsn_decapitate, SLOT( 0 ), 0, 0, FALSE,
    "", "!decapitate!",
    ""},

   {
    REMORT, NORM,
    "Fire Blast", {34, 182, 182, 182, 55, 182,
                   40, 182, 182, 182, 182, 182},
    spell_fireblast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 666 ), 80, 12, TRUE,
    "Fire Blast", "!Fire Blast!",
    ""},

   {
    REMORT, NORM,
    "Shock Storm", {42, 182, 182, 182, 68, 182,
                    42, 182, 182, 182, 68, 182},
    spell_shockstorm, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 667 ), 100, 12, TRUE,
    "Shock Storm", "!Shock Storm!",
    ""},

   {
    REMORT, NORM,
    "Cone of cold", {56, 182, 182, 182, 182, 182,
                     56, 182, 182, 182, 182, 182},
    spell_cone_cold, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 668 ), 100, 12, TRUE,
    "Cone of Cold", "!Cone of Cold!",
    ""},

   {
    REMORT, NORM,
    "Holy Wrath", {182, 80, 182, 182, 182, 182,
                   182, 40, 182, 182, 182, 182},
    spell_holy_wrath, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 669 ), 140, 12, TRUE,
    "Holy Wrath", "!Holy Wrath!",
    ""},

   {
    REMORT, NORM,
    "Wraith Touch", {182, 182, 182, 182, 46, 182,
                     182, 182, 182, 182, 182, 182},
    spell_wraith_touch, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 670 ), 120, 12, TRUE,
    "Wraith Touch", "!Wraith Touch!",
    ""},

   {
    REMORT, NORM,
    "Thought Vise", {182, 182, 182, 182, 70, 182,
                     182, 182, 182, 182, 70, 182},
    spell_thought_vise, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 670 ), 200, 12, TRUE,
    "Thought Vise", "!Thought Vise!",
    ""},

   {
    REMORT, NORM,
    "black curse", {182, 182, 182, 182, 65, 182,
                    182, 182, 182, 182, 65, 182},
    spell_black_curse, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 0 ), 120, 12, TRUE,
    "Black Curse", "!Black Curse!",
    ""},

   {
    REMORT, NORM,
    "spirit curse", {182, 182, 182, 182, 77, 182,
                     182, 182, 182, 182, 77, 182},
    spell_spirit_curse, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 0 ), 120, 12, TRUE,
    "Spirit Curse", "!Spirit Curse!",
    ""},

   {
    REMORT, NORM,
    "rictus curse", {182, 182, 182, 182, 100, 182,
                     182, 182, 182, 182, 100, 182},
    spell_rictus_curse, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 0 ), 120, 12, TRUE,
    "Rictus Curse", "!Rictus Curse!",
    ""},

   {
    ADEPT, NORM,
    "Kinetic Reversion", {182, 182, 182, 182, 2, 182},
    spell_kinetic_reversion, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 0 ), 120, 12, TRUE,
    "Kinetic Reversion", "!Kinetic Reversion!",
    ""},

   {
    REMORT, NORM,
    "cloak:misery", {182, 182, 80, 182, 73, 182,
                     182, 182, 80, 182, 73, 182},
    spell_cloak_misery, TAR_CHAR_DEFENSIVE, POS_STANDING,
    NULL, SLOT( 672 ), 550, 12, FALSE,
    "", "@@NThe @@Rcloak@@N around your body fades.",
    "@@NThe @@Rcloak@@N around $n's body fades."},

   {
    REMORT, NORM,
    "poison:quinine", {182, 182, 56, 182, 182, 182,
                       182, 182, 21, 182, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_poison_quinine, SLOT( 673 ), 80, 12, TRUE,
    "Poison Quinine", "@@N$n feels much better!",
    ""},

   {
    REMORT, NORM,
    "poison:arsenic", {182, 182, 66, 182, 182, 182,
                       182, 182, 28, 182, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_poison_arsenic, SLOT( 674 ), 80, 12, TRUE,
    "Poison Arsenic", "@@N$nFeels much better!",
    ""},

   {
    ADEPT, NORM,
    "poison:nightshade", {182, 182, 2, 182, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_poison_nightshade, SLOT( 0 ), 80, 12, TRUE,
    "Poison Nightshade", "@@N$nFeels much better!",
    ""},

   {
    REMORT, NORM,
    "riposte",        {182, 182, 182, 182, 182, 182,
                       182, 182, 182, 30, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_riposte, SLOT( 0 ), 80, 12, TRUE,
    "Riposte", "You are no longer in a position to riposte!",
    "$n is no longer in a position to riposte."},

   {
    REMORT, NORM,
    "fleche",        {182, 182, 182, 182, 182, 182,
                       182, 182, 182, 45, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_fleche, SLOT( 0 ), 80, 12, TRUE,
    "Fleche", "!Fleche!",
    ""},

   {
    REMORT, NORM,
    "holystrike",        {182, 44, 182, 29, 182, 182,
                       182, 182, 182, 182, 182, 182},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_holystrike, SLOT( 0 ), 80, 12, TRUE,
    "Holystrike", "!Holystrike!",
    ""},

   {
    ADEPT, NORM,
    "anti magic shell",{182, 182, 182, 2, 182, 182,},
    spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    &gsn_anti_magic_shell, SLOT( 0 ), 80, 12, TRUE,
    "Anti Magic Shell", "You are no longer surrounded by an anti-magic shell!",
    "$n is no longer surrounded by an anti-magic shell."},


   {
    REMORT, NORM,
    "sonic blast", {182, 182, 182, 182, 182, 80,
                    182, 182, 182, 182, 182, 80},
    spell_sonic_blast, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
    NULL, SLOT( 675 ), 140, 12, TRUE,
    "Sonic Blast", "@@N$nFeels much better!",
    ""},

   {
    MORTAL, NORM,
    "mystical potency", {40, 182, 182, 182, 182, 182},
    spell_null, TAR_IGNORE, POS_STANDING,
    &gsn_potency, SLOT( 0 ), 0, 0, FALSE,
    "", "!Enhanced Damage!",
    ""},

   {
    REMORT, NORM,
    "mystical focus", {55, 182, 182, 182, 48, 182,
                       45, 182, 182, 182, 47, 182},
    spell_mystical_focus, TAR_CHAR_SELF, POS_STANDING,
    NULL, SLOT( 676 ), 200, 10, TRUE,
    "", "Your lose your mystical focus.",
    "$N shakes his head, and seems less focused."},
};
