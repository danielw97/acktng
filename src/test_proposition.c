#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"

void proposition_obj_notify(CHAR_DATA *ch, OBJ_DATA *obj);

static int extracted_calls;
static int save_calls;
static OBJ_DATA *last_extracted;

void extract_obj(OBJ_DATA *obj)
{
    extracted_calls++;
    last_extracted = obj;
}

void do_save(CHAR_DATA *ch, char *argument)
{
    (void)ch;
    (void)argument;
    save_calls++;
}

void send_to_char(const char *txt, CHAR_DATA *ch)
{
    (void)txt;
    (void)ch;
}

static void reset_counters(void)
{
    extracted_calls = 0;
    save_calls = 0;
    last_extracted = NULL;
}

static CHAR_DATA make_char(PC_DATA *pc)
{
    CHAR_DATA ch;
    memset(&ch, 0, sizeof(ch));
    memset(pc, 0, sizeof(*pc));
    ch.pcdata = pc;
    ch.act = 0;
    ch.pcdata->prop_type = PROP_TYPE_COLLECT_ITEMS;
    return ch;
}

static OBJ_DATA make_obj(OBJ_INDEX_DATA *index, int vnum, const char *short_descr)
{
    OBJ_DATA obj;
    memset(&obj, 0, sizeof(obj));
    memset(index, 0, sizeof(*index));
    index->vnum = vnum;
    obj.pIndexData = index;
    obj.short_descr = (char *)short_descr;
    return obj;
}

static void test_extracts_and_saves_when_target_matches(void)
{
    PC_DATA pc;
    OBJ_INDEX_DATA index;
    CHAR_DATA ch = make_char(&pc);
    OBJ_DATA obj = make_obj(&index, 1234, "a target item");

    ch.pcdata->prop_num_targets = 1;
    ch.pcdata->prop_target_vnum[0] = 1234;
    ch.pcdata->prop_target_done[0] = FALSE;

    reset_counters();
    proposition_obj_notify(&ch, &obj);

    assert(ch.pcdata->prop_target_done[0] == TRUE);
    assert(extracted_calls == 1);
    assert(last_extracted == &obj);
    assert(save_calls == 1);
}

static void test_no_extract_when_not_a_target(void)
{
    PC_DATA pc;
    OBJ_INDEX_DATA index;
    CHAR_DATA ch = make_char(&pc);
    OBJ_DATA obj = make_obj(&index, 5678, "other item");

    ch.pcdata->prop_num_targets = 1;
    ch.pcdata->prop_target_vnum[0] = 1234;

    reset_counters();
    proposition_obj_notify(&ch, &obj);

    assert(ch.pcdata->prop_target_done[0] == FALSE);
    assert(extracted_calls == 0);
    assert(save_calls == 0);
}

static void test_marks_completed_when_final_item_collected(void)
{
    PC_DATA pc;
    OBJ_INDEX_DATA index;
    CHAR_DATA ch = make_char(&pc);
    OBJ_DATA obj = make_obj(&index, 2222, "final target");

    ch.pcdata->prop_num_targets = 2;
    ch.pcdata->prop_target_vnum[0] = 1111;
    ch.pcdata->prop_target_done[0] = TRUE;
    ch.pcdata->prop_target_vnum[1] = 2222;

    reset_counters();
    proposition_obj_notify(&ch, &obj);

    assert(ch.pcdata->prop_target_done[1] == TRUE);
    assert(ch.pcdata->prop_completed == TRUE);
    assert(extracted_calls == 1);
    assert(save_calls == 1);
}

int main(void)
{
    test_extracts_and_saves_when_target_matches();
    test_no_extract_when_not_a_target();
    test_marks_completed_when_final_item_collected();

    puts("test_proposition: all tests passed");
    return 0;
}
