# Proposal: Public Societies System

## Overview

Public societies are open, well-known civic institutions that players can join.
Where secret societies operate in shadow with hidden membership and deniable
operations, public societies are the visible pillars of city life — military
commands, trade guilds, religious orders, and civic services that everyone knows
about and that openly recruit members.

A player may belong to one clan, one public society, **and** one secret society
simultaneously. The three systems are independent layers of affiliation:

| System | Visibility | Discovery | Risk | Progression |
|---|---|---|---|---|
| **Clan** | Public | Player-created, invitation | War system | Political rank |
| **Public Society** | Public | Visit a hall, meet requirements | None | Institutional rank |
| **Secret Society** | Hidden | Passphrase, lore clues | Exposure, disavowal | Covert rank |

Public societies occupy the space between clans (player-run political entities)
and secret societies (covert NPC-run organizations). They are NPC-run
institutions with structured advancement, civic skills, and task-board missions
— the professional guilds and service orders that keep the five cities
functioning.

---

## Design Goals

### 1. Open Identity

A player's public society membership is visible in `who`, `score`, `finger`,
and `whois`. There is no secrecy — these are legitimate institutions. The
player's rank title appears alongside their name, communicating their standing
to everyone.

### 2. Lore Integration

Every public society is an institution already documented in the city lore
files. The Guard Command, the Harbor Syndics, the Ember Wardens, the Shrine of
the First Claw, the Wall Command, and the Road Wardens all exist in the world
already. This proposal makes them player-joinable.

### 3. Civic Gameplay

Each society offers a gameplay loop centered on its institutional function:
military patrols, trade contracts, disaster response, justice enforcement,
frontier defense, or inter-city logistics. These are straightforward,
legitimate activities — no smuggling, no sabotage, no exposure risk.

### 4. Utility Skills

Public society skills provide practical, non-combat utility: field medicine,
equipment repair, navigation, appraisal, fortification, and logistics. Like
secret society skills, they do not modify `hitroll`, `damroll`, `armor`,
`max_hit`, `max_mana`, or saving throws. They grant capabilities, not power.

### 5. Horizontal Progression

Rank in a public society grants access to better missions, higher-tier skills,
institutional services (discounts at society-affiliated shops, access to
restricted areas), and civic authority (the ability to deputize, arbitrate, or
inspect). It does not grant combat advantages.

### 6. Cooperative Structure

Public societies do not have built-in rivalries or PvP mechanics. They are
civic institutions that cooperate (or at least coexist) within the five-city
network. Inter-society interaction is diplomatic, not adversarial — a Guard
Command officer and an Ember Warden work together during a crisis, they do not
fight each other.

---

## The Six Public Societies

| Society | Base City | Lore Origin | Gameplay Loop |
|---|---|---|---|
| **The Guard Command** | Midgaard | Midgaard's primary military institution | Combat patrols, city defense, beast hunting |
| **The Harbor Syndics** | Kowloon | Kowloon's maritime commerce authority | Trade contracts, shipping, cargo management |
| **The Ember Wardens** | Rakuen | Rakuen's disaster response corps | Emergency response, botanical gathering, infrastructure repair |
| **The Shrine of the First Claw** | Mafdet | Mafdet's justice and oath-enforcement order | Bounty hunting, dispute arbitration, oath contracts |
| **The Wall Command** | Kiess | Kiess's frontier defense force | Frontier patrols, escort duty, wilderness security |
| **The Road Wardens** | Intercity | Chartered trade-route protection companies | Caravan escort, route maintenance, wilderness exploration |

Each society is detailed in its own file.

---

## Shared Mechanics

### Joining

Public society recruitment is open. A player visits the society's hall, speaks
to the enrollment officer NPC, and joins if they meet the prerequisites. No
passphrase, no initiation mission, no hidden discovery — the hall is a known
location, the officer is clearly identified, and the requirements are stated
plainly.

```
enlist <society>
```

The `enlist` command is available in any room containing a public society
enrollment officer NPC. The officer checks prerequisites and either enrolls the
player or explains what is missing.

### Prerequisites

All public societies share a base requirement:

- Must be a player (not NPC)
- Must not already be in a public society
- Must meet the society-specific level requirement
- Must not have left a public society within the last 7 real days (shorter
  cooldown than secret societies, reflecting the lower commitment)

### Rank Structure

All public societies use a shared 6-rank structure (0-5), though titles differ
per society:

| Rank | General Label | Description |
|---|---|---|
| 0 | Recruit | New member; basic missions and skills |
| 1 | Member | Full access to society hall and services |
| 2 | Veteran | Access to advanced missions and mid-tier skills |
| 3 | Officer | Can mentor recruits; access to institutional intelligence |
| 4 | Senior Officer | Can enroll new members; access to high-tier missions |
| 5 | Commander | Inner circle; institutional authority; top-tier skills |

### Advancement

Promotion requires completed tasks and reputation, similar to secret societies
but with no exposure checks or special rank missions:

| Rank | Tasks Required | Reputation Required |
|---|---|---|
| 0 -> 1 | 5 completed | 50+ |
| 1 -> 2 | 12 completed | 125+ |
| 2 -> 3 | 25 completed | 225+ |
| 3 -> 4 | 40 completed | 350+ |
| 4 -> 5 | 60 completed | 450+ |

Promotion is requested at the society's commanding officer NPC:

```
society promote
```

### Reputation

Public society reputation uses the same -500 to +500 range as secret
societies, but with different tier labels reflecting institutional standing
rather than trust/suspicion:

| Range | Label | Effect |
|---|---|---|
| -500 to -300 | Dishonorably Discharged | Expelled; barred from rejoining for 30 days |
| -299 to -100 | Disciplined | Restricted mission access; under review |
| -99 to 99 | Standing | Standard access |
| 100 to 299 | Commended | Access to priority missions |
| 300 to 499 | Decorated | Access to command missions; institutional privileges |
| 500 | Exemplary | Maximum standing; society leadership tier |

### Leaving

A player can leave their public society at any time:

```
society resign
```

This resets all public society fields and imposes a 7-day cooldown before
joining another. Voluntary departure has no reputation penalty — it is an
honorable discharge. Expulsion (reputation below -300) imposes a 30-day
cooldown.

### Visibility

Public society membership is displayed in:

- `who` output: society abbreviation and rank title after the player's name
- `score`: society name, rank, and reputation
- `finger`/`whois`: society membership line

This is the opposite of secret societies, where membership is hidden from all
public commands.

---

## Relationship to Other Systems

| System | Interaction |
|---|---|
| **Clans** | Independent membership; society tasks never target clan resources |
| **Secret Societies** | Independent membership; a player can be in both |
| **AI Dialogue** | Society NPCs recognize members and address them by rank |
| **Quest System** | Society tasks use `QUEST_DATA` with `QUEST_TYPE_PUBLIC_SOCIETY` |
| **Skills** | Public society skills stored in `learned[]` at reserved indices |
| **Save System** | Public society data persisted in player file |

---

## Scope

This proposal covers:

- Definitions of six public societies with lore grounding
- Shared membership, rank, and reputation mechanics
- Public society skills (non-combat utility)
- Task-board mission system
- Data structure additions
- Integration with existing systems

This proposal does **not** cover:

- Area file definitions for society halls (separate area-building task)
- AI dialogue persona text for society NPCs (separate AI content task)
- Specific task content beyond examples (separate content-authoring task)
