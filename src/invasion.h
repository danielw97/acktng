/***************************************************************************
 * invasion.h -- Invasion System for Shades of Evil                        *
 *                                                                         *
 * An invasion event that spawns a boss and waves of mythological mobs     *
 * that march toward room 3001 to attack Gertrude.                         *
 ***************************************************************************/

#ifndef INVASION_H
#define INVASION_H

/* -----------------------------------------------------------------------
 * Tuning constants
 * --------------------------------------------------------------------- */
#define INVASION_SPAWN_VNUM     3001   /* Gertrude's room – also the target */
#define INVASION_START_VNUM     30000  /* Room where the invasion spawns in  */
                                       /* Change this to a suitable entry    */
                                       /* point in your world.               */

/* How often a new wave of mobs *may* spawn (uses PULSE_TICK already in    *
 * update_handler). The invasion_tick() function is called from there.     */
#define INVASION_MIN_INTERVAL   (PULSE_TICK * 5)   /* minimum ticks between invasions */

/* Mob level offsets relative to player pseudo-levels                       */
#define INVASION_BOSS_OFFSET    20
#define INVASION_MOB_MIN_OFFSET  5
#define INVASION_MOB_MAX_OFFSET 15

/* Quest-point reward on success */
#define INVASION_QP_REWARD      25

/* -----------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */
void invasion_update   (void);   /* Called once per PULSE_TICK              */
void invasion_rooms_update(void);/* Called once per PULSE_ROOMS to move mobs*/
void invasion_on_death (CHAR_DATA *ch, CHAR_DATA *killer); /* hook in fight.c*/

/* Immortal / admin command */
void do_invasion       (CHAR_DATA *ch, char *argument);

#endif /* INVASION_H */
