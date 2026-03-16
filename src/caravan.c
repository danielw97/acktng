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
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include "globals.h"

#define MAX_WORLD_CARAVANS 3
#define CARAVAN_MOB_VNUM 3358
#define CARAVAN_SAY_PULSE_MIN 2
#define CARAVAN_SAY_PULSE_MAX 5
#define CARAVAN_SAY_CHANCE 35

typedef struct caravan_route_data CARAVAN_ROUTE_DATA;
struct caravan_route_data
{
   const char *origin;
   const char *destination;
   int spawn_room_vnum;
   int destination_room_vnum;
   const char **journey_lines;
   int journey_line_count;
};

static const char *midgaard_to_kiess_lines[] = {
    "Keep your contracts dry; Kiess clerks can smell wet ink from a league away.",
    "Mind the axle-song, friends. This wagon has crossed the Wild Gate more than most captains.",
    "Kiess steel buys higher than Midgaard bronze this week, so smile like you're rich.",
    "By sundown we'll trade cedar casks for forest resin and march back smelling like coin.",
    "If you hear wolves, clap twice and let the outriders do the shouting.",
    "I've hauled relic-crates to Kiess in blizzards; this weather is a gift.",
    "Remember: in Kiess, haggle politely first and ferociously second.",
    "The Prism Square tallymen wager on our arrival hour. Let's make them lose.",
    "No one naps on this leg. The road loves sleeping fools and keeps their boots.",
    "When we roll into Kiess, breathe deep. Pine and profit hit the lungs together."};

static const char *midgaard_to_kowloon_lines[] = {
    "Lantern Gate tariffs changed again; keep your manifests neat and your temper neater.",
    "The long road to Kowloon pays in spice, silk, and stories sharp enough to cut.",
    "Count every crate at dawn and dusk. Somewhere between cities, numbers learn to lie.",
    "When harbor fog kisses the walls of Kowloon, you'll know we've outrun our doubts.",
    "We ride for jade scales, lacquered steel, and tea worth more than silver.",
    "If bandits ask who's in charge, tell them debt is. Debt always travels armed.",
    "Keep bells tied tight; the Tide Gate inspectors hear loose metal and charge extra.",
    "Midgaard bread out, Kowloon pepper back. That's the rhythm that keeps us fed.",
    "I've seen caravans vanish between milestones. We survive by counting each other loud.",
    "By the time we see Kowloon lanterns, these wheels will be singing in three languages."};

static const char *kiess_to_midgaard_lines[] = {
    "Midgaard buyers pay faster when Kiess wax seals are still warm.",
    "Keep the draft team steady; these roads punish pride harder than mud.",
    "We've got forest glass, iron nails, and enough ledgers to start a priesthood.",
    "Once we pass the wildlands, Midgaard's gate stewards will weigh us by the rumor.",
    "No shortcuts today. Every shortcut on this run ends with a broken wheel.",
    "I want smiling faces at Cedar Square; tired sells cheap.",
    "The first one to spot Midgaard stone gets the last honeycake from my lockbox.",
    "Kiess taught you discipline. Midgaard will test whether you kept any.",
    "Guard the resin barrels. One cracked lid and the whole caravan smells like regret.",
    "Midgaard nights are loud, but their coinpurses are louder."};

static const char *kiess_to_kowloon_lines[] = {
    "Kowloon buyers love Kiess steel because it doesn't flatter their mistakes.",
    "Tie the cargo high. The low roads flood quick and steal whole inventories.",
    "From pinewall to harborwall in one run; that's how legends earn bookkeeping entries.",
    "Kowloon's chandlers pay in silver and gossip. Either one can sink a crew.",
    "Check your knots, then check your partner's knots. Trust is a two-handed craft.",
    "We'll trade timber bracing for lantern oil and laugh all the way back.",
    "If the scouts signal red, we form ring and move like a fortress on wheels.",
    "Don't stare at city lights when we arrive; stare at your cargo count.",
    "Kiess discipline on the road, Kowloon swagger at market. Learn both.",
    "By next moon, our route marks will be the safest path between empires."};

