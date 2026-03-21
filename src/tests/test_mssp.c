/*
 * Unit tests for MSSP and MSDP protocol framing helpers.
 *
 * These tests exercise the pure byte-framing and bitmask logic without
 * needing a real socket. write_to_descriptor() is stubbed to capture output.
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Telnet constants needed by the functions under test */
#define IAC 255
#define SB 250
#define SE 240
#define WILL 251
#define WONT 252
#define DO 253
#define DONT 254
#define GA 249
#define NOP 241

/* Protocol option numbers (mirrors socket.h) */
#define TELOPT_MSSP 70
#define TELOPT_MSDP 69
#define TELOPT_GMCP 201
#define TELOPT_MCCP2 86
#define TELOPT_MCCP3 87

/* MSSP byte markers */
#define MSSP_BYTE_VAR 1
#define MSSP_BYTE_VAL 2

/* MSDP byte markers */
#define MSDP_BYTE_VAR 1
#define MSDP_BYTE_VAL 2
#define MSDP_TABLE_OPEN 3
#define MSDP_TABLE_CLOSE 4
#define MSDP_ARRAY_OPEN 5
#define MSDP_ARRAY_CLOSE 6

/* MSDP subscription bits */
#define MSDP_BIT_HEALTH (1U << 1)
#define MSDP_BIT_HEALTH_MAX (1U << 2)
#define MSDP_BIT_MANA (1U << 3)
#define MSDP_BIT_LEVEL (1U << 7)
#define MSDP_BIT_ROOM_NAME (1U << 20)

/* Minimal stub types — no real game structures needed */
typedef struct descriptor_data DESCRIPTOR_DATA;
struct descriptor_data
{
   int descriptor; /* fd (unused in tests) */
   int msdp_active;
   unsigned int msdp_reported;
};

/* Capture buffer for write_to_descriptor output */
static unsigned char capture_buf[8192];
static int capture_len = 0;

static void reset_capture(void)
{
   capture_len = 0;
   memset(capture_buf, 0, sizeof(capture_buf));
}

/* Stub: write_to_descriptor accumulates output for inspection */
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
   return 1; /* TRUE */
}

/*
 * Minimal copy of msdp_send_var from socket.c for unit-testing the framing.
 */
static void msdp_send_var(DESCRIPTOR_DATA *d, const char *name, const char *value)
{
   unsigned char buf[1024];
   int i = 0;

   buf[i++] = (unsigned char)IAC;
   buf[i++] = (unsigned char)SB;
   buf[i++] = (unsigned char)TELOPT_MSDP;
   buf[i++] = MSDP_BYTE_VAR;
   while (*name)
      buf[i++] = (unsigned char)*name++;
   buf[i++] = MSDP_BYTE_VAL;
   while (*value)
      buf[i++] = (unsigned char)*value++;
   buf[i++] = (unsigned char)IAC;
   buf[i++] = (unsigned char)SE;
   write_to_descriptor(d, (char *)buf, i);
}

static void msdp_send_var_int(DESCRIPTOR_DATA *d, const char *name, int value)
{
   char tmp[32];
   snprintf(tmp, sizeof(tmp), "%d", value);
   msdp_send_var(d, name, tmp);
}

/*
 * Minimal copy of process_telnet_options IAC stripping logic.
 */
static void strip_iac(unsigned char *buf, int *len)
{
   int i = 0, out = 0;

   while (i < *len)
   {
      if (buf[i] != (unsigned char)IAC)
      {
         buf[out++] = buf[i++];
         continue;
      }
      if (i + 1 >= *len)
      {
         buf[out++] = buf[i++];
         break;
      }
      unsigned char cmd = buf[i + 1];
      if (cmd == (unsigned char)IAC)
      {
         buf[out++] = (unsigned char)IAC;
         i += 2;
         continue;
      }
      if (cmd == (unsigned char)GA || cmd == (unsigned char)NOP)
      {
         i += 2;
         continue;
      }
      if ((cmd == (unsigned char)WILL || cmd == (unsigned char)WONT || cmd == (unsigned char)DO ||
           cmd == (unsigned char)DONT) &&
          i + 2 < *len)
      {
         i += 3;
         continue;
      }
      buf[out++] = buf[i++];
   }
   while (i < *len)
      buf[out++] = buf[i++];
   *len = out;
   buf[out] = '\0';
}

/* ---- tests ---- */

