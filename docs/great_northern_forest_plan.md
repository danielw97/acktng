# Area Plan: The Great Northern Forest

## Overview
- **Area Name:** `@@GThe @@lGreat @@gNorthern @@rForest@@N`
- **File Name:** `great_northern_forest.are`
- **Keyword:** `greatnorthernforest`
- **Intended Levels:** 1-170 (all player levels)
- **Recommended Active Combat Band:** 20-100 in this area (higher-level progression continues into Kowloon and surrounding high-tier zones)
- **Vnum Range:** `15500-15999`
- **Vnum Count:** 500 rooms (all room vnums used)
- **Shape:** Irregular, meandering wilderness with loops, side pockets, and escalating danger bands away from the road
- **South Connection:** Room `15512` south exit connects to Midgaard room `3009` (Granite Arcade, northern wall spine)
- **North Connection:** Room `15994` north exit connects to Kowloon room `14095` (Iron Gate, southern wall)
- **Primary Travel Spine:** The Lantern Road (winding north-south road that repeatedly bends east/west)
- **Reset Rate:** 15
- **Reset Message:** `@@gmist settles between the old pines as the Great Northern Forest stirs@@N`

## Color Theme (multi-color, spec-safe)
Intentional in-string palette, avoiding `@@k` black, flashing, and background codes:

- **Canopy / life:** `@@G` green
- **Fog / age / stone:** `@@g` gray
- **Cold streams / moonlight:** `@@l` light blue
- **Mossfire / will-o-wisps:** `@@r` light green
- **Hazard / blood sign:** `@@R` red
- **Sacred sites / old oaths:** `@@W` white
- **Reset:** always `@@N`

Theme concept: a cold northern old-growth forest where civilization thins into ancestral wilderness. Near the road, colors are calmer (`@@g`, `@@l`, `@@G`). Deeper interior shifts into heightened `@@r` and danger-marking `@@R` for corrupted groves and apex territories.

## Area Header Plan (`#AREA` compliance)
- `Q 16` (mandatory)
- `K greatnorthernforest~`
- `L @@W(@@r1 170@@W)@@N~`
- `N <allocate at implementation time>`
- `I 1 170`
- `V 15500 15999`
- `F 15`
- `U @@gmist settles between the old pines as the Great Northern Forest stirs@@N~`
- `O Virant~`
- Optional builder flags as needed (`T`, etc.)

---

## Extensive Lore

### The First Treeline
Before Midgaard raised permanent stone beyond its southern rivers, the northern horizon was spoken of as a single dark wall: an endless crown of pine, cedar, and winter oak called simply **the Great Northern Forest**. Early Midgaard caravans called it a border. The elder hunters of the north called it a living nation.

The oldest songs say the forest grew from the graves of giants slain in the wars before named kingdoms. Their bones became limestone ridges, their blood became iron-rich springs, and their breath became the white mists that still pool beneath the boughs at dawn. Whether true or not, the forest has always defied easy conquest: roads sink, paths shift, and clearings vanish under root and fern in a single season.

Geographers who have studied both Midgaard's city archives and Kowloon's Jade Magistracy surveys estimate the forest covers nearly four hundred square miles of unbroken canopy. It is not a single biome but a layered continuum: temperate broadleaf near the southern verge where Midgaard's agricultural clearing ends, transitioning into dense boreal conifer stands at mid-depth, and finally giving way to ancient cold-growth stands of ironpine and ghost birch in the northern crown where Kowloon's influence begins. The treeline never truly ends; it simply yields, grudgingly, to the cleared approaches of both cities.

### Before Midgaard and Before Kowloon
Long before either city could project law beyond its walls, the forest functioned as a seasonal commons for scattered peoples: antler-herders in thaw months, resin gatherers in high summer, and masked winter-kin who moved only under snowfall. They did not map territory by straight borders. They mapped by stories: this creek where thunder struck a pine and split it in two, that hill where a mother bear spared a lost child, those standing stones where oathbreakers were buried face-down beneath gravel.

Ancient cairn carvings, found in the later Ironpine Rise, depict two recurring motifs: a lantern and a scale. Scholars now read this as an omen that the route would one day bind Midgaard's practical trade culture (the lantern of travel) with Kowloon's covenant bureaucracy (the scale of law). Whether prophecy or coincidence, modern wardens preserve these carvings as neutral heritage, not city property.

The oldest forest peoples called themselves by names that predated both cities' languages. Three groups survived long enough to leave lasting marks on the landscape:

- **The Everkeepers** were a druidic order who tended the ironpine groves of the mid-forest. They believed the trees were the pillars holding the sky above the earth, and that felling an ironpine without planting seven seedlings was a crime against the architecture of the world. Their stone planting-rings, each seven paces across, are still found throughout the Ironpine Rise, and modern wardens treat them as protected sites even when the original order is long dissolved.

- **The Mire-Speakers** inhabited the fenlands of what is now the Mosswater Bends. They built no permanent structures, living on woven reed platforms that rose and fell with the seasonal floods. Their knowledge of medicinal bog plants, fish migration patterns, and waterway navigation was encyclopedic. When Midgaard's first road expeditions reached the fens, it was Mire-Speaker guides who taught the quartermasters how to ford the black-water channels without losing pack animals to quickmud.

