# Proposal: The Druid Class Lineage

## Overview

The Druid is a 7th mortal class representing the oldest magical tradition in the
world — the pre-civic practitioners who learned to channel power through the
living substrate (forests, stone, water, root networks) before cities formalized
magic into academic doctrine or religious infrastructure. Unlike every other
caster in the game, **Druids cast spells using their own HP** rather than mana.
Their life force is the fuel.

The Druid lineage introduces a unique class mechanic: **Overgrowth**. Each spell
cast increases the Druid's Overgrowth counter, which amplifies subsequent spell
power but also increases HP cost. The Druid grows more dangerous the longer a
fight continues — but risks killing themselves if they push too far. Overgrowth
decays naturally out of combat, resetting the cycle.

This creates a fundamentally different caster experience. Mana casters manage a
depletable pool and rest to refill it. Druids manage a risk curve: every spell
is a bet that the increased power is worth the escalating price. Healing a Druid
mid-combat directly enables more casting — making Druid + healer a powerful but
codependent pairing.

### Lineage Structure

| Tier | Class | Who | Prime | HP Gain | Mana Gain | Prerequisites |
|------|-------|-----|-------|---------|-----------|---------------|
| Mortal | Druid | Dru | CON | 5 | 0 | — |
| Remort | Thornwarden | Tho | CON | 10 | 0 | Druid |
| Remort | Wildspeaker | Wil | CON | 8 | 0 | Druid |
| Adept | Hierophant | Hie | CON | 16 | 0 | Thornwarden + Wildspeaker |

All four classes have **zero mana gain**. They never use mana. Their HP gain is
higher than standard casters to compensate for spending HP to cast — a Druid
needs a large health pool the way a Magi needs a large mana pool.

### Lore Foundation

The druidic tradition predates the Wizard's Keep, the Solar Court, and the
Violet Compact. The Everkeepers tended ironpine groves in the Great Northern
Forest. The Mire-Speakers healed with fenland plants. The Cairn-Keepers
communed with ancestors through megaliths. The Root Covenant in Thornwood
understood the substrate as alive — blood opens channels, roots carry
confession.

These practitioners never institutionalized. They had no academy, no registry,
no Triple-Ledger Rule. Their power came from the living world itself, channeled
through their own bodies at personal cost. The Wizard's Keep considers them
dangerously unregulated. The Solar Court's inheritors consider them theologically
incoherent. The druids consider both traditions to be newcomers borrowing from a
world they don't listen to.

---

## The Overgrowth Mechanic

### Concept

Overgrowth is the Druid's signature class resource — a counter that tracks how
deeply the Druid has channeled the living substrate in the current engagement.
Every Druid spell increases Overgrowth. Higher Overgrowth amplifies spell power
but escalates HP cost. The mechanic models the lore's central idea: the
substrate responds to those who draw on it, feeding back more power than
requested, demanding more of the body in return.

The name comes from the Root Covenant theology: "the roots do not stop growing
when you stop asking." A Druid who channels heavily enters a state where the
substrate's power flows through them whether they want it to or not — their
spells hit harder, their body pays more, and the decision to stop casting
becomes as important as the decision to start.

### Data Structure

A new field on `CHAR_DATA`:

```c
sh_int overgrowth;   /* 0-100, current Overgrowth level */
```

Overgrowth is **not saved** to the player file. It resets to 0 on login, death,
and when the Druid has been out of combat for a sufficient decay period. It is
a purely combat-session resource.

### How Overgrowth Works

**On every Druid spell cast:**

1. Calculate base HP cost from the spell's `min_mana` field (reinterpreted as
   `min_hp_cost` for Druid spells)
2. Apply the Overgrowth cost multiplier: `actual_cost = base_cost * (100 + overgrowth * 2) / 100`
3. Apply the Overgrowth power multiplier: `actual_power = base_power * (100 + overgrowth * 3) / 100`
4. Deduct `actual_cost` from `ch->hit`
5. Increase `overgrowth` by the spell's growth value (typically 3-8 per cast)

**Example progression (base cost 30 HP, growth value 5):**

| Cast # | Overgrowth | Cost Mult | Power Mult | HP Cost | Power |
|--------|------------|-----------|------------|---------|-------|
| 1 | 0 | 1.00x | 1.00x | 30 | 100% |
| 2 | 5 | 1.10x | 1.15x | 33 | 115% |
| 3 | 10 | 1.20x | 1.30x | 36 | 130% |
| 4 | 15 | 1.30x | 1.45x | 39 | 145% |
| 5 | 20 | 1.40x | 1.60x | 42 | 160% |
| 8 | 35 | 1.70x | 2.05x | 51 | 205% |
| 12 | 55 | 2.10x | 2.65x | 63 | 265% |
| 16 | 75 | 2.50x | 3.25x | 75 | 325% |
| 20 | 95 | 2.90x | 3.85x | 87 | 385% |

Power scales faster than cost (3% per point vs 2% per point), rewarding Druids
who push deeper into Overgrowth — but the absolute HP drain becomes lethal.
A Druid at Overgrowth 95 is casting at nearly 4x power but paying nearly 3x
the HP. One more spell could kill them.

### Overgrowth Decay

Overgrowth decays when the Druid is **not fighting and not casting**:

- Decay rate: 1 point per tick (approximately 30 seconds)
- Decay starts after 2 ticks of no combat and no casting
- Resting or sleeping doubles the decay rate (2 points per tick)
- Overgrowth does not decay while fighting or while any combat cooldown
  (WAIT_STATE) is active

This means a Druid who finishes a fight at Overgrowth 40 needs roughly 20 ticks
(~10 minutes) to fully reset while resting. They can engage the next fight
before fully decaying, carrying residual Overgrowth — starting stronger but
already paying elevated costs.

### Overgrowth Cap and Death Protection

- **Hard cap**: Overgrowth cannot exceed 100
- **Death protection**: If a spell's HP cost would reduce the Druid below 1 HP,
  the spell **still fires** but the Druid is reduced to 1 HP and Overgrowth is
  immediately set to 100. The Druid receives a warning message:

```
@@dThe roots tear through you — your body can give no more.@@N
```

This means a Druid cannot directly kill themselves by casting (unlike, say,
a player who walks into a death trap). But at 1 HP and Overgrowth 100, any
subsequent damage — including the next spell cast — will likely kill them.
The mechanic creates a dramatic "one last spell" moment rather than an
anticlimactic self-death.

### Overgrowth and Healing

Because Druids spend HP to cast, incoming healing directly enables more casting.
This is intentional — it creates the Druid + Priest/Cleric synergy described in
the overview. However, healing does **not** reduce Overgrowth. The power/cost
escalation continues regardless of how much HP the Druid has. A fully healed
Druid at Overgrowth 80 still pays 2.6x costs — they just have more HP to spend.

### Overgrowth Display

The `score` command shows current Overgrowth when non-zero:

```
Overgrowth: [||||||||||||--------] 60/100
```

A prompt token `%o` displays current Overgrowth. At 0, it displays nothing
(clean prompt for non-Druids and out-of-combat Druids).

### Passive Keystone: Substrate Piercing

Each mortal class has a passive keystone unlocked through reincarnation:

| Mortal | Keystone | Effect |
|--------|----------|--------|
| Warden | Enhanced Damage | Flat melee damage bonus |
| Cipher | Enhanced Critical | Better crit chance |
| Pugilist | Counter | Counterattack on melee hit |
| Magi | Potency | +INT*2% spell damage |
| Psionicist | Spell Critical | Spell crit chance |
| Cleric | Spell Critical Damage | Spell crit multiplier |
| **Druid** | **Substrate Piercing** | **Flat % reduction to target's spell resistance/saves** |

