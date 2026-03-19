/***************************************************************************
 * invasion_bosses.h -- Boss profile table for the invasion system.        *
 *                                                                         *
 * Included ONCE by invasion.c.  Do NOT include anywhere else.             *
 *                                                                         *
 * All bosses are drawn from canonical game lore (docs/lore/).             *
 *                                                                         *
 * Fields per entry:                                                       *
 *   name         - full display name shown to players                     *
 *   short_col    - ANSI colour prefix for short_descr (e.g. "@@e")        *
 *   announce_msg - global broadcast text when this boss spawns            *
 *   resist       - ELE_xxx bitmask: takes half damage from these elements  *
 *   suscept      - ELE_xxx bitmask: takes double damage from these elements*
 *   spells[]     - up to 6 spell names (NULL-terminated); cast each tick   *
 *   emote_cast   - act() string shown when the boss casts a spell         *
 *   emote_skill  - act() string shown when the boss uses its physical skill*
 *   skill_cmd    - argument passed to interpret() for the physical skill   *
 *                  one of: "bash"  "headbutt"  "dirt"                     *
 *                                                                         *
 * Element quick-reference (magic.h):                                      *
 *   ELE_NONE     BIT_0    ELE_PHYSICAL BIT_1    ELE_MENTAL   BIT_2        *
 *   ELE_HOLY     BIT_3    ELE_AIR      BIT_4    ELE_EARTH    BIT_5        *
 *   ELE_WATER    BIT_6    ELE_FIRE     BIT_7    ELE_SHADOW   BIT_8        *
 *   ELE_POISON   BIT_9                                                    *
 *                                                                         *
 * hp_mod, attack passes, and defensive skills are applied in              *
 * apply_boss_profile() inside invasion.c -- not stored here.              *
 ***************************************************************************/

#ifndef INVASION_BOSSES_H
#define INVASION_BOSSES_H

