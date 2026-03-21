/*
 * Unit tests for GMCP protocol framing and package support bitmask logic.
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Telnet constants */
#define IAC 255
#define SB 250
#define SE 240

/* GMCP telnet option */
#define TELOPT_GMCP 201

/* GMCP package subscription bitmask (mirrors socket.h) */
#define GMCP_PKG_CHAR (1U << 0)
#define GMCP_PKG_ROOM (1U << 1)
#define GMCP_PKG_COMM (1U << 2)

/* Minimal stub descriptor */
typedef struct descriptor_data DESCRIPTOR_DATA;
struct descriptor_data
{
   int descriptor;
   int gmcp_active;
   unsigned int gmcp_supports;
   unsigned short gmcp_last_room;
};

/* Capture buffer for write_to_descriptor output */
static unsigned char capture_buf[8192];
static int capture_len = 0;

static void reset_capture(void)
{
   capture_len = 0;
   memset(capture_buf, 0, sizeof(capture_buf));
}

static int write_to_descriptor(DESCRIPTOR_DATA *d, char *txt, int length)
{
   (void)d;
   if (length <= 0)
      length = (int)strlen(txt);
   if (capture_len + length < (int)sizeof(capture_buf))
   {
      memcpy(capture_buf + capture_len, txt, length);
      capture_len += length;
   }
   return 1;
}

/*
 * Minimal copy of gmcp_send from socket.c for testing the framing.
 */
static void gmcp_send(DESCRIPTOR_DATA *d, const char *package, const char *json)
{
   unsigned char buf[4096];
   int i = 0;

   buf[i++] = (unsigned char)IAC;
   buf[i++] = (unsigned char)SB;
   buf[i++] = (unsigned char)TELOPT_GMCP;
   while (*package && i < (int)sizeof(buf) - 4)
      buf[i++] = (unsigned char)*package++;
   buf[i++] = ' ';
   while (*json && i < (int)sizeof(buf) - 3)
      buf[i++] = (unsigned char)*json++;
   buf[i++] = (unsigned char)IAC;
   buf[i++] = (unsigned char)SE;
   write_to_descriptor(d, (char *)buf, i);
}

/*
 * Minimal copy of process_gmcp_subneg package-name parsing.
 */
static void process_gmcp_subneg(DESCRIPTOR_DATA *d, unsigned char *payload, int len)
{
   char pkg[128];
   int pi = 0;
   int i = 0;

   if (!d->gmcp_active)
      d->gmcp_active = 1;

   while (i < len && payload[i] != ' ' && pi < (int)sizeof(pkg) - 1)
      pkg[pi++] = (char)payload[i++];
   pkg[pi] = '\0';

   if (!strcmp(pkg, "Core.Supports.Set") || !strcmp(pkg, "Core.Supports.Add"))
   {
      char data[1024];
      int di = 0;
      while (i < len && di < (int)sizeof(data) - 1)
         data[di++] = (char)payload[i++];
      data[di] = '\0';
      if (strstr(data, "Char"))
         d->gmcp_supports |= GMCP_PKG_CHAR;
      if (strstr(data, "Room"))
         d->gmcp_supports |= GMCP_PKG_ROOM;
      if (strstr(data, "Comm"))
         d->gmcp_supports |= GMCP_PKG_COMM;
   }
   else if (!strcmp(pkg, "Core.Supports.Remove"))
   {
      char data[1024];
      int di = 0;
      while (i < len && di < (int)sizeof(data) - 1)
         data[di++] = (char)payload[i++];
      data[di] = '\0';
      if (strstr(data, "Char"))
         d->gmcp_supports &= ~GMCP_PKG_CHAR;
      if (strstr(data, "Room"))
         d->gmcp_supports &= ~GMCP_PKG_ROOM;
      if (strstr(data, "Comm"))
         d->gmcp_supports &= ~GMCP_PKG_COMM;
   }
}

/* ---- tests ---- */

