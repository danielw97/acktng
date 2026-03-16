#include <assert.h>
#include <stdio.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

bool valid_email_addy(char *address);

static void test_accepts_common_email_patterns(void)
{
   char addr1[] = "player@example.com";
   char addr2[] = "a_b.c@foo.bar";

   assert(valid_email_addy(addr1));
   assert(valid_email_addy(addr2));
}

static void test_rejects_disallowed_characters(void)
{
   char addr1[] = "player+tag@example.com";
   char addr2[] = "player-name@example.com";
   char addr3[] = "player@example,com";

   assert(!valid_email_addy(addr1));
   assert(!valid_email_addy(addr2));
   assert(!valid_email_addy(addr3));
}

int main(void)
{
   test_accepts_common_email_patterns();
   test_rejects_disallowed_characters();

   puts("test_email: all tests passed");
   return 0;
}
