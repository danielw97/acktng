# Area Plan: The Sunken Sanctum (Expansion)

## Overview
- **Area File:** `sunken_sanctum.are`
- **Area Name:** `@@BThe Sunken Sanctum@@N`
- **Level Range Target:** `150-170`
- **Vnum Envelope (unchanged):** `9500-9599`
- **Room Vnums Used in this plan:** **all 100 room vnums** (`9500-9599`)
- **Layout Style:** Irregular, fractured dungeon with loops, collapses, vertical drops, and offset wings.

## Color Theme (multi-color, lore-aligned)
- **Ancient arcane masonry:** `@@B` (blue), `@@W` (white)
- **Ash/fire aftermath:** `@@R` (red), `@@b` (brown), `@@d` (dark grey)
- **Crystal/laboratory energy:** `@@c` (cyan), `@@l` (light blue)
- **Spiritual/undead residue:** `@@a` (light cyan), `@@m` (magenta)
- **Corruption/shadow pockets:** `@@p` (purple), `@@g` (gray)
- **Reset:** `@@N` at end of colorized strings.

> Theme policy compliance: no `@@k` black foreground, no background codes (`@@0`-`@@7`), no flashing (`@@f`). All colorized strings terminated with `@@N`.

## #AREA Header Plan
- `Q 16` — mandatory version; any other value is out of spec
- `K sunkensanctum~` — single-word keyword, no spaces, no color codes
- `L @@W(@@B150 170@@W)@@N~` — white parens, blue level numbers, white close paren, reset
- `N <allocate at implementation time>` — area number, confirmed unused across loaded area files
- `I 150 170` — endgame dungeon; controls mob-level gating and area matching logic
- `V 9500 9599` — full 100-vnum envelope; all rooms, mobs, and objects must stay within this range
- `F 20` — reset frequency in minutes (longer reset for endgame dungeon pacing)
- `U @@Ba cold wind stirs through the sunken halls and ancient dust drifts from cracked stone@@N~` — reset message; blue text, reset; no double-newlines, terminated with `~`
- `O Virant~` — area owner must always be Virant
- `R`, `W` — ACL directives omitted; set at implementation time by the builder
- Optional builder directives (`X`, `T`, `B`, `S`, `M`) set during implementation.

## Lore Direction (extensive)
The Sunken Sanctum was not merely a mage outpost; it was the hidden proving-ground of a mountain order called the **Spirebound Conclave**, an inner circle that tried to bind three forces into a stable doctrine: flame (transformation), crystal (preservation), and spirit (memory). Their public doctrine promised transcendence. Their sealed archives reveal a different truth: they were building immortal custodians from unwilling initiates.

At first, the Conclave engineered obedient defenders (the Flamebound Sentinels), then adaptive pursuit organisms (Shadow Stalkers and Gravelurkers), and finally memory-bound scholars (Spirit Mages). Each success demanded stronger ritual anchors and harsher sacrifice. The process culminated in the Vaultkeeper project: a fused sentinel-intellect housed in iron and ossified ritual bone, bound to protect a reliquary that could "stabilize continuity" after catastrophe.

The catastrophe came anyway. A mountain tremor ruptured containment circles at the same moment an internal faction sabotaged the core ritematrix. The upper sanctum burned while lower galleries flooded with alchemical runoff and glacial seep. Crystal growth surged through corridors, preserving some chambers in cold stasis while reducing others to warped slag and ash. Trapped archivists attempted to encode warnings into riddles and hidden passages; one of these is the famous mountain-riddle test.

Over centuries, the sunken complex became stratified by surviving systems:
- **Outer ruins** held by ashborn remnants and failed guardians.
- **Experiment galleries** where preserved failures still twitch in broken vitrines.
- **Crystal engine corridors** where static-charged weavers patrol old conduits.
- **Archive and spirit layers** where ghost-scholars replay unfinished catalogues.
- **Vault approaches** gated by legacy rune-locks and challenge constructs.
- **Abyssal undercroft** below the known vault, where floodwater, cave-ins, and neglected ritual machinery conceal the Conclave's final records.

