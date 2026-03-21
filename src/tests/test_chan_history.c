/*
 * test_chan_history.c — unit tests for channel history (act_comm.c)
 *
 * Tests channel_to_hist_idx(), chan_history_add(), and chan_history_show().
 * Compiled from act_comm.unit-test.o with -ffunction-sections / --gc-sections
 * so that the many do_* and talk_channel functions (and their heavy deps) are
 * dead-stripped; only the three history functions need to link cleanly.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

/* ------------------------------------------------------------------ */
/* Stubs for external symbols referenced by chan_history_show()        */
/* ------------------------------------------------------------------ */

static char captured[MAX_STRING_LENGTH * 4];

void send_to_char(const char *txt, CHAR_DATA *ch)
{
   (void)ch;
   strncat(captured, txt, sizeof(captured) - strlen(captured) - 1);
}

/* ------------------------------------------------------------------ */
/* Forward declarations of the functions under test                    */
/* ------------------------------------------------------------------ */

int channel_to_hist_idx(int channel);
void chan_history_add(int channel, const char *speaker, const char *message);
void chan_history_show(CHAR_DATA *ch, int channel, const char *verb);

/* ------------------------------------------------------------------ */
/* Helpers                                                             */
/* ------------------------------------------------------------------ */

static void reset_capture(void)
{
   captured[0] = '\0';
}

/* ------------------------------------------------------------------ */
/* Tests: channel_to_hist_idx                                          */
/* ------------------------------------------------------------------ */

static void test_known_channels_map_to_valid_indices(void)
{
   assert(channel_to_hist_idx(CHANNEL_GOSSIP) == 1);
   assert(channel_to_hist_idx(CHANNEL_AUCTION) == 0);
   assert(channel_to_hist_idx(CHANNEL_MUSIC) == 2);
   assert(channel_to_hist_idx(CHANNEL_SHOUT) == 6);
   assert(channel_to_hist_idx(CHANNEL_YELL) == 7);
   assert(channel_to_hist_idx(CHANNEL_FLAME) == 8);
   assert(channel_to_hist_idx(CHANNEL_CLAN) == 11);
   assert(channel_to_hist_idx(CHANNEL_RACE) == 10);
   assert(channel_to_hist_idx(CHANNEL_OOC) == 17);
   assert(channel_to_hist_idx(CHANNEL_GAME) == 19);
}

static void test_unknown_channel_returns_minus_one(void)
{
   assert(channel_to_hist_idx(0) == -1);
   assert(channel_to_hist_idx(-1) == -1);
   assert(channel_to_hist_idx(999999) == -1);
}

/* ------------------------------------------------------------------ */
/* Tests: chan_history_add / chan_history_show                          */
/* ------------------------------------------------------------------ */

static void test_empty_history_shows_no_history_message(void)
{
   reset_capture();
   /* Use CHANNEL_MUSIC which is unlikely to have been touched */
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));
   chan_history_show(&ch, CHANNEL_MUSIC, "music");
   assert(strstr(captured, "No music history yet.") != NULL);
}

static void test_added_message_appears_in_history(void)
{
   reset_capture();
   chan_history_add(CHANNEL_GOSSIP, "Alice", "hello world");
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));
   chan_history_show(&ch, CHANNEL_GOSSIP, "gossip");
   assert(strstr(captured, "Alice") != NULL);
   assert(strstr(captured, "hello world") != NULL);
}

static void test_multiple_messages_appear_in_order(void)
{
   /* Use CHANNEL_FLAME (fresh, not touched by prior tests) */
   chan_history_add(CHANNEL_FLAME, "Bob", "first");
   chan_history_add(CHANNEL_FLAME, "Carol", "second");
   chan_history_add(CHANNEL_FLAME, "Dave", "third");

   reset_capture();
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));
   chan_history_show(&ch, CHANNEL_FLAME, "flame");

   char *p_first = strstr(captured, "first");
   char *p_second = strstr(captured, "second");
   char *p_third = strstr(captured, "third");
   assert(p_first != NULL);
   assert(p_second != NULL);
   assert(p_third != NULL);
   assert(p_first < p_second);
   assert(p_second < p_third);
}

static void test_ring_buffer_wraps_and_keeps_last_15(void)
{
   /* CHANNEL_SHOUT — write 17 messages, expect only last 15 */
   int i;
   char msg[64];
   for (i = 1; i <= 17; i++)
   {
      snprintf(msg, sizeof(msg), "message%d", i);
      chan_history_add(CHANNEL_SHOUT, "Tester", msg);
   }

   reset_capture();
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));
   chan_history_show(&ch, CHANNEL_SHOUT, "shout");

   /* Oldest two messages (1 and 2) must be gone */
   assert(strstr(captured, "message1\"") == NULL);
   assert(strstr(captured, "message2\"") == NULL);
   /* Last 15 must be present */
   for (i = 3; i <= 17; i++)
   {
      snprintf(msg, sizeof(msg), "message%d", i);
      assert(strstr(captured, msg) != NULL);
   }
}

static void test_unknown_channel_shows_what_message(void)
{
   reset_capture();
   CHAR_DATA ch;
   memset(&ch, 0, sizeof(ch));
   /* 0 is not a valid channel index → should fall through to "X what?" */
   chan_history_show(&ch, 0, "bogus");
   assert(strstr(captured, "what?") != NULL);
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */

int main(void)
{
   test_known_channels_map_to_valid_indices();
   test_unknown_channel_returns_minus_one();
   test_empty_history_shows_no_history_message();
   test_added_message_appears_in_history();
   test_multiple_messages_appear_in_order();
   test_ring_buffer_wraps_and_keeps_last_15();
   test_unknown_channel_shows_what_message();

   puts("test_chan_history: all tests passed");
   return 0;
}