- **The Cairn-Keepers** held the high ridges and maintained the megalith sites throughout what is now the Ancestor Deeps. They practiced a form of ancestor communion at the standing stones, believing the dead could advise the living through patterns in birdsong, weather shifts, and the behavior of deer herds. Their burial customs placed the honored dead upright in stone niches facing north, toward what they called the Cold Throne, a spiritual seat of final judgment that bears an uneasy resemblance to Kowloon's later covenant stone tradition.

### Midgaard's Northern Need
As Midgaard prospered, pressure mounted to secure trade and timber to support expansion. The city's magistrates chartered ranger companies to cut a safe line northward. Most roads failed within a year. Flooded gullies, wolf packs, and territorial clans erased markers and swallowed camps.

Eventually, one expedition adopted a different rule: **do not force a straight line through a living forest**. Their quartermaster, Ilren Voss, followed game trails, ridgebacks, and old dry streambeds, allowing the road to bend where the land demanded. This became the **Lantern Road**---not a king's road, but a negotiated one. Lantern posts were raised at bends and ford points, each marked with both Midgaard runes and northern knot-signs.

Midgaard's guild records still describe this doctrine as *the Bent Mile Principle*: if a straight road costs ten lives a season, the wiser road is the one that bends around flood, den, and sacred grove. In practice, this principle explains the area's entire meandering structure.

Voss herself is remembered with unusual clarity for a figure so old. Her field journals, preserved in Midgaard's Granite Arcade archives, describe weeks spent negotiating with Mire-Speaker elders for permission to bridge their channels. She paid in iron nails, lamp oil, and a promise: the road would never dam a natural waterway. To this day, every bridge on the Lantern Road is built on pilings that allow full water flow beneath, and road wardens who violate this principle face dismissal. Voss's monument stands at the midpoint of the road, a simple limestone obelisk inscribed with the Bent Mile Principle in both Midgaard script and northern knot-notation.

### The Ash Winters and the Price of Passage
Two centuries ago, three successive ash-laden winters---likely from distant volcanic upheaval related to the same Cinderteeth eruptions that devastated Kowloon's delta with the Ashfall Monsoon---blanketed the canopy and poisoned shallow streams. Game died in migration corridors, and hunger turned minor territorial disputes into prolonged ambush wars. Caravan masters began hiring mixed escorts: Midgaard scouts for routefinding and northern bow-clans for local truces.

The Ash Winters killed an estimated third of the forest's large game population and drove several predator species into desperate ranges they had never before occupied. Wolves descended into the southern verge for the first time in living memory, and the great northern bears, starving and disoriented by ash-choked streams, attacked road camps with a ferocity that terrified even veteran rangers. The ecological damage took three generations to heal, and some scholars argue it never fully did: the predator-prey balance in the deep interior remains more volatile than it was before the ash, with periodic population crashes that send hungry beasts surging toward the road in what wardens call hunger tides.

The period forged the first written **Passage Tithe** system: caravans paid in grain, salt, lamp oil, and iron nails rather than coin alone. This stabilized key waystations, many of which later became permanent room anchors in the road band. Even now, relic toll ledgers are found in ruined guard huts throughout Mosswater Bends.

### The Northern Covenant and Kowloon Contact
Generations later, when Kowloon emerged as a disciplined city of covenant law and ledger order in the north (see `docs/kowloon_plan.md` for full Kowloon history), it sought reliable overland connection to Midgaard's markets and guild routes. Envoys from Kowloon's Harbor Syndics and Jade Magistracy met Midgaard's wardens at a neutral glade now called **The Oath-Crossing**. There, both parties adopted the forest road charter:

1. No army may march the Lantern Road in war formation.
2. Timbering along the route is restricted to safety cuts and bridge repair.
3. Shrine groves and burial mounds are inviolable.
4. Both cities maintain wardens, but neither claims the forest as sole territory.

A fifth article was added forty years later after a convoy massacre blamed on falsified seals: **all northern-bound manifests must carry dual marks**---Midgaard wax and Kowloon thread-knot. This reduced smuggling fraud and gave rise to mixed patrol teams near the northern approaches.

This compact made the Great Northern Forest not merely a passage, but a diplomatic buffer where Midgaard pragmatism meets Kowloon's covenant culture.

The Oath-Crossing negotiations were not smooth. Kowloon's envoys, trained in the city's precise contractual tradition, initially demanded that the forest charter include binding arbitration clauses enforceable by Kowloon's Jade Magistracy. Midgaard's wardens, who had spent decades earning trust from forest peoples through informal agreements and shared labor, refused any arrangement that gave a single city's courts jurisdiction over neutral ground. The compromise came from an unexpected source: a Cairn-Keeper elder named Toreth Gael, who proposed that disputes be settled at the standing stones under the old forest custom of witness judgment, where both parties presented their case before assembled neutral observers and the verdict was carved into the stone itself. This hybrid system, combining Kowloon's insistence on written record with the forest's tradition of communal witness, became the foundation of the modern warden tribunal system and is still practiced at the Oath-Crossing stones.

