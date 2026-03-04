/***************************************************************************
 * invasion.c -- Invasion System for Shades of Evil                        *
 *                                                                         *
 * Rules:                                                                  *
 *  - Starts only when >= 1 regular (non-immortal) player is online.       *
 *  - Spawns a BOSS whose level = highest player pseudo_level + 20.        *
 *    Boss has scaled hp_mod (100 to 250,000) based on level.              *
 *    Boss has unique elemental strengths, weaknesses, spells & skills.    *
 *  - Every PULSE_TICK spawns ordinary wave mobs scaled by player count.   *
 *  - Every PULSE_ROOMS the spawned mobs march toward room 3001.           *
 *  - On reaching 3001 the mobs sacrifice themselves to wound Gertrude.      *
 *  - If Gertrude dies  -> failure announcement.                           *
 *  - If the Boss dies  -> success + 25 QP + despawn ALL invasion mobs.    *
 *  - Spawned mobs are NOT aggressive, are SENTINEL, and have ACT_NOASSIST. *
 *                                                                         *
 * Data tables live in separate files:                                     *
 *   invasion_mobs.h   -- WAVE_MOB_PROFILE struct + 45-entry array         *
 *   invasion_bosses.h -- BOSS_PROFILE struct + 15-entry array             *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifndef DEC_GLOBALS_H
#include "globals.h"
#endif

#ifndef INVASION_H
#include "invasion.h"
#endif

#ifndef DEC_MAGIC_H
#include "magic.h"
#endif


/* -----------------------------------------------------------------------
 * Wave mob profile type — declared here so invasion_mobs.h can use it.
 * --------------------------------------------------------------------- */
typedef struct wave_mob_profile WAVE_MOB_PROFILE;
struct wave_mob_profile
{
    const char *name;
    const char *colour;
    const char *long_descr; /* room description without colour prefix; must end \n\r */
    int         resist;
    int         suscept;
    int         cast;
    int         def;
    int         skills;
    int         block_mod;  /* flat bonus added to mob->block_mod at spawn  */
                            /* 0=none  10=light  20=medium  30=heavy armour */
};

/* 45 wave mob profiles, #define NUM_INVASION_MOBS, and invasion_mob_names[] */
#include "invasion_mobs.h"

/* -----------------------------------------------------------------------
 * Boss profile type — declared here so invasion_bosses.h can use it.
 * --------------------------------------------------------------------- */
typedef struct boss_profile BOSS_PROFILE;
struct boss_profile
{
    const char *name;
    const char *short_col;
    const char *announce_msg;
    int         resist;
    int         suscept;
    const char *spells[7];   /* up to 6 spells + NULL terminator */
    const char *emote_cast;
    const char *emote_skill;
    const char *skill_cmd;
};

/* 15 boss profiles and #define NUM_BOSSES */
#include "invasion_bosses.h"

/* -----------------------------------------------------------------------
 * Module-level state
 * --------------------------------------------------------------------- */
static bool       invasion_active        = FALSE;
static int        invasion_timer         = 0;
static CHAR_DATA *invasion_boss          = NULL;
static int        invasion_boss_profile  = -1;
static int        invasion_boss_ticks_up = 0;
static int        invasion_room_ticks    = 0;
static int        invasion_wave_respawns = 0;
static int        invasion_gertrude_explosions = 0;
static int        invasion_last_trash_talk_profile = -1;
static int        invasion_last_trash_talk_line    = -1;

/* -----------------------------------------------------------------------
 * Forward declarations
 * --------------------------------------------------------------------- */
static ROOM_INDEX_DATA *pick_boss_room    (int boss_level);
static ROOM_INDEX_DATA *pick_random_room_in_area(AREA_DATA *area);
static void       invasion_start         (void);
static void       invasion_end           (bool success);
static CHAR_DATA *spawn_invasion_mob     (int level, bool is_boss, int prof_idx,
                                          bool allow_wide_area_spawn);
static void       apply_boss_profile     (CHAR_DATA *mob, int prof_idx, int level);
static void       apply_wave_mob_profile (CHAR_DATA *mob, int wave_idx, int level);
static bool       invasion_boss_cast     (CHAR_DATA *mob);
static void       announce               (const char *msg);
static int        count_regular_players  (int *out_lo, int *out_hi);
static CHAR_DATA *find_gertrude          (void);
static bool       mob_is_invasion_mob    (CHAR_DATA *ch);
static void       despawn_all_invasion   (void);
static int        boss_spawn_count_for_tick(int boss_ticks_up);
static bool       is_midgaard_area_name(const char *area_name);
static bool       invasion_should_advance_on_room_tick(void);
static bool       invasion_should_boss_trash_talk_for_respawn_count(int respawn_count);
static bool       invasion_should_boss_trash_talk_after_respawn(void);
static bool       invasion_should_explode_at_spawn_room(int room_vnum);
static int        invasion_random_trash_talk_index(int line_count);
static bool       room_has_valid_boss_spawn_conditions(long room_flags, int path_dir);
static bool       room_is_valid_boss_spawn(ROOM_INDEX_DATA *room, ROOM_INDEX_DATA *target_room);
static int        invasion_scaled_spawn_count(int base_count, int player_count);
static bool       invasion_is_wide_area_spawn_eligible(int spawns_this_reset);
int              invasion_reward_index_for_kill(bool is_boss, int mob_level);
static void       invasion_award_kill_reward(CHAR_DATA *killer, int reward_idx);
int              invasion_gertrude_explosions_after_tick(int current_count, int had_explosion_this_tick);
const char *invasion_gertrude_quest_message_for_explosions(int explosion_count);
bool             invasion_gertrude_should_fall_for_explosions(int explosion_count);
static const char *invasion_boss_trash_talk_for_profile(int prof_idx);
static void       invasion_boss_trash_talk(void);

#ifdef UNIT_TEST_INVASION
int invasion_test_count_regular_players(int *out_lo, int *out_hi);
int invasion_test_calculate_boss_hp_mod(int level);
int invasion_test_is_invasion_mob(CHAR_DATA *ch);
int invasion_test_boss_spawn_count_for_tick(int boss_ticks_up);
int invasion_test_is_midgaard_area_name(const char *area_name);
int invasion_test_should_self_destruct_for_path_dir(int dir);
int invasion_test_should_boss_trash_talk_for_respawn_count(int respawn_count);
int invasion_test_boss_spawn_room_is_valid(long room_flags, int path_dir);
int invasion_test_scaled_spawn_count(int base_count, int player_count);
int invasion_test_is_wide_area_spawn_eligible(int spawns_this_reset);
int invasion_test_should_explode_at_spawn_room(int room_vnum);
const char *invasion_test_trash_talk_for_profile(int prof_idx);
#endif

/* -----------------------------------------------------------------------
 * announce() - send a message to every online player
 * --------------------------------------------------------------------- */
static void announce(const char *msg)
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    snprintf(buf, sizeof(buf), "@@R[INVASION] @@W%s@@N\n\r", msg);
    for (d = first_desc; d != NULL; d = d->next)
        if (d->connected == CON_PLAYING && d->character != NULL)
            send_to_char(buf, d->character);
}

