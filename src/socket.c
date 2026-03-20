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
 * This file contains socket-related OS-dependent code:
 *   BSD sockets, TCP/IP, WebSocket support, and the main game I/O loop.
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

bool command_has_wait_flag args((CHAR_DATA * ch, const char *argument));

/*
 * Socket and TCP/IP stuff.
 */
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>

const char echo_off_str[] = {IAC, WILL, TELOPT_ECHO, '\0'};
const char echo_on_str[] = {IAC, WONT, TELOPT_ECHO, '\0'};
const char go_ahead_str[] = {IAC, GA, '\0'};

#include "socket.h"
#include "prompt.h"

/*
 * Forward declarations for functions defined in comm.c but called from socket.c
 */
extern bool merc_down;
void nanny(DESCRIPTOR_DATA *d, char *argument);
void stop_idling(CHAR_DATA *ch);

/*
 * Forward declarations for socket.c-internal functions used before their definitions
 */
void new_descriptor(int control);
void init_descriptor(DESCRIPTOR_DATA *dnew, int desc);
bool read_from_descriptor(DESCRIPTOR_DATA *d);
bool write_websocket_frame(DESCRIPTOR_DATA *d, unsigned char opcode, const unsigned char *payload,
                           size_t payload_len);

DESCRIPTOR_DATA *d_next; /* Next descriptor in loop      */

int global_port;

static const char websocket_guid[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

static void sha1_transform(uint32_t state[5], const unsigned char block[64])
{
   uint32_t w[80], a, b, c, d, e, t;
   int i;

   for (i = 0; i < 16; i++)
      w[i] = ((uint32_t)block[i * 4] << 24) | ((uint32_t)block[i * 4 + 1] << 16) |
             ((uint32_t)block[i * 4 + 2] << 8) | (uint32_t)block[i * 4 + 3];
   for (i = 16; i < 80; i++)
   {
      uint32_t v = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];
      w[i] = (v << 1) | (v >> 31);
   }

   a = state[0];
   b = state[1];
   c = state[2];
   d = state[3];
   e = state[4];

   for (i = 0; i < 80; i++)
   {
      uint32_t f, k;

      if (i < 20)
      {
         f = (b & c) | ((~b) & d);
         k = 0x5A827999;
      }
      else if (i < 40)
      {
         f = b ^ c ^ d;
         k = 0x6ED9EBA1;
      }
      else if (i < 60)
      {
         f = (b & c) | (b & d) | (c & d);
         k = 0x8F1BBCDC;
      }
      else
      {
         f = b ^ c ^ d;
         k = 0xCA62C1D6;
      }

      t = ((a << 5) | (a >> 27)) + f + e + k + w[i];
      e = d;
      d = c;
      c = (b << 30) | (b >> 2);
      b = a;
      a = t;
   }

   state[0] += a;
   state[1] += b;
   state[2] += c;
   state[3] += d;
   state[4] += e;
}

