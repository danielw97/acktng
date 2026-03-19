#ifndef DEC_SENTINEL_H
#define DEC_SENTINEL_H 1

#ifndef DEC_ACK_H
#include "ack.h"
#endif

/* Testimony system constants */
#define MAX_TESTIMONY 9
#define TESTIMONY_PASSIVE_INTERVAL 3 /* gain +1 every N combat rounds */
#define VERDICT_COOLDOWN_ROUNDS 2

/* Testimony management */
void add_testimony(CHAR_DATA *ch, int amount);
void reset_testimony(CHAR_DATA *ch);
void set_testimony_target(CHAR_DATA *ch, CHAR_DATA *victim);
bool is_sentinel_class(CHAR_DATA *ch);
int get_sentinel_level(CHAR_DATA *ch);

#endif /* DEC_SENTINEL_H */
