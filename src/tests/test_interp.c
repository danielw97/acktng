#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
 * Minimal struct definition matching the layout of cmd_type in ack.h.
 * We only need the name field, but must reproduce the full struct so
 * sizeof/offsetof match when the linker resolves cmd_table from interp.o.
 */
typedef void DO_FUN_STUB(void *, char *);

struct cmd_type
{
   char *const name;
   DO_FUN_STUB *do_fun;
   short position;
   short level;
   short log;
   short type;
   short show;
   short flags; /* CMD_FLAG_* bitmask */
};

/* Must match config.h CMD_FLAG_NINJA_OK (BIT_1 = 1), CMD_FLAG_WAIT (BIT_2 = 2) */
#define CMD_FLAG_NINJA_OK 1
#define CMD_FLAG_WAIT     2

extern const struct cmd_type cmd_table[];

static int find_cmd(const char *name)
{
   int i;
   for (i = 0; cmd_table[i].name[0] != '\0'; i++)
      if (strcmp(cmd_table[i].name, name) == 0)
         return i;
   return -1;
}

static void test_no_duplicate_commands(void)
{
   int i, j, count;

   /* Find sentinel (empty name string terminates the table). */
   for (count = 0; cmd_table[count].name[0] != '\0'; count++)
      ;

   for (i = 0; i < count; i++)
   {
      for (j = i + 1; j < count; j++)
      {
         if (strcmp(cmd_table[i].name, cmd_table[j].name) == 0)
         {
            fprintf(stderr, "FAIL: duplicate command \"%s\" at indices %d and %d\n",
                    cmd_table[i].name, i, j);
            assert(0);
         }
      }
   }
}

/*
 * These commands must keep ninja stance active (CMD_FLAG_NINJA_OK set).
 */
static void test_ninja_flag_set_on_combat_commands(void)
{
   const char *ninja_cmds[] = {"kill", "murder", "backstab", "bs", "whisper", "steal", NULL};
   int i;

   for (i = 0; ninja_cmds[i] != NULL; i++)
   {
      int idx = find_cmd(ninja_cmds[i]);
      if (idx < 0)
      {
         fprintf(stderr, "FAIL: ninja command \"%s\" not found in cmd_table\n", ninja_cmds[i]);
         assert(0);
      }
      if (!(cmd_table[idx].flags & CMD_FLAG_NINJA_OK))
      {
         fprintf(stderr, "FAIL: command \"%s\" missing CMD_FLAG_NINJA_OK\n", ninja_cmds[i]);
         assert(0);
      }
   }
}

/*
 * Ordinary commands must NOT have CMD_FLAG_NINJA_OK set.
 */
static void test_ninja_flag_absent_from_ordinary_commands(void)
{
   const char *plain_cmds[] = {"look", "score", "cast", "rest", "get", "north", NULL};
   int i;

   for (i = 0; plain_cmds[i] != NULL; i++)
   {
      int idx = find_cmd(plain_cmds[i]);
      if (idx < 0)
      {
         fprintf(stderr, "FAIL: command \"%s\" not found in cmd_table\n", plain_cmds[i]);
         assert(0);
      }
      if (cmd_table[idx].flags & CMD_FLAG_NINJA_OK)
      {
         fprintf(stderr, "FAIL: command \"%s\" unexpectedly has CMD_FLAG_NINJA_OK\n",
                 plain_cmds[i]);
         assert(0);
      }
   }
}

/*
 * Verify the table is terminated by the sentinel entry (empty name).
 */
static void test_sentinel_terminates_table(void)
{
   int i;
   for (i = 0; cmd_table[i].name[0] != '\0'; i++)
      ;
   /* The loop stopped at the sentinel; verify it really is the sentinel. */
   assert(cmd_table[i].name[0] == '\0');
   assert(cmd_table[i].do_fun == NULL);
}

/*
 * Every non-sentinel entry must have a non-NULL handler function.
 */
static void test_all_commands_have_handler(void)
{
   int i;
   for (i = 0; cmd_table[i].name[0] != '\0'; i++)
   {
      if (cmd_table[i].do_fun == NULL)
      {
         fprintf(stderr, "FAIL: command \"%s\" at index %d has NULL do_fun\n", cmd_table[i].name,
                 i);
         assert(0);
      }
   }
}

/*
 * Commands that introduce a wait state must have CMD_FLAG_WAIT set.
 */
static void test_wait_flag_set_on_wait_state_commands(void)
{
   const char *wait_cmds[] = {
      "north", "south", "east", "west", "up", "down",
      "cast", "kill", "murder", "backstab", "bs",
      "kick", "bash", "punch", "trip", "dirt",
      "charge", "fleche", "holystrike",
      "morale", "leadership", "stun",
      "checkpoint break", "road sweep", "dunmar's call", "interior discipline",
      NULL
   };
   int i;

   for (i = 0; wait_cmds[i] != NULL; i++)
   {
      int idx = find_cmd(wait_cmds[i]);
      if (idx < 0)
      {
         fprintf(stderr, "FAIL: wait command \"%s\" not found in cmd_table\n", wait_cmds[i]);
         assert(0);
      }
      if (!(cmd_table[idx].flags & CMD_FLAG_WAIT))
      {
         fprintf(stderr, "FAIL: command \"%s\" missing CMD_FLAG_WAIT\n", wait_cmds[i]);
         assert(0);
      }
   }
}

/*
 * Commands that should NOT queue must NOT have CMD_FLAG_WAIT.
 */
static void test_wait_flag_absent_from_instant_commands(void)
{
   const char *instant_cmds[] = {"look", "score", "say", "inventory", "flee", "recall",
                                  "help", "who", "quit", NULL};
   int i;

   for (i = 0; instant_cmds[i] != NULL; i++)
   {
      int idx = find_cmd(instant_cmds[i]);
      if (idx < 0)
      {
         fprintf(stderr, "FAIL: command \"%s\" not found in cmd_table\n", instant_cmds[i]);
         assert(0);
      }
      if (cmd_table[idx].flags & CMD_FLAG_WAIT)
      {
         fprintf(stderr, "FAIL: command \"%s\" unexpectedly has CMD_FLAG_WAIT\n", instant_cmds[i]);
         assert(0);
      }
   }
}

int main(void)
{
   test_no_duplicate_commands();
   test_ninja_flag_set_on_combat_commands();
   test_ninja_flag_absent_from_ordinary_commands();
   test_sentinel_terminates_table();
   test_all_commands_have_handler();
   test_wait_flag_set_on_wait_state_commands();
   test_wait_flag_absent_from_instant_commands();
   puts("test_interp: all tests passed");
   return 0;
}
