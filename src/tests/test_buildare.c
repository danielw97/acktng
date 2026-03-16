#include <assert.h>
#include <string.h>

void buildare_testable_append_permission(char *dest, size_t dest_size, const char *existing,
                                         const char *name);

int main(void)
{
   char buf[16];

   buildare_testable_append_permission(buf, sizeof(buf), "alice", "bob");
   assert(strcmp(buf, "alice bob") == 0);

   buildare_testable_append_permission(buf, 8, "abcdef", "ghijk");
   assert(buf[7] == '\0');
   return 0;
}
