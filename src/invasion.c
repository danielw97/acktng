/***************************************************************************
 * invasion.c -- Invasion System for Shades of Evil                        *
 *                                                                         *
 * Rules:                                                                  *
 *  - Starts only when >= 1 regular (non-immortal) player is online.       *
 *  - Spawns a BOSS whose level = highest player pseudo_level + 20.        *
 *    Boss has scaled hp_mod (100 to 250,000) based on level.              *
 *    Boss has unique elemental strengths, weaknesses, spells & skills.    *
 *  - Every PULSE_TICK spawns 0-2 ordinary wave mobs.                      *
 *  - Every PULSE_ROOMS the spawned mobs march toward room 3001.           *
 *  - On reaching 3001 the mobs attack Gertrude (mob named "gertrude").    *
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

/* Sentinel stored in extract_timer to mark invasion mobs */
#define INVASION_TAG  (-999)

/* -----------------------------------------------------------------------
 * Forward declarations
 * --------------------------------------------------------------------- */
static ROOM_INDEX_DATA *pick_boss_room    (int boss_level);
static void       invasion_start         (void);
static void       invasion_end           (bool success);
static CHAR_DATA *spawn_invasion_mob     (int level, bool is_boss, int prof_idx);
static void       apply_boss_profile     (CHAR_DATA *mob, int prof_idx, int level);
static void       apply_wave_mob_profile (CHAR_DATA *mob, int wave_idx, int level);
static bool       invasion_boss_cast     (CHAR_DATA *mob);
static void       announce               (const char *msg);
static int        count_regular_players  (int *out_lo, int *out_hi);
static CHAR_DATA *find_gertrude          (void);
static bool       mob_is_invasion_mob    (CHAR_DATA *ch);
static void       despawn_all_invasion   (void);

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
    return (ch->extract_timer == INVASION_TAG);
}

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
        if (ch->fighting != NULL)
            stop_fighting(ch, TRUE);
        ch->extract_timer = 0;   /* clear tag before extract */
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
 * Algorithm (reservoir sampling, single pass):
 *   Walk every qualifying area's room list, pick one room at random so
 *   that each qualifying room has equal probability of being chosen.
 *
 * Falls back to INVASION_START_VNUM if no suitable area/room is found.
 * --------------------------------------------------------------------- */
static ROOM_INDEX_DATA *pick_boss_room(int boss_level)
{
    AREA_DATA        *pArea;
    BUILD_DATA_LIST  *pList;
    ROOM_INDEX_DATA  *chosen  = NULL;
    int               n       = 0;   /* total qualifying rooms seen so far */
    int               lo      = boss_level - INVASION_BOSS_AREA_SLACK;
    int               hi      = boss_level + INVASION_BOSS_AREA_SLACK;

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

            n++;
            /* Replace chosen with probability 1/n (uniform distribution). */
            if (number_range(1, n) == 1)
                chosen = room;
        }
    }

    if (chosen != NULL)
        return chosen;

    /* Fallback: original fixed spawn room. */
    bug("pick_boss_room: no level-appropriate room for boss level %d, "
        "falling back to INVASION_START_VNUM.", boss_level);
    return get_room_index(INVASION_START_VNUM);
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
static CHAR_DATA *spawn_invasion_mob(int level, bool is_boss, int prof_idx)
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

    /* Bosses get a level-appropriate random room; wave mobs use the fixed entry point. */
    if (is_boss)
    {
        spawn_room = pick_boss_room(level);
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

    if (is_boss)
        apply_boss_profile(mob, prof_idx, level);
    else
        apply_wave_mob_profile(mob, wave_idx, level);

    mob->extract_timer = INVASION_TAG;

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

    invasion_boss = spawn_invasion_mob(boss_level, TRUE, prof);
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
                 "Everyone receives @@Y25 quest points@@G!@@N");

        for (d = first_desc; d != NULL; d = d->next)
        {
            if (d->connected != CON_PLAYING) continue;
            ch = d->character;
            if (ch == NULL || IS_NPC(ch) || IS_IMMORTAL(ch)) continue;
            ch->quest_points += INVASION_QP_REWARD;
            snprintf(buf, sizeof(buf),
                     "You receive @@Y%d quest points@@N for defending the realm!\n\r",
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
    invasion_timer        = INVASION_MIN_INTERVAL;
}

/* -----------------------------------------------------------------------
 * invasion_update() — called once per PULSE_TICK from update_handler()
 * --------------------------------------------------------------------- */
void invasion_update(void)
{
    int lo = 1, hi = 1, count, lo_lvl, hi_lvl, i;

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
        invasion_timer        = INVASION_MIN_INTERVAL;
        return;
    }

    if (invasion_boss != NULL && invasion_boss->is_free)
    {
        invasion_boss = NULL;
        invasion_end(TRUE);
        return;
    }

    if (invasion_boss != NULL && !invasion_boss->is_free
        && invasion_boss->fighting != NULL)
    {
        invasion_boss_cast(invasion_boss);
    }

    lo_lvl = URANGE(1,      lo + INVASION_MOB_MIN_OFFSET, MAX_LEVEL);
    hi_lvl = URANGE(lo_lvl, hi + INVASION_MOB_MAX_OFFSET, MAX_LEVEL);

    for (i = 0; i < number_range(0, 2); i++)
        spawn_invasion_mob(number_range(lo_lvl, hi_lvl), FALSE, -1);
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

    if (!invasion_active) return;

    target_room = get_room_index(INVASION_SPAWN_VNUM);
    if (!target_room) return;

    for (ch = first_char; ch != NULL; ch = ch_next)
    {
        ch_next = ch->next;

        if (!mob_is_invasion_mob(ch))    continue;
        if (ch == invasion_boss)         continue;  /* boss does not march */
        if (ch->in_room == NULL)         continue;
        if (ch->fighting != NULL)        continue;
        if (ch->position < POS_STANDING) continue;

        if (ch->in_room->vnum == INVASION_SPAWN_VNUM)
        {
            gertrude = find_gertrude();
            if (gertrude != NULL && gertrude->fighting == NULL)
            {
                if (ch == invasion_boss && invasion_boss_profile >= 0)
                    act(boss_profiles[invasion_boss_profile].emote_skill,
                        ch, NULL, gertrude, TO_ROOM);
                else
                    act("$n lets out a war cry and charges at Gertrude!",
                        ch, NULL, NULL, TO_ROOM);
                multi_hit(ch, gertrude, TYPE_UNDEFINED);
            }
            continue;
        }

        dir = h_find_dir(ch->in_room, target_room,
                         HUNT_WORLD | HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR);
        if (dir >= 0)
            move_char(ch, dir);
    }
}

/* -----------------------------------------------------------------------
 * invasion_on_death() — hook called just BEFORE extract_char in raw_kill()
 * --------------------------------------------------------------------- */
void invasion_on_death(CHAR_DATA *ch, CHAR_DATA *killer)
{
    (void)killer;
    if (!invasion_active) return;

    if (ch == invasion_boss)
    {
        ch->extract_timer = 0;
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
            invasion_boss->extract_timer = 0;
        invasion_boss = NULL;
        invasion_end(FALSE);
        return;
    }

    if (mob_is_invasion_mob(ch))
        ch->extract_timer = 0;
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
