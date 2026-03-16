#!/usr/bin/env python3
"""Generate /home/user/acktng/area/midgaard.are from scratch.
Run: python3 gen_midgaard.py > /home/user/acktng/area/midgaard.are
"""
import sys

# ---------------------------------------------------------------------------
# CONSTANTS
# ---------------------------------------------------------------------------

# Wear flag values (derived from confirmed area file examples)
TAKE      = 8388608
HEAD      = 8;       FACE      = 16;      EAR       = 64;    NECK      = 128
SHOULDERS = 512;     ARMS      = 1024;    WRIST     = 2048;  HANDS     = 4096
FINGER    = 8192;    HOLD      = 32768;   ABOUT     = 65536; WAIST     = 131072
BODY      = 262144;  LEGS      = 1048576; FEET      = 2097152

def W(*flags):
    return sum(flags) + TAKE

# Item type constants
IT_LIGHT = 1; IT_SCROLL = 2; IT_WEAPON = 5; IT_ARMOR = 9
IT_POTION = 10; IT_KEY = 18; IT_FOOD = 19

# Item extra flags
MAGIC     = 64
BUCKLER   = 268435456
TWO_HAND  = 2147483648
WAND      = 536870912

# Room flags
ROOM_NADA    = 0
ROOM_INDOORS = 8
ROOM_SAFE    = 315

# Sector types
SECTOR_CITY   = 1
SECTOR_INSIDE = 11

# Grid parameters
ROWS      = 10
COLS      = 20
BASE_VNUM = 917
ALLEY_COLS = {0, 1, 18, 19}

def V(row, col):
    """Return vnum for grid position (row, col)."""
    return BASE_VNUM + row * COLS + col

# ---------------------------------------------------------------------------
# ROOM EXIT GENERATION
# ---------------------------------------------------------------------------

def exits_for(row, col, extra=None):
    """Return sorted list of (dir, desc, kw, locks, key, dest) for a room."""
    ex = []

    if row == 6:  # Shop Row A
        if col in ALLEY_COLS:
            ex.append((0, '', '', 0, -1, V(5, col)))   # N -> row 5
            ex.append((2, '', '', 0, -1, V(7, col)))   # S -> row 7
        else:
            ex.append((0, '', '', 0, -1, V(5, col)))   # N only (dead-end shop)

    elif row == 8:  # Shop Row B
        if col in ALLEY_COLS:
            ex.append((0, '', '', 0, -1, V(7, col)))   # N -> row 7
            ex.append((2, '', '', 0, -1, V(9, col)))   # S -> row 9
        else:
            ex.append((0, '', '', 0, -1, V(7, col)))   # N only (dead-end shop)

    elif row == 7:  # Service Street A
        if col > 0:          ex.append((3, '', '', 0, -1, V(7, col-1)))
        if col < COLS-1:     ex.append((1, '', '', 0, -1, V(7, col+1)))
        if col in ALLEY_COLS: ex.append((0, '', '', 0, -1, V(6, col)))
        ex.append((2, '', '', 0, -1, V(8, col)))

    elif row == 9:  # Southern Gate Road
        if col > 0:          ex.append((3, '', '', 0, -1, V(9, col-1)))
        if col < COLS-1:     ex.append((1, '', '', 0, -1, V(9, col+1)))
        if col in ALLEY_COLS: ex.append((0, '', '', 0, -1, V(8, col)))

    else:  # Main grid rows 0-5
        if row > 0:          ex.append((0, '', '', 0, -1, V(row-1, col)))
        if col < COLS-1:     ex.append((1, '', '', 0, -1, V(row, col+1)))
        ex.append((2, '', '', 0, -1, V(row+1, col)))  # always has south
        if col > 0:          ex.append((3, '', '', 0, -1, V(row, col-1)))

    if extra:
        ex.extend(extra)

    ex.sort(key=lambda x: x[0])
    return ex

# ---------------------------------------------------------------------------
# OUTPUT HELPERS
# ---------------------------------------------------------------------------

def o(s=''):
    sys.stdout.write(s + '\n')

def write_room(vnum, name, desc, flags, sector, exits):
    o('#%d' % vnum)
    o('%s~' % name)
    o('%s\n~' % desc)
    o('%d %d' % (flags, sector))
    for (d, edesc, ekw, locks, key, dest) in exits:
        o('D%d' % d)
        o('%s~' % edesc)
        o('%s~' % ekw)
        o('%d %d %d' % (locks, key, dest))
    o('S')

def write_mob(vnum, keywords, short, long_desc, description,
              act, aff, align, level, sex,
              hp_mod=0, ac_mod=0, hr_mod=0, dr_mod=0,
              ext_class=0, ext_clan=0, ext_race=0,
              ext_skills=0, ext_cast=0, ext_def=0,
              pipe_sm=0, pipe_wm=0, pipe_rm=0, pipe_ps=0,
              pipe_pc=0, pipe_r=0, pipe_s=0,
              loot_amount=0):
    o('#%d' % vnum)
    o('%s~' % keywords)
    o('%s~' % short)
    o('%s\n~' % long_desc)
    o('%s\n~' % description)
    o('%d %d %d S' % (act, aff, align))
    o('%d %d' % (level, sex))
    o('%d %d %d %d' % (hp_mod, ac_mod, hr_mod, dr_mod))
    o('! %d %d %d 7 %d %d %d' % (ext_class, ext_clan, ext_race, ext_skills, ext_cast, ext_def))
    o('| %d %d %d %d %d %d %d' % (pipe_sm, pipe_wm, pipe_rm, pipe_ps, pipe_pc, pipe_r, pipe_s))
    o('+ 0 0 0 0 0 0 0 0 0')
    o('l %d 0 0 0 0 0 0 0 0 0' % loot_amount)
    o('L 0 0 0 0 0 0 0 0 0')

def write_obj(vnum, keywords, short, description,
              item_type, extra_flags, wear_flags, item_apply,
              values, weight, level):
    o('#%d' % vnum)
    o('%s~' % keywords)
    o('%s~' % short)
    o('%s\n~' % description)
    o('%d %d %d %d' % (item_type, extra_flags, wear_flags, item_apply))
    o('%d %d %d %d %d %d %d %d %d %d' % tuple(values))
    o('%d' % weight)
    o('L')
    o('%d' % level)

# ---------------------------------------------------------------------------
# DATA STORAGE
# ---------------------------------------------------------------------------

ROOM_DB = {}   # vnum -> (name, desc, flags, sector, extra_exits)
MOB_DB  = {}   # vnum -> dict of mob kwargs
OBJ_DB  = {}   # vnum -> dict of obj kwargs

def def_room(row, col, name, desc, flags=ROOM_NADA, sector=SECTOR_CITY, extra=None):
    vnum = V(row, col)
    ROOM_DB[vnum] = (name, desc, flags, sector, extra or [])

def def_mob(vnum, **kwargs):
    MOB_DB[vnum] = kwargs

def def_obj(vnum, **kwargs):
    OBJ_DB[vnum] = kwargs

# ===========================================================================
# ROOMS — Row 0 (917-936): Northern Boulevard
# ===========================================================================

def_room(0,0, '@@WNorthwestern Watch-Post@@N',
    '@@WMidgaard\'s@@N northwestern corner is anchored by a squat sandstone watch-post whose '
    '@@y lantern@@N brackets burn amber on iron hooks at each cardinal corner. '
    'The wide intersection was laid out during the @@yLantern Reform@@N era to '
    'accommodate patrol-wagon turns, and the worn flagstones carry faded chalk lines '
    'from successive watch rotations. A limestone water-trough occupies the curb corner '
    'where porters and off-duty guards pause to refill their skins.')

def_room(0,1, '@@WSilver Marketwalk@@N',
    'This stretch of the upper avenue is lined with narrow-fronted silversmiths and assay '
    'offices whose display cases catch the @@y lantern@@N light in pale flashes. '
    'The walkway is wide enough for two handcarts abreast, and the cobbles underfoot have been '
    'worn smooth by decades of trade traffic. A tin notice plate nailed to a lintel post lists '
    'the current tariff schedule in cramped municipal script.')

def_room(0,2, '@@WCrimson Concourse@@N',
    'Dyed canvas awnings in deep crimson filter a ruddy half-shade over this stretch of the '
    'boulevard, giving the road its informal name. Militia handbills are pinned to the stone '
    'columns flanking the armament factors\' offices, and the air carries the sharp tang of '
    'oil-blacked iron. Negotiations between purchasing agents and smith representatives play '
    'out on folding tables set against the pillar-bases.')

def_room(0,3, '@@WAuburn Traverse@@N',
    'Amber-glazed brickwork from the old kiln district lines both sides of this traverse, '
    'its warm tone surviving every civic renovation since the Founding Generation. '
    'A worn boot-scraper and a public hand-pump stand at the east corner of the crossing. '
    'The faint smell of beeswax polish drifts from an open workshop door set flush with the wall.')

def_room(0,4, '@@WGloamvault Mouth@@N',
    'A heavy iron grating set flush with the pavement marks the civic descent into '
    'the Gloamvault, bolted with a registry padlock that changes quarterly. '
    '@@c Ward-sigils@@N etched on the arch stones overhead glow with a faint persistent '
    'coldness, a containment measure dating to the @@pViolet Compact@@N era. '
    'A faint chill rises through the grating, carrying the mineral smell of old deep stone.',
    extra=[(5,'','',0,-1,5031)])

def_room(0,5, '@@WGilded Promenade@@N',
    'Broad pavement flanks a double row of @@y gold-painted@@N iron lamp standards whose '
    'joined crossbars form an archway effect at each junction. '
    'Cloth pennants advertising guild services hang between the lamp poles, snapping '
    'in the passage wind that funnels down this straight run. '
    'A civic notice board near the midpoint carries current watch assignments and public-health '
    'orders sealed with the district warden\'s stamp.')

def_room(0,6, '@@WReliquary Threshold@@N',
    'A limestone checkpoint arch spans the avenue here, its keystone carved with the '
    '@@pViolet Compact@@N registry glyph that dates the checkpoint to the Containment Era. '
    '@@c Ward-plinths@@N stand to either side, their carved channels still sharp despite '
    'centuries of foot traffic brushing past. '
    'The toll-clerks\' booth between the plinths has been boarded shut since the '
    'Fortification Era, its function absorbed into the central watch-house.')

def_room(0,7, '@@WBeacon Mile North@@N',
    'Tall iron beacon standards rise at regular intervals along this section of the '
    'boulevard, their multi-tier lamp housings designed to illuminate the road in all '
    'weather. Brass distance markers set flush into the curb-stone count the '
    'paces to the civic center from this point. '
    'The even @@y lamplight@@N here makes it a preferred meeting spot for off-duty couriers '
    'exchanging dispatches before the morning distribution run.')

def_room(0,8, '@@WCartographers\' Cross@@N',
    'A bronze map-post at the center of this four-way crossing marks the historic '
    'survey datum from which @@WMidgaard\'s@@N street grid was originally measured. '
    '@@c Etched compass routes@@N on the stone plinth below the post show principal '
    'distances to the city\'s gates in chiselled numerals. '
    'The cartographers\' guild seal pressed into the iron base has been rubbed bright '
    'by generations of travellers checking distances before setting out.')

def_room(0,9, '@@WNorthern Gate@@N',
    'The great stone gatehouse of @@WMidgaard\'s@@N northern approach rises two stories '
    'above the avenue, its portcullis raised and chained during daytime hours. '
    '@@R Iron gate-bars@@N in the recessed alcoves stand ready to be swung across the '
    'passage should the watch bell sound a full closure. '
    'A guard\'s brazier burns beside the duty post, and a logbook on the adjacent '
    'lectern records every departure and arrival at the gate. '
    'The Great Northern Road stretches away beyond the arch into the dark fringe of '
    'the forest, its first league torchlit by city maintenance crews.',
    extra=[(0,'','',0,-1,3751)])

def_room(0,10, '@@WWatch-Station Row@@N',
    'A series of iron-shuttered guard posts occupy the ground floors of the buildings '
    'on this section of the northern boulevard, their narrow windows angled for '
    'line-of-sight coverage of the gate approach to the west. '
    'A @@R duty roster@@N is chalked on a board outside the main post, listing shifts '
    'through the next three days. '
    'The stable yard behind the station row provides overnight quarters for patrol horses '
    'and courier mounts cleared by the gate registry.')

def_room(0,11, '@@WSignal Station Alley@@N',
    'A short spur off the main boulevard leads to the city\'s signal-relay tower, '
    'a slender iron-and-stone construction used to relay lamp messages to the '
    'outlying watch-posts. '
    'Lamp operators\' equipment is stacked in iron-bound crates beside the tower door, '
    'and a logbook of transmitted signals hangs on a chain inside a weather-proof box. '
    'The alley itself smells of lamp oil and the faint char of burned signal-cloth.')

def_room(0,12, '@@WLamp-Post Row@@N',
    'The civic lamp-maintenance authority has concentrated an unusual density of '
    '@@y lamp-posts@@N along this stretch, making it the brightest section of the '
    'northern boulevard after dark. '
    'A lamp-lighter\'s supply box, padlocked and chained to one of the posts, '
    'contains reserve oil and wick spools for emergency maintenance. '
    'The heavy oil smell in the air is noticeable even by day.')

def_room(0,13, '@@WDungeon Steps@@N',
    'Heavy @@R iron warning chains@@N span the flagstone stairway here, marking the '
    'descent into the public detention dungeon maintained by the civic authority. '
    'A @@c ward-seal@@N carved on the arch stone above the entrance glows faintly with '
    'the suppression sigils renewed each quarter by the reliquary clerks. '
    'A duty log sealed in a weather-proof iron box records the name of every guard '
    'rotation assigned to the dungeon approach.',
    extra=[(5,'','',0,-1,2294)])

def_room(0,14, '@@WMarket Portico@@N',
    'A colonnade of thick stone columns fronts the northern market approach, '
    'providing covered space for vendors\' carts on market days. '
    'The columns carry carved emblems from four different civic administrations, '
    'each era\'s additions layered over the last in a sediment of official decoration. '
    'An old coin-exchange booth between two central columns is shuttered but intact, '
    'its iron shutter marked with the assay office\'s embossed seal.')

def_room(0,15, '@@WHeralds\' Arch@@N',
    'A carved stone arch marks the formal boundary of the herald district, '
    'the city\'s official zone for proclamation, registration, and public notice. '
    'The city arms are cut deep into the keystone in the style of the Standardisation '
    'Century, still crisp despite the weathering of the stone around them. '
    'Notice boards on both sides of the arch hold current civic announcements '
    'and sealed guild ordinances awaiting public reading.')

def_room(0,16, '@@WCoin Street North@@N',
    'The northern end of the money-changers\' and assayers\' district begins here, '
    'a stretch of the boulevard where iron grates over every cellar window hint at '
    'the counting houses below. '
    'The perpetual clink of weighed coin rises from unseen rooms, and the foot traffic '
    'here moves with purpose, each party intent on their transaction. '
    'A pair of armed ledger-keepers loiters near a cornerstone bearing the assay '
    'district\'s bronze boundary marker.')

def_room(0,17, '@@WTraders\' Junction@@N',
    'A major intersection where Coin Street meets the eastern avenue, this crossing '
    'sees a constant flow of factors, merchants, and bonded couriers. '
    '@@y Lantern poles@@N mark each of the four corners, and iron ground-rings for '
    'securing laden pack animals are sunk into the curb at regular intervals. '
    'Porters with barrows navigate between standing negotiators with practised impatience.')

def_room(0,18, '@@WBell Quarter North@@N',
    'The bell-founders and clocksmiths who occupy this stretch make the air here '
    'ring with the metallic resonance of test chimes from the upper workshops. '
    'A cracked practice bell rests in an alcove behind an iron grille, '
    'retired from service but still displayed as a mark of the quarter\'s trade history. '
    'The rhythm of hammering from behind closed shutters varies from the light tap of '
    'calibration tools to the heavy ring of raw casting work.')

def_room(0,19, '@@WEastern Promenade North@@N',
    'The northeastern corner of @@WMidgaard\'s@@N upper boulevard, where the wide '
    'northern avenue begins its transition into the narrower trade-district streets to the south. '
    'Iron plaques on the corner building mark the eastern tax district boundary, '
    'a civic demarcation that determines which ward collects levies on goods passing this point. '
    'The pavement here is freshly patched with lighter-coloured stone from a recent repair.')

# ===========================================================================
# ROOMS — Row 1 (937-956): Civic Quarter
# ===========================================================================

def_room(1,0, '@@WCivic Archives West@@N',
    'The western end of the civic quarter smells of old parchment and stone dust '
    'drifting from the archives building whose tall narrow windows face the street. '
    'Index plaques on the archive facade list the document categories held within: '
    'land surveys, tax ledgers, birth registries, and watch reports going back centuries. '
    'A queue of petitioners with leather satchels waits near the intake window '
    'for the morning filing session.')

def_room(1,1, '@@WBarracks Lane@@N',
    'The barracks for the northern watch occupies the ground and first floors of the '
    'long stone building running the length of this lane on the western side. '
    'Equipment racks are visible through the open ground-floor windows, '
    'and the smell of leather oil and steel polish drifts onto the street. '
    'Off-duty guards in grey watch-coats move between the barracks gate and the '
    'nearby mess hall, a low annexe at the lane\'s northern end.')

def_room(1,2, '@@WScribes\' Square@@N',
    'A small square off the civic avenue where three competing scribes\' offices face '
    'each other across worn flagstones, their ink-stained apprentices collecting and '
    'delivering documents in constant rotation. '
    'Public copying boards along one wall display standardised contract forms '
    'available at a posted fee. '
    'The sound of quills scratching and stamps pressing wax is audible from the open '
    'casement windows above.')

def_room(1,3, '@@WAssessors\' Row@@N',
    'Property assessors and land-registry clerks occupy the buildings on this row, '
    'their red-painted doors distinguishing them from the plain stone facades around them. '
    'Queues form outside each office in the morning hours as landholders bring their '
    'annual declarations for stamping and recording. '
    'A wall-mounted notice board outside the senior assessor\'s door lists all '
    'forthcoming public property auctions and disputed boundary hearings.')

def_room(1,4, '@@WPilgrim\'s Ascent@@N',
    'An open iron staircase rises from the street level here, its treads worn smooth '
    'by the passage of countless feet ascending to the elevated pilgrimage walkway above. '
    '@@c Ward-carvings@@N along the base of the staircase walls mark this as a '
    '@@pViolet Compact@@N-era structure, built when the city first formalised the '
    'pilgrim routes connecting its outer districts to the temple. '
    'A candle niche cut into the base stone holds a stub of tallow, recently lit.',
    extra=[(4,'','',0,-1,4681)])

def_room(1,5, '@@WScholar\'s Walk@@N',
    'The scholars\' walk is a broad covered arcade built along the front of the '
    'civic library and research annex, providing shelter for reading and debate. '
    'Stone benches line the inner wall, many occupied by individuals with open volumes '
    'or rolled manuscripts balanced on their knees. '
    'The arcade floor is inlaid with @@c compass-rose tiles@@N, '
    'a decorative remnant from the original construction that no subsequent resurfacing '
    'has managed to entirely cover.')

def_room(1,6, '@@WTannery Row@@N',
    'The tanning operations on this row have been here since the Founding Generation, '
    'and the smell makes that clear. Skins stretch on frames in the open yards '
    'behind the street-front buildings, and the chemical tang of bark-pits '
    'pervades the immediate vicinity. '
    'Trade buyers from the leather-workers\' guild wait outside the largest yard\'s '
    'gate for the morning inspection round.')

def_room(1,7, '@@WMason\'s Yard@@N',
    'An open yard behind a low iron fence holds dressed stone blocks awaiting '
    'placement in the ongoing civic repair programme. '
    'Masons\' marks are chiselled into each block\'s face, identifying the cutter '
    'and the quarry source for the ward maintenance ledger. '
    'The yard supervisor\'s shed at the far corner displays the current work '
    'schedule on a chalk board visible through its open door.')

def_room(1,8, '@@WIronwright\'s Lane@@N',
    'The ironwrights\' guild maintains its eastern workshop annexe along this lane, '
    'filling the air with the clang of hammers on anvils and the hiss of quench-water. '
    'Iron window-grilles and decorative bracket-work line the facade of the '
    'guild building, each piece a demonstration of current members\' skill. '
    'Commission orders are posted in a glass-fronted notice case beside the guild door.')

def_room(1,9, '@@WSky-Watch Platform@@N',
    'An iron-railed viewing platform rises above the street here on a stone column, '
    'accessible by a locked external stair maintained by the civic weather service. '
    '@@c Astrological markings@@N are etched into the platform\'s stone railing, '
    'used by the watch service to calibrate seasonal predictions. '
    'Below the column, a brass speaking-tube connects to the weather-service office '
    'for ground-level reporting without descending the stair.',
    extra=[(4,'','',0,-1,5106)])

def_room(1,10, '@@WCommander\'s Road@@N',
    'The watch commander\'s office occupies a prominent position on this stretch, '
    'its facade slightly grander than the surrounding civic buildings '
    'with carved pilasters framing the double doors. '
    'Officers in dress uniform pass in and out at regular intervals, '
    'and a mounted dispatch rider waits at the iron ring beside the door. '
    'The road here is well-swept and free of the merchant clutter that characterises '
    'the western sections of the civic quarter.')

def_room(1,11, '@@WChandler\'s Square@@N',
    'A small square where chandlers and wax merchants cluster their stalls '
    'around a central trough used for dipping and cooling tapers. '
    'The air carries the sweet heavy scent of beeswax and tallow, '
    'and rows of hanging candles dry on wooden frames erected across the square. '
    'The @@y golden sheen@@N of fresh wax catches every light source and makes '
    'this the best-illuminated open space in the civic quarter.')

