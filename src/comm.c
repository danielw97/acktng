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
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3.1        *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/*
 * This file contains OS-dependent networking, the main game loop, and
 * the core player output system (send_to_char, show_string, act).
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 *
 * Login/character creation state machine lives in login.c.
 * Hotreboot implementation lives in hotreboot.c.
 */

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#if defined(__CYGWIN__)
#include <crypt.h>
#endif
/* For child forking and stuff */
#include <sys/wait.h>
#include <unistd.h> /* for execl */
#include "globals.h"
#include "cursor.h"
#include "socket.h"
#include "prompt.h"
#include "npc_dialogue.h"

void trigger_happy_hour(void);
void copyover_recover args((void));

#ifndef UNIT_TEST_COMM

/*
 * Global variables.
 */
FILE *fpReserve; /* Reserved file handle         */
bool god;        /* All new chars are gods!      */
bool merc_down;  /* Shutdown                     */
bool wizlock;    /* Game is wizlocked            */
bool deathmatch; /* Deathmatch happening?        */
char str_boot_time[MAX_INPUT_LENGTH];
time_t current_time; /* Time of this pulse           */

/* port and control moved from local to global for HOTreboot - Flar */
int port;
int control;

int max_players = 0;
int hotreboot_countdown = 0; /* Pulses remaining until automatic hotreboot; 0 = inactive */

void list_who_to_output(void)
{
   DESCRIPTOR_DATA *d;
   FILE *who_html;
   FILE *who_count;
   int online_count = 0;

   who_html = fopen(WHO_HTML_FILE, "w");
   if (who_html == NULL)
      return;

   fprintf(who_html, "<h2>Players Online</h2>\n");
   fprintf(who_html, "<ul>\n");

   for (d = first_desc; d != NULL; d = d->next)
   {
      CHAR_DATA *who_ch;

      if (d->connected != CON_PLAYING)
         continue;

      who_ch = (d->original != NULL) ? d->original : d->character;
      if (who_ch == NULL || IS_NPC(who_ch))
         continue;

      fprintf(who_html, "<li>%s</li>\n", who_ch->name);
      online_count++;
   }

   fprintf(who_html, "</ul>\n");
   fclose(who_html);

   who_count = fopen(WHO_COUNT_FILE, "w");
   if (who_count == NULL)
      return;

   fprintf(who_count, "<p>Players online: %d</p>\n", online_count);
   fclose(who_count);
}

int main(int argc, char **argv)
{
   struct timeval now_time;
   bool fCopyOver = FALSE; /* HOTreboot??? Well is it...is it???? - Flar */
   extern int abort_threshold;

   /*
    * Init time.
    */
   gettimeofday(&now_time, NULL);
   current_time = (time_t)now_time.tv_sec;
   strcpy(str_boot_time, ctime(&current_time));

   /*
    * Get the port number.
    */
   port = 1234;
   if (argc > 1)
   {
      if (!is_number(argv[1]))
      {
         fprintf(stderr, "Usage: %s [port #]\n", argv[0]);
         exit(1);
      }
      else if ((port = atoi(argv[1])) <= 1024)
      {
         fprintf(stderr, "Port number must be above 1024.\n");
         exit(1);
      }
   }
   /* Check for HOTreboot parameter - Flar */
   if (argv[2] && argv[2][0])
   {
      fCopyOver = TRUE;
      control = atoi(argv[3]);
   }

   else
      fCopyOver = FALSE;

   rename("../log/comlog.old", "../log/comlog.crash");
   rename("../log/comlog.txt", "../log/comlog.old");

   /*
    * Run the game.
    */
   if (!fCopyOver) /* We already have the port if Copyovered. */
   {
      control = init_socket(port);
   }
   global_port = port;
   if (fCopyOver)
      abort_threshold = BOOT_DB_ABORT_THRESHOLD;
   boot_db();
   npc_dialogue_init();
#ifndef WIN32
   init_alarm_handler();
#endif
   sprintf(log_buf, "ACK! MUD is ready on port %d.", port);
   log_string(log_buf);
   if (fCopyOver)
   {
      extern bool disable_timer_abort;
      disable_timer_abort = TRUE;
      copyover_recover();
      disable_timer_abort = FALSE;
   }

   /* Seed WHO web output immediately on boot/copyover recovery. */
   list_who_to_output();

   game_loop(control);
   close(control);

   /*
    * That's all, folks.
    */
   log_string("Normal termination of game.");
   exit(0);
   return 0;
}

