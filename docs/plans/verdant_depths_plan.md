# Area Plan: The Verdant Depths

## Overview

**Area Name:** `@@rThe @@GVerdant @@gDepths@@N`
**File Name:** `verdantgreen.are`
**Keyword:** `verdantdepths`
**Level Range:** 75-95 (I directive: `I 75 95`)
**Level Label:** `@@W(@@r75 95@@W)@@N`
**Vnum Range:** 3000-3149 (V directive: `V 3000 3149`)
**Vnum Count:** 125
**Connection:** Links from the Forest of Confusion's deep cave network (room 2509 north exit currently leads to 2503; a new exit from an appropriate Forest of Confusion room will lead into The Verdant Depths entry room at vnum 2739)
**Reset Rate:** 15
**Reset Message:** `@@gthe ancient trees creak and groan as the forest shifts around you@@N`

### Color Theme

- **Primary:** `@@G` (green) -- canopy, foliage, living forest
- **Secondary:** `@@g` (gray) -- mist, ancient stone, decay
- **Accent:** `@@r` (light green) -- bioluminescent moss, glowing fungi
- **Danger accent:** `@@R` (red) -- blood, corruption, boss lairs
- **Text reset:** `@@N` after all color sequences

The theme evokes a primeval forest grown unnaturally deep and old, where the confusion of the parent forest has given way to something deliberate and predatory. Green dominates with gray undertones suggesting age and hidden threat.

---


## Lore
See `verdant_depths_lore.md` for full extracted lore.

### Lore Summary (for implementation reference)

The Verdant Depths is the ancient core of the northern forest system — an ur-forest where the trees predate all known civilizations. The forest operates as a **distributed intelligence**: a decision-making system that emerges from billions of root connections, fungal networks, and chemical exchanges. It does not think — it responds. Its responses, refined over millennia, are sophisticated enough to look intentional.

The forest's intelligence expresses itself through **three aspects**:
1. **Growth** (Rootmother): expansion, colonization, biomass production
2. **Decay** (Blightwood Ancient): decomposition, nutrient recycling, waste processing
3. **Predation** (Thornfang Alpha): defense, territorial enforcement, intruder elimination

These three aspects form an integrated system: growth produces biomass, decay recycles it, predation defends the cycle. The three Heartwood Guardians are focal points where each aspect concentrates into a single entity.

The **Forest of Confusion** is the woodland's outermost defense — a zone of deliberate disorientation maintained by both the root network's chemical signaling and the residual druidic quarantine wards from the Withered Depths containment (see `withered_depths_lore.md`).

### Lore-to-Zone Mapping

| Zone | Forest Aspect | Lore Context |
|------|--------------|--------------|
| 1: The Threshold | Outer defense — testing zone | Forest probes intruders with low-level threats; chemical signaling evaluates |
| 2: The Old Growth | Core territory — coordinated response | Creatures respond to root-network signals; ruins of pre-civilizational observatory |
| 3: The Deepwood | Active impediment — the forest reshapes itself | Trees shift, paths close, canopy tracks movement; branches to three boss zones |
| 4a: Rootmother's Sanctum | Growth aspect at maximum intensity | Root systems reshape terrain in real time |
| 4b: Blightwood Hollow | Decay aspect — waste processing | Accelerated decomposition; processes corrupted matter from Withered Depths boundary |
| 4c: Predator's Den | Predation aspect — apex killing ground | Forest's most dangerous creatures concentrated |

### Boss Lore Context

| Boss | Aspect | Ecological Role |
|------|--------|----------------|
| Rootmother (2800) | Growth | Barrier-builder — generates biomass wall against corruption; does not move, she grows |
| Blightwood Ancient (2801) | Decay | Waste-processor — breaks down corrupted matter, neutralizes crystal resonance |
| Thornfang Alpha (2802) | Predation | Last resort — strikes only when root network identifies threats lesser predators cannot handle |

### The Overgrown Ruins (Rooms 2781–2788)

The ruins in Zone 2 are the only evidence of permanent human habitation in the Verdant Depths. Likely built by predecessors of the **Everkeepers** (see `great_northern_forest_lore.md`) as a seasonal observatory. Key archaeological details for room/object writing:

- **Lantern-and-scale motif** on a broken lintel (room 2782) — same symbol found on Lantern Road waystones, but apparently older than both cities. Lore hook: is the symbol pre-civilizational?
- **Seven-pace planting rings** around the perimeter — matching Everkeeper tradition.
- **Single eye motif** on the well capstone (room 2786) — matches Midgaard's Watchers' Cairn fragments (see `midgaard_lore.md`, section 0a). Key cross-area connection.
- Trees growing in original column spacing (room 2788) — the forest has incorporated the ruins into its root network.

### Cross-Area Lore Integration

Embed cross-area references through room descriptions, mob barks, object flavor text, and extra descriptions:

**Withered Depths conflict** (embed in boundary zones and Zone 4b):
- Evidence of corrupted organisms being processed by the decay aspect
- Thickened bark and intensified growth at the corruption boundary
- Predator patrols redirected toward Withered Depths incursions
- The Blightwood Ancient actively neutralizing crystal-contaminated matter

**Great Northern Forest connections** (embed in Zones 1–2):
- Everkeeper planting-ring remnants establishing shared tradition
- Red moss bloom warnings echoing the northern forest's corruption indicators
- Root-network signaling detectable at the northern forest's fringes (volatile compounds)
- White-stag omen sightings in outer zones

**Pre-civilizational archaeology** (embed in ruins, rooms 2781–2788):
- Eye motif linking to Midgaard's Watchers' Cairn
- Lantern-and-scale symbol predating both cities
- Stone construction techniques matching Cairn-Keeper megalith traditions
- Connection to the continent-spanning mystery of pre-civilizational peoples

**Kiess staging base** (for connected-area NPC flavor):
- Scout debriefing reports describing coordinated predator behavior
- Wall Command warnings about deep-forest zones on north-gate bulletin boards
- Herbal reagents and rare timbers flowing through forest routes into Kiess markets
- Temple of Concord sermons drawing parallels between Conclave failure and institutional overreach

### Material Culture Quick Reference

**Natural motifs:** knotted root systems forming walls and corridors, canopy filtering light into green-gold columns, amber sap pooling in natural depressions, fungal shelves in concentric rings, moss carpeting below knee height, animal territorial signs.