def_room(1,12, '@@WGlassblower\'s Way@@N',
    'Glassblowers occupy the upper floors of the buildings along this stretch, '
    'their furnace glow visible through the gaps in the wooden shutters. '
    'Finished glass panels in the ground-floor display windows show samples of '
    'tinted, etched, and patterned work available from the studios above. '
    'The ambient warmth here even in winter months is a side effect of the '
    'furnaces running through most of the day.')

def_room(1,13, '@@WDyer\'s Crossing@@N',
    'The dyers\' district begins here, and the evidence is immediate: '
    'coloured water runs in the gutter channel, and cloth hangs drying '
    'from every available horizontal line between the buildings. '
    '@@R Reds@@N, @@y yellows@@N, and deep @@p violets@@N dominate the current '
    'production, and the acrid smell of mordants bites at the nose. '
    'Dye-masters in stained aprons argue over colour consistency at the crossing corner.')

def_room(1,14, '@@WCentral Civic Plaza@@N',
    'The primary open space of the civic quarter is paved in a pattern of '
    '@@W white@@N and @@c grey@@N flagstones arranged in the traditional ward-grid design. '
    'A fountain at the plaza\'s centre has been dry for two years, '
    'its basin used as a message-drop by the courier guild. '
    'The surrounding buildings include the district magistrate\'s hall, '
    'the licensing office, and the civil registry, their facades all '
    'bearing the matching civic-authority livery of grey paint and brass numerals.')

def_room(1,15, '@@WScrivener\'s Alley@@N',
    'The scriveners\' specialist alley runs between the civic plaza and the '
    'eastern precinct, occupied by contract specialists who handle documents '
    'too complex for the general scribes\' offices. '
    'Their signs advertise services in precise language: '
    'deed transfers, guild charters, oath-ledger transcriptions, and will registration. '
    'The narrow alley concentrates foot traffic and the sound of competing '
    'apprentices calling out their masters\' specialisations to passing clients.')

def_room(1,16, '@@WClothier\'s Lane@@N',
    'Bolt-cloth merchants and tailors occupy both sides of this lane, '
    'their window displays showing current fabrics against which clients '
    'may compare samples from the market. '
    'Cut ends of cloth line the pavement outside the larger shops, '
    'free for the taking by those who need rags or small patches. '
    'A guild hall at the lane\'s eastern end controls quality standards '
    'for all fabric sold in this district.')

def_room(1,17, '@@WCobbler\'s Square@@N',
    'Cobblers and leather-stitch workers cluster around this small square, '
    'their sample boots and sandals displayed on iron pegs along the front walls. '
    'The sound of lasting hammers and stitching awls carries from every '
    'open workshop door, and the smell of fresh-cut leather is pervasive. '
    'A communal stretching rack in the centre of the square is shared '
    'by several workshops for large panel work.')

def_room(1,18, '@@WPerfumer\'s Row@@N',
    'The perfumers and herbalists on this row have transformed their section '
    'of the civic quarter into an olfactory contrast with the tanners\' lane nearby. '
    'Dried flower bundles hang from the lintels, and the air is layered with '
    'competing scents from the open bottles displayed in each window. '
    'A civic notice requires all perfume stalls to keep their display '
    'bottles stoppered during the market inspection hours.')

def_room(1,19, '@@WEastern Precinct Road@@N',
    'The eastern end of the civic quarter narrows here as the administrative '
    'buildings give way to the warehouse district beginning around the next corner. '
    'A milestone post at the road\'s edge marks the distance to the eastern gate '
    'and the toll-post on the desert road. '
    'Two civic officials in grey carry a sealed chest under escort between '
    'the tax collection office and the district treasury building.')

# ===========================================================================
# ROOMS — Row 2 (957-976): Market Quarter
# ===========================================================================

def_room(2,0, '@@WProvisioner\'s Lane West@@N',
    'The western approach to the market quarter is dominated by provisioners\' '
    'warehouses whose loading doors face the lane, kept open from before dawn '
    'until the evening bell. '
    'Sacks of grain and dried goods line the pavement under canvas awnings '
    'while factor-clerks work through consignment tallies. '
    'The lane smells of grain dust, dried herbs, and the particular oily '
    'note of preserved foodstuffs in bulk storage.')

def_room(2,1, '@@WSpice Market Row@@N',
    'The spice merchants\' row fills the air with a dense layering of '
    'cinnamon, pepper, and unfamiliar foreign aromatics from the trade routes. '
    'Coloured powder spills from imperfectly sealed bags on the lower shelves '
    'and stains the flagstones in patches of ochre and rust. '
    'Buyers from the larger cookhouses negotiate with the merchants in a '
    'mixture of standard and trade-route dialects.')

def_room(2,2, '@@WFishmonger\'s Way@@N',
    'River fish and salted sea-catch are laid out on ice-packed slate slabs '
    'along this way, the @@y lantern@@N light giving the wet scales a shifting gleam. '
    'Cats loiter with patient expertise at the base of every stall, '
    'and the fishmongers\' guild apprentice sweeps shaved ice and scales '
    'from the pavement with a wide broom at regular intervals. '
    'The smell is assertive and distinct from every other part of the market.')

def_room(2,3, '@@WButcher\'s Crossing@@N',
    'The butchers\' crossing marks the transition between the fish trade and '
    'the red-meat stalls, distinguished by the change from wet-slate tables '
    'to block-and-cleaver arrangements. '
    'Sawdust covers the pavement in front of the main stalls to absorb '
    'drippings, and the crossing itself is washed down hourly. '
    'A notice from the civic health authority is posted on the corner '
    'building specifying the permitted hours for animal delivery to the district.')

def_room(2,4, '@@WButcher\'s Plaza@@N',
    'The main plaza of the meat market is surrounded by butchers\' shops '
    'on three sides, their apprentices calling prices from the doorways. '
    'A central stone basin serves as a refuse collection point during '
    'market hours, emptied by the civic sanitation crew twice daily. '
    'The plaza is noisier and more chaotic than the surrounding streets, '
    'with buyers, sellers, and delivery hands all competing for space '
    'around the central cutting tables.')

def_room(2,5, '@@WApothecary Lane@@N',
    'The apothecaries\' lane provides a welcome change in atmosphere after '
    'the intensity of the meat market, its shuttered windows and measured '
    'trade creating an almost contemplative quiet. '
    'Coloured glass bottles in the display cases catch the light, and '
    'customers speak in low voices while waiting for prepared compounds. '
    'A guild examiner\'s seal is posted on the door of each licensed shop, '
    'indicating that the current stock has passed the quarterly purity inspection.')

def_room(2,6, '@@WHerbalist\'s Square@@N',
    'Dried herb bundles are everywhere in this square: hanging from the '
    'lintels, piled on the stall surfaces, wound into braids on display racks. '
    'The combined scent is an intense botanical blend that sharpens '
    'noticeably in warm weather. '
    'Herbalists from the outer farms attend the morning market here, '
    'bringing fresh stock to supplement the merchants\' dried-goods inventory.')

def_room(2,7, '@@WTailor\'s Row@@N',
    'The tailors\' row is a prosperous-looking stretch with polished wood '
    'frontages and display mannequins in the wider windows. '
    'Sample garments hang in the doorways and the sound of cutting shears '
    'and the hiss of steam pressing irons carries from the workshops inside. '
    'Measurement notices listing current commission times are posted in '
    'neat frames beside each shop entrance.')

def_room(2,8, '@@WWoodworker\'s Alley@@N',
    'Woodworkers and carpenters occupy this alley, their plane shavings '
    'curled on the pavement outside every open workshop door. '
    'The smell of fresh-cut timber competes with linseed oil and '
    'the sharper note of lacquer from the finishing room at the alley\'s end. '
    'Joinery samples, tool handles, and carved panels lean against the walls '
    'as display pieces for passing trade.')

def_room(2,9, '@@WMarket Gate East@@N',
    'The eastern approach to the main market district passes through a modest '
    'gate arch maintained by the market association, whose monthly dues '
    'pay for the @@y lanterns@@N burning at the arch posts day and night. '
    'A market warden in a grey and @@y yellow@@N tabard checks incoming '
    'cartloads against the registered vendor list at a small guardhouse to one side. '
    'Bulletin boards on either side of the arch post the week\'s market schedule '
    'and any changes to stall assignments.')

def_room(2,10, '@@WCaravan Approach@@N',
    'The road widens noticeably here as it approaches the caravan quarter, '
    'the extra paving laid to accommodate the larger wagons and draft teams '
    'that are the lifeblood of @@WMidgaard\'s@@N long-distance trade. '
    'Wheel ruts in the stone mark the most heavily used paths, '
    'and the smell of dung, straw, and axle grease becomes apparent '
    'well before the caravan square itself comes into view.')

def_room(2,11, '@@WCaravan Square@@N',
    'The open heart of @@WMidgaard\'s@@N caravan quarter spreads across '
    'a broad expanse of worn flagstone, large enough to hold a dozen '
    'fully laden wagon trains at once with room for their teamsters to manoeuvre. '
    'Caravans from the eastern desert routes, the northern forest roads, '
    'and the western pass all converge here, and the square is rarely quiet '
    'at any hour — at dawn the departures begin, at dusk the arrivals crowd in. '
    'Broker stalls line the southern and eastern edges, handling commissions '
    'for every major trade route the city serves. '
    'A central iron post carries route markers for outgoing wagons: '
    '@@y north to the forest gates@@N, @@R east to the desert road@@N, '
    '@@c west toward Roc Road and Kiess@@N. '
    'The ground here has been hardened by decades of iron wheel traffic '
    'and the hooves of countless draft animals.')

def_room(2,12, '@@WLivestock Market@@N',
    'Pens of livestock for sale or trade fill this area of the caravan quarter: '
    'draft oxen, pack mules, riding horses, and occasional exotic '
    'animals destined for the estates of wealthy buyers. '
    'The noise and smell are considerable, and the livestock brokers '
    'move between pens with clipboards of registration papers. '
    'A civic veterinary post marks animals that have passed the health inspection '
    'required before they can enter the city proper.')

def_room(2,13, '@@WWagon Masters\' Road@@N',
    'The wagon masters\' administration road connects the caravan square '
    'to the trade warehouses and loading bays of the outer ring. '
    'Painted kerb-markings indicate which lane is reserved for '
    'inbound heavy traffic and which for outbound light vehicles. '
    'A charter post at the road\'s midpoint displays the current fees '
    'for warehouse use, wagon impoundment, and driver registration.')

def_room(2,14, '@@WGoods Exchange Court@@N',
    'The goods exchange court is a semi-covered area where caravan '
    'masters and wholesale buyers negotiate transfers without '
    'the cargoes needing to leave the wagon. '
    'Tally clerks move between groups, recording transaction details '
    'in bound ledgers that serve as the exchange\'s official record. '
    'The court\'s stone columns are carved with the marks of the '
    'eight original trade guilds that funded its construction.')

def_room(2,15, '@@WBroker\'s Crossing@@N',
    'The main broker crossing is the busiest intersection in the '
    'caravan quarter, where trading agents from three directions '
    'converge to compare route intelligence and cargo values. '
    '@@y Lantern brackets@@N at each corner post carry coloured '
    'glass panels indicating the active broker districts: '
    '@@R red@@N for desert trade, @@c blue@@N for forest goods, '
    '@@y gold@@N for internal city commerce.')

def_room(2,16, '@@WPackhorse Lane@@N',
    'The narrow lane used by packhorse teams delivering to the '
    'upper-level warehouses runs along the eastern edge of the caravan '
    'quarter, its pavement deeply grooved by decades of hoof traffic. '
    'Iron rings for tethering are sunk into the wall at regular '
    'intervals, and a water trough and hay rack stand at the lane\'s end. '
    'Animal handlers move with practiced efficiency, '
    'attaching and releasing pack loads without slowing the queue.')

def_room(2,17, '@@WStable Yard Road@@N',
    'The road outside the main caravan stable yard is perpetually '
    'busy with grooms, farriers, and feed suppliers making their '
    'rounds of the horse lines. '
    'The stable yard gate is large enough to admit a full four-in-hand '
    'wagon team, and the keeper\'s house beside it displays the current '
    'overnight boarding rates on a wooden price board. '
    'Straw blows across the road in irregular gusts from the open yard interior.')

def_room(2,18, '@@WOutrider\'s Row@@N',
    'Scout and outrider services available for hire to caravan masters '
    'advertise along this row with painted signs showing their route '
    'specialisations and guild affiliations. '
    'The riders themselves are often visible on this stretch, '
    'maintaining their equipment or reviewing maps pinned to the wall '
    'of the outrider guild\'s open-front posting office. '
    'A chalk availability board outside the office shows which '
    'scouts are currently contracted and which are free.')

def_room(2,19, '@@WCaravan Gate East@@N',
    'The eastern perimeter of the caravan quarter ends at this gate, '
    'which marks the boundary between the caravan district and the '
    'trade road leading to the eastern desert approaches. '
    'Heavy bollards of black iron are set into the road to prevent '
    'unauthorised heavy vehicle entry outside licensed hours. '
    'A customs clerk in a booth on the south side stamps manifests '
    'for outbound caravans and records incoming declarations.')

# ===========================================================================
# ROOMS — Row 3 (977-996): Working Quarter
# ===========================================================================

def_room(3,0, '@@WWestern Workers\' Road@@N',
    'The western workers\' road separates the craftsmen\'s district from the '
    'outer market wall, its wide pavement suited to the heavy material deliveries '
    'that supply the workshops beyond. '
    'Timber and stone move along this road in constant cycle, and the '
    'carters\' guild maintains a duty post here to coordinate traffic. '
    'A public drinking fountain at the road\'s western end is one of the '
    'oldest surviving pieces of civic infrastructure in this district.')

def_room(3,1, '@@WMillwright\'s Square@@N',
    'Millwrights and gear-smiths cluster around this square, displaying '
    'sample gears, cams, and escapement mechanisms on iron-frame boards '
    'outside their workshop doors. '
    'The metallic clicking and ratcheting of test mechanisms in progress '
    'provides a constant background rhythm. '
    'A guild patent board lists the registered mechanism designs of '
    'current members, protecting their innovations from unlicensed copying.')

def_room(3,2, '@@WPotter\'s Lane@@N',
    'Clay dust coats the walls and pavement of this lane, where potters\' '
    'wheels hum and the smell of damp earth and kiln-fired clay '
    'fills every breath. '
    'Sample pots and crockery in various stages of completion line the '
    'open window sills, and the guild journeyman marks pressed into '
    'each piece\'s base are visible to anyone who examines them. '
    'The kiln smoke from the lane\'s end houses drifts east with the prevailing wind.')

def_room(3,3, '@@WMudlark\'s Road@@N',
    'The road takes its name from the clay workers who historically '
    'transported raw clay from the riverbank to the potters\' district along this route. '
    'Modern paving has replaced the old mud-track, but the name persists. '
    'Wheelbarrow marks in the softer sections of the paving near the '
    'building edges speak to the clay trade that still passes here daily. '
    'The district boundary post at the road\'s corner is still marked '
    'with the old mudlark trade symbol.')

def_room(3,4, '@@WCrossroads of the Commons@@N',
    'The central crossroads of the working quarter sees an unceasing flow '
    'of craftsmen, apprentices, material carriers, and guild officials '
    'in all directions through the working day. '
    'A communal notice board at the crossing\'s center post carries '
    'job listings, apprenticeship openings, and guild announcements '
    'from across the district. '
    'The paving at the crossing is heavier-gauge than the surrounding roads, '
    'reinforced to handle the constant heavy foot and cart traffic.')

def_room(3,5, '@@WRope-Maker\'s Walk@@N',
    'Rope-makers occupy this walk, with long lines of twisted hemp '
    'stretched between posts the full length of the road as work '
    'progresses on large orders. '
    'Pedestrians must duck under or step around the work-in-progress, '
    'and the smell of hemp oil and raw fibre is heavy in the air. '
    'A guild quality board displays the current grade standards '
    'for rope sold in the district, verified by the civic engineer\'s office.')

def_room(3,6, '@@WNail-Pounders\' Row@@N',
    'The nail-smiths\' row produces the rhythmic hammering that '
    'sets the pace for the entire working quarter from before dawn. '
    'Iron nails of every size are displayed in trays outside each shop, '
    'and bulk buyers with carts wait at the loading entrance. '
    'The guild seal on the corner building certifies that nails sold '
    'here meet the standard dimensions used by the civic construction authority.')

def_room(3,7, '@@WBarrelwright\'s Square@@N',
    'Completed barrels and casks of every size stand outside the cooperage '
    'workshops that line this square, awaiting transport to the '
    'merchants, brewers, and provisioners who ordered them. '
    'The sound of hoop-setting hammers and the creak of stave bending '
    'fills the air throughout the working day. '
    'A cooperage standard gauge, a brass template for checking barrel '
    'dimensions, is mounted on the square\'s central pillar.')

def_room(3,8, '@@WRiveter\'s Lane@@N',
    'Iron riveters and metalwork joiners make this lane ring with the '
    'percussion of their trade, the heavy thud of setting hammers '
    'audible from the street before the lane itself comes into view. '
    'Large structural components for civic construction projects '
    'are assembled and loaded here, requiring the lane to be temporarily '
    'blocked for the loading operation at least once daily. '
    'The guild foreman\'s office at the lane\'s end coordinates the '
    'day\'s work schedule on a board visible from the street.')

def_room(3,9, '@@WEastern Workers\' Road@@N',
    'The eastern end of the working quarter road sees slightly lighter '
    'traffic than the western counterpart, as most heavy deliveries '
    'arrive from the west along the main caravan route. '
    'Light manufacturing — printmaking, bookbinding, and fine leatherwork — '
    'occupies the eastern buildings, and the pace here is noticeably '
    'calmer than the percussion-heavy workshops to the west. '
    'A public bench and a small stone pool with clean water provide a '
    'brief rest point for workers in the adjacent buildings.')

def_room(3,10, '@@WWarden\'s Road West@@N',
    'The watch patrol road runs along this section of the working quarter, '
    'providing the wardens with a clear line of sight between the main '
    'crossroads and the outer district boundary. '
    'Patrol markers, small iron pegs driven into the paving at intervals, '
    'guide the night watch rotation along their established route. '
    'The warden\'s sub-post at the road\'s corner is staffed '
    'through all three watches, its iron lantern burning around the clock.')

def_room(3,11, '@@WSlate-Tile Alley@@N',
    'This alley is named for the distinctive slate-tile paving laid '
    'here by a private initiative during the Standardisation Century, '
    'still intact and far more even than the civic stone around it. '
    'Small workshops for precision craftwork — engravers, clockmakers, '
    'and seal-cutters — occupy the tight building spaces on both sides. '
    'The alley is narrow enough that two loaded porters cannot pass each other '
    'without one turning sideways.')

def_room(3,12, '@@WRow of the Weavers@@N',
    'The weavers\' district occupies this stretch with a colour and '
    'texture that softens the industrial character of the surrounding quarter. '
    'Finished cloth hangs drying from upper-floor rods, and the '
    'rhythmic click of working looms carries from the interior of each building. '
    'Sample bolts displayed in the ground-floor windows show the current '
    'pattern range, with the weavers\' guild quality mark woven into every hem.')

def_room(3,13, '@@WChandler\'s Lane@@N',
    'A secondary chandler\'s operation works out of this lane, '
    'supplying the working quarter\'s high consumption of tallow lights '
    'and oil lamps for the long hours of the craftsmen\'s shifts. '
    'Large candle molds and wax-pouring equipment are stacked outside '
    'the workshop on most mornings while the floor is being cleaned. '
    'The lane\'s heavy wax smell is less pleasant than the beeswax '
    'quarter to the north, as most stock here is utilitarian tallow.')

def_room(3,14, '@@WHeart of the Commons@@N',
    'The informal gathering place of the working quarter is this widened '
    'section of road where the lanes from the main craft districts all '
    'converge, bringing apprentices and journeymen together at the '
    'end of the working day. '
    'A large public notice board here is managed by the workers\' '
    'welfare association and carries apprenticeship postings, '
    'housing notices, and mutual-aid announcements. '
    'At midday the space fills with workers eating from paper-wrapped provisions '
    'bought at the nearby stalls.')

def_room(3,15, '@@WPresser\'s Walk@@N',
    'Steam pressing operations in the garment district produce visible '
    'wisps of steam that drift across this walk from the '
    'pressing-house vents set low in the building walls. '
    'Garments on wooden forms are wheeled in and out of the pressing-houses '
    'on delivery carts, and the humid warmth of the walk is noticeable '
    'even in cold weather. '
    'A garment association office at the walk\'s end handles quality '
    'disputes and maintains the district\'s pressing standards register.')

def_room(3,16, '@@WDye-Master\'s Row@@N',
    'Senior dye-masters maintain their premium operations on this row, '
    'catering to the estates and wealthy merchants who require exact '
    'colour matching for commission work. '
    'Colour sample cards in the window displays are precise to a degree '
    'that the common market stalls cannot match, each shade named and '
    'referenced against the guild colour register. '
    'The row smells of complex dye chemistry: '
    'mordants, fixatives, and the base pigment compounds '
    'that distinguish master-grade work.')

def_room(3,17, '@@WSpinner\'s Square@@N',
    'Spinning wheels and hand-spindles work in the open-air '
    'beneath the square\'s central covered arcade, '
    'producing thread for the weavers further along the district. '
    'The rhythmic whir of spindles is constant throughout the working day, '
    'and the air carries drifting wisps of raw fibre. '
    'Grade markers on the thread bundles indicate the '
    'twist count and fibre origin, recorded in the guild stock ledger.')

def_room(3,18, '@@WSeamstress Row@@N',
    'Seamstresses\' workshops face onto this row with double-wide '
    'windows designed to maximise natural light for close needlework. '
    'Commission boards outside each shop carry waiting times '
    'for alterations, mending, and bespoke garment work. '
    'The sound of singing from the interior of the larger workshops '
    'is a noted feature of this row, the traditional working songs '
    'keeping a shared pace among the workers inside.')

