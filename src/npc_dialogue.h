#ifndef NPC_DIALOGUE_H
#define NPC_DIALOGUE_H

#ifndef DEC_ACK_H
#include "ack.h"
#endif

/*
 * Initialize the NPC dialogue system.
 * Parses the TNGAI_URL and spawns the worker thread.
 * Must be called once from main() after boot_db().
 */
void npc_dialogue_init(void);

/*
 * Dispatch an NPC dialogue request.
 * Called from do_say() when a player speaks in a room with an ACT_AI_DIALOGUE NPC.
 * Returns immediately; the actual HTTP call happens on the worker thread.
 */
void npc_dialogue_dispatch(CHAR_DATA *npc, CHAR_DATA *player, const char *message);

/*
 * Sanitize player input before dispatch.
 * Strips model role-boundary tokens ([INST], <<SYS>>, <|system|>, etc.) and
 * replaces angle brackets with parentheses to prevent prompt injection.
 * dst must be at least 512 bytes; src and dst must not overlap.
 */
void npc_dialogue_sanitize_input(char *dst, const char *src);

/*
 * Deliver pending NPC dialogue responses.
 * Called from update_handler() at the top of each tick.
 * Drains the response queue and calls do_say() for each valid response.
 */
void npc_dialogue_deliver(void);

/*
 * Unit-test only: exposes collect_help_context when compiled with
 * -DUNIT_TEST_NPC_DIALOGUE.
 */
#ifdef UNIT_TEST_NPC_DIALOGUE
void npc_dialogue_test_collect_help_context(const char *message, char *out, size_t cap);
#endif

#endif /* NPC_DIALOGUE_H */
