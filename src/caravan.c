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

#define MAX_WORLD_CARAVANS 5
#define CARAVAN_MOB_VNUM 3358
#define CARAVAN_SAY_PULSE_MIN 2
#define CARAVAN_SAY_PULSE_MAX 5
#define CARAVAN_SAY_CHANCE 35

/* Caravan room vnums — the designated caravan gathering point in each city */
#define CARAVAN_ROOM_MIDGAARD 1151 /* Caravan Square, Midgaard             */
#define CARAVAN_ROOM_KIESS    3661 /* Caravan Rest Yard, Kiess             */
#define CARAVAN_ROOM_KOWLOON  3833 /* Caravan Staging Yard, Kowloon        */
#define CARAVAN_ROOM_MAFDET   3878 /* Gate Customs Yard, Mafdet            */
#define CARAVAN_ROOM_RAKUEN   4556 /* Lower Dispatch House, Rakuen         */

typedef struct caravan_route_data CARAVAN_ROUTE_DATA;
struct caravan_route_data
{
   const char *origin;
   const char *destination;
   const char *cargo_label;        /* thematic cargo name, e.g. "mercy-grain" */
   int spawn_room_vnum;
   int destination_room_vnum;
   const char **journey_lines;
   int journey_line_count;
};

/* -----------------------------------------------------------------------
 * Midgaard departures
 * ----------------------------------------------------------------------- */

static const char *midgaard_to_kiess_lines[] = {
    "Keep your contracts dry; Kiess clerks can smell wet ink from a league away.",
    "Mind the axle-song, friends. This wagon has crossed the Wild Gate more than most captains.",
    "Kiess steel buys higher than Midgaard bronze this week, so smile like you're rich.",
    "By sundown we'll trade iron-provisions for forest resin and march back smelling like coin.",
    "If you hear wolves, clap twice and let the outriders do the shouting.",
    "I've hauled provision-crates to Kiess in blizzards; this weather is a gift.",
    "Remember: in Kiess, haggle politely first and ferociously second.",
    "The Caravan Rest Yard tallymen wager on our arrival hour. Let's make them lose.",
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
    "Midgaard grain out, Kowloon pepper back. That's the rhythm that keeps us fed.",
    "I've seen caravans vanish between milestones. We survive by counting each other loud.",
    "By the time we see Kowloon lanterns, these wheels will be singing in three languages."};

static const char *midgaard_to_mafdet_lines[] = {
    "The desert road is long, but Mafdet pays in gold what it cannot grow in sand.",
    "Pack the grain barrels tight. Desert heat will find any weakness in the seals.",
    "Mafdet's customs inspectors check everything twice. Have your tally boards ready.",
    "The Carters' Quarter in Mafdet smells of salt and dust. You'll miss it when you're gone.",
    "Keep your water skins full. The road to the Gate Customs Yard has no shade worth trusting.",
    "These winter-stores will feed a thousand people who'd otherwise go without.",
    "When the gate towers of Mafdet rise in the heat haze, slow the draft team. They've earned rest.",
    "Desert trade is honest trade. Mafdet folk know exactly what a full grain barrel is worth."};

static const char *midgaard_to_rakuen_lines[] = {
    "The eastern run tests more than your legs. Pack patience with the grain.",
    "Rakuen sits where the canopy meets the sky. Getting there is half the trade.",
    "These mercy-grain loads have kept Rakuen's lower dispatch fed through two hard seasons.",
    "Stay on the marked path. Rakuen's outriders are friendly to those who respect the boundaries.",
    "The Registration Floor in Rakuen keeps its own time. Don't argue, just sign the manifest.",
    "Grain for the Compact means goodwill at the eastern border for another season.",
    "I've made this run in rain and shine. The canopy never looks the same twice.",
    "By the time Rakuen's dispatch house comes into view, the draft animals know the smell of rest."};

/* -----------------------------------------------------------------------
 * Kiess departures
 * ----------------------------------------------------------------------- */

