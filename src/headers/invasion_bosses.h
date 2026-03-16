/***************************************************************************
 * invasion_bosses.h -- Boss profile table for the invasion system.        *
 *                                                                         *
 * Included ONCE by invasion.c.  Do NOT include anywhere else.             *
 *                                                                         *
 * Fields per entry:                                                       *
 *   name         – full display name shown to players                     *
 *   short_col    – ANSI colour prefix for short_descr (e.g. "@@e")        *
 *   announce_msg – global broadcast text when this boss spawns            *
 *   resist       – ELE_xxx bitmask: takes half damage from these elements  *
 *   suscept      – ELE_xxx bitmask: takes double damage from these elements*
 *   spells[]     – up to 6 spell names (NULL-terminated); cast each tick   *
 *   emote_cast   – act() string shown when the boss casts a spell         *
 *   emote_skill  – act() string shown when the boss uses its physical skill*
 *   skill_cmd    – argument passed to interpret() for the physical skill   *
 *                  one of: "bash"  "headbutt"  "dirt"                     *
 *                                                                         *
 * Element quick-reference (magic.h):                                      *
 *   ELE_NONE     BIT_0    ELE_PHYSICAL BIT_1    ELE_MENTAL   BIT_2        *
 *   ELE_HOLY     BIT_3    ELE_AIR      BIT_4    ELE_EARTH    BIT_5        *
 *   ELE_WATER    BIT_6    ELE_FIRE     BIT_7    ELE_SHADOW   BIT_8        *
 *   ELE_POISON   BIT_9                                                    *
 *                                                                         *
 * hp_mod, attack passes, and defensive skills are applied in              *
 * apply_boss_profile() inside invasion.c — not stored here.              *
 ***************************************************************************/

#ifndef INVASION_BOSSES_H
#define INVASION_BOSSES_H