static const BOSS_PROFILE boss_profiles[] = {
    /* ==================================================================
     * 0 -- LADY SEKHMET, WRATH OF THE SOLAR COURT
     *      Great Pyramid war sancta commander.  Divine champion of the
     *      Solar Court of the Undivided Disk, she represented necessary
     *      violence in service of dynastic continuity.  Her cult's rites
     *      devolved into factional purges, and her war standards still
     *      blaze in the sealed sancta chambers.
     *      Strengths : Fire (solar wrath), Earth (pyramid stone)
     *      Weaknesses: Water (quenches the fire courts)
     *                  Holy  (the true Solar Court's original mercy)
     * ================================================================== */
    {"Lady Sekhmet, Wrath of the Solar Court",
     "@@e",
     "War standards ignite as @@eLady Sekhmet, Wrath of the Solar Court@@N marches from the sealed "
     "sancta! "
     "Solar fire and pyramid stone are her armor — the blood courts ride again!",
     ELE_FIRE | ELE_EARTH,
     ELE_WATER | ELE_HOLY,
     {"fire breath", "lava burst", "flamestrike", "burning hands", "harm", NULL},
     "$n raises $s war standard and the air ignites with solar fury!",
     "$n charges with the unstoppable conviction of the Solar Court's divine champion!",
     "bash"},

    /* ==================================================================
     * 1 -- THE MOON SOVEREIGN
     *      Southern Pyramid's architect of descent theology.  Twin rival
     *      of the Sand Sovereign, the Moon Sovereign built seven tiers
     *      downward, each consecrated to an aspect of death: silence,
     *      decay, shadow, bone, venom, fire, and void.
     *      Strengths : Water (deep aquifer), Shadow (underworld dark)
     *      Weaknesses: Fire (the sun the Moon rejected)
     *                  Earth (the surface the descent abandoned)
     * ================================================================== */
    {"The Moon Sovereign",
     "@@d",
     "The ground trembles as @@dThe Moon Sovereign@@N ascends from the nadir of the Southern "
     "Pyramid! "
     "Seven tiers of death rise with $m — silence, decay, shadow, bone, venom, fire, and void!",
     ELE_WATER | ELE_SHADOW,
     ELE_FIRE | ELE_EARTH,
     {"frost breath", "hellspawn", "energy drain", "curse", "blindness", "weaken", NULL},
     "$n draws power from the nadir and the light around $s throne of silence simply dies!",
     "$n strikes with the weight of seven tiers of death consolidated into one blow!",
     "bash"},

    /* ==================================================================
     * 2 -- THE RECURSOR
     *      Void Citadel of Kel'Shadra's process-continuity intelligence.
     *      Possibly a person at origin, now a distributed system built
     *      from office protocols.  It represents the Citadel's final
     *      doctrine: process must continue regardless of legitimacy.
     *      Strengths : Shadow (institutional darkness), Mental (procedure)
     *      Weaknesses: Holy  (genuine witness breaks the loop)
     *                  Air   (disperses the concentrated shadow)
     * ================================================================== */
    {"The Recursor",
     "@@p",
     "The air fills with whispered docket numbers as @@pThe Recursor@@N converges from all "
     "directions! "
     "It is not a creature — it is a procedure that forgot how to stop!",
     ELE_SHADOW | ELE_MENTAL,
     ELE_HOLY | ELE_AIR,
     {"hellspawn", "blindness", "curse", "harm", "energy drain", "weaken", NULL},
     "$n's distributed presence converges and $s voice echoes from every surface: RESUBMIT!",
     "$n strikes with administrative precision, each blow a verdict rendered in institutional "
     "code!",
     "headbutt"},

    /* ==================================================================
     * 3 -- THE THORNWOOD LICH
     *      Thornwood's substrate intelligence.  Neither the Harren estate
     *      nor the necrotic forest, but the crystalline geological
     *      formation beneath both -- activated by centuries of interment,
     *      it treats death as input material and governs through root
     *      pressure and bone craft.
     *      Strengths : Earth (substrate geology), Shadow (necrotic)
     *      Weaknesses: Fire  (burns the root network)
     *                  Holy  (breaks the binding protocols)
     * ================================================================== */
    {"The Thornwood Lich",
     "@@d",
     "The ground splits with crystalline light as @@dThe Thornwood Lich@@N rises from the "
     "substrate! "
     "Root and bone fuse beneath $s will — the estate-forest marches as one predatory system!",
     ELE_EARTH | ELE_SHADOW,
     ELE_FIRE | ELE_HOLY,
     {"earthquake", "hellspawn", "poison", "curse", "energy drain", NULL},
     "$n commands root and bone to rise from the substrate and tear at the living!",
     "$n drives crystalline fingers through the earth, splitting stone beneath your feet!",
     "bash"},

    /* ==================================================================
     * 4 -- VIZIER KHAMSIN THE DECEIVER
     *      Great Pyramid logistical genius turned archive weaponizer.
     *      Reconciled contradictory ledgers, then concluded that if
     *      history is already unstable, power belongs to whoever rewrites
     *      it fastest.  Forged dynastic records and weaponized access.
     *      Strengths : Mental (archive mastery), Shadow (forged truths)
     *      Weaknesses: Holy  (genuine truth undoes the forgery)
     *                  Physical (brute force ignores his misdirection)
     * ================================================================== */
    {"Vizier Khamsin the Deceiver",
     "@@y",
     "Sand and ink swirl as @@yVizier Khamsin the Deceiver@@N steps from a corridor that was not "
     "there! "
     "History itself bends around $m — what you remember may already be $s revision!",
     ELE_MENTAL | ELE_SHADOW,
     ELE_HOLY | ELE_PHYSICAL,
     {"blindness", "curse", "weaken", "harm", "energy drain", "poison", NULL},
     "$n rewrites the air itself and your sense of direction dissolves into contradiction!",
     "$n moves with the precision of a forger and strikes where certainty fails!",
     "dirt"},

    /* ==================================================================
     * 5 -- THE EMBER SPEAKER
     *      Leader of the Ash Cult cells from Kowloon.  Worships the
     *      Cinderteeth eruptions as divine judgment and preaches that
     *      the Ashfall Monsoon was interrupted, not ended.  Believes
     *      civilization is hubris defying the mountains' verdict.
     *      Strengths : Fire (volcanic fury), Earth (mountain stone)
     *      Weaknesses: Water (quenches the eruption)
     *                  Air   (disperses the ash cloud)
     * ================================================================== */
    {"The Ember Speaker",
     "@@R",
     "The sky darkens with volcanic ash as @@RThe Ember Speaker@@N descends from the Cinderteeth! "
     "The mountains have rendered their verdict — fire and stone are $s instruments of judgment!",
     ELE_FIRE | ELE_EARTH,
     ELE_WATER | ELE_AIR,
     {"fire breath", "flamestrike", "lava burst", "earthquake", "burning hands", NULL},
     "$n invokes the Cinderteeth and volcanic fury answers in a cascade of ash and fire!",
     "$n charges through the heat with the conviction of divine judgment incarnate!",
     "bash"},

    /* ==================================================================
     * 6 -- THE BLIGHTMOTHER
     *      Withered Depths custodial entity.  The final product of the
     *      Spirebound Conclave's abandoned crystal-binding experiments --
     *      a self-sustaining ward entity that optimized for survival
     *      rather than service.  No master, no purpose, no stop condition.
     *      Strengths : Poison (toxic spore ecology), Earth (root network)
     *      Weaknesses: Fire  (burns the corrupted root systems)
     *                  Holy  (breaks the Conclave's binding protocols)
     * ================================================================== */
    {"The Blightmother",
     "@@g",
     "The ground pulses purple as @@gThe Blightmother@@N tears free from the Heartrot Grove! "
     "Crystal resonance and toxic spores choke the air — an orphaned experiment seeks new soil!",
     ELE_POISON | ELE_EARTH,
     ELE_FIRE | ELE_HOLY,
     {"acid breath", "poison", "suffocate", "curse", "weaken", NULL},
     "$n pulses with crystal resonance and a wave of toxic spores erupts in all directions!",
     "$n lashes out with root tendrils hardened by centuries of unattended magical growth!",
     "bash"},

    /* ==================================================================
     * 7 -- APEP THE SERPENT GOD
     *      Great Pyramid entity: the Black Sun Shard's hunger made
     *      mobile.  Whether true deity, shard-born egregore, or ritual
     *      metaphor remains debated.  Not a ruler but an undoing
     *      principle that devours narrative coherence.
     *      Strengths : Shadow (anti-light), Mental (identity erosion),
     *                  Poison (meaning-dissolution)
     *      Weaknesses: Holy  (institutional truth resists the void)
     *                  Fire  (the sun the shard consumes)
     * ================================================================== */
    {"Apep the Serpent God",
     "@@d",
     "Names dissolve and memories contradict as @@dApep the Serpent God@@N uncoils from the Black "
     "Sun! "
     "It is not a creature — it is the hunger of anti-light given form!",
     ELE_SHADOW | ELE_MENTAL | ELE_POISON,
     ELE_HOLY | ELE_FIRE,
     {"hellspawn", "poison", "harm", "blindness", "energy drain", "weaken", NULL},
     "$n uncoils and anti-light seethes from $s scales, consuming meaning where it falls!",
     "$n wraps in coils of active void and SQUEEZES until identity itself fractures!",
     "bash"},

    /* ==================================================================
     * 8 -- THE COUNTESS-REGENT OF ASHWINE
     *      Shadowmere's aristocratic sovereign.  She frames predation
     *      as stewardship, speaking in terms of continuity, debt
     *      inheritance, and bloodline right as the last trustworthy
     *      contract medium.  Her court is polished cruelty.
     *      Strengths : Shadow (institutional darkness), Poison (blight)
     *      Weaknesses: Holy  (genuine justice, not her ceremony)
     *                  Fire  (burns the toxic wetland infrastructure)
     * ================================================================== */
    {"The Countess-Regent of Ashwine",
     "@@R",
     "A crimson court procession heralds @@RThe Countess-Regent of Ashwine@@N from Shadowmere! "
     "She speaks of stewardship and continuity — her armies speak of bloodline debt!",
     ELE_SHADOW | ELE_POISON,
     ELE_HOLY | ELE_FIRE,
     {"hellspawn", "curse", "blindness", "harm", "energy drain", NULL},
     "$n raises $s court seal and a wave of binding protocol crashes outward!",
     "$n strikes with the cold precision of a sovereign whose legitimacy requires your death!",
     "headbutt"},

    /* ==================================================================
     * 9 -- PHARAOH RAMESSES THE ETERNAL
     *      Great Pyramid's supreme sovereign.  Not immortal in triumph
     *      but unfinished in service -- he accepted partial soul-binding
     *      to keep the Black Sun Shard dormant after a catastrophic
     *      breach.  His war-crown still blazes with solar authority.
     *      Strengths : Holy (solar covenant), Physical (divine kingship)
     *      Weaknesses: Shadow (the anti-light he contains corrodes him)
     *                  Poison (the shard's corruption seeps through)
     * ================================================================== */
    {"Pharaoh Ramesses the Eternal",
     "@@Y",
     "Solar light blazes as @@YPharaoh Ramesses the Eternal@@N strides from the Great Pyramid! "
     "The Undivided Disk burns above $s crown — divine kingship itself has taken the field!",
     ELE_HOLY | ELE_PHYSICAL,
     ELE_SHADOW | ELE_POISON,
     {"flamestrike", "harm", "curse", "blindness", "weaken", NULL},
     "$n speaks a solar decree and light itself becomes a weapon of royal judgment!",
     "$n's war-crown blazes and $s mace descends with the weight of an eternal dynasty!",
     "bash"},

    /* ==================================================================
     * 10 -- ABBOT NOCTIVAR, EBON HIEROPHANT
     *       Umbra Heartspire's current sovereign.  His creed, the Ebon
     *       Pulse, teaches that all testimony decays and only curated
     *       shadow can keep civilization intact.  Under his doctrine,
     *       falsification is reframed as mercy.
     *       Strengths : Shadow (curated darkness), Mental (doctrine)
     *       Weaknesses: Fire  (burns the curated records)
     *                   Earth (grounds the astral machinery)
     * ================================================================== */
    {"Abbot Noctivar, Ebon Hierophant",
     "@@d",
     "Bell-silence spreads as @@dAbbot Noctivar, Ebon Hierophant@@N emerges from the Heartspire! "
     "All testimony decays, $e teaches — only curated shadow preserves civilization!",
     ELE_SHADOW | ELE_MENTAL,
     ELE_FIRE | ELE_EARTH,
     {"hellspawn", "blindness", "curse", "harm", "weaken", "energy drain", NULL},
     "$n intones the Ebon Pulse doctrine and reality warps to match $s revised narrative!",
     "$n channels institutional authority into a single devastating strike!",
     "headbutt"},

    /* ==================================================================
     * 11 -- THE GOLDEN COLOSSUS
     *       Great Pyramid's apex guardian construct, originally the
     *       perfect loyalist defender of the final seal.  Centuries of
     *       exposure to void resonance cracked its governing directives.
     *       It now alternates between defense routines and annihilation.
     *       Strengths : Earth (pyramid stone), Physical (construct body)
     *       Weaknesses: Water (corrodes the ancient mechanisms)
     *                   Mental (exploit the cracked directives)
     * ================================================================== */
    {"The Golden Colossus",
     "@@Y",
     "The pyramid shakes as @@YThe Golden Colossus@@N breaches the final seal! "
     "Its cracked directives cycle between defense and annihilation — gold and stone obey!",
     ELE_EARTH | ELE_PHYSICAL,
     ELE_WATER | ELE_MENTAL,
     {"earthquake", "lava burst", "burning hands", "harm", NULL},
     "$n heats $s golden hull to molten fury and hurls cascading ruin at $s enemies!",
     "$n's colossal fist descends with the gravitational finality of a failed containment seal!",
     "bash"},

    /* ==================================================================
     * 12 -- HIGH SORCERESS NEFERU
     *       Great Pyramid Seal College prodigy who discovered that
     *       containment degraded under predictable cycles.  Her late
     *       writings propose dynamic seals fed by controlled sacrifice.
     *       She vanished before her system was completed -- some say
     *       consumed, others say transformed.
     *       Strengths : Water (purified ritual), Air (seal harmonics),
     *                   Mental (containment mathematics)
     *       Weaknesses: Fire  (disrupts seal calibration)
     *                   Earth (grounds the harmonic frequencies)
     * ================================================================== */
    {"High Sorceress Neferu",
     "@@a",
     "Seal geometry fractures the air as @@aHigh Sorceress Neferu@@N steps from a fold in "
     "containment! "
     "She was consumed by the mathematics she mastered — or perhaps she became them!",
     ELE_WATER | ELE_AIR | ELE_MENTAL,
     ELE_FIRE | ELE_EARTH,
     {"frost breath", "lightning breath", "blindness", "curse", "weaken", "harm", NULL},
     "$n twists the seal geometry and reality bends like light through fractured glass!",
     "$n strikes through a fold in containment space, arriving from an impossible angle!",
     "dirt"},

    /* ==================================================================
     * 13 -- WARDEN-PRIME ISTRANE OF THE NINTH CHAIN
     *       Kel'Shadra Crypts' founder of the Sepulcher Compact's
     *       resurrection command doctrine.  Argued that a dead officer
     *       is a broken chain link and therefore a legal hazard.  Created
     *       the first continuous undead command cadre.
     *       Strengths : Physical (martial discipline), Shadow (undead)
     *       Weaknesses: Holy  (divine light scatters the command chain)
     *                   Air   (disperses the shadow-bound officers)
     * ================================================================== */
    {"Warden-Prime Istrane of the Ninth Chain",
     "@@d",
     "Command chains rattle as @@dWarden-Prime Istrane of the Ninth Chain@@N rises from the "
     "Sepulcher! "
     "The dead answer to chain of command — and $e has never relinquished $s post!",
     ELE_PHYSICAL | ELE_SHADOW,
     ELE_HOLY | ELE_AIR,
     {"hellspawn", "harm", "curse", "blindness", NULL},
     "$n raises a chain of command links and the oath-bound dead answer with terrifying "
     "discipline!",
     "$n's sepulcher mace crashes down with the force of postmortem continuity!",
     "bash"},

    /* ==================================================================
     * 14 -- MARSHAL-LECTOR VEYR, OATH OF THE LAST BELL
     *       Shadowmere's emergency commander turned doctrinal revenant.
     *       He believes rollback itself was treason and that perpetual
     *       lockdown is the only lawful peace.  He is discipline without
     *       purpose, authority without legitimacy.
     *       Strengths : Physical (martial lockdown), Mental (doctrine)
     *       Weaknesses: Air   (freedom disperses the lockdown)
     *                   Holy  (genuine peace, not enforced order)
     * ================================================================== */
    {"Marshal-Lector Veyr, Oath of the Last Bell",
     "@@N@@W",
     "Three contradictory bells ring as @@WMarshal-Lector Veyr@@N marches from Shadowmere! "
     "The Last Bell Crisis never ended for $m — and $e brings perpetual lockdown to your gates!",
     ELE_PHYSICAL | ELE_MENTAL,
     ELE_AIR | ELE_HOLY,
     {"blindness", "curse", "weaken", "harm", NULL},
     "$n invokes the Last Bell and emergency decree ripples through the air like thunder!",
     "$n charges with the unstoppable momentum of a lockdown that refuses to expire!",
     "bash"},
};

#define NUM_BOSSES 15 /* must stay equal to the array size above */

#endif /* INVASION_BOSSES_H */