void stop_idling(CHAR_DATA *ch)
{
   if (ch == NULL || ch->desc == NULL || ch->desc->connected != CON_PLAYING ||
       ch->was_in_room == NULL || ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
      return;

   ch->timer = 0;
   char_from_room(ch);
   char_to_room(ch, ch->was_in_room);
   ch->was_in_room = NULL;
   act("$n has returned from the void.", ch, NULL, NULL, TO_ROOM);
   return;
}

/*
 * Write a message to all active descriptors.
 */
void send_to_descrips(const char *message)
{
   return;
}

/*
 * Write to one char.
 */
void send_to_char(const char *txt, CHAR_DATA *ch)
{
   if (ch == NULL)
      return;
   if (txt == NULL || ch->desc == NULL)
      return;
   /*
    * Large leak fixed here.. -- Altrag
    */
   if (ch->desc->showstr_head != NULL)
   {
      char *ssh;

      ssh = qgetmem(strlen(ch->desc->showstr_head) + strlen(txt) + 1);
      strcpy(ssh, ch->desc->showstr_head);
      strcat(ssh, txt);
      if (ch->desc->showstr_point)
         ch->desc->showstr_point += (ssh - ch->desc->showstr_head);
      else
         ch->desc->showstr_point = ssh;
      qdispose(ch->desc->showstr_head, strlen(ch->desc->showstr_head) + 1);
      ch->desc->showstr_head = ssh;
   }
   else
   {
      ch->desc->showstr_head = qgetmem(strlen(txt) + 1);
      strcpy(ch->desc->showstr_head, txt);
      ch->desc->showstr_point = ch->desc->showstr_head;
   }
   if (ch->desc->showstr_point == ch->desc->showstr_head)
      show_string(ch->desc, "");
   return;
}

/* The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom
   for porting this SillyMud code for MERC 2.0 and laying down the groundwork.
   Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which
   the improvements to the pager was modeled from.  - Kahn */
/* Leak fixes.. alloc_mem'd stuff shouldnt be free_string'd. -- Altrag */
/* Spec: buffer overflow fixes, internal buffer sizes increased */

void show_string(struct descriptor_data *d, char *input)
{
   char buffer[MAX_STRING_LENGTH * 2];
   char buf[MAX_INPUT_LENGTH];
   register char *scan, *chk;
   int lines = 0, toggle = 1;
   int space;

   one_argument(input, buf);

   switch (UPPER(buf[0]))
   {
   case '\0':
   case 'C': /* show next page of text */
      lines = 0;
      break;

   case 'R': /* refresh current page of text */
      lines = -1 - (d->character->pcdata->pagelen);
      break;

   case 'B': /* scroll back a page of text */
      lines = -(2 * d->character->pcdata->pagelen);
      break;

   case 'H': /* Show some help */
      write_to_buffer(d, "C, or Return = continue, R = redraw this page,\n\r", 0);
      write_to_buffer(d, "B = back one page, H = this help, Q or other keys = exit.\n\r\n\r", 0);
      lines = -1 - (d->character->pcdata->pagelen);
      break;

   default: /*otherwise, stop the text viewing */
      if (d->showstr_head)
      {
         qdispose(d->showstr_head, strlen(d->showstr_head) + 1);
         d->showstr_head = 0;
      }
      d->showstr_point = 0;
      return;
   }

   /*
    * do any backing up necessary
    */
   if (lines < 0)
   {
      for (scan = d->showstr_point; scan > d->showstr_head; scan--)
         if ((*scan == '\n') || (*scan == '\r'))
         {
            toggle = -toggle;
            if (toggle < 0)
               if (!(++lines))
                  break;
         }
      d->showstr_point = scan;
   }

   /*
    * show a chunk
    */
   lines = 0;
   toggle = 1;

   space = MAX_STRING_LENGTH * 2 - 100;
   for (scan = buffer;; scan++, d->showstr_point++)
   {
      space--;
      if (((*scan = *d->showstr_point) == '\n' || *scan == '\r') && (toggle = -toggle) < 0 &&
          space > 0)
         lines++;
      else if (!*scan ||
               (d->character && !IS_NPC(d->character) && lines >= d->character->pcdata->pagelen) ||
               space <= 0)
      {

         *scan = '\0';
         write_to_buffer(d, buffer, strlen(buffer));

         /*
          * See if this is the end (or near the end) of the string
          */
         for (chk = d->showstr_point; isspace(*chk); chk++)
            ;
         if (!*chk)
         {
            if (d->showstr_head)
            {
               qdispose(d->showstr_head, strlen(d->showstr_head) + 1);
               d->showstr_head = 0;
            }
            d->showstr_point = 0;
         }
         return;
      }
   }

   return;
}

/*
 * The primary output interface for formatted output.
 */
void act(const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type)
{
   static char *const he_she[] = {"it", "he", "she"};
   static char *const him_her[] = {"it", "him", "her"};
   static char *const his_her[] = {"its", "his", "her"};

   char buf[MAX_STRING_LENGTH];
   char fname[MAX_INPUT_LENGTH];
   char tmp_str[MSL];
   CHAR_DATA *to;
   CHAR_DATA *vch = (CHAR_DATA *)arg2;
   OBJ_DATA *obj1 = (OBJ_DATA *)arg1;
   OBJ_DATA *obj2 = (OBJ_DATA *)arg2;
   const char *str;
   const char *i = "";
   char *point;
   bool do_crlf = TRUE;
   bool can_see_message = TRUE;

   /*
    * Discard null and zero-length messages.
    */
   if (format == NULL || format[0] == '\0')
      return;

   if (ch == NULL)

      return;

   if ((ch->is_free != FALSE) || (ch->in_room == NULL))
   {
      bugf("bad ch, string=%s", format);
      return;
   }

   to = ch->in_room->first_person;
   if (type == TO_VICT)
   {
      if (vch == NULL)
      {
         bugf("Act: null vch with TO_VICT, string=%s", format);
         return;
      }
      to = vch->in_room->first_person;
   }

   for (; to != NULL; to = to->next_in_room)
   {
      if ((to->desc == NULL && (IS_NPC(to) && !(to->pIndexData->progtypes & ACT_PROG))) ||
          !IS_AWAKE(to))
         continue;

      if (type == TO_CHAR && to != ch)
         continue;
      if (type == TO_VICT && (to != vch || to == ch))
         continue;
      if (type == TO_ROOM && to == ch)
         continue;
      if (type == TO_NOTVICT && (to == ch || to == vch))
         continue;

      if (type == TO_NOTVICT && vch != NULL && (ch->fighting == vch || vch->fighting == ch) &&
          shortfight_should_suppress_watched_autoattack(
              IS_NPC(to), IS_SET(to->config, CONFIG_SHORT_FIGHT), to->fighting != NULL))
         continue;

      point = buf;
      str = format;
      while (*str != '\0')
      {
         if (*str != '$')
         {
            *point++ = *str++;
            continue;
         }
         ++str;

         if (arg2 == NULL && *str >= 'M' && *str <= 'N')
         {
            bugf("Act: missing arg2 for code $%c, string=%s", *str, format);
            i = " !!!!! ";
         }

         else if (arg2 == NULL && *str >= 'S' && *str <= 'T')
         {
            bugf("Act: missing arg2 for code $%c, string=%s", *str, format);
            i = " !!!!! ";
         }
         else if (arg2 == NULL && *str == 'E')
         {
            bugf("Act: missing arg2 for code $%c, string=%s", *str, format);
            i = " !!!!! ";
         }
         else
         {
            switch (*str)
            {
            default:
               bugf("Act: bad code $%c, string=%s", *str, format);
               i = " !!!!! ";
               break;
               /*
                * Thx alex for 't' idea
                */
            case 'L':
               can_see_message = TRUE;
               if (IS_IMMORTAL(to))
               {
                  if (IS_SET(ch->act, PLR_WIZINVIS) && ch->invis > get_trust(to)
                      /*
                       * || ( IS_SET( ch->act, PLR_INCOG )
                       * && ch->incog > get_trust( to )
                       */
                  )
                     can_see_message = FALSE;
               }
               else
               {
                  if (IS_SET(ch->act, PLR_WIZINVIS) && get_trust(to) < ch->invis)
                     can_see_message = FALSE;
                  if ((IS_AFFECTED(ch, AFF_SNEAK) || item_has_apply(ch, ITEM_APPLY_SNEAK)) &&
                      ((get_psuedo_level(ch) - 20 + number_range(1, 30)) > get_psuedo_level(to)))
                     can_see_message = FALSE;
               }
               break;
            case 't':
               i = (char *)arg1;
               break;
            case 'T':
               i = (char *)arg2;
               break;
            case 'n':
               i = PERS(ch, to);
               break;
            case 'N':
               i = PERS(vch, to);
               break;
            case 'e':
               i = he_she[URANGE(0, ch->sex, 2)];
               break;
            case 'E':
               i = he_she[URANGE(0, vch->sex, 2)];
               break;
            case 'm':
               i = him_her[URANGE(0, ch->sex, 2)];
               break;
            case 'M':
               i = him_her[URANGE(0, vch->sex, 2)];
               break;
            case 's':
               i = his_her[URANGE(0, ch->sex, 2)];
               break;
            case 'S':
               i = his_her[URANGE(0, vch->sex, 2)];
               break;
            case 'k':
               one_argument(ch->name, tmp_str);
               i = (char *)tmp_str;
               break;
            case 'K':
               one_argument(vch->name, tmp_str);
               i = (char *)tmp_str;
               break;

            case 'p':
               if (obj1)
               {
                  i = can_see_obj(to, obj1) ? obj1->short_descr : "something";
               }
               break;

            case 'P':
               if (obj2)
               {
                  i = can_see_obj(to, obj2) ? obj2->short_descr : "something";
               }
               break;

            case 'd':
               if (arg2 == NULL || ((char *)arg2)[0] == '\0')
               {
                  i = "door";
               }
               else
               {
                  one_argument((char *)arg2, fname);
                  i = fname;
               }
               break;
            }
            if (i[0] == '^')
            {
               ++i;
            }
         }
         ++str;
         while ((*point = *i) != '\0')
            ++point, ++i;
      }

      if (do_crlf)
      {
         *point++ = '\n';
         *point++ = '\r';
      }
      buf[0] = UPPER(buf[0]);
      *point = '\0';
      if (to->desc && can_see_message)
         write_to_buffer(to->desc, buf, point - buf);
   }

   return;
}

void hang(const char *str)
{
   bug(str, 0);
   kill(getpid(), SIGQUIT);
}

void notify(char *message, int lv)
{
   /*
    * This function sends <message>
    * * to all players of level (lv) and above
    * * -- Stephen
    */
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   sprintf(buf, "[NOTE]: %s\n\r", message);
   for (d = first_desc; d; d = d->next)
      if ((d->connected == CON_PLAYING) && (d->character->level >= lv) && !IS_NPC(d->character) &&
          !IS_SET(d->character->deaf, CHANNEL_NOTIFY))
         send_to_char(buf, d->character);
   return;
}

void auction(char *message)
{
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   sprintf(buf, "[AUCTION]: %s\n\r", message);
   for (d = first_desc; d; d = d->next)
      if ((d->connected == CON_PLAYING) && !IS_NPC(d->character) &&
          !IS_SET(d->character->deaf, CHANNEL_AUCTION))
         send_to_char(buf, d->character);
   return;
}

void info(char *message, int lv)
{
   /*
    * This function sends <message>
    * * to all players of level (lv) and above
    * * Used mainly to send level gain, death info, etc to mortals.
    * * - Stephen
    */
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   for (d = first_desc; d; d = d->next)
      if ((d->connected == CON_PLAYING) && (d->character->level >= lv) && !IS_NPC(d->character) &&
          !IS_SET(d->character->deaf, CHANNEL_INFO))
      {
         sprintf(buf, "%s[INFO]: %s%s\n\r", color_string(d->character, "info"), message,
                 color_string(d->character, "normal"));
         send_to_char(buf, d->character);
      }
   return;
}

void log_chan(const char *message, int lv)
{
   /*
    * Used to send messages to Immortals.
    * * Level is used to determine WHO gets the message...
    */
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;

   sprintf(buf, "[LOG]: %s\n\r", message);
   for (d = first_desc; d; d = d->next)
      if ((d->connected == CON_PLAYING) && (get_trust(d->character) == MAX_LEVEL) &&
          (!IS_NPC(d->character)) && (d->character->level >= lv) &&
          (!IS_SET(d->character->deaf, CHANNEL_LOG)))
         send_to_char(buf, d->character);
   return;
}

bool item_has_apply(CHAR_DATA *ch, int bit)
{
   /*
    * Used to see if ch is HOLDING any object(s) with the specified
    * * ITEM_APPLY bit set.
    * * -S-
    */
   OBJ_DATA *obj;

   for (obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list)
      if (IS_SET(obj->item_apply, bit) && obj->wear_loc != WEAR_NONE)
         return TRUE;

   return FALSE;
}

#endif /* !UNIT_TEST_COMM */