/* -----------------------------------------------------------------------
 * count_regular_players()
 * --------------------------------------------------------------------- */
static int count_regular_players(int *out_lo, int *out_hi)
{
    DESCRIPTOR_DATA *d;
    int count = 0, lo = 9999, hi = 0;

    for (d = first_desc; d != NULL; d = d->next)
    {
        CHAR_DATA *ch;
        int pl;
        if (d->connected != CON_PLAYING)  continue;
        ch = d->character;
        if (ch == NULL || IS_NPC(ch))     continue;
        if (IS_IMMORTAL(ch))              continue;
        pl = get_psuedo_level(ch);
        if (pl < lo) lo = pl;
        if (pl > hi) hi = pl;
        count++;
    }
    if (count == 0) { lo = 0; hi = 0; }
    if (out_lo) *out_lo = lo;
    if (out_hi) *out_hi = hi;
    return count;
}

/* -----------------------------------------------------------------------
 * mob_is_invasion_mob()
 * --------------------------------------------------------------------- */
static bool mob_is_invasion_mob(CHAR_DATA *ch)
{
    if (!IS_NPC(ch)) return FALSE;
    return IS_SET(ch->act, ACT_INVASION) ? TRUE : FALSE;
}

int invasion_is_hidden_mobile(CHAR_DATA *ch)
{
    return mob_is_invasion_mob(ch) ? 1 : 0;
}

/* -----------------------------------------------------------------------
 * boss_spawn_count_for_tick()
 * Bosses spawn more minions as they survive longer in the world.
 * --------------------------------------------------------------------- */
static int boss_spawn_count_for_tick(int boss_ticks_up)
{
    int tiers;

    if (boss_ticks_up < 0)
        boss_ticks_up = 0;

    tiers = boss_ticks_up / 15;
    return UMIN(4, 1 + tiers);
}

static bool is_midgaard_area_name(const char *area_name)
{
    size_t i, len;
    char lower_name[MAX_STRING_LENGTH];

    if (area_name == NULL || area_name[0] == '\0')
        return FALSE;

    len = strlen(area_name);
    if (len >= sizeof(lower_name))
        len = sizeof(lower_name) - 1;

    for (i = 0; i < len; i++)
        lower_name[i] = (char)tolower((unsigned char)area_name[i]);
    lower_name[len] = '\0';

    return (strstr(lower_name, "midgaard") != NULL);
}

static bool invasion_should_advance_on_room_tick(void)
{
    invasion_room_ticks++;
    return ((invasion_room_ticks % 2) == 0);
}

static bool invasion_should_boss_trash_talk_for_respawn_count(int respawn_count)
{
    return (respawn_count > 0 && (respawn_count % 3) == 0);
}

static bool invasion_should_boss_trash_talk_after_respawn(void)
{
    invasion_wave_respawns++;
    return invasion_should_boss_trash_talk_for_respawn_count(invasion_wave_respawns);
}

static bool invasion_should_explode_at_spawn_room(int room_vnum)
{
    return (room_vnum == INVASION_SPAWN_VNUM);
}

static int invasion_random_trash_talk_index(int line_count)
{
#ifdef UNIT_TEST_INVASION
    (void)line_count;
    return 0;
#else
    return number_range(0, line_count - 1);
#endif
}

static bool room_has_valid_boss_spawn_conditions(long room_flags, int path_dir)
{
    if (IS_SET(room_flags, ROOM_SAFE))
        return FALSE;

    if (path_dir < 0)
        return FALSE;

    return TRUE;
}

static bool room_is_valid_boss_spawn(ROOM_INDEX_DATA *room, ROOM_INDEX_DATA *target_room)
{
    int path_dir;

    if (room == NULL || target_room == NULL)
        return FALSE;

    path_dir = h_find_dir(room, target_room,
                          HUNT_WORLD | HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR);

    return room_has_valid_boss_spawn_conditions(room->room_flags, path_dir);
}

static int invasion_scaled_spawn_count(int base_count, int player_count)
{
    int safe_base_count = UMAX(0, base_count);
    int safe_player_count = UMAX(1, player_count);

    return safe_base_count * safe_player_count;
}

static bool invasion_is_wide_area_spawn_eligible(int spawns_this_reset)
{
    return (spawns_this_reset >= 10);
}

int invasion_reward_index_for_kill(bool is_boss, int mob_level)
{
    if (is_boss || mob_level < 1)
        return -1;

    if (mob_level <= 100)
        return 0;
    if (mob_level <= 149)
        return 1;
    return 2;
}

static void invasion_award_kill_reward(CHAR_DATA *killer, int reward_idx)
{
    if (killer == NULL || IS_NPC(killer) || killer->pcdata == NULL)
        return;

    if (reward_idx == 0)
    {
        killer->pcdata->invasion_rewards[0]++;
        send_to_char("You acquire an invader's commendation.\n\r", killer);
        return;
    }

    if (reward_idx == 1)
    {
        killer->pcdata->invasion_rewards[1]++;
        send_to_char("You acquire an invader's ribbon.\n\r", killer);
        return;
    }

    if (reward_idx == 2)
    {
        killer->pcdata->invasion_rewards[2]++;
        send_to_char("You acquire an invader's medal.\n\r", killer);
    }
}

int invasion_gertrude_explosions_after_tick(int current_count, int had_explosion_this_tick)
{
    if (current_count < 0)
        current_count = 0;

    if (had_explosion_this_tick)
        current_count++;

    return UMIN(20, current_count);
}

const char *invasion_gertrude_quest_message_for_explosions(int explosion_count)
{
    if (explosion_count == 10)
        return "Please help defend the realm! These invaders are tearing me apart!";
    if (explosion_count == 15)
        return "I beg you, heroes, please hurry! I cannot hold them back much longer!";
    if (explosion_count == 19)
        return "Please! I cannot handle another hit! Defend the realm now!";

    return NULL;
}

bool invasion_gertrude_should_fall_for_explosions(int explosion_count)
{
    return (explosion_count >= 20) ? TRUE : FALSE;
}

