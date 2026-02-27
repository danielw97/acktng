/***************************************************************************
 * invasion_mobs.h -- Wave mob profile table for the invasion system.      *
 *                                                                         *
 * Included ONCE by invasion.c.  Do NOT include anywhere else.             *
 *                                                                         *
 * Fields per entry:                                                       *
 *   name      – creature name (must match invasion_mob_names[i])          *
 *   colour    – ANSI colour prefix prepended at spawn                     *
 *   long_descr– room description text (no colour prefix, no \n\r —       *
 *               both are added at spawn time)                             *
 *   resist    – ELE_xxx bitmask: takes half damage from these elements    *
 *   suscept   – ELE_xxx bitmask: takes double damage from these elements  *
 *   cast      – CAST_xxx bitmask: offensive spells the AI may use         *
 *   def       – DEF_xxx bitmask: defensive spells the AI may use          *
 *   skills    – MOB_xxx bitmask: physical combat skills                   *
 *   block_mod – added to mob->block_mod at spawn                          *
 *               0=none  10=light  20=medium  30=heavy natural armour      *
 ***************************************************************************/

#ifndef INVASION_MOBS_H
#define INVASION_MOBS_H

static const WAVE_MOB_PROFILE invasion_wave_profiles[] =
{
    /* ==================================================================== */
    /* ---- GREEK / ROMAN ------------------------------------------------- */
    /* ==================================================================== */

    /* 0  MINOTAUR */
    {
        "Minotaur", "@@y",
        "A massive bull-headed Minotaur snorts steam and paws the earth, its bronze-shod horns lowered for the charge.",
        ELE_PHYSICAL | ELE_EARTH,
        ELE_MENTAL | ELE_AIR,
        0, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
        20
    },

    /* 1  HARPY */
    {
        "Harpy", "@@y",
        "A shrieking Harpy circles overhead, filth-caked talons spread wide and hollow eyes burning with hunger.",
        ELE_AIR | ELE_PHYSICAL,
        ELE_EARTH | ELE_WATER,
        CAST_COLOR_SPRAY | CAST_STATIC, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_KICK | MOB_TRIP | MOB_DIRT
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 2  GORGON */
    {
        "Gorgon", "@@y",
        "A Gorgon advances with a crown of writhing serpents, its cold stone-grey eyes threatening to freeze all life.",
        ELE_MENTAL | ELE_EARTH,
        ELE_HOLY | ELE_AIR,
        CAST_MIND_FLAIL | CAST_BLOODY_TEARS | CAST_MIND_BOLT, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_HEADBUTT
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 3  CYCLOPS */
    {
        "Cyclops", "@@y",
        "A one-eyed Cyclops looms like a walking cliff, its single bloodshot eye fixed on you with dim and terrible hunger.",
        ELE_PHYSICAL | ELE_EARTH,
        ELE_MENTAL | ELE_AIR,
        0, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_KICK | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
        30
    },

    /* 4  CHIMERA */
    {
        "Chimera", "@@e",
        "A Chimera prowls with lion's grace and goat's fury, its serpent tail lashing and all three heads breathing death.",
        ELE_FIRE | ELE_POISON,
        ELE_WATER | ELE_MENTAL,
        CAST_FIREBALL | CAST_BURNING_HANDS | CAST_LAVA_BURST, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 5  CERBERUS */
    {
        "Cerberus", "@@d",
        "A three-headed Cerberus fills the passage, three maws slavering with hellfire and six eyes blazing with infernal rage.",
        ELE_SHADOW | ELE_PHYSICAL,
        ELE_HOLY | ELE_FIRE,
        CAST_HELLSPAWN, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_SIXTH
        | MOB_TRIP | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 6  HYDRA */
    {
        "Hydra", "@@g",
        "A writhing Hydra sways on a forest of necks, each severed head already regrowing as its acid-dripping fangs seek flesh.",
        ELE_POISON | ELE_WATER,
        ELE_FIRE | ELE_HOLY,
        CAST_ACID_BLAST | CAST_SUFFOCATE,
        DEF_CURE_LIGHT,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_SIXTH
        | MOB_TRIP | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
        20
    },

    /* 7  MANTICORE */
    {
        "Manticore", "@@e",
        "A Manticore stalks forward on lion paws, its human-like face twisted in a sneer and its scorpion tail arcing overhead.",
        ELE_POISON | ELE_PHYSICAL,
        ELE_WATER | ELE_MENTAL,
        CAST_PLANERGY | CAST_ACID_BLAST, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_KICK | MOB_TRIP | MOB_CHARGE
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 8  SATYR */
    {
        "Satyr", "@@g",
        "A wild-eyed Satyr stamps its cloven hooves and raises a set of ram horns, madness and mischief warring in its gaze.",
        ELE_EARTH | ELE_MENTAL,
        ELE_SHADOW | ELE_FIRE,
        CAST_PHOBIA | CAST_EGO_WHIP, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_KICK | MOB_HEADBUTT | MOB_DISARM | MOB_COUNTER
        | MOB_PARRY | MOB_DODGE,
        0
    },

    /* 9  MEDUSA */
    {
        "Medusa", "@@g",
        "A Medusa glides forward on a coiled serpent body, her snake-hair hissing in a frenzy as she carefully avoids your eyes.",
        ELE_EARTH | ELE_MENTAL | ELE_POISON,
        ELE_HOLY | ELE_AIR,
        CAST_MIND_BOLT | CAST_BLOODY_TEARS | CAST_MIND_FLAIL, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_TRIP
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* ==================================================================== */
    /* ---- NORSE --------------------------------------------------------- */
    /* ==================================================================== */

    /* 10 FROST GIANT */
    {
        "Frost Giant", "@@a",
        "A Frost Giant blots out the horizon, each footfall cracking the ground with ice and its breath billowing in frozen clouds.",
        ELE_WATER | ELE_PHYSICAL,
        ELE_FIRE | ELE_EARTH,
        CAST_CHAIN_LIGHTNING | CAST_COLOR_SPRAY,
        DEF_ICESHIELD,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_KICK | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
        20
    },

    /* 11 JOTUNN */
    {
        "Jotunn", "@@y",
        "A primordial Jotunn strides from the mountains like a living avalanche, hurling boulders as casually as pebbles.",
        ELE_EARTH | ELE_WATER,
        ELE_AIR | ELE_HOLY,
        CAST_EARTHQUAKE | CAST_LAVA_BURST, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        20
    },

    /* 12 DRAUGR */
    {
        "Draugr", "@@d",
        "A Draugr lurches forward trailing grave-rot and grave-cold, its rusted blades still wickedly sharp and its dead eyes burning blue.",
        ELE_SHADOW | ELE_PHYSICAL,
        ELE_FIRE | ELE_HOLY,
        CAST_HELLSPAWN,
        DEF_CURE_LIGHT,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_DUALWIELD | MOB_HEADBUTT | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 13 LINDWORM */
    {
        "Lindworm", "@@g",
        "A Lindworm uncoils from the shadows, its vast wingless body scraping the stone as venomous spittle drips from its fangs.",
        ELE_POISON | ELE_EARTH,
        ELE_FIRE | ELE_AIR,
        CAST_ACID_BLAST | CAST_SUFFOCATE, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_TRIP | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        20
    },

    /* 14 FENRIR SPAWN */
    {
        "Fenrir Spawn", "@@N@@W",
        "A pup of the World Devourer paces with eerie calm, its jaws already large enough to swallow a man whole.",
        ELE_PHYSICAL | ELE_MENTAL,
        ELE_AIR | ELE_HOLY,
        0, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_TRIP | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* ==================================================================== */
    /* ---- CELTIC / BRITISH ---------------------------------------------- */
    /* ==================================================================== */

    /* 15 BANSHEE */
    {
        "Banshee", "@@d",
        "A Banshee drifts above the ground, her torn burial shroud trailing behind her and a death-wail building in her hollow throat.",
        ELE_SHADOW | ELE_MENTAL | ELE_AIR,
        ELE_HOLY | ELE_EARTH,
        CAST_PHOBIA | CAST_MIND_BOLT | CAST_BLOODY_TEARS, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 16 REDCAP */
    {
        "Redcap", "@@R",
        "A Redcap grins with iron-filed teeth, the cap on its head dripping fresh blood as it hefts a rusted pike in gnarled hands.",
        ELE_PHYSICAL | ELE_EARTH,
        ELE_HOLY | ELE_AIR,
        0, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_KICK | MOB_HEADBUTT | MOB_DIRT | MOB_TRIP
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 17 BOGGART */
    {
        "Boggart", "@@d",
        "A Boggart shimmers in and out of visibility, its chaotic laughter echoing from no fixed point as it circles closer.",
        ELE_MENTAL | ELE_SHADOW,
        ELE_HOLY | ELE_FIRE,
        CAST_PHOBIA | CAST_EGO_WHIP, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_DIRT | MOB_COUNTER
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 18 KELPIE */
    {
        "Kelpie", "@@a",
        "A Kelpie stands with deceptive beauty, its river-soaked hide gleaming and seaweed tangled in its mane as water drips from its hooves.",
        ELE_WATER | ELE_MENTAL,
        ELE_EARTH | ELE_FIRE,
        CAST_SUFFOCATE | CAST_SHOCKING_GRASP, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_KICK | MOB_CHARGE | MOB_TRIP
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 19 NUCKELAVEE */
    {
        "Nuckelavee", "@@R",
        "A Nuckelavee lurches forward on a horse-like body, its skin entirely absent and black blood pulsing visibly through yellow veins.",
        ELE_POISON | ELE_WATER,
        ELE_EARTH | ELE_HOLY,
        CAST_ACID_BLAST | CAST_SUFFOCATE, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* ==================================================================== */
    /* ---- EASTERN ------------------------------------------------------- */
    /* ==================================================================== */

    /* 20 KIRIN */
    {
        "Kirin", "@@Y",
        "A Kirin steps from radiant mist, its scaled hooves never quite touching the ground as divine light shimmers along its flanks.",
        ELE_HOLY | ELE_AIR,
        ELE_SHADOW | ELE_POISON,
        CAST_CHAIN_LIGHTNING | CAST_FLARE,
        DEF_CURE_SERIOUS,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_KICK | MOB_COUNTER
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 21 JIANGSHI */
    {
        "Jiangshi", "@@d",
        "A Jiangshi hops stiffly forward with arms outstretched, its face a rictus of death and a paper talisman fluttering from its brow.",
        ELE_SHADOW | ELE_PHYSICAL,
        ELE_HOLY | ELE_MENTAL,
        CAST_HELLSPAWN,
        DEF_CURE_LIGHT,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
        20
    },

    /* 22 TENGU */
    {
        "Tengu", "@@d",
        "A Tengu regards you with a crow's cold intelligence, its long-nosed mask hiding an immortal face as it loosens a blade in its sash.",
        ELE_AIR | ELE_MENTAL,
        ELE_EARTH | ELE_WATER,
        CAST_COLOR_SPRAY | CAST_STATIC, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_DISARM | MOB_KICK | MOB_COUNTER
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 23 ONI */
    {
        "Oni", "@@e",
        "An Oni towers over the doorway, its red horns scraping the ceiling as it hefts a studded iron club and bares its enormous fangs.",
        ELE_FIRE | ELE_PHYSICAL,
        ELE_HOLY | ELE_WATER,
        CAST_LAVA_BURST | CAST_FLAMESTRIKE | CAST_BURNING_HANDS,
        DEF_FIRESHIELD,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        20
    },

    /* 24 NAGA */
    {
        "Naga", "@@g",
        "A Naga sways with hypnotic grace, its cobra hood flared wide and the upper half of a divine warrior rising from a coiled serpent body.",
        ELE_POISON | ELE_WATER,
        ELE_FIRE | ELE_HOLY,
        CAST_ACID_BLAST | CAST_PLANERGY, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_TRIP | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* ==================================================================== */
    /* ---- GENERIC FANTASY ----------------------------------------------- */
    /* ==================================================================== */

    /* 25 WYVERN */
    {
        "Wyvern", "@@g",
        "A Wyvern beats its leathery wings to hold position, its barbed tail coiling and a bead of venom swelling at the tip of its stinger.",
        ELE_AIR | ELE_POISON,
        ELE_WATER | ELE_EARTH,
        CAST_PLANERGY | CAST_SUFFOCATE, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_KICK | MOB_CHARGE
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 26 BASILISK */
    {
        "Basilisk", "@@g",
        "A Basilisk drags its armoured bulk forward, every plant it passes withering and the stones beneath its feet showing the first cracks of petrification.",
        ELE_EARTH | ELE_MENTAL,
        ELE_WATER | ELE_HOLY,
        CAST_MIND_BOLT | CAST_MIND_FLAIL | CAST_BLOODY_TEARS, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_TRIP
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        20
    },

    /* 27 GRIFFON */
    {
        "Griffon", "@@Y",
        "A Griffon stands with the proud bearing of a noble beast, its eagle eyes locked on you as it mantles great wings and flexes razor talons.",
        ELE_AIR | ELE_PHYSICAL,
        ELE_EARTH | ELE_SHADOW,
        CAST_STATIC | CAST_CHAIN_LIGHTNING, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_KICK | MOB_CHARGE | MOB_MARTIAL
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 28 COCKATRICE */
    {
        "Cockatrice", "@@y",
        "A Cockatrice struts forward on scaled legs, its rooster-comb quivering with agitation and stone-grey eyes already beginning to glow.",
        ELE_EARTH | ELE_MENTAL | ELE_POISON,
        ELE_WATER | ELE_HOLY,
        CAST_MIND_FLAIL | CAST_BLOODY_TEARS, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_TRIP
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 29 WENDIGO */
    {
        "Wendigo", "@@d",
        "A Wendigo materialises from a gust of arctic wind, its emaciated frame grotesquely tall and the stench of old frost and older hunger rolling off it.",
        ELE_AIR | ELE_MENTAL | ELE_SHADOW,
        ELE_FIRE | ELE_HOLY,
        CAST_PHOBIA | CAST_SUFFOCATE | CAST_EGO_WHIP, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 30 GOLEM */
    {
        "Golem", "@@y",
        "A Golem advances with the grinding inevitability of a millstone, its carved stone face expressionless and runes of binding still glowing on its chest.",
        ELE_EARTH | ELE_PHYSICAL,
        ELE_AIR | ELE_MENTAL,
        CAST_EARTHQUAKE | CAST_HEAT_ARMOR, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
        30
    },

    /* 31 LICH */
    {
        "Lich", "@@d",
        "A Lich floats inches above the floor, its skeletal form draped in robes of decay and a corona of necrotic energy crackling around its outstretched hands.",
        ELE_SHADOW | ELE_MENTAL,
        ELE_HOLY | ELE_FIRE,
        CAST_HELLSPAWN | CAST_MIND_BOLT | CAST_MIND_FLAIL | CAST_EGO_WHIP
        | CAST_BLOODY_TEARS,
        DEF_CURE_CRITIC,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_COUNTER
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 32 TROLL */
    {
        "Troll", "@@g",
        "A Troll hunches forward sniffing the air, old wounds already closing as its warty grey hide knits itself back together before your eyes.",
        ELE_EARTH | ELE_PHYSICAL,
        ELE_FIRE | ELE_POISON,
        0,
        DEF_CURE_SERIOUS,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
        10
    },

    /* 33 OGRE SHAMAN */
    {
        "Ogre Shaman", "@@y",
        "An Ogre Shaman beats a bone-adorned club against its chest and bellows a guttural war-chant, the air rippling with crude but potent magic.",
        ELE_EARTH | ELE_MENTAL,
        ELE_HOLY | ELE_AIR,
        CAST_EARTHQUAKE | CAST_LAVA_BURST | CAST_PLANERGY,
        DEF_CURE_SERIOUS,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_HEADBUTT | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 34 SHADOW DRAKE */
    {
        "Shadow Drake", "@@d",
        "A Shadow Drake pours itself through the darkness like smoke given teeth, its young scales already drinking light and its tail cutting silently through the air.",
        ELE_SHADOW | ELE_AIR,
        ELE_HOLY | ELE_FIRE,
        CAST_HELLSPAWN | CAST_CHAIN_LIGHTNING | CAST_SUFFOCATE, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 35 LEVIATHAN SPAWN */
    {
        "Leviathan Spawn", "@@a",
        "A Leviathan Spawn slithers into view still streaming seawater, its iron-hard scales grinding against the walls and its eyes the pale grey of drowned things.",
        ELE_WATER | ELE_PHYSICAL,
        ELE_FIRE | ELE_MENTAL,
        CAST_SUFFOCATE | CAST_ACID_BLAST, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_TRIP | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        20
    },

    /* 36 ROC */
    {
        "Roc", "@@y",
        "A Roc stands like a feathered mountain, each wingtip brushing opposite walls and its hooked beak large enough to swallow cattle whole.",
        ELE_AIR | ELE_PHYSICAL,
        ELE_EARTH | ELE_FIRE,
        CAST_STATIC | CAST_SUFFOCATE, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_KICK | MOB_CHARGE | MOB_MARTIAL | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 37 THUNDERBIRD */
    {
        "Thunderbird", "@@l",
        "A Thunderbird alights in a crack of lightning, its feathers crackling with stored storm-energy and every eye of every bystander watering from the ozone.",
        ELE_AIR | ELE_WATER,
        ELE_EARTH | ELE_SHADOW,
        CAST_CHAIN_LIGHTNING | CAST_STATIC | CAST_SUFFOCATE,
        DEF_SHOCKSHIELD,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_KICK | MOB_CHARGE
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 38 PERYTON */
    {
        "Peryton", "@@d",
        "A Peryton turns to face you and casts a human shadow, its stag's antlers wreathed in dark mist as its hooves find purchase on the air itself.",
        ELE_AIR | ELE_SHADOW,
        ELE_HOLY | ELE_EARTH,
        CAST_MIND_BOLT | CAST_PHOBIA, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH
        | MOB_KICK | MOB_CHARGE | MOB_COUNTER
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 39 AMPHISBAENA */
    {
        "Amphisbaena", "@@g",
        "An Amphisbaena writhes toward you with a head at each end of its body, both sets of jaws snapping independently and neither needing to watch its back.",
        ELE_POISON | ELE_EARTH,
        ELE_FIRE | ELE_HOLY,
        CAST_ACID_BLAST, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_SIXTH
        | MOB_HEADBUTT | MOB_TRIP | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        20
    },

    /* 40 HIPPOGRIFF */
    {
        "Hippogriff", "@@y",
        "A Hippogriff tosses its eagle head and stamps a foreleg, the impossible blend of raptor and stallion eyeing you with fierce golden eyes.",
        ELE_AIR | ELE_PHYSICAL,
        ELE_EARTH | ELE_SHADOW,
        CAST_STATIC, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_KICK | MOB_CHARGE | MOB_MARTIAL
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },

    /* 41 BEHEMOTH */
    {
        "Behemoth", "@@y",
        "A Behemoth fills the horizon like a breathing fortress, its armoured hide shrugging off ballista bolts as it lowers its head and begins to charge.",
        ELE_EARTH | ELE_PHYSICAL,
        ELE_AIR | ELE_MENTAL,
        CAST_EARTHQUAKE, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_SIXTH
        | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
        30
    },

    /* 42 VALKYRIE SHADE */
    {
        "Valkyrie Shade", "@@l",
        "A Valkyrie Shade hovers in spectral armour, her spear levelled and her death-pale face serene with the absolute certainty of your doom.",
        ELE_HOLY | ELE_SHADOW | ELE_AIR,
        ELE_EARTH | ELE_MENTAL,
        CAST_CHAIN_LIGHTNING | CAST_FLARE | CAST_HELLSPAWN,
        DEF_CURE_SERIOUS,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_DISARM | MOB_COUNTER
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        0
    },

    /* 43 FOMORIAN */
    {
        "Fomorian", "@@d",
        "A Fomorian demon-giant lurches forward from primordial darkness, its misshapen body bristling with weapons and its single great eye blazing with ancient malice.",
        ELE_EARTH | ELE_SHADOW,
        ELE_HOLY | ELE_FIRE,
        CAST_EARTHQUAKE | CAST_LAVA_BURST, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        20
    },

    /* 44 ABYSSAL HOUND */
    {
        "Abyssal Hound", "@@R",
        "An Abyssal Hound paces with predatory patience, its obsidian coat flickering with hellfire and its brimstone breath scorching the ground beneath its paws.",
        ELE_SHADOW | ELE_FIRE,
        ELE_HOLY | ELE_WATER,
        CAST_HELLSPAWN | CAST_FLAMESTRIKE | CAST_LAVA_BURST, 0,
        MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH
        | MOB_TRIP | MOB_CHARGE | MOB_ENHANCED
        | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
        10
    },
};

#define NUM_INVASION_MOBS  45  /* must equal the array size above */

/* Name strings — order must match invasion_wave_profiles[] above. */
static const char *invasion_mob_names[NUM_INVASION_MOBS + 1] = {
    "Minotaur",         "Harpy",            "Gorgon",
    "Cyclops",          "Chimera",          "Cerberus",
    "Hydra",            "Manticore",        "Satyr",
    "Medusa",           "Frost Giant",      "Jotunn",
    "Draugr",           "Lindworm",         "Fenrir Spawn",
    "Banshee",          "Redcap",           "Boggart",
    "Kelpie",           "Nuckelavee",       "Kirin",
    "Jiangshi",         "Tengu",            "Oni",
    "Naga",             "Wyvern",           "Basilisk",
    "Griffon",          "Cockatrice",       "Wendigo",
    "Golem",            "Lich",             "Troll",
    "Ogre Shaman",      "Shadow Drake",     "Leviathan Spawn",
    "Roc",              "Thunderbird",      "Peryton",
    "Amphisbaena",      "Hippogriff",       "Behemoth",
    "Valkyrie Shade",   "Fomorian",         "Abyssal Hound",
    NULL
};

#endif /* INVASION_MOBS_H */