**Ruin motifs:** dry-stacked stone split by roots, column bases converted to root supports, functional deep wells, carved symbols half-obscured by lichen, courtyard spaces converted to tree groves.

**Sounds:** deep wood creaking, leaf rustle without wind, predator calls, underground root movement, sap dripping, rhythmic insect chorus, the occupied silence of a space fully alive.

**Smells:** rich mulch, green sap, thick pollen, productive decomposition (healthy compost, not rot), animal musk, deep well-water minerals, faint chemical sharpness of root signaling.

**Visual textures:** bark from silver to near-black, root surfaces worn smooth by water, bioluminescent moss in sheltered hollows, geometric spider silk, visible vine redirection in response to disturbance.

## Area Header

The `#AREA` section lives in `area/verdantgreen.are`. Directive notes:

- `Q 16` -- mandatory version; any other value is out of spec
- `K verdantdepths~` -- single-word keyword, no spaces, no color codes
- `L @@W(@@r75 95@@W)@@N~` -- white parens, light-green level numbers, white close paren, reset
- `N 74` -- confirmed unused across all 57 loaded area files
- `I 75 95` -- controls mob-level gating and area matching logic
- `V 3000 3149` -- full 125-vnum envelope; all rooms, mobs, and objects must stay within this range
- `F 15` -- reset frequency in minutes, matching the Forest of Confusion parent area
- `U` -- reset message uses primary gray `@@g` with `@@N` reset; no double-newlines, terminated with `~`
- `T` -- rest of line is ignored by the loader; presence of the line enables teleport
- `O Virant~` -- area owner must always be Virant
- `R`, `W` -- ACL directives omitted; set at implementation time by the builder

---

## Room Layout (125 rooms, vnums 3000-3149)

### Zone 1: The Threshold (vnums 2739-2758, 20 rooms)

The transitional zone between the Forest of Confusion and the deeper woods. Trees grow impossibly tall, light fades, and the air thickens with spores.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2739 | Entrance to the Verdant Depths | forest | 0 | Entry room; connects back to Forest of Confusion |
| 2740 | A Darkening Trail | forest | dark | Path south from entrance |
| 2741 | Beneath Towering Oaks | forest | 0 | |
| 2742 | A Moss-Covered Clearing | forest | 0 | Small clearing |
| 2743 | Among Gnarled Roots | forest | dark | Roots obstruct movement |
| 2744 | A Fungal Hollow | forest | 0 | Glowing mushrooms |
| 2745 | The Spore Thicket | forest | 0 | Dense spore clouds |
| 2746 | A Rotting Log Bridge | forest | 0 | Crosses a ravine |
| 2747 | Beneath a Canopy of Vines | forest | 0 | |
| 2748 | A Sunken Trail | forest | dark | Trail drops lower |
| 2749 | The Whispering Ferns | forest | 0 | |
| 2750 | A Thicket of Thorns | forest | 0 | |
| 2751 | Beside a Stagnant Pool | forest | 0 | |
| 2752 | A Tangle of Briars | forest | 0 | |
| 2753 | The Old Growth Border | forest | 0 | Transition to Zone 2 |
| 2754 | A Narrow Game Trail | forest | 0 | |
| 2755 | Among Fallen Trunks | forest | 0 | |
| 2756 | A Damp Hollow | forest | dark | |
| 2757 | Beneath Dripping Boughs | forest | 0 | |
| 2758 | The Edge of the Old Growth | forest | 0 | Gateway to Zone 2 |

**Connectivity:** Mostly linear with branches. Entry at 2739 (north connects back to Forest of Confusion). Paths branch east/west with the main trunk running roughly south through to 2758.

### Zone 2: The Old Growth (vnums 2759-2793, 35 rooms)

The heart of the explorable forest. Massive ancient trees, overgrown ruins, and increasingly hostile wildlife. This is the main trash mob hunting ground.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2759 | The Old Growth | forest | 0 | Zone 2 hub |
| 2760 | A Cathedral of Trees | forest | 0 | Massive canopy overhead |
| 2761 | Among Ancient Trunks | forest | 0 | |
| 2762 | A Root-Woven Path | forest | 0 | |
| 2763 | The Verdant Corridor | forest | 0 | |
| 2764 | A Grove of Sentinel Oaks | forest | 0 | |
| 2765 | Beneath a Hollow Giant | forest | dark | Inside a massive dead tree |
| 2766 | A Carpet of Dead Leaves | forest | 0 | |
| 2767 | The Emerald Glade | forest | 0 | |
| 2768 | A Vine-Choked Passage | forest | 0 | |
| 2769 | Among Luminous Fungi | forest | dark | Bioluminescent |
| 2770 | A Sheltered Dell | forest | 0 | |
| 2771 | The Heartwood Trail | forest | 0 | |
| 2772 | Beside a Trickling Stream | forest | 0 | |
| 2773 | A Fern-Covered Slope | forest | 0 | |
| 2774 | The Twisted Arch | forest | 0 | Natural tree arch |
| 2775 | A Clearing of Stumps | forest | 0 | Old logging site reclaimed |
| 2776 | Among Creeping Ivy | forest | 0 | |
| 2777 | The Briar Labyrinth Edge | forest | 0 | |
| 2778 | A Gloomy Thicket | forest | dark | |
| 2779 | Beneath the Great Willow | forest | 0 | |
| 2780 | A Path of Stepping Stones | forest | 0 | Across boggy ground |
| 2781 | The Overgrown Ruins | forest | 0 | Ancient stone structures |
| 2782 | Among Crumbling Walls | forest | 0 | |
| 2783 | A Ruined Courtyard | forest | 0 | |
| 2784 | The Broken Tower Base | forest | 0 | |
| 2785 | A Root-Split Foundation | forest | 0 | |
| 2786 | Beside a Moss-Covered Well | forest | 0 | |
| 2787 | The Sunken Garden | forest | 0 | |
| 2788 | A Colonnade of Oaks | forest | 0 | Trees grew in old columns |
| 2789 | The Dense Underbrush | forest | 0 | |
| 2790 | A Lichen-Stained Path | forest | 0 | |
| 2791 | Among Toppled Stones | forest | 0 | |
| 2792 | The Ancient Boundary | forest | 0 | Transition deeper |
| 2793 | A Passage Through the Canopy Wall | forest | 0 | Gateway to Zone 3 |

