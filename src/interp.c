/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

#ifndef DEC_MAGIC_H
#include "magic.h"
#endif

bool check_social args((CHAR_DATA * ch, char *command, char *argument));
bool MP_Commands args((CHAR_DATA * ch));

/*
 * Command logging types.
 */
#define LOG_NORMAL 0
#define LOG_ALWAYS 1
#define LOG_NEVER 2

/*
 * Log-all switch.
 */
bool fLogAll = FALSE;

/*
 * Convenience macros for the command table.
 * CMD       - normal command (flags = 0)
 * CMD_NINJA - command that does not break ninja stance (flags = CMD_FLAG_NINJA_OK)
 */
#define CMD(name, fun, pos, lvl, log, type, show)                                                  \
   {                                                                                               \
      name, fun, pos, lvl, log, type, show, 0                                                      \
   }
#define CMD_NINJA(name, fun, pos, lvl, log, type, show)                                            \
   {                                                                                               \
      name, fun, pos, lvl, log, type, show, CMD_FLAG_NINJA_OK                                      \
   }

/*
 * Command table.
 */
const struct cmd_type cmd_table[] = {
    /*
     * Common movement commands.
     */

    /*
     * command table modified by Aeria
     */
    CMD("north", do_north, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("east", do_east, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("south", do_south, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("west", do_west, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("up", do_up, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("down", do_down, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("map", do_mapper, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    CMD("buy", do_buy, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("cast", do_cast, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("exits", do_exits, POS_RESTING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("get", do_get, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("gain", do_gain, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("inventory", do_inventory, POS_DEAD, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD_NINJA("kill", do_kill, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("look", do_look, POS_RESTING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("rest", do_rest, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("sleep", do_sleep, POS_SLEEPING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("stand", do_stand, POS_SLEEPING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("tell", do_tell, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD_NINJA("whisper", do_whisper, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("wield", do_wear, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("wizhelp", do_wizhelp, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("loot", do_loot, POS_STANDING, 20, LOG_ALWAYS, C_TYPE_OBJECT, C_SHOW_ALWAYS),

    /*
     * Informational commands.
     */

    CMD("test", do_test, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),

    CMD("affected", do_affected, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("areas", do_areas, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("appraise", do_appraise, POS_RESTING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("bug", do_bug, POS_DEAD, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),
    CMD("clist", do_clan_list, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("commands", do_commands, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("compare", do_compare, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("consider", do_consider, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("credits", do_credits, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("cwhere", do_cwhere, POS_DEAD, CLAN_ONLY, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("delete", do_delete, POS_STANDING, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("diagnose", do_diagnose, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("equipment", do_equipment, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("examine", do_examine, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("help", do_help, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("rhelp", do_rhelp, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("sthelp", do_stancehelp, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("rmlist", do_rmodhelp, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("shelp", do_shelp, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("lore", do_lore, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("heal", do_heal, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("idea", do_idea, POS_DEAD, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),

    CMD("note", do_note, POS_RESTING, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),

    CMD("report", do_report, POS_DEAD, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("pagelength", do_pagelen, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("players", do_players, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("read", do_read, POS_STANDING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("rlist", do_race_list, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("score", do_score, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("slist", do_slist, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("socials", do_socials, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("status", do_status, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("time", do_time, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("typo", do_typo, POS_DEAD, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),
    CMD("weather", do_weather, POS_RESTING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("who", do_who, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("whois", do_whois, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("wizlist", do_wizlist, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("write", do_write, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("edit", do_edit, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),

    /*
     * Configuration commands.
     */

    CMD("accept", do_accept, POS_DEAD, CLAN_ONLY, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("alias", do_alias, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("auto", do_auto, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("autoexit", do_autoexit, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("autoloot", do_autoloot, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("autosac", do_autosac, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("banish", do_banish, POS_DEAD, CLAN_ONLY, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("blank", do_blank, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("brief", do_brief, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("channels", do_channels, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("color", do_color, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("colist", do_colist, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("combine", do_combine, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("config", do_config, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("description", do_description, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("ignore", do_ignore, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("password", do_password, POS_DEAD, 0, LOG_NEVER, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("prompt", do_prompt, POS_DEAD, 0, LOG_ALWAYS, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("title", do_title, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("wimpy", do_wimpy, POS_DEAD, 0, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),
    CMD("worth", do_worth, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("stance", do_stance, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("email", do_email, POS_RESTING, 20, LOG_NORMAL, C_TYPE_CONFIG, C_SHOW_ALWAYS),

    /*
     * Communication commands.
     */

    CMD("answer", do_answer, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("ask", do_ask, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("beep", do_beep, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("clan", do_clan, POS_RESTING, CLAN_ONLY, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("creator", do_creator, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD(")", do_creator, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD(".", do_gossip, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("gossip", do_gossip, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("pemote", do_pemote, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("emote", do_emote, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD(",", do_emote, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("flame", do_flame, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("finger", do_finger, POS_DEAD, 86, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("gtell", do_gtell, POS_DEAD, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD(";", do_gtell, POS_DEAD, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("music", do_music, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("newbie", do_newbie, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("pose", do_pose, POS_RESTING, 86, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    /* one or more of the poses crash us  */
    CMD("pray", do_pray, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("iinvasion", do_invasion, POS_DEAD, LEVEL_IMMORTAL, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("invasion", do_invasion_talk, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("question", do_question, POS_SLEEPING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("race", do_race, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("reply", do_reply, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("say", do_say, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("'", do_say, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("shout", do_shout, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("tongue", do_tongue, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("yell", do_yell, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("zzz", do_zzz, POS_SLEEPING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("game", do_game, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("{", do_remorttalk, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("diplomat", do_diptalk, POS_RESTING, CLAN_ONLY, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("crusade", do_crusade, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("adept", do_adepttalk, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    CMD("ooc", do_ooc, POS_RESTING, 0, LOG_NORMAL, C_TYPE_COMM, C_SHOW_ALWAYS),
    /*
     * Combat commands.
     */
    CMD("assist", do_assist, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD_NINJA("backstab", do_backstab, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD_NINJA("bs", do_backstab, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("dirt", do_dirt, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("trip", do_trip, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("smash", do_smash, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("bash", do_bash, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("berserk", do_beserk, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("circle", do_circle, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("detox", do_detox, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("disarm", do_disarm, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("flee", do_flee, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("headbutt", do_headbutt, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("kick", do_kick, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("knee", do_knee, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("leadership", do_leadership, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("morale", do_morale, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("murde", do_murde, POS_FIGHTING, 5, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_NEVER),
    CMD_NINJA("murder", do_murder, POS_FIGHTING, 5, LOG_ALWAYS, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("palmstrike", do_palmstrike, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("poison:arsenic", do_poison_arsenic, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("poison:quinine", do_poison_quinine, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("poison:nightshade", do_poison_nightshade, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("punch", do_punch, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("rescue", do_rescue, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("revenant", do_revenant, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("stun", do_stun, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("frenzy", do_frenzy, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("charge", do_charge, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("target", do_target, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("warcry", do_warcry, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("riposte", do_riposte, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("anti magic shell", do_anti_magic_shell, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("chi surge", do_chi_surge, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("breath of endurance", do_breath_of_endurance, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("fist of the interior form", do_fist_of_the_interior_form, POS_FIGHTING, 0, LOG_NORMAL,
        C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("momentum chain", do_momentum_chain, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("iron resolve", do_iron_resolve, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("overwhelming assault", do_overwhelming_assault, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("oathshield", do_oathshield, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("sanctified strike", do_sanctified_strike, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("shadow reading", do_shadow_reading, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("hex ward", do_hex_ward, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("reflex disruption", do_reflex_disruption, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("holystrike", do_holystrike, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("shieldblock", do_shieldblock, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("chiblock", do_chiblock, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("chakra", do_chakra, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("spinfist", do_spinfist, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("pummel", do_pummel, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("aurabolt", do_aurabolt, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("phantomfist", do_phantomfist, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("mindoverbody", do_mindoverbody, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("flurry", do_flurry, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("fleche", do_fleche, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    /* Cipher skills */
    CMD_NINJA("garrote", do_garrote, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("feign death", do_feign_death, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("shadow step", do_shadow_step, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("mark target", do_mark_target, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("conceal", do_conceal, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("set trap", do_set_trap, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("read intent", do_read_intent, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    /* Warden skills */
    CMD("cleave", do_cleave, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("fortify", do_fortify, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("taunt", do_taunt, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("rend", do_rend, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("field patch", do_field_patch, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("weapon mastery", do_weapon_mastery, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    /* Pugilist skills */
    CMD("grapple", do_grapple, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("choke hold", do_choke_hold, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("leg sweep", do_leg_sweep, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("iron skin", do_iron_skin, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("roll with blow", do_roll_with_blow, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("pressure point", do_pressure_point, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_SKILL),
    CMD("feint", do_feint, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("conditioning", do_conditioning, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),

    /*
     * Object manipulation commands.
     */

    CMD("adapt", do_adapt, POS_STANDING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("auction", do_auction, POS_STANDING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("bid", do_bid, POS_STANDING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("brandish", do_brandish, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("cdonate", do_cdonate, POS_RESTING, CLAN_ONLY, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("close", do_close, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("clutch", do_clutch, POS_STANDING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("donate", do_donate, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("drop", do_drop, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("eat", do_eat, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("enter", do_enter, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("give", do_give, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("hold", do_wear, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("list", do_list, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("lock", do_lock, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("open", do_open, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),

    CMD("make", do_make, POS_DEAD, CLAN_ONLY, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("pick", do_pick, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("put", do_put, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("quaff", do_quaff, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("recite", do_recite, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("remove", do_remove, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("sedit", do_sedit, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_CONFIG, C_SHOW_NEVER),
    CMD("take", do_get, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("sacrifice", do_sacrifice, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("junk", do_junk, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("unlock", do_unlock, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("value", do_value, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("wear", do_wear, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("enchant", do_enchant, POS_STANDING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),
    CMD("assemble", do_connect, POS_RESTING, 0, LOG_NORMAL, C_TYPE_OBJECT, C_SHOW_ALWAYS),

    CMD("afk", do_afk, POS_DEAD, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),
    CMD("assassinate", do_assassinate, POS_STANDING, 20, LOG_ALWAYS, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("follow", do_follow, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("gold", do_gold, POS_DEAD, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("group", do_group, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("guild", do_guild, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_NEVER),
    CMD("hide", do_hide, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("hunt", do_hunt, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("keep", do_keep, POS_DEAD, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("leav", do_leav, POS_RESTING, CLAN_ONLY, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_NEVER),
    CMD("leave", do_leave, POS_RESTING, CLAN_ONLY, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("qui", do_qui, POS_DEAD, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_NEVER),
    CMD("quit", do_quit, POS_DEAD, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),
    CMD("recall", do_recall, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("/", do_recall, POS_FIGHTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("home", do_clan_recall, POS_STANDING, CLAN_ONLY, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("rent", do_rent, POS_DEAD, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_NEVER),
    CMD("save", do_save, POS_DEAD, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),
    CMD("scan", do_scan, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("shadowform", do_shadowform, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("sneak", do_sneak, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("spells", do_spells, POS_SLEEPING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("split", do_split, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD_NINJA("steal", do_steal, POS_STANDING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("train", do_train, POS_RESTING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("visible", do_visible, POS_SLEEPING, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),
    CMD("wake", do_wake, POS_SLEEPING, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("where", do_where, POS_RESTING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),

    CMD("disguise", do_disguise, POS_STANDING, 0, LOG_ALWAYS, C_TYPE_ACTION, C_SHOW_SKILL),
    CMD("politics", do_politics, POS_RESTING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),
    CMD("rulers", do_rulers, POS_RESTING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),

    CMD("learned", do_learned, POS_SLEEPING, 0, LOG_NORMAL, C_TYPE_INFO, C_SHOW_ALWAYS),

    CMD("scout", do_scout, POS_STANDING, 1, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_SKILL),

    /*
     * NEW CLAN COMMANDS Zen
     */

    CMD("ctoggle", do_ctoggle, POS_RESTING, BOSS_ONLY, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),
    CMD("negotiate", do_negotiate, POS_RESTING, CLAN_ONLY, LOG_NORMAL, C_TYPE_ACTION,
        C_SHOW_ALWAYS),
    CMD("council", do_council, POS_RESTING, 0, LOG_NORMAL, C_TYPE_MISC, C_SHOW_NEVER),
    CMD("qpspend", do_qpspend, POS_STANDING, 1, LOG_NORMAL, C_TYPE_MISC, C_SHOW_ALWAYS),

    /*
     * Immortal commands.
     */

    CMD("alist", build_arealist, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("alink", do_alink, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("build", do_build, POS_STANDING, 2, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("mpcr", do_mpcr, POS_STANDING, L_DEI, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("halls", do_halls, POS_RESTING, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("lhunt", do_lhunt, POS_DEAD, L_SUP, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("sstat", do_sstat, POS_DEAD, L_SUP, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("cset", do_cset, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("whoname", do_whoname, POS_DEAD, L_ANG, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("sysdata", do_sysdata, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("clutchinfo", do_clutchinfo, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("advance", do_advance, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("setclass", do_setclass, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("doggy", do_dog, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("togbuild", do_togbuild, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("togleader", do_togleader, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("isnoop", do_isnoop, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("trust", do_trust, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("deathmatc", do_deathmatc, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER),
    CMD("deathmatch", do_deathmatch, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("resetpasswd", do_resetpassword, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("iscore", do_iscore, POS_DEAD, L_DEI, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("iwhere", do_iwhere, POS_DEAD, L_DEI, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("fights", do_fights, POS_DEAD, L_DEI, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("allow", do_allow, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("iban", do_ban, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("deny", do_deny, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("freeze", do_freeze, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("reboo", do_reboo, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER),
    CMD("reboot", do_reboot, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("hotreboo", do_hotreboo, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_NEVER),
    CMD("hotreboot", do_hotreboot, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("shutdow", do_shutdow, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_NEVER),
    CMD("shutdown", do_shutdown, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("users", do_users, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("wizify", do_wizify, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("wizlock", do_wizlock, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("imtlset", do_imtlset, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("for", do_for, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("force", do_force, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("log", do_log, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("mload", do_mload, POS_DEAD, L_HER, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("monitor", do_monitor, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("areasave", do_areasave, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("mset", do_mset, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    /*{"mgive", do_mgive, POS_DEAD, L_DEI, LOG_ALWAYS,
     C_TYPE_IMM, C_SHOW_ALWAYS},*/
    CMD("noemote", do_noemote, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("nopray", do_nopray, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("notell", do_notell, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("oload", do_oload, POS_DEAD, L_HER, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("oset", do_oset, POS_DEAD, L_ANG, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("owhere", do_owhere, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("pardon", do_pardon, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("peace", do_peace, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("purge", do_purge, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("iquest", do_iquest, POS_DEAD, L_DEI, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("restore", do_restore, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("rset", do_rset, POS_DEAD, 86, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("silence", do_silence, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("sla", do_sla, POS_DEAD, 86, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("slay", do_slay, POS_DEAD, L_HER, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("snoop", do_snoop, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("sset", do_sset, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("transfer", do_transfer, POS_DEAD, L_HER, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("transdm", do_transdm, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),

    CMD("at", do_at, POS_DEAD, L_DEI, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("bamfin", do_bamfin, POS_DEAD, L_ANG, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("bamfout", do_bamfout, POS_DEAD, L_ANG, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("echo", do_echo, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("goto", do_goto, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("holylight", do_holylight, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("invis", do_invis, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("memory", do_memory, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("mfind", do_mfind, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("mfindlev", do_mfindlev, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("mstat", do_mstat, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("mwhere", do_mwhere, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("ofind", do_ofind, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("ostat", do_ostat, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("recho", do_recho, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("return", do_return, POS_DEAD, L_DEI, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("rstat", do_rstat, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("slookup", do_slookup, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("switch", do_switch, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("immtalk", do_immtalk, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD(":", do_immtalk, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("reward", do_reward, POS_DEAD, L_DEI, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("setcouncil", do_togcouncil, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("resetgain", do_gain_stat_reset, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("exlist", do_exlist, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("vlist", do_vlist, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("olmsg", do_olmsg, POS_DEAD, L_HER, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("ofindlev", do_ofindlev, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("prename", do_rename, POS_DEAD, L_GOD, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("pdelete", do_sdelete, POS_DEAD, 2, LOG_ALWAYS, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("scheck", do_scheck, POS_DEAD, L_GOD, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("immbrand", do_immbrand, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("otype", do_otype, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("owear", do_owear, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    CMD("findreset", do_findreset, POS_DEAD, L_HER, LOG_NORMAL, C_TYPE_IMM, C_SHOW_ALWAYS),
    /*
     * program commands.
     */

    /* Reincarnate */
    CMD("reincarnate", do_reincarnate, POS_STANDING, 0, LOG_ALWAYS, C_TYPE_ACTION, C_SHOW_ALWAYS),

    /* Quest */
    CMD("quest", do_quest, POS_STANDING, 1, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_ALWAYS),

    /*
     * End of list.
     */
    CMD("", 0, POS_DEAD, 0, LOG_NORMAL, C_TYPE_ACTION, C_SHOW_NEVER)

    /*
     * Immortal commands.
     */

};

/* Log all commands.. rewrite every 5 mins.. */
void comlog(CHAR_DATA *ch, int cmd, char *args)
{
   static FILE *fplog;
   static time_t ltime;

   if (!fplog || ltime + (5 * 60) <= current_time)
   {
      if (fplog != NULL)
      {
         fclose(fplog);
         fplog = NULL;
      }
      if (!(fplog = fopen("../log/comlog.txt", "w")))
         return;
      ltime = current_time;
   }
   fprintf(fplog, "%.24s :: %12.12s (%5d): %s %s\n", ctime(&current_time), ch->name,
           (IS_NPC(ch) ? ch->pIndexData->vnum : -1), cmd_table[cmd].name,
           (cmd_table[cmd].log == LOG_NEVER ? "XXX" : args));
   fflush(fplog);
}

/*
 * Returns TRUE if ch is allowed to use cmd_table[cmd].
 */
static bool cmd_allows(CHAR_DATA *ch, int cmd, int trust)
{
   if (cmd_table[cmd].level == CLAN_ONLY && !IS_NPC(ch) && ch->pcdata->clan == 0)
      return FALSE;

   if (cmd_table[cmd].level == BOSS_ONLY && !IS_NPC(ch) &&
       !IS_SET(ch->pcdata->pflags, PFLAG_CLAN_BOSS))
      return FALSE;

   return cmd_table[cmd].level <= trust || MP_Commands(ch);
}

/*
 * Searches the command table for a match.
 * exact=TRUE  requires a full name match.
 * exact=FALSE allows prefix matching.
 * Returns the table index on success, -1 if not found.
 */
static int find_command(CHAR_DATA *ch, const char *word, bool exact)
{
   int trust = get_trust(ch);
   int i;

   for (i = 0; cmd_table[i].name[0] != '\0'; i++)
   {
      if (cmd_table[i].name[0] != word[0])
         continue;
      if (!cmd_allows(ch, i, trust))
         continue;
      if (exact ? !str_cmp(word, cmd_table[i].name) : !str_prefix(word, cmd_table[i].name))
         return i;
   }
   return -1;
}

/*
 * Position-rejection messages, indexed by POS_* constant.
 */
static const char *const pos_msgs[] = {
    /* POS_DEAD     0 */ "Lie still; you are @@dDEAD@@N.\n\r",
    /* POS_MORTAL   1 */ "You are @@Rhurt@@N far too bad for that.\n\r",
    /* POS_INCAP    2 */ "You are @@Rhurt@@N far too bad for that.\n\r",
    /* POS_STUNNED  3 */ "You are too @@estunned@@N to do that.\n\r",
    /* POS_SLEEPING 4 */ "Oh, go back to @@Wsleep!@@N\n\r",
    /* POS_RESTING  5 */ "Naaaaaah... You feel too @@brelaxed@@N...\n\r",
    /* POS_FIGHTING 6 */ "Not until you @@Rstop@@N fighting!\n\r",
};

/*
 * Internal recursive entry point for interpret().
 * alias_depth tracks alias expansion depth to prevent infinite loops.
 */
static void interpret_r(CHAR_DATA *ch, char *argument, int alias_depth)
{
   char command[MAX_INPUT_LENGTH];
   char logline[MAX_INPUT_LENGTH];
   int cmd;

   if (ch->position == POS_WRITING)
   {
      write_interpret(ch, argument);
      return;
   }

   if (ch->position == POS_BUILDING)
   {
      if (argument[0] == ':')
         argument++;
      else
      {
         build_interpret(ch, argument);
         return;
      }
   }

   /*
    * Strip leading spaces.
    */
   while (isspace(*argument))
      argument++;
   if (argument[0] == '\0')
      return;

   /*
    * No hiding.
    */
   REMOVE_BIT(ch->affected_by, AFF_HIDE);

   /*
    * Implement freeze command.
    */
   if (!IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE))
   {
      send_to_char("@@a@@fYou're totally frozen!@@N\n\r", ch);
      return;
   }

   if (ch->stunTimer > 0)
   {
      send_to_char("You are too@@aSTUNNED@@N to act!\n\r", ch);
      return;
   }

   /*
    * Grab the command word.
    * Special parsing so ' can be a command,
    *   also no spaces needed after punctuation.
    */
   snprintf(logline, sizeof(logline), "%s", argument);
   if (!isalpha(argument[0]) && !isdigit(argument[0]))
   {
      command[0] = argument[0];
      command[1] = '\0';
      argument++;
      while (isspace(*argument))
         argument++;
   }
   else
   {
      argument = one_argument(argument, command);
   }

   /*
    * Look for command in command table.
    * First pass: exact match. Second pass: prefix match.
    */
   cmd = find_command(ch, command, TRUE);
   if (cmd < 0)
      cmd = find_command(ch, command, FALSE);

   /*
    * Log and snoop.
    */
   if (cmd >= 0 && cmd_table[cmd].log == LOG_NEVER)
      snprintf(logline, sizeof(logline), "%s", "XXXXXXXX XXXXXXXX XXXXXXXX@@N");

   if ((!IS_NPC(ch) && IS_SET(ch->act, PLR_LOG)) || fLogAll ||
       (cmd >= 0 && cmd_table[cmd].log == LOG_ALWAYS))
   {
      sprintf(log_buf, "Log %s: %s", ch->name, logline);
      log_string(log_buf);
      if (IS_SET(ch->act, PLR_LOG))
         monitor_chan(log_buf, MONITOR_BAD);
      else if (cmd >= 0 && cmd_table[cmd].level > LEVEL_HERO)
         monitor_chan(log_buf, MONITOR_GEN_IMM);
      else
         monitor_chan(log_buf, MONITOR_GEN_MORT);
   }

   if (ch->desc != NULL && ch->desc->snoop_by != NULL)
   {
      char snp[MAX_STRING_LENGTH];
      sprintf(snp, "[Snoop:%s] %s\n\r", ch->name, logline);
      write_to_buffer(ch->desc->snoop_by, snp, 0);
   }

   /*
    * Check aliases.  Only at depth 0 to prevent infinite loops.
    */
   if (cmd < 0 && !IS_NPC(ch) && alias_depth == 0)
   {
      int cnt;
      char foo[MAX_STRING_LENGTH];

      for (cnt = 0; cnt < MAX_ALIASES; cnt++)
      {
         if (!str_cmp(ch->pcdata->alias_name[cnt], command) &&
             str_cmp(ch->pcdata->alias_name[cnt], "<none>@@N"))
         {
            sprintf(foo, "%s %s", ch->pcdata->alias[cnt], argument);
            interpret_r(ch, foo, alias_depth + 1);
            return;
         }
      }
   }

   if (cmd < 0)
   {
      if (!check_social(ch, command, argument))
         send_to_char("Huh?\n\r", ch);
      return;
   }

   /*
    * Character not in position for command?
    */
   if (ch->position < cmd_table[cmd].position)
   {
      send_to_char(pos_msgs[ch->position], ch);
      return;
   }

   /*
    * Dispatch the command.
    */
   if (!IS_NPC(ch) && ch->stance > 0 && IS_SET(stance_app[ch->stance].specials, STANCE_NINJA) &&
       !IS_SET(cmd_table[cmd].flags, CMD_FLAG_NINJA_OK))
   {
      send_to_char("You step out of the shadows.\n\r", ch);
      ch->stance = 0;
      act("$n steps out of the Shadows!", ch, NULL, NULL, TO_ROOM);
   }
   comlog(ch, cmd, argument);
   (*cmd_table[cmd].do_fun)(ch, argument);

   tail_chain();
}

/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret(CHAR_DATA *ch, char *argument)
{
   interpret_r(ch, argument, 0);
}

bool check_social(CHAR_DATA *ch, char *command, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int cmd;
   bool found;

   found = FALSE;
   for (cmd = 0; social_table[cmd].name[0] != '\0'; cmd++)
   {
      if (command[0] == social_table[cmd].name[0] && !str_prefix(command, social_table[cmd].name))
      {
         found = TRUE;
         break;
      }
   }

   if (!found)
      return FALSE;

   if (!IS_NPC(ch) && IS_SET(ch->act, PLR_NO_EMOTE))
   {
      send_to_char("You are anti-social!\n\r", ch);
      return TRUE;
   }

   switch (ch->position)
   {
   case POS_DEAD:
      send_to_char("Lie still; you are @@dDEAD@@N.\n\r", ch);
      return TRUE;

   case POS_INCAP:
   case POS_MORTAL:
      send_to_char("You are @@Rhurt@@N far too bad for that.\n\r", ch);
      return TRUE;

   case POS_STUNNED:
      send_to_char("You are too @@estunned@@N to do that.\n\r", ch);
      return TRUE;

   case POS_SLEEPING:
      /*
       * I just know this is the path to a 12" 'if' statement.  :(
       * But two players asked for it already!  -- Furey
       */
      if (!str_cmp(social_table[cmd].name, "snore"))
         break;
      send_to_char("In your @@Wdreams@@N, or what?\n\r", ch);
      return TRUE;
   }

   one_argument(argument, arg);
   victim = NULL;
   if (arg[0] == '\0')
   {
      act(social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM);
      act(social_table[cmd].char_no_arg, ch, NULL, victim, TO_CHAR);
   }
   else if ((victim = get_char_room(ch, arg)) == NULL)
   {
      send_to_char("They aren't here.\n\r", ch);
   }
   else if (victim == ch)
   {
      act(social_table[cmd].others_auto, ch, NULL, victim, TO_ROOM);
      act(social_table[cmd].char_auto, ch, NULL, victim, TO_CHAR);
   }
   else
   {
      act(social_table[cmd].others_found, ch, NULL, victim, TO_NOTVICT);
      act(social_table[cmd].char_found, ch, NULL, victim, TO_CHAR);
      act(social_table[cmd].vict_found, ch, NULL, victim, TO_VICT);

      if (!IS_NPC(ch) && IS_NPC(victim) && !IS_AFFECTED(victim, AFF_CHARM) && IS_AWAKE(victim))
      {
         switch (number_bits(4))
         {
         case 0:
         case 1:
         case 2:
         case 3:
         case 4:
            act(social_table[cmd].others_found, victim, NULL, ch, TO_NOTVICT);
            act(social_table[cmd].char_found, victim, NULL, ch, TO_CHAR);
            act(social_table[cmd].vict_found, victim, NULL, ch, TO_VICT);
            break;

         case 5:
         case 6:
         case 7:
         case 8:
         case 9:
         case 10:
         case 11:
         case 12:
            act("$n slaps $N.", victim, NULL, ch, TO_NOTVICT);
            act("You slap $N.", victim, NULL, ch, TO_CHAR);
            act("$n slaps you.", victim, NULL, ch, TO_VICT);
            break;
         }
      }
   }

   return TRUE;
}

bool IS_SWITCHED(CHAR_DATA *ch)
{
   if (!IS_NPC(ch))
      return FALSE;

   if (ch->desc == NULL)
      return FALSE;

   return TRUE;
}

bool MP_Commands(CHAR_DATA *ch) /* Can scripted mobs
                                 * use mpcommands? TRUE if yes.
                                 * - Kahn */
{
   if (IS_SWITCHED(ch))
      return FALSE;

   if (IS_NPC(ch) && ch->pIndexData->progtypes && !IS_AFFECTED(ch, AFF_CHARM))
      return TRUE;

   return FALSE;
}