The plan preserves all existing iconic beats (collapsed entry, shattered hall, armoury, sentinels' lock, riddlegolem, secret passage, archives, antechamber, inner vault) and expands them into a full 100-room endgame arc. Narrative progression is investigative: players move from visible ruin to concealed intent, learning that the sanctum did not simply fall -- it was deliberately buried with surviving systems still active.

### Lore Expansion: Geographic and Historical Context

#### The Cinderteeth Connection
The Sunken Sanctum occupies a deep fold in the western spur of the **Cinderteeth Mountains**, the same volcanic range whose eruptions would later produce the **Ashfall Monsoon** that devastated Kowloon's delta three centuries after the sanctum's fall (see `docs/kowloon_plan.md`). The Conclave chose this site deliberately: the Cinderteeth's geothermal activity provided natural heat for forges and alchemical furnaces, while the range's remoteness offered insulation from lowland politics. Volcanic glass from the peaks was ground into lens components for the crystal conduit system, and the mountain's iron-rich springs fed the sanctum's water supply and ritual pools.

The Conclave's own records note periodic tremors and sulfurous venting from deeper vents, which they monitored through a network of resonance crystals embedded in the mountain's fault lines. These early seismic instruments were among the Conclave's genuine contributions to practical scholarship -- and their failure during the catastrophe was one of the cruelest ironies of the sanctum's fall. The same geothermal instability that made the site useful made it lethal when containment systems failed.

Modern geographers tracing the Cinderteeth's eruptive history now suspect that the seismic event that triggered the sanctum's collapse was an early precursor of the same volcanic cycle that would later produce the Ashfall Monsoon. If so, the Conclave's catastrophe and Kowloon's black rain share a common geological cause separated by centuries -- the mountain's slow exhalation of fire, first destroying the scholars who studied it, then the delta peoples who had never heard of them.

### Lore Expansion: Founding, Schism, and Burial

#### 1) Founding Era -- The Promise of Continuance
The Spirebound Conclave began as a sanctioned mountain academy tasked with preserving dangerous magical knowledge that lowland courts feared and coveted in equal measure. Its charter was defensive: contain unstable arts, catalogue them, and release only what could be governed. The first masters believed memory itself was civilization's true wall against collapse; from this belief came the doctrine of **Continuance** -- the conviction that no discovery, oath, or lesson should die with its keeper.

The early sanctum was elegant and austere. Bells marked every watch, apprentices copied ledgers by candlelight, and tribunal mages debated ethics in vaulted halls now broken and ash-streaked. At this stage, the Conclave's experiments were mostly inert: runic storage tablets, resonance crystals, and mnemonic techniques that allowed witnesses to imprint testimony into spirit-ink archives. Those efforts were celebrated as a triumph of scholarship.

#### 2) The Turning -- From Archive to Instrument
Everything changed when border wars severed supply routes and killed much of the academy's outer guard. These were not distant conflicts: the wars that isolated the Conclave were part of the same cycle of delta feuds and territorial fragmentation that wracked the lowlands before any city imposed lasting order. In Kowloon's delta, clan skiffs fought over grain bridges and canal tolls (see `docs/kowloon_plan.md`, "Before the Walls: Delta of Lanterns"). In Evermeet's western approaches, the outer districts that would eventually collapse into the founding crisis of Kiess were already bleeding soldiers and supply lines to frontier defense (see `docs/kiess_plan.md`, "The Fall of Evermeet"). In the Cinderteeth foothills, the same pattern held: petty warlords and desperate magistrates warred over trade routes, mountain passes, and timber rights, each conflict cutting another supply line to the sanctum.

The Conclave's charter had assumed that lowland courts would continue to fund and protect the academy in exchange for controlled access to its research. When those courts turned on each other, the funding stopped and the protection evaporated. Caravans that once carried reagents, food, and prisoner transfers up the mountain roads were ambushed or simply stopped coming. The academy's outer guard -- a small garrison of hired soldiers -- was killed in a series of skirmishes with mountain brigands who had once been kept at bay by regional patrols that no longer existed.

Isolated and under siege, the Conclave reframed Continuance from a scholarly ideal into a survival engine. "Preservation" began to include bodies, not only words. Volunteers were sought, then expected, then quietly replaced by condemned debtors and political prisoners transferred under sealed warrants. Some of these prisoners came from the same delta and frontier conflicts that had severed the supply routes -- war captives, debtors from collapsed trade houses, and political dissidents from courts that no longer existed by the time their sentences were carried out. The sealed warrants that authorized their transfer were countersigned by magistrates from proto-civic institutions that would eventually evolve into Midgaard's guild courts and the precursors of Kowloon's Jade Magistracy -- a detail that, if ever uncovered, would embarrass the modern successors of those institutions considerably.

This period created the sanctum's familiar horrors:
- **Flamebound Sentinels** were originally wounded veterans offered "rebirth" in fire-etched armor, their pain transmuted into obedience sigils.
- **Shadow Stalkers** began as scout prototypes intended to patrol collapsed passages, but their predatory adaptation loops made them difficult to leash.
- **Gravelurkers** were failed reinforcement subjects from subterranean traversal research, left partially calcified and half-feral.
- **Spirit Mages** were not true mages at all in many cases, but mnemonic echoes chained to duties they can no longer complete.

The Conclave's records from this era grow euphemistic: "binding" instead of coercion, "elevation" instead of vivisection, "custody" instead of imprisonment. By the time the language changed, the institution already had.

#### 3) The Schism -- Three Laws, One Betrayal
Internal factions emerged around the three sanctum pillars:
- **The Ember Canon** argued that will is forged through suffering and should be tempered by fire.
- **The Prism Thesis** claimed structure and memory could be stabilized through crystal harmonics.
- **The Pale Ledger** insisted spirit imprinting was the only moral route, since bodies failed but testimony endured.

These factions were never formally at war, yet each sabotaged the others' proofs, altered findings, and manipulated trial assignments. The Riddlegolem was commissioned in this climate -- not merely as a guardian puzzle, but as a political gate. Only those trained in sanctioned doctrine could pass to the deeper records. In effect, knowledge became territorial.

The eventual betrayal was procedural rather than theatrical: an authorized change order in the conduit matrix, countersigned by rival offices, introduced contradictory lock states during a seismic event. When the mountain shook, emergency seals competed with one another. Fire wards trapped archivists, floodgates opened into occupied work galleries, and crystal relays over-amplified into lethal static bloom.

#### 4) The Catastrophe -- One Night, Three Disasters
The sanctum's fall was a cascade:
1. **Upper firestorm:** armouries and barracks ignited; molten fittings fused doors shut.
2. **Mid-level resonance break:** crystal conduits fractured, spawning uncontrolled growth and electrical arcing.
3. **Lower flood inversion:** undercroft pumps failed, then reversed, drawing runoff and glacial melt into ritual basements.

Survivors attempted three containment responses that now define major encounter arcs:
- They bound elite constructs to fixed posts (origin of the Vaultkeeper's final charge).
- They encoded pass-phrases and riddles in floor-runes to prevent blind entry.
- They hid portable authority tokens (glyph-stones, keys, idol forms) in side chambers, assuming future recovery teams would piece them together.

No recovery team arrived. Regional powers declared the mountain cursed and deliberately collapsed upper approaches. The sanctum was not just lost; it was entombed by policy.

### Lore Expansion: The Conclave's Shadow on the Withered Depths

The Conclave's experiments did not die with the sanctum. Before the catastrophe, the Prism Thesis faction conducted field trials in remote forests south of the Cinderteeth range, testing whether crystal harmonics could be used to bind natural spirits to fixed locations as custodial anchors. These trials involved implanting resonance crystals in old-growth root systems and attempting to merge living plant matter with runic preservation matrices -- essentially the same technique used on human subjects inside the sanctum, but applied to forest ecosystems.

The trial sites were abandoned when the supply routes collapsed, but the implanted crystals were never retrieved. Over centuries, the unattended resonance matrices continued to pulse, drawing ambient magical energy into feedback loops that warped the surrounding ecology. Root systems grew unnaturally deep and tangled. Fungal colonies mutated into aggressive, semi-sentient networks. The boundary between living wood and dead matter blurred as preservation magic intended for human tissues acted on plant and fungal matter instead.

Scholars who have studied both the Sunken Sanctum's surviving records and the corruption in **the Withered Depths** (see `docs/withered_depths_plan.md`) note disturbing parallels: the blight that seeps from the primordial wound in the Withered Depths shares structural signatures with the Conclave's crystal-binding techniques. The Fungal Colossus that haunts the Mycelium Throne beneath the Withered Depths resembles nothing so much as a successful version of the Conclave's failed reinforcement subjects -- a living organism merged with mineral structure through magical binding, but grown to monstrous scale without the constraints of intentional design.

If the connection is genuine, then the Withered Depths' corruption is not a natural blight at all but an orphaned Conclave experiment running without oversight for centuries -- the same institutional carelessness that destroyed the sanctum, expressed on a geological timescale. The Blightmother at the corruption's heart may be the final product of a process the Conclave started and never finished: a custodial entity that has no master, no purpose, and no stop condition.

This connection remains speculative in-world. No surviving Conclave record explicitly names the trial sites, and the Withered Depths' druids (what remains of them) have their own origin stories for the blight. But adventurers who clear both areas may notice recurring motifs: crystal fragments embedded in corrupted root systems, preservation runes etched on fungal surfaces, and the same three-element symbolism (flame, crystal, spirit) that defines the sanctum's architecture.

### Lore Expansion: Present-Day Myth Ecology

Over generations, the surrounding settlements produced competing stories about the sealed complex. These myths vary by city and faction, reflecting each community's relationship with the Cinderteeth range and with dangerous knowledge:

**Mountain and frontier rumors (general):**
- Miners speak of the **Bell Under Stone**, heard only before cave-ins -- a low bronze tone that seems to come from below the rock rather than through it.
- Pilgrims whisper of **The Inkless Court**, where dead scholars still argue unfinished cases in a chamber where no ink can dry.
- Mercenaries trade rumors of a **vault that opens for no keyhole**, only for a hand-sized relic shape pressed into a slot of living iron.
- Survivors of deep expeditions mention a phrase carved repeatedly in old script: **"The mountain remembers what we refused."**

**Kowloon-specific rumors:**
- The **Ash Cult** cells in Kowloon (see `docs/kowloon_plan.md`, "Enemies, Tensions, and Rumors") preach that the Sunken Sanctum was divine punishment for scholars who defied the mountains' verdict -- the same theological argument they apply to the Ashfall Monsoon. The Ember Speaker reportedly claims to possess a fragment of Conclave crystal that "still hums with the mountain's judgment," though Kowloon's Temple Circle dismisses this as fabrication.
- Kowloon's **Courier Lantern Office** maintains a sealed registry of mountain expeditions that never returned, dating back to the decades after the sanctum's burial. The registry is not public, but CLO runners occasionally whisper that the list is longer than any official would admit, and that several entries bear the jade seals of Kowloon's own Jade Magistracy -- suggesting the city once sent sanctioned recovery teams and suppressed the results.
- Dockside fortune-tellers in Kowloon's **violet-lit alleys** sell "sanctum maps" to credulous travelers. The maps are universally fraudulent, but they persist because the underlying rumor is too profitable to die: somewhere in the Cinderteeth, there is a vault full of immortality research, and someone is going to find it.

**Kiess-specific rumors:**
- Kiess's **Syndic Council Annex** (see `docs/kiess_plan.md`) holds a small collection of artifacts recovered from Cinderteeth foothill ruins by Roc Road caravans over the centuries. Most are mundane -- corroded tools, blank crystal tablets, fragments of etched stone -- but one item, a palm-sized obsidian disc inscribed with three interlocking circles, matches the Conclave's triune seal exactly. The Syndics classify it as "provenance unknown" and decline to discuss it further.
- The **Temple of Concord** in Kiess includes the sanctum's fall in its annual Compact Flame recitations as a cautionary example: an institution that began with a preservation charter and ended by consuming the people it was meant to protect. Compact priests use the story to argue that Kiess's three-power governance (Temple, Syndic, Wall Command) prevents any single faction from accumulating unchecked authority -- though cynics note that the Conclave also had three factions, and balance did not save them.
- Scouts returning from deep Cinderteeth expeditions via Roc Road occasionally report finding **crystal shards embedded in mountain soil** that glow faintly blue in moonlight. Kiess's Sage's Alcove has tested several and found residual magical signatures consistent with preservation enchantments of extreme age. The sage's standing advice: "Do not carry them in pockets. Do not sleep near them. Do not assume they are inert."

**Midgaard-specific rumors:**
- Midgaard's **Granite Arcade** archives contain a single reference to the Spirebound Conclave in a trade ledger from the pre-road era: a line item recording payment of "forty iron bars and six condemned debtors" to "the mountain academy" in exchange for "consultation on defensive ward design." The entry is unremarkable in context -- condemned prisoners were routine currency in frontier-era trade -- but it establishes that Midgaard's civic predecessors directly supplied the Conclave with human subjects, a fact that modern guild historians prefer not to highlight.
- Rangers who patrol the northern reaches of the **Great Northern Forest** (see `docs/great_northern_forest_plan.md`) occasionally encounter travelers descending from the Cinderteeth passes who claim to have heard bells ringing inside the mountain. The wardens' standard response is to note the report, offer water, and advise the traveler to seek healing at the nearest waystation. The reports are filed but never investigated.

These myths map cleanly onto real area elements and can be fed into help files, quest hooks, and mob dialogue. City-specific rumors provide natural quest-hook entry points: Kowloon's CLO registry, Kiess's obsidian disc, and Midgaard's trade ledger each offer investigative threads that can lead players toward the sanctum through different narrative frames.

### Lore Expansion: Character and Encounter Anchors

To make the area feel authored rather than procedural, the plan assumes recurring named presences (whether as mobs, echoes, or environmental voice-lines):
- **Archivist-Magistrate Thalen Voss** -- signed many purge orders, later left confessions in hidden ledgers.
- **Quartermaster Edrin Hale** -- managed Sentinel induction rolls; likely source of coercive intake records.
- **Prism Adept Serai** -- authored conduit diagrams that now double as route clues.
- **The First Keeper** -- prototype of the Vaultkeeper line; buried with ceremonial regret, not triumph.

Their traces should appear as inscriptions, ghostly reenactments, desk records, broken seals, and chamber naming conventions across Zones B-H.

### Lore Expansion: Zone-by-Zone Story Beats

- **Zone A (9500-9512):** Introduction to ruin-state facts; players infer catastrophe but not intent.
- **Zone B (9513-9524):** Human cost emerges through barracks rosters, oath tablets, and forced induction logs.
- **Zone C (9525-9536):** Moral collapse made explicit: experimentation pipeline and failed ascension evidence.
- **Zone D (9537-9548):** Technical truth of the disaster; sabotage and contradictory lock-state mechanics revealed.
- **Zone E (9549-9560):** Testimony phase; spirits and records provide names, motives, and disputed accountability.
- **Zone F (9561-9572):** Physical aftermath; bodies, flood systems, and abandoned emergency engineering.
- **Zone G (9573-9584):** Judgment architecture; three lock traditions reconciled under player action.
- **Zone H (9585-9599):** Final synthesis: Continuance was never solved, only deferred into the Reliquary Core.

### Lore Expansion: Ending Interpretation

Defeating the final core guardian should not read as simple monster slaying. The intended thematic close is that players terminate a centuries-old institutional reflex: the sanctum's compulsion to preserve itself at any ethical cost. The exit chamber (`9599`) represents breath after pressure -- not a triumphant conquest, but a release from recursive duty.

## Irregular Macro Layout (by vnum clusters)
- **Zone A (9500-9512):** Existing canonical route (kept, lightly polished).
- **Zone B (9513-9524):** Burnt Barracks + Sentinel foundries (branching loops).
- **Zone C (9525-9536):** Experimental Galleries + specimen vaults (broken ring).
- **Zone D (9537-9548):** Crystal Conduit Works (zig-zag multi-loop).
- **Zone E (9549-9560):** Spirit Archives Annex + mnemonic wells (radial spokes).
- **Zone F (9561-9572):** Flooded Undercroft and cave-ins (vertical-feel traversal).
- **Zone G (9573-9584):** Rune-Lock Gauntlet and Reliquary approaches (gated knots).
- **Zone H (9585-9599):** Deep Vault of Continuance (boss/reward complex, non-linear).

---

## Room Layout Plan (9500-9599)

### Zone A — Known Sanctum Spine (9500-9512)
| Vnum | Room Name | Sector | Flags | Purpose / Notes |
|---|---|---|---|---|
| 9500 | The Collapsed Entrance | mountain (5) | 0 | Existing entry; links to outside world; collapse rubble lore anchor. |
| 9501 | The Outer Corridor | inside (11) | dark (1) | Existing mural hall introducing Conclave history. |
| 9502 | Hall of Shattered Pillars | inside (11) | dark (1) | Existing combat junction, first major branch node. |
| 9503 | The Charred Armoury | inside (11) | dark (1) | Existing flame-forged remains, Sentinel theme. |
| 9504 | The Sentinels' Watch | inside (11) | dark (1) | Existing locked vault-door staging room. |
| 9505 | Gallery of Experiments | inside (11) | dark (1) | Existing specimen gallery, horror/lab tone. |
| 9506 | The Forgotten Alcove | inside (11) | dark (1) | Existing hidden-idol cache; optional discovery room. |
| 9507 | The Crystal Laboratory | inside (11) | dark (1) | Existing crystal-faction entry point. |
| 9508 | The Eastern Corridor | inside (11) | dark (1) | Existing connector between lab and upper ruins. |
| 9509 | The Riddlegolem's Chamber | inside (11) | dark\|no_recall (8193) | Existing puzzle gate room; retains riddle identity. |
| 9510 | Riddlegolem's Secret Passage | inside (11) | dark (1) | Existing bypass conduit into deeper record halls. |
| 9511 | The Spirit Archives | inside (11) | dark (1) | Existing archive seed for spirit-lore branch. |
| 9512 | Antechamber of the Vault | inside (11) | dark\|no_recall (8193) | Existing final pre-vault gate room. |

### Zone B — Barracks of Cinder Oaths (9513-9524)
| Vnum | Room Name | Sector | Flags | Purpose / Notes |
|---|---|---|---|---|
| 9513 | Ashbound Muster Hall | inside (11) | dark (1) | Burnt training hall; lore plaques on oathbound sentries. |
| 9514 | Cinder-Choked Dormitory | inside (11) | dark (1) | Fallen bunks, ash ghosts, side loot pocket. |
| 9515 | Ruined Mess of Embers | inside (11) | dark (1) | Charred communal hall; irregular connector. |
| 9516 | The Black Bell Gallery | inside (11) | dark (1) | Bell ropes fused in glass; resonance lore. |
| 9517 | Smelter Vestibule | inside (11) | dark\|hot (33) | Heat-scarred transit to foundry ring. |
| 9518 | Sentinel Moldworks | inside (11) | dark\|hot (33) | Half-fused armor molds; heavy mob density. |
| 9519 | Forge of Living Iron | inside (11) | dark\|hot\|no_mob (37) | Elite forge room; flamebound mini-encounter. |
| 9520 | Quenching Trench | inside (11) | dark (1) | Dry trench crossing; hazard flavor. |
| 9521 | Oath-Keeper's Record Niche | inside (11) | dark (1) | Tablet room explaining coercive conscription. |
| 9522 | Collapsed Guard Ramp | inside (11) | dark (1) | Vertical-feel room; broken ramp to lower branch. |
| 9523 | Ember Reliquary Passage | inside (11) | dark (1) | Narrow, trapped-feel corridor to Zone C. |
| 9524 | Captain of Ash Post | inside (11) | dark\|no_mob (5) | Barracks mini-boss node; key fragment progression. |

### Zone C — Galleries of Failed Becoming (9525-9536)
| Vnum | Room Name | Sector | Flags | Purpose / Notes |
|---|---|---|---|---|
| 9525 | Specimen Intake Hall | inside (11) | dark (1) | Transition from military to experiment wing. |
| 9526 | Vitrine Row Alpha | inside (11) | dark (1) | Shattered glass lanes; stalker ambush geometry. |
| 9527 | Vitrine Row Beta | inside (11) | dark (1) | Parallel loop path with alternate pulls. |
| 9528 | The Resin Theater | inside (11) | dark (1) | Preserved failed forms, exposition chamber. |
| 9529 | Anatomist's Rotunda | inside (11) | dark (1) | Circular work floor, multi-exit confusion. |
| 9530 | Glyph Calibration Cell | inside (11) | dark (1) | Puzzle component room (glyph-stone continuity). |
| 9531 | Rune-Key Conservatory | inside (11) | dark (1) | Canonical key-lore reinforcement chamber. |
| 9532 | Idol Recess Chapel | inside (11) | dark (1) | Obsidian idol myth expansion. |
| 9533 | Vivisection Archive | inside (11) | dark (1) | Dark narrative reveal of human sourcing. |
| 9534 | Drainage of Grey Salts | inside (11) | dark (1) | Slippery service duct, shortcut route. |
| 9535 | Failed Ascendant Pit | inside (11) | dark (1) | Hazard pit with optional elite add. |
| 9536 | Curator Abomination Lair | inside (11) | dark\|no_mob (5) | Zone mini-boss; unlocks stable access to Zone D. |

### Zone D — Crystal Conduit Works (9537-9548)
| Vnum | Room Name | Sector | Flags | Purpose / Notes |
|---|---|---|---|---|
| 9537 | Conduit Threshold | inside (11) | dark (1) | Entry into power-distribution district. |
| 9538 | Static-Cleft Causeway | inside (11) | dark (1) | Crackling bridge, lateral split path. |
| 9539 | Prism Relay One | inside (11) | dark (1) | Relay node feeding old barriers. |
| 9540 | Prism Relay Two | inside (11) | dark (1) | Sister relay; supports loop routing. |
| 9541 | Capacitor Spine | inside (11) | dark (1) | Linear pressure corridor. |
| 9542 | Crystalline Overflow | inside (11) | dark (1) | Dense crystal growth, slowed movement flavor. |
| 9543 | Arcfall Intersection | inside (11) | dark (1) | Four-way irregular knot. |
| 9544 | Weaver's Transit Ring | inside (11) | dark (1) | Circular patrol path for crystalweavers. |
| 9545 | The Blue Fracture | inside (11) | dark (1) | Side rupture to flooded stone below. |
| 9546 | Polarity Control Dais | inside (11) | dark\|no_magic (17) | Mechanism room affecting door states. |
| 9547 | Lens of Severed Light | inside (11) | dark (1) | Puzzle-sight room; hidden text/exdesc focus. |
| 9548 | Core Conduit Heart | inside (11) | dark\|no_mob (5) | Zone capstone; powers spirit annex ingress. |

### Zone E — Mnemonic Archive Annex (9549-9560)
| Vnum | Room Name | Sector | Flags | Purpose / Notes |
|---|---|---|---|---|
| 9549 | Annex Entry Cloister | inside (11) | dark\|quiet (257) | Quiet transition into spirit-heavy wing. |
| 9550 | The Inner Vault | inside (11) | dark\|no_recall (8193) | Existing iconic vault room retained as anchor. |
| 9551 | Curved Stacks of Names | inside (11) | dark (1) | Circular stacks, name-ledger lore. |
| 9552 | Dustless Scriptorium | inside (11) | dark (1) | Time-locked chamber with pristine records. |
| 9553 | Mnemonic Well North | inside (11) | dark (1) | Echo-well that repeats dead catalogues. |
| 9554 | Mnemonic Well South | inside (11) | dark (1) | Companion well; puzzle pairing. |
| 9555 | Hall of Unfinished Last Words | inside (11) | dark (1) | Spirit encounter + emotional lore peak. |
| 9556 | Archivist Warden's Desk | inside (11) | dark (1) | Bureaucratic evidence of purge orders. |
| 9557 | Whisper Bridge | inside (11) | dark (1) | Thin crossing over record chasm. |
| 9558 | Seal of Quiet Ink | inside (11) | dark (1) | Rune-sealed records vault. |
| 9559 | The Index Catacomb | inside (11) | dark (1) | Numeric tomb-indexes, branching exits. |
| 9560 | Chamber of Bound Testimony | inside (11) | dark\|no_mob (5) | Zone climax, confessional spirit event. |

### Zone F — Flooded Undercroft (9561-9572)
| Vnum | Room Name | Sector | Flags | Purpose / Notes |
|---|---|---|---|---|
| 9561 | Subsidence Stair | inside (11) | dark (1) | Broken descent from archive foundations. |
| 9562 | Tilted Pump Hall | inside (11) | dark\|cold (65) | Mechanical ruin with waterlogged gears. |
| 9563 | The Cold Sump | water_swim (6) | dark\|cold (65) | Shallow flooded basin; ambush room. |
| 9564 | Broken Aqueduct Spine | water_swim (6) | dark\|cold (65) | Long jagged channel route. |
| 9565 | Silted Pilgrims' Niche | inside (11) | dark (1) | Forgotten shrine recess. |
| 9566 | Half-Drowned Reliquary | water_swim (6) | dark\|cold (65) | Chest bait + elite spawn chance. |
| 9567 | Echo-Cistern Mouth | inside (11) | dark\|cold (65) | Acoustic puzzle cue chamber. |
| 9568 | The Sunken Ossuary | inside (11) | dark (1) | Bone stacks of failed subjects. |
| 9569 | Floodgate Lever Room | inside (11) | dark (1) | Mechanical gate manipulation room. |
| 9570 | Undercurrent Crawl | water_swim (6) | dark\|cold (65) | Tight passage shortcut to upper loops. |
| 9571 | Cavern of Fallen Sigils | inside (11) | dark (1) | Natural cave break into sanctum architecture. |
| 9572 | Abyssal Lift Cradle | inside (11) | dark (1) | Ancient platform to Rune-Lock Gauntlet. |

### Zone G — Rune-Lock Gauntlet (9573-9584)
| Vnum | Room Name | Sector | Flags | Purpose / Notes |
|---|---|---|---|---|
| 9573 | Gauntlet Vestibule | inside (11) | dark\|no_recall (8193) | Hard-gate entry, checks progression items. |
| 9574 | First Lock: Ember Script | inside (11) | dark\|no_recall (8193) | Fire-script lock challenge. |
| 9575 | Interlock Passage I | inside (11) | dark\|no_recall (8193) | Narrow connector with patrol pressure. |
| 9576 | Second Lock: Prism Cipher | inside (11) | dark\|no_recall (8193) | Crystal-script lock challenge. |
| 9577 | Interlock Passage II | inside (11) | dark\|no_recall (8193) | Offset turn route, line-of-sight breaks. |
| 9578 | Third Lock: Spirit Writ | inside (11) | dark\|no_recall (8193) | Spirit-script lock challenge. |
| 9579 | Convergence Gallery | inside (11) | dark\|no_recall (8193) | Three-lock convergence chamber. |
| 9580 | Warden's Refusal Hall | inside (11) | dark\|no_recall (8193) | Lore wall: why sanctum was buried. |
| 9581 | The Broken Tribunal | inside (11) | dark\|no_recall\|no_mob (8197) | Fallen judges, elite trio encounter. |
| 9582 | Sigil Bridge of Continuance | inside (11) | dark\|no_recall (8193) | Transitional bridge into final vault cluster. |
| 9583 | Reliquary Outer Ring | inside (11) | dark\|no_recall (8193) | Circular pre-boss kite path. |
| 9584 | Reliquary Inner Ring | inside (11) | dark\|no_recall (8193) | Final preparation room before deep vault. |

### Zone H — Deep Vault of Continuance (9585-9599)
| Vnum | Room Name | Sector | Flags | Purpose / Notes |
|---|---|---|---|---|
| 9585 | Threshold of Silent Iron | inside (11) | dark\|no_recall\|no_teleport (24577) | Entry threshold to deepest sanctum. |
| 9586 | Hall of Sealed Alcoves | inside (11) | dark\|no_recall\|no_teleport (24577) | Loot/lore side alcoves, irregular branches. |
| 9587 | Chamber of Fused Oaths | inside (11) | dark\|no_recall\|no_teleport (24577) | Oath-ritual residue and spirit echoes. |
| 9588 | Triune Engine Perimeter | inside (11) | dark\|no_recall\|no_teleport (24577) | Outer ring around dormant core engine. |
| 9589 | Flame Node | inside (11) | dark\|no_recall\|no_teleport\|no_mob (24581) | Optional sub-objective node. |
| 9590 | Crystal Node | inside (11) | dark\|no_recall\|no_teleport\|no_mob (24581) | Optional sub-objective node. |
| 9591 | Spirit Node | inside (11) | dark\|no_recall\|no_teleport\|no_mob (24581) | Optional sub-objective node. |
| 9592 | Recombination Nexus | inside (11) | dark\|no_recall\|no_teleport (24577) | Node convergence; unlocks inner chamber. |
| 9593 | Vault of Interrupted Ascension | inside (11) | dark\|no_recall\|no_teleport (24577) | Major lore reveal room. |
| 9594 | Continuance Control Dais | inside (11) | dark\|no_recall\|no_teleport (24577) | Mechanism room, toggles final seals. |
| 9595 | The Penitent Gallery | inside (11) | dark\|no_recall\|no_teleport (24577) | Ghost procession event lane. |
| 9596 | Grave of the First Keeper | inside (11) | dark\|no_recall\|no_teleport\|no_mob (24581) | Relic-grade optional encounter. |
| 9597 | The Reliquary Core | inside (11) | dark\|no_recall\|no_teleport\|no_mob (24581) | Final boss chamber (evolved Vaultkeeper state). |
| 9598 | Collapse Escape Conduit | inside (11) | dark (1) | Post-boss escape route, timed-feel path. |
| 9599 | The Mountain's Breath | mountain (5) | 0 | Exit/epilogue chamber reconnecting upper ruin. |

---

## Expanded Mob Plan (High-Density Population)

Target: significantly increase mob variety and density across all expansion zones while preserving the level 150-170 curve.

### Mob Distribution Goals
- **Total planned mob templates:** 48 (trash, elites, specialists, mini-bosses, and bosses)
- **Base roaming/reset population target per full repop:** 140-190 active NPCs
- **Elite/mini-boss anchors:** 14-18 fixed or semi-fixed placements
- **Boss-tier anchors:** 5 major encounters (including final core guardian)

### Planned Mob Vnum Allocation
- **Existing mob vnums retained:** `9500-9508` (9 mobs)
- **New planned mob vnums:** `9509-9547` (39 additional mobs, sequential with no gaps)
- All mob vnums within area envelope `9500-9599`.

### Bosses (flagged `is_npc|sentinel|no_flee|stay_area|boss`, placed in `no_mob` rooms only)

| Vnum | Name | short_descr | Level | Room | act flags | Notes |
|---|---|---|---|---|---|---|
| 9509 | Edrin Hale, Ashsworn Quartermaster | `@@Rthe Ashsworn Quartermaster@@N` | 165 | 9524 | is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss | Zone B boss. Strong: fire. Weak: water. Cast: fireball, flamestrike. Def: heal, fireshield. Spec: `spec_cast_mage`. |
| 9510 | the Curator Abomination | `@@pthe Curator Abomination@@N` | 166 | 9536 | is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss | Zone C boss. Strong: poison, shadow. Weak: holy, fire. Cast: acid_blast, suffocate. Def: heal, iceshield. Spec: `spec_cast_mage`. |
| 9511 | Serai's Prism Reflection | `@@lSerai's Prism Reflection@@N` | 167 | 9548 | is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss | Zone D boss. Strong: air. Weak: earth. Cast: chain_light, static, light_bolt. Def: heal, shockshield. Spec: `spec_cast_mage`. |
| 9512 | the First Keeper's Memory | `@@athe First Keeper's Memory@@N` | 168 | 9596 | is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|undead\|boss | Zone H optional boss. Strong: mental, shadow. Weak: holy. Cast: mind_flail, mindflame, ego_whip. Def: heal. Spec: `spec_cast_undead`. |
| 9513 | Reliquary Core of Continuance | `@@Bthe Reliquary Core of Continuance@@N` | 170 | 9597 | is_npc\|sentinel\|no_flee\|aggressive\|stay_area\|boss | Final boss. Strong: fire, shadow, mental. Weak: holy. Cast: nerve_fire, mindflame, lava_burst. Def: heal, fireshield, iceshield, shockshield. Spec: `spec_cast_bigtime`. Skills: 5_attack, enhanced, counter, parry, dodge, nodisarm, notrip. |

### Strong Mobs (flagged `is_npc|aggressive|stay_area|solo`, levels 162-168)

| Vnum | Name | Level | Zone | Notes |
|---|---|---|---|---|
| 9514 | a Warden of Quiet Ink | 165 | E | Archive mini-boss. Cast: mind_bolt, ego_whip. Def: heal. Spec: `spec_cast_undead`. |
| 9515 | Voss, the Regret-Bound Magistrate | 166 | G | Spirit-judge elite. undead. Cast: mindflame, mind_flail. Def: heal. Spec: `spec_cast_undead`. |
| 9516 | a Polarity Regulator | 164 | D | Crystal elite. Cast: chain_light, static. Def: shockshield. |
| 9517 | a Continuance Core Herald | 168 | H | Pre-boss elite. Skills: 4_attack, enhanced, parry, dodge, counter. |
| 9518 | a Reliquary Ironbound | 166 | H | High armor frontline. Skills: 4_attack, enhanced, parry, nodisarm, notrip. |

### Trash Mobs (flagged `is_npc|aggressive|stay_area`, levels 150-164)

| Vnum | Name | Level | Zone(s) | Notes |
|---|---|---|---|---|
| 9519 | an ashbound spear-remnant | 150 | A, B | Melee. Skills: 2_attack, enhanced, kick. |
| 9520 | a cinder oath-recruit | 152 | A, B | Melee. Skills: 2_attack, enhanced, punch. |
| 9521 | a flamebound halberd sentinel | 158 | B | Elite bruiser. Skills: 3_attack, enhanced, parry, dodge. |
| 9522 | a bell-tower ember warden | 155 | B | Caster. Cast: fireball, flamestrike. Def: cure_critic. Spec: `spec_cast_mage`. |
| 9523 | a meltplate armorer husk | 156 | B | Heavy melee. Skills: 3_attack, enhanced, nodisarm. |
| 9524 | a vitrine breaker | 154 | C | Ambush skirmisher. Skills: 2_attack, enhanced, dodge, trip. |
| 9525 | a resin-bound penitent | 156 | C | Slow tank. Skills: 2_attack, enhanced, parry, nodisarm. |
| 9526 | a grey-salt anatomist | 155 | C | Debuffer. Cast: faerie_fire, ego_whip. Spec: `spec_cast_mage`. |
| 9527 | a stitched pursuer | 158 | C, F | Hunter. Skills: 3_attack, enhanced, dodge, trip, kick. |
| 9528 | a drain-crawl chimera | 157 | F | Water threat. Skills: 3_attack, enhanced, punch, headbutt. |
| 9529 | a prism relay custodian | 156 | D | Caster. Cast: chain_light, light_bolt. Def: cure_critic. Spec: `spec_cast_mage`. |
| 9530 | a static-lashed weaver | 158 | D | Control mob. Cast: static, shock_grasp. Skills: 2_attack, dodge. |
| 9531 | a conduit harmonizer | 154 | D | Support. Cast: flare, faerie_fire. Def: cure_serious. |
| 9532 | an arcfall sentinel | 160 | D | Shock elite. Cast: chain_light. Skills: 3_attack, enhanced, parry. |
| 9533 | a ledger-bound scribe | 153 | E | Caster. Cast: mind_flail, phobia. Spec: `spec_cast_undead`. undead. |
| 9534 | an unfinished witness | 155 | E | Mental pressure. undead, no_body. Cast: ego_whip, mind_bolt. |
| 9535 | a mnemonic bailiff | 158 | E | Control tank. undead. Skills: 3_attack, enhanced, parry, nodisarm. |
| 9536 | an inkless archivist echo | 154 | E | Debuff support. undead, no_body. Cast: faerie_fire, phobia. |
| 9537 | a testimony collector | 156 | E | Loot/key mob. undead. Skills: 2_attack, enhanced, dodge. |
| 9538 | a triune node emissary of flame | 164 | H | Node guardian. Cast: fireball, lava_burst. Def: fireshield. Spec: `spec_cast_mage`. |
| 9539 | a triune node emissary of crystal | 164 | H | Node guardian. Cast: chain_light, static. Def: shockshield. Spec: `spec_cast_mage`. |
| 9540 | a triune node emissary of spirit | 164 | H | Node guardian. undead. Cast: mindflame, mind_flail. Def: heal. Spec: `spec_cast_undead`. |
| 9541 | a penitent processional knight | 162 | H | Elite patroller. Skills: 4_attack, enhanced, parry, dodge, kick. |
| 9542 | a fracture lens guardian | 160 | D, G | Puzzle defender. Cast: light_bolt, flare. Skills: 2_attack, dodge. |
| 9543 | a flooded ossuary ghoul | 155 | F | Undead water mob. undead. Skills: 2_attack, enhanced, punch. |
| 9544 | a subsidence lurker | 153 | F | Ambush mob. Skills: 2_attack, enhanced, trip. |
| 9545 | a gauntlet rune-sentry | 160 | G | Lock guardian. Skills: 3_attack, enhanced, parry, dodge. |
| 9546 | a broken tribunal specter | 162 | G | Spirit. undead, no_body. Cast: mind_bolt, ego_whip. Def: cure_critic. |
| 9547 | a gravelurker | 152 | A, F | Calcified ambusher. no_mind. Skills: 2_attack, enhanced, headbutt. |

### Zone-by-Zone Mob Density Targets
- **Zone A (9500-9512):** 20-28 active (intro + existing canonical mobs)
- **Zone B (9513-9524):** 24-34 active (military packs + ash elites)
- **Zone C (9525-9536):** 22-30 active (ambush/lab predator pressure)
- **Zone D (9537-9548):** 20-28 active (caster-heavy conduit fights)
- **Zone E (9549-9560):** 18-26 active (spirit/control pressure)
- **Zone F (9561-9572):** 16-24 active (hazard + burst encounters)
- **Zone G (9573-9584):** 12-18 active (gauntlet + lock guardians)
- **Zone H (9585-9599):** 16-24 active baseline + boss events

---

## Expanded Object Plan (Large Loot and Utility Catalog)

Target: provide a broad object ecosystem (armor, weapons, keys, utility tokens) supporting exploration and progression. All objects follow spec weight-archetype encoding (`1`-`5` caster, `6`-`10` melee, `11`-`15` tank). Item stats are runtime-generated; area files define only identity/behavior data.

### Object Distribution Goals
- **Total planned object templates:** 60+
- **Wearable armor/accessories:** 30+
- **Weapons/offhands:** 14+
- **Progression keys and tokens:** 10+
- **Consumables and flavor objects:** 6+

### Planned Object Vnum Allocation
- **Existing object vnums retained:** `9500-9508` (existing items from original area)
- **New planned object vnums:** `9509-9568` (60 additional object slots, sequential, no gaps)
- All object vnums within area envelope `9500-9599`.

### Boss Equipment (Level 170, flagged `ITEM_NODROP|ITEM_BOSS|ITEM_MAGIC`)

`extra_flags` base = ITEM_NODROP(128) + ITEM_BOSS(134217728) + ITEM_MAGIC(64) = 134217920.
Items delivered via boss loot tables (`l`/`L` extensions) additionally get `ITEM_LOOT` (67108864), total = 201326784.

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | weight | Level | Source Boss |
|---|---|---|---|---|---|---|---|---|
| 9509 | an ashsworn helm of command | `@@Ra helm of ashsworn command@@N` | armor (9) | 201326784 | head\|take | 11 | L 170 | Edrin Hale (loot) |
| 9510 | the Quartermaster's ember halberd | `@@Rthe Quartermaster's ember halberd@@N` | weapon (5) | 201326784 + ITEM_TWO_HANDED(2147483648) = 2348810432 | hold\|take | 6 | L 170 | Edrin Hale (loot). Two-handed. value3=11 (pierce). |
| 9511 | a curator's carapace | `@@pthe Curator's carapace@@N` | armor (9) | 201326784 | body\|take | 11 | L 170 | Curator Abomination (loot) |
| 9512 | the Curator's surgical blade | `@@pthe Curator's surgical blade@@N` | weapon (5) | 201326784 | hold\|take | 1 | L 170 | Curator Abomination (loot). value3=2 (stab). |
| 9513 | a prismweave circlet | `@@lthe Prism Reflection's circlet@@N` | armor (9) | 201326784 | head\|take | 1 | L 170 | Serai's Prism Reflection (loot) |
| 9514 | a relay arc wand | `@@lthe Reflection's relay wand@@N` | weapon (5) | 201326784 + ITEM_EXTRA_WAND(536870912) = 738197696 | hold\|take | 1 | L 170 | Serai's Prism Reflection (loot). Wand. value3=6 (blast). |
| 9515 | a memory-keeper's shroud | `@@athe First Keeper's shroud@@N` | armor (9) | 201326784 | about\|take | 1 | L 170 | First Keeper's Memory (loot) |
| 9516 | the First Keeper's ossified mace | `@@athe First Keeper's ossified mace@@N` | weapon (5) | 201326784 | hold\|take | 6 | L 170 | First Keeper's Memory (loot). value3=7 (pound). |
| 9517 | a continuance core chestplate | `@@Bthe Continuance Core chestplate@@N` | armor (9) | 201326784 | body\|take | 11 | L 170 | Reliquary Core (loot) |
| 9518 | the Reliquary Core scepter | `@@Bthe Reliquary Core scepter@@N` | weapon (5) | 201326784 | hold\|take | 1 | L 170 | Reliquary Core (loot). value3=7 (pound). |
| 9519 | a reliquary ironbone shield | `@@Bthe Reliquary ironbone shield@@N` | armor (9) | 201326784 | hold\|take | 11 | L 170 | Reliquary Core (loot). Shield. |

### Trash / Elite Equipment (Level 150-168, flagged `ITEM_NODROP`)

Reset equipment (E/G): extra_flags = ITEM_NODROP(128) = 128.
Loot-table equipment: extra_flags = ITEM_NODROP(128) + ITEM_LOOT(67108864) = 67108992.

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | weight | Level | Delivery |
|---|---|---|---|---|---|---|---|---|
| 9520 | an ashbound iron helm | `@@Ran ashbound iron helm@@N` | armor (9) | 67108992 | head\|take | 11 | L 155 | Loot from ashbound spear-remnant |
| 9521 | a cinder-oathed spear | `@@Ra cinder-oathed spear@@N` | weapon (5) | 128 | hold\|take | 6 | L 152 | Reset (E) on cinder oath-recruit. value3=11 (pierce). |
| 9522 | a flamebound sentinel's greaves | `@@Ra flamebound sentinel's greaves@@N` | armor (9) | 128 | legs\|take | 11 | L 158 | Reset (E) on flamebound halberd sentinel |
| 9523 | a meltplate vambrace | `@@Ra meltplate vambrace@@N` | armor (9) | 67108992 | wrist\|take | 11 | L 156 | Loot from meltplate armorer husk |
| 9524 | a vitrine shard dagger | `@@pa vitrine shard dagger@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 154 | Loot from vitrine breaker. value3=2 (stab). |
| 9525 | a resin-coated buckler | `@@pa resin-coated buckler@@N` | armor (9) | 67108992 + ITEM_BUCKLER(268435456) = 335544320 | hold\|take | 11 | L 156 | Loot from resin-bound penitent. Buckler. |
| 9526 | a grey-salt analyst's cowl | `@@pa grey-salt analyst's cowl@@N` | armor (9) | 67108992 | head\|take | 1 | L 155 | Loot from grey-salt anatomist |
| 9527 | a stitched pursuer's cloak | `@@pa stitched pursuer's cloak@@N` | armor (9) | 67108992 | about\|take | 6 | L 158 | Loot from stitched pursuer |
| 9528 | a chimera fang blade | `@@pa chimera fang blade@@N` | weapon (5) | 67108992 | hold\|take | 6 | L 157 | Loot from drain-crawl chimera. value3=3 (slash). |
| 9529 | a prism custodian's robe | `@@la prism custodian's robe@@N` | armor (9) | 67108992 | body\|take | 1 | L 156 | Loot from prism relay custodian |
| 9530 | a static weaver's gauntlets | `@@la static weaver's gauntlets@@N` | armor (9) | 67108992 | hands\|take | 1 | L 158 | Loot from static-lashed weaver |
| 9531 | an arcfall sentinel's tower shield | `@@lan arcfall sentinel's tower shield@@N` | armor (9) | 128 | hold\|take | 11 | L 160 | Reset (E) on arcfall sentinel. Shield. |
| 9532 | a ledger-scribe's quill wand | `@@aa ledger-scribe's quill wand@@N` | weapon (5) | 67108992 + ITEM_EXTRA_WAND(536870912) = 604110848 | hold\|take | 1 | L 153 | Loot from ledger-bound scribe. Wand. value3=6 (blast). |
| 9533 | a mnemonic bailiff's helm | `@@aa mnemonic bailiff's helm@@N` | armor (9) | 128 | head\|take | 11 | L 158 | Reset (E) on mnemonic bailiff |
| 9534 | a testimony collector's ring | `@@aa testimony collector's ring@@N` | armor (9) | 67108992 | finger\|take | 1 | L 156 | Loot from testimony collector |
| 9535 | a gauntlet rune-sentry's halberd | `@@Ba gauntlet rune-sentry's halberd@@N` | weapon (5) | 128 + ITEM_TWO_HANDED(2147483648) = 2147483776 | hold\|take | 6 | L 160 | Reset (E) on gauntlet rune-sentry. Two-handed. value3=11 (pierce). |
| 9536 | a processional knight's sabatons | `@@Ba processional knight's sabatons@@N` | armor (9) | 67108992 | feet\|take | 11 | L 162 | Loot from penitent processional knight |
| 9537 | an ironbound cuirass | `@@Ban ironbound cuirass@@N` | armor (9) | 128 | body\|take | 11 | L 166 | Reset (E) on reliquary ironbound |
| 9538 | a gravelurker's calcified fist | `@@da gravelurker's calcified fist@@N` | weapon (5) | 67108992 + ITEM_FIST(1073741824) = 1140850816 | hold\|take | 6 | L 152 | Loot from gravelurker. Fist. value3=0 (hit). |

### Keys and Progression Objects

| Vnum | Name | short_descr | item_type | extra_flags | wear_flags | Level | Notes |
|---|---|---|---|---|---|---|---|
| 9539 | a cinder sigil key | `@@Ra cinder sigil key@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 160 | Opens charred lattice gate (9523/9525). |
| 9540 | a prism relay key | `@@la prism relay key@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 162 | Opens runed crystal shutter (9531/9537). |
| 9541 | an ink seal writ | `@@aan ink seal writ@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 164 | Opens ink-seal door (9558/9560). |
| 9542 | a floodgate torque wheel | `@@da floodgate torque wheel@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 163 | Opens floodgate bulkhead (9569/9573). |
| 9543 | a triune node core of flame | `@@Ra triune node core of flame@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 168 | Required for continuance seal. Drops from flame node emissary. |
| 9544 | a triune node core of crystal | `@@la triune node core of crystal@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 168 | Required for continuance seal. Drops from crystal node emissary. |
| 9545 | a triune node core of spirit | `@@aa triune node core of spirit@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 168 | Required for continuance seal. Drops from spirit node emissary. |
| 9546 | a vault door master key | `@@Ba vault door master key@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 155 | Opens vault doors at 9504 and 9512. |
| 9547 | a riddlegolem's token | `@@ca riddlegolem's token@@N` | key (18) | ITEM_NODROP (128) | take (8388608) | L 155 | Opens magical barrier at 9509. |

### Consumables (in rooms via `O` reset)

| Vnum | Name | item_type | extra_flags | wear_flags | Level | Notes |
|---|---|---|---|---|---|---|
| 9548 | an ember salve vial | potion (10) | 128 | take | L 155 | Spawns in Zone B rooms. |
| 9549 | a crystal tonic | potion (10) | 128 | take | L 160 | Spawns in Zone D rooms. |
| 9550 | a mnemonic phial | potion (10) | 128 | take | L 160 | Spawns in Zone E rooms. |

### Drop Philosophy
- Trash mobs: loot tables with zone-appropriate equipment drops.
- Elites/solo mobs: tighter weighted loot tables with signature pieces.
- Bosses: guaranteed equipment via loot tables; all boss items flagged `ITEM_BOSS`.
- Keys drop from specific mob kills or spawn in designated rooms.
- All loot-table items flagged `ITEM_LOOT`; reset equipment (E/G) not flagged `ITEM_LOOT`.

---

## Planned Door & Gate Specification

All doors follow spec: `D<door>` exit lines set `<locks>` bitvector including `door` bit (`1` = `EX_ISDOOR`). Runtime state (`closed`/`locked`) is authored through `#RESETS` command `D`, not in the room exit line. Locked doors require valid key object vnums. Every door is bidirectional (both sides defined).

### Door Summary Table

| From Room | Dir | To Room | Door Keyword | Locks Bits | Key Vnum | Reset State | Notes |
|---|---|---|---|---|---|---|---|
| 9504 | south (2) | 9550 | `^vault door` | door (1) | 9546 | `D 0 9504 2 2` (locked) | Canonical vault door. |
| 9550 | north (0) | 9504 | `^vault door` | door (1) | 9546 | `D 0 9550 0 2` (locked) | Reverse side. |
| 9512 | south (2) | 9550 | `^vault door` | door (1) | 9546 | `D 0 9512 2 2` (locked) | Canonical vault door. |
| 9550 | south (2) | 9512 | `^vault door` | door (1) | 9546 | `D 0 9550 2 2` (locked) | Reverse side. |
| 9509 | north (0) | 9510 | `^magical barrier` | door (1) | 9547 | `D 0 9509 0 2` (locked) | Riddle gate; opened by riddlegolem token. |
| 9510 | south (2) | 9509 | `^magical barrier` | door (1) | 9547 | `D 0 9510 2 2` (locked) | Reverse side. |
| 9523 | east (1) | 9525 | `^charred lattice gate` | door (1) | 9539 | `D 0 9523 1 2` (locked) | Barracks-to-gallery progression. Key: cinder sigil key. |
| 9525 | west (3) | 9523 | `^charred lattice gate` | door (1) | 9539 | `D 0 9525 3 2` (locked) | Reverse side. |
| 9531 | north (0) | 9537 | `^runed crystal shutter` | door (1) | 9540 | `D 0 9531 0 2` (locked) | Gallery-to-conduit progression. Key: prism relay key. |
| 9537 | south (2) | 9531 | `^runed crystal shutter` | door (1) | 9540 | `D 0 9537 2 2` (locked) | Reverse side. |
| 9546 | down (5) | 9561 | `^conduit iris` | door (1) | -1 | `D 0 9546 5 1` (closed) | Conduit-to-undercroft. Closed, not locked. |
| 9561 | up (4) | 9546 | `^conduit iris` | door (1) | -1 | `D 0 9561 4 1` (closed) | Reverse side. |
| 9558 | east (1) | 9560 | `^ink-seal door` | door (1) | 9541 | `D 0 9558 1 2` (locked) | Archive seal. Key: ink seal writ. |
| 9560 | west (3) | 9558 | `^ink-seal door` | door (1) | 9541 | `D 0 9560 3 2` (locked) | Reverse side. |
| 9569 | east (1) | 9573 | `^floodgate bulkhead` | door (1) | 9542 | `D 0 9569 1 2` (locked) | Undercroft-to-gauntlet. Key: floodgate torque wheel. |
| 9573 | west (3) | 9569 | `^floodgate bulkhead` | door (1) | 9542 | `D 0 9573 3 2` (locked) | Reverse side. |
| 9584 | east (1) | 9585 | `^continuance seal` | door (1) | 9543 | `D 0 9584 1 2` (locked) | Final vault entry. Key: triune node core of flame (any of the three node keys accepted). |
| 9585 | west (3) | 9584 | `^continuance seal` | door (1) | 9543 | `D 0 9585 3 2` (locked) | Reverse side. |
| 9597 | east (1) | 9598 | `^failsafe breach` | door (1) | -1 | `D 0 9597 1 1` (closed) | Post-boss exit. Closed, not locked. |
| 9598 | west (3) | 9597 | `^failsafe breach` | door (1) | -1 | `D 0 9598 3 1` (closed) | Reverse side. |

Note: Room 9599 connects up to 9500 as an open (non-door) exit for completion loopback. No door bits on this exit.

## Connectivity Shape Notes (irregular mandate)
- No perfect grid blocks; each zone uses offset spokes, broken rings, dead-end lore pockets, and loop-backs.
- At least one alternate route between each major zone transition (combat path vs. lore/utility path).
- Vertical-feel transitions represented through `up/down` exits in: `9522`, `9546`, `9561`, `9572`, `9599`.
- Final act supports both forward progression and post-boss return route without hard soft-locks.

## Encounter/Progression Intent (150-170)
- **9500-9530:** Level 150-158 onboarding for area mechanics.
- **9531-9560:** Level 158-164 sustained pressure with puzzle-combat mixing.
- **9561-9584:** Level 164-168 high-density elite routing.
- **9585-9599:** Level 168-170 climax path, final boss in 9597.

## Resets Plan

### Mob Resets (`M` commands)

Each boss spawns once (limit 1) in its designated `no_mob` room:
- `M 0 9509 1 9524` — Edrin Hale in Captain of Ash Post
- `M 0 9510 1 9536` — Curator Abomination in Curator Abomination Lair
- `M 0 9511 1 9548` — Serai's Prism Reflection in Core Conduit Heart
- `M 0 9512 1 9596` — First Keeper's Memory in Grave of the First Keeper
- `M 0 9513 1 9597` — Reliquary Core of Continuance in The Reliquary Core

Solo mobs spawn 1-2 times each in their zones:
- `M 0 9514 1 9560` — Warden of Quiet Ink in Chamber of Bound Testimony
- `M 0 9515 1 9581` — Voss in The Broken Tribunal
- `M 0 9516 2 <room>` — Polarity Regulators (2x in Zone D)
- `M 0 9517 2 <room>` — Continuance Core Heralds (2x in Zone H)
- `M 0 9518 2 <room>` — Reliquary Ironbounds (2x in Zone H)

Trash mobs spawn 2-4 times each, distributed across their zones. Total approximately 140-190 mob resets to populate 100 rooms.

Node emissaries (limit 1 each) in their designated `no_mob` rooms:
- `M 0 9538 1 9589` — Triune Node Emissary of Flame in Flame Node
- `M 0 9539 1 9590` — Triune Node Emissary of Crystal in Crystal Node
- `M 0 9540 1 9591` — Triune Node Emissary of Spirit in Spirit Node

### Equipment Resets (`E` commands, after corresponding `M`)

Reset equipment (non-loot, flagged ITEM_NODROP only):
- `E 0 9521 1 <hold_loc>` — Cinder-oathed spear on cinder oath-recruit
- `E 0 9522 1 <legs_loc>` — Flamebound sentinel's greaves on flamebound sentinel
- `E 0 9531 1 <hold_loc>` — Arcfall sentinel's tower shield on arcfall sentinel
- `E 0 9533 1 <head_loc>` — Mnemonic bailiff's helm on mnemonic bailiff
- `E 0 9535 1 <hold_loc>` — Gauntlet rune-sentry's halberd on gauntlet rune-sentry
- `E 0 9537 1 <body_loc>` — Ironbound cuirass on reliquary ironbound

### Object Resets (`O` commands, items in rooms)

- `O 0 9548 1 <room>` — Ember salve vials in Zone B rooms (2-3 placements)
- `O 0 9549 1 <room>` — Crystal tonics in Zone D rooms (2-3 placements)
- `O 0 9550 1 <room>` — Mnemonic phials in Zone E rooms (2-3 placements)

### Door Resets (`D` commands)

All doors listed in the Door Summary Table above. Total: 20 door resets (10 bidirectional door pairs).

---

## Specials Plan

```
M 9509 spec_cast_mage
M 9510 spec_cast_mage
M 9511 spec_cast_mage
M 9512 spec_cast_undead
M 9513 spec_cast_bigtime
M 9522 spec_cast_mage
M 9526 spec_cast_mage
M 9529 spec_cast_mage
M 9533 spec_cast_undead
M 9540 spec_cast_undead
M 9546 spec_cast_undead
```

Builder policy compliance: no `spec_summon_*` specs used (runtime-only). No `spec_keep*` specs used (manual-only).

---

## NPC Spec Compliance Notes

All mobile definitions must follow these spec requirements:

- **All mobs** must have `stay_area` (`64`) set in `act` flags. No exceptions.
- **All mobs** must have `is_npc` (`1`) set in `act` flags.
- **Boss mobs** (Edrin Hale, Curator Abomination, Serai's Prism Reflection, First Keeper's Memory, Reliquary Core): must have `sentinel` (`2`) + `boss` (`67108864`) + `stay_area` (`64`) + `no_flee` (`16`) and be placed only in rooms flagged `no_mob` (`4`).
- **Strong non-boss mobs** (Warden of Quiet Ink, Voss, Polarity Regulator, Core Herald, Ironbound): must have `solo` (`33554432`) + `stay_area` (`64`).
- **Aggressive mobs** (all combat mobs in this dungeon): set `aggressive` (`32`) in `act` flags.
- **Undead mobs** (spirit/echo types): set `undead` (`16384`) in `act` flags.
- **No-body mobs** (ghostly echoes): set `no_body` (`65536`) in `act` flags.
- **No-mind mobs** (gravelurkers, constructs): set `no_mind` (`262144`) in `act` flags.
- `invasion` flag (`536870912`) must never be set by builders in area files.
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. No multi-line `long_descr` is permitted.
- Mobile `description` must end with exactly one trailing newline before `~`.
- Vnums must never appear in any mobile description text (`long_descr` or `description`).

---

## Room Description Requirements (spec compliance)

- Every room `<description>~` must contain at least 3 sentences of text.
- Important rooms (9500 entry, 9509 riddlegolem, 9550 inner vault, 9597 final boss, 9599 exit) must contain at least 5 sentences.
- Every room description must be unique (no reused descriptions across rooms).
- Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
- When a room uses a named exit (non-empty `<exit_keyword>`), that exit name must appear in at least one of: the room's main `<description>~`, an object that spawns in the room, or an `E` extra description in the room.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main `<description>~`.
- Room descriptions must use the area's color theme intentionally and consistently.
- Each room's main `<description>~` must end with exactly one trailing newline immediately before the terminating `~`.

---

## Connection Plan (external links)

### Entry Connection
- **Sanctum room:** `9500` (The Collapsed Entrance, sector `mountain`)
- **External room:** TBD — connection to a Cinderteeth Mountains approach room or a mountain pass in an adjacent area.
- **Exit:** `9500` has an exit (direction TBD) to the external approach. Reverse exit on the external room points back to `9500`.
- **Door:** No door on the entry exit (open approach through collapsed rubble).
- **Lore:** The collapsed entrance opens onto a wind-scoured mountain ledge in the western Cinderteeth spur. Rubble from the deliberate upper-approach collapse is still visible, now overgrown with hardy mountain scrub and streaked with mineral deposits from centuries of glacial runoff.

### Completion Loopback
- **Room `9599`** (The Mountain's Breath) has an `up` exit to `9500` (The Collapsed Entrance).
- **Room `9500`** has a `down` exit to `9599`.
- No door on this exit. This provides a post-boss return route without requiring recall.

---

## Implementation Notes
- Keep existing named rooms and signature mechanics intact as historical spine.
- Expand using all remaining room vnums in `9500-9599` to realize full sanctum strata.
- Maintain consistent multi-color motif in room names/descriptions/messages while preserving readability and `@@N` resets.
- Ensure every lock has a corresponding in-area clue, item, or event so progression remains discoverable.

---

## Compliance Checklist
- [x] Plan only (no area file generation)
- [x] Uses `docs/area_file_spec.md` constraints in planning
- [x] `#AREA` header fully spec-compliant (`Q 16`, `O Virant~`, vnum bounds, level band)
- [x] Color theme defined (no `@@k`, no background codes, no flashing)
- [x] 100-room plan with all vnums used (`9500-9599`)
- [x] Room sector_type and room_flags specified for each room
- [x] Mob vnums sequential within area envelope, act flags specified
- [x] Object vnums sequential within area envelope, item_type/extra_flags/wear_flags/weight specified
- [x] Door plan fully spec-compliant (locks bits, key_vnums, bidirectional, reset states)
- [x] All locked doors have corresponding key objects in `#OBJECTS`
- [x] Boss mobs placed only in `no_mob` rooms, flagged `boss|sentinel|stay_area`
- [x] Solo mobs flagged `solo|stay_area`
- [x] All mobs flagged `stay_area`
- [x] Loot-table items flagged `ITEM_LOOT`; boss items flagged `ITEM_BOSS`
- [x] No `ITEM_GENERATED` set on area-authored objects
- [x] NPC spec compliance section included
- [x] Room description requirements section included
- [x] Resets plan with proper command format
- [x] Specials plan with allowed spec functions only
- [x] Connection plan specified
- [x] Lore cross-references Kowloon (Cinderteeth, Ashfall Monsoon), Kiess (border wars, founding), Midgaard (trade ledgers), Withered Depths (Conclave experiments)