static void sha1_digest(const unsigned char *data, size_t len, unsigned char out[20])
{
   uint32_t state[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
   unsigned char block[64];
   uint64_t bit_len = (uint64_t)len * 8;
   size_t i, rem;

   for (i = 0; i + 64 <= len; i += 64)
      sha1_transform(state, data + i);

   rem = len - i;
   memset(block, 0, sizeof(block));
   if (rem > 0)
      memcpy(block, data + i, rem);
   block[rem] = 0x80;

   if (rem >= 56)
   {
      sha1_transform(state, block);
      memset(block, 0, sizeof(block));
   }

   for (i = 0; i < 8; i++)
      block[56 + i] = (unsigned char)(bit_len >> ((7 - i) * 8));

   sha1_transform(state, block);

   for (i = 0; i < 5; i++)
   {
      out[i * 4] = (unsigned char)(state[i] >> 24);
      out[i * 4 + 1] = (unsigned char)(state[i] >> 16);
      out[i * 4 + 2] = (unsigned char)(state[i] >> 8);
      out[i * 4 + 3] = (unsigned char)state[i];
   }
}

static const char *find_http_header_value(const char *headers, const char *header_name)
{
   const char *line_start;
   size_t name_len;

   if (headers == NULL || header_name == NULL || header_name[0] == '\0')
      return NULL;

   name_len = strlen(header_name);
   line_start = headers;

   while (*line_start != '\0')
   {
      const char *line_end = strstr(line_start, "\r\n");
      size_t line_len;

      if (line_end == NULL)
         break;

      line_len = (size_t)(line_end - line_start);
      if (line_len == 0)
         break;

      if (line_len > name_len + 1 && strncasecmp(line_start, header_name, name_len) == 0 &&
          line_start[name_len] == ':')
      {
         const char *value_start = line_start + name_len + 1;
         while (*value_start == ' ' || *value_start == '\t')
            value_start++;
         return value_start;
      }

      line_start = line_end + 2;
   }

   return NULL;
}

static bool header_value_contains_token(const char *value, const char *token)
{
   const char *cursor;
   size_t token_len;

   if (value == NULL || token == NULL || token[0] == '\0')
      return FALSE;

   token_len = strlen(token);
   cursor = value;

   while (*cursor != '\0' && *cursor != '\r' && *cursor != '\n')
   {
      const char *entry_start = cursor;
      const char *entry_end;
      size_t entry_len;

      while (*entry_start == ' ' || *entry_start == '\t' || *entry_start == ',')
         entry_start++;

      if (*entry_start == '\0' || *entry_start == '\r' || *entry_start == '\n')
         break;

      entry_end = entry_start;
      while (*entry_end != '\0' && *entry_end != '\r' && *entry_end != '\n' && *entry_end != ',')
         entry_end++;

      while (entry_end > entry_start && (entry_end[-1] == ' ' || entry_end[-1] == '\t'))
         entry_end--;

      entry_len = (size_t)(entry_end - entry_start);
      if (entry_len == token_len && strncasecmp(entry_start, token, token_len) == 0)
         return TRUE;

      cursor = (*entry_end == ',') ? entry_end + 1 : entry_end;
   }

   return FALSE;
}

static void send_music_play(DESCRIPTOR_DATA *d, const char *filename);
static void base64_encode(const unsigned char *in, size_t in_len, char *out, size_t out_size)
{
   static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   size_t i, j = 0;

   if (out_size == 0)
      return;

   for (i = 0; i < in_len && j + 4 < out_size; i += 3)
   {
      uint32_t octet_a = in[i];
      uint32_t octet_b = (i + 1 < in_len) ? in[i + 1] : 0;
      uint32_t octet_c = (i + 2 < in_len) ? in[i + 2] : 0;
      uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

      out[j++] = table[(triple >> 18) & 0x3F];
      out[j++] = table[(triple >> 12) & 0x3F];
      out[j++] = (i + 1 < in_len) ? table[(triple >> 6) & 0x3F] : '=';
      out[j++] = (i + 2 < in_len) ? table[triple & 0x3F] : '=';
   }

   out[j] = '\0';
}

void queue_login_greeting(DESCRIPTOR_DATA *d)
{
   char buf[MAX_STRING_LENGTH];
   HELP_DATA *pHelp;
   extern HELP_DATA *first_help;

   if (d == NULL || d->greeting_sent)
      return;

   d->greeting_sent = TRUE;
   sprintf(buf, "greeting%d", number_range(1, 6));

   for (pHelp = first_help; pHelp != NULL; pHelp = pHelp->next)
      if (!str_cmp(pHelp->keyword, buf))
      {
         if (pHelp->text[0] == '.')
            write_to_buffer(d, pHelp->text + 1, 0);
         else
            write_to_buffer(d, pHelp->text, 0);
         break;
      }
}

bool handle_websocket_handshake(DESCRIPTOR_DATA *d)
{
   char *end_headers;
   char *key_line;
   char *key_start;
   char key[256], challenge[512], accept_key[64], response[512];
   unsigned char digest[20];
   size_t key_len;

   if (d == NULL || d->websocket_handshake_complete || d->inbuf_len <= 0)
      return TRUE;

   d->inbuf[d->inbuf_len] = '\0';
   if (strncmp(d->inbuf, "GET ", 4) != 0)
   {
      d->websocket_handshake_complete = TRUE;
      queue_login_greeting(d);
      return TRUE;
   }

   end_headers = strstr(d->inbuf, "\r\n\r\n");
   if (end_headers == NULL)
      return TRUE;

   key_line = (char *)find_http_header_value(d->inbuf, "Upgrade");
   if (key_line == NULL || !header_value_contains_token(key_line, "websocket"))
      return FALSE;

   key_start = (char *)find_http_header_value(d->inbuf, "Sec-WebSocket-Key");
   if (key_start == NULL)
      return FALSE;

   while (*key_start == ' ' || *key_start == '\t')
      key_start++;

   key_len = strcspn(key_start, "\r\n");
   if (key_len == 0 || key_len >= sizeof(key))
      return FALSE;

   memcpy(key, key_start, key_len);
   key[key_len] = '\0';

   snprintf(challenge, sizeof(challenge), "%s%s", key, websocket_guid);
   sha1_digest((const unsigned char *)challenge, strlen(challenge), digest);
   base64_encode(digest, sizeof(digest), accept_key, sizeof(accept_key));

   snprintf(response, sizeof(response),
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: %s\r\n\r\n",
            accept_key);

   if (!write_to_descriptor(d->descriptor, response, 0))
      return FALSE;

   d->websocket_active = TRUE;
   d->websocket_handshake_complete = TRUE;

   d->inbuf_len -= (int)((end_headers + 4) - d->inbuf);
   memmove(d->inbuf, end_headers + 4, d->inbuf_len);
   d->inbuf[d->inbuf_len] = '\0';

   send_music_play(d, "theme.mp3");

   queue_login_greeting(d);
   return TRUE;
}

static void send_music_play(DESCRIPTOR_DATA *d, const char *filename)
{
   char buf[256];
   snprintf(buf, sizeof(buf), "{\"type\":\"music\",\"action\":\"play\",\"url\":\"/web/mp3/%s\"}",
            filename);
   write_websocket_frame(d, 0x1, (const unsigned char *)buf, strlen(buf));
}

void send_area_music(CHAR_DATA *ch)
{
   DESCRIPTOR_DATA *d;
   const char *track;

   if (IS_NPC(ch) || (d = ch->desc) == NULL || !d->websocket_active)
      return;

   track = ch->in_room->area->music; /* NULL means default theme */

   if (track == d->websocket_current_music)
      return;

   send_music_play(d, track ? track : "theme.mp3");
   d->websocket_current_music = track;
}

static size_t sanitize_websocket_text_payload(const unsigned char *src, size_t src_len,
                                              unsigned char *dst, size_t dst_cap)
{
   size_t i = 0, j = 0;

   if (src == NULL || dst == NULL || dst_cap == 0)
      return 0;

   while (i < src_len && j < dst_cap)
   {
      if (src[i] == (unsigned char)IAC)
      {
         if (i + 1 >= src_len)
         {
            i++;
            continue;
         }

         if (src[i + 1] == (unsigned char)GA)
         {
            i += 2;
            continue;
         }

         if ((src[i + 1] == (unsigned char)WILL || src[i + 1] == (unsigned char)WONT ||
              src[i + 1] == (unsigned char)DO || src[i + 1] == (unsigned char)DONT) &&
             i + 2 < src_len)
         {
            i += 3;
            continue;
         }
      }

      if (src[i] == '\r')
      {
         i++;
         continue; /* strip carriage return */
      }

      dst[j++] = src[i++];
   }

   return j;
}

bool write_websocket_frame(DESCRIPTOR_DATA *d, unsigned char opcode, const unsigned char *payload,
                           size_t payload_len)
{
   unsigned char header[4];
   size_t hlen;

   if (payload_len > 65535)
      return FALSE;

   header[0] = 0x80 | (opcode & 0x0F);
   if (payload_len < 126)
   {
      header[1] = (unsigned char)payload_len;
      hlen = 2;
   }
   else
   {
      header[1] = 126;
      header[2] = (unsigned char)(payload_len >> 8);
      header[3] = (unsigned char)(payload_len & 0xFF);
      hlen = 4;
   }

   if (!write_to_descriptor(d->descriptor, (char *)header, (int)hlen))
      return FALSE;
   if (payload_len > 0 && !write_to_descriptor(d->descriptor, (char *)payload, (int)payload_len))
      return FALSE;

   return TRUE;
}

bool process_websocket_input(DESCRIPTOR_DATA *d)
{
   unsigned char *buf = (unsigned char *)d->inbuf;

   if (d == NULL || d->incomm[0] != '\0')
      return TRUE;

   while (d->inbuf_len >= 2)
   {
      size_t pos = 2, payload_len, frame_len;
      unsigned char opcode;
      unsigned char mask[4];
      int i, k = 0;

      buf = (unsigned char *)d->inbuf;
      opcode = buf[0] & 0x0F;
      payload_len = buf[1] & 0x7F;

      if (payload_len == 126)
      {
         if (d->inbuf_len < 4)
            return TRUE;
         payload_len = ((size_t)buf[2] << 8) | buf[3];
         pos = 4;
      }
      else if (payload_len == 127 || (buf[1] & 0x80) == 0)
         return FALSE;

      if ((size_t)d->inbuf_len < pos + 4 + payload_len)
         return TRUE;

      memcpy(mask, buf + pos, 4);
      pos += 4;
      frame_len = pos + payload_len;

      for (i = 0; i < (int)payload_len; i++)
         buf[pos + i] ^= mask[i % 4];

      if (opcode == 0x8)
         return FALSE;
      if (opcode == 0x9)
      {
         if (!write_websocket_frame(d, 0xA, buf + pos, payload_len))
            return FALSE;
      }
      else if (opcode == 0x1 || opcode == 0x0 || opcode == 0x2)
      {
         if (payload_len >= MAX_INPUT_LENGTH - 1)
            write_to_descriptor(d->descriptor, "Line too long.\n\r", 0);
         else
         {
            for (i = 0; i < (int)payload_len; i++)
            {
               unsigned char c = buf[pos + i];
               if (c == '\b' && k > 0)
                  --k;
               else if (isprint(c))
                  d->incomm[k++] = (char)c;
            }
            if (k == 0)
               d->incomm[k++] = ' ';
            d->incomm[k] = '\0';
         }
      }

      d->inbuf_len -= (int)frame_len;
      memmove(d->inbuf, d->inbuf + frame_len, d->inbuf_len);
      d->inbuf[d->inbuf_len] = '\0';

      if (d->incomm[0] != '\0')
         return TRUE;
   }

   return TRUE;
}

int init_socket(int port)
{
   static struct sockaddr_in sa_zero;
   struct sockaddr_in sa;
   int x = 1;
   int fd;

   if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      perror("Init_socket: socket");
      exit(1);
   }

   if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&x, sizeof(x)) < 0)
   {
      perror("Init_socket: SO_REUSEADDR");
      close(fd);
      exit(1);
   }