### How Kowloon Influence Appears in the Forest
Kowloon's influence is strongest in the northern crown and checkpoint rings:
- Waystations use strict inventory chalkboards and sealed ration lockers.
- Messenger shrines include covenant plaques naming neutral protections.
- Dispute circles use witness stones, mirroring Kowloon's public-judgment ethic.
- Northern patrol captains record incidents in duplicate: one ledger for Kowloon, one for shared forest archives.
- Kowloon's Wardens of Iron maintain a rotating garrison at the northern customs post, staffed on the same seasonal rotation principle used at Kowloon's city gates: no single commander holds the post for more than one season.
- The Courier Lantern Office operates signal relay stations in the northern crown, maintaining the communication chain between Kowloon proper and the forest's northern waystations.

These practices do not replace local custom; they coexist uneasily with older clan rites, which is a constant source of plot hooks and faction tension.

### How Midgaard Influence Appears in the Forest
Midgaard's influence is strongest in the southern verge and along the road:
- Road wardens carry Midgaard guild chits and report to the Granite Arcade administrative offices.
- Lamp-keepers are paid from Midgaard's civic treasury through a line item that has survived every budget dispute for two centuries, a testament to how essential the road is to Midgaard's economy.
- Southern patrol camps fly Midgaard's colors and operate under Midgaard's civil law, though enforcement becomes informal beyond the first few road-miles.
- Timber surveyors from Midgaard's Woodwrights Guild conduct annual assessments of the southern verge, marking trees approved for harvest with the Guild's green chalk seal. Unauthorized felling is punished by confiscation of tools and a season's ban from the road.
- The Granite Arcade in Midgaard, the northernmost point on the city's main spine (room `3009`), serves as the administrative hub for all forest-related commerce, contracts, and ranger writs. Its north exit opens directly onto the Lantern Road's southern approach.

### The Hidden Peoples
The forest is not empty. It is home to:
- **Roadwardens** and **lamp-keepers** maintaining waystones and posts. Roadwardens are recruited from both cities and from forest-born families, creating a unique mixed culture of practical wilderness competence and dual civic loyalty. They answer to a joint commission and carry identification recognized by both Midgaard and Kowloon.
- **Mire-clans** in fen basins who trade resin, fish oil, and medicinal bark. These are the modern descendants of the old Mire-Speakers, though they have adopted many Midgaard trade practices and some now accept coin alongside barter goods. Their reed-platform villages in the Mosswater Bends are semi-permanent, dismantled and rebuilt on higher ground during flood seasons.
- **Stone-antler circles** guarding megalith rings older than either city. Descendants of the Cairn-Keepers, they remain the most insular of the forest peoples. They tolerate road traffic but forbid entry to the megalith sites without ritual permission, and they have been known to ambush trespassers with coordinated bow-and-spear tactics that suggest military training far beyond what their small numbers would imply.
- **Ashfang warbands** and **winter predators** in far interior zones. The Ashfang are not a single tribe but a loose coalition of exiles, deserters, and opportunists who reject both cities' authority. They are named for their practice of blackening their teeth with ash, a deliberate inversion of the forest's old Ash Winter survivor traditions. They raid road traffic, extort smaller forest communities, and occasionally sell intelligence to whichever city will pay.
- **Rootbound cult remnants** who reject both cities and practice dangerous old rites. The Rootbound believe the forest itself is a sleeping deity whose dreams shape reality, and that the Lantern Road is a wound in the deity's flesh. Their sabotage of road infrastructure, lamp-posts, and bridge pilings makes them the most actively dangerous faction in the forest. Their leader, known as the Thornmother, is believed to operate from a hidden grove deep in the Ancestor Deeps. Kowloon's Wardens of Iron have posted a bounty; Midgaard's rangers consider her a priority target.

Most conflict is not full war but contested territory. Near-road inhabitants tolerate traffic for trade. Deep interior factions punish intrusion, creating a natural level gradient as danger rises with distance from the Lantern Road.

### Faction Timeline for Encounter Design
- **Founding Era:** Roadwardens and lamp-keepers dominate the route; low-threat wildlife and small brigands.
- **Tithe Era:** Mire-clans and toll houses rise; negotiated safe pockets form around bridges and fords.
- **Covenant Era:** Dual-city checkpoint protocol appears; disciplined but politically sensitive patrol rooms.
- **Fracture Era (current):** Rootbound splinters and Ashfang opportunists strike at weak links, pushing danger outward from the road into deep branches.

This timeline supports mob variety from roadside level ~20 patrols to high-threat interior cult enforcers and apex beasts.

### Ecology and Seasons
The forest's ecology varies dramatically by region and creates natural encounter variety:

- **Southern Verge:** Mixed broadleaf and pine, with significant undergrowth cleared by road wardens for visibility. Deer, wild boar, and foxes are common. Wolves appear seasonally during hard winters. The soil is fertile loam over clay, and the streams run clear. Travelers here see the most sunlight and encounter the least hostile wildlife.

- **Mosswater Bends:** A transition zone where the ground drops into fenland basins fed by seasonal overflow from unnamed tributaries. The canopy thins over water but thickens on the hummocks between channels. Giant frogs, marsh snakes, and territorial herons the size of a man's torso inhabit the fen edges. The water ranges from knee-deep to drowning-deep with little warning, and the mud preserves footprints and scent for predators to follow.

- **Ironpine Rise:** Elevation climbs sharply as the forest floor transitions from loam to thin soil over granite. The ironpines themselves are enormous conifers with bark that turns nearly black with age and wood so dense it dulls axes. Wolves and mountain cats are the primary predators, but the ridge corridors also channel wind into howling funnels that can mask the sound of approaching threats. The Everkeeper planting-rings, now overgrown, create natural clearings where ambush encounters are common.

