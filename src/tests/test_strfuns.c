#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"

char *get_adept_name(CHAR_DATA *ch);
int nocol_strlen(const char *text);

/* Minimal gclass_table: entries CLASS_GMA through CLASS_GMA+MAX_CLASS-1 are ADEPT */
const struct class_type gclass_table[MAX_TOTAL_CLASS] = {
   [CLASS_GMA + 0] = {.tier = ADEPT},
   [CLASS_GMA + 1] = {.tier = ADEPT},
   [CLASS_GMA + 2] = {.tier = ADEPT},
   [CLASS_GMA + 3] = {.tier = ADEPT},
   [CLASS_GMA + 4] = {.tier = ADEPT},
   [CLASS_GMA + 5] = {.tier = ADEPT},
};

static void clear_character(CHAR_DATA *ch)
{
   memset(ch, 0, sizeof(*ch));
}

static void test_nocol_strlen_strips_color_codes(void)
{
   assert(nocol_strlen("Hello") == 5);
   assert(nocol_strlen("@@bHello@@N") == 5);
   assert(nocol_strlen("@@r Red @@N text @@g") == 11);
   assert(nocol_strlen("") == 0);
}

static void test_get_adept_name_fallback_is_17_visible(void)
{
   CHAR_DATA ch;
   clear_character(&ch);
   /* No adept levels set: returns generic fallback */
   char *name = get_adept_name(&ch);
   int visible = nocol_strlen(name);
   if (visible != 17)
      printf("FAIL fallback: '%s' visible=%d (expected 17)\n", name, visible);
   assert(visible == 17);
}

static void test_get_adept_name_realm_lord_is_17_visible(void)
{
   CHAR_DATA ch;
   clear_character(&ch);
   /* Set a level beyond MAX_ADEPT to trigger "Realm Lord" return */
   ch.class_level[CLASS_GMA] = MAX_ADEPT + 1;
   char *name = get_adept_name(&ch);
   int visible = nocol_strlen(name);
   if (visible != 17)
      printf("FAIL realm lord: '%s' visible=%d (expected 17)\n", name, visible);
   assert(visible == 17);
}

static void test_get_adept_name_all_titles_are_17_visible(void)
{
   CHAR_DATA ch;
   for (int cls = 0; cls < MAX_CLASS; cls++)
   {
      for (int level = 1; level <= MAX_ADEPT; level++)
      {
         clear_character(&ch);
         ch.class_level[CLASS_GMA + cls] = level;
         char *name = get_adept_name(&ch);
         int visible = nocol_strlen(name);
         if (visible != 17)
            printf("FAIL class=%d level=%d: '%s' visible=%d (expected 17)\n", cls, level, name, visible);
         assert(visible == 17);
      }
   }
}

int main(void)
{
   test_nocol_strlen_strips_color_codes();
   test_get_adept_name_fallback_is_17_visible();
   test_get_adept_name_realm_lord_is_17_visible();
   test_get_adept_name_all_titles_are_17_visible();
   puts("test_strfuns: all tests passed");
   return 0;
}
