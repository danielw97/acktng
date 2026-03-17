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

/*
 * save.c — shared state and utilities for the save module.
 *
 * Contains global variables and helper functions used by multiple save files:
 *   save_players.c  — player character file save/load
 *   save_objects.c  — object/chest save/load
 *   save_mobs.c     — mob corpse save/load
 *   save_areas.c    — area-level game state (marks, bans)
 */

#include "save.h"

/*
 * Shared global state used across the save module.
 */

/* Current save-file revision, read from the Revision field during load. */
int cur_revision = 0;

/* Recursion counter for fwrite_obj to guard against runaway nesting. */
int loop_counter;

/* Array of containers used to re-nest objects when loading. */
OBJ_DATA *rgObjNest[MAX_NEST];

/* Hash table mapping old vnums to new vnums (from area_changes.txt). */
hash_table *hash_changed_vnums = NULL;

/* so the stack doesn't get hosed */
void abort_wrapper(void)
{
   abort();
}

void *hash_ref_from_vnum(int vnum)
{
   return (void *)(intptr_t)vnum;
}

int vnum_from_hash_ref(void *ref)
{
   return (int)(intptr_t)ref;
}

void init_changed_vnum_hash(void)
{
   FILE *fp;
   int oldvnum, newvnum;

   if (hash_changed_vnums != NULL)
      return;

   hash_changed_vnums = create_hash_table(1024);

   fp = fopen("area_changes.txt", "r");
   if (fp == NULL)
      return;

   while (!feof(fp))
   {
      if (str_cmp(fread_word(fp), "Obj:") || fread_letter(fp) != '[' ||
          (oldvnum = fread_number(fp)) == 0 || fread_letter(fp) != ']' ||
          str_cmp(fread_word(fp), "->") || fread_letter(fp) != '[' ||
          (newvnum = fread_number(fp)) == 0 || fread_letter(fp) != ']')
         fread_to_eol(fp);
      else
      {
         fread_to_eol(fp);
         add_hash_entry(hash_changed_vnums, oldvnum, hash_ref_from_vnum(newvnum));
      }
   }

   fclose(fp);
}