**Connectivity:** Web-like with multiple paths through. Main corridors branch into sub-loops around the ruins (2781-2788). Multiple exits lead south/down into Zone 3.

### Zone 3: The Deepwood (vnums 2794-2828, 35 rooms)

The forest grows alien and hostile. Trees move when unobserved. The three boss zones branch off from this area.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2794 | The Deepwood | forest | 0 | Zone 3 hub |
| 2795 | A Living Corridor | forest | 0 | Trees form walls |
| 2796 | The Breathing Grove | forest | 0 | Trees visibly move |
| 2797 | Among Watchful Boughs | forest | dark | |
| 2798 | A Floor of Writhing Roots | forest | 0 | |
| 2799 | The Amber Sap Pools | forest | 0 | Pools of ancient sap |
| 2800 | Beneath Intertwined Canopy | forest | 0 | |
| 2801 | A Narrow Root Tunnel | forest | dark | Underground passage |
| 2802 | The Petrified Ring | forest | 0 | Circle of stone trees |
| 2803 | Among Thorned Sentinels | forest | 0 | |
| 2804 | A Bioluminescent Grotto | forest | dark | Glowing cavern |
| 2805 | The Weeping Hollow | forest | 0 | Sap drips constantly |
| 2806 | A Canopy Bridge | forest | 0 | High path between trees |
| 2807 | The Rotwood Stretch | forest | 0 | Decaying zone |
| 2808 | Among Bloated Toadstools | forest | 0 | |
| 2809 | A Clearing of Black Moss | forest | 0 | |
| 2810 | The Strangler Fig Passage | forest | 0 | |
| 2811 | Beneath a Web of Roots | forest | dark | |
| 2812 | The Verdant Maw | forest | 0 | Leads to boss 1 |
| 2813 | A Path of Bone-White Bark | forest | 0 | |
| 2814 | The Fungal Cathedral | forest | 0 | |
| 2815 | Among Pulsing Vines | forest | 0 | |
| 2816 | A Grove of Dead Saplings | forest | 0 | |
| 2817 | The Decaying Threshold | forest | 0 | Leads to boss 2 |
| 2818 | Beside a River of Sap | forest | 0 | |
| 2819 | A Carnivorous Garden | forest | 0 | |
| 2820 | The Predator's Trail | forest | 0 | |
| 2821 | Among Skeletal Branches | forest | dark | |
| 2822 | The Hunter's Blind | forest | 0 | Leads to boss 3 |
| 2823 | A Hollow of Whispering Leaves | forest | 0 | |
| 2824 | The Twisted Crossroads | forest | 0 | |
| 2825 | Beneath the Reaching Canopy | forest | 0 | |
| 2826 | A Sinkhole of Mulch | forest | 0 | |
| 2827 | The Groaning Path | forest | 0 | |
| 2828 | A Wall of Living Wood | forest | 0 | Dead-end feeling |

**Connectivity:** Branching layout. Three paths lead to the three boss sub-zones (Zone 4a/4b/4c). Multiple loops for hunting trash mobs.

### Zone 4a: The Rootmother's Sanctum (vnums 2829-2840, 12 rooms)

Growth-themed boss zone. Enter from 2812.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2829 | The Rootmother's Approach | forest | 0 | |
| 2830 | Among Prehensile Roots | forest | 0 | |
| 2831 | A Tunnel of Living Wood | forest | dark | |
| 2832 | The Root Nexus | forest | 0 | |
| 2833 | Beneath the Mother Tree | forest | 0 | |
| 2834 | A Chamber of Coiled Roots | forest | 0 | |
| 2835 | The Flowering Heart | forest | 0 | |
| 2836 | Among Creeping Tendrils | forest | 0 | |
| 2837 | The Nourishing Dark | forest | dark | |
| 2838 | A Bower of Ancient Growth | forest | 0 | |
| 2839 | The Rootmother's Cradle | forest | 0 | |
| 2840 | The Rootmother's Sanctum | forest | no_mob | **BOSS ROOM** - The Rootmother spawns here |

### Zone 4b: The Blightwood Hollow (vnums 2841-2851, 11 rooms)

Decay-themed boss zone. Enter from 2817.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2841 | The Blightwood Approach | forest | 0 | |
| 2842 | Among Rotting Trunks | forest | 0 | |
| 2843 | A Carpet of Fungal Growth | forest | dark | |
| 2844 | The Decomposing Glade | forest | 0 | |
| 2845 | Beneath Crumbling Bark | forest | 0 | |
| 2846 | A Miasma of Spores | forest | 0 | |
| 2847 | The Festering Pool | forest | 0 | |
| 2848 | Among Collapsing Canopy | forest | 0 | |
| 2849 | The Sporecrown Ring | forest | 0 | |
| 2850 | A Hollow of Black Mold | forest | dark | |
| 2851 | The Blightwood Throne | forest | no_mob | **BOSS ROOM** - The Blightwood Ancient spawns here |

### Zone 4c: The Predator's Den (vnums 2852-2863, 12 rooms)

Predation-themed boss zone. Enter from 2822.

| Vnum | Room Name | Sector | Flags | Notes |
|------|-----------|--------|-------|-------|
| 2852 | The Predator's Approach | forest | 0 | |
| 2853 | Among Trophy Bones | forest | 0 | |
| 2854 | A Trail of Claw Marks | forest | 0 | |
| 2855 | The Stalking Grounds | forest | 0 | |
| 2856 | Beneath a Canopy of Webs | forest | dark | |
| 2857 | A Den of Shed Scales | forest | 0 | |
| 2858 | The Feeding Ground | forest | 0 | |
| 2859 | Among Gnawed Remains | forest | 0 | |
| 2860 | The Broodmother's Passage | forest | 0 | |
| 2861 | A Nest of Matted Fur | forest | dark | |
| 2862 | The Kill Floor | forest | 0 | |
| 2863 | The Apex Predator's Lair | forest | no_mob | **BOSS ROOM** - Thornfang Alpha spawns here |

---

## Mobile Definitions

### Trash Mobs (levels 80-95)

All trash mobs: `act` flags include `is_npc|stay_area|aggressive`. Strong (non-boss) mobs include `solo`.

