#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

bool str_cmp(const char *s1, const char *s2)
{
    if (s1 == NULL && s2 == NULL)
        return FALSE;
    if (s1 == NULL || s2 == NULL)
        return TRUE;
    return (strcasecmp(s1, s2) != 0) ? TRUE : FALSE;
}

void keep_format_chest_short_descr(const char *owner_name, char *dest, size_t dest_size);
void keep_format_room_name(const char *owner_name, char *dest, size_t dest_size);
void keep_format_room_description(const char *owner_name, char *dest, size_t dest_size);
int keep_chest_max_items(void);
int keep_storage_tier_for_test(int max_items);
int keep_storage_upgrade_cost_for_test(int current_max_items);
int keep_storage_next_max_items_for_test(int current_max_items);
int keep_is_customization_command(const char *arg);
int keep_is_upgrade_command(const char *arg);
int keep_player_can_customize(const CHAR_DATA *ch);

static void test_keep_chest_name_uses_creator(void)
{
    char buf[128];

    keep_format_chest_short_descr("Creator", buf, sizeof(buf));
    assert(strcmp(buf, "Creator's Keep Chest") == 0);
}

static void test_keep_chest_name_handles_missing_owner(void)
{
    char buf[128];

    keep_format_chest_short_descr(NULL, buf, sizeof(buf));
    assert(strcmp(buf, "Unknown's Keep Chest") == 0);
}

static void test_keep_chest_name_handles_empty_owner(void)
{
    char buf[128];

    keep_format_chest_short_descr("", buf, sizeof(buf));
    assert(strcmp(buf, "Unknown's Keep Chest") == 0);
}

static void test_keep_chest_name_truncates_to_fit_buffer(void)
{
    char buf[8];

    keep_format_chest_short_descr("Creator", buf, sizeof(buf));
    assert(strcmp(buf, "Creator") == 0);
}

static void test_keep_chest_name_noops_for_null_dest(void)
{
    keep_format_chest_short_descr("Creator", NULL, 16);
}

static void test_keep_chest_name_noops_for_zero_dest_size(void)
{
    char buf[8] = "init";

    keep_format_chest_short_descr("Creator", buf, 0);
    assert(strcmp(buf, "init") == 0);
}

static void test_keep_chest_max_items(void)
{
    assert(keep_chest_max_items() == 50);
}

static void test_keep_storage_tier_calculation(void)
{
    assert(keep_storage_tier_for_test(50) == 0);
    assert(keep_storage_tier_for_test(51) == 1);
    assert(keep_storage_tier_for_test(55) == 1);
    assert(keep_storage_tier_for_test(56) == 2);
    assert(keep_storage_tier_for_test(60) == 2);
    assert(keep_storage_tier_for_test(65) == 3);
}

static void test_keep_storage_upgrade_cost_tiers(void)
{
    assert(keep_storage_upgrade_cost_for_test(50) == 50);
    assert(keep_storage_upgrade_cost_for_test(55) == 100);
    assert(keep_storage_upgrade_cost_for_test(60) == 150);
    assert(keep_storage_upgrade_cost_for_test(65) == 200);
}

static void test_keep_storage_next_max_items_steps_by_five(void)
{
    assert(keep_storage_next_max_items_for_test(45) == 55);
    assert(keep_storage_next_max_items_for_test(50) == 55);
    assert(keep_storage_next_max_items_for_test(55) == 60);
    assert(keep_storage_next_max_items_for_test(60) == 65);
}

static void test_keep_room_name_uses_creator(void)
{
    char buf[128];

    keep_format_room_name("Playername", buf, sizeof(buf));
    assert(strcmp(buf, "Playername's Keep") == 0);
}

static void test_keep_room_name_handles_missing_owner(void)
{
    char buf[128];

    keep_format_room_name(NULL, buf, sizeof(buf));
    assert(strcmp(buf, "Unknown's Keep") == 0);
}

static void test_keep_room_description_uses_creator(void)
{
    char buf[128];

    keep_format_room_description("Playername", buf, sizeof(buf));
    assert(strcmp(buf, "Keep of Playername") == 0);
}

static void test_keep_room_description_handles_missing_owner(void)
{
    char buf[128];

    keep_format_room_description("", buf, sizeof(buf));
    assert(strcmp(buf, "Keep of Unknown") == 0);
}

static void test_keep_customization_command_detection(void)
{
    assert(keep_is_customization_command("title") != 0);
    assert(keep_is_customization_command("desc") != 0);
    assert(keep_is_customization_command("create") == 0);
    assert(keep_is_customization_command(NULL) == 0);
}


static void test_keep_upgrade_command_detection(void)
{
    assert(keep_is_upgrade_command("regen") != 0);
    assert(keep_is_upgrade_command("inside") != 0);
    assert(keep_is_upgrade_command("storage") != 0);
    assert(keep_is_upgrade_command("create") == 0);
    assert(keep_is_upgrade_command(NULL) == 0);
}

static void test_keep_player_can_customize(void)
{
    PC_DATA pcdata;
    ROOM_INDEX_DATA room;
    CHAR_DATA ch;

    memset(&pcdata, 0, sizeof(pcdata));
    memset(&room, 0, sizeof(room));
    memset(&ch, 0, sizeof(ch));

    pcdata.keep_vnum = 1234;
    room.vnum = 1234;
    ch.pcdata = &pcdata;
    ch.in_room = &room;

    assert(keep_player_can_customize(&ch) != 0);

    room.vnum = 1235;
    assert(keep_player_can_customize(&ch) == 0);

    ch.in_room = NULL;
    assert(keep_player_can_customize(&ch) == 0);

    ch.in_room = &room;
    ch.pcdata = NULL;
    assert(keep_player_can_customize(&ch) == 0);
}


int main(void)
{
    test_keep_chest_name_uses_creator();
    test_keep_chest_name_handles_missing_owner();
    test_keep_chest_name_handles_empty_owner();
    test_keep_chest_name_truncates_to_fit_buffer();
    test_keep_chest_name_noops_for_null_dest();
    test_keep_chest_name_noops_for_zero_dest_size();
    test_keep_chest_max_items();
    test_keep_storage_tier_calculation();
    test_keep_storage_upgrade_cost_tiers();
    test_keep_storage_next_max_items_steps_by_five();
    test_keep_room_name_uses_creator();
    test_keep_room_name_handles_missing_owner();
    test_keep_room_description_uses_creator();
    test_keep_room_description_handles_missing_owner();
    test_keep_customization_command_detection();
    test_keep_upgrade_command_detection();
    test_keep_player_can_customize();
    puts("test_keep: all tests passed");
    return 0;
}
