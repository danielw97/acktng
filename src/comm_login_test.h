#ifndef COMM_LOGIN_TEST_H
#define COMM_LOGIN_TEST_H

typedef enum
{
   LOGIN_TRANSITION_NAME_REJECTED,
   LOGIN_TRANSITION_PASSWORD_PROMPTED,
   LOGIN_TRANSITION_WRONG_PASSWORD,
   LOGIN_TRANSITION_SHOW_MOTD,
   LOGIN_TRANSITION_ENTERED_GAME
} LOGIN_TRANSITION_RESULT;

LOGIN_TRANSITION_RESULT simulate_existing_player_login_transition(
   int *connected_state,
   const char *input,
   int is_name_valid,
   int is_existing_player,
   int is_password_correct);


typedef struct char_data CHAR_DATA;

bool should_show_default_prompt_hp(CHAR_DATA *ch);
bool should_show_default_prompt_mana(CHAR_DATA *ch);
bool should_show_default_prompt_move(CHAR_DATA *ch);
long prompt_max_value_for_code(CHAR_DATA *ch, char code);

#endif
