#ifndef PROMPT_TEST_H
#define PROMPT_TEST_H

/*
 * Declarations for prompt.c internal functions that are accessible for testing.
 * These functions are non-static so they can be called from test binaries.
 */

typedef struct char_data CHAR_DATA;

bool prompt_should_show_hp(CHAR_DATA *ch);
bool prompt_should_show_mana(CHAR_DATA *ch);
bool prompt_should_show_move(CHAR_DATA *ch);
long prompt_max_value_for_code(CHAR_DATA *ch, char code);
void format_builder_prompt(char *dest, size_t dest_size, const char *mode, const char *details);

#endif /* PROMPT_TEST_H */