static const char *invasion_boss_trash_talk_for_profile(int prof_idx)
{
    static const char *const fallback_line =
        "You fight the inevitable. Every third wave proves it.";
    static const char *const trash_talk_by_profile[NUM_BOSSES][15] =
    {
        {
            "I am Typhon, sire of monsters; your bones are my hatchery.",
            "Zeus chained me once. You will not.",
            "Every roar you hear is one of my hundred heads naming your doom.",
            "I breathe wildfire and venom; choose which death you prefer.",
            "Mountains split when I strike. What chance has mortal steel?",
            "Your heroes are kindling for my dragon-throated wrath.",
            "Kneel, and I may let one of my heads finish you swiftly.",
            "I forged the Hydra's hunger; your town will feed it in spirit.",
            "The earth cracks because it knows its true king has returned.",
            "You call this resistance? I call it a warm-up.",
            "My offspring devoured gods' champions; you are lesser prey.",
            "Venom in your veins, fire in your lungs, terror in your eyes.",
            "When I fall, I rise in legend; when you fall, you are forgotten.",
            "Run to your temples. Even stone altars melt in my breath.",
            "I am the storm beneath the mountain, and you are standing on it."
        },
        {
            "The World-Serpent's blood remembers you, little shore-crawlers.",
            "I am the tide before Ragnarok. You are driftwood.",
            "Your walls are sandcastles at high tide.",
            "Breathe deep; that is my venom replacing your courage.",
            "Midgard will drown while I coil and smile.",
            "Each wave that rises carries another of your names to Hel.",
            "Throw fire if you like; the sea will swallow your sparks.",
            "I crush ships in my sleep. You are smaller than splinters.",
            "The deep has no mercy, and neither do I.",
            "Your champions thrash like fish on a deck.",
            "Hear the surf? It chants your funeral rites.",
            "I was spawned for endings, and yours begins now.",
            "Knees buckle quickly when the ocean enters your lungs.",
            "The serpent below Midgard hungers, and I am its jaw.",
            "Pray to thunder-gods. Their hammer cannot find me beneath the waves."
        },
        {
            "Hope is a lantern; I am the hand that snuffs it.",
            "Your minds are loud. I will silence them one scream at a time.",
            "I was old when your gods were still whispers.",
            "Watch your allies closely; paranoia is my favorite blade.",
            "I do not kill bodies first. I kill certainty.",
            "Kneel in the dark and perhaps I pass over your thoughts.",
            "I have eaten brighter souls than yours for less offense.",
            "Your prayers echo beautifully in the void before they die.",
            "Light cannot save you when the shadow is inside your skull.",
            "Your nightmares report to me.",
            "Steel rings, spells flare, and still your fear smells strongest.",
            "Every heartbeat is another crack in your sanity.",
            "I am not your end. I am your unmaking.",
            "Close your eyes; it changes nothing.",
            "You call me pale because terror drained your own color first."
        },
        {
            "I crawled from the Abyss to teach your world despair.",
            "Stone breaks beneath me as easily as men do.",
            "Your courage is a candle in a demon wind.",
            "I wear night like armor and hate like a crown.",
            "Each crack in the ground is my signature.",
            "You stand on borrowed earth. I am here to reclaim it.",
            "Angels turned their faces when I rose. Wise of them.",
            "My laughter shakes foundations because your gods built them poorly.",
            "Come closer. The Abyss has room for all of you.",
            "I have broken paladins better than your strongest.",
            "Your city is a toy fort to my talons.",
            "Fear me now, or fear me later from beneath the rubble.",
            "I am the pit staring back.",
            "Every wound you deal is just a new mouth for me to grin through.",
            "When I am done, even your ruins will beg for silence."
        },
        {
            "Inferno does not bargain. It consumes.",
            "Your armor glows red because my legion is near.",
            "I march from Hell with drums made of broken vows.",
            "Each spark in this sky carries a contract for your soul.",
            "You smell brimstone because destiny just opened.",
            "My hounds have your scent, and they are very patient.",
            "I have burned saints and sinners; both screamed the same.",
            "Mercy was left at the gate of Dis.",
            "Raise shields if you wish; flame climbs over pride.",
            "The pit sings tonight, and your names are in the chorus.",
            "I was forged in punishment. You are merely practice.",
            "The damned cheer each time one of you falls.",
            "I will brand this battlefield with your failures.",
            "Your water hisses; my fire laughs.",
            "When dawn comes, it will rise over cinders and regret."
        },
        {
            "I am Surtr's edge at the end of ages.",
            "The horizon reddens because I have begun to smile.",
            "You cannot quench what was born to end the world.",
            "My blade wrote prophecies in fire long before you were born.",
            "Stand firm; it only helps the flames catch faster.",
            "Even gods feared this fire at Ragnarok.",
            "Your banners will be torches by sunset.",
            "I carve runes of ruin in burning air.",
            "Try water. It only makes the steam scream louder.",
            "Each step I take is a funeral pyre waiting for a body.",
            "I do not chase prey; the fire drives it to me.",
            "Your healers mend flesh, not ash.",
            "There is no shade in Muspelheim, and none here tonight.",
            "My sword is sunrise for the dead.",
            "When this blaze fades, your names will fade with it."
        },
        {
            "I am the maw beneath the waves and I am still hungry.",
            "Ships, crews, heroes—everything spins the same in my throat.",
            "Feel the pull? That is hopelessness gaining current.",
            "Anchor yourselves if you like. I drag continents in my dreams.",
            "Wind obeys me, and water kneels.",
            "Your battle line is already a whirlpool.",
            "I swallow prayers and belch storms.",
            "Do not scream; the sea mutes all confessions.",
            "Every splash is another promise you cannot keep.",
            "I have drowned kings in calmer waters than these.",
            "Breathe now while you still own the air.",
            "Your boots are heavy. Good. Sink faster.",
            "The deep keeps what it takes.",
            "Tides turn. So do fortunes. Yours just did.",
            "I am Charybdis—escape is a myth sailors tell children."
        },
        {
            "I gnawed Yggdrasil's roots; your walls are softer.",
            "Corpse-venom is my wine, and you are tonight's feast.",
            "Nastrond taught me patience and cruelty in equal measure.",
            "Your flesh will rot before it cools.",
            "I have chewed through oaths, bones, and worlds.",
            "Holy light stings, but your fear tastes sweeter.",
            "Hear that scraping? That is my hunger sharpening itself.",
            "I drag the stink of the underworld wherever I crawl.",
            "The dead envy the living until I arrive.",
            "You clutch talismans; I clutch throats.",
            "I do not hoard gold. I hoard endings.",
            "Your champions will decorate Hel's shore by dawn.",
            "One bite for marrow, one for memory.",
            "Rot is inevitable. I simply hurry it.",
            "Yggdrasil endured me. You will not."
        },
        {
            "Hear my song and ride with me into your own grave.",
            "The Wild Hunt never misses debtors.",
            "Your heartbeat is the hoofbeat beneath my stirrup.",
            "I take children from cradles and kings from thrones alike.",
            "The night wind carries your surrender already.",
            "Do not look into my eyes unless you wish to follow forever.",
            "Iron charms tremble when my reins snap.",
            "I promised your souls to the dark wood before I arrived.",
            "My hounds know your scent better than your lovers do.",
            "Run through the forest; every branch bends toward me.",
            "You are not hunted for sport. You are hunted for silence.",
            "Moonlight is just my lantern on this ride.",
            "Your bravest rider would not survive my first gallop.",
            "Kiss your world goodbye; the Hunt has sounded.",
            "I am the Erlking, and every path tonight ends at my saddle."
        },
        {
            "The scales of Ma'at tip, and you are found wanting.",
            "Bring me your hearts; I judge by hunger.",
            "Lion's wrath, crocodile's jaws, hippo's strength—your verdict is death.",
            "I devour the unworthy, and I am never full.",
            "You cannot argue with a beast built from sentence and punishment.",
            "Your sins weigh more than your steel.",
            "The hall of judgment sent me your names in advance.",
            "Pray for mercy; I am not authorized to grant it.",
            "Truth is heavy. Your hearts are heavier.",
            "I hear your pulse fluttering like a guilty prayer.",
            "Even brave souls taste of fear when the scales tilt.",
            "I am the final mouth after final judgment.",
            "The righteous pass by me. You do not.",
            "Struggle if you must; it seasons the heart.",
            "Sentence delivered: consumed."
        },
        {
            "Order is a fragile sun. I am the shadow across it.",
            "I coil around dawn and squeeze until daylight screams.",
            "Ra burns above, but chaos rises below.",
            "Your formations amuse me; entropy has no formation.",
            "Each command you shout dissolves into panic.",
            "I have swallowed certainties older than your temples.",
            "Poison is honest—it tells you exactly how quickly you lose.",
            "The Duat opens and your courage leaks into it.",
            "You cannot kill chaos. You can only delay your fear.",
            "Look skyward. Even the sun blinks when I move.",
            "Your laws, your vows, your walls—all kindling for disorder.",
            "I am the knot in creation's throat.",
            "When I hiss, empires forget their own names.",
            "Stand in line if you want; I break lines for sport.",
            "I am Apep. Night itself marches under my scales."
        },
        {
            "Bronze does not panic. Bronze advances.",
            "I was forged by a god-smith; you were not.",
            "Hear my stride—Crete remembers this sound as judgment.",
            "Your blades ring against me like chimes in a storm.",
            "Molten ichor is the only blood I need.",
            "I patrol, I punish, I persist.",
            "Rust is your prayer; steel is mine.",
            "You are flesh confronting engineering.",
            "I circled islands in a day. I can circle your graves all night.",
            "Every footfall is a siege engine.",
            "Your cleverness fails against inevitability and mass.",
            "I do not tire, hesitate, or forgive.",
            "The bronze giant has no fear to exploit.",
            "You are dents waiting to happen.",
            "When I fall, I rise heated and angrier."
        },
        {
            "Sleep no more. Your nightmares have a face now.",
            "I drink screams and exhale prophecy.",
            "The moon dimmed so you could see me better.",
            "I was stitched from fear and fed on omens.",
            "Your minds are open doors in a storm.",
            "Do not blink; horrors breed in that instant.",
            "I crawl between thoughts where prayers cannot follow.",
            "Each shadow behind you belongs to me.",
            "I break heroes by showing them tomorrow.",
            "Courage cracks fastest in the dark.",
            "Your pulse keeps time for my feast.",
            "I am the reason children fear sleeping alone.",
            "Call me nightmare, call me doom—the result is unchanged.",
            "Even your victories taste like dread when I am near.",
            "Tonight, terror graduates from rumor to memory."
        },
        {
            "I am Fenrir's hunger and the dark between stars.",
            "Your moon is only a lantern for my feast.",
            "I bite through fate, not just flesh.",
            "The pack has no pity for trembling prey.",
            "I hunted gods in dreams before I hunted you awake.",
            "Hear the drums? That is extinction finding rhythm.",
            "Your bravest howl still sounds like fear to me.",
            "Run—predators enjoy a chase.",
            "I wear midnight like fur and wrath like teeth.",
            "Blood on snow is a poem I write often.",
            "I am the first shadow at dusk and the last at dawn.",
            "Your silver shakes. Good. So should you.",
            "The feast begins with your champions.",
            "I do not stalk. I arrive.",
            "When I am done, even the ravens will whisper your names."
        },
        {
            "I am no dragon, no god—only the void that survives both.",
            "Stars die quietly. You will not.",
            "Fate is thread; I am the hand that unravels it.",
            "Your reality frays each time I breathe.",
            "The cosmos forgot me on purpose. I returned anyway.",
            "I speak in silences between screams.",
            "You clutch meaning. I deal in endings.",
            "Even time slows to watch me erase you.",
            "Your names are sparks in a windless dark.",
            "I watched civilizations bloom and vanish like frost.",
            "Steel, magic, prayer—noise before oblivion.",
            "There is no sanctuary at the edge of existence.",
            "I do not conquer worlds. I outlast them.",
            "Be grateful: oblivion is impartial.",
            "Witness the void's rhythm, and count your remaining heartbeats."
        }
    };
    int line_idx;

    if (prof_idx < 0 || prof_idx >= NUM_BOSSES)
        return fallback_line;

    line_idx = invasion_random_trash_talk_index(15);

    if (invasion_last_trash_talk_profile == prof_idx
        && invasion_last_trash_talk_line == line_idx)
        line_idx = (line_idx + 1) % 15;

    invasion_last_trash_talk_profile = prof_idx;
    invasion_last_trash_talk_line = line_idx;
    return trash_talk_by_profile[prof_idx][line_idx];
}