- **Ancestor Deeps:** The oldest part of the forest, where ghost birch and ancient ironpines grow so thick that the canopy blocks most sunlight. The ground is carpeted with luminescent moss that gives the area an eerie green-white glow. The air is noticeably colder here, and breath steams even in summer. Large predators here include cave bears, dire wolves, and solitary territorial beasts that have grown enormous in the absence of human hunting pressure. The standing stones of the Cairn-Keepers create natural waypoints, but the spaces between them are dense, dark, and disorienting.

- **Northern Crown:** The forest thins slightly as it approaches Kowloon's cleared approach zone, but the trees that remain are the oldest and largest in the entire forest. Cold mist pools in the valleys between ridges, reducing visibility to arm's length on bad days. The wildlife here shows signs of Kowloon's proximity: fewer large predators (the Wardens of Iron conduct periodic clearing patrols), but more cunning mid-size threats---ambush cats, venomous serpents, and the occasional organized Ashfang warband testing Kowloon's southern defenses.

### Myths, Omens, and Recurring Symbols
To keep area flavor cohesive, recurring lore symbols appear in room descriptions, objects, and NPC speech:
- **Lantern-and-scale motif:** lawful passage and mutual accountability. This symbol appears carved into Lantern Road waystones, stamped on patrol writ-seals, and woven into the sashes of joint-commission wardens.
- **Split pine motif:** survival by adaptation, not domination. Ilren Voss adopted this as her personal seal, and it persists on road markers, bridge keystones, and the Bent Mile monument.
- **White-stag omen:** appears before major weather shifts or faction war. Forest peoples consider sighting a white stag either a blessing or a warning, depending on the direction it runs: toward the road means peace, away from the road means danger approaches.
- **Red moss bloom:** warning sign of corrupted groves and level-spike pockets. When the luminescent moss of the Ancestor Deeps turns red, wardens know that something has disturbed the deep forest's equilibrium. Rootbound cult activity frequently coincides with red moss blooms.

These symbols reinforce the multi-color theme and give players narrative continuity while traversing different difficulty bands.

### Why This Area Fits All Levels
- New and mid-level travelers follow Lantern Road patrol lines and service camps.
- Experienced players branch into side valleys, barrows, and ruin pockets.
- High-level players push into far-rim primal zones where apex spirits and war-chiefs hold ground.
- Narrative progression naturally points south to Midgaard institutions (Granite Arcade ranger contracts, guild writs, caravan escort postings) and north to Kowloon's urban power centers (Iron Gate customs processing, Jade Magistracy tariff filings, Harbor Syndic contracts).
- Social gameplay is encouraged through escort contracts, checkpoint diplomacy quests, and deep-interior bounty chains.

---

## Macro Layout (Irregular by Design)
The 500 rooms are arranged as an **irregular 20x25 coordinate envelope** (x=0..19, y=0..24), but connectivity is non-grid: loops, deadwood pockets, branching tributaries, and offset subregions.

- **All vnums `15500-15999` are rooms in this area plan**.
- Coordinates are planning coordinates only; final exits intentionally avoid full cardinal mesh.
- The Lantern Road weaves across the map, never remaining straight for more than 3-4 rooms.

### Coordinate-to-Vnum Formula
`vnum = 15500 + (y * 20) + x`

Example:
- `(x=0,y=0) => 15500`
- `(x=12,y=0) => 15512` (south link to Midgaard `3009`)
- `(x=14,y=24) => 15994` (north link to Kowloon `14095`)

---

## 20x25 Room Layout Grid (all 500 vnums)

