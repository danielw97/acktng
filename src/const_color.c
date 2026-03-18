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

#include "globals.h"

/*
 * color table for say, gossip, shout, etc.
 * Put here for ease of editing. -S-
 */

const struct color_type color_table[MAX_COLOR] = {
    {"say", 0},    {"tell", 1},     {"gossip", 2},   {"auction", 3}, {"music", 4}, {"shout", 5},
    {"yell", 6},   {"clan", 7},     {"race", 8},     {"flame", 9},   {"info", 10}, {"stats", 11},
    {"rooms", 12}, {"objects", 13}, {"mobiles", 14}, {"OOC", 15}

};

const struct ansi_type ansi_table[MAX_ANSI] = {
    {"gray", "\033[0;37m", 0, 'g', 7},
    {"red", "\033[0;31m", 1, 'R', 7},
    {"green", "\033[0;32m", 2, 'G', 7},
    {"brown", "\033[0;33m", 3, 'b', 7},
    {"blue", "\033[0;34m", 4, 'B', 7},
    {"magenta", "\033[0;35m", 5, 'm', 7},
    {"cyan", "\033[0;36m", 6, 'c', 7},
    {"black", "\033[0;30m", 7, 'k', 7}, /* was 0;33 */
    {"yellow", "\033[1;33m", 8, 'y', 7},
    {"white", "\033[1;37m", 9, 'W', 7},
    {"normal", "\033[0;0m\033[0;40m", 10, 'N', 13},
    {"purple", "\033[1;35m", 11, 'p', 7},
    {"dark_grey", "\033[1;30m", 12, 'd', 7},
    {"light_blue", "\033[1;34m", 13, 'l', 7},
    {"light_green", "\033[1;32m", 14, 'r', 7},
    {"light_cyan", "\033[1;36m", 15, 'a', 7},
    {"light_red", "\033[1;31m", 16, 'e', 7},
    {"bold", "\033[1m", 17, 'x', 4},
    {"flashing", "\033[5m", 18, 'f', 4},
    {"inverse", "\033[7m", 19, 'i', 4},
    {"back_red", "\033[0;41m", 20, '2', 7},
    {"back_green", "\033[0;42m", 21, '3', 7},
    {"back_yellow", "\033[0;43m", 22, '4', 7},
    {"back_blue", "\033[0;44m", 23, '1', 7},
    {"back_magenta", "\033[0;45m", 24, '5', 7},
    {"back_cyan", "\033[0;46m", 25, '6', 7},
    {"back_black", "\033[0;40m", 26, '0', 7},
    {"back_white", "\033[1;47m", 27, '7', 7},

};