| Vnum | Name | Level | Act Flags | Notes |
|------|------|-------|-----------|-------|
| 2739 | a verdant stalker | 80 | is_npc, stay_area, aggressive | Camouflaged predator; Zones 1-2 |
| 2740 | a thornback briar beast | 82 | is_npc, stay_area, aggressive | Walking thorn bush; Zone 1-2 |
| 2741 | a canopy lurker | 84 | is_npc, stay_area, aggressive | Drops from above; Zone 2 |
| 2742 | an ancient treant | 86 | is_npc, stay_area, aggressive | Hostile tree creature; Zone 2 |
| 2743 | a sporecap shambler | 83 | is_npc, stay_area, aggressive | Fungal humanoid; Zone 2 |
| 2744 | a deepwood wolf | 85 | is_npc, stay_area, aggressive | Oversized wolf; Zone 2-3 |
| 2745 | a vine strangler | 87 | is_npc, stay_area, aggressive | Animated vine mass; Zone 2-3 |
| 2746 | a moss-covered golem | 88 | is_npc, stay_area, aggressive | Stone golem overgrown; Zone 2-3 |
| 2747 | a blighted dryad | 85 | is_npc, stay_area, aggressive | Corrupted forest spirit; Zone 3 |
| 2748 | a thornfang stalker | 90 | is_npc, stay_area, aggressive | Pack hunter; Zone 3 |
| 2749 | a rootweaver spider | 88 | is_npc, stay_area, aggressive | Giant spider; Zone 3 |
| 2750 | a petrified sentinel | 92 | is_npc, stay_area, aggressive, solo | Animated stone tree; Zone 3 |
| 2751 | a fungal abomination | 90 | is_npc, stay_area, aggressive | Massive mushroom creature; Zone 3 |
| 2752 | a heartwood guardian | 93 | is_npc, stay_area, aggressive, solo | Elite protector; Zone 4 approaches |
| 2753 | a rotwood shambler | 86 | is_npc, stay_area, aggressive | Decaying tree-creature; Zone 4b |
| 2754 | a sporecrown drone | 84 | is_npc, stay_area, aggressive | Fungal servant; Zone 4b |
| 2755 | a thornfang packmate | 89 | is_npc, stay_area, aggressive | Wolf creature; Zone 4c |
| 2756 | a root tendril mass | 87 | is_npc, stay_area, aggressive | Animated roots; Zone 4a |
| 2757 | a canopy weaver | 91 | is_npc, stay_area, aggressive, solo | Giant web-spinning beast; Zone 4c |
| 2758 | a verdant wraith | 95 | is_npc, stay_area, aggressive, solo | Forest ghost; rare spawn in Zone 3 |
| 2759 | a creeping shadow fox | 81 | is_npc, stay_area, aggressive | Dark-furred fox stalking the threshold; Zone 1 |
| 2760 | a tanglewood serpent | 83 | is_npc, stay_area, aggressive | Snake camouflaged in undergrowth; Zone 1-2 |
| 2761 | a fungal tick swarm | 80 | is_npc, stay_area, aggressive, no_body | Swarm of parasitic fungal ticks; Zone 1 |
| 2762 | a grasping bramble | 81 | is_npc, stay_area, aggressive | Animated bramble patch; Zone 1 |
| 2763 | a hollow tree lurker | 83 | is_npc, stay_area, aggressive | Creature nesting in dead trunks; Zone 1-2 |
| 2764 | a mosswort beetle | 80 | is_npc, stay_area, aggressive | Giant armored beetle; Zone 1 |
| 2765 | a deepwood bear | 88 | is_npc, stay_area, aggressive | Massive corrupted bear; Zone 2 |
| 2766 | a sap-blooded horror | 90 | is_npc, stay_area, aggressive | Ooze-like animated tree sap; Zone 2-3 |
| 2767 | a whispering shade | 86 | is_npc, stay_area, aggressive | Shadow creature between the trees; Zone 2 |
| 2768 | a bark-skinned ogre | 89 | is_npc, stay_area, aggressive | Forest ogre with bark-like hide; Zone 2-3 |
| 2769 | a thorn-crowned stag | 84 | is_npc, stay_area, aggressive | Corrupted great stag; Zone 2 |
| 2770 | a mycelium crawler | 82 | is_npc, stay_area, aggressive | Underground fungal burrower; Zone 3 |
| 2771 | a spore-maddened boar | 85 | is_npc, stay_area, aggressive | Infected wild boar; Zone 2-3 |
| 2772 | an elder vine serpent | 91 | is_npc, stay_area, aggressive, solo | Giant snake-like vine predator; Zone 3 |
| 2773 | a living deadfall | 87 | is_npc, stay_area, aggressive | Animated pile of fallen logs; Zone 3 |
| 2774 | a deepwood shaman | 93 | is_npc, stay_area, aggressive, solo | Corrupted humanoid caster; Zone 3 |
| 2775 | a rootwoven horror | 94 | is_npc, stay_area, aggressive, solo | Massive tangled root construct; Zone 3 |
| 2776 | a blightspore walker | 86 | is_npc, stay_area, aggressive | Fungal shambling zombie; Zone 4b |
| 2777 | a thornfang whelp | 82 | is_npc, stay_area, aggressive | Young predator beast; Zone 4c |
| 2778 | a rootbound guardian | 90 | is_npc, stay_area, aggressive | Living root sentinel; Zone 4a |
| 2779 | a canopy strangler | 88 | is_npc, stay_area, aggressive | Vine creature that drops from above; Zone 2-3 |
| 2780 | a withered dryad | 91 | is_npc, stay_area, aggressive, solo | Ancient decayed forest spirit; Zone 4b |
| 2781 | a mosswort queen | 85 | is_npc, stay_area, aggressive | Larger beetle matriarch; Zone 2 |
| 2782 | a verdant hulk | 92 | is_npc, stay_area, aggressive, solo | Massive plant-beast amalgamation; Zone 3 |
| 2783 | a rotting stag | 83 | is_npc, stay_area, aggressive, undead | Undead deer wreathed in fungus; Zone 2 |
| 2784 | a thorn wasp swarm | 81 | is_npc, stay_area, aggressive, no_body | Cloud of stinging thorn wasps; Zone 1-2 |
| 2785 | a barkskin wolf | 87 | is_npc, stay_area, aggressive | Wolf with bark-hardened hide; Zone 2-3 |
| 2786 | a deepwood centipede | 84 | is_npc, stay_area, aggressive | Giant venomous centipede; Zone 2 |
| 2787 | a bog troll | 89 | is_npc, stay_area, aggressive | Swamp troll covered in moss; Zone 2-3 |
| 2788 | a fungal broodmother | 93 | is_npc, stay_area, aggressive, solo | Giant fungal matriarch; Zone 4b |
| 2789 | an ancient moss elemental | 90 | is_npc, stay_area, aggressive | Elemental of living moss and earth; Zone 3 |
| 2790 | a twisted ent | 88 | is_npc, stay_area, aggressive | Corrupted tree creature; Zone 2-3 |
| 2791 | a thornveil assassin | 94 | is_npc, stay_area, aggressive, solo | Stealthy plant-like predator; Zone 3 |
| 2792 | a gloomwing moth | 82 | is_npc, stay_area, aggressive | Giant moth with toxic wing dust; Zone 1-2 |
| 2793 | a sap ooze | 85 | is_npc, stay_area, aggressive, no_body, no_mind | Animated blob of tree sap; Zone 2 |
| 2794 | a feral grove keeper | 91 | is_npc, stay_area, aggressive, solo | Corrupted nature guardian; Zone 3 |
| 2795 | a corpseflower lurker | 86 | is_npc, stay_area, aggressive | Carnivorous plant that feeds on the dead; Zone 4b |
| 2796 | a brambleback tortoise | 88 | is_npc, stay_area, aggressive | Huge tortoise with thorn-covered shell; Zone 2-3 |
| 2797 | a sporeveil phantom | 90 | is_npc, stay_area, aggressive | Ghostly shape in the spore clouds; Zone 3 |
| 2798 | a thornroot drake | 95 | is_npc, stay_area, aggressive, solo | Dragon-like plant creature; Zone 3 rare |
| 2799 | a deepwood prowler | 84 | is_npc, stay_area, aggressive | Large feline predator; Zone 2 |