static void invasion_boss_trash_talk(void)
{
    if (invasion_boss == NULL || invasion_boss->is_free)
        return;

    do_quest2(invasion_boss, (char *)invasion_boss_trash_talk_for_profile(invasion_boss_profile));
}

#ifdef UNIT_TEST_INVASION
int invasion_test_count_regular_players(int *out_lo, int *out_hi)
{
    return count_regular_players(out_lo, out_hi);
}

int invasion_test_calculate_boss_hp_mod(int level)
{
    long hp_bonus = 100L + ((long)level * (long)level * 15L);
    return (int)URANGE(100L, hp_bonus, 250000L);
}

int invasion_test_is_invasion_mob(CHAR_DATA *ch)
{
    return mob_is_invasion_mob(ch);
}

int invasion_test_boss_spawn_count_for_tick(int boss_ticks_up)
{
    return boss_spawn_count_for_tick(boss_ticks_up);
}

int invasion_test_is_midgaard_area_name(const char *area_name)
{
    return is_midgaard_area_name(area_name) ? 1 : 0;
}

int invasion_test_should_self_destruct_for_path_dir(int dir)
{
    return (dir < 0) ? 1 : 0;
}

int invasion_test_should_boss_trash_talk_for_respawn_count(int respawn_count)
{
    return invasion_should_boss_trash_talk_for_respawn_count(respawn_count) ? 1 : 0;
}

int invasion_test_boss_spawn_room_is_valid(long room_flags, int path_dir)
{
    return room_has_valid_boss_spawn_conditions(room_flags, path_dir) ? 1 : 0;
}

int invasion_test_scaled_spawn_count(int base_count, int player_count)
{
    return invasion_scaled_spawn_count(base_count, player_count);
}

int invasion_test_is_wide_area_spawn_eligible(int spawns_this_reset)
{
    return invasion_is_wide_area_spawn_eligible(spawns_this_reset) ? 1 : 0;
}

int invasion_test_should_explode_at_spawn_room(int room_vnum)
{
    return invasion_should_explode_at_spawn_room(room_vnum) ? 1 : 0;
}

const char *invasion_test_trash_talk_for_profile(int prof_idx)
{
    return invasion_boss_trash_talk_for_profile(prof_idx);
}

#endif

/* -----------------------------------------------------------------------
 * find_gertrude()
 * --------------------------------------------------------------------- */
static CHAR_DATA *find_gertrude(void)
{
    ROOM_INDEX_DATA *room = get_room_index(INVASION_SPAWN_VNUM);
    CHAR_DATA *ch;
    if (!room) return NULL;
    for (ch = room->first_person; ch != NULL; ch = ch->next_in_room)
        if (IS_NPC(ch) && is_name("gertrude", ch->name))
            return ch;
    return NULL;
}