static const char *kowloon_to_midgaard_lines[] = {
    "Midgaard collectors pay madness for Kowloon lacquer when the seal is unbroken.",
    "Keep the tea chests upright. One spill and we'll smell rich while starving.",
    "From Tide Gate dawn to Wild Gate dusk—may our wheels stay truer than promises.",
    "These silks bought us escorts in Kowloon; they'll buy us favors in Midgaard.",
    "Mind the road grit, it chews axles like a moneylender chews excuses.",
    "When we reach Midgaard, no one mentions how many tolls we argued down.",
    "Harbor fog behind us, stone avenues ahead. That's a good omen if I've ever seen one.",
    "I want every crate signed before Cedar Square bells. No heroics, just precision.",
    "Kowloon sends elegance, Midgaard sends iron, and we keep both honest.",
    "If the night gets quiet, sing. Silence on trade roads invites bad mathematics."};

static const char *kowloon_to_kiess_lines[] = {
    "Kiess quartermasters pay premium for Kowloon fittings that survive winter bite.",
    "Count the spice bundles twice; cold air makes fingers and thieves equally quick.",
    "From lantern markets to pine walls, this run writes itself in disciplined profit.",
    "We'll swap harbor copper for Kiess hardwood and call it diplomacy.",
    "Keep formation tight; mountain winds love to scatter the overconfident.",
    "When Kiess gates rise, walk proud but keep your invoices humble.",
    "Our outriders say the roads are clear. I say we trust them and verify anyway.",
    "Kowloon taught us grace under noise; Kiess rewards calm under pressure.",
    "No wasted motion, no wasted words, no wasted bolts. That's this journey.",
    "The first scent of pine means we've survived the hard miles and earned the easy lies."};

static const CARAVAN_ROUTE_DATA caravan_routes[] = {
    {"Midgaard", "Kiess", 968, 3350, midgaard_to_kiess_lines,
     sizeof(midgaard_to_kiess_lines) / sizeof(midgaard_to_kiess_lines[0])},
    {"Midgaard", "Kowloon", 968, 3522, midgaard_to_kowloon_lines,
     sizeof(midgaard_to_kowloon_lines) / sizeof(midgaard_to_kowloon_lines[0])},
    {"Kiess", "Midgaard", 3350, 968, kiess_to_midgaard_lines,
     sizeof(kiess_to_midgaard_lines) / sizeof(kiess_to_midgaard_lines[0])},
    {"Kiess", "Kowloon", 3350, 3522, kiess_to_kowloon_lines,
     sizeof(kiess_to_kowloon_lines) / sizeof(kiess_to_kowloon_lines[0])},
    {"Kowloon", "Midgaard", 3522, 968, kowloon_to_midgaard_lines,
     sizeof(kowloon_to_midgaard_lines) / sizeof(kowloon_to_midgaard_lines[0])},
    {"Kowloon", "Kiess", 3522, 3350, kowloon_to_kiess_lines,
     sizeof(kowloon_to_kiess_lines) / sizeof(kowloon_to_kiess_lines[0])}};

static bool is_active_caravan(CHAR_DATA *ch)
{
   return IS_NPC(ch) && ch->searching != NULL && !str_prefix("caravan:", ch->searching);
}

static int active_caravan_count(void)
{
   CHAR_DATA *ch;
   int count = 0;

   for (ch = first_char; ch != NULL; ch = ch->next)
      if (is_active_caravan(ch))
         count++;

   return count;
}

static const CARAVAN_ROUTE_DATA *caravan_route_for_index(int route_index)
{
   if (route_index < 0 || route_index >= (int)(sizeof(caravan_routes) / sizeof(caravan_routes[0])))
      return NULL;

   return &caravan_routes[route_index];
}

static int caravan_route_index_for_mob(CHAR_DATA *ch)
{
   int route_index = -1;

   if (ch->searching == NULL)
      return -1;

   if (sscanf(ch->searching, "caravan:%d", &route_index) != 1)
      return -1;

   return route_index;
}

