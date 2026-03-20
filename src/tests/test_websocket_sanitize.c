#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Minimal telnet constants needed by the function under test */
#define IAC  255
#define GA   249
#define WILL 251
#define WONT 252
#define DO   253
#define DONT 254

/*
 * This is a copy of sanitize_websocket_text_payload() from socket.c.
 * It must be kept in sync with the production function.
 */
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

/* ---- helpers ---- */

static size_t sanitize_str(const char *input, char *out, size_t out_cap)
{
   return sanitize_websocket_text_payload((const unsigned char *)input, strlen(input),
                                          (unsigned char *)out, out_cap);
}

/* ---- tests ---- */

static void test_newline_cr_becomes_newline(void)
{
   char out[64];
   size_t n = sanitize_str("line1\n\rline2\n\r", out, sizeof(out));
   out[n] = '\0';
   assert(n == 12);
   assert(strcmp(out, "line1\nline2\n") == 0);
}

static void test_cr_lf_becomes_lf(void)
{
   char out[64];
   size_t n = sanitize_str("a\r\nb", out, sizeof(out));
   out[n] = '\0';
   assert(n == 3);
   assert(strcmp(out, "a\nb") == 0);
}

static void test_bare_cr_stripped(void)
{
   char out[64];
   size_t n = sanitize_str("a\rb", out, sizeof(out));
   out[n] = '\0';
   assert(n == 2);
   assert(strcmp(out, "ab") == 0);
}

static void test_bare_lf_kept(void)
{
   char out[64];
   size_t n = sanitize_str("a\nb", out, sizeof(out));
   out[n] = '\0';
   assert(n == 3);
   assert(strcmp(out, "a\nb") == 0);
}

static void test_plain_text_unchanged(void)
{
   char out[64];
   size_t n = sanitize_str("hello world", out, sizeof(out));
   out[n] = '\0';
   assert(n == 11);
   assert(strcmp(out, "hello world") == 0);
}

static void test_iac_ga_stripped(void)
{
   unsigned char input[] = {'A', IAC, GA, 'B'};
   unsigned char out[64];
   size_t n =
       sanitize_websocket_text_payload(input, sizeof(input), out, sizeof(out));
   assert(n == 2);
   assert(out[0] == 'A');
   assert(out[1] == 'B');
}

static void test_iac_will_stripped(void)
{
   unsigned char input[] = {'X', IAC, WILL, 1, 'Y'};
   unsigned char out[64];
   size_t n =
       sanitize_websocket_text_payload(input, sizeof(input), out, sizeof(out));
   assert(n == 2);
   assert(out[0] == 'X');
   assert(out[1] == 'Y');
}

static void test_null_inputs_return_zero(void)
{
   unsigned char out[8];
   assert(sanitize_websocket_text_payload(NULL, 4, out, sizeof(out)) == 0);
   assert(sanitize_websocket_text_payload((unsigned char *)"x", 1, NULL, 0) == 0);
}

int main(void)
{
   test_newline_cr_becomes_newline();
   test_cr_lf_becomes_lf();
   test_bare_cr_stripped();
   test_bare_lf_kept();
   test_plain_text_unchanged();
   test_iac_ga_stripped();
   test_iac_will_stripped();
   test_null_inputs_return_zero();

   puts("test_websocket_sanitize: all tests passed");
   return 0;
}