/* -----------------------------------------------------------------------
 * despawn_all_invasion()
 * Extracts every invasion-tagged character (wave mobs and any living boss).
 * --------------------------------------------------------------------- */
static void despawn_all_invasion(void)
{
    CHAR_DATA *ch, *ch_next;

    for (ch = first_char; ch != NULL; ch = ch_next)
    {
        ch_next = ch->next;
        if (!mob_is_invasion_mob(ch)) continue;
        if (is_fighting(ch))
            stop_fighting(ch, TRUE);
        REMOVE_BIT(ch->act, ACT_INVASION);
        extract_char(ch, TRUE);
    }
    invasion_boss = NULL;
}

/* -----------------------------------------------------------------------
 * apply_boss_profile()
 *
 * hp_mod scale (clamped to [100, 250000]):
 *   hp_mod = 100 + (level^2 * 15)
 *   level   1  ->       115
 *   level  30  ->    13,600
 *   level  60  ->    54,100
 *   level 100  ->   150,100
 *   level 129  ->   249,715  (capped at 250,000)
 * --------------------------------------------------------------------- */
static void apply_boss_profile(CHAR_DATA *mob, int prof_idx, int level)
{
    const BOSS_PROFILE *bp;
    long hp_bonus;

    if (prof_idx < 0 || prof_idx >= NUM_BOSSES) return;
    bp = &boss_profiles[prof_idx];

    /* Elemental profile */
    mob->resist  = bp->resist;
    mob->suscept = bp->suscept;

    /* Scaled hp_mod */
    hp_bonus  = 100L + ((long)level * (long)level * 15L);
    hp_bonus  = URANGE(100L, hp_bonus, 250000L);
    mob->hp_mod = (int)hp_bonus;

    /* Recalculate max_hit including hp_mod */
    mob->max_hit = mob->level * 20
                 + number_range(mob->level * 5, mob->level * 10)
                 + mob->hp_mod;
    mob->hit = mob->max_hit;

    /* Combat stat bonuses scale with level */
    mob->dr_mod += level / 4;
    mob->hr_mod += level / 4;
    mob->ac_mod -= level / 3;

    /* Multi-attack passes */
    SET_BIT(mob->skills, MOB_SECOND);
    SET_BIT(mob->skills, MOB_THIRD);
    SET_BIT(mob->skills, MOB_FOURTH);
    SET_BIT(mob->skills, MOB_ENHANCED);
    if (level >= 50) SET_BIT(mob->skills, MOB_FIFTH);
    if (level >= 80) SET_BIT(mob->skills, MOB_SIXTH);

    /* Defensive skills */
    SET_BIT(mob->skills, MOB_DODGE);
    SET_BIT(mob->skills, MOB_PARRY);
    SET_BIT(mob->skills, MOB_NODISARM);
    SET_BIT(mob->skills, MOB_NOTRIP);

    /* Solo flag so damage caps apply correctly */
    SET_BIT(mob->act, ACT_SOLO);
}

/* -----------------------------------------------------------------------
 * apply_wave_mob_profile()
 *
 * Applies the per-creature profile from invasion_mobs.h then enforces a
 * mandatory floor so every wave mob is genuinely threatening:
 *
 *   ATTACK FLOOR  : MOB_SECOND + MOB_THIRD always set (3 attacks minimum).
 *                   Profiles add MOB_FOURTH / FIFTH / SIXTH on top.
 *   DEFENCE FLOOR : MOB_PARRY + MOB_DODGE + MOB_NODISARM always set.
 *   BLOCK         : profile's block_mod is added to mob->block_mod.
 *
 * HP is kept modest (wave mobs should die, but put up a fight):
 *   max_hit = level*20 + rand(level*3, level*6) + level*5
 * --------------------------------------------------------------------- */
static void apply_wave_mob_profile(CHAR_DATA *mob, int wave_idx, int level)
{
    const WAVE_MOB_PROFILE *wp;

    if (wave_idx < 0 || wave_idx >= NUM_INVASION_MOBS) wave_idx = 0;
    wp = &invasion_wave_profiles[wave_idx];

    /* Elemental profile */
    mob->resist  = wp->resist;
    mob->suscept = wp->suscept;

    /* Spell and defensive cast bits (AI uses these automatically) */
    mob->cast = wp->cast;
    mob->def  = wp->def;

    /* Physical skill bits from the profile */
    mob->skills = wp->skills;

    /* ---- Mandatory floors --------------------------------------------- */
    SET_BIT(mob->skills, MOB_SECOND);   /* minimum 3 attacks              */
    SET_BIT(mob->skills, MOB_THIRD);
    SET_BIT(mob->skills, MOB_PARRY);    /* every wave mob can parry        */
    SET_BIT(mob->skills, MOB_DODGE);    /* every wave mob can dodge        */
    SET_BIT(mob->skills, MOB_NODISARM); /* natural weapons can't be taken  */

    /* Natural armour / tough hide */
    mob->block_mod += wp->block_mod;

    /* HP */
    mob->hp_mod  = level * 5;
    mob->max_hit = level * 20
                 + number_range(level * 3, level * 6)
                 + mob->hp_mod;
    mob->hit = mob->max_hit;

    /* Modest combat stat scaling with level */
    mob->dr_mod += level / 8;
    mob->hr_mod += level / 8;
}

/* -----------------------------------------------------------------------
 * invasion_boss_cast()
 * Picks a random spell from the boss profile and casts it at its target.
 * Called once per PULSE_TICK while the boss is fighting.
 * Returns TRUE if a spell fired.
 * --------------------------------------------------------------------- */
static bool invasion_boss_cast(CHAR_DATA *mob)
{
    const BOSS_PROFILE *bp;
    CHAR_DATA *victim;
    int sn, spell_count, idx;

    if (invasion_boss_profile < 0)               return FALSE;
    bp = &boss_profiles[invasion_boss_profile];

    if (mob->position != POS_FIGHTING)           return FALSE;
    if (number_percent() < 40)                   return FALSE;  /* 60% chance */

    victim = mob->fighting;
    if (victim == NULL || IS_NPC(victim))        return FALSE;

    for (spell_count = 0; bp->spells[spell_count] != NULL; spell_count++)
        ;
    if (spell_count == 0)                        return FALSE;

    idx = number_range(0, spell_count - 1);
    sn  = skill_lookup((char *)bp->spells[idx]);
    if (sn < 0)                                  return FALSE;

    act(bp->emote_cast, mob, NULL, victim, TO_ROOM);
    (*skill_table[sn].spell_fun)(sn, mob->level, mob, victim, NULL);
    return TRUE;
}

/* -----------------------------------------------------------------------
 * pick_boss_room()
 * Finds a random room inside a level-appropriate area for the boss to
 * spawn in.  An area qualifies if its min_level..max_level range overlaps
 * [boss_level - INVASION_BOSS_AREA_SLACK, boss_level + INVASION_BOSS_AREA_SLACK].
 *
 * Additional constraint: the chosen room must be able to path to Gertrude's
 * room (INVASION_SPAWN_VNUM / room 3001) via h_find_dir.  Rooms that have
 * no valid path to that room are skipped.
 *
 * Algorithm (reservoir sampling, single pass):
 *   Walk every qualifying area's room list, skip rooms that cannot reach
 *   the target, then pick one qualifying room at random so that each
 *   reachable room has equal probability of being chosen.
 *
 * Falls back to INVASION_START_VNUM if no suitable area/room is found.
 * --------------------------------------------------------------------- */
