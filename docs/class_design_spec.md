# Class Design Specification

This document defines the intended design, role, and mechanical identity of every class across all three tiers: Mortal, Remort, and Adept.

## Tier Overview

- **Mortal** (6 classes): Base classes available at character creation.
- **Remort** (12 classes): Advanced classes, each requiring one mortal prerequisite.
- **Adept** (6 classes): Apex classes, each requiring two remort prerequisites. All adept classes gain +500 damcap.

---

## Mortal Classes

### Warden (War)

Wardens are a physically offensive class. Their specialized skill is **enhanced damage**, which increases physical damage output. They receive a damage-increasing stance (**Warden**) at level 60. They receive **parry** and **shield block** as base skills.

- Prime: STR
- Stance (level 60): Warden (DR+1)
- Key skills: enhanced damage, parry, shield block, bash, berserk, cleave, rend, fortify

### Cleric (Cle)

Clerics are intended to heal, cure, purify, and be the general party support. They receive a stance to enhance healing (**Healer**) at level 60.

- Prime: WIS
- Stance (level 60): Healer (Heal+1)
- Key spells: heal, cure blindness, cure poison, bless, sanctuary, restoration, holy word, divine strike

### Cipher (Cip)

Ciphers are a physically offensive class focused on crits. Their specialized skill is **enhanced critical**, enhancing both physical crit frequency and crit damage. They also receive a 1% chance to CRACK on backstab or circle (doubling damage). They receive **parry** and **dodge**.

- Prime: DEX
- Stance (level 60): Cipher (AC-1, HR+1)
- Key skills: enhanced critical, backstab, circle, dodge, parry, sneak, hide, shadow step, garrote

### Magi (Mag)

Magi are a magically offensive class focused on base damage and a large selection of elements. They receive many direct damage spells across various elements. They also receive **mystical potency**, which increases spell damage (analogous to enhanced damage for physical). They receive a HoT (healing over time) spell (**regen**). They receive **fly** to deal with difficult terrain, and **pass door** to deal with doors. They also receive **dispel magic** to help with both objects and enemies, as well as **portal**. They receive a spell damage stance (**Magi**) at level 60.

- Prime: INT
- Stance (level 60): Magi (AC+5, DR-3, HR-3, Spell+2, MULTI_CAST, NO_HIT)
- Key spells: fireball, lightning bolt, acid blast, chain lightning, fly, pass door, dispel magic, portal, regen
- Key skills: mystical potency

### Pugilist (Pug)

Pugilists are a physically defensive class. They receive **martial arts** and **counter** as their specialized skills, and prefer not to wield weapons. They receive **shield block** and **dodge** as base skills. They receive a defensive stance (**Pugilist**) at level 60.

- Prime: CON
- Stance (level 60): Pugilist (Speed+1)
- Key skills: martial arts, counter, shield block, dodge, iron skin, roll with blow, pressure point, conditioning

### Psionicist (Psi)

Mind spells. Control over the mind. Control over the body. Psionicists focus on either offensive mental spells or on control over their own mind and body. They receive self-target cure poison (**purify**), self-target heals (**psionic recovery**), and self-target buffs. They receive a spell critical stance (**Psionicist**) at level 60.

- Prime: INT
- Stance (level 60): Psionicist (AC+5, DR-3, HR-3, Spell+2, MULTI_CAST, NO_HIT)
- Key spells: mind flail, psychic crush, ego whip, mind bolt, mindflame, purify, psionic recovery, barrier, mindwall, precognition, fighting trance
- Key skills: spell critical, spell critical damage

---

## Remort Classes

### Sorcerer (Sor)

**Prerequisite:** Magi

Sorcerers build on Magi and get elemental spells that cause debuffs. Having certain debuffs on a mob while casting certain spells will cause varying effects.

- Prime: INT
- Stance: Sorcerer (AC+2, DR-3, HR-3, Heal-2, Spell+5, DUAL_CAST, NO_HIT)
- Key spells/skills: ice bolt, fire blast, lava burst, cone of cold, shock storm, earth shatter, fireshield, iceshield, shockshield, mystical focus