| y\\x | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 00 | 15500 | 15501 | 15502 | 15503 | 15504 | 15505 | 15506 | 15507 | 15508 | 15509 | 15510 | 15511 | 15512 | 15513 | 15514 | 15515 | 15516 | 15517 | 15518 | 15519 |
| 01 | 15520 | 15521 | 15522 | 15523 | 15524 | 15525 | 15526 | 15527 | 15528 | 15529 | 15530 | 15531 | 15532 | 15533 | 15534 | 15535 | 15536 | 15537 | 15538 | 15539 |
| 02 | 15540 | 15541 | 15542 | 15543 | 15544 | 15545 | 15546 | 15547 | 15548 | 15549 | 15550 | 15551 | 15552 | 15553 | 15554 | 15555 | 15556 | 15557 | 15558 | 15559 |
| 03 | 15560 | 15561 | 15562 | 15563 | 15564 | 15565 | 15566 | 15567 | 15568 | 15569 | 15570 | 15571 | 15572 | 15573 | 15574 | 15575 | 15576 | 15577 | 15578 | 15579 |
| 04 | 15580 | 15581 | 15582 | 15583 | 15584 | 15585 | 15586 | 15587 | 15588 | 15589 | 15590 | 15591 | 15592 | 15593 | 15594 | 15595 | 15596 | 15597 | 15598 | 15599 |
| 05 | 15600 | 15601 | 15602 | 15603 | 15604 | 15605 | 15606 | 15607 | 15608 | 15609 | 15610 | 15611 | 15612 | 15613 | 15614 | 15615 | 15616 | 15617 | 15618 | 15619 |
| 06 | 15620 | 15621 | 15622 | 15623 | 15624 | 15625 | 15626 | 15627 | 15628 | 15629 | 15630 | 15631 | 15632 | 15633 | 15634 | 15635 | 15636 | 15637 | 15638 | 15639 |
| 07 | 15640 | 15641 | 15642 | 15643 | 15644 | 15645 | 15646 | 15647 | 15648 | 15649 | 15650 | 15651 | 15652 | 15653 | 15654 | 15655 | 15656 | 15657 | 15658 | 15659 |
| 08 | 15660 | 15661 | 15662 | 15663 | 15664 | 15665 | 15666 | 15667 | 15668 | 15669 | 15670 | 15671 | 15672 | 15673 | 15674 | 15675 | 15676 | 15677 | 15678 | 15679 |
| 09 | 15680 | 15681 | 15682 | 15683 | 15684 | 15685 | 15686 | 15687 | 15688 | 15689 | 15690 | 15691 | 15692 | 15693 | 15694 | 15695 | 15696 | 15697 | 15698 | 15699 |
| 10 | 15700 | 15701 | 15702 | 15703 | 15704 | 15705 | 15706 | 15707 | 15708 | 15709 | 15710 | 15711 | 15712 | 15713 | 15714 | 15715 | 15716 | 15717 | 15718 | 15719 |
| 11 | 15720 | 15721 | 15722 | 15723 | 15724 | 15725 | 15726 | 15727 | 15728 | 15729 | 15730 | 15731 | 15732 | 15733 | 15734 | 15735 | 15736 | 15737 | 15738 | 15739 |
| 12 | 15740 | 15741 | 15742 | 15743 | 15744 | 15745 | 15746 | 15747 | 15748 | 15749 | 15750 | 15751 | 15752 | 15753 | 15754 | 15755 | 15756 | 15757 | 15758 | 15759 |
| 13 | 15760 | 15761 | 15762 | 15763 | 15764 | 15765 | 15766 | 15767 | 15768 | 15769 | 15770 | 15771 | 15772 | 15773 | 15774 | 15775 | 15776 | 15777 | 15778 | 15779 |
| 14 | 15780 | 15781 | 15782 | 15783 | 15784 | 15785 | 15786 | 15787 | 15788 | 15789 | 15790 | 15791 | 15792 | 15793 | 15794 | 15795 | 15796 | 15797 | 15798 | 15799 |
| 15 | 15800 | 15801 | 15802 | 15803 | 15804 | 15805 | 15806 | 15807 | 15808 | 15809 | 15810 | 15811 | 15812 | 15813 | 15814 | 15815 | 15816 | 15817 | 15818 | 15819 |
| 16 | 15820 | 15821 | 15822 | 15823 | 15824 | 15825 | 15826 | 15827 | 15828 | 15829 | 15830 | 15831 | 15832 | 15833 | 15834 | 15835 | 15836 | 15837 | 15838 | 15839 |
| 17 | 15840 | 15841 | 15842 | 15843 | 15844 | 15845 | 15846 | 15847 | 15848 | 15849 | 15850 | 15851 | 15852 | 15853 | 15854 | 15855 | 15856 | 15857 | 15858 | 15859 |
| 18 | 15860 | 15861 | 15862 | 15863 | 15864 | 15865 | 15866 | 15867 | 15868 | 15869 | 15870 | 15871 | 15872 | 15873 | 15874 | 15875 | 15876 | 15877 | 15878 | 15879 |
| 19 | 15880 | 15881 | 15882 | 15883 | 15884 | 15885 | 15886 | 15887 | 15888 | 15889 | 15890 | 15891 | 15892 | 15893 | 15894 | 15895 | 15896 | 15897 | 15898 | 15899 |
| 20 | 15900 | 15901 | 15902 | 15903 | 15904 | 15905 | 15906 | 15907 | 15908 | 15909 | 15910 | 15911 | 15912 | 15913 | 15914 | 15915 | 15916 | 15917 | 15918 | 15919 |
| 21 | 15920 | 15921 | 15922 | 15923 | 15924 | 15925 | 15926 | 15927 | 15928 | 15929 | 15930 | 15931 | 15932 | 15933 | 15934 | 15935 | 15936 | 15937 | 15938 | 15939 |
| 22 | 15940 | 15941 | 15942 | 15943 | 15944 | 15945 | 15946 | 15947 | 15948 | 15949 | 15950 | 15951 | 15952 | 15953 | 15954 | 15955 | 15956 | 15957 | 15958 | 15959 |
| 23 | 15960 | 15961 | 15962 | 15963 | 15964 | 15965 | 15966 | 15967 | 15968 | 15969 | 15970 | 15971 | 15972 | 15973 | 15974 | 15975 | 15976 | 15977 | 15978 | 15979 |
| 24 | 15980 | 15981 | 15982 | 15983 | 15984 | 15985 | 15986 | 15987 | 15988 | 15989 | 15990 | 15991 | 15992 | 15993 | 15994 | 15995 | 15996 | 15997 | 15998 | 15999 |

---

## Irregular Region Partition and Progression Bands

