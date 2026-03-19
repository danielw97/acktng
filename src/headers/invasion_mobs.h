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

static const WAVE_MOB_PROFILE invasion_wave_profiles[] = {
    /* ==================================================================== */
    /* ---- SOLAR COURT / DESERT PYRAMID ---------------------------------- */
    /* ==================================================================== */

    /* 0  TOMB SENTINEL */
    {"Tomb Sentinel", "@@y",
     "A Tomb Sentinel strides forward in layered bronze-gilt armour, its preserved face locked in "
     "eternal vigilance and the Solar Court's cartouche still legible on its breastplate.",
     ELE_PHYSICAL | ELE_EARTH, ELE_FIRE | ELE_HOLY, 0, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     20},

    /* 1  SAND-VEIL WRAITH */
    {"Sand-Veil Wraith", "@@y",
     "A Sand-Veil Wraith coalesces from the drift of hot grit, its outline blurring where flesh "
     "should be as hollowed eye-pits track your movement.",
     ELE_AIR | ELE_SHADOW, ELE_HOLY | ELE_EARTH, CAST_PHOBIA | CAST_SUFFOCATE, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_DIRT | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 2  EMBALMER-PRIEST */
    {"Embalmer-Priest", "@@y",
     "An Embalmer-Priest advances with hands still stained in the preserving rites, its wrapped "
     "form radiating the cold of the preparation chamber and its tools repurposed for harm.",
     ELE_POISON | ELE_MENTAL, ELE_FIRE | ELE_HOLY,
     CAST_ACID_BLAST | CAST_MIND_BOLT | CAST_BLOODY_TEARS, DEF_CURE_LIGHT,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 3  ASP CULTIST */
    {"Asp Cultist", "@@g",
     "An Asp Cultist moves in the patient weaving gait of a strike about to land, serpent-scale "
     "tattoos livid across its throat and poison-grooved blades loose in both hands.",
     ELE_POISON | ELE_PHYSICAL, ELE_HOLY | ELE_WATER, CAST_PLANERGY | CAST_ACID_BLAST, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_KICK | MOB_TRIP | MOB_DISARM |
         MOB_COUNTER | MOB_PARRY | MOB_DODGE,
     10},

    /* 4  SOLAR SCARAB SWARM */
    {"Solar Scarab Swarm", "@@e",
     "A Solar Scarab Swarm rolls forward like a living tide of bronze-shelled fire, each beetle "
     "carrying a fragment of the pyramid's consecrated heat in its shell.",
     ELE_FIRE | ELE_EARTH, ELE_WATER | ELE_AIR, CAST_LAVA_BURST | CAST_BURNING_HANDS, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_SIXTH | MOB_CHARGE | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     10},

    /* 5  BONE-WEAVER */
    {"Bone-Weaver", "@@d",
     "A Bone-Weaver pauses at the threshold and begins to reassemble itself from the nearest "
     "wreckage, suturing new components into its frame as it studies you with vacant courtesy.",
     ELE_SHADOW | ELE_EARTH, ELE_HOLY | ELE_FIRE, CAST_HELLSPAWN, DEF_CURE_LIGHT,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_ENHANCED | MOB_PARRY |
         MOB_DODGE | MOB_NODISARM,
     10},

    /* 6  VENOM-HIEROPHANT */
    {"Venom-Hierophant", "@@g",
     "A Venom-Hierophant of the Moon Sovereign's fifth tier sweeps forward in robes damp with "
     "offered poison, its liturgy reduced to hissing syllables that raise blisters on the air.",
     ELE_POISON | ELE_SHADOW, ELE_HOLY | ELE_FIRE, CAST_ACID_BLAST | CAST_PLANERGY | CAST_SUFFOCATE,
     DEF_CURE_SERIOUS,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 7  KHAMSIN ILLUSIONIST */
    {"Khamsin Illusionist", "@@d",
     "A Khamsin Illusionist from the Vizier's court steps through a door that was not there a "
     "moment ago, composing its deception from shadow and reflected torchlight.",
     ELE_MENTAL | ELE_SHADOW, ELE_HOLY | ELE_PHYSICAL,
     CAST_MIND_BOLT | CAST_EGO_WHIP | CAST_PHOBIA | CAST_MIND_FLAIL, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_DIRT | MOB_COUNTER | MOB_PARRY | MOB_DODGE |
         MOB_NODISARM,
     0},

    /* 8  SUNDERED SCARAB-KNIGHT */
    {"Sundered Scarab-Knight", "@@e",
     "A Sundered Scarab-Knight of the Solar Court charges with a champion's form, the golden "
     "carapace of its ritual armour cracked and its oath of service rewritten by a darker hand.",
     ELE_PHYSICAL | ELE_FIRE, ELE_WATER | ELE_SHADOW, CAST_HEAT_ARMOR | CAST_FLAMESTRIKE, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_CHARGE | MOB_MARTIAL |
         MOB_ENHANCED | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     20},

    /* 9  APEP'S COIL */
    {"Apep's Coil", "@@d",
     "A Coil of Apep unwinds from the darkness, a section of the god-serpent's endless body "
     "animated by spite alone, its scales drinking all light that falls on them.",
     ELE_SHADOW | ELE_POISON, ELE_HOLY | ELE_FIRE, CAST_SUFFOCATE | CAST_HELLSPAWN, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_SIXTH | MOB_TRIP | MOB_CHARGE |
         MOB_ENHANCED | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     20},

    /* ==================================================================== */
    /* ---- KEL'SHADRA INSTITUTIONAL COMPLEX ------------------------------ */
    /* ==================================================================== */

    /* 10 CHAIN-OFFICER */
    {"Chain-Officer", "@@d",
     "A Chain-Officer of the Ninth Chain steps from the Crypts' enforcement corridor still wearing "
     "its docket of active warrants, restraint-links jangling with the weight of old sentences.",
     ELE_PHYSICAL | ELE_SHADOW, ELE_HOLY | ELE_AIR, 0, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_DUALWIELD | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     20},

    /* 11 NULL-SCRIBED SHADE */
    {"Null-Scribed Shade", "@@d",
     "A Null-Scribed Shade drifts where its file once was, its name struck from every legible "
     "surface in the Chamber of Redactions and its remaining selfhood condensed to pure grievance.",
     ELE_MENTAL | ELE_SHADOW, ELE_HOLY | ELE_EARTH, CAST_MIND_BOLT | CAST_PHOBIA, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_DIRT | MOB_COUNTER | MOB_PARRY | MOB_DODGE |
         MOB_NODISARM,
     0},

    /* 12 GLOAMVAULT WARDEN */
    {"Gloamvault Warden", "@@d",
     "A Gloamvault Warden stands at its eternal post, the archive-keys fused to its fingers and "
     "the Vaultkeeper's seal still burning at its throat even as its orders have long since "
     "rotted to something unrecognisable.",
     ELE_SHADOW | ELE_MENTAL, ELE_FIRE | ELE_HOLY,
     CAST_MIND_FLAIL | CAST_MIND_BOLT | CAST_BLOODY_TEARS, DEF_CURE_CRITIC,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 13 RECURRENCE SENTINEL */
    {"Recurrence Sentinel", "@@l",
     "A Recurrence Sentinel of the Void Citadel advances in perfect formation-step, its chassis "
     "a construct of institutional memory and its last recorded order still executing flawlessly.",
     ELE_MENTAL | ELE_AIR, ELE_EARTH | ELE_FIRE, CAST_CHAIN_LIGHTNING | CAST_STATIC, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_DISARM | MOB_COUNTER | MOB_MARTIAL |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     10},

    /* 14 CENSURE-BOUND */
    {"Censure-Bound", "@@d",
     "A Censure-Bound soldier lurches under the weight of its own punishment, the administrative "
     "seal of its sentence bolted through its collar and its weapons still issued by an office "
     "that disavowed it decades ago.",
     ELE_PHYSICAL | ELE_MENTAL, ELE_HOLY | ELE_AIR, 0, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_DUALWIELD | MOB_HEADBUTT | MOB_COUNTER |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     10},

    /* 15 ARCHIVE REVENANT */
    {"Archive Revenant", "@@d",
     "An Archive Revenant assembles from scattered documents, its form built from compacted ledger "
     "pages and its voice the dry scratch of an autoquill still transcribing.",
     ELE_MENTAL | ELE_SHADOW, ELE_HOLY | ELE_FIRE,
     CAST_EGO_WHIP | CAST_MIND_BOLT | CAST_BLOODY_TEARS, DEF_CURE_SERIOUS,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 16 VOID HERALD */
    {"Void Herald", "@@l",
     "A Void Herald drops from an aperture in the air above you, its courier satchel still sealed "
     "and the Recurrence Chancery's stamp confirming a dispatch date that has not yet arrived.",
     ELE_AIR | ELE_MENTAL, ELE_EARTH | ELE_SHADOW, CAST_STATIC | CAST_COLOR_SPRAY, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_KICK | MOB_COUNTER | MOB_PARRY | MOB_DODGE |
         MOB_NODISARM,
     0},

    /* 17 WITNESS-STRIPPED */
    {"Witness-Stripped", "@@d",
     "A Witness-Stripped wraith moves on instinct alone, its identity dissolved by the Inverse "
     "Chancel's procedures and its remaining behaviour shaped entirely by what it was forced "
     "to testify against.",
     ELE_SHADOW | ELE_MENTAL, ELE_HOLY | ELE_FIRE, CAST_BLOODY_TEARS | CAST_PHOBIA, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_DIRT | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 18 WARDEN-SHARD */
    {"Warden-Shard", "@@d",
     "A Warden-Shard is a splinter of Warden-Prime Istrane's fractured authority given independent "
     "will, still wearing the insignia of the Ninth Chain and still executing containment "
     "protocol.",
     ELE_SHADOW | ELE_PHYSICAL, ELE_HOLY | ELE_AIR, 0, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     20},

    /* 19 RECURSOR-SPAWN */
    {"Recursor-Spawn", "@@d",
     "A Recursor-Spawn materialises as a partial echo of the Void Citadel's recurrence engine, "
     "its logic intact but its purpose narrowed to a single unresolved case file: yours.",
     ELE_MENTAL | ELE_SHADOW, ELE_HOLY | ELE_EARTH,
     CAST_MIND_FLAIL | CAST_HELLSPAWN | CAST_EGO_WHIP, DEF_CURE_LIGHT,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* ==================================================================== */
    /* ---- SPIREBOUND CONCLAVE / DEPTHS ---------------------------------- */
    /* ==================================================================== */

    /* 20 FLAMEBOUND SENTINEL */
    {"Flamebound Sentinel", "@@e",
     "A Flamebound Sentinel of the Ember Canon stands at the Sunken Sanctum's perimeter, its "
     "body a lattice of heat-calcined stone and active combustion, the Conclave's fire-seal "
     "still blazing at its core.",
     ELE_FIRE | ELE_EARTH, ELE_WATER | ELE_AIR,
     CAST_FLAMESTRIKE | CAST_LAVA_BURST | CAST_BURNING_HANDS, DEF_FIRESHIELD,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     20},

    /* 21 PRISM-CORRUPTED SCOUT */
    {"Prism-Corrupted Scout", "@@p",
     "A Prism-Corrupted Scout from the Conclave's failed field trials stumbles forward with "
     "fractured light leaking from its joints, its senses split across too many spectra to "
     "resolve a single coherent reality.",
     ELE_MENTAL | ELE_POISON, ELE_HOLY | ELE_FIRE,
     CAST_MIND_BOLT | CAST_ACID_BLAST | CAST_COLOR_SPRAY, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_TRIP | MOB_DIRT | MOB_PARRY | MOB_DODGE |
         MOB_NODISARM,
     0},

    /* 22 WITHERED DRUID */
    {"Withered Druid", "@@g",
     "A Withered Druid staggers from the Depths' infected root-line, the Blightmother's spores "
     "flowering along its clavicles and its earth-commune rites now channelling pure corruption.",
     ELE_EARTH | ELE_POISON, ELE_FIRE | ELE_HOLY, CAST_EARTHQUAKE | CAST_ACID_BLAST | CAST_PLANERGY,
     DEF_CURE_LIGHT,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_HEADBUTT | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 23 BLIGHTWOOD SHAMBLER */
    {"Blightwood Shambler", "@@g",
     "A Blightwood Shambler uproots itself from the infected soil, a walking knot of diseased "
     "heartwood and suppurating fungal matter, its hollow core pulsing with the Blightmother's "
     "substrate rhythm.",
     ELE_EARTH | ELE_WATER, ELE_FIRE | ELE_HOLY, CAST_EARTHQUAKE | CAST_SUFFOCATE, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_CHARGE | MOB_TRIP |
         MOB_ENHANCED | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     30},

    /* 24 PALE LEDGER ARCHIVIST */
    {"Pale Ledger Archivist", "@@W",
     "A Pale Ledger Archivist of the Sunken Sanctum's administrative fraction drifts forward, "
     "its form half-preserved in the Conclave's documentation rites and its quill still "
     "scratching entries no living hand will read.",
     ELE_SHADOW | ELE_MENTAL, ELE_HOLY | ELE_FIRE,
     CAST_HELLSPAWN | CAST_EGO_WHIP | CAST_BLOODY_TEARS, DEF_CURE_SERIOUS,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 25 THORNFANG HUNTER */
    {"Thornfang Hunter", "@@g",
     "A Thornfang Hunter from the Verdant Depths' apex predator line circles at the edge of "
     "striking distance, its barbed flanks quivering and the tracking-sense of ten generations "
     "of territorial selection locked onto you.",
     ELE_POISON | ELE_PHYSICAL, ELE_FIRE | ELE_HOLY, CAST_PLANERGY | CAST_ACID_BLAST, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_KICK | MOB_TRIP | MOB_CHARGE |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     10},

    /* 26 ROOTMOTHER'S TENDRIL */
    {"Rootmother's Tendril", "@@g",
     "A Tendril of the Rootmother punches through the floor and gropes toward you with "
     "vegetative patience, the Verdant Depths' primary intelligence directing it from kilometres "
     "below through a network of pressurised sap.",
     ELE_EARTH | ELE_WATER, ELE_FIRE | ELE_AIR, CAST_PLANERGY | CAST_SUFFOCATE, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_SIXTH | MOB_TRIP | MOB_CHARGE |
         MOB_ENHANCED | MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     20},

    /* 27 ASHFALL REVENANT */
    {"Ashfall Revenant", "@@e",
     "An Ashfall Revenant burns with the preserved memory of the Monsoon, its body a column of "
     "cindered remains and its heat an ambient threat that chars the floorboards at its feet.",
     ELE_FIRE | ELE_SHADOW, ELE_WATER | ELE_HOLY,
     CAST_FLAMESTRIKE | CAST_HELLSPAWN | CAST_LAVA_BURST, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_CHARGE | MOB_ENHANCED | MOB_PARRY |
         MOB_DODGE | MOB_NODISARM,
     10},

    /* 28 CRYSTAL SUBSTRATE WARDEN */
    {"Crystal Substrate Warden", "@@W",
     "A Crystal Substrate Warden of Thornwood stands on a bed of self-grown facets, its body "
     "threaded through with the crystal substrate's binding lattice and its patrol-range "
     "extended beyond any prior record.",
     ELE_EARTH | ELE_MENTAL, ELE_FIRE | ELE_AIR, CAST_EARTHQUAKE | CAST_MIND_BOLT | CAST_HEAT_ARMOR,
     DEF_CURE_LIGHT,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED | MOB_PARRY |
         MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     20},

    /* 29 HEARTWOOD GUARDIAN */
    {"Heartwood Guardian", "@@g",
     "A Heartwood Guardian of the Verdant Depths unfolds from a standing tree-form, its bark "
     "face creased with the patience of centuries and its root-anchored limbs already "
     "calculating the shortest path to your vital organs.",
     ELE_EARTH | ELE_WATER, ELE_FIRE | ELE_SHADOW, CAST_EARTHQUAKE | CAST_SUFFOCATE, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_TRIP | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     30},

    /* ==================================================================== */
    /* ---- SHADOWMERE / UMBRA HEARTSPIRE --------------------------------- */
    /* ==================================================================== */

    /* 30 OATHBOUND MARCH SOLDIER */
    {"Oathbound March Soldier", "@@d",
     "An Oathbound March Soldier patrols the processional road it died defending, its emergency "
     "command articles still sealed at its belt and its hollow eyes registering you as an "
     "unclassified intrusion requiring termination.",
     ELE_PHYSICAL | ELE_SHADOW, ELE_FIRE | ELE_HOLY, 0, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_DUALWIELD | MOB_HEADBUTT | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     20},

    /* 31 BINDER COLLEGIUM CLERK */
    {"Binder Collegium Clerk", "@@d",
     "A Binder Collegium Clerk advances with ink-blackened fingers and the methodical calm of "
     "someone who treats every soul it encounters as a case file awaiting proper categorisation.",
     ELE_SHADOW | ELE_MENTAL, ELE_HOLY | ELE_FIRE, CAST_HELLSPAWN | CAST_MIND_BOLT | CAST_EGO_WHIP,
     DEF_CURE_SERIOUS,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 32 GALLOWS SYNDICATE ENFORCER */
    {"Gallows Syndicate Enforcer", "@@d",
     "A Gallows Syndicate Enforcer from Shadowmere's sentence infrastructure approaches with "
     "its coil of toxin-soaked wire and its chit-ledger of outstanding collection orders, "
     "your name already pencilled onto the next available line.",
     ELE_POISON | ELE_PHYSICAL, ELE_HOLY | ELE_FIRE, CAST_ACID_BLAST | CAST_PLANERGY, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_KICK | MOB_TRIP | MOB_DIRT | MOB_DISARM | MOB_PARRY |
         MOB_DODGE,
     10},

    /* 33 CRIMSON HOUSEHOLD RETAINER */
    {"Crimson Household Retainer", "@@R",
     "A Crimson Household Retainer of the Countess-Regent's court glides forward with courtly "
     "precision, its audience-ritual training and its blood-cask logistics reduced to a single "
     "polished form of lethality.",
     ELE_SHADOW | ELE_PHYSICAL, ELE_HOLY | ELE_FIRE, 0, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_DUALWIELD | MOB_DISARM | MOB_COUNTER |
         MOB_MARTIAL | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     10},

    /* 34 ASTRAL MORTIFIER */
    {"Astral Mortifier", "@@p",
     "An Astral Mortifier of the Heartspire's Orrery Wing adjusts its null-sigil instruments "
     "as it walks, treating you as a variable whose removal is already cosmically ratified in "
     "a forecast it filed three years ago.",
     ELE_MENTAL | ELE_AIR, ELE_EARTH | ELE_FIRE,
     CAST_MIND_FLAIL | CAST_MIND_BOLT | CAST_EGO_WHIP | CAST_PHOBIA, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 35 EBON PULSE VESSEL */
    {"Ebon Pulse Vessel", "@@d",
     "An Ebon Pulse Vessel is a body the Ebon Hierophancy emptied of its previous occupant and "
     "filled with Noctivar's doctrine, its eyes the flat violet of the Heartspire's deep "
     "shadow registers and its purpose the perpetuation of institutional continuity at any cost.",
     ELE_SHADOW | ELE_MENTAL, ELE_HOLY | ELE_FIRE,
     CAST_HELLSPAWN | CAST_BLOODY_TEARS | CAST_PHOBIA | CAST_MIND_BOLT, DEF_CURE_SERIOUS,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_COUNTER | MOB_PARRY | MOB_DODGE | MOB_NODISARM, 0},

    /* 36 LAST BELL REVENANT */
    {"Last Bell Revenant", "@@d",
     "A Last Bell Revenant holds its post at the position where three contradictory decrees "
     "reached it simultaneously, its discipline intact and its loyalties permanently "
     "unresolvable, executing all three orders in rotation.",
     ELE_PHYSICAL | ELE_SHADOW, ELE_HOLY | ELE_AIR, 0, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM | MOB_NOTRIP,
     20},

    /* 37 NULL-ABSOLUTION WRAITH */
    {"Null-Absolution Wraith", "@@d",
     "A Null-Absolution Wraith drifts from the Binder Collegium's circle chambers, a soul "
     "processed through a null-absolution rite that granted neither release nor continuity, "
     "caught in permanent administrative suspension.",
     ELE_SHADOW | ELE_MENTAL, ELE_HOLY | ELE_EARTH, CAST_PHOBIA | CAST_EGO_WHIP, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_DIRT | MOB_COUNTER | MOB_PARRY | MOB_DODGE |
         MOB_NODISARM,
     0},

    /* 38 IRON WAKE INQUISITOR */
    {"Iron Wake Inquisitor", "@@d",
     "An Iron Wake Inquisitor steps from the Heartspire's Bastion Wing with a sentence already "
     "prepared, its chain-of-custody records updated in real time and its methods refined by "
     "three centuries of making adjudication indistinguishable from execution.",
     ELE_PHYSICAL | ELE_MENTAL, ELE_HOLY | ELE_AIR, CAST_MIND_BOLT | CAST_HEAT_ARMOR, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_DISARM | MOB_COUNTER | MOB_KICK |
         MOB_MARTIAL | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     10},

    /* ==================================================================== */
    /* ---- THORNWOOD / KOWLOON / FRONTIER -------------------------------- */
    /* ==================================================================== */

    /* 39 WILD HUNT RIDER */
    {"Wild Hunt Rider", "@@W",
     "A Wild Hunt Rider breaks from the treeline at full gallop, its mount's hooves never quite "
     "settling on the ground and its antlered helm framing a face that has been watching you "
     "since you entered Thornwood's outermost root-shadow.",
     ELE_AIR | ELE_PHYSICAL, ELE_EARTH | ELE_SHADOW, CAST_STATIC | CAST_CHAIN_LIGHTNING, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_KICK | MOB_CHARGE | MOB_MARTIAL |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     10},

    /* 40 ROOT COVENANT DRUID */
    {"Root Covenant Druid", "@@g",
     "A Root Covenant Druid of the Thornwood rises from the loam it was kneeling in, the "
     "earth-communion of the crystal substrate still threading its fingers and its binding "
     "rites drawn from geology rather than sky.",
     ELE_EARTH | ELE_WATER, ELE_FIRE | ELE_AIR, CAST_EARTHQUAKE | CAST_PLANERGY | CAST_SUFFOCATE,
     DEF_CURE_SERIOUS,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_HEADBUTT | MOB_COUNTER | MOB_PARRY | MOB_DODGE |
         MOB_NODISARM,
     0},

    /* 41 ASH CULT ZEALOT */
    {"Ash Cult Zealot", "@@e",
     "An Ash Cult Zealot of the Ember Speaker's congregation surges forward with the reckless "
     "certainty of the converted, its skin scarified in the Cult's ignition-marks and its "
     "contempt for Kowloon's cooler doctrines made physical.",
     ELE_FIRE | ELE_MENTAL, ELE_WATER | ELE_HOLY,
     CAST_FLAMESTRIKE | CAST_LAVA_BURST | CAST_BURNING_HANDS, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_HEADBUTT | MOB_CHARGE | MOB_ENHANCED |
         MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     10},

    /* 42 NEON COVENANT OPERATIVE */
    {"Neon Covenant Operative", "@@l",
     "A Neon Covenant Operative of Kowloon's rival doctrine moves in bursts of controlled "
     "speed, its augmented senses scanning for threat vectors and the Covenant's light-script "
     "tattoos pulsing with each calculation.",
     ELE_AIR | ELE_MENTAL, ELE_EARTH | ELE_SHADOW,
     CAST_STATIC | CAST_CHAIN_LIGHTNING | CAST_COLOR_SPRAY, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_DISARM | MOB_KICK | MOB_COUNTER |
         MOB_MARTIAL | MOB_PARRY | MOB_DODGE | MOB_NODISARM,
     0},

    /* 43 BLACKTIDE CORSAIR */
    {"Blacktide Corsair", "@@a",
     "A Blacktide Corsair from Captain Blacktide Shen's fleet vaults the rail with a boarding "
     "hook trailing, salt-crusted weapons in both hands and the territorial contempt of someone "
     "who has never conceded a route dispute.",
     ELE_PHYSICAL | ELE_WATER, ELE_FIRE | ELE_EARTH, CAST_SHOCKING_GRASP | CAST_STATIC, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_DUALWIELD | MOB_KICK | MOB_TRIP |
         MOB_DISARM | MOB_COUNTER | MOB_PARRY | MOB_DODGE,
     10},

    /* 44 NIGHTFALL SHADE */
    {"Nightfall Shade", "@@d",
     "A Nightfall Shade pools at the junction of shadow-lines, a remnant of the Night of "
     "Unsounded Bells preserved in suspension and now released by the collapse of the "
     "institutional wards that kept it contained.",
     ELE_SHADOW | ELE_AIR, ELE_HOLY | ELE_FIRE,
     CAST_MIND_BOLT | CAST_BLOODY_TEARS | CAST_PHOBIA | CAST_SUFFOCATE, 0,
     MOB_SECOND | MOB_THIRD | MOB_FOURTH | MOB_FIFTH | MOB_DIRT | MOB_COUNTER | MOB_PARRY |
         MOB_DODGE | MOB_NODISARM,
     0},
};

#define NUM_INVASION_MOBS 45 /* must equal the array size above */

/* Name strings — order must match invasion_wave_profiles[] above. */
static const char *invasion_mob_names[NUM_INVASION_MOBS + 1]
    __attribute__((unused)) = {"Tomb Sentinel",
                               "Sand-Veil Wraith",
                               "Embalmer-Priest",
                               "Asp Cultist",
                               "Solar Scarab Swarm",
                               "Bone-Weaver",
                               "Venom-Hierophant",
                               "Khamsin Illusionist",
                               "Sundered Scarab-Knight",
                               "Apep's Coil",
                               "Chain-Officer",
                               "Null-Scribed Shade",
                               "Gloamvault Warden",
                               "Recurrence Sentinel",
                               "Censure-Bound",
                               "Archive Revenant",
                               "Void Herald",
                               "Witness-Stripped",
                               "Warden-Shard",
                               "Recursor-Spawn",
                               "Flamebound Sentinel",
                               "Prism-Corrupted Scout",
                               "Withered Druid",
                               "Blightwood Shambler",
                               "Pale Ledger Archivist",
                               "Thornfang Hunter",
                               "Rootmother's Tendril",
                               "Ashfall Revenant",
                               "Crystal Substrate Warden",
                               "Heartwood Guardian",
                               "Oathbound March Soldier",
                               "Binder Collegium Clerk",
                               "Gallows Syndicate Enforcer",
                               "Crimson Household Retainer",
                               "Astral Mortifier",
                               "Ebon Pulse Vessel",
                               "Last Bell Revenant",
                               "Null-Absolution Wraith",
                               "Iron Wake Inquisitor",
                               "Wild Hunt Rider",
                               "Root Covenant Druid",
                               "Ash Cult Zealot",
                               "Neon Covenant Operative",
                               "Blacktide Corsair",
                               "Nightfall Shade",
                               NULL};

#endif /* INVASION_MOBS_H */
