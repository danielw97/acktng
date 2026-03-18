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

const float hr_damTable[121] = {
    0.311, 0.325, 0.341, 0.358, 0.378, 0.399, 0.423, 0.449, 0.478, 0.509, 0.543, 0.579, 0.618,
    0.659, 0.703, 0.749, 0.797, 0.846, 0.897, 0.948, 1.000, 1.012, 1.024, 1.036, 1.048, 1.060,
    1.071, 1.083, 1.094, 1.105, 1.117, 1.127, 1.138, 1.149, 1.159, 1.169, 1.178, 1.188, 1.197,
    1.206, 1.215, 1.223, 1.231, 1.239, 1.247, 1.254, 1.261, 1.268, 1.274, 1.281, 1.287, 1.292,
    1.298, 1.303, 1.308, 1.313, 1.317, 1.322, 1.326, 1.330, 1.333, 1.337, 1.340, 1.344, 1.347,
    1.350, 1.352, 1.355, 1.357, 1.360, 1.362, 1.364, 1.366, 1.368, 1.370, 1.372, 1.373, 1.375,
    1.376, 1.377, 1.379, 1.380, 1.381, 1.382, 1.383, 1.384, 1.385, 1.386, 1.387, 1.387, 1.388,
    1.389, 1.389, 1.390, 1.391, 1.391, 1.392, 1.392, 1.393, 1.393, 1.393, 1.394, 1.394, 1.395,
    1.395, 1.395, 1.395, 1.396, 1.396, 1.396, 1.396, 1.397, 1.397, 1.397, 1.397, 1.397, 1.397,
    1.398, 1.398, 1.398, 1.398};