**Mob combat extensions (`!` line):**
- Lower trash (80-86): 2-3 attacks, basic skills (punch, kick, dodge)
- Mid trash (87-90): 3-4 attacks, enhanced skills (headbutt, disarm, parry, dodge)
- Upper trash (91-95) and solo: 4-5 attacks, full combat skills (trip, counter, enhanced, parry, dodge)
- Caster mobs (blighted dryad, verdant wraith, deepwood shaman, withered dryad, feral grove keeper): cast bitvectors for nature-themed spells (flamestrike, earthquake, faerie_fire)
- Defensive mobs (moss-covered golem, petrified sentinel, rootbound guardian, brambleback tortoise, ancient moss elemental): def bitvectors (fireshield, iceshield)
- Poison mobs (rootweaver spider, fungal abomination, blightspore walker, mycelium crawler, deepwood centipede, gloomwing moth, corpseflower lurker): spec_poison

**Mob elemental extensions (`|` line):**
- Plant-type mobs (treant, vine strangler, grasping bramble, living deadfall, rootwoven horror, rootbound guardian, twisted ent, verdant hulk, thornveil assassin, corpseflower lurker): strong earth, weak fire, resist poison, suscept fire
- Fungal mobs (sporecap shambler, fungal abomination, mycelium crawler, blightspore walker, sporecrown drone, fungal broodmother, fungal tick swarm): strong poison, weak fire, resist earth, suscept fire
- Predator mobs (deepwood wolf, thornfang stalker/packmate/whelp, deepwood bear, spore-maddened boar, elder vine serpent, barkskin wolf, deepwood prowler, thornroot drake, creeping shadow fox, tanglewood serpent): strong physical, resist physical
- Spirit mobs (verdant wraith, blighted dryad, whispering shade, withered dryad, sporeveil phantom): strong mental, weak physical
- Insect mobs (mosswort beetle, mosswort queen, rootweaver spider, canopy weaver, thorn wasp swarm, deepwood centipede, gloomwing moth): strong earth, resist poison
- Construct mobs (moss-covered golem, petrified sentinel): strong earth, strong physical, resist physical, resist earth
- Ooze mobs (sap ooze, sap-blooded horror): strong poison, resist physical, suscept fire, no_mind
- Troll/Ogre mobs (bog troll, bark-skinned ogre): strong physical, resist earth

### Boss Mobs (level 100)

All boss mobs: `act` flags include `is_npc|sentinel|stay_area|boss|no_flee`. Placed only in `no_mob` rooms.

| Vnum | Name | Level | Room | Theme |
|------|------|-------|------|-------|
| 2800 | the Rootmother | 100 | 2840 | Growth boss -- massive living tree entity |
| 2801 | the Blightwood Ancient | 100 | 2851 | Decay boss -- ancient rotting mega-treant |
| 2802 | Thornfang Alpha | 100 | 2863 | Predation boss -- apex forest predator beast |

**Boss combat extensions:**
- All bosses: 5-6 attacks, enhanced, counter, parry, dodge, nodisarm, notrip
- Rootmother: cast earthquake + flamestrike; def heal + fireshield; strong earth, resist physical, suscept fire
- Blightwood Ancient: cast acid_blast + chain_light + faerie_fire; def heal + iceshield; strong poison, resist earth, suscept fire
- Thornfang Alpha: pure melee focused; 6 attacks, enhanced, counter, parry, dodge, kick, headbutt, martial; strong physical, resist physical, suscept mental

**Boss `+` extension mods:**
- All bosses get moderate spellpower, crit, parry, dodge, and block modifiers appropriate for level 100 encounters

---

## Object Definitions

### Trash Gear (levels 80-85, flagged `nodrop`)

`extra_flags` for all trash gear: `ITEM_NODROP` (128). All items include `take` (8388608) in wear_flags.