#if defined(SO_DONTLINGER) && !defined(SYSV)
   {
      struct linger ld;

      ld.l_onoff = 1;
      ld.l_linger = 1000;

      if (setsockopt(fd, SOL_SOCKET, SO_DONTLINGER, (char *)&ld, sizeof(ld)) < 0)
      {
         perror("Init_socket: SO_DONTLINGER");
         close(fd);
         exit(1);
      }
   }
#endif
   sa = sa_zero;
   sa.sin_family = AF_INET;
   sa.sin_port = htons(port);

   if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
   {
      perror("Init_socket: bind");
      close(fd);
      exit(1);
   }

   if (listen(fd, 3) < 0)
   {
      perror("Init_socket: listen");
      close(fd);
      exit(1);
   }

   return fd;
}

int cur_hour = 0;
int cur_players = 0;

/* + */
int reopen_flag;
void reopen_socket(int sig)
{
   reopen_flag = 1;
   signal(SIGUSR1, reopen_socket);
}

/* + */

void game_loop(int control)
{
   static struct timeval null_time;
   struct timeval last_time;

   signal(SIGPIPE, SIG_IGN);
   /*+*/
   /*
    * On a SIGUSR1, open and close the control socket (anti-port-locking
    * *  thing) -- Spectrum
    */
   reopen_flag = 0;
   signal(SIGUSR1, reopen_socket);

   /*+*/

   gettimeofday(&last_time, NULL);
   current_time = (time_t)last_time.tv_sec;

   /*
    * Main loop
    */
   while (!merc_down)
   {
      fd_set in_set;
      fd_set out_set;
      fd_set exc_set;
      DESCRIPTOR_DATA *d;
      int maxdesc;

      /*
       * handle reopening the control socket
       * * don't worry about locking here, we assume that SIGUSR1's are
       * * relatively rare
       */
      if (reopen_flag)
      {
         log_string("SIGUSR1 received, reopening control socket");
         close(control);
         control = init_socket(global_port);
         reopen_flag = 0;
      }

      /*
       * Poll all active descriptors.
       */
      FD_ZERO(&in_set);
      FD_ZERO(&out_set);
      FD_ZERO(&exc_set);
      FD_SET(control, &in_set);
      maxdesc = control;

      for (d = first_desc; d; d = d->next)
      {
         if ((d->flags && DESC_FLAG_PASSTHROUGH) == 0)
         {
            maxdesc = UMAX(maxdesc, d->descriptor);
            FD_SET(d->descriptor, &in_set);
            FD_SET(d->descriptor, &out_set);
            FD_SET(d->descriptor, &exc_set);
         }
         else
         {
            /*
             * Check to see if child process has terminated
             */
            if (waitpid(d->childpid, NULL, WNOHANG) != 0)
            {
               /*
                * Terminated or error
                */
               d->childpid = 0;
               REMOVE_BIT(d->flags, DESC_FLAG_PASSTHROUGH);
            }
         }
      }

      if (select(maxdesc + 1, &in_set, &out_set, &exc_set, &null_time) < 0)
      {
         perror("Game_loop: select: poll");
         exit(1);
      }

      /*
       * New connection?
       */
      if (FD_ISSET(control, &in_set))
         new_descriptor(control);

      /*
       * Kick out the freaky folks.
       */
      for (d = first_desc; d != NULL; d = d_next)
      {
         d_next = d->next;
         if (FD_ISSET(d->descriptor, &exc_set))
         {
            FD_CLR(d->descriptor, &in_set);
            FD_CLR(d->descriptor, &out_set);
            if (d->character)
               save_char_obj(d->character);
            d->outtop = 0;
            close_socket(d);
         }
      }

      /*
       * Process input.
       */
      for (d = first_desc; d != NULL; d = d_next)
      {
         d_next = d->next;
         d->fcommand = FALSE;

         if (FD_ISSET(d->descriptor, &in_set))
         {
            if (d->character != NULL)
               d->character->timer = 0;
            if (!read_from_descriptor(d))
            {
               FD_CLR(d->descriptor, &out_set);
               if (d->character != NULL)
                  save_char_obj(d->character);
               d->outtop = 0;
               close_socket(d);
               continue;
            }
         }

         /* Read the next command before checking wait, so we can inspect it. */
         read_from_buffer(d);

         if (d->character != NULL && d->character->wait > 0)
         {
            --d->character->wait;

            /*
             * If the pending command introduces a wait state, keep it queued.
             * incomm is left non-empty; read_from_buffer will not overwrite it
             * on the next pulse, so the command stays queued until wait expires.
             * Non-wait-state commands (look, score, say, etc.) fall through and
             * execute immediately.
             */
            if (d->incomm[0] != '\0' && d->connected == CON_PLAYING &&
                command_has_wait_flag(d->character, d->incomm))
               continue;
         }

         if (d->incomm[0] != '\0')
         {
            d->fcommand = TRUE;
            stop_idling(d->character);
            d->timeout = current_time + 180; /* spec: stop idling */

            if (d->connected == CON_PLAYING)
               if (d->showstr_point)
                  show_string(d, d->incomm);
               else
                  interpret(d->character, d->incomm);
            else
               nanny(d, d->incomm);

            d->incomm[0] = '\0';
         }
      }

      /*
       * Autonomous game motion.
       */
      update_handler();
      waitpid(0, 0, WNOHANG);

      /*
       * Output.
       */
      for (d = first_desc; d != NULL; d = d_next)
      {
         d_next = d->next;

         /*
          * spec: disconnect people idling on login
          */
         if (d->connected < 0 && d->timeout < current_time)
         {
            if (d->websocket_active)
               write_websocket_frame(d, 0x1, (const unsigned char *)"Login timeout (180s)\n\r",
                                     strlen("Login timeout (180s)\n\r"));
            else
               write_to_descriptor(d->descriptor, "Login timeout (180s)\n\r", 0);
            close_socket(d);
            continue;
         }

         if ((d->fcommand || d->outtop > 0) && FD_ISSET(d->descriptor, &out_set))
         {
            if (!process_output(d, TRUE))
            {
               if (d->character != NULL)
                  save_char_obj(d->character);
               d->outtop = 0;
               close_socket(d);
            }
         }
      }

      /*
       * Synchronize to a clock.
       * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
       * Careful here of signed versus unsigned arithmetic.
       */
      {
         struct timeval now_time;
         struct tm *now_bd_time;
         FILE *out_file;
         long secDelta;
         long usecDelta;

         long temp_time;

         gettimeofday(&now_time, NULL);
         temp_time = (time_t)now_time.tv_sec;
         now_bd_time = localtime(&temp_time);

         if (now_bd_time->tm_hour != cur_hour)
         {
            cur_hour = now_bd_time->tm_hour;
            out_file = fopen("players.num", "a");
            fprintf(out_file, "%i %i %i\n", now_bd_time->tm_mday, cur_hour, max_players);
            if (out_file != NULL)
            {
               fclose(out_file);
               out_file = NULL;
            }
         }

         usecDelta =
             ((int)last_time.tv_usec) - ((int)now_time.tv_usec) + 1000000 / PULSE_PER_SECOND;
         secDelta = ((int)last_time.tv_sec) - ((int)now_time.tv_sec);
         while (usecDelta < 0)
         {
            usecDelta += 1000000;
            secDelta -= 1;
         }

         while (usecDelta >= 1000000)
         {
            usecDelta -= 1000000;
            secDelta += 1;
         }

         if (secDelta > 0 || (secDelta == 0 && usecDelta > 0))
         {
            struct timeval stall_time;

            stall_time.tv_usec = usecDelta;
            stall_time.tv_sec = secDelta;
            if (select(0, NULL, NULL, NULL, &stall_time) < 0 && errno != EINTR)
            {
               perror("Game_loop: select: stall");
               exit(1);
            }
         }
      }

      gettimeofday(&last_time, NULL);
      current_time = (time_t)last_time.tv_sec;
   }

   return;
}

