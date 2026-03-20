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

const struct clan_type clan_table[MAX_CLAN] = {
    /* 0: No clan */
    {"None", "None ", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 1: The Lantern Accord - Midgaard civic continuity */
    {"The Lantern Accord", "@@yACCRD@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 2: The Open Ledger - Archive reform and truth-seekers */
    {"The Open Ledger", "@@WLEDGR@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 3: The Jade Compact - Kowloon delta commerce */
    {"The Jade Compact", "@@aJADEC@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 4: The Ember Speakers - Ash Cult volcanic theology */
    {"The Ember Speakers", "@@REMBER@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 5: The Red Sand Accounts - Mafdet maritime debt enforcement */
    {"The Red Sand Accounts", "@@eRDSND@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 6: The Charter Wardens - Kiess anti-secrecy legal reform */
    {"The Charter Wardens", "@@lCHRTR@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 7: The Jackal Synod - Desert death-rite toll enforcement */
    {"The Jackal Synod", "@@dJCKAL@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 8: The Root Covenant - Thornwood substrate theology */
    {"The Root Covenant", "@@gROOTC@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 9: The Waystone Circle - Desert road preservation */
    {"The Waystone Circle", "@@GWYSTN@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},

    /* 10: The Oathbound March - Multi-race recognition alliance */
    {"The Oathbound March", "@@pOATHB@@N", 0, 0, "N/A", " ", {-1, -1, -1, -1, -1, -1}},
};
