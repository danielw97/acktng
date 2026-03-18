/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#ifndef DEC_SKILLS_H
#define DEC_SKILLS_H 1

#ifndef DEC_ACK_H
#include "ack.h"
#endif

/* skills.c */
bool can_hit_skill(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);
bool subtract_energy_cost(CHAR_DATA *ch, int gsn);
int get_energy_cost(CHAR_DATA *ch, int gsn);
void trip(CHAR_DATA *ch, CHAR_DATA *victim);

/* skills_poison.c */
void backstab(CHAR_DATA *ch, CHAR_DATA *victim, bool do_backstab);
bool do_poison(CHAR_DATA *ch, char *argument, int gsn);

/* skills_chi.c */
int get_chi(CHAR_DATA *ch);
int get_max_chi(CHAR_DATA *ch);
int chi_skill_cost(int base_cost, int cooldown);
bool chi_should_block_mindoverbody(bool has_mindoverbody, bool has_named_mindoverbody);
void pug_attack(CHAR_DATA *ch, char *argument, int gsn);

/* skills_combo.c */
bool combo(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);
bool is_valid_finisher(CHAR_DATA *ch);
bool is_ready_finisher(CHAR_DATA *ch);
void reset_combo(CHAR_DATA *ch);
int get_max_combo(CHAR_DATA *ch);
int get_combo_count(CHAR_DATA *ch);
void war_attack(CHAR_DATA *ch, char *argument, int gsn);

/* skills_combo.c */
void stun(CHAR_DATA *ch, CHAR_DATA *victim);

/* skills_obj.c */
void disarm(CHAR_DATA *ch, CHAR_DATA *victim);
int disarm_select_weapon_slot(int left_type, int right_type, int two_handed_type);
int find_door(CHAR_DATA *ch, char *arg);

#endif /* DEC_SKILLS_H */