def_room(3,19, '@@WWarden\'s Road East@@N',
    'The eastern boundary of the working quarter is marked by a '
    'final patrol road that separates the craftsmen\'s district from '
    'the southern commons beginning beyond. '
    'A patrol sub-post on the corner maintains oversight of both districts '
    'and coordinates with the adjacent warden posts via the lamp-signal '
    'system rigged between the upper-floor windows. '
    'The paving quality changes noticeably south of this road '
    'as civic maintenance resources are distributed less evenly.')

# ===========================================================================
# ROOMS — Row 4 (997-1016): Southern Commons
# ===========================================================================

def_room(4,0, '@@WIrongate Road West@@N',
    'The western approach to the southern commons runs past the irongate '
    'warehouse complex whose @@R iron-bound@@N loading doors line the '
    'road\'s northern side. '
    'Heavy goods wagons from the working quarter deliver to the warehouses '
    'here before the morning market opens, blocking the road in shifts. '
    'A gate-keeper\'s lodge at the warehouse complex entrance checks deliveries '
    'against a day-book before allowing entry to the loading yard.')

def_room(4,1, '@@WCoppice Lane@@N',
    'Coppiced timber poles for scaffolding and fuel wood are stacked '
    'along this lane from the lumber merchants whose yards back onto it. '
    'The smell of green wood and bark dust is pervasive, and the '
    'ground is always slightly soft with sawdust and wood pulp. '
    'A lumber grader\'s post at the lane\'s midpoint is where quality '
    'assessors mark each pole\'s grade before it enters the supply chain.')

def_room(4,2, '@@WOld Wall Road@@N',
    'The road follows the line of @@WMidgaard\'s@@N oldest defensive wall, '
    'long since incorporated into the city\'s fabric but still visible '
    'as a change in building age and stone type along the road\'s southern side. '
    'Sections of the original wall face, much older than the '
    'Founding Generation stonework, protrude between later construction. '
    'A heritage marker placed by the civic archive designates this '
    'as a protected historic streetscape.')

def_room(4,3, '@@WSouthern Square@@N',
    'A secondary market square serving the southern commons district '
    'operates here with a more working-class character than the '
    'main market to the north. '
    'Stall goods lean toward provisions, tools, and used equipment '
    'rather than the premium goods traded in the upper market. '
    'The square is noisy with haggling voices, and the central pump '
    'sees constant use from the residents of the surrounding tenements.')

def_room(4,4, '@@WCrossroads of the South@@N',
    'The southern district\'s main crossroads is wider than the streets '
    'that feed it, with a painted hub-ring on the paving directing '
    'cart traffic in a one-way circuit around the central point. '
    'Watch posts at two of the four corners maintain visibility '
    'across the full width of the intersection. '
    'The district\'s public notice board here carries welfare postings '
    'and civic labour requests alongside guild announcements.')

def_room(4,5, '@@WWayfarer\'s Lane@@N',
    'The lane used by travellers moving between the southern gate '
    'district and the central city passes through this section, '
    'with inn signs and guest-house boards marking the buildings '
    'that cater to the transit trade. '
    'Travellers with laden mule-packs and worn road boots are a '
    'common sight here regardless of the hour. '
    'A public noticeboard outside the largest guest-house carries '
    'travel advisories and road condition reports from the gate wardens.')

def_room(4,6, '@@WCourier\'s Row@@N',
    'The city\'s bonded courier stations occupy this row, '
    'their horses always ready and their riders identifiable by the '
    '@@R red-banded@@N leather satchels strapped across their chests. '
    'Dispatch times and route fees are posted on the external boards, '
    'and a constant trickle of clients arrives with sealed packages '
    'and correspondence for the various route services. '
    'The row smells of horse, saddle leather, and the wax used for '
    'sealing dispatches.')

def_room(4,7, '@@WStonebrick Walk@@N',
    'The oldest surviving stretch of paved road in the southern district '
    'uses the distinctive small-format stonebrick that predates the '
    'civic standardisation of road surfaces. '
    'The pattern shows repair work from at least six different eras, '
    'each generation of masons choosing slightly different stone. '
    'A civic preservation notice is mounted on the wall of the '
    'corner building indicating that the stonebrick paving cannot '
    'be replaced without the archive\'s approval.')

def_room(4,8, '@@WLantern Row South@@N',
    'The southern district\'s secondary lamp maintenance route runs here, '
    'its @@y lantern posts@@N fewer and older than the northern boulevard '
    'standards but still providing functional illumination. '
    'Several posts show patched repairs to the iron casting, '
    'evidence of the age of the infrastructure in this part of the city. '
    'A lamp-lighter\'s hand-cart is parked against one post, '
    'its oil cans indicating a maintenance round in progress.')

def_room(4,9, '@@WIrongate Road East@@N',
    'The eastern end of the irongate road approaches the district boundary '
    'where the warehouse zone gives way to the beginning of the '
    'temple approach to the south. '
    'The change in character is marked: the warehouse district\'s functional '
    'iron architecture transitions to the stone-and-mortar civic style '
    'that characterises the temple quarter buildings ahead. '
    'A district boundary post here carries the ward numbers for '
    'both the commercial district and the temple quarter.')

def_room(4,10, '@@WTemple Quarter West@@N',
    'The western boundary of the temple quarter begins at this point, '
    'indicated by the change in flagstone colour from the grey of the '
    'commons district to the @@W white-veined@@N stone used throughout '
    'the temple precinct. '
    'The ambient noise level drops noticeably here, civic ordinances '
    'restricting commercial activity within the temple quarter boundary. '
    'A gate post without a gate — symbolic only — marks the formal entrance '
    'to the precinct, its carved arch dating to the Containment Era.')

def_room(4,11, '@@WReliquary Lane@@N',
    'The reliquary transport lane connects the western approaches to the '
    'temple\'s collection storage on the eastern side of the precinct. '
    'Sealed cases marked with @@p reliquary custody symbols@@N are sometimes '
    'visible being transferred by uniformed temple servants. '
    'The lane\'s walls carry repeated @@c ward-inscriptions@@N '
    'at regular intervals, maintaining a passive containment field '
    'along the route as required by the Violet Compact protocols.')

def_room(4,12, '@@WCandle-Maker\'s Row@@N',
    'The candle-makers who supply the temple\'s substantial wax consumption '
    'maintain their shops on this row, close to the precinct\'s main entrance. '
    'Votive candles, altar lights, and the large devotional pillars '
    'used in the main sanctum are produced here in a range of sizes '
    'and scents specified by the temple authority. '
    'The smell here is clean beeswax, quite different from the '
    'tallow operations in the working quarter.')

def_room(4,13, '@@WIncense Crossing@@N',
    'The crossing of the two main temple quarter approach roads is '
    'permanently marked by a faint trace of incense smoke drifting '
    'from the devotional burning that has continued here for generations. '
    'Small stone incense niches are cut into the bases of the corner '
    'posts, tended by voluntary devotees who replace the burning cones '
    'from before dawn to after dusk. '
    'The crossing has become an informal gathering point for pilgrims '
    'orientating themselves on first arrival in the precinct.')

def_room(4,14, '@@WPilgrim\'s Rest@@N',
    'Benches of polished stone line both sides of this wider section '
    'of the temple approach, providing rest for pilgrims who have '
    'walked long distances to reach the city. '
    'A water bowl and ladle are maintained by the temple welfare office '
    'in a stone alcove with a carved @@pprotection glyph@@N above it. '
    'The rest stop\'s existence is recorded in the temple\'s welfare charter '
    'as one of the original public services of the Founding Generation compact.')

def_room(4,15, '@@WShrine Walk@@N',
    'A processional walk lined with small wayside shrines forms a '
    'formal approach route to the temple district from the south. '
    'Each shrine represents a different era of the city\'s history, '
    'their offerings and dedications varying from practical civic tokens '
    'to the elaborate @@preliquary frames@@N of the Containment Era. '
    'Pilgrims move slowly here, pausing at each shrine in sequence '
    'according to their particular devotional tradition.')

def_room(4,16, '@@WPrayer Lane@@N',
    'The quietest lane in the temple quarter is kept so by an '
    'informal civic convention that has persisted for generations: '
    'no commercial activity, no raised voices. '
    'Priests and devotees use this lane as a meditation walk, '
    'and the buildings on both sides are temple-affiliated residences '
    'whose occupants observe the quiet hours contract. '
    'The @@c ward-markings@@N on the lane\'s walls include a specific '
    'sound-dampening inscription at each end.')

def_room(4,17, '@@WSanctuary Road@@N',
    'The road leading to the main temple sanctuary is the most '
    'formally maintained stretch of paving in the entire quarter, '
    'resurfaced after every significant festival and kept clean '
    'by a dedicated temple grounds team. '
    'Carved stone bollards line both sides, preventing vehicles '
    'from the road surface during the major pilgrimage days. '
    'The building facades here have been uniformly rendered in '
    '@@W white mortar@@N, creating a visual corridor toward the temple ahead.')

def_room(4,18, '@@WCloister Walk@@N',
    'A covered cloister walk provides a sheltered route between '
    'the temple approach and the inner precinct buildings. '
    'Stone columns support the cloister vault, their bases carved '
    'with @@pdevotional scenes@@N from the city\'s founding myths. '
    'The walk is used by priests in procession and by pilgrims '
    'seeking the quieter route to the main sanctum without passing '
    'through the busier outer court.')

def_room(4,19, '@@WTemple Quarter East@@N',
    'The eastern boundary of the temple quarter opens onto the '
    'trade routes connecting to the eastern gate and the desert road. '
    'The transition here is more abrupt than the western boundary: '
    'the @@W white stone@@N of the precinct meets the practical grey '
    'of the commercial district with only a boundary post between them. '
    'Temple officials staff a small registry office at the boundary '
    'to monitor commercial activity approaching the precinct.')

# ===========================================================================
# ROOMS — Row 5 (1017-1036): Temple Row
# ===========================================================================

def_room(5,0, '@@WRoc Road Junction@@N',
    'The western terminus of the temple row meets the great @@WRoc Road@@N '
    'that stretches away to the west toward Kiess and the ruins of old Evermeet. '
    'The junction is marked by a civic waypost bearing four carved arms: '
    '@@y east to the temple@@N, @@c west to Roc Road@@N, '
    '@@R north to the civic quarter@@N, and @@p south to the service district@@N. '
    'The road surface here is better maintained than average, '
    'reflecting the military and trade importance of the western route. '
    'A guard detail monitors the junction at all hours, inspecting '
    'vehicles entering from the western approach.',
    extra=[(3,'','',0,-1,1117)])

def_room(5,1, '@@WGate of Mercy@@N',
    'The ceremonial gate that marks the formal western entrance to the temple '
    'precinct stands here, its arch of @@W white limestone@@N carved with the '
    '@@pprotection sigils@@N of the Founding Compact. '
    'The gate stands open during the day but is sealed after the evening bell '
    'by the temple gate-wardens. '
    'Supplicants and pilgrims who arrive after the closing bell are directed to '
    'the welfare shelter on the southern side of the gate.')

def_room(5,2, '@@WHigh Altar Road@@N',
    'The road that connects the outer gate to the main temple altar approach '
    'is paved in the @@W white-veined@@N stone that the temple authority '
    'maintains at considerable expense. '
    'Ritual washing basins are set at intervals along the roadside, '
    'maintained full by temple novices from before dawn. '
    'The sound of devotional chanting from the inner buildings '
    'accompanies the walk along this stretch throughout the day.')

def_room(5,3, '@@WCanons\' Walk@@N',
    'The walk used by the temple\'s senior canons for their morning '
    'and evening processions is flanked by carved stone screens '
    'filtering the @@y lantern light@@N into geometric patterns on the pavement. '
    'Canon offices occupy the buildings to either side, their '
    'double-shut doors providing sound isolation for the '
    'administrative work carried on within. '
    'A carved stone lectern in an alcove holds the temple\'s '
    'public proclamation schedule, updated each morning.')

def_room(5,4, '@@WHall of Vigils@@N',
    'The hall of vigils is a long covered corridor connecting the '
    'outer courts to the inner sanctuary, its walls lined with '
    'memorial niches holding the ashes of distinguished temple servants. '
    '@@p Violet candles@@N burn in most niches, tended by the vigil-keeping '
    'novices who rotate in two-hour shifts through the night hours. '
    'The hall\'s stone floor is polished to a mirror-quality '
    'by decades of careful maintenance.')

def_room(5,5, '@@WReliquary Square@@N',
    'The temple\'s reliquary district opens onto this square, where '
    '@@p violet-robed@@N registry clerks manage the intake and '
    'certification of claimed sacred objects brought by pilgrims. '
    'A queue of pilgrims with wrapped items waits beside the reliquary '
    'intake office, their expressions ranging from hopeful to reverent. '
    'The square\'s flagstones are inscribed with the @@c containment grid@@N '
    'required by the Violet Compact for any outdoor reliquary handling.')

def_room(5,6, '@@WTemple Approach@@N',
    'The formal approach to @@WMidgaard\'s@@N central temple narrows here '
    'to a width that focuses attention on the temple facade ahead. '
    'Stone benches on both sides allow pilgrims to compose themselves '
    'before entering the main sanctum. '
    'Temple guardians in ceremonial dress stand at regular intervals, '
    'their presence both protective and symbolic of the boundary '
    'between the outer city and the sacred interior.')

def_room(5,7, '@@WShrine of the Fallen@@N',
    'A major memorial shrine occupies this section of the temple approach, '
    'its stone walls covered in inscribed names of those who fell '
    'defending the city during the Ash-and-Iron decades. '
    '@@p Votives@@N in brass and @@y gold@@N are placed on the ledge '
    'before the central memorial stone, some of them many decades old. '
    'Temple attendants maintain the shrine with careful reverence, '
    'cleaning the stone face and replacing lamps at the prescribed hours.')

def_room(5,8, '@@WTemple Vestibule@@N',
    'The antechamber of @@WMidgaard\'s@@N central temple is a high-ceilinged '
    'space of @@W white stone@@N and @@p violet-hung@@N columns, designed to '
    'prepare the spirit for the sanctum beyond. '
    'Novice attendants help arriving pilgrims with the ritual preparation '
    'before proceeding to the main hall. '
    'The vestibule always carries a faint scent of the incense burned '
    'in the adjacent preparation rooms.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(5,9, '@@WThe Temple of @@yMidgaard@@N',
    'The great hall of @@WMidgaard\'s@@N central temple rises in columns of '
    '@@W white stone@@N to a vaulted ceiling painted with the city\'s founding '
    'myths in @@y gold@@N and @@p violet@@N. '
    'At the hall\'s center stands the altar of the First Stone Accord, '
    'its surface worn smooth by the hands of supplicants across generations. '
    '@@R Braziers@@N burn at each corner of the altar platform, their '
    'smoke drawn upward by shafts cut through the vault. '
    'The temple serves as a sanctuary, a registry of oaths, and the '
    'city\'s primary consecrated space — the place from which all '
    'civic ceremony is measured. '
    '@@c Ward-sigils@@N in the floor glow with a steady, calming light, '
    'and the silence here is profound despite the traffic of pilgrims '
    'and officials moving through the outer halls.',
    flags=ROOM_SAFE, sector=SECTOR_INSIDE,
    extra=[(4,'','',0,-1,4564)])

def_room(5,10, '@@WTemple Gardens East@@N',
    'A walled garden maintained by the temple grounds crew occupies '
    'the space between the main sanctum and the eastern approach road. '
    'Medicinal herbs and devotional flowers are cultivated in the '
    'geometric beds, tended by novices as part of their morning duties. '
    'The garden wall carries @@p carved reliefs@@N depicting the seasons '
    'of civic life, their detail exceptional even by temple standards.',
    sector=SECTOR_CITY)

def_room(5,11, '@@WVault Row@@N',
    'The temple\'s secure storage and document vault annex runs '
    'along this row behind a facade of heavy iron shuttered windows '
    'and @@R iron-reinforced doors@@N. '
    'Vault clerks move between the row\'s buildings with sealed '
    'transfer cases under escort from temple guardians. '
    'Public access to the vault annex requires appointment and '
    'the countersignature of two canon-level officials.')

def_room(5,12, '@@WPilgrim\'s Crossing South@@N',
    'The southern pilgrim route crosses the temple row at this point, '
    'connecting the outer city approach to the garden paths inside the precinct. '
    'A carved stone marker at the crossing shows the way to the main '
    'sanctum, the reliquary, and the welfare shelter with incised symbols '
    'readable even by those unfamiliar with the city\'s script. '
    'Temple charity workers distribute sealed water flasks to arriving '
    'pilgrims at the crossing during the high-traffic morning hours.')

def_room(5,13, '@@WHeralds\' Gate South@@N',
    'The southern herald gate of the temple precinct is where public '
    'proclamations from the temple authority are posted for the wider city. '
    'Announcement boards flank the gate on both sides, carrying '
    'festival schedules, welfare announcements, and civic-religious '
    'notices from the canon council. '
    'A herald in temple livery attends the gate during the main '
    'posting hours, answering questions about current announcements.')

def_room(5,14, '@@WMausoleum Walk@@N',
    'The walk past the temple\'s mausoleum district carries a solemn '
    'character that reduces traffic to respectful passage. '
    'Stone-faced mausoleum entries line the walk\'s southern side, '
    'each bearing a carved family or guild crest above the sealed door. '
    'Maintenance workers polish the entry stones on a rotation schedule, '
    'and fresh flowers are placed by relatives on the ledges before each sealed face.')

def_room(5,15, '@@WBone Road@@N',
    'The informal name given to this section of the temple road refers '
    'to the ancient burial ground that lies beneath the paving, '
    'pre-dating the city\'s current layout and recorded in the '
    'civic archive as a protected subsurface site. '
    '@@c Ward-markers@@N in the pavement maintain the containment '
    'protocols required by the Violet Compact for any built-over burial. '
    'The road itself shows no visible signs of the history below, '
    'but pilgrims familiar with the archive often pause here in acknowledgement.')

def_room(5,16, '@@WRiver Approach@@N',
    'The temple row\'s approach to the city\'s eastern water-course '
    'carries the faint smell of moving water from the channel that '
    'flows near the precinct\'s outer wall. '
    'Water-transport access to the temple\'s service yards enters '
    'through a gated channel here, used for delivering bulk provisions '
    'and removing waste without entering the public precinct. '
    'The channel gate is operated by the temple maintenance crew '
    'and monitored from the guardhouse on the road\'s southern side.')

def_room(5,17, '@@WHarbour Gate Road@@N',
    'The road connecting the temple quarter to the harbour approach '
    'begins here, its surface upgraded to handle the commercial '
    'traffic that supplies both the temple stores and the '
    'adjacent administrative buildings. '
    'Draymen with harbour loads navigate the road carefully to '
    'respect the precinct boundary, their cargoes checked at the '
    'boundary post before being admitted to the temple service yards.')

def_room(5,18, '@@WEastern Precinct South@@N',
    'The southeastern approach to the temple precinct is the '
    'least ceremonially marked of the four boundary points, '
    'reflecting its historical use as a service and logistics entrance. '
    'Utilitarian buildings here handle the practical support of '
    'the temple: stores, laundry, equipment maintenance, and '
    'the records office for the eastern ward. '
    'Novices assigned to service duties move between these buildings '
    'with the purposeful air of those on a fixed schedule.')

def_room(5,19, '@@WEastern Gate@@N',
    'The eastern boundary of @@WMidgaard@@N opens here onto the desert road '
    'that stretches away toward the great dunes and the oasis settlements beyond. '
    'The gate structure is heavier than the northern approach, '
    'reinforced during the Ash-and-Iron decades when desert incursions were a '
    'regular threat to the eastern quarter. '
    '@@R Iron-shod doors@@N stand open in their recesses during the day, '
    'their mass impressive even in repose. '
    'The eastern gate register records every traveller entering from the desert road, '
    'and gate wardens inspect all trade goods before admitting them to the city.',
    extra=[(1,'','',0,-1,1832)])

# ===========================================================================
# ROOMS — Row 6 (1037-1056): Shop Row A (alleys + named shops)
# ===========================================================================

