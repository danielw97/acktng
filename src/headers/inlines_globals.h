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
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * inlines_globals.h — Static inline functions that reference global tables.
 *
 * Included only when globals.h was actually processed (DEC_GLOBALS_H_LOADED
 * is defined at the end of globals.h).  This guard ensures unit tests that
 * suppress globals.h and provide their own stub tables do not see conflicting
 * extern declarations here.
 *
 * Must be included after all struct definitions (i.e. near the end of ack.h).
 */

#ifndef DEC_INLINES_GLOBALS_H
#define DEC_INLINES_GLOBALS_H 1

/*
 * Extern declarations for the global tables used below.  These are compatible
 * with (and may precede) the declarations in globals.h — duplicate compatible
 * externs are harmless in C.  The declarations here are needed because
 * globals.h includes ack.h internally, meaning this file is processed before
 * globals.h has emitted its own extern lines.
 */
extern const struct skill_type skill_table[];
extern const struct class_type gclass_table[];
extern WEATHER_DATA weather_info;

/* ── skill_table ──────────────────────────────────────────────────────── */

static inline int MANA_COST(const CHAR_DATA *ch, int sn)
{
   return IS_NPC(ch) ? 0 : UMAX(50, skill_table[sn].min_mana);
}

/* ── weather_info ─────────────────────────────────────────────────────── */

static inline bool IS_DAYTIME(void)
{
   return weather_info.sunlight == SUN_RISE || weather_info.sunlight == SUN_LIGHT;
}

static inline bool IS_NIGHTTIME(void)
{
   return !IS_DAYTIME();
}

/* ── gclass_table ─────────────────────────────────────────────────────── */

static inline int CLASS_TIER(int c)
{
   return gclass_table[c].tier;
}

static inline bool IS_MORTAL_CLASS(int c)
{
   return gclass_table[c].tier == MORTAL;
}

static inline bool IS_REMORT_CLASS(int c)
{
   return gclass_table[c].tier == REMORT;
}

static inline bool IS_ADEPT_CLASS(int c)
{
   return gclass_table[c].tier == ADEPT;
}

#endif /* DEC_INLINES_GLOBALS_H */
