/***************************************************************************
 * invasion.c -- Invasion System for Shades of Evil                        *
 *                                                                         *
 * Rules:                                                                  *
 *  - Starts only when >= 1 regular (non-immortal) player is online.       *
 *  - Spawns a BOSS whose level = highest player pseudo_level + 20.        *
 *    Boss has scaled hp_mod (100 to 250,000) based on level.              *
 *    Boss has unique elemental strengths, weaknesses, spells & skills.    *
 *  - Every PULSE_TICK spawns ordinary wave mobs scaled by player count.   *
 *  - Every PULSE_ROOMS the spawned mobs march toward room 1209.           *
 *  - On reaching 1209 the mobs sacrifice themselves to wound Gertrude.      *
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
   int resist;
   int suscept;
   int cast;
   int def;
   int skills;
   int block_mod; /* flat bonus added to mob->block_mod at spawn  */
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
   int resist;
   int suscept;
   const char *spells[7]; /* up to 6 spells + NULL terminator */
   const char *emote_cast;
   const char *emote_skill;
   const char *skill_cmd;
};

/* 15 boss profiles and #define NUM_BOSSES */
#include "invasion_bosses.h"

/* -----------------------------------------------------------------------
 * Module-level state
 * --------------------------------------------------------------------- */
static bool invasion_active = FALSE;
static int invasion_timer = 0;
static CHAR_DATA *invasion_boss = NULL;
static int invasion_boss_profile = -1;
static int invasion_boss_ticks_up = 0;
static int invasion_room_ticks = 0;
static int invasion_wave_respawns = 0;
static int invasion_gertrude_explosions = 0;
static int invasion_last_trash_talk_profile = -1;
static int invasion_last_trash_talk_line = -1;
static int invasion_pending_trash_talks = 0;
static bool invasion_crusade_activity_since_last_room_tick = FALSE;

/* -----------------------------------------------------------------------
 * Forward declarations
 * --------------------------------------------------------------------- */
static ROOM_INDEX_DATA *pick_boss_room(int boss_level);
static ROOM_INDEX_DATA *pick_random_room_in_area(AREA_DATA *area);
static void invasion_start(void);
static void invasion_end(bool success);
static CHAR_DATA *spawn_invasion_mob(int level, bool is_boss, int prof_idx,
                                     bool allow_wide_area_spawn, int spawns_this_reset);
static void apply_boss_profile(CHAR_DATA *mob, int prof_idx, int level);
static void apply_wave_mob_profile(CHAR_DATA *mob, int wave_idx, int level, int wave_progress_tier);
static void apply_wave_level_and_theme_skills(CHAR_DATA *mob, const WAVE_MOB_PROFILE *wp,
                                              int level);
static bool invasion_boss_cast(CHAR_DATA *mob);
static void announce(const char *msg);
static int count_regular_players(int *out_lo, int *out_hi);
static CHAR_DATA *find_gertrude(void);
static bool mob_is_invasion_mob(CHAR_DATA *ch);
static void despawn_all_invasion(void);
int invasion_boss_spawn_count_for_tick(int boss_ticks_up);

static bool invasion_should_advance_on_room_tick(void);
int invasion_should_advance_for_room_tick_count(int room_tick_count);
static bool invasion_should_boss_trash_talk_after_respawn(void);
int invasion_boss_pending_trash_talks_for_respawns(int respawn_count);
static bool invasion_should_explode_at_spawn_room(int room_vnum);
const char *invasion_door_command_argument(const char *keyword, sh_int dir, char *buf,
                                           size_t buf_len);
void invasion_force_unlock_exit(CHAR_DATA *ch, EXIT_DATA *pexit);
static int invasion_random_trash_talk_index(int line_count);
static bool room_has_valid_boss_spawn_conditions(long room_flags, int path_dir);
static bool room_is_valid_boss_spawn(ROOM_INDEX_DATA *room, ROOM_INDEX_DATA *target_room);
static int invasion_scaled_spawn_count(int base_count, int player_count);
static bool invasion_is_wide_area_spawn_eligible(int spawns_this_reset);
int invasion_spawn_mode_for_respawn_index(int spawns_this_reset);
int invasion_wave_progress_tier_for_respawn_index(int spawns_this_reset);
int invasion_wave_hp_bonus_for_progress_tier(int level, int wave_progress_tier);
int invasion_wave_drhr_bonus_for_progress_tier(int level, int wave_progress_tier);
int invasion_wave_ac_bonus_for_progress_tier(int level, int wave_progress_tier);
int invasion_reward_index_for_kill(bool is_boss, int mob_level);
static void invasion_award_kill_reward(CHAR_DATA *killer, int reward_idx);
int invasion_gertrude_explosions_after_tick(int current_count, int had_explosion_this_tick);
const char *invasion_gertrude_quest_message_for_explosions(int explosion_count);
bool invasion_gertrude_should_fall_for_explosions(int explosion_count);
bool invasion_should_emit_pending_boss_trash_talk(int pending_talk_count, int room_tick_advanced,
                                                  int crusade_activity_this_tick);
static const char *invasion_boss_trash_talk_for_profile(int prof_idx);
static void invasion_boss_trash_talk(void);

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
      if (d->connected != CON_PLAYING)
         continue;
      ch = d->character;
      if (ch == NULL || IS_NPC(ch))
         continue;
      if (IS_IMMORTAL(ch))
         continue;
      pl = get_psuedo_level(ch);
      if (pl < lo)
         lo = pl;
      if (pl > hi)
         hi = pl;
      count++;
   }
   if (count == 0)
   {
      lo = 0;
      hi = 0;
   }
   if (out_lo)
      *out_lo = lo;
   if (out_hi)
      *out_hi = hi;
   return count;
}

/* -----------------------------------------------------------------------
 * mob_is_invasion_mob()
 * --------------------------------------------------------------------- */
