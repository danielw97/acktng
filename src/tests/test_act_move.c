#include <assert.h>
#include <string.h>

#include "globals.h"

OBJ_DATA *has_key(CHAR_DATA *ch, int key);

static void test_has_key_finds_matching_key_in_carry_list(void)
{
   CHAR_DATA ch;
   OBJ_DATA first;
   OBJ_DATA second;
   OBJ_INDEX_DATA first_index;
   OBJ_INDEX_DATA second_index;

   memset(&ch, 0, sizeof(ch));
   memset(&first, 0, sizeof(first));
   memset(&second, 0, sizeof(second));
   memset(&first_index, 0, sizeof(first_index));
   memset(&second_index, 0, sizeof(second_index));

   first_index.vnum = 1001;
   second_index.vnum = 2002;

   first.pIndexData = &first_index;
   second.pIndexData = &second_index;
   first.next_in_carry_list = &second;

   ch.first_carry = &first;

   assert(has_key(&ch, 2002) == &second);
   assert(has_key(&ch, 1001) == &first);
}

static void test_has_key_returns_null_when_not_found_or_empty_inventory(void)
{
   CHAR_DATA ch;
   OBJ_DATA held;
   OBJ_INDEX_DATA held_index;

   memset(&ch, 0, sizeof(ch));
   memset(&held, 0, sizeof(held));
   memset(&held_index, 0, sizeof(held_index));

   held_index.vnum = 3003;
   held.pIndexData = &held_index;
   ch.first_carry = &held;

   assert(has_key(&ch, 4040) == NULL);

   ch.first_carry = NULL;
   assert(has_key(&ch, 3003) == NULL);
}

int main(void)
{
   test_has_key_finds_matching_key_in_carry_list();
   test_has_key_returns_null_when_not_found_or_empty_inventory();
   return 0;
}