def_room(6,0, '@@WWestern Service Passage@@N',
    'A narrow vaulted passage cut through the building block connects '
    'the temple approach to the service lane below. '
    'The walls carry utility pipe-work and lamp brackets at head height, '
    'and the floor is well-worn from decades of service traffic. '
    'The passage smells faintly of lamp oil and the damp of the stones '
    'above, which collect moisture from the city\'s drainage system.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,1, '@@WNarrow Western Alley@@N',
    'A second service alley running parallel to the western passage, '
    'slightly wider and used by deliveries that cannot fit through '
    'the tighter lane to its west. '
    'Iron hooks for hanging goods during sorting are set into the alley '
    'wall at regular intervals, and a drain channel runs the centre of the floor. '
    'The alley is lit by a single lamp bracket halfway along its length.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,2, '@@WThe Silkweave Outfitter@@N',
    'A gentled shop interior of polished wooden shelving and soft @@y lamplight@@N '
    'displays caster robes, enchanted veils, and light arcane '
    'protective wear in the lower tier ranges. '
    'The shopkeeper keeps each item in an individual display niche, '
    'labelled with its ward rating and recommended discipline. '
    'The shop smells of cedar wood used in the display cases and '
    'the faint ozonic trace of active enchantment residue.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,3, '@@WThe Resonant Vestments@@N',
    'Mid-tier caster protective wear fills the shelving of this '
    'well-appointed shop, each piece stored between boards of soft '
    'treated cloth to prevent resonance interference between enchanted items. '
    'A fitting alcove behind a curtain allows clients to assess '
    'the feel and weight of each garment before purchase. '
    'The shopkeeper monitors the room from an elevated desk, '
    'visible through an opening in the rear partition.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,4, '@@WThe Voidbound Armory@@N',
    'High-end caster protective pieces are stored behind glass panels '
    'in this prestigious shop, their enchantments still visible as '
    'faint @@c blue halos@@N around the displayed items. '
    'The shopkeeper requires prospective buyers to present a practitioner\'s '
    'attestation before examining the senior-tier inventory. '
    'The room\'s walls are lined with @@p containment inscriptions@@N '
    'that prevent unintended resonance cascade from the concentrated enchantment stock.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,5, '@@WThe Iron Shield@@N',
    'Functional melee and tank protective gear occupies this shop, '
    'its display racks built of heavy iron and rated for the weight '
    'of full plate components. '
    'The floor is reinforced beneath the heaviest display sections, '
    'and the smell of metal treatment compound and leather harness is pervasive. '
    'Samples of each item type are available for weight and fit assessment '
    'on a rack near the entrance.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,6, '@@WThe Tempered Forge@@N',
    'Mid-tier melee and tank armour in tempered steel and reinforced '
    'leather fills the tiered shelving of this well-stocked shop. '
    'Each piece is displayed with its temper-mark, a quality indicator '
    'from the independent assayer whose seal appears on the shop\'s external board. '
    'The shopkeeper can describe the production process of every item '
    'in stock, a selling point advertised on the sign above the entrance.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,7, '@@WThe Ironclad Vault@@N',
    'Heavy plate, reinforced mail, and tower-shield components of the '
    'senior tier fill this shop from floor to ceiling on iron-pinned shelving. '
    'The vault name refers to the shop\'s origin as a surplus store '
    'for the city\'s military reserve, and items here carry the '
    'quality certifications of the civic armoury inspectors. '
    'The smell of metal, oil treatment, and the faint tang of '
    'heat-treated steel is immediately noticeable on entry.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,8, '@@WThe Arcanist\'s Arsenal@@N',
    'Caster-appropriate weapons — wands, focuses, bladed ritual tools, '
    'and enchanted staves — are arranged on velvet-lined display boards '
    'that run the full length of the shop walls. '
    'Each item carries a small card describing its enchantment type '
    'and the discipline recommended for its use. '
    'The shopkeeper maintains a separate sealed case for the senior-range '
    'pieces, accessible only after consulting with the client\'s evident level of mastery.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,9, '@@WThe War Blade@@N',
    'Melee and tank weapons from shortswords to two-handed great-weapons '
    'are displayed on wall-mounted iron brackets in this spacious shop. '
    'Balance racks at the center of the shop allow customers to feel '
    'the weight distribution of each blade or bludgeon type before committing. '
    'The shopkeeper, a compact person whose forearms show old sparring scars, '
    'can give an informed opinion on the combat application of every piece in stock.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,10, '@@WThe Lamplighter\'s Supply@@N',
    'Lanterns, torches, magical light sources, and illumination devices '
    'of every practical grade line the walls of this general supply shop. '
    'Items for beginners share space with exotic light sources designed '
    'for the highest-difficulty environments, all arranged by brightness '
    'rating on the tiered shelving. '
    'The shop is perpetually one of the best-lit in the city, '
    'as it uses its own stock for interior illumination.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,11, '@@WThe Postmaster\'s Office@@N',
    'A functional and organised office where the city\'s postmaster '
    'handles dispatch, courier assignment, and message relay services. '
    'Pigeonhole racks cover the entire rear wall, sorted by route '
    'destination and delivery priority. '
    'The postmaster\'s desk is set behind a wooden counter, its surface '
    'covered in stamps, sealing wax, and open ledgers tracking '
    'current consignments through the delivery network.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,12, '@@WThe Quartermaster\'s Depot@@N',
    'The civic quartermaster\'s supply depot stocks practical mixed '
    'armour and weapon options at standard civic contract rates. '
    'Supply manifests are visible behind the counter, and the '
    'quartermaster\'s assistant moves between the stacked inventory '
    'with the efficiency of someone who has done this work for years. '
    'The depot smells of preservation oil, canvas, and the damp stone '
    'of the deep storage section visible through the rear arch.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,13, '@@WInvasion Armory@@N',
    'Maintained at the directive of the civic defence council, '
    'the invasion armory stocks high-tier armour and weapons '
    'suitable for organised city defence operations. '
    'The inventory here is drawn from military-grade requisition '
    'contracts and carries the civic defence authority\'s quality mark. '
    'The armorer in charge monitors access carefully and can '
    'produce records of every item sold and to whom.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,14, '@@WAbandoned Millinery@@N',
    'A hat and trim shop that closed some years ago still has '
    'the ghost of its last display in the dusty window niche: '
    'a hat-stand draped in cobwebs and a faded order book on the counter. '
    'The sign outside has been removed, leaving only the bracket holes '
    'and a rectangle of less-weathered paint where it hung. '
    'The room is empty and smells of old fabric and disuse.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,15, '@@WClosed Perfumery@@N',
    'The perfumery that occupied this space left its mark in the '
    'residual fragrance that clings to the walls and shelving even '
    'now, years after the last bottle was sold. '
    'Empty display racks and a stripped counter are all that remain '
    'of the original interior fittings. '
    'A notice in the boarded window reads: '
    '@@R PREMISES TO LET — apply registry office.@@N',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,16, '@@WEmpty Haberdashery@@N',
    'The haberdashery here closed with somewhat less ceremony than '
    'its neighbours: the shelves are not just empty but pulled '
    'from the walls, and the counter has been taken entirely. '
    'Someone has used the space briefly as a temporary store, '
    'evident from a few wood shavings and a discarded wrapping cord. '
    'The back room door hangs open on a broken hinge, revealing '
    'a space equally cleared of everything useful.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,17, '@@WVacant Cooperage@@N',
    'A cooperage that moved its operations to a larger site south of '
    'the city left this space vacant and took nothing it could carry. '
    'Two broken barrel-ring formers rest against one wall, '
    'too damaged to transport but evidently not worth destroying. '
    'The smell of oak sawdust and old wood still permeates the walls '
    'and floor, resistant to the years of standing empty.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,18, '@@WEastern Service Passage@@N',
    'A mirror of the western passage on the far side of the service row, '
    'this vaulted alley provides the eastern through-route between '
    'the temple row above and the service lane below. '
    'The walls show maintenance markings from the civic works crew, '
    'and the floor is in slightly better repair than the western counterpart. '
    'A single lamp bracket at the passage\'s midpoint is kept lit '
    'through all hours by the building maintenance rotation.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(6,19, '@@WNarrow Eastern Alley@@N',
    'The narrow eastern alley runs close beside the building\'s '
    'outer wall, smelling of old stone and the drainage from the '
    'upper building\'s downspout that discharges into the floor channel here. '
    'It is functional but uninviting, used primarily by delivery '
    'personnel who need the eastern through-access without the longer '
    'westward detour. '
    'Iron hooks on the wall hold utility equipment for the building\'s service staff.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

# ===========================================================================
# ROOMS — Row 7 (1057-1076): Service Street A
# ===========================================================================

def_room(7,0, '@@WService Lane West End@@N',
    'The western end of the service lane emerges from the passage above '
    'into a narrow but fully open street connecting the two shop rows. '
    'Maintenance carts and supply deliveries use this end as a staging area, '
    'and a civic works storage locker is bolted to the wall near the alley exit. '
    'The lane receives less @@y lamplight@@N than the main streets above.')

def_room(7,1, '@@WLamp-Room Alley@@N',
    'The lamp-room alley houses the maintenance store for the '
    'service lane\'s minimal lighting infrastructure. '
    'A steel cabinet secured to the wall holds spare wicks, '
    'oil cans, and the tools needed for lamp upkeep in the passage. '
    'The alley itself is dim by day and darker than most at night, '
    'relying on the lamp-room store\'s single functioning bracket.')

def_room(7,2, '@@WService Lane Step@@N',
    'A slight incline in the service lane\'s pavement is marked '
    'by a worn painted step-notice on the wall. '
    'The step accommodates the grade change between the passage-level '
    'and the flat centre section of the service lane. '
    'Delivery workers are occasionally surprised by it in low light, '
    'and the scuff marks on the wall at hand height speak to many impacts.')

def_room(7,3, '@@WDelivery Access Road@@N',
    'This section of the service lane widens slightly to allow '
    'two-way delivery traffic for the shops on the row above. '
    'Loading marks on the pavement indicate the permitted stop zones '
    'for each shop\'s delivery access point. '
    'A civic traffic directive is nailed to a post here limiting '
    'delivery vehicles to specific hours to prevent lane blockage.')

def_room(7,4, '@@WInspector\'s Walk@@N',
    'The civic health inspector uses this service lane section '
    'for their quarterly circuit of the shop row\'s rear access points. '
    'An inspection board mounted on the wall carries the last '
    'cycle\'s results with pass or remediation markers beside each unit. '
    'The lane is swept regularly in preparation for inspection visits '
    'that may arrive without advance notice.')

def_room(7,5, '@@WBackstreet West@@N',
    'A plain functional stretch of the service lane between the '
    'western delivery bays and the central crossing. '
    'The walls on both sides show the back faces of the shop buildings, '
    'their service doors all fitted with iron bolts and padlocks. '
    'A water drain runs the lane\'s centre channel, and the surface '
    'is slightly concave to direct runoff toward it.')

def_room(7,6, '@@WService Lane Centre West@@N',
    'The centre-western section of the service lane is the quietest '
    'stretch between the two main access passages. '
    'A single bench — apparently left here by a previous tenant and '
    'never removed — provides an unintentional but well-used rest point. '
    'The sounds from the shops above are muted here by the building mass '
    'between the lane and the occupied floors.')

def_room(7,7, '@@WService Lane Centre@@N',
    'The middle point of the service lane, equidistant from the '
    'western and eastern passages, has become an informal meeting place '
    'for service workers from the adjacent shops. '
    'A chalk message board nailed to one wall carries notes from '
    'workers to each other, ranging from roster changes to personal '
    'notices written in a variety of hands.')

def_room(7,8, '@@WService Lane Centre East@@N',
    'The centre-eastern section of the service lane is marked by '
    'a slight narrowing where two building extensions encroach slightly '
    'on the lane\'s permitted width. '
    'Civic notices on both walls remind the building owners of the '
    'lane clearance requirements, but no enforcement action is evident. '
    'The narrowing is still passable by most delivery vehicles.')

def_room(7,9, '@@WService Lane Crossing@@N',
    'The lane\'s central crosspoint where a perpendicular maintenance '
    'corridor intersects the main service run. '
    'Directional arrows painted on the pavement guide delivery drivers '
    'through the crossing without ambiguity. '
    'A civic maintenance valve access panel is set into the floor '
    'near the crossing centre, its iron cover worn smooth from regular use.')

def_room(7,10, '@@WBackstreet East@@N',
    'The eastern section of the backstreet begins the approach toward '
    'the eastern passages, its pavement slightly better maintained '
    'due to the heavier delivery traffic from the eastern shops. '
    'Service entries for the supply depot and the armory are set into '
    'the wall above, their iron-banded doors marked with the respective '
    'building\'s trade designation.')

def_room(7,11, '@@WMaintenance Bay@@N',
    'A maintenance bay cut into the service lane\'s eastern section '
    'provides a pull-in point for delivery vehicles offloading '
    'to the upper shop floor service entries. '
    'Painted loading-zone markings define the bay limits, '
    'and a civic parking notice specifies the maximum time a vehicle '
    'may occupy the bay before incurring a warden\'s fine.')

def_room(7,12, '@@WSouthside Passage@@N',
    'A short connecting passage between the service lane and a '
    'secondary maintenance corridor provides access to the pipe-work '
    'and drainage infrastructure under the service level. '
    'Service crews descend to the lower level through a hatch here '
    'on a scheduled maintenance cycle. '
    'The passage carries a faint chemical smell from the treatment compounds '
    'used in the drainage system below.')

def_room(7,13, '@@WService Lane East Approach@@N',
    'The approach to the eastern end of the service lane narrows '
    'between two building walls that were extended in different eras '
    'and never aligned with each other. '
    'A warning sign for the clearance height of the passage ahead '
    'is posted here for the benefit of drivers with tall loads. '
    'The lane floor here has been resurfaced more recently than '
    'the surrounding sections, its lighter stone standing out clearly.')

def_room(7,14, '@@WSouthern Service Bay@@N',
    'A loading bay matched to the southern access requirements of '
    'the abandoned shop row below provides a staging area for any '
    'future reoccupation of those premises. '
    'Currently the bay stands empty except for some stored civic '
    'equipment under a waxed canvas cover. '
    'A service gate between this bay and the southern passage '
    'is locked with a civic works padlock.')

def_room(7,15, '@@WService Lane South-East@@N',
    'The southeastern section of the service lane carries more foot '
    'traffic than the equivalent section on the western end, '
    'serving as an informal shortcut between the eastern passage and '
    'the maintenance access points on this side of the lane. '
    'Scuff marks on the building corners speak to the tight navigation '
    'required through this section with any load wider than a handcart.')

def_room(7,16, '@@WStorage Alcove@@N',
    'A bricked-up alcove in the service lane wall was once a '
    'storage access point for a building that no longer exists '
    'at the surface level. '
    'The brick facing is newer than the surrounding wall, '
    'and the shape of the original arch is still visible as a '
    'discolouration in the stonework above the infill. '
    'Someone has chalked a directional arrow on the alcove face '
    'pointing toward the eastern exit.')

def_room(7,17, '@@WService Lane Far East@@N',
    'The far-eastern section of the service lane is less frequented '
    'than the passages around the main shop access points. '
    'The pavement here is older and shows more wear than the '
    'recently maintained central section. '
    'A notice from the civic works department is nailed to the wall, '
    'advising that the section will be resurfaced in the coming quarter.')

def_room(7,18, '@@WService Lane East End@@N',
    'The eastern end of the service lane opens onto the eastern '
    'passage approach, its threshold worn smooth by the traffic '
    'of workers moving between the two levels. '
    'A posted timetable on the wall lists the delivery windows '
    'allocated to each shop in the row above, helping to '
    'prevent conflicting access at the passages.')

def_room(7,19, '@@WEast Access Point@@N',
    'The access point at the service lane\'s eastern extremity '
    'provides the entry to the eastern passages connecting the '
    'service lane to the rows above and below. '
    'A civic access log is kept in a weatherproof box here, '
    'recording after-hours entry with a date stamp maintained '
    'by the building\'s night watchman on his evening circuit.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

# ===========================================================================
# ROOMS — Row 8 (1077-1096): Shop Row B — Abandoned
# ===========================================================================

def_room(8,0, '@@WWestern Side Passage@@N',
    'The western through-passage of the lower service level is older '
    'than its counterpart above, its stonework dating to the '
    'Founding Generation construction of the original service block. '
    'The walls have been periodically whitewashed but the original stone '
    'courses are visible beneath the newest coat. '
    'Carved civic marks from successive maintenance teams are layered '
    'on the passage keystone above the south entry.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,1, '@@WNarrow Back Alley West@@N',
    'A secondary passage barely wide enough for a loaded porter '
    'runs parallel to the western side passage, its floor worn into '
    'a channel by centuries of foot traffic. '
    'The walls on both sides are slick with moisture from the '
    'water table beneath the service level. '
    'A lamp here is broken and has not been replaced, leaving '
    'this stretch considerably darker than the rest of the level.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,2, '@@WFormer Cordwainer\'s@@N',
    'A specialist shoe workshop that operated here for three '
    'generations finally closed when the family\'s last member '
    'retired to the outer districts. '
    'A sign in the language of the previous quarter\'s dominant '
    'trade community still reads above the door in faded paint. '
    'The workshop interior holds nothing now but old benches and '
    'the distinctive smell of hide and thread that decades of trade left behind.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,3, '@@WDisused Potter\'s Shop@@N',
    'The potter who worked from this unit relocated to the main '
    'pottery lane upstairs when the service row fell out of commercial favour. '
    'Two firing racks too awkward to move remain in the corner, '
    'and a crack in the wall where a kiln pipe once connected '
    'to the building\'s flue system has been roughly filled with mortar. '
    'The floor still carries dried clay in the joins of the flagstones.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,4, '@@WClosed Dye Workshop@@N',
    'A dye workshop that was cleared out rapidly — evidenced by the '
    'spilled pigment ground into the floor that no cleaning has '
    'fully removed — left this space in colourful disarray. '
    'Patches of @@R red@@N, @@c blue@@N, and @@p purple@@N stain the '
    'flagstones near where the mixing stations once stood. '
    'The residual smell of mordant compounds is still detectable on warm days.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,5, '@@WEmpty Glassblower\'s@@N',
    'The specialized furnace infrastructure for glassblowing was '
    'stripped out when this unit closed, leaving gaping holes in '
    'the wall where the flue connections were removed. '
    'The floor shows the marks of the heavy furnace base, '
    'a rectangle of lighter-coloured stone where the equipment '
    'protected the pavement from decades of thermal cycling. '
    'The building is structurally sound but functionally gutted.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,6, '@@WFormer Chandler\'s Stall@@N',
    'Wax residue on the counter surface and the permanent bloom of '
    'tallow smell in the walls are the only inheritance of the '
    'chandler who kept this stall for many years. '
    'A price board still hangs on the wall with old prices chalked '
    'on its surface, the chalk faded but legible: standard taper, '
    'work-lamp oil, refined beeswax block. '
    'The stall has been empty long enough that mice have built '
    'in the hollow beneath the counter.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,7, '@@WAbandoned Scrivener\'s@@N',
    'A scrivener\'s compact office was set up here during a period '
    'when the lower row attracted commercial traffic. '
    'The writing desk is still in place, bolted to the wall, '
    'and a few old quill stubs are jammed into the inkwell groove '
    'on its surface. '
    'The room is barely large enough for two people and the desk '
    'simultaneously, which may have contributed to its closure.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,8, '@@WFormer Apothecary@@N',
    'Traces of the apothecary operation — residue rings from '
    'glass bottle bases on the shelving, a faint herbal smell that '
    'persists in the plaster — suggest a legitimate pharmaceutical '
    'operation once worked from this unit. '
    'A government certification board still hangs on the wall, '
    'its date and certifier\'s stamp legible despite the yellowing '
    'of the document behind the cracked glass. '
    'The shelving units are still in good condition.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,9, '@@WEmpty Millinery@@N',
    'A second millinery that closed — this one with less sentiment '
    'than the one above — left its hatstand frames stacked in '
    'the corner and a bolt of dusty velvet behind the counter. '
    'The velvet is too degraded to be useful but also too bulky to '
    'move without effort, so it remains as the room\'s sole occupant. '
    'The window niche still has the display pegs for '
    'sample hats, their painted tips now chipped and dull.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,10, '@@WClosed Tailor\'s Unit@@N',
    'The tailor who occupied this unit left in a hurry based on '
    'the pins still scattered on the floor and the unfinished '
    'garment form still on the fitting stand. '
    'The form itself has no garment on it now, '
    'but the chalk fitting marks are still faintly visible on '
    'its padded surface. '
    'The space would be immediately serviceable for another tailor '
    'with minimal cleaning and some basic fixtures.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,11, '@@WDisused Rope Store@@N',
    'A rope merchant used this unit as overflow storage when '
    'the main yard above was full. '
    'Rope fragments too short to sell are still looped on '
    'the wall hooks in untidy bundles, and the hemp smell '
    'is as strong as if the stock were still fresh. '
    'The floor shows hemp-fibre staining from the bales that rested here, '
    'ground into the stone by weight and time.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,12, '@@WAbandoned Tool Store@@N',
    'A tool merchant\'s annex that became uneconomic to maintain '
    'as the district\'s commercial activity declined. '
    'Iron wall brackets designed for hanging tool stock are still in place, '
    'the tools long gone but the bracket shapes clearly showing '
    'the intended inventory: saws, adzes, planes, and hammers '
    'of workshop size. '
    'The room is draughty where a shutter hinge has failed.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,13, '@@WFormer Weighing Room@@N',
    'This unit served as a commercial weighing and assay point, '
    'its floor still marked with the positions of the scale bases '
    'in darker stone worn by the weight of the equipment. '
    'The scale mounting bolts are still in the floor, '
    'their holes roughly plugged with lead after the equipment was removed. '
    'The assayer\'s certification board on the wall is blank, '
    'the removable panel long since taken.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,14, '@@WEmpty Cold Store@@N',
    'A cold storage unit built into the building\'s foundation level '
    'was used for perishable goods before the district\'s commercial decline. '
    'The insulating packing material has been removed, but the '
    'thick stone walls still make this unit noticeably cooler '
    'than the surrounding rooms even in warm weather. '
    'A faint smell of preserved goods persists in the stone.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,15, '@@WDisused Packager\'s Unit@@N',
    'A packaging and wrapping service operated here until the '
    'volume of trade through the lower service row dropped below '
    'the threshold of profitability. '
    'Remnants of packaging material — paper sheets, cordage, '
    'and old straw — are piled in the corner where the sweepers '
    'left them after the final clearout. '
    'The unit\'s roll-shutter entrance still works but is kept '
    'rolled down and padlocked against casual entry.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,16, '@@WAbandoned Cooper\'s Annex@@N',
    'The lower-level cooper\'s annex served as a finishing and '
    'inspection room for barrel work that came up through the '
    'service passage from the main cooperage above. '
    'The ring-setting block is still fixed to the floor in the '
    'centre of the room, too embedded to remove economically. '
    'A water drain channel cut specifically for the cooperage '
    'process runs from the block to the wall drain.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,17, '@@WFormer Finishing Room@@N',
    'A general finishing and quality-check room used by several '
    'of the upper-level shops for final inspection before sale. '
    'The bright ceiling lanterns that made this room functional '
    'for close work are gone, but the brass bracket mounts remain, '
    'still reflecting the reduced illumination from the single '
    'remaining floor lamp. '
    'The room is in better structural condition than most of '
    'the adjacent abandoned units.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,18, '@@WEastern Side Passage Lower@@N',
    'The lower eastern passage mirrors the upper level\'s eastern alley, '
    'its construction matching the Founding Generation stonework '
    'throughout the southern service block. '
    'The passage floor is slightly lower than the lane it connects '
    'to, creating a slight ramp at both ends that is worn slick '
    'by foot traffic despite repeated anti-slip treatments.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(8,19, '@@WNarrow Back Alley East@@N',
    'The narrow eastern back alley at the lower level is darker '
    'and less maintained than the upper counterpart. '
    'Moisture stains the walls and the floor drain is partially '
    'blocked, creating a shallow puddle that never fully clears '
    'except in the driest summer months. '
    'The only active lamp is a small oil light maintained by '
    'the passage\'s own service contract.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

# ===========================================================================
# ROOMS — Row 9 (1097-1116): Southern Gate Road
# ===========================================================================

def_room(9,0, '@@WSouthern Gate Road West@@N',
    'The western end of the southern gate road begins its run toward '
    'the city\'s southern exit, its pavement wider than the service '
    'streets above to accommodate the volume of departing trade traffic. '
    'Guard posts at the road\'s entry point check outbound heavy loads '
    'against the gate manifest before allowing them to proceed. '
    'The morning queue of southbound carts is a fixture of this stretch '
    'on market days.')

def_room(9,1, '@@WSouthgate Approach West@@N',
    'The western approach to the southern gate district carries '
    'the smell of livestock from the animal-transport yards that '
    'border the road on the northern side. '
    'Drovers with southbound herds form a regular part of the '
    'morning traffic, and the pavement shows the distinctive '
    'wear of hoof traffic mixed with wheel ruts. '
    'A warden\'s post at the kerb maintains order at the livestock '
    'crossing point during the peak morning movement.')

def_room(9,2, '@@WCarter\'s Way@@N',
    'The carters\' professional community maintains its southern '
    'district offices along this section of the gate road. '
    'Route boards and freight rate lists are posted in every '
    'window, advertising runs to the southern settlements, '
    'the desert oasis routes, and the forest roads. '
    'Dispute-resolution services and cargo insurance brokers '
    'occupy the buildings between the major carters\' offices.')

def_room(9,3, '@@WSouthern Transit Street@@N',
    'A busy transit street connecting the residential southern '
    'district to the main gate road, this stretch handles a '
    'mix of pedestrian commuters and light cart traffic. '
    'Coffee-house windows glow @@y warm@@N at the morning hours '
    'when departing travellers take a last hot drink before '
    'heading for the gate. '
    'A public clock on the corner building keeps gate-time '
    'for travellers coordinating departure schedules.')

def_room(9,4, '@@WGate Market South@@N',
    'A small permanent market at the gate road\'s approach caters '
    'to departing travellers with last-minute provisions, maps, '
    'and travel equipment. '
    'The stalls are compact and efficient, their operators well '
    'practised at serving customers who have one eye on the gate '
    'and little time to negotiate. '
    'Road guides, water skins, and preserved food dominate the '
    'available goods.')

def_room(9,5, '@@WSouthern Commons Road@@N',
    'The road through the southern commons connects the inner city '
    'to the gate district, carrying the mundane flow of residential '
    'life: deliveries, children, workers, and errand-runners. '
    'The buildings here are older than the civic quarter standards '
    'and more varied in material and construction. '
    'A public water post at the road\'s midpoint is one of several '
    'maintained throughout the district by the civic welfare fund.')

def_room(9,6, '@@WGate-Road Crossing@@N',
    'The principal crossing of the southern gate road sees traffic '
    'from the residential district, the gate approach, and the '
    'service corridors all converging at once during the peak hours. '
    'Civic traffic posts at each corner help direct the flow, '
    'and a warden on foot duty is assigned to the crossing '
    'during the morning and evening traffic peaks. '
    'The crossing pavement is more heavily maintained than '
    'surrounding stretches due to the wear volume.')

def_room(9,7, '@@WProvisions Lane@@N',
    'Provisions merchants catering to the road traffic occupy '
    'this lane between the southern commons and the gate approach. '
    'Dried goods, preserved meat, and baked travel bread are '
    'displayed from baskets and table-fronts at every shop entrance. '
    'The smell of fresh bread from the bakers who hold the '
    'early trade is one of the more pleasant features of '
    'this otherwise purely functional stretch.')

def_room(9,8, '@@WSouthern District Lane@@N',
    'The southern district lane runs parallel to the gate road, '
    'used by residents who prefer to avoid the main traffic flow. '
    'Smaller and quieter than the gate road, this lane sees '
    'foot traffic primarily from the adjacent tenements whose '
    'back entrances face onto it. '
    'Laundry lines between buildings and potted plants on windowsills '
    'give the lane a domestic character absent from the route to the gate.')

def_room(9,9, '@@WSouth Gate@@N',
    'The southern gate of @@WMidgaard@@N stands at the city\'s lower boundary, '
    'its stone gatehouse rising above the road with the same solid authority '
    'as the northern approach but with a more weathered character. '
    '@@R Iron-bound gates@@N stand open in their recessed channel grooves, '
    'their surfaces showing the patina of centuries of weather and use. '
    'The gate road beyond stretches south through the woodlands '
    'whose canopy is visible in the distance from the arch. '
    'A duty register at the gate post records departures and arrivals, '
    'and a guard with a manifest checks southbound loads against declared cargo.',
    extra=[(2,'','',0,-1,1434)])

def_room(9,10, '@@WSouthgate Road East@@N',
    'The eastern stretch of the gate approach road carries '
    'lighter traffic than the main gate, primarily foot travellers '
    'and the occasional light cart not requiring the wider main lane. '
    'The buildings here are the southern district\'s commercial fringe: '
    'small factor\'s offices and travel services competing with the '
    'larger establishments on the road\'s western side. '
    'A guild courier relay point at the building corner operates '
    'from dawn to dusk with rotating rider coverage.')

def_room(9,11, '@@WEastern Gate Approach@@N',
    'The eastern gate approach connects the southern gate district '
    'to the eastern road network, an important junction for trade '
    'traffic that has come south from the desert route before '
    'turning south again toward the woodland settlements. '
    'The road surface here carries heavy wear from the turning '
    'movements of large wagons navigating the junction. '
    'A road mileage post on the corner gives distances to the '
    'major settlements on both the eastern and southern routes.')

def_room(9,12, '@@WGate District Row@@N',
    'The main commercial row of the gate district houses the '
    'services that travellers need immediately on arrival or '
    'just before departure: money-changing, luggage storage, '
    'message holding, and basic provisioning. '
    'The buildings are functional rather than impressive, '
    'their owners focused on high-volume service rather than '
    'prestige trade. '
    'Signs here are in multiple scripts to serve the diverse '
    'traffic from the eastern and southern routes.')

def_room(9,13, '@@WSouthern Exchange Lane@@N',
    'The exchange lane in the southern gate district handles '
    'currency conversion for travellers from beyond the city\'s '
    'standard coin zone. '
    'Money-changers\' boards outside each exchange office '
    'show current conversion rates for the major trade currencies. '
    'The lane is busy through most of the day, with a brief '
    'lull at midday when the exchangers take their break rotation.')

def_room(9,14, '@@WOutbound Road@@N',
    'The final stretch of road approaching the gate proper is '
    'kept clear of stalls and obstacles by civic ordinance, '
    'ensuring that outbound traffic can flow without obstruction '
    'toward the gate inspection point. '
    'A civic barrier post at the road\'s approach marks the '
    'beginning of the restricted zone where no commercial '
    'soliciting is permitted. '
    'Gate wardens monitor compliance from the nearby post.')

def_room(9,15, '@@WGate Holding Area@@N',
    'Inbound travellers cleared at the southern gate are directed '
    'to this holding area while their manifests are processed '
    'and their goods assessed for the appropriate entry levy. '
    'Benches line the perimeter wall, and a water post provides '
    'refreshment for those waiting through the processing queue. '
    'The holding area is covered by a timber roof on iron posts, '
    'providing shelter from poor weather during the inspection process.')

def_room(9,16, '@@WSouthern Gate East Road@@N',
    'The eastern section of the southern gate area is the '
    'less busy of the two sides, its traffic primarily residential '
    'commuters from the eastern tenements and light goods delivery. '
    'A patrol route marking on the wall indicates the watch\'s '
    'nighttime circuit through this section of the gate district. '
    'Small residential services — a laundry, a cobbler, '
    'a bread seller — occupy the ground floors of the terrace.')

def_room(9,17, '@@WGate Road Far East@@N',
    'The far eastern section of the southern gate road loses the '
    'commercial character of the main gate approach and becomes '
    'primarily residential, with tenement buildings replacing '
    'the factor\'s offices and travel services. '
    'The sound of the gate area fades here, replaced by the '
    'domestic sounds of the neighbourhood: conversation through '
    'open windows, children, cooking smells from ground-floor kitchens.')

def_room(9,18, '@@WEastern Gate Passage@@N',
    'An enclosed passage at the eastern end of the southern gate '
    'road provides covered access between the gate level and the '
    'passages above it in the service level. '
    'The passage is maintained as part of the civic infrastructure '
    'contract that covers the entire southern service block. '
    'Iron brackets hold reserve lamp equipment for the southern '
    'district patrol route.',
    flags=ROOM_INDOORS, sector=SECTOR_INSIDE)

def_room(9,19, '@@WSouthern Gate Road End@@N',
    'The easternmost end of the southern gate road terminates at '
    'the boundary of the outer district, where the civic road '
    'maintenance responsibility ends and the private property '
    'boundary begins. '
    'A civic marker post establishes the exact boundary, '
    'its position unchanged since the Standardisation Century '
    'survey that formalised the district limits. '
    'The road surface beyond the post is noticeably less maintained.')

# ===========================================================================
# MOBILES — Batch 1 (917-929): Guards, Gertrude, Healer, Priest, Flavour NPCs
# ===========================================================================

# act flags
ACT_GUARD      = 65      # is_npc(1) + stay_area(64)
ACT_SENTINEL   = 67      # is_npc(1) + sentinel(2) + stay_area(64)
ACT_HEALER     = 4163    # is_npc(1) + sentinel(2) + stay_area(64) + heal(4096)
ACT_POSTMASTER = 524363  # is_npc(1) + sentinel(2) + stay_area(64) + remember(8) + postman(524288)
ACT_GERTRUDE   = 131167  # original value preserved

# affected_by flags
AFF_NONE       = 0
AFF_DETECTS    = 8320    # detect_invis + protect (used by healer in original)

def_mob(917,
    keywords='city guard midgaard',
    short='a city guard',
    long_desc='A city guard stands watch here, one hand resting on the pommel of a short sword.',
    description='One of the many posted wardens maintaining order in the streets of @@WMidgaard@@N. The grey tabard and iron-capped boots are standard civic watch issue, and the guard\'s alert posture suggests someone who takes the duty seriously.',
    act=ACT_GUARD, aff=AFF_NONE, align=0, level=20, sex=1,
    hp_mod=0, ac_mod=0, hr_mod=2, dr_mod=2)

def_mob(918,
    keywords='gate warden northern gate',
    short='the gate warden',
    long_desc='The gate warden stands beside the northern gatehouse, watching the road beyond.',
    description='The gate warden\'s station at the northern gatehouse is a post of some trust, requiring the judgement to distinguish travellers from threats. This one has clearly held the position for some time, their manner unhurried but their attention absolute.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=500, level=25, sex=1,
    hp_mod=0, ac_mod=-20, hr_mod=3, dr_mod=3)

def_mob(919,
    keywords='temple guardian guard',
    short='a temple guardian',
    long_desc='A temple guardian stands in ceremonial armour, blocking the path to the inner sanctum.',
    description='The temple guardians combine ceremonial function with genuine fighting capability, their training a blend of martial discipline and @@pdevotional@@N focus. The polished armour and @@c ward-inscribed@@N shield mark this one as a senior-rank guardian.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=750, level=30, sex=0,
    hp_mod=0, ac_mod=-30, hr_mod=3, dr_mod=3)

def_mob(920,
    keywords='street merchant vendor',
    short='a street merchant',
    long_desc='A street merchant hawks wares from a canvas-covered cart parked at the kerb.',
    description='Not affiliated with any of the established guild shops, this merchant operates on a roving permit that allows vending from any public street in the district. The selection of goods changes daily based on what sources were willing to sell that morning.',
    act=ACT_GUARD, aff=AFF_NONE, align=0, level=10, sex=1)

def_mob(921,
    keywords='caravan master captain',
    short='the caravan master',
    long_desc='The caravan master stands at the center of the square, directing the wagon arrangements.',
    description='Caravan masters earn their title through years of successfully navigating the continent\'s most dangerous trade routes. This one\'s weathered appearance and the quality of their equipment suggest experience hard-won on the desert and forest roads.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=0, level=20, sex=1,
    hp_mod=0, ac_mod=0, hr_mod=3, dr_mod=3)

def_mob(922,
    keywords='caravan guard escort',
    short='a caravan guard',
    long_desc='A caravan guard leans against a wagon wheel, scanning the square with practised vigilance.',
    description='Hired muscle for the caravan circuit, this guard\'s relaxed posture is deceptive — the hand near the weapon and the positioned feet indicate someone ready to respond immediately. The route brands on their pack identify this as an eastern desert regular.',
    act=ACT_GUARD, aff=AFF_NONE, align=0, level=15, sex=1,
    hp_mod=0, ac_mod=0, hr_mod=2, dr_mod=2)

def_mob(923,
    keywords='street vendor seller',
    short='a street vendor',
    long_desc='A street vendor calls out prices from behind a small folding table covered in goods.',
    description='This vendor\'s permit is visible on a card hung from the table edge, confirming their licensed status in the district. The goods change with the season and the supply, and the vendor\'s quick assessment of a customer\'s likely budget precedes every interaction.',
    act=ACT_GUARD, aff=AFF_NONE, align=0, level=8, sex=0)

def_mob(924,
    keywords='pilgrim traveller',
    short='a weary pilgrim',
    long_desc='A weary pilgrim rests near the shrine, eyes closed in silent devotion.',
    description='Pilgrims arriving in @@WMidgaard@@N represent a spectrum of faiths and origins, their common ground the acknowledgement that the city\'s temple holds something worth the journey. This one shows the signs of a long road: worn boots, road dust, and the particular stillness of someone who has been walking for weeks.',
    act=ACT_GUARD, aff=AFF_NONE, align=700, level=5, sex=0)

def_mob(925,
    keywords='beggar street',
    short='a city beggar',
    long_desc='A city beggar extends a bowl toward passing strangers with a practised gesture.',
    description='One of @@WMidgaard\'s@@N many displaced citizens, this beggar occupies a regular pitch and is known to the ward welfare office by name. The civic welfare programme provides a daily ration, but the gap between that and sufficiency drives the street collection.',
    act=ACT_GUARD, aff=AFF_NONE, align=-200, level=3, sex=0)

def_mob(926,
    keywords='noble citizen resident',
    short='a noble citizen',
    long_desc='A noble citizen walks the boulevard with the unhurried confidence of one who belongs here.',
    description='The upper districts of @@WMidgaard@@N sustain a class of citizens whose wealth largely insulates them from the city\'s practical difficulties. This one\'s clothing quality and the quality of their escort — clearly hired, not obligatory — mark them as comfortably established in the northern quarter.',
    act=ACT_GUARD, aff=AFF_NONE, align=200, level=15, sex=1)

def_mob(927,
    keywords='gertrude executioner',
    short='Gertrude',
    long_desc='Gertrude the Executioner stands here, methodically polishing the edge of her blade.',
    description='A masked woman whose muscular frame speaks to years of disciplined training with heavy weapons. The look in her eyes suggests she takes genuine satisfaction in her official role, and the quality of her equipment marks her as one of @@WMidgaard\'s@@N most feared civic servants.',
    act=ACT_GERTRUDE, aff=1633020, align=1000, level=150, sex=2,
    hp_mod=0, ac_mod=0, hr_mod=0, dr_mod=0,
    ext_skills=0, ext_cast=0, ext_def=32,
    pipe_wm=7637056, pipe_s=48)

def_mob(928,
    keywords='healer temple',
    short='the healer',
    long_desc='A healer sits here, surrounded by the quiet hum of restorative magic.',
    description='Although long retired from adventuring, this healer continues to serve @@WMidgaard\'s@@N citizens and travelers with practiced skill. The lines on her face map decades of service, and her hands still carry the faint glow of channeled restoration.',
    act=ACT_HEALER, aff=AFF_DETECTS, align=1000, level=114, sex=1)

def_mob(929,
    keywords='temple priest canon',
    short='the temple priest',
    long_desc='The temple priest moves through the vestibule with quiet purpose, performing the morning rites.',
    description='The canon-level priest who leads the daily observances in the outer hall carries the calm authority of long practice. Ritual familiarity has made the movement sequences automatic, freeing attention for the spiritual focus the canons\' rule demands.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=1000, level=30, sex=0)


# ===========================================================================
# MOBILES — Batch 2 (930-941): Shopkeepers
# ===========================================================================

def_mob(930,
    keywords='tailor seamstress caster apprentice',
    short='Miriel the Seamstress',
    long_desc='Miriel the Seamstress works behind a counter draped in bolts of enchanted cloth.',
    description='Miriel has spent twenty years learning which enchantment threads complement robes and which clash with them. Her shop serves apprentice casters and junior mages who need gear appropriate to their developing skills, and she is patient with customers who do not yet know what they want.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=300, level=25, sex=2)

def_mob(931,
    keywords='magister clothier caster mid',
    short='Aldric the Magister',
    long_desc='Aldric the Magister examines a robe under a magnifying glass, checking the ward-stitching.',
    description='A former student of the Reliquary Academy who transitioned from practice to supply, Aldric understands the mid-tier caster\'s needs with professional precision. His stock runs from comfortable journeyman gear to the first pieces of genuinely high-end robing.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=400, level=50, sex=1)

def_mob(932,
    keywords='archwarden arcanist caster high',
    short='Seraphel the Archwarden',
    long_desc='Seraphel the Archwarden stands motionless amid racks of shimmering arcane garments.',
    description='Seraphel is rumoured to have assisted in the creation of several of the garments she now sells, having stepped back from field work once the commissions became more profitable than the adventuring. Her stock is the finest caster armour available in @@WMidgaard@@N, priced accordingly.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=500, level=90, sex=2)

def_mob(933,
    keywords='armourer smith melee low',
    short='Brekka the Armourer',
    long_desc='Brekka the Armourer hammers a dent from a shield with methodical strokes.',
    description='A journeyman-grade smith who supplemented her forge training with a stint as a caravan guard, Brekka understands both the making and the using end of mid-weight armour. Her shop caters to warriors who have outgrown the training yard but not yet reached the advanced circuits.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=100, level=25, sex=2)

def_mob(934,
    keywords='mastersmith armourer melee mid',
    short='Vorn the Mastersmith',
    long_desc='Vorn the Mastersmith rests one massive hand on the counter, waiting for a customer who knows what they want.',
    description='Vorn has little patience for indecision but infinite patience for a customer who comes in with specific requirements. His mid-tier melee stock is the result of long supply relationships with several highland forge-teams and is reliably current to the fighting styles of the past decade.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=200, level=50, sex=1)

def_mob(935,
    keywords='warlord smith armourer melee high',
    short='Draga Ironclad',
    long_desc='Draga Ironclad polishes a section of plate armour with slow, meditative movements.',
    description='A retired campaign veteran who spent her second career supplying the very forces she once fought alongside, Draga\'s familiarity with high-end combat equipment is entirely practical. The pieces she stocks have seen real combat testing — she will not sell anything she would not have worn herself.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=300, level=90, sex=2)

def_mob(936,
    keywords='enchanter weaponsmith caster weapons',
    short='Thalindra the Enchanter',
    long_desc='Thalindra the Enchanter runs a slender finger along a staff\'s grain, testing the resonance.',
    description='Thalindra sources her weapons from craftspeople who understand that a caster\'s weapon is a focus as much as a weapon. The staves, wands, and focus-blades she carries are rated both for physical function and for their arcane transmission efficiency, a specialisation few weapon dealers bother with.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=300, level=50, sex=2)

def_mob(937,
    keywords='weaponmaster smith melee weapons',
    short='Gorven Bladewright',
    long_desc='Gorven Bladewright stands with arms crossed, surveying the weapons mounted on the walls.',
    description='Gorven has supplied weapons to everyone from first-mission fighters to veteran champions over the course of a thirty-year dealing career. He can identify the right weapon for a fighter\'s style within a few questions and stocks a range from first-purchase starter blades to the upper tier of non-custom work.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=100, level=50, sex=1)

def_mob(938,
    keywords='storekeeper trader general store',
    short='Oswin the Storekeeper',
    long_desc='Oswin the Storekeeper leans on the counter with the comfortable air of someone who has seen everything twice.',
    description='Oswin runs @@WMidgaard\'s@@N most comprehensively stocked general store with the calm efficiency of someone who has been doing it for twenty years. He stocks illumination equipment for every archetype at every level, and his pricing is fair enough that most adventurers who stop in once become regulars.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=200, level=30, sex=1)

def_mob(939,
    keywords='postmaster herald courier',
    short='the postmaster',
    long_desc='The postmaster sorts a bundle of sealed letters with efficient, practiced hands.',
    description='The civic postal authority employs postmasters at key points throughout @@WMidgaard@@N to handle the city\'s substantial correspondence traffic. This one\'s station serves the central district and handles priority deliveries for guild and civic senders, as well as the usual adventurer and commercial mail.',
    act=ACT_POSTMASTER, aff=AFF_NONE, align=200, level=30, sex=1)

def_mob(940,
    keywords='quartermaster supply logistics',
    short='the quartermaster',
    long_desc='The quartermaster works through an inventory list with a stylus and wax tablet.',
    description='Responsible for maintaining the strategic supply cache of @@WMidgaard\'s@@N central district, the quartermaster stocks the goods that sustaining a city\'s defence and civic operation requires. The catalogue is deliberately broad — useful items for every fighting style, priced for practical supply rather than retail profit.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=300, level=40, sex=1)

def_mob(941,
    keywords='invasion armory keeper warden',
    short='the invasion warden',
    long_desc='The invasion warden stands behind a heavy counter of iron-banded oak, watching the door.',
    description='The invasion warden maintains the emergency armory that becomes critically important whenever @@WMidgaard@@N faces incursion. The stock is functional and battle-tested, and the warden\'s manner conveys the expectation that anyone shopping here is planning to use what they buy.',
    act=ACT_SENTINEL, aff=AFF_NONE, align=500, level=60, sex=1)

# ===========================================================================
# OBJECTS — Batch 1 (917-931): Caster Armor Low (levels 10-60)
# Shop room: 1039 (row 6 col 2), keeper: mob 930
# Wear flags: body, head, legs, feet, hands, wrist, arms, about, neck, shoulders
# Weight 1-5 = caster archetype
# ===========================================================================

# item_apply: 0=none used here (apply handled by wear + stat fields in value[])
# values for IT_ARMOR (9): [ac, 0, 0, 0, 0, 0, 0, 0, 0, 0]

def_obj(917,
    keywords='apprentice robe grey', short='an apprentice\'s grey robe',
    description='A plain grey robe stitched with basic ward-thread at the cuffs and hem.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[2,0,0,0,0,0,0,0,0,0], weight=3, level=10)

def_obj(918,
    keywords='cloth cowl soft grey', short='a soft grey cloth cowl',
    description='A lightweight cowl that offers minimal protection but channels arcane focus well.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[1,0,0,0,0,0,0,0,0,0], weight=2, level=10)

def_obj(919,
    keywords='linen leggings pale blue', short='pale blue linen leggings',
    description='Light linen leggings reinforced at the knee with woven sigil-cloth.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[2,0,0,0,0,0,0,0,0,0], weight=2, level=15)

def_obj(920,
    keywords='soft slippers silk enchanted', short='a pair of enchanted silk slippers',
    description='Silk slippers with a thin ward sole that dampens arcane feedback.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[1,0,0,0,0,0,0,0,0,0], weight=1, level=15)

def_obj(921,
    keywords='scholar robe velvet deep blue', short='a deep blue velvet scholar\'s robe',
    description='Velvet robe with layered protective stitching in the deep-blue style of the Academy.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[4,0,0,0,0,0,0,0,0,0], weight=3, level=20)

def_obj(922,
    keywords='arcane hood silk silver thread', short='a silver-threaded arcane hood',
    description='A hood woven with fine silver thread that provides mild ward against mental intrusion.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[3,0,0,0,0,0,0,0,0,0], weight=2, level=20)

def_obj(923,
    keywords='ward cloak light grey', short='a light grey ward cloak',
    description='A travel cloak with ward-sigils sewn into the lining at collar and hem.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ABOUT),
    item_apply=0, values=[3,0,0,0,0,0,0,0,0,0], weight=3, level=25)

def_obj(924,
    keywords='runed gloves silk pale', short='pale silk runed gloves',
    description='Light gloves with runes at the palm and fingertips to improve spell-shaping.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HANDS),
    item_apply=0, values=[2,0,0,0,0,0,0,0,0,0], weight=1, level=25)

def_obj(925,
    keywords='acolyte robe purple', short='an acolyte\'s purple robe',
    description='Standard acolyte robing in the purple of the Reliquary order, magically reinforced.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[5,0,0,0,0,0,0,0,0,0], weight=3, level=30)

def_obj(926,
    keywords='channeller wristband woven sigil', short='a woven sigil wristband',
    description='A thick wristband of woven sigil-cloth that steadies channelling at moderate output.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(WRIST),
    item_apply=0, values=[2,0,0,0,0,0,0,0,0,0], weight=1, level=30)

def_obj(927,
    keywords='journeyman robe teal', short='a teal journeyman mage\'s robe',
    description='A teal robe marking journeyman mage status, with strengthened ward-stitching throughout.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[7,0,0,0,0,0,0,0,0,0], weight=3, level=40)

def_obj(928,
    keywords='arcane leggings deep green', short='deep green arcane leggings',
    description='Reinforced leggings in deep green with arcane channelling cord woven through the seams.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[5,0,0,0,0,0,0,0,0,0], weight=2, level=40)

def_obj(929,
    keywords='sage cloak dark blue trimmed', short='a dark blue sage\'s cloak',
    description='A heavy cloak in the dark blue of the Sage order, trimmed with ward-embroidery.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ABOUT),
    item_apply=0, values=[6,0,0,0,0,0,0,0,0,0], weight=4, level=50)

def_obj(930,
    keywords='elder robe crimson gold trim', short='a crimson elder mage\'s robe',
    description='A crimson robe with gold ward-trim, reaching the lower tier of elder mage appointments.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[9,0,0,0,0,0,0,0,0,0], weight=3, level=55)

def_obj(931,
    keywords='elder hood crimson gold', short='a crimson gold-traced elder hood',
    description='A crimson hood traced with gold stitching that marks elder-tier arcane protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[7,0,0,0,0,0,0,0,0,0], weight=2, level=60)

# ===========================================================================
# OBJECTS — Batch 2 (932-946): Caster Armor Mid (levels 70-105)
# Shop room: 1040 (row 6 col 3), keeper: mob 931
# ===========================================================================

def_obj(932,
    keywords='adept robe midnight blue', short='a midnight blue adept\'s robe',
    description='A midnight blue robe with strengthened channels for adept-level arcane throughput.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[11,0,0,0,0,0,0,0,0,0], weight=3, level=70)

def_obj(933,
    keywords='adept hood midnight blue silver', short='a midnight blue silver-trimmed adept hood',
    description='A hood matching the adept robe, silver trim marking mid-tier arcane rank.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[9,0,0,0,0,0,0,0,0,0], weight=2, level=70)

def_obj(934,
    keywords='warded cloak deep violet', short='a deep violet warded cloak',
    description='A violet cloak with multi-layer ward panels sewn into the body.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ABOUT),
    item_apply=0, values=[9,0,0,0,0,0,0,0,0,0], weight=4, level=75)

def_obj(935,
    keywords='adept leggings violet deep', short='deep violet adept leggings',
    description='Adept leggings with ward-cord reinforcement along the channel lines.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[9,0,0,0,0,0,0,0,0,0], weight=2, level=75)

def_obj(936,
    keywords='wrist bracer arcane focus copper', short='copper arcane-focus bracers',
    description='A pair of copper-inlaid bracers that improve arcane focus at adept throughput.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(WRIST),
    item_apply=0, values=[7,0,0,0,0,0,0,0,0,0], weight=2, level=80)

def_obj(937,
    keywords='arcane slippers silk copper rune', short='copper-rune silk arcane slippers',
    description='Silk slippers with copper-rune soles that reduce arcane feedback at the extremities.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[7,0,0,0,0,0,0,0,0,0], weight=2, level=80)

def_obj(938,
    keywords='high adept robe white gold', short='a white and gold high-adept robe',
    description='A white robe with gold ward-stitching marking the upper adept tier.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[13,0,0,0,0,0,0,0,0,0], weight=3, level=85)

def_obj(939,
    keywords='arcane pauldrons cloth silver', short='silver cloth arcane pauldrons',
    description='Light pauldrons of silver-woven cloth that protect shoulder channels during casting.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(SHOULDERS),
    item_apply=0, values=[8,0,0,0,0,0,0,0,0,0], weight=3, level=85)

def_obj(940,
    keywords='channeller gloves silver thread', short='silver-thread channeller\'s gloves',
    description='Gloves with fine silver thread woven through the palm to stabilise channelling at high output.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HANDS),
    item_apply=0, values=[8,0,0,0,0,0,0,0,0,0], weight=1, level=90)

def_obj(941,
    keywords='arcane coif mithril weave', short='a mithril-weave arcane coif',
    description='A coif of mithril-thread weave that shields the caster\'s mind at advanced throughput.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[11,0,0,0,0,0,0,0,0,0], weight=2, level=90)

def_obj(942,
    keywords='arcane robe deep scarlet', short='a deep scarlet arcane robe',
    description='A scarlet robe of the kind issued to senior members of the Reliquary\'s field division.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[15,0,0,0,0,0,0,0,0,0], weight=3, level=95)

def_obj(943,
    keywords='arcane leggings deep scarlet', short='deep scarlet arcane leggings',
    description='Leggings matching the senior Reliquary robe, with reinforced ward-cord throughout.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[12,0,0,0,0,0,0,0,0,0], weight=2, level=95)

def_obj(944,
    keywords='greater ward cloak black silver', short='a black silver-edged greater ward cloak',
    description='A black cloak edged in silver with multi-layer warding that approaches high-tier standard.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ABOUT),
    item_apply=0, values=[12,0,0,0,0,0,0,0,0,0], weight=4, level=100)

def_obj(945,
    keywords='arcane boots black mithril', short='black mithril-soled arcane boots',
    description='Black boots with mithril soles that eliminate ground-feedback from high-throughput casting.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[10,0,0,0,0,0,0,0,0,0], weight=2, level=100)

def_obj(946,
    keywords='senior robe black gold deep', short='a deep black and gold senior arcane robe',
    description='A black robe with heavy gold ward-stitching approaching the lower threshold of high-tier caster gear.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[17,0,0,0,0,0,0,0,0,0], weight=3, level=105)

# ===========================================================================
# OBJECTS — Batch 3 (947-961): Caster Armor High (levels 110-135)
# Shop room: 1041 (row 6 col 4), keeper: mob 932
# ===========================================================================

def_obj(947,
    keywords='archmage robe white silver', short='a white silver-blazoned archmage robe',
    description='A robe of the highest caster grade, white with silver blazons marking archmage standing.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[19,0,0,0,0,0,0,0,0,0], weight=3, level=110)

def_obj(948,
    keywords='archmage hood white silver', short='a white silver-traced archmage hood',
    description='Hood of the archmage grade, silver tracing marking the highest ward rating.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[15,0,0,0,0,0,0,0,0,0], weight=2, level=110)

def_obj(949,
    keywords='grand ward cloak white luminous', short='a luminous white grand ward cloak',
    description='A ward cloak that emits a faint pale luminescence from the density of its protective sigils.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ABOUT),
    item_apply=0, values=[16,0,0,0,0,0,0,0,0,0], weight=4, level=115)

def_obj(950,
    keywords='archmage leggings white silver', short='white silver-traced archmage leggings',
    description='High-ward leggings matching the archmage robe grade, with silver channel-tracing.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[16,0,0,0,0,0,0,0,0,0], weight=2, level=115)

def_obj(951,
    keywords='arcane vambrace platinum sigil', short='platinum sigil-carved arcane vambraces',
    description='Platinum-engraved vambraces carved with the strongest channelling sigils in current use.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ARMS),
    item_apply=0, values=[14,0,0,0,0,0,0,0,0,0], weight=3, level=120)

def_obj(952,
    keywords='arcane slippers platinum crystal', short='crystal-soled platinum arcane slippers',
    description='Slippers with crystal soles that eliminate all ground feedback during high-power casting.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[14,0,0,0,0,0,0,0,0,0], weight=2, level=120)

def_obj(953,
    keywords='grand arcane robe azure platinum', short='an azure platinum-edged grand arcane robe',
    description='An azure robe edged in platinum with the highest ward-stitching density commercially available.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[21,0,0,0,0,0,0,0,0,0], weight=3, level=125)

def_obj(954,
    keywords='grand arcane hood azure platinum', short='an azure platinum-traced grand arcane hood',
    description='A hood matching the grand arcane robe, with platinum trace and peak ward rating.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[17,0,0,0,0,0,0,0,0,0], weight=2, level=125)

def_obj(955,
    keywords='supreme ward gloves platinum', short='platinum supreme ward gloves',
    description='Gloves at the peak of commercially produced caster hand-protection, platinum-woven throughout.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HANDS),
    item_apply=0, values=[15,0,0,0,0,0,0,0,0,0], weight=2, level=125)

def_obj(956,
    keywords='grand pauldrons platinum cloth', short='platinum grand arcane pauldrons',
    description='Platinum-reinforced pauldrons at the high-tier grade for shoulder ward rating.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(SHOULDERS),
    item_apply=0, values=[15,0,0,0,0,0,0,0,0,0], weight=3, level=130)

def_obj(957,
    keywords='wristguard mithril platinum rune', short='mithril-platinum rune wristguards',
    description='Mithril-platinum wristguards carved with overlapping rune sets for extreme channelling stability.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(WRIST),
    item_apply=0, values=[14,0,0,0,0,0,0,0,0,0], weight=2, level=130)

def_obj(958,
    keywords='grandmaster robe ultimate white gold', short='an ultimate white and gold grandmaster robe',
    description='The finest robe Seraphel stocks: white and gold ward-weave at the peak of the high tier.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[23,0,0,0,0,0,0,0,0,0], weight=3, level=130)

def_obj(959,
    keywords='grandmaster cloak white gold', short='a white gold-blazed grandmaster cloak',
    description='A grandmaster cloak blazed in gold at the ultimate tier of commercial ward protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ABOUT),
    item_apply=0, values=[20,0,0,0,0,0,0,0,0,0], weight=4, level=135)

def_obj(960,
    keywords='grandmaster leggings white gold', short='white gold-traced grandmaster leggings',
    description='Grandmaster leggings with the highest commercially available ward-cord tracing.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[20,0,0,0,0,0,0,0,0,0], weight=2, level=135)

def_obj(961,
    keywords='grandmaster hood white gold', short='a white gold-traced grandmaster hood',
    description='A grandmaster hood at peak ward rating, completing the grandmaster set.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[18,0,0,0,0,0,0,0,0,0], weight=2, level=135)

# ===========================================================================
# OBJECTS — Batch 4 (962-976): Melee/Tank Armor Low (levels 10-60)
# Shop room: 1042 (row 6 col 5), keeper: mob 933
# Weight 6-10 = melee; 11-15 = tank
# IT_ARMOR values: [ac, ...]
# ===========================================================================

def_obj(962,
    keywords='leather vest studded light', short='a light studded leather vest',
    description='A basic studded leather vest offering entry-level physical protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[3,0,0,0,0,0,0,0,0,0], weight=7, level=10)

def_obj(963,
    keywords='leather cap studded', short='a studded leather cap',
    description='A leather cap reinforced with iron studs along the crown.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[2,0,0,0,0,0,0,0,0,0], weight=6, level=10)

def_obj(964,
    keywords='leather greaves simple', short='simple leather greaves',
    description='Leather greaves that protect the shins and calves without restricting movement.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[2,0,0,0,0,0,0,0,0,0], weight=7, level=15)

def_obj(965,
    keywords='leather boots thick', short='a pair of thick leather boots',
    description='Heavy-soled leather boots reinforced at the ankle and toe.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[2,0,0,0,0,0,0,0,0,0], weight=8, level=15)

def_obj(966,
    keywords='ringmail shirt light', short='a light ringmail shirt',
    description='Ringmail shirt at the lighter end of metal armour, suitable for active fighters.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[5,0,0,0,0,0,0,0,0,0], weight=9, level=20)

def_obj(967,
    keywords='iron helmet open face', short='an open-faced iron helmet',
    description='A functional iron helmet with an open face and padded interior.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[4,0,0,0,0,0,0,0,0,0], weight=10, level=20)

def_obj(968,
    keywords='chainmail coif iron', short='an iron chainmail coif',
    description='A chainmail coif that covers head and neck with interlocking iron rings.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[5,0,0,0,0,0,0,0,0,0], weight=10, level=25)

def_obj(969,
    keywords='brigandine vest iron plate', short='an iron-plated brigandine vest',
    description='A brigandine coat with iron plates riveted between two layers of leather.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[7,0,0,0,0,0,0,0,0,0], weight=11, level=25)

def_obj(970,
    keywords='steel vambraces solid', short='a pair of solid steel vambraces',
    description='Solid steel vambraces that protect the forearms and can double as a blocking surface.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ARMS),
    item_apply=0, values=[4,0,0,0,0,0,0,0,0,0], weight=9, level=30)

def_obj(971,
    keywords='platemail chestpiece steel', short='a steel platemail chestpiece',
    description='A solid steel plate chest piece appropriate for front-line tank duty.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[9,0,0,0,0,0,0,0,0,0], weight=13, level=30)

def_obj(972,
    keywords='scale greaves heavy', short='a pair of heavy scale greaves',
    description='Heavy greaves of overlapping steel scale that provide substantial leg protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[7,0,0,0,0,0,0,0,0,0], weight=12, level=40)

def_obj(973,
    keywords='full helm steel enclosed', short='an enclosed steel full helm',
    description='A closed steel helm offering full head protection at the cost of some peripheral vision.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[8,0,0,0,0,0,0,0,0,0], weight=12, level=40)

def_obj(974,
    keywords='plate gauntlets steel reinforced', short='reinforced steel plate gauntlets',
    description='Heavy steel gauntlets reinforced at the knuckle and back of hand for close combat.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HANDS),
    item_apply=0, values=[6,0,0,0,0,0,0,0,0,0], weight=10, level=50)

def_obj(975,
    keywords='plate boots steel heavy', short='heavy steel plate boots',
    description='Heavy steel plate boots that provide maximum foot protection at the cost of speed.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[6,0,0,0,0,0,0,0,0,0], weight=12, level=50)

def_obj(976,
    keywords='plate pauldrons heavy steel', short='heavy steel plate pauldrons',
    description='Plate pauldrons of the kind issued to heavy infantry in the city militia.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(SHOULDERS),
    item_apply=0, values=[7,0,0,0,0,0,0,0,0,0], weight=12, level=60)

# ===========================================================================
# OBJECTS — Batch 5 (977-991): Melee/Tank Armor Mid (levels 70-105)
# Shop room: 1043 (row 6 col 6), keeper: mob 934
# ===========================================================================

def_obj(977,
    keywords='reinforced plate chest dwarven', short='a dwarven reinforced plate chest',
    description='A plate chestpiece forged in the dwarven highland tradition, with interlocking reinforcement.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[12,0,0,0,0,0,0,0,0,0], weight=13, level=70)

def_obj(978,
    keywords='full plate helm dwarven steel', short='a dwarven full plate helm',
    description='A dwarven-forged full helm with improved visor and reinforced crown.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[10,0,0,0,0,0,0,0,0,0], weight=13, level=70)

def_obj(979,
    keywords='plate greaves dwarven heavy', short='heavy dwarven plate greaves',
    description='Dwarven plate greaves with overlapping panels that allow good articulation despite weight.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[10,0,0,0,0,0,0,0,0,0], weight=13, level=75)

def_obj(980,
    keywords='dwarven boots plate heavy', short='heavy dwarven plate boots',
    description='Dwarven plate boots with reinforced toe-cap and ankle guard.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[10,0,0,0,0,0,0,0,0,0], weight=13, level=75)

def_obj(981,
    keywords='tower shield arm guard', short='a tower shield arm guard',
    description='A left-arm mounted shield extension that adds protection without occupying the hand.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ARMS),
    item_apply=0, values=[9,0,0,0,0,0,0,0,0,0], weight=12, level=80)

def_obj(982,
    keywords='plate belt heavy iron', short='a heavy iron plate belt',
    description='A solid iron plate belt that protects the midsection without restricting movement.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(WAIST),
    item_apply=0, values=[8,0,0,0,0,0,0,0,0,0], weight=11, level=80)

def_obj(983,
    keywords='high guard plate chest steel alloy', short='a high-guard steel alloy chest plate',
    description='A chest plate in a steel alloy providing better protection than standard steel at lower weight.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[14,0,0,0,0,0,0,0,0,0], weight=13, level=85)

def_obj(984,
    keywords='alloy pauldrons steel high', short='high-grade steel alloy pauldrons',
    description='Pauldrons in high-grade steel alloy, improved over standard issue in both weight and rating.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(SHOULDERS),
    item_apply=0, values=[11,0,0,0,0,0,0,0,0,0], weight=12, level=85)

def_obj(985,
    keywords='alloy gauntlets high grade', short='high-grade alloy plate gauntlets',
    description='Heavy gauntlets in steel alloy, offering mid-tier high hand protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HANDS),
    item_apply=0, values=[10,0,0,0,0,0,0,0,0,0], weight=11, level=90)

def_obj(986,
    keywords='mithril chest half plate', short='a mithril half-plate chest',
    description='A half-plate chest piece in mithril alloy, providing excellent protection with reduced weight.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[16,0,0,0,0,0,0,0,0,0], weight=12, level=90)

def_obj(987,
    keywords='mithril helm half plate', short='a mithril half-plate helm',
    description='A mithril helm offering outstanding head protection at significantly reduced weight.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[13,0,0,0,0,0,0,0,0,0], weight=11, level=95)

def_obj(988,
    keywords='mithril greaves half plate', short='mithril half-plate greaves',
    description='Mithril greaves at mid-high tier, combining excellent protection with light-footed performance.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[13,0,0,0,0,0,0,0,0,0], weight=11, level=95)

def_obj(989,
    keywords='full mithril plate chest', short='a full mithril plate chest',
    description='A full plate chest in mithril construction, approaching the high tier of tank protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[18,0,0,0,0,0,0,0,0,0], weight=13, level=100)

def_obj(990,
    keywords='mithril boots full plate', short='full mithril plate boots',
    description='Full mithril plate boots at the high end of the mid tier.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[13,0,0,0,0,0,0,0,0,0], weight=12, level=100)

def_obj(991,
    keywords='mithril plate arms vambrace', short='mithril plate vambraces',
    description='Mithril plate vambraces approaching high-tier protection for the forearms.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ARMS),
    item_apply=0, values=[13,0,0,0,0,0,0,0,0,0], weight=11, level=105)

# ===========================================================================
# OBJECTS — Batch 6 (992-1006): Melee/Tank Armor High (levels 110-135)
# Shop room: 1044 (row 6 col 7), keeper: mob 935
# ===========================================================================

def_obj(992,
    keywords='adamantine plate chest', short='an adamantine plate chest',
    description='A chest plate forged from adamantine, offering exceptional protection at high tier.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[20,0,0,0,0,0,0,0,0,0], weight=14, level=110)

def_obj(993,
    keywords='adamantine plate helm', short='an adamantine plate helm',
    description='A full helm cast from adamantine, the hardest commercially worked metal.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[16,0,0,0,0,0,0,0,0,0], weight=13, level=110)

def_obj(994,
    keywords='adamantine plate greaves', short='adamantine plate greaves',
    description='Adamantine greaves that protect the legs with extreme durability.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[16,0,0,0,0,0,0,0,0,0], weight=13, level=115)

def_obj(995,
    keywords='adamantine plate boots', short='adamantine plate boots',
    description='Adamantine boots that provide maximum foot protection for front-line combat.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[16,0,0,0,0,0,0,0,0,0], weight=13, level=115)

def_obj(996,
    keywords='adamantine gauntlets plate', short='adamantine plate gauntlets',
    description='Heavy adamantine gauntlets that turn aside blows at the highest common tier.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HANDS),
    item_apply=0, values=[14,0,0,0,0,0,0,0,0,0], weight=12, level=120)

def_obj(997,
    keywords='adamantine pauldrons plate', short='adamantine plate pauldrons',
    description='Adamantine pauldrons protecting the shoulder at the highest commercial standard.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(SHOULDERS),
    item_apply=0, values=[14,0,0,0,0,0,0,0,0,0], weight=13, level=120)

def_obj(998,
    keywords='warlord plate chest supreme', short='a supreme warlord plate chest',
    description='A chest plate used by warlord-rank fighters, the pinnacle of high-tier melee protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[22,0,0,0,0,0,0,0,0,0], weight=15, level=125)

def_obj(999,
    keywords='warlord plate helm supreme', short='a supreme warlord plate helm',
    description='A warlord-grade helm offering supreme head protection in the high tier.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[18,0,0,0,0,0,0,0,0,0], weight=14, level=125)

def_obj(1000,
    keywords='warlord plate greaves supreme', short='supreme warlord plate greaves',
    description='Warlord-grade greaves, supreme protection for the legs in high-tier combat.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[18,0,0,0,0,0,0,0,0,0], weight=14, level=125)

def_obj(1001,
    keywords='warlord plate boots supreme', short='supreme warlord plate boots',
    description='Warlord-grade boots at the peak of high-tier foot protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FEET),
    item_apply=0, values=[18,0,0,0,0,0,0,0,0,0], weight=14, level=130)

def_obj(1002,
    keywords='warlord vambraces plate supreme', short='supreme warlord plate vambraces',
    description='Supreme warlord vambraces at high-tier arm protection standard.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ARMS),
    item_apply=0, values=[17,0,0,0,0,0,0,0,0,0], weight=13, level=130)

def_obj(1003,
    keywords='champion plate chest ultimate', short='an ultimate champion plate chest',
    description='The finest plate chest Draga stocks: ultimate-grade champion armour at the tier ceiling.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[25,0,0,0,0,0,0,0,0,0], weight=15, level=130)

def_obj(1004,
    keywords='champion plate helm ultimate', short='an ultimate champion plate helm',
    description='Ultimate champion helm at the ceiling of commercially available head protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[20,0,0,0,0,0,0,0,0,0], weight=14, level=135)

def_obj(1005,
    keywords='champion plate greaves ultimate', short='ultimate champion plate greaves',
    description='Ultimate champion greaves at the high-tier ceiling for leg protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(LEGS),
    item_apply=0, values=[20,0,0,0,0,0,0,0,0,0], weight=14, level=135)

def_obj(1006,
    keywords='champion belt waist plate ultimate', short='an ultimate champion plate belt',
    description='An ultimate plate belt at the peak of commercially available waist protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(WAIST),
    item_apply=0, values=[17,0,0,0,0,0,0,0,0,0], weight=13, level=135)

# ===========================================================================
# OBJECTS — Batch 7 (1007-1021): Caster Weapons (levels 10-145)
# Shop room: 1045 (row 6 col 8), keeper: mob 936
# IT_WEAPON (5) values: [weapon_class, num_dice, size_dice, spell_or_0, 0, ...]
# Caster weapons: staves, wands, daggers, focus items
# weapon_class: 0=hit, 1=slash, 2=stab, 3=whip, 4=crush, 6=claw, 7=pierce
# ===========================================================================

def_obj(1007,
    keywords='gnarled walking staff apprentice', short='a gnarled apprentice walking staff',
    description='A simple gnarled staff cut from hawthorn wood, sufficient for an apprentice caster.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,1,4,0,0,0,0,0,0,0], weight=3, level=10)

def_obj(1008,
    keywords='focus dagger iron sigil etched', short='an iron sigil-etched focus dagger',
    description='A short iron dagger with sigils etched into the flat, useful as a spell-casting focus.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[7,1,5,0,0,0,0,0,0,0], weight=2, level=20)

def_obj(1009,
    keywords='crystal tipped wand short', short='a short crystal-tipped wand',
    description='A wand tipped with a clear crystal that amplifies basic channelling output.',
    item_type=IT_WEAPON, extra_flags=MAGIC|WAND, wear_flags=W(HOLD),
    item_apply=0, values=[4,1,6,0,0,0,0,0,0,0], weight=1, level=25)

def_obj(1010,
    keywords='staff ironwood long', short='a long ironwood staff',
    description='A staff cut from seasoned ironwood, with the natural hardness to serve mid-tier combat.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,2,4,0,0,0,0,0,0,0], weight=4, level=35)

def_obj(1011,
    keywords='runed dagger silver arcane', short='a silver arcane runed dagger',
    description='A silver dagger with arcane runes ground into the blade that improve channelling efficiency.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[7,2,5,0,0,0,0,0,0,0], weight=2, level=45)

def_obj(1012,
    keywords='focus wand ebony carved', short='a carved ebony focus wand',
    description='An ebony wand carved with complex focus sigils for mid-tier channelling work.',
    item_type=IT_WEAPON, extra_flags=MAGIC|WAND, wear_flags=W(HOLD),
    item_apply=0, values=[4,2,6,0,0,0,0,0,0,0], weight=2, level=55)

def_obj(1013,
    keywords='mage staff bound silver', short='a silver-bound mage staff',
    description='A staff bound at shaft and tip with silver that significantly improves arcane resonance.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,2,7,0,0,0,0,0,0,0], weight=4, level=65)

def_obj(1014,
    keywords='adept staff mithril cap', short='a mithril-capped adept staff',
    description='A sturdy staff with mithril caps at both ends, offering adept-tier channelling support.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,3,6,0,0,0,0,0,0,0], weight=4, level=75)

def_obj(1015,
    keywords='arcane blade short slender', short='a slender short arcane blade',
    description='A slender blade with arcane channels ground along its length for focus-combat hybrid use.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,3,6,0,0,0,0,0,0,0], weight=3, level=85)

def_obj(1016,
    keywords='high adept wand platinum', short='a platinum high-adept wand',
    description='A platinum-alloy wand resonating at high-adept channelling frequency.',
    item_type=IT_WEAPON, extra_flags=MAGIC|WAND, wear_flags=W(HOLD),
    item_apply=0, values=[4,3,7,0,0,0,0,0,0,0], weight=2, level=95)

def_obj(1017,
    keywords='archmage staff platinum rune', short='a platinum rune-inscribed archmage staff',
    description='A staff inscribed with platinum runes, rated for archmage-level output.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,4,6,0,0,0,0,0,0,0], weight=4, level=105)

def_obj(1018,
    keywords='grand staff crystal arcane', short='a grand crystal arcane staff',
    description='A staff set with a large arcane crystal that provides outstanding channelling amplification.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,4,7,0,0,0,0,0,0,0], weight=4, level=115)

def_obj(1019,
    keywords='supreme wand mithril crystal', short='a mithril crystal supreme wand',
    description='A supreme-tier wand with mithril body and embedded crystal for maximum output.',
    item_type=IT_WEAPON, extra_flags=MAGIC|WAND, wear_flags=W(HOLD),
    item_apply=0, values=[4,4,8,0,0,0,0,0,0,0], weight=2, level=125)

def_obj(1020,
    keywords='grandmaster staff adamantine', short='an adamantine grandmaster staff',
    description='A grandmaster staff with adamantine shaft and caps, at the commercial tier ceiling for staves.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,5,7,0,0,0,0,0,0,0], weight=4, level=135)

def_obj(1021,
    keywords='ultimate focus blade arcane', short='an ultimate arcane focus blade',
    description='Thalindra\'s finest: an arcane blade at the ultimate commercial tier for caster weapons.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,5,8,0,0,0,0,0,0,0], weight=3, level=145)

# ===========================================================================
# OBJECTS — Batch 8 (1022-1036): Melee/Tank Weapons (levels 10-145)
# Shop room: 1046 (row 6 col 9), keeper: mob 937
# weapon_class: 0=hit, 1=slash, 2=stab, 4=crush
# TWO_HAND flag for two-handed weapons
# ===========================================================================

def_obj(1022,
    keywords='short sword iron training', short='an iron training short sword',
    description='A standard iron short sword of the kind issued to militia trainees.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,1,5,0,0,0,0,0,0,0], weight=7, level=10)

def_obj(1023,
    keywords='iron mace simple', short='a simple iron mace',
    description='A basic iron mace with a flanged head, reliable if unadorned.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,1,6,0,0,0,0,0,0,0], weight=9, level=15)

def_obj(1024,
    keywords='steel sword straight one hand', short='a straight steel one-handed sword',
    description='A straight steel blade suitable for one-handed fighting styles.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,2,5,0,0,0,0,0,0,0], weight=8, level=25)

def_obj(1025,
    keywords='battle axe steel', short='a steel battle axe',
    description='A well-balanced steel battle axe with a single broad blade.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,2,6,0,0,0,0,0,0,0], weight=10, level=35)

def_obj(1026,
    keywords='warhammer iron reinforced', short='a reinforced iron warhammer',
    description='An iron warhammer with reinforced handle and double-face head for smashing blows.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,2,7,0,0,0,0,0,0,0], weight=12, level=45)

def_obj(1027,
    keywords='long sword steel quality', short='a quality steel long sword',
    description='A quality long sword with a blade length appropriate for mid-tier swordwork.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,3,6,0,0,0,0,0,0,0], weight=9, level=55)

def_obj(1028,
    keywords='great sword two handed steel', short='a steel two-handed great sword',
    description='A steel great sword requiring both hands, dealing heavy slashing damage.',
    item_type=IT_WEAPON, extra_flags=MAGIC|TWO_HAND, wear_flags=W(HOLD),
    item_apply=0, values=[1,3,8,0,0,0,0,0,0,0], weight=14, level=65)

def_obj(1029,
    keywords='mithril sword one hand', short='a mithril one-handed sword',
    description='A mithril alloy sword with an edge that holds longer than standard steel.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,3,7,0,0,0,0,0,0,0], weight=8, level=75)

def_obj(1030,
    keywords='mithril warhammer heavy', short='a heavy mithril warhammer',
    description='A mithril warhammer with exceptional impact at the mid-high tier.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,3,8,0,0,0,0,0,0,0], weight=12, level=85)

def_obj(1031,
    keywords='mithril great sword two hand', short='a mithril two-handed great sword',
    description='A mithril great sword offering superior performance for heavy two-handed fighters.',
    item_type=IT_WEAPON, extra_flags=MAGIC|TWO_HAND, wear_flags=W(HOLD),
    item_apply=0, values=[1,4,7,0,0,0,0,0,0,0], weight=13, level=95)

def_obj(1032,
    keywords='adamantine sword one hand', short='an adamantine one-handed sword',
    description='An adamantine sword at the high tier of one-handed combat performance.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,4,7,0,0,0,0,0,0,0], weight=9, level=105)

def_obj(1033,
    keywords='adamantine maul heavy two hand', short='a heavy adamantine maul',
    description='An adamantine maul for two-handed smashing combat at the high tier.',
    item_type=IT_WEAPON, extra_flags=MAGIC|TWO_HAND, wear_flags=W(HOLD),
    item_apply=0, values=[4,4,8,0,0,0,0,0,0,0], weight=15, level=115)

def_obj(1034,
    keywords='warlord sword adamantine supreme', short='a supreme warlord adamantine sword',
    description='A warlord-grade adamantine sword near the ceiling of high-tier one-handed performance.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,5,7,0,0,0,0,0,0,0], weight=10, level=125)

def_obj(1035,
    keywords='champion great sword ultimate two hand', short='an ultimate champion two-handed great sword',
    description='A two-handed champion-grade great sword at the ultimate commercial tier.',
    item_type=IT_WEAPON, extra_flags=MAGIC|TWO_HAND, wear_flags=W(HOLD),
    item_apply=0, values=[1,5,9,0,0,0,0,0,0,0], weight=15, level=135)

def_obj(1036,
    keywords='ultimate warhammer champion heavy', short='an ultimate champion warhammer',
    description='Gorven\'s finest: an ultimate-tier champion warhammer at the ceiling of commercial melee weapons.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,5,8,0,0,0,0,0,0,0], weight=14, level=145)

# ===========================================================================
# OBJECTS — Batch 9 (1037-1051): General Store Lights (levels 10-145)
# Shop room: 1047 (row 6 col 10), keeper: mob 938
# IT_LIGHT (1) values: [0, 0, hours, 0, 0, 0, 0, 0, 0, 0]
# hours: 0 = infinite, positive = timed
# ===========================================================================

def_obj(1037,
    keywords='tallow candle short', short='a short tallow candle',
    description='A cheap tallow candle that provides enough light for reading or corridor work.',
    item_type=IT_LIGHT, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,48,0,0,0,0,0,0,0], weight=1, level=10)

def_obj(1038,
    keywords='lantern oil common', short='a common oil lantern',
    description='A standard oil lantern with a shuttered glass face and iron handle.',
    item_type=IT_LIGHT, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,72,0,0,0,0,0,0,0], weight=4, level=10)

def_obj(1039,
    keywords='torch pine tar', short='a pine-tar torch',
    description='A pine-tar soaked torch that burns hot and bright.',
    item_type=IT_LIGHT, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,36,0,0,0,0,0,0,0], weight=2, level=15)

def_obj(1040,
    keywords='glow crystal small', short='a small glow crystal',
    description='A small crystal with a persistent luminescent charge useful for casters.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=1, level=20)

def_obj(1041,
    keywords='ward lamp small arcane', short='a small arcane ward lamp',
    description='A ward lamp whose light suppresses low-level shadows and hostile minor entities.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=3, level=30)

def_obj(1042,
    keywords='beacon stone cold fire', short='a cold-fire beacon stone',
    description='A stone that emits cold bluish light without heat, favoured by dungeon explorers.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=2, level=40)

def_obj(1043,
    keywords='lantern silver enchanted', short='an enchanted silver lantern',
    description='A silver lantern whose enchanted flame burns indefinitely without fuel.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=3, level=50)

def_obj(1044,
    keywords='arcane torch floating', short='a floating arcane torch',
    description='An arcane torch that hovers at shoulder height and follows the bearer.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=1, level=60)

def_obj(1045,
    keywords='war lantern iron heavy', short='a heavy iron war lantern',
    description='A heavy iron lantern built to withstand combat conditions and stay lit.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=7, level=70)

def_obj(1046,
    keywords='mithril lantern bright', short='a bright mithril lantern',
    description='A mithril-bodied lantern that provides exceptional illumination in dark environments.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=4, level=85)

def_obj(1047,
    keywords='ward globe silver arcane bright', short='a bright silver arcane ward globe',
    description='A globe of compressed arcane light, bright enough to illuminate a full corridor.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=2, level=100)

def_obj(1048,
    keywords='sunstone brilliant gold', short='a brilliant gold sunstone',
    description='A gold-veined sunstone that emits light equivalent to daylight in its immediate vicinity.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=3, level=110)

def_obj(1049,
    keywords='adamantine lantern sealed', short='a sealed adamantine lantern',
    description='An adamantine lantern sealed against weather and impacts, burning indefinitely.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=8, level=125)

def_obj(1050,
    keywords='prime focus crystal light ultimate', short='an ultimate prime focus crystal',
    description='A focus crystal at the apex of light items, providing extraordinary illumination.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=1, level=135)

def_obj(1051,
    keywords='eternal flame shard sacred', short='a shard of eternal sacred flame',
    description='Oswin\'s rarest stock: a shard of eternal flame that never dims regardless of conditions.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=1, level=145)

# ===========================================================================
# OBJECTS — Batch 10 (1052-1066): Postmaster Items (scrolls/maps)
# Shop room: 1048 (row 6 col 11), keeper: mob 939
# IT_SCROLL (2) values: [level, spell1, spell2, spell3, 0, ...]
# spell vnums: using 0 = no spell (placeholder items for postmaster flavour)
# ===========================================================================

def_obj(1052,
    keywords='scroll recall city blank', short='a blank city recall scroll',
    description='A scroll enchanted to return the bearer to the city centre when activated.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[10,62,0,0,0,0,0,0,0,0], weight=1, level=10)

def_obj(1053,
    keywords='map district northern quarter', short='a map of the northern quarter',
    description='A detailed hand-drawn map of the northern quarter, current to the last civic survey.',
    item_type=IT_SCROLL, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[5,0,0,0,0,0,0,0,0,0], weight=1, level=10)

def_obj(1054,
    keywords='sealed letter courier standard', short='a standard courier\'s sealed letter',
    description='A sealed letter ready for delivery, handed in at any postal station.',
    item_type=IT_SCROLL, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[5,0,0,0,0,0,0,0,0,0], weight=1, level=10)

def_obj(1055,
    keywords='scroll identify basic', short='a basic identify scroll',
    description='A scroll that reveals the basic properties of an item when read.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[15,53,0,0,0,0,0,0,0,0], weight=1, level=15)

def_obj(1056,
    keywords='scroll detect invis', short='a detect invis scroll',
    description='A scroll granting temporary ability to detect invisible objects and creatures.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[20,17,0,0,0,0,0,0,0,0], weight=1, level=20)

def_obj(1057,
    keywords='scroll fly levitation', short='a levitation scroll',
    description='A scroll that grants brief levitation to the user.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[25,56,0,0,0,0,0,0,0,0], weight=1, level=25)

def_obj(1058,
    keywords='scroll haste speed', short='a haste scroll',
    description='A scroll that temporarily speeds the user\'s reflexes and movement.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[30,44,0,0,0,0,0,0,0,0], weight=1, level=30)

def_obj(1059,
    keywords='scroll pass door', short='a pass door scroll',
    description='A scroll that allows the user to pass through a locked or barred door once.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[35,76,0,0,0,0,0,0,0,0], weight=1, level=35)

def_obj(1060,
    keywords='scroll protection evil', short='a protection from evil scroll',
    description='A scroll that raises a ward against evil-aligned attacks and creatures.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[40,79,0,0,0,0,0,0,0,0], weight=1, level=40)

def_obj(1061,
    keywords='scroll protection good', short='a protection from good scroll',
    description='A scroll that raises a ward against good-aligned threats.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[40,80,0,0,0,0,0,0,0,0], weight=1, level=40)

def_obj(1062,
    keywords='scroll word recall premium', short='a premium word of recall scroll',
    description='A premium-grade recall scroll with a strengthened enchantment for reliable function.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[50,62,0,0,0,0,0,0,0,0], weight=1, level=50)

def_obj(1063,
    keywords='scroll remove curse', short='a remove curse scroll',
    description='A scroll with sufficient power to lift most standard curses.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[55,84,0,0,0,0,0,0,0,0], weight=1, level=55)

def_obj(1064,
    keywords='scroll teleport short range', short='a short-range teleport scroll',
    description='A scroll that teleports the user to a random nearby location.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[60,112,0,0,0,0,0,0,0,0], weight=1, level=60)

def_obj(1065,
    keywords='scroll gate realm', short='a realm gate scroll',
    description='A scroll that opens a temporary gate to a known destination.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[75,39,0,0,0,0,0,0,0,0], weight=1, level=75)

def_obj(1066,
    keywords='scroll dispel magic master', short='a master dispel magic scroll',
    description='A powerful scroll capable of stripping significant enchantments from a target.',
    item_type=IT_SCROLL, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[90,20,0,0,0,0,0,0,0,0], weight=1, level=90)

# ===========================================================================
# OBJECTS — Batch 11 (1067-1081): Quartermaster Items
# Shop room: 1049 (row 6 col 12), keeper: mob 940
# Mix of potions, utility armor, and misc gear across all levels
# IT_POTION (10) values: [level, spell1, spell2, spell3, ...]
# ===========================================================================

def_obj(1067,
    keywords='potion cure light wounds', short='a potion of cure light wounds',
    description='A small flask of restorative liquid that heals minor wounds.',
    item_type=IT_POTION, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[10,2,0,0,0,0,0,0,0,0], weight=1, level=10)

def_obj(1068,
    keywords='potion cure serious wounds', short='a potion of cure serious wounds',
    description='A flask of restorative liquid that heals significant wounds.',
    item_type=IT_POTION, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[30,4,0,0,0,0,0,0,0,0], weight=1, level=30)

def_obj(1069,
    keywords='potion cure critical wounds', short='a potion of cure critical wounds',
    description='A flask of potent healing that restores major injuries.',
    item_type=IT_POTION, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[60,5,0,0,0,0,0,0,0,0], weight=1, level=60)

def_obj(1070,
    keywords='potion heroism bless strength', short='a potion of heroism',
    description='A potion that grants temporary battlefield enhancement to the drinker.',
    item_type=IT_POTION, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[40,6,94,0,0,0,0,0,0,0], weight=1, level=40)

def_obj(1071,
    keywords='iron ring grip plain', short='a plain iron grip ring',
    description='A plain iron ring worn on the finger to improve weapon grip in wet conditions.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FINGER),
    item_apply=0, values=[1,0,0,0,0,0,0,0,0,0], weight=1, level=20)

def_obj(1072,
    keywords='neck clasp iron simple', short='a simple iron neck clasp',
    description='A simple iron clasp worn at the neck to secure cloaks and provide minimal protection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(NECK),
    item_apply=0, values=[2,0,0,0,0,0,0,0,0,0], weight=1, level=20)

def_obj(1073,
    keywords='waist sash reinforced leather', short='a reinforced leather waist sash',
    description='A thick leather sash with iron-reinforced panels protecting the midsection.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(WAIST),
    item_apply=0, values=[3,0,0,0,0,0,0,0,0,0], weight=4, level=25)

def_obj(1074,
    keywords='ear stud iron ward small', short='a small iron ward ear stud',
    description='A small iron stud worn in the ear with a faint protective ward.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(EAR),
    item_apply=0, values=[1,0,0,0,0,0,0,0,0,0], weight=0, level=15)

def_obj(1075,
    keywords='potion strength bull', short='a potion of bull\'s strength',
    description='A potion that temporarily grants exceptional physical strength.',
    item_type=IT_POTION, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[50,94,0,0,0,0,0,0,0,0], weight=1, level=50)

def_obj(1076,
    keywords='potion sanctuary divine', short='a potion of divine sanctuary',
    description='A potion that briefly wraps the drinker in a protective divine aura.',
    item_type=IT_POTION, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[70,88,0,0,0,0,0,0,0,0], weight=1, level=70)

def_obj(1077,
    keywords='wrist wrap combat iron', short='an iron combat wrist wrap',
    description='Iron-reinforced wrist wraps for combat protection at the wrist joint.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(WRIST),
    item_apply=0, values=[4,0,0,0,0,0,0,0,0,0], weight=3, level=35)

def_obj(1078,
    keywords='ring steel fortified', short='a fortified steel ring',
    description='A thick steel ring worn on the finger with basic fortifying enchantment.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(FINGER),
    item_apply=0, values=[3,0,0,0,0,0,0,0,0,0], weight=1, level=50)

def_obj(1079,
    keywords='neck gorget steel', short='a steel neck gorget',
    description='A steel gorget protecting the throat and neck, worn over or under armour.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(NECK),
    item_apply=0, values=[5,0,0,0,0,0,0,0,0,0], weight=5, level=55)

def_obj(1080,
    keywords='potion full heal restoration', short='a potion of full restoration',
    description='A premium restorative potion that fully heals most moderate injuries.',
    item_type=IT_POTION, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[100,7,0,0,0,0,0,0,0,0], weight=1, level=100)

def_obj(1081,
    keywords='ear crystal mithril ward', short='a mithril ward ear crystal',
    description='A mithril crystal worn in the ear with a strong protective ward.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(EAR),
    item_apply=0, values=[5,0,0,0,0,0,0,0,0,0], weight=0, level=75)

# ===========================================================================
# OBJECTS — Batch 12 (1082-1096): Invasion Armory Items
# Shop room: 1050 (row 6 col 13), keeper: mob 941
# Battle-ready gear across archetypes, mid-to-high levels
# ===========================================================================

def_obj(1082,
    keywords='invasion shield buckler iron', short='an iron invasion buckler',
    description='A heavy iron buckler issued to front-line defenders during city invasions.',
    item_type=IT_ARMOR, extra_flags=MAGIC|BUCKLER, wear_flags=W(HOLD),
    item_apply=0, values=[8,0,0,0,0,0,0,0,0,0], weight=10, level=50)

def_obj(1083,
    keywords='siege cloak heavy warded', short='a heavy warded siege cloak',
    description='A heavy cloak with multi-layer battle-ward stitching for active combat conditions.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(ABOUT),
    item_apply=0, values=[8,0,0,0,0,0,0,0,0,0], weight=6, level=55)

def_obj(1084,
    keywords='invasion robe battle mage', short='a battle-mage invasion robe',
    description='A reinforced robe designed for casters who need to maintain position under fire.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[10,0,0,0,0,0,0,0,0,0], weight=4, level=65)

def_obj(1085,
    keywords='invasion plate chest battle', short='a battle invasion plate chest',
    description='Heavy chest armour specifically rated for the sustained impact of invasion combat.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[13,0,0,0,0,0,0,0,0,0], weight=14, level=75)

def_obj(1086,
    keywords='siege hammer two hand invasion', short='a two-handed siege hammer',
    description='A heavy two-handed hammer for breaking through defensive formations.',
    item_type=IT_WEAPON, extra_flags=MAGIC|TWO_HAND, wear_flags=W(HOLD),
    item_apply=0, values=[4,4,6,0,0,0,0,0,0,0], weight=15, level=70)

def_obj(1087,
    keywords='invasion sword battle straight', short='a straight battle invasion sword',
    description='A straight sword designed for fast repeated strikes in close-quarters invasion fighting.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,3,7,0,0,0,0,0,0,0], weight=9, level=70)

def_obj(1088,
    keywords='siege staff battle mage heavy', short='a heavy battle-mage siege staff',
    description='A staff reinforced for battle use, maintaining arcane function in active combat.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,3,7,0,0,0,0,0,0,0], weight=5, level=80)

def_obj(1089,
    keywords='battle shield large invasion', short='a large invasion battle shield',
    description='A large rectangular shield providing substantial cover during invasion assaults.',
    item_type=IT_ARMOR, extra_flags=MAGIC|BUCKLER, wear_flags=W(HOLD),
    item_apply=0, values=[12,0,0,0,0,0,0,0,0,0], weight=14, level=85)

def_obj(1090,
    keywords='invasion helm warplate', short='a warplate invasion helm',
    description='A heavy warplate helm rated for invasion-level combat without visor obstruction.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HEAD),
    item_apply=0, values=[13,0,0,0,0,0,0,0,0,0], weight=14, level=90)

def_obj(1091,
    keywords='battle robe arcane reinforced', short='a reinforced arcane battle robe',
    description='A battle robe with triple-layer ward panels for high-tier casters in invasion combat.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(BODY),
    item_apply=0, values=[15,0,0,0,0,0,0,0,0,0], weight=4, level=95)

def_obj(1092,
    keywords='invasion gauntlets mithril battle', short='mithril invasion battle gauntlets',
    description='Mithril gauntlets specifically reinforced for the sustained combat of invasion scenarios.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(HANDS),
    item_apply=0, values=[12,0,0,0,0,0,0,0,0,0], weight=11, level=95)

def_obj(1093,
    keywords='siege sword mithril heavy', short='a heavy mithril siege sword',
    description='A heavy mithril sword for sustained siege combat, with superior edge retention.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,4,8,0,0,0,0,0,0,0], weight=10, level=100)

def_obj(1094,
    keywords='grand invasion buckler adamantine', short='an adamantine grand invasion buckler',
    description='An adamantine buckler at the high tier of defensive invasion equipment.',
    item_type=IT_ARMOR, extra_flags=MAGIC|BUCKLER, wear_flags=W(HOLD),
    item_apply=0, values=[16,0,0,0,0,0,0,0,0,0], weight=12, level=110)

def_obj(1095,
    keywords='battle staff adamantine siege', short='an adamantine siege battle staff',
    description='An adamantine staff built for high-tier battle-casters in invasion conditions.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,5,7,0,0,0,0,0,0,0], weight=5, level=120)

def_obj(1096,
    keywords='supreme invasion sword champion', short='a champion supreme invasion sword',
    description='The warden\'s finest: a champion-grade invasion sword at the upper tier.',
    item_type=IT_WEAPON, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[1,5,8,0,0,0,0,0,0,0], weight=11, level=130)

# ===========================================================================
# OBJECTS — Batch 13 (1097-1116): Misc Items (ambient, keys, food, flavour)
# Not in shops — used in resets for ambient flavour and functional items
# ===========================================================================

def_obj(1097,
    keywords='bread loaf fresh', short='a fresh loaf of bread',
    description='A loaf of fresh bread from the market bakery, still warm.',
    item_type=IT_FOOD, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[8,0,0,0,0,0,0,0,0,0], weight=2, level=1)

def_obj(1098,
    keywords='coin gold midgaard standard', short='a gold standard coin',
    description='A standard gold coin bearing the civic seal of @@WMidgaard@@N.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=0, level=1)

def_obj(1099,
    keywords='temple pendant silver ward', short='a silver ward pendant',
    description='A silver pendant bearing the temple\'s ward-sigil, worn by the faithful.',
    item_type=IT_ARMOR, extra_flags=MAGIC, wear_flags=W(NECK),
    item_apply=0, values=[1,0,0,0,0,0,0,0,0,0], weight=1, level=5)

def_obj(1100,
    keywords='city watch badge iron', short='an iron city watch badge',
    description='An iron badge identifying the bearer as a member of the city watch.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=1, level=1)

def_obj(1101,
    keywords='civic notice board placard', short='a civic notice placard',
    description='A placard from the civic notice board listing current public announcements.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=0, level=1)

def_obj(1102,
    keywords='pilgrim token stone worn', short='a worn pilgrim token',
    description='A smooth stone token carried by a pilgrim, marked with a devotional symbol.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=1, level=1)

def_obj(1103,
    keywords='caravan manifest paper sealed', short='a sealed caravan manifest',
    description='A sealed cargo manifest from the current caravan, marked with a wax seal.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=0, level=1)

def_obj(1104,
    keywords='torch extra spare', short='a spare torch',
    description='A spare torch kept behind the counter for emergencies.',
    item_type=IT_LIGHT, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,36,0,0,0,0,0,0,0], weight=2, level=1)

def_obj(1105,
    keywords='quartermaster manifest supply list', short='a supply manifest',
    description='The quartermaster\'s current supply list, detailing available stock.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=0, level=1)

def_obj(1106,
    keywords='postal bag sealed courier', short='a sealed courier postal bag',
    description='A sealed postal bag ready for the next courier run.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=3, level=1)

def_obj(1107,
    keywords='ritual candle purple temple', short='a purple ritual candle',
    description='A purple candle used in the daily temple observances.',
    item_type=IT_LIGHT, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,24,0,0,0,0,0,0,0], weight=1, level=5)

def_obj(1108,
    keywords='herb bundle tied healer', short='a tied bundle of healing herbs',
    description='A bundle of herbs tied with cord, used by healers in their restorative work.',
    item_type=IT_FOOD, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[4,0,0,0,0,0,0,0,0,0], weight=1, level=10)

def_obj(1109,
    keywords='parchment roll blank inscription', short='a blank parchment roll',
    description='A roll of blank parchment for correspondence or inscription work.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=1, level=1)

def_obj(1110,
    keywords='iron key standard civic', short='a standard civic iron key',
    description='A standard iron key for civic district doors, widely duplicated.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=1, level=1)

def_obj(1111,
    keywords='merchant ledger accounts bound', short='a bound merchant ledger',
    description='A bound merchant ledger filled with accounts in tight script.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=3, level=1)

def_obj(1112,
    keywords='waterskin full travel', short='a full travel waterskin',
    description='A full waterskin for road travel, the standard issue for caravans.',
    item_type=IT_FOOD, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[6,0,0,0,0,0,0,0,0,0], weight=3, level=1)

def_obj(1113,
    keywords='rope hemp standard coil', short='a coil of hemp rope',
    description='A standard coil of hemp rope, twenty feet in length.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=5, level=1)

def_obj(1114,
    keywords='gem sapphire cut small', short='a small cut sapphire',
    description='A small cut sapphire from the jewellers\' district, modest quality.',
    item_type=IT_KEY, extra_flags=0, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=0, level=1)

def_obj(1115,
    keywords='ward chalk stick reliquary', short='a stick of reliquary ward chalk',
    description='Ward chalk used by the reliquary clerks for drawing protective sigils.',
    item_type=IT_KEY, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=0, level=5)

def_obj(1116,
    keywords='temple incense stick sacred', short='a stick of sacred temple incense',
    description='Sacred incense burned at the temple\'s inner altar during the morning rites.',
    item_type=IT_KEY, extra_flags=MAGIC, wear_flags=W(HOLD),
    item_apply=0, values=[0,0,0,0,0,0,0,0,0,0], weight=0, level=5)

# ===========================================================================
# MAIN OUTPUT FUNCTION
# ===========================================================================

def main():
    # -----------------------------------------------------------------------
    # #AREA header
    # -----------------------------------------------------------------------
    o('#AREA')
    o('Midgaard City Center~')
    o('Q 16')
    o('K midgaard city center~')
    o('L @@W1  @@W145~')
    o('N 100')
    o('I 1 145')
    o('V 917 1116')
    o('X 0')
    o('F 0')
    o('U @@WMidgaard\'s lamplit streets bustle with merchants, guards, and pilgrims.@@N~')
    o('O all~')
    o('R all~')
    o('W all~')
    o()

    # -----------------------------------------------------------------------
    # #ROOMS
    # -----------------------------------------------------------------------
    o('#ROOMS')
    for vnum in sorted(ROOM_DB.keys()):
        name, desc, flags, sector, extra = ROOM_DB[vnum]
        row = (vnum - BASE_VNUM) // COLS
        col = (vnum - BASE_VNUM) % COLS
        exits = exits_for(row, col, extra)
        write_room(vnum, name, desc, flags, sector, exits)
    o('#0')
    o()

    # -----------------------------------------------------------------------
    # #MOBILES
    # -----------------------------------------------------------------------
    o('#MOBILES')
    for vnum in sorted(MOB_DB.keys()):
        kw = MOB_DB[vnum]
        write_mob(vnum, **kw)
    o('#0')
    o()

    # -----------------------------------------------------------------------
    # #OBJECTS
    # -----------------------------------------------------------------------
    o('#OBJECTS')
    for vnum in sorted(OBJ_DB.keys()):
        kw = OBJ_DB[vnum]
        write_obj(vnum, **kw)
    o('#0')
    o()

    # -----------------------------------------------------------------------
    # #SHOPS
    # Format: keeper buy0 buy1 buy2 buy3 buy4 profit_buy profit_sell open close
    # buy types: 9=armor, 5=weapon, 1=light, 2=scroll, 10=potion
    # -----------------------------------------------------------------------
    o('#SHOPS')
    # keeper  buy0  buy1  buy2  buy3  buy4  pbuy  psell  open  close
    # Caster armor low (mob 930, room 1039) — sells armor only
    o('930   9 0 0 0 0   100 120   0 24')
    # Caster armor mid (mob 931, room 1040)
    o('931   9 0 0 0 0   100 120   0 24')
    # Caster armor high (mob 932, room 1041)
    o('932   9 0 0 0 0   100 120   0 24')
    # Melee armor low (mob 933, room 1042)
    o('933   9 0 0 0 0   100 120   0 24')
    # Melee armor mid (mob 934, room 1043)
    o('934   9 0 0 0 0   100 120   0 24')
    # Melee armor high (mob 935, room 1044)
    o('935   9 0 0 0 0   100 120   0 24')
    # Caster weapons (mob 936, room 1045) — weapons and armor
    o('936   5 9 0 0 0   100 120   0 24')
    # Melee/tank weapons (mob 937, room 1046)
    o('937   5 9 0 0 0   100 120   0 24')
    # General store lights (mob 938, room 1047) — lights, potions, scrolls
    o('938   1 10 2 0 0   100 120   0 24')
    # Postmaster (mob 939, room 1048) — scrolls
    o('939   2 0 0 0 0   100 120   0 24')
    # Quartermaster (mob 940, room 1049) — armor, potions, weapons
    o('940   9 10 5 0 0   100 120   0 24')
    # Invasion armory (mob 941, room 1050) — weapons and armor
    o('941   5 9 0 0 0   100 120   0 24')
    o('0')
    o()

    # -----------------------------------------------------------------------
    # #RESETS
    # M 0 mob_vnum limit room_vnum
    # G 0 obj_vnum limit  (give last placed mob this object)
    # -----------------------------------------------------------------------
    o('#RESETS')
    o('* Ambient NPCs — scattered across main grid')
    o('M 0 917 20 937')   # city guard at row 1 col 0 (main street)
    o('M 0 917 20 957')   # city guard at row 2 col 0
    o('M 0 917 20 977')   # city guard at row 3 col 0
    o('M 0 917 20 997')   # city guard at row 4 col 0
    o('M 0 918 1 926')    # gate warden at northern gate (r0c9)
    o('M 0 919 2 1026')   # temple guardian at temple
    o('M 0 920 5 937')    # street merchant at row 1 col 0
    o('M 0 920 5 957')    # street merchant at row 2 col 0
    o('M 0 921 1 968')    # caravan master at caravan square (r2c11)
    o('M 0 922 3 968')    # caravan guard at caravan square
    o('M 0 923 5 937')    # street vendor
    o('M 0 923 5 977')    # street vendor
    o('M 0 924 3 1026')   # pilgrim at temple
    o('M 0 925 5 937')    # beggar
    o('M 0 926 3 946')    # noble citizen r1c9
    o()
    o('* Gertrude at temple (vnum 1026)')
    o('M 0 927 1 1026')
    o()
    o('* Healer and priest')
    o('M 0 928 1 1026')
    o('M 0 929 1 1026')
    o()
    o('* Shopkeepers with inventory immediately following each M reset')
    shop_data = [
        (930, 1039, range(917, 932),  'Caster Armor Low'),
        (931, 1040, range(932, 947),  'Caster Armor Mid'),
        (932, 1041, range(947, 962),  'Caster Armor High'),
        (933, 1042, range(962, 977),  'Melee Armor Low'),
        (934, 1043, range(977, 992),  'Melee Armor Mid'),
        (935, 1044, range(992, 1007), 'Melee Armor High'),
        (936, 1045, range(1007,1022), 'Caster Weapons'),
        (937, 1046, range(1022,1037), 'Melee/Tank Weapons'),
        (938, 1047, range(1037,1052), 'General Store Lights'),
        (939, 1048, range(1052,1067), 'Postmaster Scrolls'),
        (940, 1049, range(1067,1082), 'Quartermaster'),
        (941, 1050, range(1082,1097), 'Invasion Armory'),
    ]
    for mob_v, room_v, obj_range, label in shop_data:
        o('* %s (mob %d, room %d)' % (label, mob_v, room_v))
        o('M 0 %d 1 %d' % (mob_v, room_v))
        for ov in obj_range:
            o('G 0 %d 1' % ov)
        o()
    o('* Ambient objects (given to last reset mob — invasion warden)')
    o('G 0 1104 5')   # spare torches scattered
    o('G 0 1097 5')   # bread in market area
    o('G 0 1112 5')   # waterskins
    o('S')
    o()

    # -----------------------------------------------------------------------
    # #SPECIALS
    # Assignments chosen for thematic fit:
    #  917 city guard       → spec_policeman (hunts killers/thieves on patrol)
    #  918 gate warden      → spec_midgaard_gate_warden (Northern Gate dialogue,
    #                          arrivals/departures log, road conditions, portcullis)
    #  919 temple guardian  → spec_cast_cleric (divine combat, earthquake/flamestrike)
    #  920 street merchant  → spec_vendor (calls out goods, tariff/guild patter)
    #  921 caravan master   → spec_midgaard_caravan_master (Caravan Square staging,
    #                          Trade Syndic compacts, route brands, Ledgerhouse refs)
    #  922 caravan guard    → spec_policeman (law enforcement)
    #  923 street vendor    → spec_vendor (street calls)
    #  924 pilgrim          → spec_midgaard_pilgrim (arrived at the temple, First Stone
    #                          Accord, ward sigils, altar observance)
    #  925 beggar           → spec_thief (pickpocket, thematically a street beggar)
    #  927 Gertrude         → spec_executioner (hunts killers/thieves/undead, summons guards)
    #  928 healer           → spec_cast_cleric (cleric combat proc; healing via ACT_HEALER)
    #  929 temple priest    → spec_lay_sister (temple liturgy, memorial observance)
    #  930-938 shopkeepers  → spec_vendor (merchant atmosphere for all shop rows)
    #  939 postmaster       → spec_posting_clerk (notice board + quest hooks)
    #  940 quartermaster    → spec_records_keeper (inventory ledger + survey quests)
    #  941 invasion warden  → spec_warden (Compact checkpoint, registry notation)
    # -----------------------------------------------------------------------
    o('#SPECIALS')
    o('M 917 spec_policeman')
    o('M 918 spec_midgaard_gate_warden')
    o('M 919 spec_cast_cleric')
    o('M 920 spec_vendor')
    o('M 921 spec_midgaard_caravan_master')
    o('M 922 spec_policeman')
    o('M 923 spec_vendor')
    o('M 924 spec_midgaard_pilgrim')
    o('M 925 spec_thief')
    o('M 927 spec_executioner')
    o('M 928 spec_cast_cleric')
    o('M 929 spec_lay_sister')
    o('M 930 spec_vendor')
    o('M 931 spec_vendor')
    o('M 932 spec_vendor')
    o('M 933 spec_vendor')
    o('M 934 spec_vendor')
    o('M 935 spec_vendor')
    o('M 936 spec_vendor')
    o('M 937 spec_vendor')
    o('M 938 spec_vendor')
    o('M 939 spec_posting_clerk')
    o('M 940 spec_records_keeper')
    o('M 941 spec_warden')
    o('S')
    o()

    o('#$')

if __name__ == '__main__':
    main()
