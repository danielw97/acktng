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
 * inlines.h — Static inline replacements for function-like macros.
 *
 * This header is included at the end of ack.h, after all struct definitions
 * are complete, so inline function bodies may safely dereference struct fields.
 *
 * Forward declarations here allow the inline functions to reference external
 * functions and global arrays without pulling in additional headers.
 */

#ifndef DEC_INLINES_H
#define DEC_INLINES_H 1

/*
 * Forward declarations for functions called by inline functions below.
 * get_trust and can_see take non-const pointers; IS_STAFF, IS_HERO, and PERS
 * cast their const arguments when delegating to these functions (both are
 * read-only in practice).
 */
int get_trust(CHAR_DATA *ch);
bool can_see(CHAR_DATA *looker, CHAR_DATA *victim);

/* ── Character checks ─────────────────────────────────────────────────── */

static inline bool IS_NPC(const CHAR_DATA *ch)
{
   return IS_SET(ch->act, ACT_IS_NPC) != 0;
}

static inline bool IS_AFFECTED(const CHAR_DATA *ch, int sn)
{
   return IS_SET(ch->affected_by, sn) != 0;
}

static inline bool IS_GOOD(const CHAR_DATA *ch)
{
   return ch->alignment >= 0;
}

static inline bool IS_EVIL(const CHAR_DATA *ch)
{
   return ch->alignment < 0;
}

static inline bool IS_AWAKE(const CHAR_DATA *ch)
{
   return ch->position > POS_SLEEPING;
}

static inline bool IS_OUTSIDE(const CHAR_DATA *ch)
{
   return IS_SET(ch->in_room->room_flags, ROOM_INDOORS) == 0;
}

static inline bool IS_UNDEAD(const CHAR_DATA *ch)
{
   return IS_NPC(ch) && IS_SET(ch->act, ACT_UNDEAD) != 0;
}

/*
 * IS_STAFF / IS_HERO delegate to get_trust() which takes a non-const pointer.
 * The cast is safe: get_trust is read-only with respect to the character.
 */
static inline bool IS_STAFF(const CHAR_DATA *ch)
{
   return get_trust((CHAR_DATA *)ch) >= LEVEL_STAFF;
}

static inline bool IS_HERO(const CHAR_DATA *ch)
{
   return get_trust((CHAR_DATA *)ch) >= LEVEL_HERO;
}

static inline bool PLAYTESTER(const CHAR_DATA *ch)
{
   return !IS_NPC(ch) && IS_SET(ch->pcdata->pflags, PFLAG_TESTER) != 0;
}

static inline bool HAS_BODY(const CHAR_DATA *ch)
{
   return !IS_NPC(ch) || IS_SET(ch->act, ACT_NO_BODY) == 0;
}

static inline bool HAS_MIND(const CHAR_DATA *ch)
{
   return !IS_NPC(ch) || IS_SET(ch->act, ACT_NOMIND) == 0;
}

static inline int ADEPT_LEVEL(const CHAR_DATA *ch)
{
   return IS_NPC(ch) ? ch->level / 7 : ch->adept_level;
}

/* ── Numeric utilities ────────────────────────────────────────────────── */

static inline int UMIN(int a, int b)
{
   return a < b ? a : b;
}

static inline int UMAX(int a, int b)
{
   return a > b ? a : b;
}

static inline int URANGE(int a, int b, int c)
{
   return b < a ? a : (b > c ? c : b);
}

/* ── Character utilities ──────────────────────────────────────────────── */

static inline char LOWER(char c)
{
   return (c >= 'A' && c <= 'Z') ? (char)(c + 'a' - 'A') : c;
}

static inline char UPPER(char c)
{
   return (c >= 'a' && c <= 'z') ? (char)(c + 'A' - 'a') : c;
}

static inline bool IS_LETTER(char c)
{
   return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static inline void WAIT_STATE(CHAR_DATA *ch, int npulse)
{
   ch->wait = UMAX(ch->wait, npulse);
}

/* ── Name / descriptor helpers ────────────────────────────────────────── */

static inline char *NAME(const CHAR_DATA *ch)
{
   return IS_NPC(ch) ? ch->short_descr : ch->name;
}

static inline CHAR_DATA *CH(DESCRIPTOR_DATA *d)
{
   return d->original ? d->original : d->character;
}

/*
 * PERS: delegates to can_see() and IS_STAFF() which take non-const pointers.
 * Returns char * to match the old macro's implicit type (ch->name / short_descr
 * are char *, and string literals used here are not modified in practice).
 */
static inline char *PERS(const CHAR_DATA *ch, CHAR_DATA *looker)
{
   if (can_see(looker, (CHAR_DATA *)ch))
      return IS_NPC(ch) ? ch->short_descr : ch->name;
   return IS_STAFF(ch) ? "A Mystical Being" : "Someone";
}

/* ── Object checks ────────────────────────────────────────────────────── */

static inline bool CAN_WEAR(const OBJ_DATA *obj, int part)
{
   return IS_SET(obj->wear_flags, part) != 0;
}

static inline bool IS_OBJ_STAT(const OBJ_DATA *obj, int stat)
{
   return IS_SET(obj->extra_flags, stat) != 0;
}

static inline bool IS_WEAPON(const OBJ_DATA *eq)
{
   return eq != NULL && eq->item_type == ITEM_WEAPON;
}

static inline bool IS_SHIELD(const OBJ_DATA *eq)
{
   return eq != NULL && eq->item_type == ITEM_ARMOR;
}

static inline bool IS_OBJ_BONDED(const OBJ_DATA *obj)
{
   return IS_SET(obj->extra_flags, ITEM_BONDED) ? TRUE : FALSE;
}

/*
 * IS_DAYTIME, IS_NIGHTTIME, MANA_COST, IS_MORTAL_CLASS, IS_REMORT_CLASS,
 * IS_ADEPT_CLASS, and CLASS_TIER reference global tables (weather_info,
 * skill_table, gclass_table) that are declared in globals.h.  Because
 * globals.h is sometimes suppressed in unit tests (which provide their own
 * stub definitions), these remain as macros in utils.h and config.h rather
 * than inline functions here — macros expand at the call site where those
 * globals are always visible.
 */

#endif /* DEC_INLINES_H */