static void test_gmcp_send_framing(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));
   reset_capture();

   gmcp_send(&d, "Char.Vitals", "{\"hp\":100}");

   /* Must start with IAC SB GMCP */
   assert(capture_len >= 5);
   assert((unsigned char)capture_buf[0] == IAC);
   assert((unsigned char)capture_buf[1] == SB);
   assert((unsigned char)capture_buf[2] == TELOPT_GMCP);

   /* Must end with IAC SE */
   assert((unsigned char)capture_buf[capture_len - 2] == IAC);
   assert((unsigned char)capture_buf[capture_len - 1] == SE);

   /* Package name present in output */
   assert(memchr(capture_buf, 'C', capture_len) != NULL);
}

static void test_gmcp_send_contains_json(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));
   reset_capture();

   gmcp_send(&d, "Room.Info", "{\"vnum\":1234}");

   /* JSON payload is present */
   capture_buf[capture_len] = '\0';
   assert(strstr((char *)capture_buf, "1234") != NULL);
}

static void test_gmcp_subneg_char_support(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));

   const char *payload_str = "Core.Supports.Set [\"Char 1\",\"Room 1\"]";
   process_gmcp_subneg(&d, (unsigned char *)payload_str, (int)strlen(payload_str));

   assert(d.gmcp_active == 1);
   assert(d.gmcp_supports & GMCP_PKG_CHAR);
   assert(d.gmcp_supports & GMCP_PKG_ROOM);
   assert(!(d.gmcp_supports & GMCP_PKG_COMM));
}

static void test_gmcp_subneg_add(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));

   const char *p1 = "Core.Supports.Add [\"Char 1\"]";
   process_gmcp_subneg(&d, (unsigned char *)p1, (int)strlen(p1));
   assert(d.gmcp_supports & GMCP_PKG_CHAR);
   assert(!(d.gmcp_supports & GMCP_PKG_COMM));

   const char *p2 = "Core.Supports.Add [\"Comm 1\"]";
   process_gmcp_subneg(&d, (unsigned char *)p2, (int)strlen(p2));
   assert(d.gmcp_supports & GMCP_PKG_CHAR);
   assert(d.gmcp_supports & GMCP_PKG_COMM);
}

static void test_gmcp_subneg_remove(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));
   d.gmcp_active = 1;
   d.gmcp_supports = GMCP_PKG_CHAR | GMCP_PKG_ROOM | GMCP_PKG_COMM;

   const char *payload_str = "Core.Supports.Remove [\"Room 1\"]";
   process_gmcp_subneg(&d, (unsigned char *)payload_str, (int)strlen(payload_str));

   assert(d.gmcp_supports & GMCP_PKG_CHAR);
   assert(!(d.gmcp_supports & GMCP_PKG_ROOM));
   assert(d.gmcp_supports & GMCP_PKG_COMM);
}

static void test_gmcp_bitmask_independence(void)
{
   unsigned int flags = 0;
   flags |= GMCP_PKG_CHAR;
   flags |= GMCP_PKG_COMM;

   assert(flags & GMCP_PKG_CHAR);
   assert(!(flags & GMCP_PKG_ROOM));
   assert(flags & GMCP_PKG_COMM);

   flags &= ~GMCP_PKG_CHAR;
   assert(!(flags & GMCP_PKG_CHAR));
   assert(flags & GMCP_PKG_COMM);
}

static void test_gmcp_last_room_tracking(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));

   /* Simulate room change detection */
   unsigned short new_room = 1000;
   assert(d.gmcp_last_room != new_room);
   d.gmcp_last_room = new_room;
   assert(d.gmcp_last_room == 1000);

   /* Same room — no change */
   unsigned short same = 1000;
   assert(d.gmcp_last_room == same); /* would not trigger re-send */
}

int main(void)
{
   test_gmcp_send_framing();
   test_gmcp_send_contains_json();
   test_gmcp_subneg_char_support();
   test_gmcp_subneg_add();
   test_gmcp_subneg_remove();
   test_gmcp_bitmask_independence();
   test_gmcp_last_room_tracking();

   puts("test_gmcp: all tests passed");
   return 0;
}