static ROOM_INDEX_DATA *pick_boss_room(int boss_level)
{
    AREA_DATA        *pArea;
    BUILD_DATA_LIST  *pList;
    ROOM_INDEX_DATA  *chosen      = NULL;
    ROOM_INDEX_DATA  *target_room = NULL;
    int               n           = 0;   /* total qualifying rooms seen so far */
    int               lo          = boss_level - INVASION_BOSS_AREA_SLACK;
    int               hi          = boss_level + INVASION_BOSS_AREA_SLACK;

    /* Grab Gertrude's room once up front. */
    target_room = get_room_index(INVASION_SPAWN_VNUM);   /* vnum 3001 */
    if (target_room == NULL)
    {
        bug("pick_boss_room: target room vnum %d not found.", INVASION_SPAWN_VNUM);
        return get_room_index(INVASION_START_VNUM);
    }

    for (pArea = first_area; pArea != NULL; pArea = pArea->next)
    {
        /* Skip areas with no level data set (both zero means "unset"). */
        if (pArea->min_level == 0 && pArea->max_level == 0)
            continue;

        /* Area level range must overlap the boss level window. */
        if (pArea->max_level < lo || pArea->min_level > hi)
            continue;

        /* Reservoir-sample every room in this qualifying area. */
        for (pList = pArea->first_area_room; pList != NULL; pList = pList->next)
        {
            ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *)pList->data;
            if (room == NULL) continue;

            /*
             * Reachability check: verify there is at least one valid exit
             * direction from this room toward the target (room 3001).
             * h_find_dir returns >= 0 when a path exists, -1 when it does not.
             */
            if (!room_is_valid_boss_spawn(room, target_room))
                continue;   /* safe or no path to Gertrude – skip this room */

            n++;
            /* Replace chosen with probability 1/n (uniform distribution). */
            if (number_range(1, n) == 1)
                chosen = room;
        }
    }

    if (chosen != NULL)
        return chosen;

    /* Fallback: original fixed spawn room. */
    bug("pick_boss_room: no reachable level-appropriate room for boss level %d, "
        "falling back to INVASION_START_VNUM.", boss_level);
    return get_room_index(INVASION_START_VNUM);
}

static ROOM_INDEX_DATA *pick_random_room_in_area(AREA_DATA *area)
{
    BUILD_DATA_LIST *pList;
    ROOM_INDEX_DATA *chosen = NULL;
    int n = 0;

    if (area == NULL)
        return NULL;

    for (pList = area->first_area_room; pList != NULL; pList = pList->next)
    {
        ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *)pList->data;
        if (room == NULL) continue;

        n++;
        if (number_range(1, n) == 1)
            chosen = room;
    }

    return chosen;
}

/* -----------------------------------------------------------------------
 * spawn_invasion_mob()
 * Creates a clone of the template mob and configures it as an invasion mob.
 * prof_idx is only used when is_boss is TRUE.
 *
 * Bosses   : placed in a random room inside a level-appropriate area.
 * Wave mobs: placed in INVASION_START_VNUM as before.
 * All mobs : flagged ACT_UNDEAD, ACT_SENTINEL, ACT_NOASSIST.
 * --------------------------------------------------------------------- */
static CHAR_DATA *spawn_invasion_mob(int level, bool is_boss, int prof_idx,
                                     bool allow_wide_area_spawn)
{
    ROOM_INDEX_DATA *spawn_room;
    MOB_INDEX_DATA  *pIndex;
    CHAR_DATA       *mob;
    const char      *creature_name;
    char             name_buf[MAX_STRING_LENGTH];
    char             short_buf[MAX_STRING_LENGTH];
    char             long_buf[MAX_STRING_LENGTH];

#define INVASION_TEMPLATE_MOB  3    /* <-- set to a real mob vnum in your world */

    if ((pIndex = get_mob_index(INVASION_TEMPLATE_MOB)) == NULL)
    {
        bug("spawn_invasion_mob: template mob vnum %d not found.", INVASION_TEMPLATE_MOB);
        return NULL;
    }

    /* Bosses and overflow waves use level-appropriate area picks; regular waves use the boss area. */
    if (is_boss)
    {
        spawn_room = pick_boss_room(level);
    }
    else if (allow_wide_area_spawn)
    {
        spawn_room = pick_boss_room(level);
    }
    else if (invasion_boss != NULL && !invasion_boss->is_free
             && invasion_boss->in_room != NULL && invasion_boss->in_room->area != NULL)
    {
        spawn_room = pick_random_room_in_area(invasion_boss->in_room->area);
    }
    else
    {
        spawn_room = get_room_index(INVASION_START_VNUM);
    }

    if (spawn_room == NULL)
    {
        bug("spawn_invasion_mob: no valid spawn room found.", 0);
        return NULL;
    }

    int wave_idx = 0;
    if (is_boss)
    {
        creature_name = boss_profiles[prof_idx].name;
    }
    else
    {
        wave_idx      = number_range(0, NUM_INVASION_MOBS - 1);
        creature_name = invasion_wave_profiles[wave_idx].name;
    }

    mob = create_mobile(pIndex);
    if (mob == NULL) return NULL;

    if (mob->name)        free_string(mob->name);
    if (mob->short_descr) free_string(mob->short_descr);
    if (mob->long_descr)  free_string(mob->long_descr);

    if (is_boss)
    {
        const BOSS_PROFILE *bp = &boss_profiles[prof_idx];
        snprintf(name_buf,  sizeof(name_buf),  "%s invasion boss", creature_name);
        snprintf(short_buf, sizeof(short_buf), "%s%s@@N",           bp->short_col, creature_name);
        snprintf(long_buf,  sizeof(long_buf),
                 "%s%s@@N stands here, radiating a terrifying aura of power.\n\r",
                 bp->short_col, creature_name);
    }
    else
    {
        const WAVE_MOB_PROFILE *wp = &invasion_wave_profiles[wave_idx];
        snprintf(name_buf,  sizeof(name_buf),  "%s invader",      creature_name);
        snprintf(short_buf, sizeof(short_buf), "%s%s invader@@N", wp->colour, creature_name);
        snprintf(long_buf,  sizeof(long_buf),  "%s%s@@N\n\r",     wp->colour, wp->long_descr);
    }

    mob->name        = str_dup(name_buf);
    mob->short_descr = str_dup(short_buf);
    mob->long_descr  = str_dup(long_buf);

    mob->level = URANGE(1, level, MAX_LEVEL);

    REMOVE_BIT(mob->act, ACT_AGGRESSIVE);
    SET_BIT(mob->act, ACT_SENTINEL);
    SET_BIT(mob->act, ACT_NOASSIST);
    SET_BIT(mob->act, ACT_UNDEAD);   /* all invasion mobs are undead */
    SET_BIT(mob->act, ACT_NO_HUNT);  /* cannot be hunted */

    if (is_boss)
        apply_boss_profile(mob, prof_idx, level);
    else
        apply_wave_mob_profile(mob, wave_idx, level);

    SET_BIT(mob->act, ACT_INVASION);

    char_to_room(mob, spawn_room);
    return mob;
}