static void test_msdp_var_framing(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));
   reset_capture();

   msdp_send_var(&d, "HEALTH", "100");

   /* Should start with IAC SB MSDP */
   assert(capture_len >= 6);
   assert((unsigned char)capture_buf[0] == IAC);
   assert((unsigned char)capture_buf[1] == SB);
   assert((unsigned char)capture_buf[2] == TELOPT_MSDP);
   assert((unsigned char)capture_buf[3] == MSDP_BYTE_VAR);
   /* Should end with IAC SE */
   assert((unsigned char)capture_buf[capture_len - 2] == IAC);
   assert((unsigned char)capture_buf[capture_len - 1] == SE);

   /* Check variable name and value are present */
   assert(memchr(capture_buf, 'H', capture_len) != NULL);
}

static void test_msdp_var_int(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));
   reset_capture();

   msdp_send_var_int(&d, "LEVEL", 42);

   assert(capture_len > 0);
   assert((unsigned char)capture_buf[0] == IAC);
   assert((unsigned char)capture_buf[1] == SB);
   assert((unsigned char)capture_buf[2] == TELOPT_MSDP);
   /* Contains "42" as a string value */
   assert(memchr(capture_buf, '4', capture_len) != NULL);
}

static void test_iac_do_stripped(void)
{
   /* IAC DO MSDP sequence should be stripped from input */
   unsigned char buf[16];
   int len = 0;
   buf[len++] = 'a';
   buf[len++] = (unsigned char)IAC;
   buf[len++] = (unsigned char)DO;
   buf[len++] = (unsigned char)TELOPT_MSDP;
   buf[len++] = 'b';

   strip_iac(buf, &len);

   assert(len == 2);
   assert(buf[0] == 'a');
   assert(buf[1] == 'b');
}

static void test_iac_will_stripped(void)
{
   unsigned char buf[16];
   int len = 0;
   buf[len++] = 'x';
   buf[len++] = (unsigned char)IAC;
   buf[len++] = (unsigned char)WILL;
   buf[len++] = (unsigned char)TELOPT_GMCP;
   buf[len++] = 'y';

   strip_iac(buf, &len);

   assert(len == 2);
   assert(buf[0] == 'x');
   assert(buf[1] == 'y');
}

static void test_iac_ga_stripped(void)
{
   unsigned char buf[16];
   int len = 0;
   buf[len++] = 'A';
   buf[len++] = (unsigned char)IAC;
   buf[len++] = (unsigned char)GA;
   buf[len++] = 'B';

   strip_iac(buf, &len);

   assert(len == 2);
   assert(buf[0] == 'A');
   assert(buf[1] == 'B');
}

static void test_escaped_iac_kept(void)
{
   /* IAC IAC should produce one IAC in output */
   unsigned char buf[16];
   int len = 0;
   buf[len++] = (unsigned char)IAC;
   buf[len++] = (unsigned char)IAC;

   strip_iac(buf, &len);

   assert(len == 1);
   assert((unsigned char)buf[0] == IAC);
}

static void test_plain_text_unchanged(void)
{
   unsigned char buf[32];
   int len = 0;
   const char *text = "hello";
   while (*text)
      buf[len++] = (unsigned char)*text++;

   strip_iac(buf, &len);

   assert(len == 5);
   assert(buf[0] == 'h');
   assert(buf[4] == 'o');
}

static void test_msdp_bitmask(void)
{
   DESCRIPTOR_DATA d;
   memset(&d, 0, sizeof(d));

   d.msdp_reported = 0;
   d.msdp_reported |= MSDP_BIT_HEALTH;
   d.msdp_reported |= MSDP_BIT_LEVEL;

   assert(d.msdp_reported & MSDP_BIT_HEALTH);
   assert(d.msdp_reported & MSDP_BIT_LEVEL);
   assert(!(d.msdp_reported & MSDP_BIT_MANA));

   d.msdp_reported &= ~MSDP_BIT_HEALTH;
   assert(!(d.msdp_reported & MSDP_BIT_HEALTH));
   assert(d.msdp_reported & MSDP_BIT_LEVEL);
}

int main(void)
{
   test_msdp_var_framing();
   test_msdp_var_int();
   test_iac_do_stripped();
   test_iac_will_stripped();
   test_iac_ga_stripped();
   test_escaped_iac_kept();
   test_plain_text_unchanged();
   test_msdp_bitmask();

   puts("test_mssp: all tests passed");
   return 0;
}