static void spawn_caravan(void)
{
   int route_index;
   MOB_INDEX_DATA *pMobIndex;
   ROOM_INDEX_DATA *spawn_room;
   CHAR_DATA *caravan;
   const CARAVAN_ROUTE_DATA *route;
   char buf[MAX_STRING_LENGTH];

   route_index = number_range(0, (int)(sizeof(caravan_routes) / sizeof(caravan_routes[0])) - 1);
   route = caravan_route_for_index(route_index);
   if (route == NULL)
      return;

   pMobIndex = get_mob_index(CARAVAN_MOB_VNUM);
   if (pMobIndex == NULL)
   {
      bug("spawn_caravan: caravan mob vnum not found.", CARAVAN_MOB_VNUM);
      return;
   }

   spawn_room = get_room_index(route->spawn_room_vnum);
   if (spawn_room == NULL)
   {
      bug("spawn_caravan: spawn room not found.", route->spawn_room_vnum);
      return;
   }

   caravan = create_mobile(pMobIndex);
   if (caravan == NULL)
      return;

   free_string(caravan->name);
   free_string(caravan->short_descr);
   free_string(caravan->long_descr);
   if (caravan->long_descr_orig != NULL)
      free_string(caravan->long_descr_orig);

   sprintf(buf, "caravan drover teamster trader convoy %s %s", route->origin, route->destination);
   caravan->name = str_dup(buf);

   sprintf(buf, "a %s caravan master bound for %s", route->origin, route->destination);
   caravan->short_descr = str_dup(buf);

   sprintf(buf, "A %s caravan master bound for %s checks manifests beside loaded wagons.\n\r",
           route->origin, route->destination);
   caravan->long_descr = str_dup(buf);
   caravan->long_descr_orig = str_dup(buf);

   SET_BIT(caravan->act, ACT_SENTINEL);
   REMOVE_BIT(caravan->act, ACT_STAY_AREA);
   SET_BIT(caravan->act, ACT_WIMPY);

   if (caravan->searching != NULL)
      free_string(caravan->searching);
   sprintf(buf, "caravan:%d", route_index);
   caravan->searching = str_dup(buf);

   char_to_room(caravan, spawn_room);
   act("$n barks out route orders as a trade caravan forms up.", caravan, NULL, NULL, TO_ROOM);
}

void caravan_update(void)
{
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;
   int count;
   static int say_pulse = 1;
   bool can_say = FALSE;

   count = active_caravan_count();
   if (count < MAX_WORLD_CARAVANS && number_percent() <= 35)
      spawn_caravan();

   if (--say_pulse <= 0)
   {
      can_say = TRUE;
      say_pulse = number_range(CARAVAN_SAY_PULSE_MIN, CARAVAN_SAY_PULSE_MAX);
   }

   for (ch = first_char; ch != NULL; ch = ch_next)
   {
      const CARAVAN_ROUTE_DATA *route;
      int route_index;
      ROOM_INDEX_DATA *destination_room;
      int dir;

      ch_next = ch->next;

      if (!is_active_caravan(ch) || ch->in_room == NULL)
         continue;

      route_index = caravan_route_index_for_mob(ch);
      route = caravan_route_for_index(route_index);
      if (route == NULL)
         continue;

      destination_room = get_room_index(route->destination_room_vnum);
      if (destination_room == NULL)
      {
         bug("caravan_update: destination room not found.", route->destination_room_vnum);
         continue;
      }

      if (ch->in_room == destination_room)
      {
         act("$n raises a hand in triumph as the caravan reaches its destination and breaks "
             "formation.",
             ch, NULL, NULL, TO_ROOM);
         extract_char(ch, TRUE);
         continue;
      }

      dir = h_find_dir(ch->in_room, destination_room, HUNT_WORLD | HUNT_OPENDOOR);
      if (dir < 0)
      {
         act("$n studies a map, curses softly, and orders the caravan to disperse.", ch, NULL, NULL,
             TO_ROOM);
         extract_char(ch, TRUE);
         continue;
      }

      move_char(ch, dir);

      if (ch->in_room == destination_room)
      {
         act("$n raises a hand in triumph as the caravan reaches its destination and breaks "
             "formation.",
             ch, NULL, NULL, TO_ROOM);
         extract_char(ch, TRUE);
         continue;
      }

      if (can_say && number_percent() <= CARAVAN_SAY_CHANCE)
      {
         const char *line = route->journey_lines[number_range(0, route->journey_line_count - 1)];
         char line_buf[MAX_STRING_LENGTH];

         sprintf(line_buf, "$n says '%s'", line);
         act(line_buf, ch, NULL, NULL, TO_ROOM);
      }
   }
}