/* -----------------------------------------------------------------------
 * invasion_start()
 * --------------------------------------------------------------------- */
static void invasion_start(void)
{
    int lo = 1, hi = 1, player_count, boss_level, prof;
    char buf[MAX_STRING_LENGTH];

    player_count = count_regular_players(&lo, &hi);
    if (player_count < 1) return;

    prof       = number_range(0, NUM_BOSSES - 1);
    boss_level = hi + INVASION_BOSS_OFFSET;

    invasion_boss_profile = prof;
    invasion_active       = TRUE;
    invasion_boss_ticks_up = 0;
    invasion_room_ticks    = 0;
    invasion_wave_respawns = 0;
    invasion_gertrude_explosions = 0;
    invasion_last_trash_talk_profile = -1;
    invasion_last_trash_talk_line = -1;

    invasion_boss = spawn_invasion_mob(boss_level, TRUE, prof, FALSE);
    if (invasion_boss == NULL)
    {
        bug("invasion_start: failed to spawn boss.", 0);
        invasion_active       = FALSE;
        invasion_boss_profile = -1;
        return;
    }

    snprintf(buf, sizeof(buf), "%s", boss_profiles[prof].announce_msg);
    announce(buf);
}

/* -----------------------------------------------------------------------
 * invasion_end()
 * --------------------------------------------------------------------- */
static void invasion_end(bool success)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch;
    char buf[MAX_STRING_LENGTH];

    if (success)
    {
        announce("@@GThe invasion has been repelled!  The heroes of the realm "
                 "have saved Gertrude and the town!  "
                 "Everyone receives @@Y25 quest points@@G and @@Y1 invasion point@@G!@@N");

        for (d = first_desc; d != NULL; d = d->next)
        {
            if (d->connected != CON_PLAYING) continue;
            ch = d->character;
            if (ch == NULL || IS_NPC(ch) || IS_IMMORTAL(ch)) continue;
            ch->quest_points += INVASION_QP_REWARD;
            ch->pcdata->invasion_points += 1;
            snprintf(buf, sizeof(buf),
                     "You receive @@Y%d quest points@@N and @@Y1 invasion point@@N for defending the realm!\n\r",
                     INVASION_QP_REWARD);
            send_to_char(buf, ch);
            do_save(ch, "");
        }
    }
    else
    {
        announce("@@RGertrude has fallen!  The invasion has succeeded — "
                 "the realm mourns the loss of its beloved innkeeper.@@N");
    }

    despawn_all_invasion();

    invasion_boss_profile = -1;
    invasion_active       = FALSE;
    invasion_boss_ticks_up = 0;
    invasion_room_ticks    = 0;
    invasion_wave_respawns = 0;
    invasion_gertrude_explosions = 0;
    invasion_last_trash_talk_profile = -1;
    invasion_last_trash_talk_line = -1;
    invasion_timer        = INVASION_MIN_INTERVAL;
}

/* -----------------------------------------------------------------------
 * invasion_update() — called once per PULSE_TICK from update_handler()
 * --------------------------------------------------------------------- */
void invasion_update(void)
{
    int lo = 1, hi = 1, count, lo_lvl, hi_lvl, i;
    int spawns_this_reset = 0;

    if (invasion_timer > 0) { invasion_timer--; return; }

    if (!invasion_active)
    {
        count = count_regular_players(&lo, &hi);
        if (count < 1)            return;
        if (number_percent() > 5) return;
        invasion_start();
        return;
    }

    count = count_regular_players(&lo, &hi);

    if (count < 1)
    {
        despawn_all_invasion();
        invasion_boss_profile = -1;
        invasion_active       = FALSE;
        invasion_room_ticks   = 0;
        invasion_wave_respawns = 0;
        invasion_gertrude_explosions = 0;
        invasion_last_trash_talk_profile = -1;
        invasion_last_trash_talk_line = -1;
        invasion_timer        = INVASION_MIN_INTERVAL;
        return;
    }

    if (invasion_boss != NULL && invasion_boss->is_free)
    {
        invasion_boss = NULL;
        invasion_end(TRUE);
        return;
    }

    if (invasion_boss != NULL && !invasion_boss->is_free)
    {
        int boss_spawn_count;
        int scaled_boss_spawn_count;

        invasion_boss_ticks_up++;
        boss_spawn_count = boss_spawn_count_for_tick(invasion_boss_ticks_up);
        scaled_boss_spawn_count = invasion_scaled_spawn_count(boss_spawn_count, count);

        for (i = 0; i < scaled_boss_spawn_count; i++)
        {
            bool allow_wide_area_spawn = invasion_is_wide_area_spawn_eligible(spawns_this_reset);

            if (spawn_invasion_mob(number_range(lo, hi), FALSE, -1,
                                   allow_wide_area_spawn) != NULL)
            {
                spawns_this_reset++;
                if (invasion_should_boss_trash_talk_after_respawn())
                    invasion_boss_trash_talk();
            }
        }
    }

    if (invasion_boss != NULL && !invasion_boss->is_free
        && invasion_boss->fighting != NULL)
    {
        invasion_boss_cast(invasion_boss);
    }

    lo_lvl = URANGE(1,      lo + INVASION_MOB_MIN_OFFSET, MAX_LEVEL);
    hi_lvl = URANGE(lo_lvl, hi + INVASION_MOB_MAX_OFFSET, MAX_LEVEL);

    for (i = 0; i < invasion_scaled_spawn_count(number_range(0, 2), count); i++)
    {
        bool allow_wide_area_spawn = invasion_is_wide_area_spawn_eligible(spawns_this_reset);

        if (spawn_invasion_mob(number_range(lo_lvl, hi_lvl), FALSE, -1,
                               allow_wide_area_spawn) != NULL)
        {
            spawns_this_reset++;
            if (invasion_should_boss_trash_talk_after_respawn())
                invasion_boss_trash_talk();
        }
    }
}

/* -----------------------------------------------------------------------
 * invasion_rooms_update() — called once per PULSE_ROOMS
 * --------------------------------------------------------------------- */
