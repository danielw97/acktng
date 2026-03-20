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

#include "globals.h"
#ifndef DEC_MAGIC_H
#include "magic.h"
#endif

const struct race_type race_table[MAX_RACE] = {
    {
        "Hmn",
        "Human",
        101,
        0,
        19,
        19,
        19,
        19,
        19,
        RACE_MOD_NONE,
        /*
         * racial wear slots
         */
        {TRUE,  TRUE, TRUE, FALSE, TRUE, TRUE,  FALSE, TRUE, TRUE,  TRUE, TRUE,
         FALSE, TRUE, TRUE, TRUE,  TRUE, TRUE,  TRUE,  TRUE, FALSE, TRUE, TRUE,
         TRUE,  TRUE, TRUE, TRUE,  TRUE, FALSE, TRUE,  TRUE, FALSE, TRUE, TRUE},
        MAX_CLASS,
        {3, 4, 5, 2, 0, 1},
        "War, Cip, Pug, Psi, Mag, Cle",
        "",
        ELE_NONE, /* strong magic realms for race */
        ELE_NONE, /* weak */
        ELE_NONE, /* resist */
        ELE_NONE, /* suscept */
        TRUE      /* player race */
    },

    /* Khenari — jackal-folk of Akh'enet: death-keepers, psychopomps, balance-walkers.
     * WIS 22 grants: spell critical damage, spell critical.
     * STR:12 INT:18 WIS:22 DEX:16 CON:16 = 84 */
    {"Khn",
     "Khenari",
     3001,
     0,
     12,
     18,
     22,
     16,
     16,
     RACE_MOD_DARKNESS | RACE_MOD_TAIL,
     {TRUE,  TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE, TRUE,  TRUE, TRUE,
      FALSE, TRUE, TRUE, TRUE,  TRUE, TRUE, TRUE,  TRUE, FALSE, TRUE, TRUE,
      TRUE,  TRUE, TRUE, TRUE,  TRUE, TRUE, TRUE,  TRUE, FALSE, TRUE, TRUE},
     MAX_CLASS,
     {2, 0, 3, 4, 1, 5},
     "Cle, Psi, Mag, Cip, War, Pug",
     "sneak vigilance \'spell critical damage\' \'spell critical\'",
     ELE_SHADOW | ELE_HOLY,   /* strong magic realms */
     ELE_PHYSICAL | ELE_FIRE, /* weak */
     ELE_SHADOW | ELE_MENTAL, /* resist */
     ELE_FIRE | ELE_PHYSICAL, /* suscept */
     TRUE},

    /* Khephari — scarab-beetle folk of the desert, reborn from the earth.
     * STR 22 grants: enhanced damage.  CON 22 grants: counter.
     * STR:22 INT:12 WIS:14 DEX:14 CON:22 = 84 */
    {"Kph",
     "Khephari",
     3001,
     0,
     22,
     12,
     14,
     14,
     22,
     RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN,
     {TRUE,  TRUE, TRUE, TRUE, FALSE, TRUE,  FALSE, TRUE, TRUE,  TRUE, TRUE,
      FALSE, TRUE, TRUE, TRUE, TRUE,  FALSE, TRUE,  TRUE, TRUE,  TRUE, TRUE,
      TRUE,  TRUE, TRUE, TRUE, TRUE,  FALSE, TRUE,  TRUE, FALSE, TRUE, TRUE},
     MAX_CLASS,
     {4, 3, 2, 0, 5, 1},
     "War, Pug, Cip, Cle, Mag, Psi",
     "\'enhanced damage\' counter",
     ELE_EARTH | ELE_POISON, /* strong magic realms */
     ELE_AIR | ELE_MENTAL,   /* weak */
     ELE_EARTH | ELE_POISON, /* resist */
     ELE_AIR | ELE_FIRE,     /* suscept */
     TRUE},

    /* Ashborn — volcanic fire-kin of the Cinderteeth Mountains.
     * STR 22 grants: enhanced damage.  CON 22 grants: counter.
     * STR:22 INT:16 WIS:10 DEX:14 CON:22 = 84 */
    {"Asb",
     "Ashborn",
     3001,
     0,
     22,
     16,
     10,
     14,
     22,
     RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN,
     {TRUE,  TRUE, TRUE, FALSE, TRUE, TRUE,  FALSE, TRUE, TRUE,  TRUE, TRUE,
      FALSE, TRUE, TRUE, TRUE,  TRUE, TRUE,  TRUE,  TRUE, FALSE, TRUE, TRUE,
      TRUE,  TRUE, TRUE, TRUE,  TRUE, FALSE, TRUE,  TRUE, FALSE, TRUE, TRUE},
     MAX_CLASS,
     {2, 3, 4, 0, 5, 1},
     "War, Pug, Mag, Cle, Cip, Psi",
     "\'fire breath\' \'enhanced damage\' counter",
     ELE_FIRE | ELE_EARTH,    /* strong magic realms */
     ELE_WATER | ELE_MENTAL,  /* weak */
     ELE_FIRE | ELE_PHYSICAL, /* resist */
     ELE_WATER | ELE_AIR,     /* suscept */
     TRUE},

    /* Umbral — shadow-walkers shaped by the Black Sun Shard and the Gloamvault.
     * INT 22 grants: mystical potency.  DEX 22 grants: enhanced critical.
     * STR:10 INT:22 WIS:14 DEX:22 CON:16 = 84 */
    {"Umb",
     "Umbral",
     3001,
     0,
     10,
     22,
     14,
     22,
     16,
     RACE_MOD_DARKNESS | RACE_MOD_SLOW_HEAL,
     {TRUE,  TRUE, TRUE, FALSE, TRUE, TRUE,  FALSE, TRUE, TRUE,  TRUE, TRUE,
      FALSE, TRUE, TRUE, TRUE,  TRUE, TRUE,  TRUE,  TRUE, FALSE, TRUE, TRUE,
      TRUE,  TRUE, TRUE, TRUE,  TRUE, FALSE, TRUE,  TRUE, FALSE, TRUE, TRUE},
     MAX_CLASS,
     {0, 3, 2, 4, 1, 5},
     "Mag, Psi, Cip, Cle, War, Pug",
     "sneak \'mystical potency\' \'enhanced critical\'",
     ELE_SHADOW | ELE_MENTAL, /* strong magic realms */
     ELE_HOLY | ELE_PHYSICAL, /* weak */
     ELE_SHADOW | ELE_MENTAL, /* resist */
     ELE_HOLY | ELE_FIRE,     /* suscept */
     TRUE},

    /* Rivennid — fungal mycelians born of the Conclave's abandoned trials in the Withered Depths.
     * INT 22 grants: mystical potency.  CON 22 grants: counter.
     * STR:14 INT:22 WIS:14 DEX:12 CON:22 = 84 */
    {"Rvn",
     "Rivennid",
     3001,
     0,
     14,
     22,
     14,
     12,
     22,
     RACE_MOD_DARKNESS | RACE_MOD_TOUGH_SKIN,
     {TRUE,  TRUE, TRUE, FALSE, TRUE, TRUE,  FALSE, TRUE, TRUE,  TRUE, TRUE,
      FALSE, TRUE, TRUE, TRUE,  TRUE, TRUE,  TRUE,  TRUE, FALSE, TRUE, TRUE,
      TRUE,  TRUE, TRUE, TRUE,  TRUE, FALSE, TRUE,  TRUE, FALSE, TRUE, TRUE},
     MAX_CLASS,
     {0, 2, 3, 4, 1, 5},
     "Mag, Psi, Cle, Cip, War, Pug",
     "\'mystical potency\' counter",
     ELE_EARTH | ELE_POISON, /* strong magic realms */
     ELE_FIRE | ELE_AIR,     /* weak */
     ELE_POISON | ELE_EARTH, /* resist */
     ELE_FIRE | ELE_HOLY,    /* suscept */
     TRUE},

    /* Deltari — river delta folk of Kowloon, water-wise traders and Reed Ledger heirs.
     * WIS 22 grants: spell critical damage, spell critical.
     * STR:12 INT:18 WIS:22 DEX:18 CON:14 = 84 */
    {"Del",
     "Deltari",
     3001,
     0,
     12,
     18,
     22,
     18,
     14,
     RACE_MOD_NONE,
     {TRUE,  TRUE, TRUE, FALSE, TRUE, TRUE,  FALSE, TRUE, TRUE,  TRUE, TRUE,
      FALSE, TRUE, TRUE, TRUE,  TRUE, TRUE,  TRUE,  TRUE, FALSE, TRUE, TRUE,
      TRUE,  TRUE, TRUE, TRUE,  TRUE, FALSE, TRUE,  TRUE, FALSE, TRUE, TRUE},
     MAX_CLASS,
     {3, 0, 2, 4, 1, 5},
     "Cle, Psi, Cip, Mag, War, Pug",
     "hunt vigilance \'spell critical damage\' \'spell critical\'",
     ELE_WATER | ELE_MENTAL, /* strong magic realms */
     ELE_FIRE | ELE_EARTH,   /* weak */
     ELE_WATER | ELE_POISON, /* resist */
     ELE_FIRE | ELE_EARTH,   /* suscept */
     TRUE},

    /* Ushabti — animated stone guardians of the Great Pyramid, vessels of cosmic law.
     * STR 22 grants: enhanced damage.  WIS 22 grants: spell critical damage, spell critical.
     * STR:22 INT:12 WIS:22 DEX:10 CON:18 = 84 */
    {"Ush",
     "Ushabti",
     3001,
     0,
     22,
     12,
     22,
     10,
     18,
     RACE_MOD_HUGE | RACE_MOD_SLOW_HEAL | RACE_MOD_STONE_SKIN,
     {TRUE,  TRUE, TRUE, FALSE, TRUE, TRUE,  FALSE, TRUE, TRUE,  TRUE, TRUE,
      FALSE, TRUE, TRUE, TRUE,  TRUE, TRUE,  TRUE,  TRUE, FALSE, TRUE, TRUE,
      TRUE,  TRUE, TRUE, TRUE,  TRUE, FALSE, TRUE,  TRUE, FALSE, TRUE, TRUE},
     MAX_CLASS,
     {3, 1, 4, 0, 5, 2},
     "War, Cle, Pug, Mag, Cip, Psi",
     "\'stone skin\' \'enhanced damage\' vigilance \'spell critical damage\' \'spell critical\'",
     ELE_EARTH | ELE_HOLY,     /* strong magic realms */
     ELE_SHADOW | ELE_WATER,   /* weak */
     ELE_EARTH | ELE_PHYSICAL, /* resist */
     ELE_SHADOW | ELE_MENTAL,  /* suscept */
     TRUE},

    /* Serathi — sky-born avian folk of the western air-roads; swift hunters of wind and height.
     * DEX 22 grants: enhanced critical.  CON 22 grants: counter.  Lore grants: hunt, sneak.
     * STR:16 INT:12 WIS:12 DEX:22 CON:22 = 84
     * Wear: BEAK (not FACE), WINGS, HOOVES (not FEET), TAIL; no HEAD, no EARS (feathered). */
    {"Ser",
     "Serathi",
     3001,
     0,
     16,
     12,
     12,
     22,
     22,
     RACE_MOD_TAIL | RACE_MOD_FAST_HEAL,
     /*  LGT  HAL  AUR  HRN  HED  FCE  BEK  E_L  E_R  NK1  NK2
         WNG  SHO  ARM  W_L  W_R  HND  F_L  F_R  CLW  HH_L HH_R
         TWO  BCK  ABT  WST  BOD  TAL  LEG  FET  HOV  CLN  INV */
     {TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE,  TRUE,  TRUE,  TRUE, TRUE,  FALSE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE,  TRUE,  TRUE,  TRUE, FALSE, TRUE,  TRUE, TRUE},
     MAX_CLASS,
     {4, 5, 0, 2, 3, 1},
     "Cip, Pug, War, Psi, Mag, Cle",
     "hunt \'enhanced critical\' counter sneak",
     ELE_AIR | ELE_MENTAL,     /* strong magic realms */
     ELE_EARTH | ELE_PHYSICAL, /* weak */
     ELE_AIR | ELE_POISON,     /* resist */
     ELE_EARTH | ELE_FIRE,     /* suscept */
     TRUE},

    /* Kethari — ancient serpentine folk of the Saltglass Reach; venom-wise, scale-armored
     * survivors. WIS 22 grants: spell critical damage, spell critical.  CON 22 grants: counter.
     * STR:12 INT:14 WIS:22 DEX:14 CON:22 = 84
     * Wear: TAIL (serpentine lower body — no LEGS, no FEET, no HOOVES). */
    {"Ket",
     "Kethari",
     3001,
     0,
     12,
     14,
     22,
     14,
     22,
     RACE_MOD_TAIL | RACE_MOD_TOUGH_SKIN | RACE_MOD_IMMUNE_POISON,
     /*  LGT  HAL  AUR  HRN  HED  FCE  BEK  E_L  E_R  NK1  NK2
         WNG  SHO  ARM  W_L  W_R  HND  F_L  F_R  CLW  HH_L HH_R
         TWO  BCK  ABT  WST  BOD  TAL  LEG  FET  HOV  CLN  INV */
     {TRUE,  TRUE, TRUE, FALSE, TRUE, TRUE, FALSE, TRUE,  TRUE,  TRUE, TRUE,
      FALSE, TRUE, TRUE, TRUE,  TRUE, TRUE, TRUE,  TRUE,  FALSE, TRUE, TRUE,
      TRUE,  TRUE, TRUE, TRUE,  TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE},
     MAX_CLASS,
     {4, 0, 3, 5, 2, 1},
     "Cle, Pug, Psi, Cip, Mag, War",
     "counter vigilance \'spell critical damage\' \'spell critical\'",
     ELE_WATER | ELE_POISON, /* strong magic realms */
     ELE_FIRE | ELE_AIR,     /* weak */
     ELE_POISON | ELE_WATER, /* resist */
     ELE_FIRE | ELE_HOLY,    /* suscept */
     TRUE},

    /* ------------------------------------------------------------------ *
     *  NPC-ONLY RACES  (player_allowed = FALSE, indices 10–22)           *
     *  Stats are nominal; element profiles drive combat behaviour.       *
     * ------------------------------------------------------------------ */

    /* Undead — reanimated dead powered by shadow and necrotic force.
     * DARKNESS: shun light.  SLOW_HEAL: barely mend between fights.
     * Resist poison (undead don't metabolise toxins). */
    {"Und",
     "Undead",
     3001,
     0,
     15,
     15,
     15,
     15,
     15,
     RACE_MOD_DARKNESS | RACE_MOD_SLOW_HEAL,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_SHADOW | ELE_PHYSICAL, /* strong */
     ELE_HOLY | ELE_MENTAL,     /* weak */
     ELE_SHADOW | ELE_POISON,   /* resist */
     ELE_HOLY | ELE_FIRE,       /* suscept */
     FALSE},

    /* Serpent — actual snakes (not Kethari sapients); venomous constrictors.
     * TAIL: serpentine body.  TOUGH_SKIN: scaled hide.  IMMUNE_POISON. */
    {"Snk",
     "Serpent",
     3001,
     0,
     15,
     15,
     15,
     18,
     15,
     RACE_MOD_TAIL | RACE_MOD_TOUGH_SKIN | RACE_MOD_IMMUNE_POISON,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_POISON | ELE_WATER, /* strong */
     ELE_FIRE | ELE_AIR,     /* weak */
     ELE_POISON,             /* resist */
     ELE_FIRE | ELE_HOLY,    /* suscept */
     FALSE},

    /* Arachnid — spiders, scorpions, glassworms; multi-legged venomous hunters.
     * TOUGH_SKIN: chitinous exoskeleton. */
    {"Arc",
     "Arachnid",
     3001,
     0,
     15,
     15,
     15,
     18,
     16,
     RACE_MOD_TOUGH_SKIN,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_POISON | ELE_EARTH, /* strong */
     ELE_FIRE | ELE_AIR,     /* weak */
     ELE_POISON | ELE_EARTH, /* resist */
     ELE_FIRE,               /* suscept */
     FALSE},

    /* Insect — beetles, scarabs, swarms, centipedes, leeches, crickets.
     * TINY: diminutive form.  TOUGH_SKIN: hard carapace. */
    {"Ins",
     "Insect",
     3001,
     0,
     10,
     15,
     15,
     20,
     15,
     RACE_MOD_TINY | RACE_MOD_TOUGH_SKIN,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_EARTH | ELE_POISON, /* strong */
     ELE_FIRE | ELE_AIR,     /* weak */
     ELE_POISON,             /* resist */
     ELE_FIRE | ELE_AIR,     /* suscept */
     FALSE},

    /* Canine — wolves, jackals (animal), hounds, foxes, bears; pack hunters.
     * FAST_HEAL: vigorous natural recovery. */
    {"Can",
     "Canine",
     3001,
     0,
     17,
     12,
     14,
     18,
     16,
     RACE_MOD_FAST_HEAL,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_PHYSICAL | ELE_AIR,  /* strong */
     ELE_SHADOW | ELE_MENTAL, /* weak */
     ELE_PHYSICAL,            /* resist */
     ELE_SHADOW | ELE_MENTAL, /* suscept */
     FALSE},

    /* Avian — birds of prey, scavengers, bats; swift and keen-eyed.
     * FAST_HEAL: rapid metabolism and recovery. */
    {"Avi",
     "Avian",
     3001,
     0,
     12,
     14,
     15,
     22,
     14,
     RACE_MOD_FAST_HEAL,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_AIR | ELE_PHYSICAL, /* strong */
     ELE_EARTH | ELE_SHADOW, /* weak */
     ELE_AIR,                /* resist */
     ELE_EARTH | ELE_FIRE,   /* suscept */
     FALSE},

    /* Feline — cats, lions, panthers, tigers; also large ungulates as mammal catch-all.
     * FAST_HEAL: natural vitality. */
    {"Fel",
     "Feline",
     3001,
     0,
     17,
     12,
     14,
     20,
     15,
     RACE_MOD_FAST_HEAL,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_PHYSICAL | ELE_AIR,  /* strong */
     ELE_SHADOW | ELE_MENTAL, /* weak */
     ELE_PHYSICAL,            /* resist */
     ELE_SHADOW | ELE_MENTAL, /* suscept */
     FALSE},

    /* Reptile — lizards, crocodiles, basilisks, frogs, tortoises; cold-blooded.
     * TOUGH_SKIN: thick scaly hide. */
    {"Rep",
     "Reptile",
     3001,
     0,
     16,
     12,
     14,
     15,
     18,
     RACE_MOD_TOUGH_SKIN,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_EARTH | ELE_POISON, /* strong */
     ELE_AIR | ELE_MENTAL,   /* weak */
     ELE_EARTH | ELE_POISON, /* resist */
     ELE_AIR | ELE_FIRE,     /* suscept */
     FALSE},

    /* Dragon — all dragons, drakes, wyrms, wyverns; apex predators.
     * HUGE: enormous size.  TOUGH_SKIN: armoured scales.  FAST_HEAL: draconic vitality. */
    {"Drg",
     "Dragon",
     3001,
     0,
     22,
     18,
     16,
     16,
     22,
     RACE_MOD_HUGE | RACE_MOD_TOUGH_SKIN | RACE_MOD_FAST_HEAL,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_FIRE | ELE_EARTH,    /* strong */
     ELE_MENTAL | ELE_SHADOW, /* weak */
     ELE_FIRE | ELE_PHYSICAL, /* resist */
     ELE_MENTAL | ELE_HOLY,   /* suscept */
     FALSE},

    /* Elemental — elementals, golems, animated constructs, dust devils, sandstorms, djinn.
     * HUGE: massive forms.  TOUGH_SKIN: stone/metal/crystal body.  SLOW_HEAL: inorganic repair. */
    {"Elm",
     "Elemental",
     3001,
     0,
     22,
     15,
     15,
     12,
     22,
     RACE_MOD_HUGE | RACE_MOD_TOUGH_SKIN | RACE_MOD_SLOW_HEAL,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_EARTH | ELE_PHYSICAL,  /* strong */
     ELE_HOLY | ELE_MENTAL,     /* weak */
     ELE_PHYSICAL | ELE_POISON, /* resist */
     ELE_HOLY | ELE_MENTAL,     /* suscept */
     FALSE},

    /* Demon — demons, fiends, gargoyles, infernals; shadow-born corruptors.
     * LARGE: imposing size.  TOUGH_SKIN: hide or stone.  DARKNESS: children of shadow. */
    {"Dem",
     "Demon",
     3001,
     0,
     20,
     18,
     14,
     18,
     18,
     RACE_MOD_LARGE | RACE_MOD_TOUGH_SKIN | RACE_MOD_DARKNESS,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_SHADOW | ELE_FIRE,              /* strong */
     ELE_HOLY | ELE_MENTAL,              /* weak */
     ELE_SHADOW | ELE_FIRE | ELE_POISON, /* resist */
     ELE_HOLY | ELE_AIR,                 /* suscept */
     FALSE},

    /* Plant — treants, tendrils, vine creatures, fungal horrors, bog beasts, dryads.
     * TOUGH_SKIN: bark or fibrous hide.  SLOW_HEAL: plant regeneration is slow. */
    {"Pla",
     "Plant",
     3001,
     0,
     18,
     12,
     16,
     10,
     20,
     RACE_MOD_TOUGH_SKIN | RACE_MOD_SLOW_HEAL,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_EARTH | ELE_WATER,  /* strong */
     ELE_FIRE | ELE_AIR,     /* weak */
     ELE_EARTH | ELE_POISON, /* resist */
     ELE_FIRE | ELE_AIR,     /* suscept */
     FALSE},

    /* Aquatic — fish, eels, crabs, water nymphs, bog hydras, tidal lurkers.
     * IMMUNE_POISON: aquatic creatures tolerate waterborne toxins. */
    {"Aqu",
     "Aquatic",
     3001,
     0,
     15,
     14,
     14,
     18,
     16,
     RACE_MOD_IMMUNE_POISON,
     {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,
      TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE},
     0,
     {0, 1, 2, 3, 4, 5},
     "",
     "",
     ELE_WATER | ELE_POISON, /* strong */
     ELE_FIRE | ELE_EARTH,   /* weak */
     ELE_WATER | ELE_POISON, /* resist */
     ELE_FIRE | ELE_EARTH,   /* suscept */
     FALSE},

};