| Vnum | Name | Type | Wear | Level | Weight | Archetype | Notes |
|------|------|------|------|-------|--------|-----------|-------|
| 2739 | a thornback claw | weapon | hold, take | 80 | 6 | melee | slash; from thornback briar beasts |
| 2740 | a canopy lurker's fang | weapon | hold, take | 82 | 6 | melee | pierce; from canopy lurkers |
| 2741 | a treant bark shield | armor | hold, take | 83 | 11 | tank | shield; from ancient treants |
| 2742 | a sporecap cowl | armor | head, take | 80 | 1 | caster | head slot; from sporecap shamblers |
| 2743 | a wolf-hide mantle | armor | shoulders, take | 81 | 6 | melee | shoulders; from deepwood wolves |
| 2744 | a vine-woven tunic | armor | body, take | 84 | 11 | tank | body; from vine stranglers |
| 2745 | a moss golem's fist | weapon | hold, take, fist | 85 | 6 | melee | hit (fist weapon); from moss golems |
| 2746 | a blighted branch wand | weapon | hold, take, wand | 82 | 1 | caster | wand; from blighted dryads |
| 2747 | a thornfang pelt | armor | about, take | 83 | 6 | melee | about body; from thornfang stalkers |
| 2748 | a web-silk sash | armor | waist, take | 80 | 1 | caster | waist; from rootweaver spiders |
| 2749 | a petrified bark bracer | armor | wrist, take | 85 | 11 | tank | wrist; from petrified sentinels |
| 2750 | a fungal tendril whip | weapon | hold, take | 84 | 1 | caster | whip; from fungal abominations |
| 2751 | a heartwood splint | armor | legs, take | 85 | 11 | tank | legs; from heartwood guardians |
| 2752 | a rotwood club | weapon | hold, take | 81 | 6 | melee | pound; from rotwood shamblers |
| 2753 | a sporecrown circlet | armor | head, take | 82 | 1 | caster | head; from sporecrown drones |
| 2754 | a thornfang tooth necklace | armor | neck, take | 83 | 6 | melee | neck; from thornfang packmates |
| 2755 | a root tendril armband | armor | arms, take | 80 | 11 | tank | arms; from root tendril masses |
| 2756 | a canopy weaver's silk gloves | armor | hands, take | 84 | 1 | caster | hands; from canopy weavers |
| 2757 | a verdant wraith's shroud | armor | about, take | 85 | 1 | caster | about; from verdant wraiths |
| 2758 | a deepwood wolf's ear | treasure | take | 80 | 6 | melee | trophy/quest item |
| 2759 | a shadow fox pelt cloak | armor | about, take | 81 | 6 | melee | about; from creeping shadow foxes |
| 2760 | a tanglewood fang blade | weapon | hold, take | 83 | 6 | melee | stab; from tanglewood serpents |
| 2761 | a tick-chitin armguard | armor | wrist, take | 80 | 11 | tank | wrist; from fungal tick swarms |
| 2762 | a verdant hulk's knuckle | weapon | hold, take, fist | 85 | 11 | tank | hit (fist); from verdant hulks |
| 2763 | a rotting stag antler | weapon | hold, take | 82 | 6 | melee | stab; from rotting stags |
| 2764 | a thorn wasp wing mantle | armor | shoulders, take | 81 | 1 | caster | shoulders; from thorn wasp swarms |
| 2765 | a barkskin wolf pelt | armor | about, take | 84 | 6 | melee | about; from barkskin wolves |
| 2766 | a centipede fang earring | armor | ear, take | 82 | 1 | caster | ear; from deepwood centipedes |
| 2767 | a bog troll's hide belt | armor | waist, take | 85 | 11 | tank | waist; from bog trolls |
| 2768 | a broodmother's spore sac | armor | hold, take | 85 | 1 | caster | held; from fungal broodmothers |
| 2769 | a moss elemental's core | treasure | take | 84 | 11 | tank | trophy; from ancient moss elementals |
| 2770 | a twisted ent branch | weapon | hold, take | 84 | 6 | melee | pound; from twisted ents |
| 2771 | a thornveil assassin's cowl | armor | head, take | 85 | 1 | caster | head; from thornveil assassins |
| 2772 | a gloomwing dust pouch | armor | hold, take | 80 | 1 | caster | held; from gloomwing moths |
| 2773 | a brambleback shell plate | armor | body, take | 85 | 11 | tank | body; from brambleback tortoises |
| 2774 | a bramble-thorn dagger | weapon | hold, take | 81 | 6 | melee | stab; from grasping brambles |
| 2775 | a beetle-shell pauldron | armor | shoulders, take | 80 | 11 | tank | shoulders; from mosswort beetles |
| 2776 | a deepwood bear paw | weapon | hold, take, fist | 83 | 6 | melee | hit (fist); from deepwood bears |
| 2777 | a shade-touched ring | armor | finger, take | 82 | 1 | caster | finger; from whispering shades |
| 2778 | a bark-skinned armguard | armor | wrist, take | 84 | 11 | tank | wrist; from bark-skinned ogres |
| 2779 | a crown of twisted antlers | armor | horns, take | 83 | 6 | melee | horns; from thorn-crowned stags |
| 2780 | a mycelium-threaded robe | armor | body, take | 82 | 1 | caster | body; from mycelium crawlers |
| 2781 | a boar-tusk earring | armor | ear, take | 80 | 6 | melee | ear; from spore-maddened boars |
| 2782 | an elder vine lash | weapon | hold, take | 85 | 1 | caster | whip; from elder vine serpents |
| 2783 | a deadfall bark vest | armor | body, take | 84 | 11 | tank | body; from living deadfalls |
| 2784 | a shaman's gnarled staff | weapon (two-handed) | hold, take | 85 | 1 | caster | pound; from deepwood shamans |
| 2785 | rootwoven greaves | armor | legs, take | 85 | 6 | melee | legs; from rootwoven horrors |
| 2786 | a blightspore mask | armor | face, take | 81 | 1 | caster | face; from blightspore walkers |
| 2787 | a thornfang whelp's collar | armor | neck, take | 80 | 6 | melee | neck; from thornfang whelps |
| 2788 | a rootbound gauntlet | armor | hands, take | 84 | 11 | tank | hands; from rootbound guardians |
| 2789 | a sap-crusted hauberk | armor | body, take | 85 | 11 | tank | body; from sap-blooded horrors |
| 2790 | a strangler vine belt | armor | waist, take | 82 | 6 | melee | waist; from canopy stranglers |
| 2791 | a withered dryad's circlet | armor | head, take | 84 | 1 | caster | head; from withered dryads |
| 2792 | a mosswort queen's mandible | weapon | hold, take | 83 | 6 | melee | slash; from mosswort queens |
| 2793 | a hollow lurker's hide boots | armor | feet, take | 81 | 6 | melee | feet; from hollow tree lurkers |
| 2794 | a stalker's vine garrote | weapon | hold, take | 80 | 1 | caster | crush; from verdant stalkers |
| 2795 | a beetle-shell buckler | armor (buckler) | hold, take | 83 | 11 | tank | buckler; from mosswort beetles |
| 2796 | an ogre-hewn great club | weapon (two-handed) | hold, take | 85 | 6 | melee | crush; from bark-skinned ogres |
| 2797 | a sap-hardened ring | armor | finger, take | 84 | 11 | tank | finger; from sap-blooded horrors |
| 2798 | a bear-claw amulet | armor | neck, take | 82 | 6 | melee | neck; from deepwood bears |

