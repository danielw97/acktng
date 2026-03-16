#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

void mapper_test_append_color_display(char *dest, int max_len, const char *color,
                                      const char *display);

void safe_strcat(int max_len, char *dest, char *source)
{
   if (max_len <= 0 || dest == NULL || source == NULL)
   {
      return;
   }

   strncat(dest, source, (size_t)(max_len - (int)strlen(dest) - 1));
   dest[max_len - 1] = '\0';
}

static void test_append_color_display_combines_text(void)
{
   char out[64] = "";

   mapper_test_append_color_display(out, sizeof(out), "@@R", "x");
   assert(strcmp(out, "@@Rx") == 0);
}

static void test_append_color_display_respects_buffer_size(void)
{
   char out[16] = "prefix";
   char big[64];

   memset(big, 'z', sizeof(big) - 1);
   big[sizeof(big) - 1] = '\0';

   mapper_test_append_color_display(out, sizeof(out), big, big);
   assert(out[sizeof(out) - 1] == '\0');
}

int main(void)
{
   test_append_color_display_combines_text();
   test_append_color_display_respects_buffer_size();

   puts("test_mapper: all tests passed");
   return 0;
}