void free_desc(DESCRIPTOR_DATA *d)
{
   DESCRIPTOR_DATA *sd;

   d->snoop_by = NULL;
   for (sd = first_desc; sd; sd = sd->next)
      if (sd->snoop_by == d)
         sd->snoop_by = NULL;
   if (d->original)
      do_return(d->character, "");
   if (d->character)
      free_char(d->character);
   free_string(d->host);
   close(d->descriptor);
   if (d->showstr_head)
      qdispose(d->showstr_head, strlen(d->showstr_head) + 1);
   if (d->outbuf)
      dispose(d->outbuf, d->outsize);
}

void new_descriptor(int control)
{
   static DESCRIPTOR_DATA d_zero;
   char buf[MSL];
   DESCRIPTOR_DATA *dnew;
   /*
    * BAN_DATA *pban;
    */
   struct sockaddr_in sock;
   int desc;
   socklen_t size;

   size = sizeof(sock);
   getsockname(control, (struct sockaddr *)&sock, &size);
   if ((desc = accept(control, (struct sockaddr *)&sock, &size)) < 0)
   {
      perror("New_descriptor: accept");
      return;
   }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

   if (fcntl(desc, F_SETFL, FNDELAY) == -1)
   {
      perror("New_descriptor: fcntl: FNDELAY");
      return;
   }

   /*
    * Cons a new descriptor.
    */
   GET_FREE(dnew, desc_free);
   *dnew = d_zero;
   init_descriptor(dnew, desc); /* Not sure is this right? */
   dnew->descriptor = desc;
   dnew->connected = CON_GET_NAME;
   dnew->showstr_head = NULL;
   dnew->showstr_point = NULL;
   dnew->outsize = 2000;
   dnew->flags = 0;
   dnew->childpid = 0;

   size = sizeof(sock);
   if (getpeername(desc, (struct sockaddr *)&sock, &size) < 0)
   {
      perror("New_descriptor: getpeername");
      dnew->host = str_dup("(unknown)");
   }
   else
   {
      /*
       * Would be nice to use inet_ntoa here but it takes a struct arg,
       * which ain't very compatible between gcc and system libraries.
       */
      int addr;

      addr = ntohl(sock.sin_addr.s_addr);
      sprintf(buf, "%d.%d.%d.%d", (addr >> 24) & 0xFF, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF,
              (addr) & 0xFF);
      sprintf(log_buf, "Sock.sinaddr:  %s (%d)", buf, ntohs(sock.sin_port));
      log_string(log_buf);
      sprintf(log_buf, "Connection formed from %s.", buf);
      monitor_chan(log_buf, MONITOR_CONNECT);

      dnew->remote_port = ntohs(sock.sin_port);

      /*
       * From unused to prevent possible ns lockup
       * from = gethostbyaddr( (char*) &sock.sin_addr,
       * sizeof(sock.sin_addr), AF_INET );
       */

      dnew->host = str_dup(buf);
   }

   /*
    * Swiftest: I added the following to ban sites.  I don't
    * endorse banning of sites, but Copper has few descriptors now
    * and some people from certain sites keep abusing access by
    * using automated 'autodialers' and leaving connections hanging.
    *
    * Furey: added suffix check by request of Nickel of HiddenWorlds.
    *
    * Stephen: As we use IP address now, want to use prefix check,
    * so we can ban whole domains....
    */
   /*
    * MOVED TO LOWER FOR NEWBIEBAN
    *
    * for ( pban = first_ban; pban != NULL; pban = pban->next )
    * {
    * if ( !str_prefix( pban->name, dnew->host ) && !( pban->newbie ) )
    * {
    * char buf[MAX_STRING_LENGTH];
    * sprintf( buf, "Denying access to banned site %s", dnew->host );
    * monitor_chan( buf, MONITOR_CONNECT );
    * write_to_descriptor( desc,
    * "Your site has been banned from this Mud.  BYE BYE!\n\r", 0 );
    * free_desc(dnew);
    * PUT_FREE(dnew, desc_free);
    * return;
    * }
    * }
    *
    *
    */
   /*
    * Init descriptor data.
    */
   LINK(dnew, first_desc, last_desc, next, prev);

   /*
    * spec: set initial login timeout
    */
   dnew->timeout = current_time + 180;

   queue_login_greeting(dnew);

   cur_players++;

   return;
}