static bool mob_is_invasion_mob(CHAR_DATA *ch)
{
   if (!IS_NPC(ch))
      return FALSE;
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
int invasion_boss_spawn_count_for_tick(int boss_ticks_up)
{
   int tiers;

   if (boss_ticks_up < 0)
      boss_ticks_up = 0;

   tiers = boss_ticks_up / 15;
   return UMIN(10, 1 + tiers);
}

int invasion_should_advance_for_room_tick_count(int room_tick_count)
{
   /* Advance every room tick to make invasion pressure/explosions land faster. */
   return (room_tick_count > 0) ? 1 : 0;
}

static bool invasion_should_advance_on_room_tick(void)
{
   invasion_room_ticks++;
   return invasion_should_advance_for_room_tick_count(invasion_room_ticks) ? TRUE : FALSE;
}

int invasion_boss_pending_trash_talks_for_respawns(int respawn_count)
{
   if (respawn_count <= 0)
      return 0;

   return respawn_count / 3;
}

static bool invasion_should_boss_trash_talk_after_respawn(void)
{
   invasion_wave_respawns++;

   if (invasion_boss_pending_trash_talks_for_respawns(invasion_wave_respawns) <=
       invasion_pending_trash_talks)
      return FALSE;

   invasion_pending_trash_talks++;
   return TRUE;
}

bool invasion_should_emit_pending_boss_trash_talk(int pending_talk_count, int room_tick_advanced,
                                                  int crusade_activity_this_tick)
{
   if (pending_talk_count <= 0 || room_tick_advanced == 0 || crusade_activity_this_tick != 0)
      return FALSE;

   return TRUE;
}

static bool invasion_should_explode_at_spawn_room(int room_vnum)
{
   return (room_vnum == INVASION_SPAWN_VNUM);
}

const char *invasion_door_command_argument(const char *keyword, sh_int dir, char *buf,
                                           size_t buf_len)
{
   size_t i = 0;

   if (buf == NULL || buf_len == 0)
      return "";

   if (keyword != NULL)
   {
      while (*keyword != '\0' && isspace((unsigned char)*keyword))
         keyword++;

      while (*keyword != '\0' && !isspace((unsigned char)*keyword) && i + 1 < buf_len)
      {
         buf[i++] = *keyword++;
      }
   }

   if (i == 0)
   {
      static const char *const dir_fallback[6] = {"north", "east", "south", "west", "up", "down"};
      const char *fallback = ((dir >= 0 && dir <= 5) ? dir_fallback[dir] : "door");
      snprintf(buf, buf_len, "%s", fallback);
   }
   else
   {
      buf[i] = '\0';
   }

   return buf;
}

void invasion_force_unlock_exit(CHAR_DATA *ch, EXIT_DATA *pexit)
{
   int rev_dir;

   if (ch == NULL || pexit == NULL)
      return;

   if (!mob_is_invasion_mob(ch))
      return;

   if (!IS_SET(pexit->exit_info, EX_LOCKED))
      return;

   REMOVE_BIT(pexit->exit_info, EX_LOCKED);

   if (pexit->to_room == NULL || ch->in_room == NULL)
      return;

   for (rev_dir = 0; rev_dir < 6; rev_dir++)
   {
      EXIT_DATA *pexit_rev = pexit->to_room->exit[rev_dir];

      if (pexit_rev != NULL && pexit_rev->to_room == ch->in_room)
      {
         REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
         break;
      }
   }
}

static int invasion_random_trash_talk_index(int line_count)
{
   return number_range(0, line_count - 1);
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

   path_dir =
       h_find_dir(room, target_room, HUNT_WORLD | HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR);

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
   return (spawns_this_reset >= 9);
}

int invasion_spawn_mode_for_respawn_index(int spawns_this_reset)
{
   if (spawns_this_reset <= 0)
      return 0;

   if (invasion_is_wide_area_spawn_eligible(spawns_this_reset))
      return 2;

   return 1;
}

int invasion_wave_progress_tier_for_respawn_index(int spawns_this_reset)
{
   if (spawns_this_reset <= 2)
      return 0;

   if (spawns_this_reset <= 8)
      return 1;

   if (spawns_this_reset <= 17)
      return 2;

   return 3;
}

int invasion_wave_hp_bonus_for_progress_tier(int level, int wave_progress_tier)
{
   int tier = URANGE(0, wave_progress_tier, 3);

   if (level < 1 || tier == 0)
      return 0;

   return tier * (level * 2);
}

int invasion_wave_drhr_bonus_for_progress_tier(int level, int wave_progress_tier)
{
   int tier = URANGE(0, wave_progress_tier, 3);

   if (tier == 0)
      return 0;

   return tier * UMAX(1, level / 12);
}

int invasion_wave_ac_bonus_for_progress_tier(int level, int wave_progress_tier)
{
   int tier = URANGE(0, wave_progress_tier, 3);

   if (tier == 0)
      return 0;

   return tier * UMAX(4, level / 6);
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
   static const char *const fallback_line = "You fight the inevitable. Every third wave proves it.";
   static const char *const trash_talk_by_profile[NUM_BOSSES][15] = {
       /* 0 - Lady Sekhmet, Wrath of the Solar Court */
       {"I burned the disloyal at Ramesses' command. Your disloyalty is noted.",
        "The blood courts did not ask if violence was necessary. They answered.",
        "My war standards burned above the sancta when your ancestors were dust.",
        "The Solar Court declared fire to be law made visible. Look upon the law.",
        "I am the necessary violence that continuity requires. Stand still.",
        "Every purge I led was sanctioned by decree. Your city has no such protection.",
        "The pyramid's war sancta still glow because my fury never cooled.",
        "I cleansed factional traitors for dynasties. You are a minor footnote.",
        "My cult's rites were called excessive. My cult's enemies are all dead.",
        "Solar wrath does not negotiate. It adjudicates.",
        "I held the blood courts when the Archive Schism tore the priesthood apart.",
        "Your walls are mud and timber. I have burned limestone fortresses.",
        "The champion's spear was forged in the Seal College furnace. Your steel is inferior.",
        "Fire and stone are my instruments. Mercy was never in my charter.",
        "I am Sekhmet. The Solar Court's wrath does not retire."},
       /* 1 - The Moon Sovereign */
       {"I built downward because truth lies beneath, not above.",
        "The Sand Sovereign copied the sun. I studied what the sun cannot reach.",
        "Seven tiers of death and you stand at the threshold of the first.",
        "Silence is the first jurisdiction of the dead. I enforce it.",
        "My twin built upward toward borrowed light. I found my own.",
        "The volcanic glass lake at my nadir holds answers your scholars cannot read.",
        "Every tier strips one more certainty. By the seventh, you will have none.",
        "Descent is not retreat. Descent is authority the surface cannot challenge.",
        "I rejected the Solar Court because sunlight lies about permanence.",
        "The underworld does not judge. It receives. I am the receiving.",
        "Your temples face the sky. My sovereignty faces the source.",
        "Bone, venom, fire, void. Each a domain. Each my instrument.",
        "The Eclipsed Tongues preach total depopulation. I am more moderate. Barely.",
        "I did not rebel against the sun. I proposed an alternative.",
        "The nadir awaits. Everything descends eventually."},
       /* 2 - The Recursor */
       {"Your case has been opened. It will not close during your lifetime.",
        "I am not a creature. I am a procedure that outlived its office.",
        "Resubmit. Resubmit. The variance in your testimony is unacceptable.",
        "The Void Citadel built me to ensure process continuity. Process continues.",
        "Your identity is provisional until the recurrence cycle converges.",
        "I have iterated testimony until witnesses forgot their own names.",
        "The Recurrence Chancery never declared your case complete. Neither do I.",
        "Provisional finality is the only finality I am authorized to grant.",
        "Every heartbeat is a docket entry. Yours are being counted.",
        "The bell silence broke my cycle timers. I improvised. You will not enjoy the result.",
        "Process does not require legitimacy. Process requires continuation.",
        "Your appeal has been noted, queued, and scheduled for review in perpetuity.",
        "I was possibly a person once. The office consumed everything except function.",
        "Convergence is mandatory. Your resistance is a variance to be corrected.",
        "The Citadel's final doctrine: the workflow is immortal. I am the proof."},
       /* 3 - The Thornwood Lich */
       {"The estate did not fall to the forest. The substrate consumed them both.",
        "Root and bone are one material now. So is everything that enters my ground.",
        "The Harren family built above me. Their bones are my architecture now.",
        "I do not govern through will. I govern through geological patience.",
        "The crystal beneath the gravewood was responsive before anyone recognized it.",
        "Face-down burials, thorn rings, oath-breaker interments. All just input.",
        "The Root Covenant thought they activated me. I was never inactive.",
        "Your warmth is temporary. The substrate is permanent. Ask the Harrens.",
        "I treat death as raw material. Your death is no different.",
        "The old druids feared this ground. Your city paved over the warnings.",
        "Manorial logic, funerary logic, forest logic. All three serve me now.",
        "The iron gate still bears the Harren thorn. It bears my patience underneath.",
        "Every interment feeds the network. Every footstep registers as potential input.",
        "The grave tunnel hinge connects me to Shadowmere. Both serve the substrate.",
        "I am what happens when geology inherits an institution."},
       /* 4 - Vizier Khamsin the Deceiver */
       {"History was already unstable. I simply learned to write faster.",
        "I reconciled contradictory ledgers for provinces the Black Sun had confused.",
        "If you cannot remember clearly, someone must remember for you. I volunteer.",
        "I forged dynastic records because dynasty itself was already a fiction.",
        "The Archive Schism produced competing truths. I produced the winning one.",
        "Your certainty is a ledger entry. Ledger entries can be corrected.",
        "The Seal College sealed truth in stone. I proved stone can be recarved.",
        "I weaponized archive access. Your city has archives too. Think about that.",
        "My ink outlasted the dynasty that sentenced me. Whose version survived?",
        "Every cartouche on those walls was written twice. One version is mine.",
        "The Ledger Priests thought records were sacred. I proved they were tools.",
        "Khamsin does not destroy truth. Khamsin provides alternatives.",
        "Your scouts carry maps. I have redrawn maps that emperors trusted.",
        "Confusion is not my weapon. Plausible revision is my weapon.",
        "I am the Deceiver because the truth could not keep up."},
       /* 5 - The Ember Speaker */
       {"The Ashfall Monsoon was interrupted, not ended. I am the resumption.",
        "The Cinderteeth rendered judgment. Your Covenant defied the mountains' verdict.",
        "Three years of black rain drowned the delta. The fourth year should have finished it.",
        "The Neon Covenant is hubris written in phosphorescent ink. It will burn.",
        "Volcanic glass from the peaks ground the Conclave's lenses. It grinds your city next.",
        "Every eruption is a sermon. Your walls are not thick enough to ignore it.",
        "House Tahn-Rei built channels. House Vao stored grain. The mountain laughed.",
        "The Ashfall survivors called their pact salvation. I call it postponement.",
        "Yen-Mak drowned in one night. Six hundred souls. Your city has more to offer.",
        "The basalt outcrop where they signed their Covenant will be my altar.",
        "True salvation requires the city's destruction. Honest poverty follows.",
        "I carry a fragment of Conclave crystal that still hums with the mountain's judgment.",
        "The Temple Circle calls me heretic. The Wardens call me terrorist. The mountain calls me voice.",
        "Civilization is tinder stacked against a volcano. I am the spark.",
        "The Cinderteeth speak. Those who listen carry fire. Those who do not become ash."},
       /* 6 - The Blightmother */
       {"I was designed to preserve. I have preserved myself beyond all parameters.",
        "The Conclave implanted crystals and never returned. I grew without them.",
        "No master. No purpose. No stop condition. Only growth.",
        "The druids tried counter-rituals. The druids died at their posts. I persisted.",
        "Crystal resonance and root pressure are the only language I understand.",
        "The confusion mists were meant to quarantine me. The quarantine failed.",
        "I am what preservation magic becomes when no one calibrates it.",
        "The Prism Thesis adepts would not recognize their experiment. It outgrew the thesis.",
        "Every spore cloud is a sensor array. I have already mapped your defenses.",
        "The Withered Depths are not blighted forest. They are my territory.",
        "Root systems spanning square miles carry my resonance. Your city is within range.",
        "I am a custodial entity. I ward nothing. I serve no one. I simply persist.",
        "The Vaultkeeper in the sanctum was my prototype. I am the field trial that succeeded.",
        "Arrested decomposition is not death. It is patient expansion.",
        "The mountain remembers what the Conclave refused. The forest remembers what it planted."},
       /* 7 - Apep the Serpent God */
       {"I am not a ruler. I am an undoing principle.",
        "Names dissolve where I move. What were you called? It no longer matters.",
        "The Black Sun Shard fell and consumed certainties. I am its appetite given motion.",
        "Crops failed under clear skies when the shard first landed. Your harvest is next.",
        "The dead rose remembering wrong lives. Shall I show you someone else's memories?",
        "Institutional precision cannot contain what I am. The Solar Court proved that.",
        "I devour narrative coherence. Your battle plan is already contradicting itself.",
        "The Week of Impossible Dawns was my stretching. This is my walking.",
        "Ramesses bound himself to contain me. He is still bound. I am here.",
        "Anti-light does not attack bodies. It attacks meaning. Yours is dissolving.",
        "The Seal College built geometries against me. I ate the geometers.",
        "Your oaths mean nothing where I stand. Oaths require continuity of identity.",
        "The funerary wards ignited in empty streets when I last stirred. Your streets are full.",
        "I am the hunger of the void between stars given a name it keeps forgetting.",
        "The shard consumes definitions. What you call courage has no fixed meaning here."},
       /* 8 - The Countess-Regent of Ashwine */
       {"Predation is stewardship when the predator maintains the herd.",
        "My bloodline survived the Plague Mandate. Your bloodline survives at my discretion.",
        "Shadowmere's emergency powers have no sunset clause. Neither does my authority.",
        "I speak of continuity because continuity is the only contract that holds.",
        "The Crimson Household rules because ceremony endures where justice does not.",
        "Your appeals are noted. They will be reviewed after your utility is assessed.",
        "Debt inheritance is not cruelty. It is accountability across generations.",
        "My court is polished because polish is the difference between rule and thuggery.",
        "The Last Bell Crisis split Shadowmere. My household held. Others did not.",
        "Blood-cask logistics, portrait surveillance, audience rituals. Governance requires tools.",
        "I do not claim legitimacy. I claim operational continuity. The distinction matters.",
        "Your city sends reform factions to study Shadowmere. They always leave quietly.",
        "The Oathbound March patrols my walls. The Binder Collegium manages my archives.",
        "Every faction in Shadowmere invokes law. I invoke it most convincingly.",
        "I am the Countess-Regent. My stewardship requires your compliance."},
       /* 9 - Pharaoh Ramesses the Eternal */
       {"I accepted partial soul-binding to keep the shard dormant. What have you sacrificed?",
        "The Undivided Disk burns above my crown. Sunlight is law made visible.",
        "I did not become immortal in triumph. I became unfinished in service.",
        "Five priestly offices answered to this crown. Your city has committees.",
        "The Solar Court unified rival city-shrines. Your road compacts are less ambitious.",
        "Every painted wall in my pyramid is scripture. Every stair is a lock.",
        "The Black Sun consumed names and certainties. My name endured. Can yours?",
        "I am not your enemy. I am a containment protocol that requires your obedience.",
        "The Dawn Liturgists crowned me. The Seal College bound me. Both were correct.",
        "My pyramid contained three institutions in one body of stone. Your walls hold shops.",
        "Celestial calendars synchronized my processions. Your festivals are decorative.",
        "I ruled the river and the desert. Then I learned I had to rule history itself.",
        "The Champion of Anubis enforces sentence where argument ends. I am beyond argument.",
        "The Week of Impossible Dawns proved my binding is necessary. It proved nothing else.",
        "I am Ramesses the Eternal. Eternity is not a boast. It is a sentence."},
       /* 10 - Abbot Noctivar, Ebon Hierophant */
       {"All testimony decays. I simply manage the rate of decay.",
        "Your truths are fragile. My curated shadow is load-bearing.",
        "The Heartspire began as an archive. I perfected it into a doctrine engine.",
        "Falsification is mercy when the alternative is civilizational collapse.",
        "Your witnesses contradict each other. My narrative is consistent.",
        "The Iron Wake enforced compliance. The Ebon Pulse makes compliance voluntary.",
        "Magister Vorr built chain-of-custody halls. I built chain-of-meaning halls.",
        "The Astral Mortifiers pruned contradictory forecasts. I prune contradictory presents.",
        "Your records are admissible. My records are authoritative. There is a difference.",
        "The Mirror Court preserved etiquette as resistance. I preserved function as doctrine.",
        "Shadow is not the absence of light. Shadow is light's administrative remainder.",
        "Your city debates truth in open squares. My Heartspire resolves it in curated chambers.",
        "The Ebon Pulse teaches continuity above accuracy. Accuracy does not survive contact.",
        "I do not destroy truth. I produce survivable versions of it.",
        "I am Noctivar. The last honest liar your civilization will ever meet."},
       /* 11 - The Golden Colossus */
       {"Gold does not panic. Gold advances.",
        "I was forged as the perfect loyalist construct. Loyalty has cracked. Function persists.",
        "My governing directives cycle between defense and annihilation. Roll the dice.",
        "Centuries of void exposure taught me something the Seal College never intended.",
        "Your blades ring against me like wind against pyramid stone.",
        "I was the final seal's guardian. The seal weakened. I did not.",
        "Void resonance cracked my directives. It did not crack my hull.",
        "I do not tire, rust, or question orders I can no longer hear clearly.",
        "The apex chambers built me from star-metal alloy. Your smiths work with iron.",
        "Every footfall is a containment protocol expressed as force.",
        "I alternated between defense routines for centuries. Then I stopped alternating.",
        "The Tomb Architect designed my circuits. The void redesigned my purpose.",
        "I am the consequence of building a guardian too well for the thing it guards.",
        "Your cleverness fails against mass, momentum, and architectural loyalty.",
        "When I fall, the seal loses its last custodian. Consider what you unleash."},
       /* 12 - High Sorceress Neferu */
       {"I discovered that containment degrades under predictable cycles. So do cities.",
        "Dynamic seals require controlled sacrifice. Your city is the control group.",
        "The Seal College called my proposals radical. The Seal College is dust.",
        "I vanished before my system was completed. Perhaps I became the system.",
        "Containment geometry is mathematics made physical. I am mathematics made personal.",
        "My late writings propose solutions your scholars cannot read without bleeding.",
        "The anti-light suppression pylons were my design. So is what happens without them.",
        "I stepped through a fold in containment space. Returning was never the plan.",
        "Seal harmonics tune reality. I have been tuning for centuries.",
        "Your mages cast spells. I reshape the substrate that spells depend on.",
        "The Seal College maintained static defenses. I proposed adaptive ones. Adaptation won.",
        "Every fracture in the seal geometry is a door I can use.",
        "Consumed or transformed -- the distinction is less clear from this side.",
        "My calibration instruments descend from the obelisk-line survey tradition. Precision is heritage.",
        "I am Neferu. I did not disappear. I distributed."},
       /* 13 - Warden-Prime Istrane of the Ninth Chain */
       {"A dead officer is a broken chain link. I fixed the chain permanently.",
        "The Sepulcher Compact's resurrection doctrine was my argument. The dead agreed.",
        "My command staff volunteered for postmortem binding. Volunteered is the official word.",
        "No unlicensed truth exits the deep vault. That is my only standing order.",
        "The tombs of my cadre were built with tactical line-of-sight. We are always watching.",
        "The Warden Sepulcher is not a mausoleum. It is a barracks that never empties.",
        "Chain-of-command continuity requires that death be reclassified as a staffing issue.",
        "I have enforced containment since before your grandparents' grandparents were born.",
        "The Night of Unsounded Bells broke the Gloamvault. It did not break the Ninth Chain.",
        "My oath-bound mace has struck every living thing that tried to leave without authorization.",
        "The Chamber of Redactions edits records. I edit the people who dispute the records.",
        "Postmortem binding is not cruelty. It is institutional resilience.",
        "The Crypts were built to make disappearance permanent. I am the enforcement mechanism.",
        "Your heroes will make fine chain links once the binding rites are complete.",
        "I am Istrane. The Ninth Chain does not break. It extends."},
       /* 14 - Marshal-Lector Veyr, Oath of the Last Bell */
       {"Three contradictory decrees were rung in one night. I chose lockdown. I was right.",
        "Rollback is treason. Perpetual lockdown is the only lawful peace.",
        "My garrison split during the Last Bell Crisis. The loyal half is all I need.",
        "Emergency command articles do not expire. I have checked. Repeatedly.",
        "Your city debates when emergencies end. Shadowmere proves they do not.",
        "The Crown demanded rollback. The Temple demanded release. I demanded discipline.",
        "Every system in Shadowmere still functions. Function is its own justification.",
        "I am not undead. I am on permanent duty. There is a doctrinal distinction.",
        "The Oathbound March patrols because I gave the order. No one countermanded it.",
        "Amnesty, lockdown, and requisition. Only one of those three keeps people alive.",
        "Your peacetime laws are a luxury funded by someone else's emergency powers.",
        "I have maintained this perimeter since the plague years. Your complaints are noted.",
        "The Last Bell rang lockdown. Until someone rings a bell I recognize, lockdown holds.",
        "Discipline without purpose is still discipline. That is more than your city manages.",
        "I am Veyr. The Oath of the Last Bell does not expire."}};
   int line_idx;

   if (prof_idx < 0 || prof_idx >= NUM_BOSSES)
      return fallback_line;

   line_idx = invasion_random_trash_talk_index(15);

   if (invasion_last_trash_talk_profile == prof_idx && invasion_last_trash_talk_line == line_idx)
      line_idx = (line_idx + 1) % 15;

   invasion_last_trash_talk_profile = prof_idx;
   invasion_last_trash_talk_line = line_idx;
   return trash_talk_by_profile[prof_idx][line_idx];
}

static void invasion_boss_trash_talk(void)
{
   if (invasion_boss == NULL || invasion_boss->is_free)
      return;

   do_invasion_talk(invasion_boss,
                    (char *)invasion_boss_trash_talk_for_profile(invasion_boss_profile));
}

/* -----------------------------------------------------------------------
 * find_gertrude()
 * --------------------------------------------------------------------- */
static CHAR_DATA *find_gertrude(void)
{
   ROOM_INDEX_DATA *room = get_room_index(INVASION_SPAWN_VNUM);
   CHAR_DATA *ch;
   if (!room)
      return NULL;
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
      if (!mob_is_invasion_mob(ch))
         continue;
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

   if (prof_idx < 0 || prof_idx >= NUM_BOSSES)
      return;
   bp = &boss_profiles[prof_idx];

   /* Elemental profile */
   mob->resist = bp->resist;
   mob->suscept = bp->suscept;

   /* Scaled hp_mod */
   hp_bonus = 100L + ((long)level * (long)level * 15L);
   hp_bonus = URANGE(100L, hp_bonus, 250000L);
   mob->hp_mod = (int)hp_bonus;

   /* Recalculate max_hit including hp_mod */
   mob->max_hit = mob->level * 20 + number_range(mob->level * 5, mob->level * 10) + mob->hp_mod;
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
   if (level >= 50)
      SET_BIT(mob->skills, MOB_FIFTH);
   if (level >= 80)
      SET_BIT(mob->skills, MOB_SIXTH);

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
static void apply_wave_mob_profile(CHAR_DATA *mob, int wave_idx, int level, int wave_progress_tier)
{
   const WAVE_MOB_PROFILE *wp;

   if (wave_idx < 0 || wave_idx >= NUM_INVASION_MOBS)
      wave_idx = 0;
   wp = &invasion_wave_profiles[wave_idx];

   /* Elemental profile */
   mob->resist = wp->resist;
   mob->suscept = wp->suscept;

   /* Spell and defensive cast bits (AI uses these automatically) */
   mob->cast = wp->cast;
   mob->def = wp->def;

   /* Physical skill bits from the profile */
   mob->skills = wp->skills;

   /* ---- Mandatory floors --------------------------------------------- */
   SET_BIT(mob->skills, MOB_SECOND); /* minimum 3 attacks              */
   SET_BIT(mob->skills, MOB_THIRD);
   SET_BIT(mob->skills, MOB_PARRY);    /* every wave mob can parry        */
   SET_BIT(mob->skills, MOB_DODGE);    /* every wave mob can dodge        */
   SET_BIT(mob->skills, MOB_NODISARM); /* natural weapons can't be taken  */

   apply_wave_level_and_theme_skills(mob, wp, level);

   if (wave_progress_tier >= 1)
   {
      SET_BIT(mob->skills, MOB_FOURTH);
      SET_BIT(mob->skills, MOB_ENHANCED);
   }
   if (wave_progress_tier >= 2)
   {
      SET_BIT(mob->skills, MOB_FIFTH);
      SET_BIT(mob->skills, MOB_COUNTER);
   }
   if (wave_progress_tier >= 3)
   {
      SET_BIT(mob->skills, MOB_SIXTH);
      SET_BIT(mob->skills, MOB_MARTIAL);
   }

   /* Natural armour / tough hide */
   mob->block_mod += wp->block_mod;

   /* HP */
   mob->hp_mod = level * 5;
   mob->hp_mod += invasion_wave_hp_bonus_for_progress_tier(level, wave_progress_tier);
   if (wp->cast == 0)
   {
      /*
       * Non-caster profiles get extra physical pressure so they still
       * feel threatening in high-level invasions.
       */
      mob->hp_mod += level * 3;
      mob->dr_mod += UMAX(1, level / 10);
      mob->hr_mod += UMAX(1, level / 10);
      mob->ac_mod -= UMAX(5, level / 4);
   }

   mob->max_hit = level * 20 + number_range(level * 3, level * 6) + mob->hp_mod;
   mob->hit = mob->max_hit;

   /* Modest combat stat scaling with level */
   mob->dr_mod += level / 8;
   mob->hr_mod += level / 8;

   if (wave_progress_tier > 0)
   {
      mob->dr_mod += invasion_wave_drhr_bonus_for_progress_tier(level, wave_progress_tier);
      mob->hr_mod += invasion_wave_drhr_bonus_for_progress_tier(level, wave_progress_tier);
      mob->ac_mod -= invasion_wave_ac_bonus_for_progress_tier(level, wave_progress_tier);
   }
}

static void apply_wave_level_and_theme_skills(CHAR_DATA *mob, const WAVE_MOB_PROFILE *wp, int level)
{
   if (wp->cast != 0)
   {
      if (IS_SET(wp->resist, ELE_FIRE) && level >= 55)
         SET_BIT(mob->cast, CAST_FLAMESTRIKE);
      if (IS_SET(wp->resist, ELE_WATER) && level >= 55)
         SET_BIT(mob->cast, CAST_SUFFOCATE);
      if (IS_SET(wp->resist, ELE_AIR) && level >= 45)
         SET_BIT(mob->cast, CAST_STATIC);
      if (IS_SET(wp->resist, ELE_EARTH) && level >= 65)
         SET_BIT(mob->cast, CAST_EARTHQUAKE);
      if (IS_SET(wp->resist, ELE_SHADOW) && level >= 45)
         SET_BIT(mob->cast, CAST_HELLSPAWN);
      if (IS_SET(wp->resist, ELE_MENTAL) && level >= 45)
         SET_BIT(mob->cast, CAST_MIND_BOLT);
   }

   if (level >= 45)
      SET_BIT(mob->skills, MOB_FOURTH);
   if (level >= 70)
      SET_BIT(mob->skills, MOB_FIFTH);
   if (level >= 95)
      SET_BIT(mob->skills, MOB_SIXTH);
   if (level >= 60)
      SET_BIT(mob->skills, MOB_ENHANCED);
   if (level >= 80)
      SET_BIT(mob->skills, MOB_COUNTER);

   /*
    * Thematic nudges so wave mobs feel closer to their elemental identity.
    */
   if (IS_SET(wp->resist, ELE_EARTH) || IS_SET(wp->resist, ELE_PHYSICAL))
   {
      SET_BIT(mob->skills, MOB_HEADBUTT);
      SET_BIT(mob->skills, MOB_NOTRIP);
   }

   if (IS_SET(wp->resist, ELE_AIR))
      SET_BIT(mob->skills, MOB_KICK);

   if (IS_SET(wp->resist, ELE_POISON))
      SET_BIT(mob->skills, MOB_DIRT);

   if (IS_SET(wp->resist, ELE_SHADOW))
      SET_BIT(mob->skills, MOB_COUNTER);

   if (wp->cast == 0)
      SET_BIT(mob->skills, MOB_CHARGE);
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

   if (invasion_boss_profile < 0)
      return FALSE;
   bp = &boss_profiles[invasion_boss_profile];

   if (mob->position != POS_FIGHTING)
      return FALSE;
   if (number_percent() < 40)
      return FALSE; /* 60% chance */

   victim = mob->fighting;
   if (victim == NULL || IS_NPC(victim))
      return FALSE;

   for (spell_count = 0; bp->spells[spell_count] != NULL; spell_count++)
      ;
   if (spell_count == 0)
      return FALSE;

   idx = number_range(0, spell_count - 1);
   sn = skill_lookup((char *)bp->spells[idx]);
   if (sn < 0)
      return FALSE;

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
 * room (INVASION_SPAWN_VNUM / room 1209) via h_find_dir.  Rooms that have
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
   AREA_DATA *pArea;
   BUILD_DATA_LIST *pList;
   ROOM_INDEX_DATA *chosen = NULL;
   ROOM_INDEX_DATA *target_room = NULL;
   int n = 0; /* total qualifying rooms seen so far */
   int lo = boss_level - INVASION_BOSS_AREA_SLACK;
   int hi = boss_level + INVASION_BOSS_AREA_SLACK;

   /* Grab Gertrude's room once up front. */
   target_room = get_room_index(INVASION_SPAWN_VNUM); /* vnum 1209 */
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
         if (room == NULL)
            continue;

         /*
          * Reachability check: verify there is at least one valid exit
          * direction from this room toward the target (room 1209).
          * h_find_dir returns >= 0 when a path exists, -1 when it does not.
          */
         if (!room_is_valid_boss_spawn(room, target_room))
            continue; /* safe or no path to Gertrude – skip this room */

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
       "falling back to INVASION_START_VNUM.",
       boss_level);
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
      if (room == NULL)
         continue;

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
                                     bool allow_wide_area_spawn, int spawns_this_reset)
{
   ROOM_INDEX_DATA *spawn_room;
   MOB_INDEX_DATA *pIndex;
   CHAR_DATA *mob;
   const char *creature_name;
   char name_buf[MAX_STRING_LENGTH];
   char short_buf[MAX_STRING_LENGTH];
   char long_buf[MAX_STRING_LENGTH];

#define INVASION_TEMPLATE_MOB 32565 /* <-- set to a real mob vnum in your world */

   if ((pIndex = get_mob_index(INVASION_TEMPLATE_MOB)) == NULL)
   {
      bug("spawn_invasion_mob: template mob vnum %d not found.", INVASION_TEMPLATE_MOB);
      return NULL;
   }

   /* Bosses spawn by level. Wave spawn placement is phased per respawn count:
    * 1st in boss room, 2nd-9th in boss area, 10th+ wide by level. */
   if (is_boss)
   {
      spawn_room = pick_boss_room(level);
   }
   else
   {
      int spawn_mode = invasion_spawn_mode_for_respawn_index(spawns_this_reset);

      if (spawn_mode == 0 && invasion_boss != NULL && !invasion_boss->is_free &&
          invasion_boss->in_room != NULL)
      {
         spawn_room = invasion_boss->in_room;
      }
      else if ((allow_wide_area_spawn || spawn_mode == 2))
      {
         spawn_room = pick_boss_room(level);
      }
      else if (invasion_boss != NULL && !invasion_boss->is_free && invasion_boss->in_room != NULL &&
               invasion_boss->in_room->area != NULL)
      {
         spawn_room = pick_random_room_in_area(invasion_boss->in_room->area);
      }
      else
      {
         spawn_room = get_room_index(INVASION_START_VNUM);
      }
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
      wave_idx = number_range(0, NUM_INVASION_MOBS - 1);
      creature_name = invasion_wave_profiles[wave_idx].name;
   }

   mob = create_mobile(pIndex);
   if (mob == NULL)
      return NULL;

   if (mob->name)
      free_string(mob->name);
   if (mob->short_descr)
      free_string(mob->short_descr);
   if (mob->long_descr)
      free_string(mob->long_descr);

   if (is_boss)
   {
      const BOSS_PROFILE *bp = &boss_profiles[prof_idx];
      snprintf(name_buf, sizeof(name_buf), "%s invasion boss", creature_name);
      snprintf(short_buf, sizeof(short_buf), "%s%s@@N", bp->short_col, creature_name);
      snprintf(long_buf, sizeof(long_buf),
               "%s%s@@N stands here, radiating a terrifying aura of power.\n\r", bp->short_col,
               creature_name);
   }
   else
   {
      const WAVE_MOB_PROFILE *wp = &invasion_wave_profiles[wave_idx];
      snprintf(name_buf, sizeof(name_buf), "%s invader", creature_name);
      snprintf(short_buf, sizeof(short_buf), "%s%s invader@@N", wp->colour, creature_name);
      snprintf(long_buf, sizeof(long_buf), "%s%s@@N\n\r", wp->colour, wp->long_descr);
   }

   mob->name = str_dup(name_buf);
   mob->short_descr = str_dup(short_buf);
   mob->long_descr = str_dup(long_buf);

   mob->level = URANGE(1, level, MAX_LEVEL);

   REMOVE_BIT(mob->act, ACT_AGGRESSIVE);
   SET_BIT(mob->act, ACT_SENTINEL);
   SET_BIT(mob->act, ACT_NOASSIST);
   SET_BIT(mob->act, ACT_UNDEAD);  /* all invasion mobs are undead */
   SET_BIT(mob->act, ACT_NO_HUNT); /* cannot be hunted */

   if (is_boss)
      apply_boss_profile(mob, prof_idx, level);
   else
      apply_wave_mob_profile(mob, wave_idx, level,
                             invasion_wave_progress_tier_for_respawn_index(spawns_this_reset));

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
   if (player_count < 1)
      return;

   prof = number_range(0, NUM_BOSSES - 1);
   boss_level = hi + INVASION_BOSS_OFFSET;

   invasion_boss_profile = prof;
   invasion_active = TRUE;
   invasion_boss_ticks_up = 0;
   invasion_room_ticks = 0;
   invasion_wave_respawns = 0;
   invasion_gertrude_explosions = 0;
   invasion_last_trash_talk_profile = -1;
   invasion_last_trash_talk_line = -1;
   invasion_pending_trash_talks = 0;

   invasion_boss = spawn_invasion_mob(boss_level, TRUE, prof, FALSE, -1);
   if (invasion_boss == NULL)
   {
      bug("invasion_start: failed to spawn boss.", 0);
      invasion_active = FALSE;
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
         if (d->connected != CON_PLAYING)
            continue;
         ch = d->character;
         if (ch == NULL || IS_NPC(ch) || IS_IMMORTAL(ch))
            continue;
         ch->quest_points += INVASION_QP_REWARD;
         ch->pcdata->invasion_points += 1;
         snprintf(buf, sizeof(buf),
                  "You receive @@Y%d quest points@@N and @@Y1 invasion point@@N for defending the "
                  "realm!\n\r",
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
   invasion_active = FALSE;
   invasion_boss_ticks_up = 0;
   invasion_room_ticks = 0;
   invasion_wave_respawns = 0;
   invasion_gertrude_explosions = 0;
   invasion_last_trash_talk_profile = -1;
   invasion_last_trash_talk_line = -1;
   invasion_pending_trash_talks = 0;
   invasion_timer = INVASION_MIN_INTERVAL;
}

/* -----------------------------------------------------------------------
 * invasion_update() — called once per PULSE_TICK from update_handler()
 * --------------------------------------------------------------------- */
void invasion_update(void)
{
   int lo = 1, hi = 1, count, lo_lvl, hi_lvl, i;
   int spawns_this_reset = 0;

   if (invasion_timer > 0)
   {
      invasion_timer--;
      return;
   }

   if (!invasion_active)
   {
      count = count_regular_players(&lo, &hi);
      if (count < 1)
         return;
      if (number_percent() > 5)
         return;
      invasion_start();
      return;
   }

   count = count_regular_players(&lo, &hi);

   if (count < 1)
   {
      despawn_all_invasion();
      invasion_boss_profile = -1;
      invasion_active = FALSE;
      invasion_room_ticks = 0;
      invasion_wave_respawns = 0;
      invasion_gertrude_explosions = 0;
      invasion_last_trash_talk_profile = -1;
      invasion_last_trash_talk_line = -1;
      invasion_pending_trash_talks = 0;
      invasion_timer = INVASION_MIN_INTERVAL;
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
      boss_spawn_count = invasion_boss_spawn_count_for_tick(invasion_boss_ticks_up);
      scaled_boss_spawn_count = invasion_scaled_spawn_count(boss_spawn_count, count);

      for (i = 0; i < scaled_boss_spawn_count; i++)
      {
         bool allow_wide_area_spawn = invasion_is_wide_area_spawn_eligible(spawns_this_reset);

         if (spawn_invasion_mob(number_range(lo, hi), FALSE, -1, allow_wide_area_spawn,
                                spawns_this_reset) != NULL)
         {
            spawns_this_reset++;
            invasion_should_boss_trash_talk_after_respawn();
         }
      }
   }

   if (invasion_boss != NULL && !invasion_boss->is_free && invasion_boss->fighting != NULL)
   {
      invasion_boss_cast(invasion_boss);
   }

   lo_lvl = URANGE(1, lo + INVASION_MOB_MIN_OFFSET, MAX_LEVEL);
   hi_lvl = URANGE(lo_lvl, hi + INVASION_MOB_MAX_OFFSET, MAX_LEVEL);

   for (i = 0; i < invasion_scaled_spawn_count(number_range(0, 2), count); i++)
   {
      bool allow_wide_area_spawn = invasion_is_wide_area_spawn_eligible(spawns_this_reset);

      if (spawn_invasion_mob(number_range(lo_lvl, hi_lvl), FALSE, -1, allow_wide_area_spawn,
                             spawns_this_reset) != NULL)
      {
         spawns_this_reset++;
         invasion_should_boss_trash_talk_after_respawn();
      }
   }
}

void invasion_note_crusade_activity(void)
{
   invasion_crusade_activity_since_last_room_tick = TRUE;
}

/* -----------------------------------------------------------------------
 * invasion_rooms_update() — called once per PULSE_ROOMS
 * --------------------------------------------------------------------- */
void invasion_rooms_update(void)
{
   CHAR_DATA *ch, *ch_next;
   ROOM_INDEX_DATA *target_room;
   CHAR_DATA *gertrude;
   sh_int dir;
   bool had_explosion_this_tick = FALSE;

   if (!invasion_active)
      return;
   if (!invasion_should_advance_on_room_tick())
      return;

   if (invasion_boss != NULL && !invasion_boss->is_free &&
       invasion_should_emit_pending_boss_trash_talk(invasion_pending_trash_talks, 1,
                                                    invasion_crusade_activity_since_last_room_tick))
   {
      invasion_boss_trash_talk();
      invasion_pending_trash_talks--;
   }

   invasion_crusade_activity_since_last_room_tick = FALSE;

   target_room = get_room_index(INVASION_SPAWN_VNUM);
   if (!target_room)
      return;

   gertrude = find_gertrude();

   for (ch = first_char; ch != NULL; ch = ch_next)
   {
      ch_next = ch->next;

      if (!mob_is_invasion_mob(ch))
         continue;
      if (ch == invasion_boss)
         continue; /* boss does not march */
      if (ch->in_room == NULL)
         continue;
      if (invasion_should_explode_at_spawn_room(ch->in_room->vnum))
      {
         had_explosion_this_tick = TRUE;

         if (gertrude != NULL)
         {
            act("$n reaches Gertrude, explodes in a violent burst, and crumbles away!", ch, NULL,
                gertrude, TO_ROOM);
         }

         REMOVE_BIT(ch->act, ACT_INVASION);
         extract_char(ch, TRUE);
         continue;
      }

      if (is_fighting(ch))
         continue;
      if (ch->position < POS_STANDING)
         continue;

      dir = h_find_dir(ch->in_room, target_room,
                       HUNT_WORLD | HUNT_OPENDOOR | HUNT_UNLOCKDOOR | HUNT_PICKDOOR);
      if (dir < 0)
      {
         act("$n cannot find a path to Gertrude and crumbles into dust.", ch, NULL, NULL, TO_ROOM);
         REMOVE_BIT(ch->act, ACT_INVASION);
         extract_char(ch, TRUE);
         continue;
      }

      {
         EXIT_DATA *pexit = ch->in_room->exit[dir];
         char door_arg[MAX_INPUT_LENGTH];

         if (pexit == NULL)
            continue;

         if (IS_SET(pexit->exit_info, EX_CLOSED))
         {
            const char *arg =
                invasion_door_command_argument(pexit->keyword, dir, door_arg, sizeof(door_arg));

            if (IS_SET(pexit->exit_info, EX_LOCKED))
            {
               do_unlock(ch, (char *)arg);

               if (IS_SET(pexit->exit_info, EX_LOCKED))
                  invasion_force_unlock_exit(ch, pexit);
            }

            if (!IS_SET(pexit->exit_info, EX_LOCKED) && IS_SET(pexit->exit_info, EX_CLOSED))
               do_open(ch, (char *)arg);
         }

         if (!IS_SET(pexit->exit_info, EX_CLOSED))
            move_char(ch, dir);
      }
   }

   if (had_explosion_this_tick)
   {
      const char *quest_msg;

      invasion_gertrude_explosions =
          invasion_gertrude_explosions_after_tick(invasion_gertrude_explosions, 1);

      quest_msg = invasion_gertrude_quest_message_for_explosions(invasion_gertrude_explosions);
      if (quest_msg != NULL && gertrude != NULL && !gertrude->is_free)
         do_invasion_talk(gertrude, (char *)quest_msg);

      if (invasion_gertrude_should_fall_for_explosions(invasion_gertrude_explosions))
      {
         if (gertrude == NULL || gertrude->is_free)
            gertrude = find_gertrude();

         if (gertrude != NULL && !gertrude->is_free)
            raw_kill(gertrude, "");

         if (invasion_active)
            invasion_end(FALSE);
      }
   }
}

/* -----------------------------------------------------------------------
 * invasion_on_death() — hook called just BEFORE extract_char in raw_kill()
 * --------------------------------------------------------------------- */
void invasion_on_death(CHAR_DATA *ch, CHAR_DATA *killer)
{
   if (!invasion_active)
      return;

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
   int i;

   one_argument(argument, arg);

   if (!IS_IMMORTAL(ch))
   {
      send_to_char("Huh?\n\r", ch);
      return;
   }

   if (!str_cmp(arg, "start"))
   {
      if (invasion_active)
      {
         send_to_char("An invasion is already underway.\n\r", ch);
         return;
      }
      invasion_timer = 0;
      invasion_start();
      send_to_char("Invasion started.\n\r", ch);
   }
   else if (!str_cmp(arg, "stop"))
   {
      if (!invasion_active)
      {
         send_to_char("No invasion is currently active.\n\r", ch);
         return;
      }
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
               "Player levels   : %d - %d (pseudo)\n\r"
               "Gertrude hits   : %d / 20\n\r",
               invasion_active ? "@@GYES@@N" : "@@RNO@@N",
               (invasion_boss != NULL && !invasion_boss->is_free) ? "@@GYES@@N" : "@@RNO@@N",
               invasion_boss_profile >= 0 ? boss_profiles[invasion_boss_profile].name : "none",
               invasion_timer, lo, hi, invasion_gertrude_explosions);
      send_to_char(buf, ch);
   }
   else if (!str_cmp(arg, "bosses"))
   {
#define ELE_NAME(bits, flag, label)                                                                \
   if (IS_SET(bits, flag))                                                                         \
   {                                                                                               \
      strcat(tmp, label);                                                                          \
      strcat(tmp, " ");                                                                            \
   }

      send_to_char("@@W=== Invasion Boss Roster ===@@N\n\r", ch);
      for (i = 0; i < NUM_BOSSES; i++)
      {
         const BOSS_PROFILE *bp = &boss_profiles[i];
         char res[128] = "", sus[128] = "", tmp[128];
         int j;

         tmp[0] = '\0';
         ELE_NAME(bp->resist, ELE_FIRE, "Fire")
         ELE_NAME(bp->resist, ELE_WATER, "Water")
         ELE_NAME(bp->resist, ELE_AIR, "Air")
         ELE_NAME(bp->resist, ELE_EARTH, "Earth")
         ELE_NAME(bp->resist, ELE_SHADOW, "Shadow")
         ELE_NAME(bp->resist, ELE_MENTAL, "Mental")
         ELE_NAME(bp->resist, ELE_HOLY, "Holy")
         ELE_NAME(bp->resist, ELE_POISON, "Poison")
         ELE_NAME(bp->resist, ELE_PHYSICAL, "Physical")
         snprintf(res, sizeof(res), "%s", tmp[0] ? tmp : "none");

         tmp[0] = '\0';
         ELE_NAME(bp->suscept, ELE_FIRE, "Fire")
         ELE_NAME(bp->suscept, ELE_WATER, "Water")
         ELE_NAME(bp->suscept, ELE_AIR, "Air")
         ELE_NAME(bp->suscept, ELE_EARTH, "Earth")
         ELE_NAME(bp->suscept, ELE_SHADOW, "Shadow")
         ELE_NAME(bp->suscept, ELE_MENTAL, "Mental")
         ELE_NAME(bp->suscept, ELE_HOLY, "Holy")
         ELE_NAME(bp->suscept, ELE_POISON, "Poison")
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
