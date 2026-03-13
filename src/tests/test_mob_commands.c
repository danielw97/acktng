#include <assert.h>
#include <string.h>

char *mprog_type_to_name(int type);

enum
{
   IN_FILE_PROG = 0,
   ACT_PROG = 1,
   SPEECH_PROG = 2,
   RAND_PROG = 4,
   FIGHT_PROG = 8,
   DEATH_PROG = 16,
   HITPRCNT_PROG = 32,
   ENTRY_PROG = 64,
   GREET_PROG = 128,
   ALL_GREET_PROG = 256,
   GIVE_PROG = 512,
   BRIBE_PROG = 1024
};

static void test_mprog_type_to_name_known_types(void)
{
   assert(strcmp(mprog_type_to_name(IN_FILE_PROG), "in_file_prog") == 0);
   assert(strcmp(mprog_type_to_name(ACT_PROG), "act_prog") == 0);
   assert(strcmp(mprog_type_to_name(SPEECH_PROG), "speech_prog") == 0);
   assert(strcmp(mprog_type_to_name(RAND_PROG), "rand_prog") == 0);
   assert(strcmp(mprog_type_to_name(FIGHT_PROG), "fight_prog") == 0);
   assert(strcmp(mprog_type_to_name(HITPRCNT_PROG), "hitprcnt_prog") == 0);
   assert(strcmp(mprog_type_to_name(DEATH_PROG), "death_prog") == 0);
   assert(strcmp(mprog_type_to_name(ENTRY_PROG), "entry_prog") == 0);
   assert(strcmp(mprog_type_to_name(GREET_PROG), "greet_prog") == 0);
   assert(strcmp(mprog_type_to_name(ALL_GREET_PROG), "all_greet_prog") == 0);
   assert(strcmp(mprog_type_to_name(GIVE_PROG), "give_prog") == 0);
   assert(strcmp(mprog_type_to_name(BRIBE_PROG), "bribe_prog") == 0);
}

static void test_mprog_type_to_name_unknown_type(void)
{
   assert(strcmp(mprog_type_to_name(-1), "ERROR_PROG") == 0);
   assert(strcmp(mprog_type_to_name(3), "ERROR_PROG") == 0);
}

int main(void)
{
   test_mprog_type_to_name_known_types();
   test_mprog_type_to_name_unknown_type();
   return 0;
}