Substrate Piercing reduces the target's effective spell resistance against the
caster's spells. This is universally valuable — every class that casts benefits
from targets resisting less. A Grand Magi with Substrate Piercing lands more
damage through saves. A Priest with Substrate Piercing lands debuffs more
reliably. A Nightblade with Substrate Piercing gets hexes through resistance
that would otherwise block them.

**Thematic basis:** The Druid's magic doesn't travel through formal arcane
channels or divine invocation — it comes through the substrate itself. The
ground beneath the target's feet, the air in their lungs, the water in their
body. You can ward against a fireball. You can't ward against the earth
deciding you don't belong. A character who has trained as a Druid carries this
understanding into every subsequent class: their spells find paths through
resistance that purely institutional casters miss.

**Mechanical effect:** -10% to target's effective spell saves/resistance when
the caster has Substrate Piercing. Applied before the save roll, multiplicative
with existing resistance. Does not stack with itself.

**Implementation:** `gsn_substrate_piercing` is added to the `reinc_rules`
table in `skills.c`:

```c
{&gsn_substrate_piercing, CLASS_DRU, -1, 1},
```

Applied in `spell_save()` or equivalent resistance check:

```c
if (can_use_skill(ch, gsn_substrate_piercing))
    save_chance = save_chance * 90 / 100;  /* 10% reduction */
```