### Wizard (Wiz)

**Prerequisite:** Magi

Wizards focus around charmies and elemental DoTs as well as HoTs. Wizard and Sorcerer can potentially combine their abilities for interesting effects.

- Prime: INT
- Stance: Wizard (AC+2, DR-3, HR-3, Heal-2, Spell+5, DUAL_CAST, NO_HIT)
- Key spells/skills: iron golem, diamond golem, creature bond, arc lightning, terra rend, tidal lash, phoenix flare, mana drain, room dispel, mystical focus

### Paladin (Pal)

**Prerequisite:** Cleric

Paladins are a hybrid between Priest and Knight. Instead of having to go knight/priest, they offer a slightly less potent option in each respective area while allowing players to choose a different class. Paladins receive 75% of Priest's bonus to all healing abilities (based on level) and an increase in damage to all physical abilities, unique among the remorts.

- Prime: WIS
- Stance: Paladin (DR+3, HR+3, Speed+1, Heal+2)
- Key spells/skills: group heal, healing light, holy armor, holy light, holy avenger, holy wrath, holystrike, leadership, morale, charge, divine intervention

### Priest (Pri)

**Prerequisite:** Cleric

Priests heal. They heal really, really well. You want a healer? Get a priest.

- Prime: WIS
- Stance: Priest (AC-3, Heal+3)
- Key spells/skills: healing light, divine intervention, group heal, holy armor, holy light, holy avenger, holy wrath, holyshield, frenzy

### Assassin (Ass)

**Prerequisite:** Cipher

Assassins focus around melee combat and poisons. BIG damage backstabs and circles.

- Prime: DEX
- Stances: Assassin (AC-3, HR+3, Speed+1), Ninja (Speed+2, NINJA)
- Key skills: backstab, circle, dualwield, poison weapon, poison:quinine, poison:arsenic, throwing star, cripple, disguise

### Warlock (Wlk)

**Prerequisite:** Cipher

Warlocks are a hybrid offensive caster/melee. They receive 75% of the bonuses of Sorcerer and 75% of the bonuses of Assassin.

- Prime: DEX
- Stance: Warlock (AC-2, HR+2, Spell+2)
- Key skills: backstab, circle, dualwield, poison weapon, poison:quinine, poison:arsenic, throwing needle, throwing star, cripple, disguise

### Knight (Kni)

**Prerequisite:** Warden

The Knight is the natural evolution of the Warden. They receive large bonuses to combo damage.

- Prime: STR
- Stance: Knight (AC-3, DR+2, HR+2, Speed+1)
- Key skills: two handed, morale, leadership, unit tactics, charge, holystrike, shieldblock

### Swordsman (Swo)

**Prerequisite:** Warden

Swordsmen focus on swords. All of their bonuses focus around swords. All of their attacks focus on swords. Swords are the beginning and the end for Swordsmen. They also receive large bonuses (equivalent to Knight) to combo damage. They receive a combo finisher, **Riposte**, which neutralizes an attack and counters. They receive a **fleche** combo attack.

- Prime: STR
- Stance: Swordsman (AC-3, DR+2, HR+2, Speed+1)
- Key skills: enhanced sword damage, enhanced sword critical, weapon bond, riposte, fleche, two handed, frenzy, unit tactics, charge

### Necromancer (Nec)

**Prerequisite:** Psionicist

Necromancers deal in darkness, death, damage over time, and resurrected monsters.

- Prime: INT
- Stance: Necromancer (AC+2, DR-3, HR-3, Spell+5, DUAL_CAST, NO_HIT)
- Key spells/skills: skeleton, revenant, soul net, condense soul, infuse, soul thief, wraith touch, black curse, spirit curse, rictus curse, corrupt bond, mana drain

### Egomancer (Ego)

**Prerequisite:** Psionicist

An Egomancer is the natural culmination of the Psionicist. Everything a Psi is, just taken to the next level. Egomancer receives **enhanced purify**, which removes all normal poisons, blinds, and curses.