### Region A: Southern Verge / Midgaard March (15500-15619, 120 rooms)
- Closest to Midgaard; patrol camps, hunter tracks, shrines, and safer edges.
- Lantern Road begins at `15512` and drifts northwest then northeast before resuming northward.
- **Road mobs:** level 18-26 (target ~20).
- **Off-road mobs:** level 24-38.

### Region B: Mosswater Bends (15620-15719, 100 rooms)
- Wet hollows, streams, and fen bridges.
- Multiple side loops with reed villages and trapped beast dens.
- **Road mobs:** level 22-32.
- **Off-road mobs:** level 35-55.

### Region C: Ironpine Rise (15720-15819, 100 rooms)
- Higher ridges, wolf-clan hunting grounds, abandoned watch cairns.
- Irregular switchbacks and cliffside connectors.
- **Road mobs:** level 28-40.
- **Off-road mobs:** level 48-72.

### Region D: Ancestor Deeps (15820-15919, 100 rooms)
- Megalith rings, spirit groves, and root-cavern mouths.
- Dense branch structure intentionally far from road.
- **Road mobs:** level 35-50.
- **Off-road mobs:** level 65-90.

### Region E: Northern Crown / Kowloon Reach (15920-15999, 80 rooms)
- Final approach toward Kowloon contact zone.
- Cold mist valleys, oath-steles, and elite hostile pockets.
- **Road mobs:** level 38-55.
- **Farthest interior apex pockets:** level 95-100 cap.

> Design rule enforced: the **highest mobs (~100)** are at the **greatest graph distance from Lantern Road**, not merely by north/south position.

---

## Lantern Road Meander Plan (primary route)

Main road sequence (south to north), deliberately winding:

`15512 -> 15511 -> 15531 -> 15530 -> 15550 -> 15551 -> 15571 -> 15572 -> 15592 -> 15612 -> 15613 -> 15633 -> 15653 -> 15652 -> 15672 -> 15673 -> 15693 -> 15713 -> 15712 -> 15732 -> 15752 -> 15753 -> 15773 -> 15774 -> 15794 -> 15814 -> 15813 -> 15833 -> 15834 -> 15854 -> 15874 -> 15873 -> 15893 -> 15913 -> 15912 -> 15932 -> 15952 -> 15953 -> 15973 -> 15974 -> 15994`

Road adjacency verification (all consecutive pairs are cardinally adjacent on the 20x25 grid):
- `15512(12,0)` west to `15511(11,0)` north to `15531(11,1)` west to `15530(10,1)` north to `15550(10,2)` east to `15551(11,2)` ... continuing with verified cardinal adjacency through each step ... north to `15974(14,23)` north to `15994(14,24)`.

- Connects south to Midgaard via `15512` south exit to Midgaard room `3009` (Granite Arcade).
- Connects north to Kowloon via `15994` north exit to Kowloon room `14095` (Iron Gate).
- Never forms a straight uninterrupted cardinal strip.
- Road-adjacent service clearings branch 1-3 steps off this chain.

---

## Connection Plan (external links)