**Trash gear affects (`A` lines):**
- Weapons: +hit and +damage appropriate for level range
- Armor: +ac, +hp, or +mana depending on archetype (caster=mana, melee=hit/dam, tank=ac/hp)

### Boss Gear (level 90, flagged `nodrop` + `boss`)

`extra_flags` for all boss gear: `ITEM_NODROP` (128) | `ITEM_BOSS` -- note: ITEM_BOSS does not exist as a named constant in the spec; use the `boss` bit if available, otherwise this needs to be verified against `extra_flags` definitions. Per the spec, `boss` is an `act` flag on mobs (67108864). For items, the user's intent is that boss drops are distinctly flagged. **Resolution:** Use `ITEM_UNIQUE` (33554432) combined with `ITEM_NODROP` (128) for boss items unless a dedicated ITEM_BOSS extra flag is confirmed. The user specified "flagged as boss" -- if `boss` maps to a valid item extra flag, use it; otherwise, clarify with the user.

**Confirmed from spec and source:** `ITEM_BOSS` = `134217728`, `ITEM_NODROP` = `128`, `ITEM_LOOT` = `67108864`. Boss gear `extra_flags` = `ITEM_NODROP | ITEM_BOSS` = `134217856`. Loot-table items additionally need `ITEM_LOOT` = `134217984` total.

| Vnum | Name | Type | Wear | Level | Weight | Archetype | Boss Source |
|------|------|------|------|-------|--------|-----------|-------------|
| 2799 | the Rootmother's living staff | weapon (two-handed) | hold, take | 90 | 1 | caster | Rootmother |
| 2800 | a crown of living thorns | armor | head, take | 90 | 1 | caster | Rootmother |
| 2801 | the Rootmother's heartwood buckler | armor (buckler) | hold, take | 90 | 11 | tank | Rootmother |
| 2802 | greaves of ancient bark | armor | legs, take | 90 | 11 | tank | Rootmother |
| 2803 | a rootweave tunic | armor | body, take | 90 | 6 | melee | Rootmother |
| 2804 | the Rootmother's seed | armor | hold, take | 90 | 1 | caster | Rootmother; held orb-like item |
| 2805 | living bark vambraces | armor | wrist, take | 90 | 11 | tank | Rootmother |
| 2806 | a blade of blighted wood | weapon | hold, take | 90 | 6 | melee | Blightwood Ancient |
| 2807 | a sporecrown helm | armor | head, take | 90 | 11 | tank | Blightwood Ancient |
| 2808 | a cloak of rotting leaves | armor | about, take | 90 | 1 | caster | Blightwood Ancient |
| 2809 | blightwood gauntlets | armor | hands, take | 90 | 6 | melee | Blightwood Ancient |
| 2810 | the Blightwood Ancient's fungal shield | armor | hold, take | 90 | 11 | tank | Blightwood Ancient |
| 2811 | a blightwood ring of decay | armor | finger, take | 90 | 1 | caster | Blightwood Ancient |
| 2812 | a sporewoven sash | armor | waist, take | 90 | 6 | melee | Blightwood Ancient |
| 2813 | Thornfang Alpha's great claw | weapon (two-handed) | hold, take | 90 | 6 | melee | Thornfang Alpha |
| 2814 | a collar of alpha fangs | armor | neck, take | 90 | 6 | melee | Thornfang Alpha |
| 2815 | a pelt of the apex predator | armor | body, take | 90 | 11 | tank | Thornfang Alpha |
| 2816 | the predator's eye pendant | armor | neck, take | 90 | 1 | caster | Thornfang Alpha |
| 2817 | thornfang hide boots | armor | feet, take | 90 | 6 | melee | Thornfang Alpha |
| 2818 | the predator's maw wand | weapon (wand) | hold, take | 90 | 1 | caster | Thornfang Alpha; wand |
| 2819 | thornfang alpha's shoulder hide | armor | shoulders, take | 90 | 11 | tank | Thornfang Alpha |

**Boss gear affects (`A` lines):**
- Boss weapons: stronger +hit and +damage than trash equivalents
- Boss armor: stronger +ac, +hp, or +mana; some pieces with item_apply effects (e.g., enhanced, det_invis)
- Rootmother drops 7 items (vnums 2799-2805): 2 caster, 3 tank, 2 melee
- Blightwood Ancient drops 7 items (vnums 2806-2812): 2 caster, 2 tank, 3 melee
- Thornfang Alpha drops 7 items (vnums 2813-2819): 2 caster, 2 tank, 3 melee

### Delivery Method

- **Trash gear:** Equipped on mobs via `E` resets or placed in mob inventory via `G` resets. Some can also use loot tables (`l`/`L` extensions) with `ITEM_LOOT` flag added.
- **Boss gear:** Equipped on boss mobs via `E` resets and/or in boss loot tables. All boss items carry `ITEM_BOSS` extra flag per spec policy.

---

## Resets Plan

### Mob Resets (`M` commands)

**Zone 1 (2739-2758):** ~12-15 trash mob spawns
- verdant stalkers, thornback briar beasts, grasping brambles, mosswort beetles, hollow tree lurkers
- Lower-level mobs (80-84) for zone entry
- Limit of 1-2 per spawn point

**Zone 2 (2759-2793):** ~25-30 trash mob spawns
- Full range of mid-level trash: canopy lurkers, ancient treants, sporecap shamblers, deepwood wolves, vine stranglers, deepwood bears, thorn-crowned stags, bark-skinned ogres, whispering shades, mosswort queens, spore-maddened boars, canopy stranglers
- Heavier density; this is the primary farming zone
- Limits of 1-3 per spawn point