- Prime: INT
- Stance: Egomancer (AC+2, DR-3, HR-3, Spell+5, DUAL_CAST, NO_HIT)
- Key spells/skills: enhanced purify, thought devourer, thought vise, sonic blast, overdrive, magical supremacy, psishield, mystical focus

### Monk (Mon)

**Prerequisite:** Pugilist

A Monk is everything a Pugilist is, but taken to the next level. They gain **Bare Hands** that only gets a bonus if you have nothing equipped in your hands, taking Martial Arts to the next level. They increase autoattack damage significantly.

- Prime: CON
- Stance: Monk (DR+3, HR+3, Speed+2)
- Key skills: bare hand, equip fist, dual fist, aurabolt, chiblock, phantomfist, chakra

### Brawler (Bra)

**Prerequisite:** Pugilist

A Brawler is a Pugilist that gains combo skills. Gains 75% of Monk's autoattack damage as well, and acquires all the same passives as Monk.

- Prime: CON
- Stance: Brawler (DR+3, HR+3, Speed+2)
- Key skills: bare hand, equip fist, dual fist, palmstrike, spinfist, phantomfist, aurabolt, chiblock

---

## Adept Classes

All adept classes gain +500 damcap.

### Nightblade (Nig)

**Prerequisites:** Assassin + Warlock

Increases Backstab and Circle's damage. Gains a double backstab (STANCE_DUAL_BACKSTAB). Enhances critical. Adds a new poison (Nightshade). Adds an extra attack. Has a new stance, **Nightblade**.

- Prime: DEX
- Stance: Nightblade (AC-3, DR+5, HR+5, Speed+3, Spell+3, DUAL_BACKSTAB)
- Key additions: poison:nightshade, backstab/circle damage scaling, crit bonuses, +1 extra attack

### Templar (Tem)

**Prerequisites:** Paladin + Priest

Increases healing done. Adds cure poison, cure blind, remove curse to group heal and heal (Enhanced Heal). Has a new AoE holy cast. Increases damage from physical damage and holy spells. Adds an extra attack. Has a new stance, **Templar**.

- Prime: WIS
- Stance: Templar (AC-2, DR+3, HR+3, Speed+3, Heal+5)
- Key additions: enhanced heal (integrates status cures into healing), physical/holy damage scaling, +1 extra attack

### Martial Artist (Mar)

**Prerequisites:** Monk + Brawler

Increases autoattack damage. Adds an extra attack. Has a new combo skill. Has a new stance, **Martial**.

- Prime: CON
- Stance: Martial (AC-1, DR+5, HR+5, Speed+5)
- Key additions: chi pool expansion (20 to 25), chi-technique damage scaling, bare-hand strike damage scaling, +1 extra attack

### Crusader (Cru)

**Prerequisites:** Knight + Swordsman

Increases combo damage. Adds an extra attack. Has a new combo skill. Has a new stance, **Crusader**.

- Prime: STR
- Stance: Crusader (AC-3, DR+3, HR+3, Speed+3)
- Key additions: anti magic shell, combo skill damage scaling, +1 extra attack

### Grand Magi (Gma)

**Prerequisites:** Sorcerer + Wizard

Increases spell damage and damcap. Will have new spells. Has an existing stance, **Magi**.

- Prime: INT
- Stance: Magi (AC+2, DR-1, HR-1, Spell+3, EITHER_CLASS, MULTI_CAST, DUAL_CAST, NO_HIT)
- Key additions: elemental inferno, elemental shock, elemental deluge, elemental rupture, non-physical spell damage scaling

### Kinetimancer (Kin)

**Prerequisites:** Necromancer + Egomancer

Increases spell damage and damcap. Will have new spells. Has an existing stance, **Magi** (shared with Grand Magi via EITHER_CLASS).

- Prime: INT
- Stance: Magi (AC+2, DR-1, HR-1, Spell+3, EITHER_CLASS, MULTI_CAST, DUAL_CAST, NO_HIT)
- Key additions: kinetic reversion, non-physical spell damage scaling, shadow/poison/mental damage bonus