**Note:** The Overgrowth cost reduction (originally proposed as the keystone)
is moved to a Thornwarden passive skill (Deep Roots) and a Hierophant passive
(Hierophant's Wisdom), keeping the mechanic within the Druid lineage where it
is relevant rather than as a universal unlock.

---

### Implementation: HP Cost in do_cast

The existing `do_cast()` in `magic.c` checks `ch->mana < mana` and deducts
from `ch->mana`. For Druid classes, this path is replaced:

```c
if (is_druid_class(ch))
{
    int base_cost = skill_table[sn].min_mana;
    int cost = base_cost * (100 + ch->overgrowth * 2) / 100;

    if (ch->hit <= cost && ch->hit <= 1)
    {
        send_to_char("Your body has nothing left to give.\n\r", ch);
        return;
    }

    /* Deduct HP, apply death protection */
    ch->hit -= cost;
    if (ch->hit < 1)
    {
        ch->hit = 1;
        ch->overgrowth = 100;
        send_to_char(
            "@@dThe roots tear through you"
            " -- your body can give no more.@@N\n\r", ch);
    }

    /* Increase Overgrowth */
    ch->overgrowth += skill_table[sn].growth;
    if (ch->overgrowth > 100)
        ch->overgrowth = 100;
}
else
{
    /* Standard mana path (existing code) */
    if (ch->mana < mana)
    {
        send_to_char("You don't have enough mana.\n\r", ch);
        return;
    }
    ch->mana -= mana;
}
```

The `is_druid_class()` helper returns TRUE for Druid, Thornwarden, Wildspeaker,
and Hierophant — any class in the Druid lineage.

### Implementation: Overgrowth Power Multiplier

The Overgrowth power multiplier is applied in damage calculation functions.
A new helper:

```c
int apply_overgrowth_bonus(CHAR_DATA *ch, int base_damage)
{
    if (!is_druid_class(ch) || ch->overgrowth == 0)
        return base_damage;
    return base_damage * (100 + ch->overgrowth * 3) / 100;
}
```

This is called in `spell_damage()` (for offensive spells) and in healing spell
functions (Overgrowth also amplifies healing spells — the substrate gives more
of everything). Applied after spellpower modifiers, before victim resistance.

### Implementation: Overgrowth Decay in update.c

```c
/* In char_update(), after existing regen logic */
if (is_druid_class(ch) && ch->overgrowth > 0
    && !is_fighting(ch) && ch->wait == 0)
{
    int decay = 1;
    if (ch->position == POS_SLEEPING || ch->position == POS_RESTING)
        decay = 2;
    ch->overgrowth = UMAX(0, ch->overgrowth - decay);
}
```

### New Field on skill_table

A new `sh_int growth` field is added to the skill/spell table entry:

```c
sh_int growth;   /* Overgrowth points generated per cast (0 for non-Druid spells) */
```

Non-Druid spells have `growth = 0`. Druid spells have `growth` values from
3 (minor spells) to 8 (powerful spells), controlling how quickly Overgrowth
escalates. Cheap utility spells grow slowly; powerful damage/healing spells
grow fast.

---

## The Four Classes

### Druid (Mortal)

**Who:** Dru | **Prime:** WIS | **HP Gain:** 5 | **Mana Gain:** 0

**Gameplay Identity:** The Druid is a nature caster who trades health for power.
At mortal tier they have access to basic substrate magic — damage spells drawn
from earth, thorn, and decay; self-healing that costs less HP than it restores
(net positive, but still subject to Overgrowth escalation); and utility spells
for survival in the wilderness. Their spell list is smaller than a Magi's or
Cleric's at equivalent levels, but each spell hits harder per cast because
Overgrowth amplifies everything.

The mortal Druid teaches the fundamental tension: cast more to deal more damage,
but every cast brings you closer to death. New Druids who spam spells die. Good
Druids learn when to stop casting and let Overgrowth decay before re-engaging.

**Lore:** The mortal Druid represents the common substrate practitioner — someone
who learned to listen to the living world and channel its power through their
body. This is the tradition of the fenland herbalists, the forest wardens, the
village healers who worked without institutional backing. They are not formalized
into schools or orders. Their training comes from apprenticeship, observation,
and trial — the trial being literal, since channeling too much kills you.

The Wizard's Keep does not recognize Druid practice as legitimate magic. The
Violet Compact's registry framework has no category for HP-based casting — it
was designed around mana expenditure as the measurable unit of magical effort.
Druids exist in a legal gray area in most cities: not outlawed (their healing
is too useful), not registered (their practice doesn't fit the forms), not
trusted (unregulated power makes institutions nervous).

**Spell Themes:**
- **Earth/stone damage:** Rockburst, Thornlash, Quake Strike
- **Decay/entropy damage:** Wither, Rot, Blight Touch
- **Self-healing:** Sap Mend (heals more HP than it costs, but adds Overgrowth)
- **Utility:** Barkskin (AC buff), Roothold (prevents flee/push), Forestsight
  (detect hidden in outdoor rooms)

---

### Thornwarden (Remort — from Druid)

**Who:** Tho | **Prime:** WIS | **HP Gain:** 10 | **Mana Gain:** 0

**Gameplay Identity:** The Thornwarden is the Druid's **offensive and defensive**
remort path — the wall of thorns. They specialize in damage-over-time effects,
area denial, and tanking through substrate-reinforced resilience. Their spells
cost more HP but generate less Overgrowth per cast (growth values of 2-5),
allowing sustained casting over longer fights. The Thornwarden is the Druid who
learned to endure.

Where other tanks (Warden, Knight, Crusader) rely on armor and HP to survive,
the Thornwarden relies on **retaliatory damage** — thorns that punish attackers,
roots that immobilize, and decay auras that weaken enemies over time. They are
less immediately durable than a Warden but more dangerous to fight in sustained
engagements.

**Lore:** The Thornwarden tradition descends from the **Cairn-Keepers** and
the **Root Covenant** practitioners of Thornwood. The Cairn-Keepers maintained
megalith circles in the Great Northern Forest where the boundary between living
and dead was thin — stone that remembered, earth that judged. The Root Covenant
in Thornwood took this further: blood opens channels, roots carry confession,
the substrate itself becomes judge and executioner.

When the Thornwood binding began failing and the dead rose with morphological
adaptations, the Root Covenant practitioners were the first to stand against
it — not with weapons or wards, but by turning the substrate's own defensive
mechanisms against the corruption. They became wardens of the boundary between
growth and decay, understanding both as expressions of the same living system.

The Thornwarden's retaliatory magic reflects this: they do not attack so much as
make their environment hostile to threats. The thorns grow because something
threatened the forest. The rot spreads because something violated the substrate's
integrity. The Thornwarden is the immune response.

**Spell Themes:**
- **Damage-over-time:** Creeping Rot, Thorn Embrace, Fungal Infestation
- **Area denial:** Wall of Thorns (blocks exits), Blighted Ground (room DoT)
- **Retaliatory:** Bark of Thorns (damages attackers), Root Retaliation
- **Defensive:** Ironwood Skin (high AC buff), Stone Roots (immovable stance)
- **Debuffs:** Sap Strength (STR drain), Wither Limbs (DEX drain)

---

### Wildspeaker (Remort — from Druid)

**Who:** Wil | **Prime:** WIS | **HP Gain:** 8 | **Mana Gain:** 0

**Gameplay Identity:** The Wildspeaker is the Druid's **healing and support**
remort path — the voice of the living world. They specialize in restoring allies,
buffing groups, and manipulating the battlefield through growth and
transformation. Their spells generate higher Overgrowth (growth values of 4-8)
but have lower base HP costs, creating a pattern of rapid escalation that
rewards short, intense bursts of support.

The Wildspeaker's healing is structurally different from Cleric/Priest healing.
Cleric healing costs mana and has no escalation mechanic — a Priest can heal
steadily for as long as their mana holds. Wildspeaker healing costs HP and
escalates through Overgrowth — they heal in intense bursts, then must stop
and recover. The tradeoff: Wildspeaker heals are stronger per cast (amplified
by Overgrowth) but cannot be sustained indefinitely.

This creates a distinct group role: the Wildspeaker is the **emergency healer**
who can output massive healing in a crisis but burns out. The Priest is the
**sustained healer** who keeps the group alive over long encounters. Groups
benefit from both.

**Lore:** The Wildspeaker tradition descends from the **Mire-Speakers** and
the **Everkeepers** — the fenland herbalists who healed with wetland plants and
the ironpine tenders who maintained the Great Northern Forest's canopy health.
These practitioners understood healing not as a divine gift or an academic
technique but as a conversation with the living substrate: you ask the roots to
carry vitality, you ask the water to flush toxins, you ask the growth to close
wounds.

The "speaking" in Wildspeaker is literal in the tradition's self-understanding.
The substrate is alive and responsive. It does not require prayer (Cleric) or
formal invocation (Magi) — it requires listening, asking, and paying the price
of the answer. The Wildspeaker's HP cost represents the substrate drawing on the
speaker's own vitality to fuel the healing — the forest does not create energy
from nothing. It borrows from one body to restore another.

The Eccentric Woodland's shifting paths and local ritual codes are understood
by Wildspeakers as the forest's active communication — not hostility but
conversation in a language most travelers don't speak. Wildspeakers who enter
the Eccentric Woodland report that the paths hold steady for them, that the
clearings welcome rather than test. The forest recognizes its speakers.

**Spell Themes:**
- **Healing:** Verdant Mend (strong single-target), Canopy Shelter (group HoT),
  Sap Transfusion (transfer own HP to target — bypasses Overgrowth cost scaling)
- **Buffs:** Gift of the Forest (stat boost), Ironpine Vigor (max HP increase),
  Windborne Speed (haste equivalent)
- **Battlefield control:** Entangling Growth (root/snare), Obscuring Spores
  (blind), Shifting Paths (confusion/misdirection)
- **Transformation:** Beastskin (temporary stat shift), Thornform (melee damage
  bonus + thorns retaliation)

---

### Hierophant (Adept — from Thornwarden + Wildspeaker)

**Who:** Hie | **Prime:** WIS | **HP Gain:** 16 | **Mana Gain:** 0

**Gameplay Identity:** The Hierophant is the synthesis of Thornwarden endurance
and Wildspeaker intensity — a master of the full substrate cycle (growth, decay,
renewal). They have access to both remort spell lists plus exclusive adept
abilities that **manipulate Overgrowth itself** as a tactical resource.

The Hierophant's unique mechanic: **Controlled Burn**. Three adept-exclusive
abilities let the Hierophant spend or redistribute Overgrowth in ways that
the remort classes cannot:

1. **Rootflare** — Instantly convert all current Overgrowth into a single burst
   of damage. Damage scales with Overgrowth spent. Resets Overgrowth to 0.
   The Hierophant's finisher: build Overgrowth through sustained casting, then
   cash it all in for one devastating strike.

2. **Substrate Communion** — Instantly reduce Overgrowth by 30 points. Costs a
   flat 15% of max HP (not subject to Overgrowth cost scaling). Usable once per
   fight. The Hierophant's pressure valve: when Overgrowth is dangerously high,
   they can burn max HP to reset partially and continue casting.

3. **Cycle of Renewal** — Channel Overgrowth into a group heal. Healing power
   scales with Overgrowth spent. Resets Overgrowth to 0. The support-oriented
   alternative to Rootflare: instead of converting accumulated power into
   damage, convert it into restoration.

These three abilities give the Hierophant tactical control over the Overgrowth
curve that lower-tier Druids lack. A mortal Druid or remort Druid can only
manage Overgrowth by stopping casting. The Hierophant can actively spend it,
turning the escalation mechanic from a liability into a resource.

**Lore:** The Hierophant is the rarest and most complete expression of the
substrate tradition — a practitioner who has mastered both the Thornwarden's
boundary-defense and the Wildspeaker's restorative communion, and synthesized
them into a unified understanding of the living world's full cycle.

The term "Hierophant" is not native to the druidic tradition. It was applied by
Wizard's Keep scholars who encountered Hierophants during the Ash-and-Iron
Decades, when volcanic instability from the Cinderteeth required practitioners
of every tradition to cooperate against environmental catastrophe. The Keep
scholars, trained to classify everything, needed a word for "the one who
shows the sacred things" — the substrate practitioner who could demonstrate
that growth and decay were not opposing forces but phases of a single system.

The name stuck because it was accurate. Hierophants do not simply cast spells
from two traditions — they demonstrate, through their practice, that the
Thornwarden's decay and the Wildspeaker's growth are the same power expressed
at different phases of the cycle. The Overgrowth mechanic itself is a
manifestation of this understanding: the escalating power and cost are not a
flaw to be managed but the natural rhythm of the substrate's breathing. The
Hierophant breathes with it.

During the Thornwood binding failure, Hierophants were among the few
practitioners who could operate in the boundary zone between living forest and
necrotic corruption. Where Thornwardens could resist the corruption and
Wildspeakers could restore damaged growth, Hierophants could do both
simultaneously — holding the line while regenerating it, cycling between
defensive decay-management and restorative growth faster than either specialist.
This is the Controlled Burn in practice: sustained combat cycling between
Overgrowth buildup and strategic expenditure.

**Exclusive Spell Themes:**
- **Overgrowth manipulation:** Rootflare (damage finisher), Substrate Communion
  (Overgrowth reduction), Cycle of Renewal (healing finisher)
- **Cycle spells:** Verdant Collapse (high damage + self-heal, very high
  Overgrowth growth), Deathbloom (DoT that heals the caster for damage dealt)
- **Ultimate buffs:** World's Embrace (group-wide damage reduction + regen),
  Ancient Roothold (party immovable + thorns retaliation)
- **Transformation:** Substrate Form (fully merge with the living substrate —
  massive stat boost, all spells free of HP cost for 3 rounds, then Overgrowth
  instantly set to 100)

---

## Spell and Skill List

All spells in the Druid lineage use HP instead of mana. The `min_mana` field
in `skill_table` is reinterpreted as `min_hp_cost` for these spells. The
`growth` field controls Overgrowth escalation per cast.

Spells marked **shared** are available to all Druid lineage classes at the
listed level. Spells marked with a specific class are exclusive to that class
and its successors (Thornwarden spells are available to Hierophant, etc.).

### Mortal Druid Spells (Levels 1-100)

Mortal caster spell counts for reference: Magi has 45 spells (levels 3-80),
Cleric has 32 spells (levels 5-89), Psionicist has 31 spells (levels 4-80).
The mortal Druid targets ~35 spells across levels 3-90, with a mix of
earth/poison/decay offense, self/ally healing, buffs, debuffs, and utility.

All HP costs below are base values before Overgrowth scaling. All Druid spells
use the existing element system: `ELE_EARTH`, `ELE_POISON`, `ELE_WATER`,
`ELE_PHYSICAL`, or `ELE_NONE` for buffs/heals.

#### Offensive Spells

| Spell | Lvl | HP Cost | Growth | Beats | Target | Element | Damage Noun | Notes |
|-------|-----|---------|--------|-------|--------|---------|-------------|-------|
| thornlash | 3 | 12 | 3 | 12 | TAR_CHAR_OFFENSIVE | EARTH | thornlash | Entry-level damage. Equivalent to magic missile. |
| nettlesting | 6 | 12 | 3 | 12 | TAR_CHAR_OFFENSIVE | POISON | nettlesting | Poison-element variant. |
| rockburst | 10 | 15 | 4 | 12 | TAR_CHAR_OFFENSIVE | EARTH | rockburst | Scaling earth damage. Equivalent to burning hands. |
| wither | 16 | 15 | 4 | 12 | TAR_CHAR_OFFENSIVE | POISON | wither | Decay damage. Equivalent to chill touch. |
| briar volley | 20 | 18 | 4 | 12 | TAR_CHAR_OFFENSIVE | EARTH | briar volley | Thorn projectiles. Equivalent to lightning bolt. |
| blight touch | 25 | 20 | 5 | 12 | TAR_CHAR_OFFENSIVE | POISON | blight touch | Apply poison DoT (4 ticks). Like poison spell. |
| tremor | 32 | 20 | 5 | 12 | TAR_CHAR_OFFENSIVE | EARTH | tremor | Heavy earth damage. Equivalent to fireball. |
| creeping rot | 38 | 22 | 5 | 12 | TAR_CHAR_OFFENSIVE | POISON | creeping rot | Decay DoT + minor healing reduction. |
| fissure | 46 | 25 | 6 | 12 | TAR_CHAR_OFFENSIVE | EARTH | fissure | High earth damage, chance to knock down. |
| fungal eruption | 55 | 30 | 6 | 24 | TAR_IGNORE | POISON | fungal eruption | Area poison damage to all enemies in room. |
| quake | 60 | 30 | 6 | 12 | TAR_IGNORE | EARTH | earthquake | Area earth damage. Equivalent to earthquake. |
| strangling vines | 65 | 35 | 5 | 24 | TAR_CHAR_OFFENSIVE | EARTH | strangling vines | Heavy damage + 2-tick snare (victim can't flee). |
| rot | 75 | 40 | 7 | 12 | TAR_CHAR_OFFENSIVE | POISON | rot | Heavy decay damage. High-level nuke. |
| upheaval | 85 | 50 | 7 | 24 | TAR_IGNORE | EARTH | upheaval | Major area earth damage. Capstone AoE. |

#### Healing Spells

| Spell | Lvl | HP Cost | Growth | Beats | Target | Notes |
|-------|-----|---------|--------|-------|--------|-------|
| sap mend | 3 | 8 | 3 | 12 | TAR_CHAR_SELF | Heals 20-35 HP. Net positive. Self only. |
| mend wounds | 12 | 12 | 4 | 12 | TAR_CHAR_DEFENSIVE | Heals target 25-45 HP. First other-target heal. |
| purge toxin | 22 | 10 | 3 | 12 | TAR_CHAR_DEFENSIVE | Cure poison on target. Equivalent to cure poison. |
| verdant mend | 35 | 20 | 5 | 12 | TAR_CHAR_DEFENSIVE | Heals target 50-80 HP. Main heal. Equivalent to heal. |
| heartwood balm | 50 | 15 | 5 | 12 | TAR_CHAR_DEFENSIVE | Cure blindness + cure poison combined. |
| primal restoration | 70 | 35 | 6 | 12 | TAR_CHAR_DEFENSIVE | Large heal (80-120 HP) + cure poison + cure blind. |
| sap transfusion | 80 | Special | 3 | 12 | TAR_CHAR_DEFENSIVE | Transfer own HP to target at 1:1 ratio. Not subject to Overgrowth cost scaling. Flat Overgrowth growth. |

Healing spells always cost less HP than they restore (net positive). The
Overgrowth growth makes each successive heal generate more Overgrowth,
increasing the cost of the *next* spell cast. Self-healing keeps the Druid
alive; the Overgrowth tax is paid on the spell that follows.

#### Buff Spells

| Spell | Lvl | HP Cost | Growth | Beats | Target | Notes |
|-------|-----|---------|--------|-------|--------|-------|
| barkskin | 5 | 10 | 2 | 12 | TAR_CHAR_SELF | AC bonus, 12 ticks. Equivalent to armor. |
| roothold | 9 | 12 | 2 | 18 | TAR_CHAR_SELF | Prevents flee/push on self, 6 ticks. |
| thornhide | 14 | 15 | 2 | 18 | TAR_CHAR_SELF | Reflects a portion of melee damage, 8 ticks. |
| stone skin | 28 | 18 | 3 | 18 | TAR_CHAR_SELF | Major AC buff, 10 ticks. Same as Magi stone skin. |
| ironpine vigor | 36 | 20 | 3 | 18 | TAR_CHAR_DEFENSIVE | Increase target max HP by 10%, 10 ticks. |
| gift of the forest | 45 | 20 | 3 | 18 | TAR_CHAR_DEFENSIVE | +2 all stats, 10 ticks. Equivalent to bless+. |
| nature's ward | 55 | 25 | 3 | 24 | TAR_CHAR_SELF | Resist poison + earth damage, 10 ticks. |
| ancient bark | 68 | 35 | 3 | 24 | TAR_CHAR_SELF | Large AC buff + thorns reflection, 8 ticks. Stacks with barkskin. |

#### Debuff Spells

| Spell | Lvl | HP Cost | Growth | Beats | Target | Notes |
|-------|-----|---------|--------|-------|--------|-------|
| tangling roots | 8 | 10 | 3 | 12 | TAR_CHAR_OFFENSIVE | Faerie fire equivalent (lower AC on target). |
| sap strength | 18 | 15 | 4 | 12 | TAR_CHAR_OFFENSIVE | -2 STR on target, 6 ticks. |
| spore cloud | 30 | 18 | 4 | 12 | TAR_CHAR_OFFENSIVE | Blind, 3 ticks. Equivalent to blindness. |
| wither limbs | 40 | 20 | 4 | 12 | TAR_CHAR_OFFENSIVE | -2 DEX on target, 6 ticks. |
| entangling growth | 52 | 25 | 4 | 24 | TAR_CHAR_OFFENSIVE | Root/snare — victim cannot flee, 3 ticks. |
| curse of decay | 62 | 30 | 5 | 12 | TAR_CHAR_OFFENSIVE | Curse equivalent + poison DoT combined. |
| creeping blight | 78 | 40 | 5 | 24 | TAR_CHAR_OFFENSIVE | -3 STR, -3 CON, healing reduction, 6 ticks. High-level debuff. |

#### Utility Spells

| Spell | Lvl | HP Cost | Growth | Beats | Target | Notes |
|-------|-----|---------|--------|-------|--------|-------|
| forestsight | 7 | 5 | 1 | 12 | TAR_CHAR_SELF | Detect hidden. Equivalent to detect hidden. |
| sense life | 11 | 5 | 1 | 12 | TAR_CHAR_SELF | Detect invis. Equivalent to detect invis. |
| know terrain | 15 | 8 | 1 | 12 | TAR_CHAR_SELF | Know alignment equivalent (sense target nature). |
| passage | 24 | 12 | 2 | 18 | TAR_CHAR_SELF | Pass door equivalent (roots find a path). |
| greensight | 42 | 15 | 2 | 24 | TAR_CHAR_DEFENSIVE | Infravision equivalent + detect hidden combined. |
| rootway | 58 | 30 | 3 | 12 | TAR_IGNORE | Teleport equivalent. Travel through root network. |
| call of the wild | 72 | 40 | 3 | 12 | TAR_IGNORE | Summon equivalent. Call target through substrate. |
| living bridge | 90 | 60 | 3 | 12 | TAR_IGNORE | Portal equivalent. Grow a passage to target's room. |

**Total: 36 spells** (14 offensive, 7 healing, 8 buff, 7 debuff, 8 utility —
some spells bridge categories). Distributed across levels 3-90.

### Mortal Druid Skills (Levels 1-100)

Mortal casters have very few skills (Magi: 1, Cleric: 1, Psionicist: 3).
The Druid follows this pattern — one passive keystone skill.

| Skill | Lvl | Beats | Target | Notes |
|-------|-----|-------|--------|-------|
| substrate piercing | 20 | 0 | TAR_IGNORE | Passive keystone. -10% target spell resistance. See keystone section. |

### Thornwarden Spells (Remort, Levels 1-100)

| Spell | Level | Base HP Cost | Growth | Target | Effect |
|-------|-------|-------------|--------|--------|--------|
| Creeping Rot | 5 | 25 | 3 | Offensive | Strong DoT, 6 ticks |
| Bark of Thorns | 10 | 30 | 2 | Self | Reflects 20% melee damage, 8 ticks |
| Ironwood Skin | 20 | 40 | 3 | Self | Major AC buff, 10 ticks |
| Sap Strength | 28 | 35 | 4 | Offensive | -3 STR for 8 ticks |
| Fungal Infestation | 35 | 45 | 4 | Offensive | DoT + healing reduction on target |
| Wall of Thorns | 42 | 50 | 3 | Room | Blocks one exit, 5 ticks |
| Wither Limbs | 50 | 40 | 4 | Offensive | -3 DEX for 8 ticks |
| Root Retaliation | 58 | 55 | 3 | Self | Counter-spell on melee hit, 6 ticks |
| Blighted Ground | 68 | 60 | 5 | Room | Area DoT to all enemies, 4 ticks |
| Stone Roots | 78 | 50 | 2 | Self | Immovable + bash immune, 4 ticks |
| Thorn Embrace | 88 | 65 | 5 | Offensive | Heavy DoT + snare, 4 ticks |
| Decay Aura | 95 | 70 | 4 | Self | Passive damage to all attackers, 8 ticks |

### Thornwarden Skills (Remort, Levels 1-100)

| Skill | Level | Effect |
|-------|-------|--------|
| Endure | 15 | Passive: reduce HP cost of spells by 5% |
| Third Attack | 25 | Standard third attack |
| Substrate Anchor | 40 | Resist dispel on self-buffs |
| Thorn Strike | 55 | Melee attack that adds nature DoT |
| Ironbark | 70 | Passive: +5% max HP |
| Deep Roots | 85 | Passive: Overgrowth cost multiplier reduced by 10% |

### Wildspeaker Spells (Remort, Levels 1-100)

| Spell | Level | Base HP Cost | Growth | Target | Effect |
|-------|-------|-------------|--------|--------|--------|
| Canopy Shelter | 5 | 25 | 5 | Group | Group HoT, 4 ticks |
| Windborne Speed | 12 | 30 | 4 | Defensive | Haste equivalent, 8 ticks |
| Obscuring Spores | 20 | 25 | 5 | Offensive | Blind, 3 ticks |
| Sap Transfusion | 28 | Special | 3 | Defensive | Transfer own HP to target (1:1, no Overgrowth cost scaling) |
| Beastskin | 35 | 35 | 6 | Self | Shift stats toward combat form |
| Shifting Paths | 42 | 40 | 5 | Offensive | Confusion/misdirection, 3 ticks |
| Verdant Surge | 50 | 30 | 6 | Defensive | Large single-target heal (80-120) |
| Thornform | 60 | 45 | 6 | Self | Melee damage bonus + thorns, 6 ticks |
| Nature's Bounty | 70 | 40 | 5 | Group | Group stat buff (+1 all), 10 ticks |
| Regenerative Spores | 80 | 50 | 7 | Group | Strong group HoT, 6 ticks |
| Primal Restoration | 90 | 60 | 8 | Defensive | Full single-target heal + cleanse |
| Windstorm | 95 | 55 | 7 | Room | Area damage + knockback |

### Wildspeaker Skills (Remort, Levels 1-100)

| Skill | Level | Effect |
|-------|-------|--------|
| Empathic Bond | 10 | See target's HP percentage |
| Third Attack | 25 | Standard third attack |
| Wild Instinct | 35 | Passive: dodge bonus in outdoor rooms |
| Growth Pulse | 50 | Passive: healing spells restore 10% more |
| Voice of the Wild | 65 | Calm aggressive mobs in outdoor rooms |
| Overgrowth Mastery | 85 | Passive: reduce Overgrowth growth by 1 per cast |

### Hierophant Spells (Adept, Levels 1-100)

| Spell | Level | Base HP Cost | Growth | Target | Effect |
|-------|-------|-------------|--------|--------|--------|
| Rootflare | 1 | 0 (spends Overgrowth) | 0 | Offensive | Burst damage = Overgrowth * 15. Resets to 0. |
| Cycle of Renewal | 1 | 0 (spends Overgrowth) | 0 | Group | Group heal = Overgrowth * 10. Resets to 0. |
| Substrate Communion | 5 | 15% max HP | 0 | Self | Reduce Overgrowth by 30. Once per fight. |
| Deathbloom | 15 | 50 | 6 | Offensive | DoT that heals caster for 50% damage dealt |
| Verdant Collapse | 30 | 70 | 8 | Offensive | Heavy damage + self-heal 30% of damage |
| World's Embrace | 45 | 80 | 5 | Group | Group damage reduction 15% + regen, 8 ticks |
| Ancient Roothold | 60 | 90 | 4 | Group | Party immovable + thorns retaliation, 4 ticks |
| Substrate Form | 80 | 50% max HP | 0 | Self | All spells free for 3 rounds, then Overgrowth → 100 |
| Worldseed | 95 | 100 | 8 | Room | Massive area damage + group heal + area denial |

### Hierophant Skills (Adept, Levels 1-100)

| Skill | Level | Effect |
|-------|-------|--------|
| Fourth Attack | 10 | Standard fourth attack |
| Cycle Mastery | 20 | Passive: Rootflare/Cycle of Renewal damage/healing +20% |
| Substrate Resilience | 40 | Passive: HP regen continues in combat |
| Deep Communion | 55 | Passive: Substrate Communion usable twice per fight |
| Living Conduit | 70 | Passive: 5% of damage dealt heals self |
| Hierophant's Wisdom | 90 | Passive: Overgrowth power multiplier +1% per point (4% total) |

---

## Balance Analysis

### Comparison to Existing Casters

The Druid lineage occupies a fundamentally different resource curve than any
existing class. This section analyzes how the HP-cost + Overgrowth system
compares to mana-based casters across key gameplay scenarios.

**Sustained DPS (long fight):**

| Class | Resource | Behavior | Sustained DPS |
|-------|----------|----------|---------------|
| Grand Magi | Mana (20/level) | Constant damage until OOM | High, flat |
| Kinetimancer | Mana (16/level) | Constant damage until OOM | High, flat |
| Thornwarden | HP (10/level) | Escalating damage, escalating cost | Starts moderate, peaks very high, then must stop |
| Hierophant | HP (16/level) | Escalating, then Rootflare reset | Sawtooth: escalate → burst → reset → repeat |

The Druid's damage curve is fundamentally different: it starts below mana casters
but crosses above them around Overgrowth 30-40, then exceeds them significantly
at high Overgrowth. However, the Druid must periodically stop (or use
Hierophant resets) while mana casters continue at constant output.

**Burst DPS (short fight):**

Mana casters are stronger in short fights. Overgrowth doesn't have time to
ramp. A Grand Magi casting at full power from round 1 outperforms a Druid
who starts at Overgrowth 0. However, a Druid who enters a fight with residual
Overgrowth from a previous encounter starts pre-ramped — risky but powerful.

**Healing throughput:**

| Class | Resource | Healing Model |
|-------|----------|---------------|
| Priest | Mana (8/level) | Constant, sustained, predictable |
| Templar | Mana (12/level) | Moderate sustained, combat hybrid |
| Wildspeaker | HP (8/level) | Burst healing, escalating cost |
| Hierophant | HP (16/level) | Burst healing + Cycle of Renewal finisher |

Wildspeaker/Hierophant healing is **stronger per cast** than Priest healing at
equivalent Overgrowth levels, but cannot be sustained as long. A Priest can
heal for an entire boss fight. A Wildspeaker heals explosively for 8-12 casts,
then must stop or die. The Hierophant's Cycle of Renewal converts accumulated
Overgrowth into a massive group heal, then resets — allowing them to start
healing again at base cost.

**Survivability:**

Druids are the most fragile caster lineage *because they spend their HP to
cast*. A Druid at 30% HP might have 70% of their mana-caster equivalent's
health pool remaining, but they've spent it on spells rather than losing it to
enemy damage. This means:

- Druids benefit enormously from external healing (Priest, Cleric, potions)
- Druids are extremely vulnerable to burst damage during high-Overgrowth casting
- Druids must make constant risk decisions that no other class faces
- A Druid who miscalculates dies to their own casting, not to the enemy

This fragility is the intended tradeoff for the escalating power. The Druid
lineage is high-risk, high-reward in a way that no other class achieves.

### Overgrowth Tuning Levers

The balance of the Overgrowth system can be adjusted through several
independent parameters without restructuring the mechanic:

| Parameter | Effect of Increase | Current Value |
|-----------|-------------------|---------------|
| Cost multiplier per Overgrowth point | Spells get expensive faster | 2% per point |
| Power multiplier per Overgrowth point | Spells get powerful faster | 3% per point |
| Growth per spell (individual) | Overgrowth escalates faster | 3-8 per spell |
| Decay rate | Overgrowth resets faster between fights | 1/tick (2/tick resting) |
| Decay delay | How long before decay starts | 2 ticks |
| Overgrowth cap | Maximum escalation | 100 |

If Druids prove too strong in sustained fights, increase the cost multiplier.
If they're too weak in short fights, reduce growth values so Overgrowth builds
slower (keeping them at the lower, more efficient part of the curve longer).
If the healer-dependency is too punishing, increase HP gain per level. Each
parameter is independently tunable.

### Group Dynamics

The Druid lineage creates new group dynamics that don't exist with mana casters:

- **Druid + Priest:** The strongest synergy. The Priest's sustained healing
  directly fuels the Druid's casting. The Priest never runs out of targets to
  heal because the Druid is always spending HP. The Druid never runs out of HP
  because the Priest keeps refilling it. Together they create a sustained
  high-output engine that neither can achieve alone.

- **Druid + Druid:** Risky but explosive. Two Druids can trade Sap Transfusion
  heals to keep each other casting, but both accumulate Overgrowth. The ceiling
  is high; the crash is spectacular.

- **Druid + Tank:** The Druid provides strong DoT and debuff support from the
  back line while the tank holds aggro. The Druid's self-healing (Sap Mend,
  Verdant Mend) keeps them alive as long as they're not taking direct hits.

- **Solo Druid:** Viable but demanding. The Druid must balance offense with
  self-healing, managing Overgrowth carefully. Mortal Druids solo more
  cautiously than Magi (who can burst and rest). Thornwardens solo well through
  retaliatory damage and endurance. Wildspeakers solo poorly (support-oriented).
  Hierophants solo well through Overgrowth manipulation.

---

## Implementation Impact

Adding a 7th mortal class with 2 remort paths and 1 adept requires structural
changes to the class system. This section documents every affected system.

### Class System Expansion

**config.h changes:**

```c
/* Change MAX_CLASS from 6 to 7 */
#define MAX_CLASS 7

/* MAX_REMORT becomes 14 (7 * 2) */
#define MAX_REMORT (MAX_CLASS * 2)

/* MAX_TOTAL_CLASS becomes 7 + 14 + 7 = 28 */
#define MAX_TOTAL_CLASS (MAX_CLASS + MAX_REMORT + MAX_CLASS)

/* New class IDs */
/* Mortal (0-6) */
#define CLASS_DRU 6    /* Druid — replaces CLASS_SOR's slot */

/* All remort IDs shift: remort range is now 7-20 */
#define CLASS_SOR 7
#define CLASS_PAL 8
#define CLASS_ASS 9
#define CLASS_KNI 10
#define CLASS_NEC 11
#define CLASS_MON 12
#define CLASS_WIZ 13
#define CLASS_PRI 14
#define CLASS_WLK 15
#define CLASS_SWO 16
#define CLASS_EGO 17
#define CLASS_BRA 18
#define CLASS_THO 19   /* Thornwarden */
#define CLASS_WIL 20   /* Wildspeaker */

/* Adept range is now 21-27 */
#define CLASS_GMA 21
#define CLASS_TEM 22
#define CLASS_NIG 23
#define CLASS_CRU 24
#define CLASS_KIN 25
#define CLASS_MAR 26
#define CLASS_HIE 27   /* Hierophant */
```

**WARNING:** Renumbering all class IDs is the most disruptive change in this
proposal. Every reference to a class constant throughout the codebase must be
updated. The alternative — appending new class IDs at the end (e.g., Druid=24,
Thornwarden=25, Wildspeaker=26, Hierophant=27) — avoids renumbering but breaks
the tier-range assumptions that `IS_MORTAL_CLASS`, `IS_REMORT_CLASS`, and
`IS_ADEPT_CLASS` depend on.

**Recommended approach:** Append at end and modify the tier macros to use
`gclass_table[c].tier` (which they already do) rather than range checks. The
class table's `tier` field already correctly identifies class tier regardless of
ID ordering. This avoids renumbering:

```c
/* Append new classes after existing 24 */
#define CLASS_DRU 24   /* Druid (mortal) */
#define CLASS_THO 25   /* Thornwarden (remort) */
#define CLASS_WIL 26   /* Wildspeaker (remort) */
#define CLASS_HIE 27   /* Hierophant (adept) */

/* Expand total count */
#define MAX_TOTAL_CLASS 28
```

The `MAX_CLASS`, `MAX_REMORT`, and adept count macros would need to be
decoupled from the assumption that classes are contiguous by tier. Since the
tier macros already use `gclass_table[c].tier`, this is the lower-risk path.

### const_class.c Changes

Four new entries appended to `gclass_table`:

```c
/* Druid lineage (appended after existing 24) */
{"Dru", "Druid",        APPLY_CON,  5, 0, MORTAL, {-1, -1}},
{"Tho", "Thornwarden",  APPLY_CON, 10, 0, REMORT, {CLASS_DRU, -1}},
{"Wil", "Wildspeaker",  APPLY_CON,  8, 0, REMORT, {CLASS_DRU, -1}},
{"Hie", "Hierophant",   APPLY_CON, 16, 0, ADEPT,  {CLASS_THO, CLASS_WIL}},
```

All four have `mana_gain = 0`.

### Spell/Skill Table Expansion

Every entry in `skill_table` has a `skill_level[MAX_TOTAL_CLASS]` array. With
`MAX_TOTAL_CLASS` increasing from 24 to 28, all entries grow by 4 slots. The
`LEVELS_INIT` macro already fills all slots with `NO_USE`, so existing spells
automatically exclude the new classes — no per-spell changes needed for
non-Druid spells.

New Druid spells are added to `spell_table_data.c` with `L(CLASS_DRU, level)`,
`L(CLASS_THO, level)`, `L(CLASS_WIL, level)`, and `L(CLASS_HIE, level)` in
their `skill_level` initializers.

The new `growth` field is added to the skill table struct. All existing spells
get `growth = 0` (no Overgrowth generation). Druid spells set `growth` to their
specific values (3-8).

### CHAR_DATA Changes

New field:

```c
sh_int overgrowth;   /* Druid Overgrowth counter, 0-100 */
```

Initialized to 0. Not saved to player file (resets on login). Reset to 0 on
death in `raw_kill()`.

### magic.c Changes

The `do_cast()` function needs a branch for Druid HP-cost casting. See the
Overgrowth Mechanic section above for the implementation sketch. Key changes:

1. Before the mana check: if `is_druid_class(ch)`, use HP-cost path instead
2. After successful cast: if `is_druid_class(ch)`, increment Overgrowth
3. In damage application: call `apply_overgrowth_bonus()` for Druid casters

### update.c Changes

In `char_update()`: add Overgrowth decay logic for Druid classes when out of
combat and not casting (see Overgrowth Mechanic section).

### save.c Changes

The `overgrowth` field is NOT saved. However, `class_level[MAX_TOTAL_CLASS]`
and `reincarnations[MAX_TOTAL_CLASS]` arrays in the player save format grow
from 24 to 28 entries. The save/load format must handle this expansion.

**Save format compatibility:** Existing player files have 24 class level
entries. When loaded with the new 28-entry format, the extra 4 entries should
default to 0. The save format uses space-separated integers on a line — the
loader should handle short lines by zero-filling missing entries.

### Character Creation (comm.c)

The character creation flow in `nanny()` presents class choices. Druid is added
as a 7th mortal option:

```
Select a class: [Magi] [Cleric] [Cipher] [Warden] [Psionicist] [Pugilist] [Druid]
```

### Remort System

The remort system in `do_remort()` checks class prerequisites. No structural
changes needed — the existing prereq check (`gclass_table[c].prereq[0]` and
`prereq[1]`) works for the new classes. A Druid who has leveled to 100 can
remort into Thornwarden or Wildspeaker. A character who has completed both
can advance to Hierophant.

### Integration Test

The integration test (`integration-test.sh`) walks the new-player login flow
including class selection. It must be updated to handle 7 class options instead
of 6. Since it selects a class by number, the Druid (option 7) may or may not
be selected depending on the test's random or fixed choice.

### Affected Files Summary

| File | Change |
|------|--------|
| `src/headers/config.h` | New class constants, MAX_TOTAL_CLASS = 28 |
| `src/const_class.c` | 4 new gclass_table entries |
| `src/headers/ack.h` | `sh_int overgrowth` on CHAR_DATA, `sh_int growth` on skill table |
| `src/magic.c` | HP-cost casting branch, Overgrowth increment, power bonus |
| `src/update.c` | Overgrowth decay in char_update() |
| `src/save.c` | Handle 28-entry class arrays (backward-compatible load) |
| `src/comm.c` | Druid in character creation class list |
| `src/death.c` | Reset overgrowth to 0 on death |
| `src/spells/spell_table_data.c` | New Druid spells with growth values |
| `src/skills/` | New Druid skill command files |
| `src/act_info.c` | Overgrowth display in score, prompt %o token |
| `integration-test.sh` | Handle 7th class option |
| Every unit test with gclass_table stub | Expand array to 28 entries |

---

## Lore Integration

### New Lore Documents Required

The Druid lineage requires four new lore documents in `docs/lore/`, following
the pattern of existing class lore files:

- **`druid_lore.md`** — The mortal Druid tradition. Covers the pre-civic
  substrate practitioners, their relationship to the Everkeepers/Mire-Speakers/
  Cairn-Keepers, the HP-as-fuel theology ("the substrate borrows from the body
  to do its work"), the legal gray area under the Violet Compact, and the
  tradition's presence in each of the five cities.

- **`thornwarden_lore.md`** — The Thornwarden remort path. Covers the
  Cairn-Keeper and Root Covenant lineage, the Thornwood boundary work, the
  retaliatory-defense philosophy, and the role Thornwardens played during the
  Thornwood binding failure.

- **`wildspeaker_lore.md`** — The Wildspeaker remort path. Covers the
  Mire-Speaker and Everkeeper lineage, the "speaking" theology (the substrate
  as conversational partner), the Eccentric Woodland as sacred ground, and the
  healing-as-conversation philosophy.

- **`hierophant_lore.md`** — The Hierophant adept. Covers the synthesis of
  growth and decay, the Wizard's Keep scholars naming the tradition, the
  Ash-and-Iron cooperation period, the Thornwood crisis response, and the
  Controlled Burn as philosophical and practical achievement.

### Existing Lore Cross-References

Several existing lore documents already describe traditions that feed into the
Druid lineage. These documents should be cross-referenced (not modified — the
Druid lore documents should reference them, not the reverse):

| Document | Relevant Content |
|----------|-----------------|
| `great_northern_forest_lore.md` | Everkeepers, Mire-Speakers, Cairn-Keepers |
| `thornwood_lore.md` | Root Covenant, binding failure, substrate theology |
| `verdant_depths_lore.md` | Ancient forest intelligence, druidic quarantine |
| `forest_of_confusion_lore.md` | Druidic ward remnants, containment history |
| `eccentric_woodland_lore.md` | Ritual codes, shifting paths, forest communication |
| `withered_depths_lore.md` | Conclave crystal contamination of root systems |
| `rivennid_lore.md` | Mycelial network communion, substrate awareness |
| `rocroad_lore.md` | Druid-adjacent custodians, Antler-Warden composite entity |
| `timeline.md` | Pre-civic era, forest peoples, Thornwood events |

### Druid Tradition in the Five Cities

Brief summary of how each city relates to the Druid tradition (to be expanded
in `druid_lore.md`):

**Midgaard:** Druids operate outside the Violet Compact registry. The Lantern
Registry has attempted multiple times to create a classification for
HP-based casting; each attempt failed because the registry's documentation
framework assumes mana expenditure as the measurable unit. Druids in Midgaard
practice quietly, offering healing services in lower wards where registry
enforcement is sporadic.

**Kowloon:** The delta city's hydraulic infrastructure was partially designed
with Deltari and substrate-practitioner input during the founding. The Temple
Circle tolerates Druids more openly than Midgaard's institutions — the Neon
Covenant's pragmatism extends to useful practitioners regardless of registry
status. Mire-Speaker fenland healers operate openly in Kowloon's wetland
districts.

**Kiess:** The anti-sealed-warrant charter's broad protections extend to Druid
practitioners. Kiess is the only city where Druids can practice fully openly
without registry pressure. The Forest of Confusion's proximity creates
practical demand for substrate expertise — Wall Command consults Druids for
mist-fever assessment and forest-approach planning.

**Rakuen:** The closest alignment between civic ideology and Druid philosophy.
Rakuen's founding Paradise Thesis (fusion of civic order and forest harmony)
echoes the Druid understanding of living systems. Druid practitioners serve
openly in Rakuen's disaster-response teams, particularly for agricultural
recovery and water purification.

**Mafdet:** Least presence. The maritime city has little forest substrate to
commune with. Druids in Mafdet are primarily travelers passing through the
Saltglass Reach, where Kethari water-memory techniques overlap with substrate
communion in ways that neither tradition fully acknowledges.

---

## Implementation Checklist

### Phase 1: Core System (class infrastructure)

- [ ] Add `CLASS_DRU`, `CLASS_THO`, `CLASS_WIL`, `CLASS_HIE` constants to
  `config.h`; expand `MAX_TOTAL_CLASS` to 28
- [ ] Add 4 new entries to `gclass_table` in `const_class.c`
- [ ] Add `sh_int overgrowth` field to `CHAR_DATA` in `ack.h`
- [ ] Add `sh_int growth` field to skill table struct in `ack.h`
- [ ] Set `growth = 0` for all existing spells/skills
- [ ] Add `is_druid_class()` helper function
- [ ] Add Druid to character creation class list in `comm.c`
- [ ] Handle 28-entry class arrays in `save.c` (backward-compatible load)
- [ ] Reset `overgrowth` to 0 in `raw_kill()` / death path
- [ ] Add `gsn_substrate_piercing` global and reinc_rules entry in `skills.c`
- [ ] Apply Substrate Piercing in spell save/resistance checks
- [ ] Expand `gclass_table` stubs in all unit test files to 28 entries
- [ ] Update integration test for 7th class option
- [ ] Run `make unit-tests` to verify no regressions

### Phase 2: Overgrowth Mechanic

- [ ] Implement HP-cost casting branch in `do_cast()` (`magic.c`)
- [ ] Implement Overgrowth increment on cast
- [ ] Implement `apply_overgrowth_bonus()` for spell damage/healing
- [ ] Implement Overgrowth decay in `char_update()` (`update.c`)
- [ ] Implement death protection (reduce to 1 HP, set Overgrowth to 100)
- [ ] Add `%o` prompt token for Overgrowth display
- [ ] Add Overgrowth display to `score` command in `act_info.c`
- [ ] Write unit tests for Overgrowth cost calculation
- [ ] Write unit tests for Overgrowth power bonus calculation
- [ ] Write unit tests for Overgrowth decay
- [ ] Write unit tests for death protection (1 HP floor)
- [ ] Run `make unit-tests`

### Phase 3: Mortal Druid Spells and Skills

- [ ] Add mortal Druid spell entries to `spell_table_data.c` with HP costs
  and growth values
- [ ] Implement spell functions for each new Druid spell
- [ ] Add mortal Druid skill entries and command functions
- [ ] Write `help` and `shelp` entries for all new spells/skills
- [ ] Write unit tests for representative Druid spell interactions
- [ ] Run `make unit-tests`

### Phase 4: Remort Classes (Thornwarden + Wildspeaker)

- [ ] Add Thornwarden spell/skill entries to spell and skill tables
- [ ] Implement Thornwarden spell functions (DoTs, thorns, area denial)
- [ ] Add Wildspeaker spell/skill entries
- [ ] Implement Wildspeaker spell functions (group healing, buffs, transforms)
- [ ] Implement Sap Transfusion (HP transfer, bypass Overgrowth cost scaling)
- [ ] Add Thornwarden and Wildspeaker passive skills (Endure, Deep Roots,
  Overgrowth Mastery, etc.)
- [ ] Write `help` and `shelp` entries for all remort spells/skills
- [ ] Write unit tests for remort-specific mechanics
- [ ] Run `make unit-tests`

### Phase 5: Adept Class (Hierophant)

- [ ] Implement Rootflare (spend Overgrowth for burst damage)
- [ ] Implement Cycle of Renewal (spend Overgrowth for group heal)
- [ ] Implement Substrate Communion (spend max HP to reduce Overgrowth,
  once-per-fight tracking)
- [ ] Implement Substrate Form (free casting for 3 rounds, then
  Overgrowth → 100)
- [ ] Add Hierophant passive skills (Cycle Mastery, Deep Communion, etc.)
- [ ] Implement Worldseed (area damage + group heal + area denial)
- [ ] Write `help` and `shelp` entries for all adept spells/skills
- [ ] Write unit tests for Hierophant Overgrowth manipulation abilities
- [ ] Run `make unit-tests`

### Phase 6: Lore and Documentation

- [ ] Write `docs/lore/druid_lore.md`
- [ ] Write `docs/lore/thornwarden_lore.md`
- [ ] Write `docs/lore/wildspeaker_lore.md`
- [ ] Write `docs/lore/hierophant_lore.md`
- [ ] Update `docs/proposals/druid.md` with any design changes made during
  implementation
- [ ] Update the `KNOW_NEWPLAYER` topic block (in the AI proposal) to include
  the Druid class in the class progression description

### Phase 7: Balance Testing

- [ ] Playtest mortal Druid solo viability (levels 1-100)
- [ ] Playtest Druid + Priest group synergy
- [ ] Playtest Overgrowth curve: verify 15-20 casts before lethal escalation
- [ ] Playtest Hierophant Rootflare damage at various Overgrowth levels
- [ ] Tune growth values per spell if escalation is too fast or too slow
- [ ] Tune cost/power multiplier ratio if risk/reward is off
- [ ] Verify backward compatibility of expanded player save format
