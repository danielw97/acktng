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
 * save_areas.c — Area-level game state save.
 *
 * Saves persistent area-level data that survives server reboots:
 *   save_marks  — room marks/annotations
 *   save_bans   — site ban list
 */

#include "save.h"

void save_marks()
{

   FILE *fp;
   char mark_file_name[MAX_STRING_LENGTH];
   MARK_LIST_MEMBER *mark_list;

   if (fpReserve != NULL)
   {
      fclose(fpReserve);
      fpReserve = NULL;
   }
   sprintf(mark_file_name, "%s", MARKS_FILE);

   if ((fp = fopen(mark_file_name, "w")) == NULL)
   {
      bug("Save Mark list: fopen", 0);
      perror("failed open of roommarks.lst in save_marks");
   }
   else
   {
      for (mark_list = first_mark_list; mark_list != NULL; mark_list = mark_list->next)
      {
         fprintf(fp, "#MARK~\n");
         fprintf(fp, "%d\n", mark_list->mark->room_vnum);
         fprintf(fp, "%s~\n", mark_list->mark->message);
         fprintf(fp, "%s~\n", mark_list->mark->author);
         fprintf(fp, "%d\n", mark_list->mark->duration);
         fprintf(fp, "%d\n", mark_list->mark->type);
      }
      fprintf(fp, "#END~\n\n");
   }

   fflush(fp);
   if (fp != NULL)
   {
      fclose(fp);
      fp = NULL;
   }

   return;
}

void save_bans()
{

   FILE *fp;
   char ban_file_name[MAX_STRING_LENGTH];
   BAN_DATA *pban;

   if (fpReserve != NULL)
   {
      fclose(fpReserve);
      fpReserve = NULL;
   }
   sprintf(ban_file_name, "%s", BANS_FILE);

   if ((fp = fopen(ban_file_name, "w")) == NULL)
   {
      bug("Save ban list: fopen", 0);
      perror("failed open of bans.lst in save_ban");
   }
   else
   {
      for (pban = first_ban; pban != NULL; pban = pban->next)
      {
         fprintf(fp, "#BAN~\n");
         fprintf(fp, "%d\n", (pban->newbie ? 1 : 0));
         fprintf(fp, "%s~\n", pban->name);
         fprintf(fp, "%s~\n", pban->banned_by);
      }
      fprintf(fp, "#END~\n\n");
   }

   fflush(fp);
   if (fp != NULL)
   {
      fclose(fp);
      fp = NULL;
   }

   return;
}