static const BOSS_PROFILE boss_profiles[] = {
    /* ==================================================================
     * 0 — TYPHON, LORD OF ALL MONSTERS
     *     Primordial fire/earth/poison titan of Greek myth.
     *     He birthed the Chimera, Hydra, and Cerberus.
     *     Strengths : Fire, Earth, Poison
     *     Weaknesses: Water (extinguishes his volcanic wounds)
     *                 Holy  (Zeus's divine lightning)
     * ================================================================== */
    {"Typhon, Lord of All Monsters",
     "@@e",
     "The earth cracks open and @@eTyphon, Lord of All Monsters@@N ascends! "
     "A hundred dragon-heads roar in unison. Fire and venom are his breath!",
     ELE_FIRE | ELE_EARTH | ELE_POISON,
     ELE_WATER | ELE_HOLY,
     {"fire breath", "lava burst", "earthquake", "poison", "flamestrike", "weaken", NULL},
     "$n opens all one hundred of $s serpentine heads and unleashes a torrent of FIRE!",
     "$n slams the earth with mountain-levelling force, sending shockwaves across the land!",
     "bash"},

    /* ==================================================================
     * 1 — JORMUNGANDR'S HERALD
     *     Serpent spawn of Loki that encircles Midgard.
     *     Strengths : Water (sea-born), Poison (world-serpent venom)
     *     Weaknesses: Fire (dries the sea), Holy (Mjolnir's blessing)
     * ================================================================== */
    {"Jormungandr's Herald",
     "@@g",
     "The seas surge as @@gJormungandr's Herald@@N rises from the deep! "
     "Its venomous scales repel water and poison. The smell of death precedes it!",
     ELE_WATER | ELE_POISON,
     ELE_FIRE | ELE_HOLY,
     {"acid breath", "poison", "frost breath", "chill touch", "weaken", NULL},
     "$n rears back and spews a torrent of world-serpent venom that dissolves armour!",
     "$n coils around $s foe and CRUSHES with the strength of the tide!",
     "bash"},

    /* ==================================================================
     * 2 — THE PALE DESTROYER
     *     Ancient undead horror born of shadow and psychic malice.
     *     Strengths : Shadow, Mental
     *     Weaknesses: Holy (divine light scatters undead)
     *                 Air  (dispels clinging darkness)
     * ================================================================== */
    {"The Pale Destroyer",
     "@@d",
     "Darkness coils into form as @@dThe Pale Destroyer@@N materialises! "
     "Shadow and thought-attacks slide off $s form like water off obsidian!",
     ELE_SHADOW | ELE_MENTAL,
     ELE_HOLY | ELE_AIR,
     {"hellspawn", "blindness", "curse", "harm", "energy drain", "weaken", NULL},
     "$n extends $s hand and reality itself seems to crack and wither at the touch!",
     "$n tears at $s foe's very sanity with claws made of pure thought!",
     "headbutt"},

    /* ==================================================================
     * 3 — THE ABYSSAL TITAN
     *     A demon-giant that rose from the Abyss itself.
     *     Strengths : Earth (stonework armour), Shadow (demon nature)
     *     Weaknesses: Air (disperses shadow), Holy (divine radiance)
     * ================================================================== */
    {"The Abyssal Titan",
     "@@R",
     "The Abyss yawns wide and @@RThe Abyssal Titan@@N claws free! "
     "Stone and shadow seem to heal him — his laughter shakes the foundations of the earth!",
     ELE_EARTH | ELE_SHADOW,
     ELE_AIR | ELE_HOLY,
     {"earthquake", "hellspawn", "harm", "curse", "blindness", NULL},
     "$n tears a rift in the earth and hurls boulders of pure shadow at $s foes!",
     "$n raises $s iron fist and shatters the very ground beneath your feet!",
     "bash"},

    /* ==================================================================
     * 4 — SCYLLA THE DEVOURER
     *     Six-headed sea monster from the Strait of Messina.
     *     Strengths : Water (home element), Physical (thick chitinous hide)
     *     Weaknesses: Fire (dries her out), Mental (no unified mind)
     * ================================================================== */
    {"Scylla the Devourer",
     "@@a",
     "The harbour waters boil as @@aScylla the Devourer@@N emerges! "
     "Six dog-heads snap and tear. Water and physical blows bounce off her chitinous hull!",
     ELE_WATER | ELE_PHYSICAL,
     ELE_FIRE | ELE_MENTAL,
     {"frost breath", "chill touch", "acid breath", "poison", NULL},
     "$n's six heads spit simultaneous streams of ice-cold acid in all directions!",
     "$n lunges forward with three of $s dog-headed necks and RENDS flesh from bone!",
     "headbutt"},

    /* ==================================================================
     * 5 — SURTR, LORD OF FLAME
     *     Fire giant from Muspelheim destined to scorch all creation.
     *     Strengths : Fire (his domain), Earth (stone of Muspelheim)
     *     Weaknesses: Water (quenches flames), Air (fans but scatters)
     * ================================================================== */
    {"Surtr, Lord of Flame",
     "@@e",
     "The sky turns crimson as @@eSurtr, Lord of Flame@@N strides from Muspelheim! "
     "His burning sword makes fire and the burning earth his servants!",
     ELE_FIRE | ELE_EARTH,
     ELE_WATER | ELE_AIR,
     {"fire breath", "flamestrike", "lava burst", "burning hands", "fireball", NULL},
     "$n's flaming sword arcs overhead in a blazing arc that trails rivers of fire!",
     "$n charges with the speed of a wildfire, engulfing the battlefield in solar flame!",
     "bash"},

    /* ==================================================================
     * 6 — CHARYBDIS THE MAELSTROM
     *     Living whirlpool monster that swallows ships whole.
     *     Strengths : Water (the sea itself), Air (cyclonic power)
     *     Weaknesses: Earth (anchors and grounds her), Fire (turns to steam)
     * ================================================================== */
    {"Charybdis the Maelstrom",
     "@@a",
     "The ocean floor heaves as @@aCharybdis the Maelstrom@@N awakens! "
     "Wind and water feed her power — the horizon spins with her hunger!",
     ELE_WATER | ELE_AIR,
     ELE_EARTH | ELE_FIRE,
     {"frost breath", "lightning breath", "gas breath", "chill touch", "weaken", NULL},
     "$n opens $s maw and generates a cyclonic maelstrom of freezing, crushing air!",
     "$n creates a whirlpool, dragging everyone inexorably toward $s gnashing teeth!",
     "bash"},

    /* ==================================================================
     * 7 — NIDHOGG THE CORPSE-GNAWER
     *     Dragon that gnaws Yggdrasil's roots from below.
     *     Strengths : Poison (endless rot-venom), Shadow (underworld dark)
     *     Weaknesses: Holy (divine Yggdrasil sap), Fire (burns the rot)
     * ================================================================== */
    {"Nidhogg the Corpse-Gnawer",
     "@@d",
     "Yggdrasil shudders as @@dNidhogg the Corpse-Gnawer@@N tears free from Nastrond! "
     "Eons of corpse-venom coat every scale. Poison and darkness only feed it!",
     ELE_POISON | ELE_SHADOW,
     ELE_HOLY | ELE_FIRE,
     {"acid breath", "poison", "curse", "hellspawn", "energy drain", "blindness", NULL},
     "$n gnaws the air and breathes clouds of Hel-poison that dissolve the living!",
     "$n claws with talons coated in millennia of rotting Nastrond corpses!",
     "headbutt"},

    /* ==================================================================
     * 8 — THE ERLKING
     *     King of the faerie dead who rides to claim mortal souls.
     *     Strengths : Air (rides the night wind), Mental (fae glamour)
     *     Weaknesses: Fire (iron and flame break fae magic), Earth (iron)
     * ================================================================== */
    {"The Erlking",
     "@@g",
     "The forest recoils as @@gThe Erlking@@N rides forth on his pale horse! "
     "The night wind carries his voice — those who hear it are doomed!",
     ELE_AIR | ELE_MENTAL,
     ELE_FIRE | ELE_EARTH,
     {"gas breath", "weaken", "blindness", "curse", "harm", "chill touch", NULL},
     "$n whistles a faerie dirge so beautiful and so terrible that flesh strips from bone!",
     "$n's spectral mount rears and tramples everything beneath iron-shod hooves!",
     "bash"},

    /* ==================================================================
     * 9 — AMMIT, DEVOURER OF THE UNWORTHY
     *     Egyptian chimera that eats the hearts of the judged dead.
     *     Strengths : Holy (divine judgment), Physical (beast body)
     *     Weaknesses: Shadow (Ma'at's opposite), Poison (impurity)
     * ================================================================== */
    {"Ammit, Devourer of the Unworthy",
     "@@Y",
     "The scales of Ma'at tip as @@YAmmit, Devourer of the Unworthy@@N is unleashed! "
     "Holy light and mortal steel are her instruments — she judges all souls forfeit!",
     ELE_HOLY | ELE_PHYSICAL,
     ELE_SHADOW | ELE_POISON,
     {"flamestrike", "harm", "curse", "blindness", "weaken", NULL},
     "$n judges your soul and finds it WANTING — $s jaws close on your very essence!",
     "$n pounces with lion's body, crocodile's jaws, and hippo's crushing force!",
     "bash"},

    /* ==================================================================
     * 10 — APEP, THE CHAOS SERPENT
     *      Egyptian serpent of chaos that seeks to devour the sun.
     *      Strengths : Shadow, Mental, Poison
     *      Weaknesses: Holy (Ra's divine fire), Fire (the sun itself)
     * ================================================================== */
    {"Apep, the Chaos Serpent",
     "@@d",
     "The sun flickers as @@dApep, the Chaos Serpent@@N uncoils from the Duat! "
     "He drinks shadow and madness — the concept of order itself recoils from him!",
     ELE_SHADOW | ELE_MENTAL | ELE_POISON,
     ELE_HOLY | ELE_FIRE,
     {"hellspawn", "poison", "harm", "blindness", "energy drain", "weaken", NULL},
     "$n roars and reality tears — the ORDER of creation bends under $s chaos!",
     "$n wraps in coils of living darkness and begins to SQUEEZE the life away!",
     "bash"},

    /* ==================================================================
     * 11 — TALOS, THE BRONZE COLOSSUS
     *      Automaton giant of Crete, forged by Hephaestus.
     *      Strengths : Earth (bronze from Cretan rock), Physical (metal body)
     *      Weaknesses: Water (rust and bronze fatigue), Mental (no mind)
     * ================================================================== */
    {"Talos, the Bronze Colossus",
     "@@Y",
     "The island shakes as @@YTalos, the Bronze Colossus@@N strides ashore! "
     "His bronze hide deflects physical blows and stone shrapnel alike!",
     ELE_EARTH | ELE_PHYSICAL,
     ELE_WATER | ELE_MENTAL,
     {"earthquake", "lava burst", "burning hands", "harm", NULL},
     "$n heats $s bronze blood to molten ichor and hurls it at $s foes!",
     "$n's bronze foot descends with the gravitational force of a falling mountain!",
     "bash"},

    /* ==================================================================
     * 12 — YUKI-ONNA, THE SNOW WOMAN
     *      Japanese spirit of blizzards and frozen death.
     *      Strengths : Water (ice and snow), Air (blizzard winds), Mental (fae)
     *      Weaknesses: Fire (melts snow), Earth (grounds the storm)
     * ================================================================== */
    {"Yuki-Onna, the Snow Woman",
     "@@a",
     "A blizzard sweeps in from nowhere as @@aYuki-Onna, the Snow Woman@@N drifts forth! "
     "Cold, wind and the terror of frozen death are her playthings!",
     ELE_WATER | ELE_AIR | ELE_MENTAL,
     ELE_FIRE | ELE_EARTH,
     {"frost breath", "chill touch", "blindness", "weaken", "harm", NULL},
     "$n exhales a breath so cold it freezes the very air into lethal crystalline razors!",
     "$n dissolves into a howling blizzard and reforms inside your guard with deadly grace!",
     "dirt"},

    /* ==================================================================
     * 13 — BALOR, KING OF THE FOMORIANS
     *      Irish giant whose single eye obliterates everything it looks upon.
     *      Strengths : Fire (his eye-beam), Shadow (deep-ocean darkness)
     *      Weaknesses: Water (cools the eye), Holy (Lugh's divine spear)
     * ================================================================== */
    {"Balor, King of the Fomorians",
     "@@e",
     "The world shakes as @@eBalor, King of the Fomorians@@N opens his cyclopean eye! "
     "That baleful gaze emanates fire and shadow — nothing can shield you from it!",
     ELE_FIRE | ELE_SHADOW,
     ELE_WATER | ELE_HOLY,
     {"fire breath", "hellspawn", "flamestrike", "fireball", "blindness", "curse", NULL},
     "$n slowly lifts $s cyclopean eyelid — the Evil Eye opens and OBLITERATES!",
     "$n hurls $s burning iron club, levelling everything within a half-mile arc!",
     "bash"},

    /* ==================================================================
     * 14 — FENRISULFR REBORN
     *      The great wolf of Ragnarok, reborn and unbound.
     *      Strengths : Physical (legendary toughness), Mental (feral will)
     *      Weaknesses: Air (Gleipnir's ethereal chain), Holy (Tyr's sacrifice)
     * ================================================================== */
    {"Fenrisulfr Reborn",
     "@@N@@W",
     "The sky splits as @@WFenrisulfr Reborn@@N shatters his eternal chains! "
     "Physical blows and mental assaults seem to strengthen the great wolf!",
     ELE_PHYSICAL | ELE_MENTAL,
     ELE_AIR | ELE_HOLY,
     {"blindness", "curse", "weaken", "harm", NULL},
     "$n howls with jaws wide enough to swallow the sky — the sound alone SHATTERS stone!",
     "$n lunges and bites with jaws that could swallow the sun and moon in one snap!",
     "bash"},
};

#define NUM_BOSSES 15 /* must stay equal to the array size above */

#endif /* INVASION_BOSSES_H */