void invasion_rooms_update(void)
{
    CHAR_DATA       *ch, *ch_next;
    ROOM_INDEX_DATA *target_room;
    CHAR_DATA       *gertrude;
    sh_int           dir;
    bool             had_explosion_this_tick = FALSE;

    if (!invasion_active) return;
    if (!invasion_should_advance_on_room_tick()) return;

    target_room = get_room_index(INVASION_SPAWN_VNUM);
    if (!target_room) return;

    gertrude = find_gertrude();

    for (ch = first_char; ch != NULL; ch = ch_next)
    {
        ch_next = ch->next;

        if (!mob_is_invasion_mob(ch))    continue;
        if (ch == invasion_boss)         continue;  /* boss does not march */
        if (ch->in_room == NULL)         continue;
        if (invasion_should_explode_at_spawn_room(ch->in_room->vnum))
        {
            had_explosion_this_tick = TRUE;

            if (gertrude != NULL)
            {
                act("$n reaches Gertrude, explodes in a violent burst, and crumbles away!",
                    ch, NULL, gertrude, TO_ROOM);
            }

            REMOVE_BIT(ch->act, ACT_INVASION);
            extract_char(ch, TRUE);
            continue;
        }

        if (is_fighting(ch))             continue;
        if (ch->position < POS_STANDING) continue;

        dir = h_find_dir(ch->in_room, target_room,
                         HUNT_WORLD | HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR);
        if (dir < 0)
        {
            act("$n cannot find a path to Gertrude and crumbles into dust.",
                ch, NULL, NULL, TO_ROOM);
            REMOVE_BIT(ch->act, ACT_INVASION);
            extract_char(ch, TRUE);
            continue;
        }

        /*
         * Use hunt_move so invasion mobs can open/unlock/pick doors along
         * their route to Gertrude, matching the h_find_dir flags above.
         */
        hunt_move(ch, dir);
    }

    if (had_explosion_this_tick)
    {
        const char *quest_msg;

        invasion_gertrude_explosions = invasion_gertrude_explosions_after_tick(
            invasion_gertrude_explosions, 1);

        quest_msg = invasion_gertrude_quest_message_for_explosions(invasion_gertrude_explosions);
        if (quest_msg != NULL && gertrude != NULL && !gertrude->is_free)
            do_quest2(gertrude, (char *)quest_msg);

        if (invasion_gertrude_should_fall_for_explosions(invasion_gertrude_explosions)
            && gertrude != NULL && !gertrude->is_free)
            raw_kill(gertrude, "");
    }
}

/* -----------------------------------------------------------------------
 * invasion_on_death() — hook called just BEFORE extract_char in raw_kill()
 * --------------------------------------------------------------------- */
void invasion_on_death(CHAR_DATA *ch, CHAR_DATA *killer)
{
    if (!invasion_active) return;

    if (ch == invasion_boss)
    {
        REMOVE_BIT(ch->act, ACT_INVASION);
        invasion_boss = NULL;
        invasion_end(TRUE);
        return;
    }

    if (IS_NPC(ch) && is_name("gertrude", ch->name))
    {
        /* Clear the boss's invasion tag before despawn_all_invasion()
         * walks the char list, so the boss extracts cleanly like any
         * other mob rather than being left in a half-extracted state. */
        if (invasion_boss != NULL && !invasion_boss->is_free)
            REMOVE_BIT(invasion_boss->act, ACT_INVASION);
        invasion_boss = NULL;
        invasion_end(FALSE);
        return;
    }

    if (mob_is_invasion_mob(ch))
    {
        int reward_idx = invasion_reward_index_for_kill(FALSE, ch->level);

        invasion_award_kill_reward(killer, reward_idx);
        REMOVE_BIT(ch->act, ACT_INVASION);
    }
}

/* -----------------------------------------------------------------------
 * do_invasion() — immortal command
 * Usage: invasion <start | stop | status | bosses>
 * --------------------------------------------------------------------- */
void do_invasion(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int  i;

    one_argument(argument, arg);

    if (!IS_IMMORTAL(ch))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "start"))
    {
        if (invasion_active)
        { send_to_char("An invasion is already underway.\n\r", ch); return; }
        invasion_timer = 0;
        invasion_start();
        send_to_char("Invasion started.\n\r", ch);
    }
    else if (!str_cmp(arg, "stop"))
    {
        if (!invasion_active)
        { send_to_char("No invasion is currently active.\n\r", ch); return; }
        invasion_end(FALSE);
        send_to_char("Invasion stopped.\n\r", ch);
    }
    else if (!str_cmp(arg, "status"))
    {
        int lo = 0, hi = 0;
        count_regular_players(&lo, &hi);
        snprintf(buf, sizeof(buf),
                 "Invasion active : %s\n\r"
                 "Boss alive      : %s\n\r"
                 "Boss profile    : %s\n\r"
                 "Cooldown        : %d ticks remaining\n\r"
                 "Player levels   : %d - %d (pseudo)\n\r",
                 invasion_active ? "@@GYES@@N" : "@@RNO@@N",
                 (invasion_boss != NULL && !invasion_boss->is_free) ? "@@GYES@@N" : "@@RNO@@N",
                 invasion_boss_profile >= 0
                     ? boss_profiles[invasion_boss_profile].name : "none",
                 invasion_timer, lo, hi);
        send_to_char(buf, ch);
    }
    else if (!str_cmp(arg, "bosses"))
    {
        #define ELE_NAME(bits, flag, label) \
            if (IS_SET(bits, flag)) { strcat(tmp, label); strcat(tmp, " "); }

        send_to_char("@@W=== Invasion Boss Roster ===@@N\n\r", ch);
        for (i = 0; i < NUM_BOSSES; i++)
        {
            const BOSS_PROFILE *bp = &boss_profiles[i];
            char res[128] = "", sus[128] = "", tmp[128];
            int j;

            tmp[0] = '\0';
            ELE_NAME(bp->resist, ELE_FIRE,     "Fire")
            ELE_NAME(bp->resist, ELE_WATER,    "Water")
            ELE_NAME(bp->resist, ELE_AIR,      "Air")
            ELE_NAME(bp->resist, ELE_EARTH,    "Earth")
            ELE_NAME(bp->resist, ELE_SHADOW,   "Shadow")
            ELE_NAME(bp->resist, ELE_MENTAL,   "Mental")
            ELE_NAME(bp->resist, ELE_HOLY,     "Holy")
            ELE_NAME(bp->resist, ELE_POISON,   "Poison")
            ELE_NAME(bp->resist, ELE_PHYSICAL, "Physical")
            snprintf(res, sizeof(res), "%s", tmp[0] ? tmp : "none");

            tmp[0] = '\0';
            ELE_NAME(bp->suscept, ELE_FIRE,     "Fire")
            ELE_NAME(bp->suscept, ELE_WATER,    "Water")
            ELE_NAME(bp->suscept, ELE_AIR,      "Air")
            ELE_NAME(bp->suscept, ELE_EARTH,    "Earth")
            ELE_NAME(bp->suscept, ELE_SHADOW,   "Shadow")
            ELE_NAME(bp->suscept, ELE_MENTAL,   "Mental")
            ELE_NAME(bp->suscept, ELE_HOLY,     "Holy")
            ELE_NAME(bp->suscept, ELE_POISON,   "Poison")
            ELE_NAME(bp->suscept, ELE_PHYSICAL, "Physical")
            snprintf(sus, sizeof(sus), "%s", tmp[0] ? tmp : "none");

            snprintf(buf, sizeof(buf),
                     " @@Y%2d@@N. %s%s@@N\n\r"
                     "     @@GResist: %s@@N  @@RWeak: %s@@N\n\r"
                     "     Spells: ",
                     i, bp->short_col, bp->name, res, sus);
            send_to_char(buf, ch);

            for (j = 0; bp->spells[j] != NULL; j++)
            {
                snprintf(buf, sizeof(buf), "%s%s", j ? ", " : "", bp->spells[j]);
                send_to_char(buf, ch);
            }
            send_to_char("\n\r", ch);
        }
        #undef ELE_NAME
    }
    else
    {
        send_to_char("Usage: invasion <start|stop|status|bosses>\n\r", ch);
    }
}
