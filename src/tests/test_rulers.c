#include <assert.h>
#include <string.h>

const char *get_ruler_title(int ruler_rank, int sex);
size_t rulers_format_status_message(char *dest, size_t dest_size, const char *status,
                                    const char *filename);

#define SEX_NEUTRAL 0
#define SEX_MALE 1
#define SEX_FEMALE 2

static void test_get_ruler_title_by_rank_and_sex(void)
{
   assert(strcmp(get_ruler_title(1, SEX_NEUTRAL), "@@WMonitor@@N ") == 0);
   assert(strcmp(get_ruler_title(2, SEX_MALE), "@@eBaron@@N ") == 0);
   assert(strcmp(get_ruler_title(3, SEX_FEMALE), "@@mDuchess@@N ") == 0);
   assert(strcmp(get_ruler_title(4, SEX_MALE), "@@lPrince@@N ") == 0);
   assert(strcmp(get_ruler_title(5, SEX_FEMALE), "@@aQueen@@N ") == 0);
}

static void test_get_ruler_title_default_rank(void)
{
   assert(strcmp(get_ruler_title(0, SEX_MALE), "") == 0);
   assert(strcmp(get_ruler_title(42, SEX_MALE), "Lord") == 0);
}

static void test_rulers_format_status_message_handles_truncation_and_nulls(void)
{
   char buf[32];
   size_t written;

   written = rulers_format_status_message(buf, sizeof(buf), "Loading", "rulers.lst");
   assert(written == strlen("Loading rulers.lst\n\r"));
   assert(strcmp(buf, "Loading rulers.lst\n\r") == 0);

   written = rulers_format_status_message(buf, 10, "Done", "very_long_filename");
   assert(written >= 10);
   assert(buf[9] == '\0');

   written = rulers_format_status_message(buf, sizeof(buf), NULL, NULL);
   assert(written == strlen(" \n\r"));
   assert(strcmp(buf, " \n\r") == 0);

   assert(rulers_format_status_message(NULL, sizeof(buf), "x", "y") == 0);
   assert(rulers_format_status_message(buf, 0, "x", "y") == 0);
}

int main(void)
{
   test_get_ruler_title_by_rank_and_sex();
   test_get_ruler_title_default_rank();
   test_rulers_format_status_message_handles_truncation_and_nulls();
   return 0;
}
