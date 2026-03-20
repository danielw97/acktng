#ifndef NPC_DIALOGUE_H
#define NPC_DIALOGUE_H

#ifndef DEC_ACK_H
#include "ack.h"
#endif

/*
 * Delivery routing for NPC dialogue responses.
 * DLG_DELIVER_SAY     — normal room say (do_say); used by ACT_AI_DIALOGUE mobs.
 * DLG_DELIVER_CRUSADE — crusade channel (do_crusade); used for quest-mob
 *                       responses dispatched via npc_dialogue_dispatch_crusade().
 */
typedef enum
{
   DLG_DELIVER_SAY,
   DLG_DELIVER_CRUSADE
} dlg_delivery_t;

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
 * Dispatch a crusade-channel NPC dialogue request.
 * Like npc_dialogue_dispatch() but routes the response through do_crusade()
 * rather than do_say(), and appends quest_context to the system prompt so the
 * NPC is aware of the current crusade state.  quest_context may be NULL.
 */
void npc_dialogue_dispatch_crusade(CHAR_DATA *npc, CHAR_DATA *player, const char *message,
                                   const char *quest_context);

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
 * Drains the response queue and calls do_say() or do_crusade() depending on
 * the delivery field set at dispatch time.
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