void init_descriptor(DESCRIPTOR_DATA *dnew, int desc)
{
   static DESCRIPTOR_DATA d_zero;
   *dnew = d_zero;
   dnew->descriptor = desc;
   dnew->connected = CON_GET_NAME;
   dnew->showstr_head = NULL;
   dnew->showstr_point = NULL;
   dnew->outsize = 2000;
   dnew->outbuf = getmem(dnew->outsize);
   dnew->flags = 0;
   dnew->childpid = 0;
   dnew->inbuf_len = 0;
   dnew->websocket_active = FALSE;
   dnew->websocket_handshake_complete = FALSE;
   dnew->greeting_sent = FALSE;
   dnew->websocket_current_music = NULL;
}

void close_socket(DESCRIPTOR_DATA *dclose)
{
   CHAR_DATA *ch;

   if (dclose->outtop > 0)
      process_output(dclose, FALSE);

   if (dclose->snoop_by != NULL)
   {
      write_to_buffer(dclose->snoop_by, "Your victim has left the game.\n\r", 0);
   }

   {
      DESCRIPTOR_DATA *d;

      for (d = first_desc; d != NULL; d = d->next)
      {
         if (d->snoop_by == dclose)
            d->snoop_by = NULL;
      }
   }

   if (dclose->original)
   {
      if (dclose->character)
         do_return(dclose->character, "");
      else
      {
         bug("Close_socket: original without ch", 0);
         dclose->character = dclose->original;
         dclose->original = NULL;
      }
   }

   if ((ch = dclose->character) != NULL)
   {
      sprintf(log_buf, "Closing link to %s.", ch->name);
      log_string(log_buf);
      monitor_chan(log_buf, MONITOR_CONNECT);
      if (dclose->connected == CON_PLAYING)
      {
         act("$n has lost $s link.", ch, NULL, NULL, TO_ROOM);
         ch->desc = NULL;
      }
      else
      {
         free_char(dclose->character);
      }
      /*      stop_fighting( ch, TRUE );
            save_char_obj( ch );
            extract_char( ch, TRUE );  */
   }

   if (d_next == dclose)
      d_next = d_next->next;

   UNLINK(dclose, first_desc, last_desc, next, prev);
   close(dclose->descriptor);
   free_string(dclose->host);
   if (dclose->outbuf)
      dispose(dclose->outbuf, dclose->outsize);
   if (dclose->showstr_head)
      qdispose(dclose->showstr_head, strlen(dclose->showstr_head) + 1);
   PUT_FREE(dclose, desc_free);

   cur_players--;
   list_who_to_output();
   return;
}

