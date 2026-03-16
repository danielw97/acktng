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
};

extern const struct cmd_type cmd_table[];

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

int main(void)
{
   test_no_duplicate_commands();
   puts("test_interp: all tests passed");
   return 0;
}