### South Connection: Midgaard
- **Forest room:** `15512` (South Lantern Gate, coordinate x=12, y=0)
- **Midgaard room:** `3009` (Granite Arcade, northernmost point of Midgaard's main north-south spine)
- **Exit:** `15512` has `D2` (south) exit to `3009`. Midgaard `3009` gets `D0` (north) exit to `15512`.
- **Door:** Door flag (`EX_ISDOOR`) set on the `15512` south exit. Reverse exit on `3009` north must also have door flag set.
- **Reset:** `D 0 15512 2 1` (south door, state closed). `D 0 3009 0 1` on Midgaard's side (added when forest is implemented).
- **Key:** `-1` (no key; closed but not locked).
- **Lore:** The South Lantern Gate marks the formal boundary where Midgaard's cleared approaches end and the forest canopy begins. A pair of iron-banded lamp posts flanks the road, their bases carved with Midgaard runes and the split-pine motif of Ilren Voss.

### North Connection: Kowloon
- **Forest room:** `15994` (North Lantern Gate, coordinate x=14, y=24)
- **Kowloon room:** `14095` (Iron Gate, southern wall of Kowloon, per `docs/kowloon_plan.md`)
- **Exit:** `15994` has `D0` (north) exit to `14095`. Kowloon `14095` has `D2` (south) exit to `15994`.
- **Door:** Door flag (`EX_ISDOOR`) set on the `15994` north exit. Reverse exit on `14095` south already has door flag set per Kowloon plan.
- **Reset:** `D 0 15994 0 1` (north door, state closed). Kowloon's `D 0 14095 2 1` handles the reverse.
- **Key:** `-1` (no key; closed but not locked).
- **Lore:** The North Lantern Gate stands where the forest's last ironpines give way to Kowloon's cleared customs approach. A Kowloon-standard chalkboard lists current tariff rates and caravan inspection hours. The gate posts bear both the lantern-and-scale motif and Kowloon's Wardens of Iron insignia.

---

## Door Plan (explicit, spec-compliant)
Only major political boundary gates and select road checkpoints use doors; interior forest paths are undoored natural movement.

### Perimeter Gates

1. **South Lantern Gate** (`15512` south exit to Midgaard `3009`)
   - `D2` (south) exit: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS` command `D`: `D 0 15512 2 1` (door south, state `closed`).
   - Reverse exit on Midgaard `3009` (north) must also have `door` bit set.

2. **North Lantern Gate** (`15994` north exit to Kowloon `14095`)
   - `D0` (north) exit: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS` command `D`: `D 0 15994 0 1` (door north, state `closed`).
   - Reverse exit on Kowloon `14095` (south) already has `door` bit set per Kowloon plan.

### Optional Internal Checkpoint Doors (if desired for pacing, max 4)

3. **Fen Toll Bridge Gate** (`15673` east exit `<->` `15674` west exit)
   - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS`: `D 0 15673 1 1` (east door, state closed).
   - Note: `15673` is a road room (x=13, y=8); `15674` (x=14, y=8) is adjacent east.

4. **Ridge Pass Barricade** (`15794` east exit `<->` `15795` west exit)
   - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS`: `D 0 15794 1 1` (east door, state closed).
   - Note: `15794` is a road room (x=14, y=14); `15795` (x=15, y=14) is adjacent east.

5. **Ancestor Ring Ward Gate** (`15893` east exit `<->` `15894` west exit)
   - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS`: `D 0 15893 1 1` (east door, state closed).
   - Note: `15893` is a road room (x=13, y=19); `15894` (x=14, y=19) is adjacent east.

6. **Northern Customs Post** (`15952` east exit `<->` `15953` west exit)
   - Both exits: `<locks>` includes `door` bit (`1`). `<key_vnum>` = `-1`.
   - `#RESETS`: `D 0 15952 1 1` (east door, state closed).
   - Note: `15952` and `15953` are consecutive road rooms (x=12/13, y=22). This checkpoint gates passage along the road itself.

All checkpoint doors reset to **closed** (state `1`), not locked. Since they are not locked on reset, no key objects are required (spec: key objects are only mandatory when `D` state = `2` locked). `<key_vnum>` is set to `-1` per spec for exits that cannot be locked/unlocked with a key.

If optional checkpoint doors are omitted, only the two perimeter gates remain.

---

## Mob Containment Strategy (strict anti-wander)
All mobs must be local to their intended micro-region. Use one of two containment methods per spawn group:

1. **Sentinel method:**
   - Use `sentinel` + `stay_area` for stationary defenders, camp NPCs, shrine guardians, and named encounters.

2. **No-mob boundary method:**
   - Build small subregion perimeters where all exits out of a mob pocket pass through `ROOM_NO_MOB` choke rooms.
   - Mobs can roam within their pocket but cannot cross perimeter exits.
   - Roaming mobs must still have `stay_area` set.

### Placement rules
- **Roadside patrol/traffic mobs** (target ~20 near south road, scaling modestly north): mostly sentinel or 2-4 room no_mob-bounded patrol loops.
- **Interior predator packs:** no_mob-bounded subgraphs with 6-20 rooms each.
- **Elite apex mobs (95-100):** sentinel or tiny no_mob cages in farthest nodes (e.g., deep branches off `15966-15969`, `15990-15993` interior pockets).
- No free-roaming area-wide wanderers.

---

## Difficulty Gradient by Distance-from-Road
Use shortest-path distance in rooms from nearest Lantern Road node:

- **Distance 0-1:** level 18-30 (road guards, merchants, poachers)
- **Distance 2-3:** level 28-45 (wolves, bandits, mire reptiles)
- **Distance 4-6:** level 42-70 (clan hunters, stone-antler zealots, corrupted beasts)
- **Distance 7-9:** level 68-90 (spirit champions, ancient predators)
- **Distance 10+:** level 90-100 (apex entities, oathbreaker warlords)

This ensures progression is spatially intuitive and compliant with the requested cap behavior.

---

## Room Layout Rules (implementation guidance)
- All 500 vnums are rooms in this area.
- Exits are primarily cardinal; vertical exits used sparingly for caverns/ridges.
- Bi-directional exits required unless one-way is explicitly justified.
- Irregularity enforced via:
  - nonuniform branch lengths,
  - asymmetric loops,
  - cul-de-sacs with rewards/lore,
  - off-axis reconnectors between regions.
- Avoid perfect rectangles of connected rooms in final exit graph.

### Suggested structural percentages
- 40% road + near-road rooms
- 35% mid-depth wilderness loops
- 20% deep interior danger pockets
- 5% special sites (shrines, waystations, megalith rings, diplomatic markers)

---

## Key Story Sites (anchor rooms)
- `15512` **South Lantern Gate** (Midgaard link to `3009`)
- `15673` **Mosswater Toll Bridge**
- `15752` **Ilren's Bent Mile Marker** (historic roadbuilder monument)
- `15893` **Oath-Crossing Stones** (Midgaard-Kowloon covenant marker)
- `15932` **North Warden Camp**
- `15994` **North Lantern Gate** (Kowloon link to `14095`)
- `15997-15999` **Crown of Antlers** (farthest apex wilderness)

These anchors tie lore directly to traversal and level escalation.

---

## NPC Spec Compliance Notes
All mobile definitions must follow these spec requirements:

- **All mobs** must have `stay_area` (`64`) set in `act` flags. No exceptions.
- **All mobs** must have `is_npc` (`1`) set in `act` flags.
- **Sentinel mobs** (roadwardens, lamp-keepers, shrine guardians, named NPCs): `act` flags include `is_npc` (`1`), `sentinel` (`2`), `stay_area` (`64`).
- **Boss mobs** (if any): must have `sentinel` (`2`) + `boss` (`67108864`) + `stay_area` (`64`) and be placed only in rooms flagged `no_mob` (`4`).
- **Strong non-boss mobs** (e.g., Ashfang war-chief, Thornmother lieutenant, apex predators): must have `solo` (`33554432`) + `stay_area` (`64`).
- **Aggressive mobs** (feral beasts, hostile undead, Rootbound cultists, Ashfang raiders): set `aggressive` (`32`) in `act` flags.
- **Non-hostile mobs** (roadwardens, merchants, lamp-keepers, mire-clan traders): leave `aggressive` unset.
- Mobile `long_descr` must be exactly one text line followed by a newline and a `~`-only line. No multi-line `long_descr` is permitted.
- Mobile `description` must end with exactly one trailing newline before `~`.
- Vnums must never appear in any mobile description text (`long_descr` or `description`).

### NPC Level Targets
- **Road wardens and lamp-keepers:** level 30-50 (sentinel, scales with road region).
- **Checkpoint guards and patrol captains:** level 50-80 (sentinel at gates, stronger near Kowloon approach).
- **Mire-clan traders and forest workers:** level 20-35 (non-aggressive, sentinel).
- **Wildlife (wolves, boar, bears, snakes):** level 20-90 (aggressive, scaled by distance from road).
- **Ashfang warbands:** level 50-90 (aggressive, scaled by depth).
- **Rootbound cultists:** level 60-95 (aggressive, deep interior).
- **Stone-antler circle guardians:** level 55-85 (aggressive only in sacred sites, sentinel at megaliths).
- **Apex entities (Crown of Antlers):** level 95-100 (solo or boss, sentinel, farthest from road).

---

## Room Description Requirements (spec compliance)
- Every room `<description>~` must contain at least 3 sentences of text.
- Important rooms (South Lantern Gate, North Lantern Gate, Oath-Crossing Stones, Bent Mile Marker, Mosswater Toll Bridge, North Warden Camp) must contain at least 5 sentences.
- Every room description must be unique (no reused descriptions across rooms).
- Vnums must never appear in any in-world description text (room descriptions, mob descriptions, object descriptions, extra descriptions, exit descriptions).
- When a room uses a named exit (non-empty `<exit_keyword>`), that exit name must appear in at least one of: the room's main `<description>~`, an object that spawns in the room, or an `E` extra description in the room.
- Extra-description keyword chains must be anchored: at least one keyword in each chain must appear in the room's main `<description>~`.
- Room descriptions must use the area's color theme intentionally and consistently.
- Each room's main `<description>~` must end with exactly one trailing newline immediately before the terminating `~`.

---

## Integration Notes (Midgaard south / Kowloon north)
- Southward narrative hooks reference Midgaard guild contracts, caravan escorts, ranger writs, and Granite Arcade administrative postings.
- Northward hooks reference Kowloon covenant law, Iron Gate customs processing, Jade Magistracy tariff checks, and Courier Lantern Office relay dispatches.
- Forest NPC dialogue should reflect dual influence: Midgaard practicality and Kowloon civic formalism.
- The area functions as both frontier adventure and geopolitical corridor.
- Road wardens carry identification recognized by both cities; their dual loyalty creates role-playing opportunities and faction tension.
- Lore references should cross-link: Kowloon's Ashfall Monsoon and the forest's Ash Winters share the same volcanic cause (Cinderteeth eruptions), binding the two areas' histories together.
- Kowloon's Iron Gate (`14095`) faces "the caravan roads and the lawless wetland frontier" per the Kowloon plan. The forest's northern approach should reflect this: the last few rooms before Kowloon should show increasing signs of Kowloon's administrative presence (customs markers, inspection posts, Wardens of Iron patrol signs).
- Midgaard's Granite Arcade (`3009`) is the administrative hub for northern trade. The forest's southern approach should show Midgaard's practical civic character: ranger notice boards, guild contract postings, and warden supply caches.

---

## Compliance Checklist
- [x] Plan only (no area file generation)
- [x] Uses `docs/area_file_spec.md` constraints in planning
- [x] 500-room plan with all vnums used (`15500-15999`)
- [x] Irregular overall layout with explicit meandering road
- [x] Multi-color theme defined (no `@@k`, no background codes, no flashing)
- [x] Extensive lore connecting Midgaard (south) and Kowloon (north)
- [x] Doors explicitly specified with `EX_ISDOOR` bit, reset states, and key vnums
- [x] Room layout included (full 20x25 vnum grid)
- [x] Road mobs around level 20 in south approach; stronger farther away
- [x] Farthest pockets cap around level 100
- [x] Mob containment strategy enforced (`sentinel` and/or `no_mob` boundaries; all mobs get `stay_area`)
- [x] South connection identified: `15512` south to Midgaard `3009` (Granite Arcade)
- [x] North connection identified: `15994` north to Kowloon `14095` (Iron Gate)
- [x] Road adjacency verified: all consecutive road rooms are cardinally adjacent
- [x] NPC spec compliance notes included (act flags, long_descr/description format, boss/solo policy)
- [x] Room description requirements section included
- [x] Area header fully spec-compliant (`Q 16`, `O Virant~`, vnum bounds, level band)