bool read_from_descriptor(DESCRIPTOR_DATA *d)
{
   int iStart;

   /*
    * Hold horses if pending command already.
    */
   if (d->incomm[0] != '\0')
      return TRUE;

   /*
    * Check for overflow.
    */
   iStart = d->inbuf_len;
   if (iStart >= sizeof(d->inbuf) - 10)
   {
      sprintf(log_buf, "%s input overflow!", d->host);
      log_string(log_buf);
      sprintf(log_buf, "input overflow by %s (%s)",
              (d->character == NULL) ? "[login]" : d->character->name, d->host);
      monitor_chan(log_buf, MONITOR_CONNECT);
      write_to_descriptor(d->descriptor, "\n\r SPAMMING IS RUDE, BYE BYE! \n\r", 0);
      return FALSE;
   }

   /*
    * Snarf input.
    */
   for (;;)
   {
      int nRead;

      nRead = read(d->descriptor, d->inbuf + iStart, sizeof(d->inbuf) - 10 - iStart);
      if (nRead > 0)
      {
         iStart += nRead;
         if (d->websocket_active)
            break;
         if (d->inbuf[iStart - 1] == '\n' || d->inbuf[iStart - 1] == '\r')
            break;
      }
      else if (nRead == 0)
      {
         log_string("EOF encountered on read.");
         return FALSE;
      }
      else if (errno == EWOULDBLOCK)
         break;
      else
      {
         perror("Read_from_descriptor");
         return FALSE;
      }
   }
   d->inbuf_len = iStart;
   d->inbuf[iStart] = '\0';

   if (!d->websocket_handshake_complete)
   {
      if (!handle_websocket_handshake(d))
         return FALSE;
      if (!d->websocket_active && d->websocket_handshake_complete)
         queue_login_greeting(d);
   }

   return TRUE;
}