static const char *kiess_to_midgaard_lines[] = {
    "Midgaard buyers pay faster when Kiess wax seals are still warm.",
    "Keep the draft team steady; these roads punish pride harder than mud.",
    "We've got cold-cedar, iron nails, and enough ledgers to start a priesthood.",
    "Once we pass the wildlands, Midgaard's gate stewards will weigh us by the rumor.",
    "No shortcuts today. Every shortcut on this run ends with a broken wheel.",
    "I want smiling faces at Caravan Square; tired sells cheap.",
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

static const char *kiess_to_mafdet_lines[] = {
    "Desert craftsmen pay extraordinary sums for Kiess pine resin. It seals what sand erodes.",
    "The run south means swapping cold pine air for hot desert stone. Dress accordingly.",
    "Keep the resin barrels shaded at all stops. Heat is their only natural enemy.",
    "Mafdet's Gate Customs Yard has inspectors who know their resins by smell alone. Impress them.",
    "Pine resin from Kiess is worth more than silver in a desert city. Guard it accordingly.",
    "We ride out of forest and into rock and sand. Watch your footing and your cargoes.",
    "The southern trade isn't glamorous, but Mafdet's merchants pay clean and pay early.",
    "Breathe deep when we leave. The pine scent doesn't follow past the forest edge."};

static const char *kiess_to_rakuen_lines[] = {
    "Rakuen builders know Kiess wood doesn't split. That reputation built this trade.",
    "The eastern roads are long and the milestones trust your patience.",
    "These pinewood beams will frame a building in Rakuen that stands for two hundred years.",
    "Canopy city or no, Rakuen needs what grows at the forest floor. We bring it.",
    "Keep the joints dry and the manifest current. Rakuen registrars count every board-foot.",
    "When the canopy overhead thickens, we're close. The Registration Floor is at the base of the dispatch spire.",
    "Kiess craft and Rakuen patience make the finest structures in the east. This load proves it.",
    "The smell of pine at the end of an eastern run is a promise that the next one will happen too."};

/* -----------------------------------------------------------------------
 * Kowloon departures
 * ----------------------------------------------------------------------- */

static const char *kowloon_to_midgaard_lines[] = {
    "Midgaard collectors pay madness for Kowloon lacquer when the seal is unbroken.",
    "Keep the tea chests upright. One spill and we'll smell rich while starving.",
    "From Tide Gate dawn to Wild Gate dusk — may our wheels stay truer than promises.",
    "These silks bought us escorts in Kowloon; they'll buy us favors in Midgaard.",
    "Mind the road grit; it chews axles like a moneylender chews excuses.",
    "When we reach Midgaard, no one mentions how many tolls we argued down.",
    "Harbor fog behind us, stone avenues ahead. That's a good omen if I've ever seen one.",
    "I want every crate signed before Caravan Square bells. No heroics, just precision.",
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

static const char *kowloon_to_mafdet_lines[] = {
    "Mafdet's spice merchants argue hard but pay fair. Know your quantities before they do.",
    "Kowloon pepper is worth twice what Mafdet grows in their sand-gardens. Let them know it.",
    "Keep the spice bundles sealed. Desert air pulls the oils right out of open sacks.",
    "The customs inspectors in Mafdet weigh spice separately from everything else. Plan for that.",
    "Silk and spice from Kowloon to the desert — this is what makes the southern route worthwhile.",
    "Don't let the heat fool you. Mafdet's markets are busiest in the early morning cool.",
    "Our outriders say the salt road is clear. Kowloon pepper will reach Mafdet Gate by nightfall.",
    "Spice pays for the trip both ways. What we earn here funds the next Kiess run."};

static const char *kowloon_to_rakuen_lines[] = {
    "Rakuen's weavers pay well for Kowloon silk because it takes canopy dye better than their own cloth.",
    "The eastern road after the delta is long and dry. Our silk cargo is worth the discomfort.",
    "Keep the silk bales off the wagon floor. Mud stains reduce the price before you open your mouth.",
    "Rakuen's dispatch house runs a strict manifest system. Every bolt of silk gets its own line.",
    "Southern silk is soft enough to trade on reputation alone. We don't need to speak; the cloth does.",
    "The last hour before Rakuen is uphill. The wagons feel it and so will you.",
    "Eastern buyers know value. Kowloon silk at the Registration Floor sells before we unhitch.",
    "What starts as sea-trade in Kowloon becomes land-legend by the time it reaches the canopy."};

/* -----------------------------------------------------------------------
 * Mafdet departures
 * ----------------------------------------------------------------------- */

static const char *mafdet_to_midgaard_lines[] = {
    "Midgaard glassblowers will fight over this sand-salt. We just need to get there first.",
    "Desert glass is worth ten times more once it crosses the wild gate. Keep that in mind.",
    "The salt loads sweat in humid air. Reseal any barrel that sounds loose by nightfall.",
    "Midgaard's Caravan Square will be crowded at arrival. Don't lose a crate in the shuffle.",
    "We leave dry stone and reach green road, then finally stone avenue. All worth it.",
    "Salt is the oldest trade good in the world. We're just keeping the tradition honest.",
    "When Midgaard stone walls rise, the hard part is over. Negotiating with buyers is entertainment.",
    "Desert salt in Midgaard kitchens — I find that poetic. It keeps the trade going another season."};

static const char *mafdet_to_kiess_lines[] = {
    "Kiess smiths don't have desert alloys. That's our advantage and their premium.",
    "Desert-bronze doesn't rust in forest damp the way regular bronze does. The Kiess smiths know it.",
    "Keep the ingot crates level. Uneven loads twist axles on these northern roads.",
    "The temperature drop between Mafdet and Kiess is dramatic. Dress in layers, load accordingly.",
    "Bronze from the desert foundries is denser. Kiess craftsmen pay for the difference.",
    "We leave heat and salt air behind and gain pine cold and forge smoke. Both have their merits.",
    "No shortcuts to Kiess. Every shortcut on this run ends with a broken wheel or a wrong turn.",
    "Desert metal in a forest city — this is what keeps the roads honest and the smiths supplied."};

static const char *mafdet_to_kowloon_lines[] = {
    "Kowloon's harbor merchants pay harbor prices for desert spice. We benefit from both.",
    "The coastal road after the salt flats is the best road on this run. Enjoy it.",
    "Sand-spice from the eastern desert dunes sells differently in Kowloon than anything from the south.",
    "Keep the cargo manifests current. Lantern Gate tariffs are based on declared weight, not guessed weight.",
    "Kowloon fog is thick at dawn. We aim for the Caravan Staging Yard and trust the drovers' instinct.",
    "Mafdet sends the desert east and Kowloon sends the sea north. Between them we make a living.",
    "The Tide Gate inspectors know their spice. Don't try to pass cumin as pepper.",
    "By the time Kowloon lanterns come into view, this cargo has already earned its manifest fees."};

static const char *mafdet_to_rakuen_lines[] = {
    "Rakuen does not produce enough salt for its lower city. We're the solution to that problem.",
    "Desert salt in an eastern canopy city — the trade route is strange; the profit is not.",
    "Keep the salt barrels tight. Canopy humidity is as bad as sea air for loose seals.",
    "Rakuen's dispatch house was built assuming trade from the east, not the south. That's why we get the better rate.",
    "The long road from Mafdet to Rakuen crosses three different kinds of weather. Pack for all of them.",
    "Salt preserves food. Dune-salt from Mafdet preserves a relationship with the Compact.",
    "When the canopy first appears overhead, we're in the final approach. Slow and steady to the dispatch floor.",
    "Desert salt in a forest dispatch house — somewhere a geographer is making notes."};

/* -----------------------------------------------------------------------
 * Rakuen departures
 * ----------------------------------------------------------------------- */

static const char *rakuen_to_midgaard_lines[] = {
    "Midgaard's apothecaries have been waiting on this bark-oil for two seasons. We'll be welcomed.",
    "Bark-oil doesn't spoil, but it shifts. Keep the casks upright or we'll smell like a forest fire.",
    "The road from the canopy to the central city crosses every landscape the world bothers making.",
    "Midgaard buyers don't know what bark-oil is, but they know what it does. That's enough.",
    "We leave the Registration Floor and won't see it again for a month. That's the rhythm of this trade.",
    "Rakuen bark-oil in Midgaard means healers can work another season. That's worth the road.",
    "When the wild gate comes into view, we're close. Caravan Square by nightfall if the roads are kind.",
    "The Compact sends oil and asks for grain in return. Everyone profits from a fair exchange."};

static const char *rakuen_to_kiess_lines[] = {
    "Kiess has good forest wood. Rakuen has better. The Kiess smiths know the difference.",
    "Canopy timber is dense, straight, and seasons faster than anything grown at ground level.",
    "We leave the dispatch floor and head into roads that get colder with every league. Watch the cargo.",
    "Kiess craftsmen use Rakuen timber for their finest work. We carry the finest work's raw material.",
    "The run west is long and the canopy smell fades within an hour. After that, pine takes over.",
    "Keep the timber loads roped on both sides. Road vibration unships long cargo faster than bandits.",
    "When the Caravan Rest Yard at Kiess comes into view, the smell of sawdust means we're welcome.",
    "Wood from the canopy, built into things in the pine city — trade at its most practical."};

static const char *rakuen_to_kowloon_lines[] = {
    "Kowloon's herbalists buy Rakuen medicinal herbs at premium and sell them at miracle prices.",
    "Eastern herbs don't travel well in heat. We ride at night when the route allows.",
    "Keep the herb bundles separated by type. Mixed herbs cut the value before you reach the market.",
    "The southern run from Rakuen to Kowloon is the most diverse road in the trade network.",
    "Kowloon fog is kind to herb cargo. The humidity keeps them fresher than dry road air.",
    "When the Caravan Staging Yard comes into view, the Kowloon herbalists are already waiting.",
    "Rakuen herbs in a harbor city — every ship that leaves Kowloon carries some of what we brought.",
    "Eastern medicine in the harbor market means our next run is already half-sold."};

static const char *rakuen_to_mafdet_lines[] = {
    "Mafdet's traders know canopy-weave because it doesn't fade in desert sun. That's the selling point.",
    "The run south from Rakuen means trading canopy cool for desert heat. Dress for both.",
    "Keep the cloth bales wrapped in sailcloth for the desert crossing. Sand in woven-canopy ruins the pattern.",
    "Mafdet's merchants are precise. They know the thread count before we open the bale. Impress them anyway.",
    "Canopy weave is lighter than desert linen and stronger than harbor cotton. Mafdet knows a good deal.",
    "The Gate Customs Yard in Mafdet is organized chaos. Stand your ground at the manifest desk.",
    "When the gate towers of Mafdet rise in the heat shimmer, the canopy cloth is already sold in buyers' minds.",
    "Eastern weave to a desert city — some roads make sense only when you look at the accounts."};

/* -----------------------------------------------------------------------
 * Route table — all 20 city-pair routes
 * ----------------------------------------------------------------------- */

static const CARAVAN_ROUTE_DATA caravan_routes[] = {
    /* Midgaard departures */
    {"Midgaard", "Kiess", "iron-provisions",
     CARAVAN_ROOM_MIDGAARD, CARAVAN_ROOM_KIESS,
     midgaard_to_kiess_lines,
     sizeof(midgaard_to_kiess_lines) / sizeof(midgaard_to_kiess_lines[0])},
    {"Midgaard", "Kowloon", "grain-and-wool",
     CARAVAN_ROOM_MIDGAARD, CARAVAN_ROOM_KOWLOON,
     midgaard_to_kowloon_lines,
     sizeof(midgaard_to_kowloon_lines) / sizeof(midgaard_to_kowloon_lines[0])},
    {"Midgaard", "Mafdet", "winter-stores",
     CARAVAN_ROOM_MIDGAARD, CARAVAN_ROOM_MAFDET,
     midgaard_to_mafdet_lines,
     sizeof(midgaard_to_mafdet_lines) / sizeof(midgaard_to_mafdet_lines[0])},
    {"Midgaard", "Rakuen", "mercy-grain",
     CARAVAN_ROOM_MIDGAARD, CARAVAN_ROOM_RAKUEN,
     midgaard_to_rakuen_lines,
     sizeof(midgaard_to_rakuen_lines) / sizeof(midgaard_to_rakuen_lines[0])},
    /* Kiess departures */
    {"Kiess", "Midgaard", "cold-cedar",
     CARAVAN_ROOM_KIESS, CARAVAN_ROOM_MIDGAARD,
     kiess_to_midgaard_lines,
     sizeof(kiess_to_midgaard_lines) / sizeof(kiess_to_midgaard_lines[0])},
    {"Kiess", "Kowloon", "timber-bracing",
     CARAVAN_ROOM_KIESS, CARAVAN_ROOM_KOWLOON,
     kiess_to_kowloon_lines,
     sizeof(kiess_to_kowloon_lines) / sizeof(kiess_to_kowloon_lines[0])},
    {"Kiess", "Mafdet", "forest-resin",
     CARAVAN_ROOM_KIESS, CARAVAN_ROOM_MAFDET,
     kiess_to_mafdet_lines,
     sizeof(kiess_to_mafdet_lines) / sizeof(kiess_to_mafdet_lines[0])},
    {"Kiess", "Rakuen", "pinewood-craft",
     CARAVAN_ROOM_KIESS, CARAVAN_ROOM_RAKUEN,
     kiess_to_rakuen_lines,
     sizeof(kiess_to_rakuen_lines) / sizeof(kiess_to_rakuen_lines[0])},
    /* Kowloon departures */
    {"Kowloon", "Midgaard", "lacquer-and-silk",
     CARAVAN_ROOM_KOWLOON, CARAVAN_ROOM_MIDGAARD,
     kowloon_to_midgaard_lines,
     sizeof(kowloon_to_midgaard_lines) / sizeof(kowloon_to_midgaard_lines[0])},
    {"Kowloon", "Kiess", "harbor-fittings",
     CARAVAN_ROOM_KOWLOON, CARAVAN_ROOM_KIESS,
     kowloon_to_kiess_lines,
     sizeof(kowloon_to_kiess_lines) / sizeof(kowloon_to_kiess_lines[0])},
    {"Kowloon", "Mafdet", "spice-and-pepper",
     CARAVAN_ROOM_KOWLOON, CARAVAN_ROOM_MAFDET,
     kowloon_to_mafdet_lines,
     sizeof(kowloon_to_mafdet_lines) / sizeof(kowloon_to_mafdet_lines[0])},
    {"Kowloon", "Rakuen", "southern-silk",
     CARAVAN_ROOM_KOWLOON, CARAVAN_ROOM_RAKUEN,
     kowloon_to_rakuen_lines,
     sizeof(kowloon_to_rakuen_lines) / sizeof(kowloon_to_rakuen_lines[0])},
    /* Mafdet departures */
    {"Mafdet", "Midgaard", "salt-and-glass",
     CARAVAN_ROOM_MAFDET, CARAVAN_ROOM_MIDGAARD,
     mafdet_to_midgaard_lines,
     sizeof(mafdet_to_midgaard_lines) / sizeof(mafdet_to_midgaard_lines[0])},
    {"Mafdet", "Kiess", "desert-bronze",
     CARAVAN_ROOM_MAFDET, CARAVAN_ROOM_KIESS,
     mafdet_to_kiess_lines,
     sizeof(mafdet_to_kiess_lines) / sizeof(mafdet_to_kiess_lines[0])},
    {"Mafdet", "Kowloon", "sand-spice",
     CARAVAN_ROOM_MAFDET, CARAVAN_ROOM_KOWLOON,
     mafdet_to_kowloon_lines,
     sizeof(mafdet_to_kowloon_lines) / sizeof(mafdet_to_kowloon_lines[0])},
    {"Mafdet", "Rakuen", "dune-salt",
     CARAVAN_ROOM_MAFDET, CARAVAN_ROOM_RAKUEN,
     mafdet_to_rakuen_lines,
     sizeof(mafdet_to_rakuen_lines) / sizeof(mafdet_to_rakuen_lines[0])},
    /* Rakuen departures */
    {"Rakuen", "Midgaard", "bark-oil",
     CARAVAN_ROOM_RAKUEN, CARAVAN_ROOM_MIDGAARD,
     rakuen_to_midgaard_lines,
     sizeof(rakuen_to_midgaard_lines) / sizeof(rakuen_to_midgaard_lines[0])},
    {"Rakuen", "Kiess", "canopy-timber",
     CARAVAN_ROOM_RAKUEN, CARAVAN_ROOM_KIESS,
     rakuen_to_kiess_lines,
     sizeof(rakuen_to_kiess_lines) / sizeof(rakuen_to_kiess_lines[0])},
    {"Rakuen", "Kowloon", "eastern-herbs",
     CARAVAN_ROOM_RAKUEN, CARAVAN_ROOM_KOWLOON,
     rakuen_to_kowloon_lines,
     sizeof(rakuen_to_kowloon_lines) / sizeof(rakuen_to_kowloon_lines[0])},
    {"Rakuen", "Mafdet", "woven-canopy",
     CARAVAN_ROOM_RAKUEN, CARAVAN_ROOM_MAFDET,
     rakuen_to_mafdet_lines,
     sizeof(rakuen_to_mafdet_lines) / sizeof(rakuen_to_mafdet_lines[0])}};

#define NUM_CARAVAN_ROUTES (int)(sizeof(caravan_routes) / sizeof(caravan_routes[0]))

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
   if (route_index < 0 || route_index >= NUM_CARAVAN_ROUTES)
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

   route_index = number_range(0, NUM_CARAVAN_ROUTES - 1);
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

   sprintf(buf, "caravan drover teamster trader convoy %s %s %s",
           route->origin, route->destination, route->cargo_label);
   caravan->name = str_dup(buf);

   sprintf(buf, "the %s caravan to %s", route->cargo_label, route->destination);
   caravan->short_descr = str_dup(buf);

   sprintf(buf, "The %s caravan to %s stands ready, wagons loaded and drovers in position.\n\r",
           route->cargo_label, route->destination);
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