**Zone 3 (2794-2828):** ~20-25 trash mob spawns
- Higher-level trash: thornfang stalkers, rootweaver spiders, petrified sentinels, fungal abominations, sap-blooded horrors, elder vine serpents, living deadfalls, deepwood shamans, rootwoven horrors
- Solo-flagged mobs more common here
- Include verdant wraiths as rare encounters

**Zone 4a (2829-2840):** ~6-8 trash mob spawns + 1 boss
- root tendril masses, rootbound guardians, heartwood guardians as guards
- M 0 2800 1 2840 -- Rootmother boss (limit 1, in no_mob room)

**Zone 4b (2841-2851):** ~6-8 trash mob spawns + 1 boss
- rotwood shamblers, sporecrown drones, blightspore walkers, withered dryads, fungal broodmothers, corpseflower lurkers
- M 0 2801 1 2851 -- Blightwood Ancient boss (limit 1, in no_mob room)

**Zone 4c (2852-2863):** ~6-8 trash mob spawns + 1 boss
- thornfang packmates, thornfang whelps, canopy weavers
- M 0 2802 1 2863 -- Thornfang Alpha boss (limit 1, in no_mob room)

### Equipment Resets (`E`/`G` commands)

- Each trash mob type has 1-2 pieces of thematically matched equipment via `E` resets
- Each boss has 4-5 pieces of equipment via `E` resets (weapons wielded/held, armor equipped)
- Remaining boss items (2-3 per boss) via loot table (`l`/`L` mob extensions) with `ITEM_LOOT` added to those items' extra_flags

### Object Resets (`O` commands)

- No standalone room object spawns planned (all gear delivered through mobs)

---

## Connection to Forest of Confusion

Two bidirectional links connect the areas, giving players a surface entrance and an underground shortcut.

### Link 1: Surface Entrance (Primary)

**Room 2434** ("Dense Woodland") is a dead-end in the forest's dense woodland zone with only a D3 (west) exit to 2433. This is the natural "deeper into the forest" transition point.

- **confusn.are edit:** Add D1 (east) exit to room 2434, destination vnum 2739
- **verdant_depths.are:** Room 2739 (Entrance to the Verdant Depths) has D3 (west) exit back to 2434

| Direction | From | To | Description |
|-----------|------|----|-------------|
| east | 2434 (Dense Woodland) | 2739 (Entrance to the Verdant Depths) | Into the new area |
| west | 2739 (Entrance to the Verdant Depths) | 2434 (Dense Woodland) | Back to Forest of Confusion |

### Link 2: Underground Passage (Secondary)

**Room 2509** ("In a Maze of Caves") is the deepest dead-end in the cave maze with only a D0 (north) exit to 2503. This connects to the Deepwood zone underground, providing an alternate route for players who explored the caves.

- **confusn.are edit:** Add D2 (south) exit to room 2509, destination vnum 2801
- **verdant_depths.are:** Room 2801 (A Narrow Root Tunnel) has D0 (north) exit back to 2509

| Direction | From | To | Description |
|-----------|------|----|-------------|
| south | 2509 (In a Maze of Caves) | 2801 (A Narrow Root Tunnel) | Underground shortcut to Zone 3 |
| north | 2801 (A Narrow Root Tunnel) | 2509 (In a Maze of Caves) | Back to cave maze |

### Why These Rooms

- **2434** is a surface forest dead-end deep in the woodland -- thematically perfect for "the forest gets darker and deeper." Players following the main overland path naturally discover the primary entrance.
- **2509** is the deepest cave dead-end in the entire maze network -- a hidden underground passage rewards exploration of the caves with a shortcut directly into Zone 3 (The Deepwood), bypassing Zones 1-2.
- **2508** (A Small Cavern) was considered but rejected: it has zero exits and is completely unreachable in the current area. It appears to be a stranded/orphaned room.

### Required Edits to confusn.are

1. Room 2434: Add `D1` exit block (east to 2739)
2. Room 2509: Add `D2` exit block (south to 2801)

---

## Specials Plan

| Mob Vnum | Spec Function | Reason |
|----------|--------------|--------|
| 2747 | spec_cast_mage | Blighted dryad -- nature magic caster |
| 2758 | spec_cast_mage | Verdant wraith -- ghostly caster |
| 2774 | spec_cast_mage | Deepwood shaman -- corrupted caster |
| 2780 | spec_cast_mage | Withered dryad -- decay magic caster |
| 2794 | spec_cast_mage | Feral grove keeper -- corrupted guardian caster |
| 2800 | spec_cast_cleric | Rootmother boss -- healing/nature |
| 2801 | spec_cast_mage | Blightwood Ancient boss -- decay magic |
| 2749 | spec_poison | Rootweaver spider -- venomous |
| 2751 | spec_poison | Fungal abomination -- toxic |
| 2770 | spec_poison | Mycelium crawler -- toxic spores |
| 2776 | spec_poison | Blightspore walker -- fungal toxin |
| 2786 | spec_poison | Deepwood centipede -- venomous bite |
| 2792 | spec_poison | Gloomwing moth -- toxic wing dust |
| 2795 | spec_poison | Corpseflower lurker -- poisonous pollen |

---

## Implementation Checklist

1. [x] Create `area/verdantgreen.are` -- file exists with `#AREA` header and `#$`
3. [ ] Write `#MOBILES` section into `verdantgreen.are`
4. [ ] Write `#MOBILES` section (61 trash mobs + 3 bosses = 64 mobs, vnums 2739-2802)
5. [ ] Write `#OBJECTS` section (60 trash items + 21 boss items = 81 objects, vnums 2739-2819)
6. [ ] Write `#ROOMS` section (125 rooms, vnums 3000-3149, all used)
7. [ ] Write `#RESETS` section (~80-100 mob spawns with equipment)
8. [ ] Write `#SPECIALS` section
9. [ ] Add `D1` (east) exit to room 2434 in `confusn.are` pointing to 2739
10. [ ] Add `D2` (south) exit to room 2509 in `confusn.are` pointing to 2801
11. [ ] Add `verdant_depths.are` to `area/area.lst`
11. [ ] Run `cd src && make unit-tests` to validate
12. [ ] Verify all vnums within 3000-3149 range
13. [ ] Verify no vnum conflicts with existing areas