/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer(DESCRIPTOR_DATA *d)
{
   int i, j, k;

   /*
    * Hold horses if pending command already.
    */
   if (d->incomm[0] != '\0')
      return;

   if (d->websocket_active)
   {
      process_websocket_input(d);
      return;
   }

   /*
    * Look for at least one new line.
    */
   for (i = 0; i < d->inbuf_len && d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++)
      ;
   if (i >= d->inbuf_len)
      return;

   /*
    * Canonical input processing.
    */
   for (i = 0, k = 0; i < d->inbuf_len && d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++)
   {
      if (k >= MAX_INPUT_LENGTH - 2)
      {
         write_to_descriptor(d->descriptor, "Line too long.\n\r", 0);

         /*
          * skip the rest of the line
          */
         for (; i < d->inbuf_len; i++)
         {
            if (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
               break;
         }
         break;
      }

      if (d->inbuf[i] == '\b' && k > 0)
         --k;
      else if (isascii(d->inbuf[i]) && isprint(d->inbuf[i]))
         d->incomm[k++] = d->inbuf[i];
   }

   /*
    * Finish off the line.
    */
   if (k == 0)
      d->incomm[k++] = ' ';
   d->incomm[k] = '\0';

   /*
    * Deal with bozos with #repeat 1000 ...
    */
   if (k > 1 || d->incomm[0] == '!')
   {
      if (d->incomm[0] != '!' && strcmp(d->incomm, d->inlast))
      {
         d->repeat = 0;
      }
      else
      {
         if (++d->repeat >= 30)
         {
            if (d->connected == CON_PLAYING)
            {
               sprintf(log_buf, "%s input spamming!", d->character->name);
               log_string(log_buf);
               monitor_chan(log_buf, MONITOR_CONNECT);
            }
            write_to_descriptor(d->descriptor, "\n\r***** SHUT UP!! *****\n\r", 0);
            close_socket(d);
            /*
             * old way: strcpy( d->incomm, "quit" );
             */
         }
      }
   }

   /*
    * Do '!' substitution.
    */
   if (d->incomm[0] == '!')
      strcpy(d->incomm, d->inlast);
   else
      strcpy(d->inlast, d->incomm);

   /*
    * Shift the input buffer.
    */
   while (i < d->inbuf_len && (d->inbuf[i] == '\n' || d->inbuf[i] == '\r'))
      i++;
   for (j = 0; i + j < d->inbuf_len; j++)
      d->inbuf[j] = d->inbuf[i + j];
   d->inbuf_len = j;
   d->inbuf[d->inbuf_len] = '\0';
   return;
}

/*
 * Low level output function.
 */
bool process_output(DESCRIPTOR_DATA *d, bool fPrompt)
{
   extern bool merc_down;

   /*
    * Bust a prompt.
    */
   if (fPrompt && !merc_down && d->connected == CON_PLAYING)
   {
      if (d->showstr_point)
         write_to_buffer(
             d, "[Please type (c)ontinue, (r)efresh, (b)ack, (h)elp, (q)uit, or RETURN]:  ", 0);
      else
      {
         CHAR_DATA *ch;

         ch = d->original ? d->original : d->character;
         if (IS_SET(ch->config, CONFIG_BLANK))
            write_to_buffer(d, "\n\r", 2);
         if (ch->hunting || ch->hunt_obj)
            char_hunt(ch);
         bust_a_prompt(d);
         if (IS_SET(ch->config, CONFIG_TELNET_GA))
            write_to_buffer(d, go_ahead_str, 0);
      }
   }
   /*
    * Short-circuit if nothing to write.
    */
   if (d->outtop == 0)
      return TRUE;
   /*
    * Snoop-o-rama.
    */
   if (d->snoop_by != NULL)
   {
      char foo[MAX_STRING_LENGTH];
      CHAR_DATA *snoop_ch;

      snoop_ch = d->original != NULL ? d->original : d->character;
      if (snoop_ch != NULL)
         sprintf(foo, "[SNOOP:%s] ", snoop_ch->name);
      write_to_buffer(d->snoop_by, foo, 0);
      write_to_buffer(d->snoop_by, d->outbuf, d->outtop);
   }
   /*
    * OS-dependent output.
    */
   if (d->websocket_active)
   {
      unsigned char *payload;
      size_t payload_len;

      payload = (unsigned char *)malloc((size_t)d->outtop + 1);
      if (payload == NULL)
      {
         d->outtop = 0;
         return FALSE;
      }

      payload_len = sanitize_websocket_text_payload(
          (const unsigned char *)d->outbuf, (size_t)d->outtop, payload, (size_t)d->outtop + 1);
      if (!write_websocket_frame(d, 0x1, payload, payload_len))
      {
         free(payload);
         d->outtop = 0;
         return FALSE;
      }
      free(payload);
      d->outtop = 0;
      return TRUE;
   }
   else if (!write_to_descriptor(d->descriptor, d->outbuf, d->outtop))
   {
      d->outtop = 0;
      return FALSE;
   }
   else
   {
      d->outtop = 0;
      return TRUE;
   }
}

/*
 * Append onto an output buffer.
 */

#define color_MARGIN 20

void write_to_buffer(DESCRIPTOR_DATA *d, const char *txt, int length)
{
   if (d == NULL)
      return;

   if (d->outbuf == NULL)
   {
      bugf("write_to_buffer with NULL outbuf, string=%s", txt);
      return;
   }

   /*
    * Find length in case caller didn't.
    */
   if (length <= 0)
      length = strlen(txt);

   /*
    * Initial \n\r if needed.
    */
   if (d->outtop == 0 && !d->fcommand)
   {
      d->outbuf[0] = '\n';
      d->outbuf[1] = '\r';
      d->outtop = 2;
   }

   /*
    * Expand the buffer as needed.
    */
   while (d->outtop + length + 1 + color_MARGIN >= d->outsize)
   {
      char *outbuf;

      outbuf = getmem(2 * d->outsize);
      strncpy(outbuf, d->outbuf, d->outtop);
      dispose(d->outbuf, d->outsize);
      d->outbuf = outbuf;
      d->outsize *= 2;
   }

   /*
    * Copy.
    */

   /*
    * ONLY COPY length number of Bytes, otherwise things can go WRONG
    */
   /*
    * MAG mod to strncpy
    */
   /*
    * Old was  strcpy( d->outbuf + d->outtop, txt );
    */

   /*
    * Before Mod2: strncpy( d->outbuf + d->outtop, txt, length );
    */

   /*
    * MAG Mod2: Convert color strings, while copying. Also expand buffer as neede.
    */
   {
      char c;
      char lookup;
      char *dest;
      int count = length;
      CHAR_DATA *ch;
      char *colstr;
      int collen, cnt;

      dest = d->outbuf + d->outtop;

      for (; count > 0;)
      {
         c = *(txt++);
         if (c != '@')
         {
            *(dest++) = c;
            count--;
            continue;
         }
         else
         {
            if (*txt != '@')
            {
               *(dest++) = c;
               count--;
               continue;
            }

            txt++;        /* txt now points at color code. */
            c = *(txt++); /* c is color code. */
            length = length - 3;
            count = count - 3;

            if (c == '@')
            {
               length++;
               *(dest++) = c;
               continue;
            }

            ch = d->original != NULL ? d->original : d->character;
            if (ch != NULL && !IS_SET(ch->config, CONFIG_COLOR))
               continue;
            /* set to default highlight or dim set by player */
            lookup = c;
            if (ch != NULL && !IS_NPC(ch)) /* shouldn't happen, but... */
            {
               if (lookup == '!')
                  lookup = ch->pcdata->hicol;
               else if (lookup == '.')
                  lookup = ch->pcdata->dimcol;
            }

            for (cnt = 0; cnt < MAX_ANSI; cnt++)
               if (ansi_table[cnt].letter == lookup)
                  break;

            if (cnt == MAX_ANSI)
            {
               colstr = ansi_table[10].value;
               collen = ansi_table[10].stlen;
            }
            else
            {
               colstr = ansi_table[cnt].value;
               collen = ansi_table[cnt].stlen;
            }

            while (d->outtop + length + collen + 1 >= d->outsize)
            {
               char *outbuf;

               outbuf = getmem(2 * d->outsize);
               strncpy(outbuf, d->outbuf, d->outtop + length - count);
               dispose(d->outbuf, d->outsize);
               d->outbuf = outbuf;
               d->outsize *= 2;
            }

            dest = d->outbuf + d->outtop + length - count;
            strncpy(dest, colstr, collen);
            dest += collen;

            length = length + collen;
         }
      }
   }

   /*
    * Make sure we have a \0 at the end
    */
   *(d->outbuf + d->outtop + length) = '\0';

   d->outtop += length;
   return;
}

/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor(int desc, char *txt, int length)
{
   int iStart;
   int nWrite;
   int nBlock;

   if (length <= 0)
      length = strlen(txt);

   for (iStart = 0; iStart < length; iStart += nWrite)
   {
      nBlock = UMIN(length - iStart, 4096);
      if ((nWrite = write(desc, txt + iStart, nBlock)) < 0)
      {
         perror("Write_to_descriptor");
         return FALSE;
      }
   }
   return TRUE;
}
