#ifndef COMM_LOGIN_TEST_H
#define COMM_LOGIN_TEST_H

#include <stdbool.h>

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
   bool is_name_valid,
   bool is_existing_player,
   bool is_password_correct);

#endif
