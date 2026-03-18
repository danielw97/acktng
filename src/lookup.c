#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "quest.h"

static char buf[MAX_STRING_LENGTH];

/*
 * Find a char in the room.
 */
CHAR_DATA *get_char_room(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *rch;
   int number;
   int count;

   number = number_argument(argument, arg);
   count = 0;
   if (arg[0] == '\0')
      return NULL;

   if (!str_cmp(arg, "self"))
      return ch;
   if (!str_cmp(arg, "tank"))
   {
      if (!is_fighting(ch))
      {
         send_to_char("You aren't fighting anyone!\n\r", ch);
         return NULL;
      }
      else if (ch->fighting->fighting == NULL)
      {
         send_to_char("Hmm, that's weird..where did he go?\n\r", ch);
         return NULL;
      }
      else
      {
         return ch->fighting->fighting;
      }
   }

   if (!str_cmp(arg, "enemy"))
   {
      if (!is_fighting(ch))
      {
         send_to_char("You aren't fighting anyone!\n\r", ch);
         return NULL;
      }
      else if (ch->fighting->fighting == NULL)
      {
         send_to_char("Hmm, that's weird..where did he go?\n\r", ch);
         return NULL;
      }
      if (ch->fighting->fighting->fighting == NULL)
      {
         send_to_char("Hmm, that's weird..where did he go?\n\r", ch);
         return NULL;
      }

      else
      {
         return ch->fighting->fighting->fighting;
      }
   }

   for (rch = ch->in_room->first_person; rch != NULL; rch = rch->next_in_room)
   {
      if (!can_see(ch, rch) || !is_name_relaxed(arg, rch->name))
         continue;
      if (++count == number)
         return rch;
   }

   return NULL;
}

/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *wch;
   int number;
   int count;

   if ((wch = get_char_room(ch, argument)) != NULL)
      return wch;

   number = number_argument(argument, arg);
   count = 0;
   for (wch = first_char; wch != NULL; wch = wch->next)
   {
      if (!can_see(ch, wch) || !is_name_relaxed(arg, wch->name))
         continue;
      if (++count == number)
         return wch;
   }

   return NULL;
}

/*
 * Find a char in the area.
 */
CHAR_DATA *get_char_area(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *wch;
   int number;
   int count;

   if ((wch = get_char_room(ch, argument)) != NULL)
      return wch;

   number = number_argument(argument, arg);
   count = 0;
   for (wch = first_char; wch != NULL; wch = wch->next)
   {
      if (wch->in_room == NULL || wch->in_room->area != ch->in_room->area)
         continue;
      if (!can_see(ch, wch) || !is_name_relaxed(arg, wch->name))
         continue;
      if (++count == number)
         return wch;
   }

   return NULL;
}

/*
 * Find a char's original body (for switched chars).
 */
CHAR_DATA *get_char(CHAR_DATA *ch)
{
   if (!ch->pcdata)
      return ch->desc->original;
   else
      return ch;
}

/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type(OBJ_INDEX_DATA *pObjIndex)
{
   OBJ_DATA *obj;

   for (obj = first_obj; obj != NULL; obj = obj->next)
   {
      if (obj->pIndexData == pObjIndex)
         return obj;
   }

   return NULL;
}

/*
 * Find an obj in a room.
 */
OBJ_DATA *get_obj_room(CHAR_DATA *ch, char *argument, OBJ_DATA *list)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   number = number_argument(argument, arg);
   count = 0;
   for (obj = list; obj != NULL; obj = obj->next_in_room)
   {
      if (can_see_obj(ch, obj) && is_name_relaxed(arg, obj->name))
      {
         if (++count == number)
            return obj;
      }
   }

   return NULL;
}

/*
 * Find an obj in a room.
 */
OBJ_DATA *get_obj_list(CHAR_DATA *ch, char *argument, OBJ_DATA *list)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   number = number_argument(argument, arg);
   count = 0;
   for (obj = list; obj != NULL; obj = obj->next_in_carry_list)
   {
      if (can_see_obj(ch, obj) && is_name(arg, obj->name))
      {
         if (++count == number)
            return obj;
      }
   }

   return NULL;
}

/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   number = number_argument(argument, arg);
   count = 0;
   for (obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list)
   {
      if (obj->wear_loc == WEAR_NONE && can_see_obj(ch, obj) && is_name(arg, obj->name))
      {
         if (++count == number)
            return obj;
      }
   }

   return NULL;
}

/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   number = number_argument(argument, arg);
   count = 0;
   for (obj = ch->first_carry; obj != NULL; obj = obj->next_in_carry_list)
   {
      if (obj->wear_loc != WEAR_NONE && can_see_obj(ch, obj) && is_name(arg, obj->name))
      {
         if (++count == number)
            return obj;
      }
   }

   return NULL;
}

/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here(CHAR_DATA *ch, char *argument)
{
   OBJ_DATA *obj;

   obj = get_obj_room(ch, argument, ch->in_room->first_content);
   if (obj != NULL)
      return obj;

   if ((obj = get_obj_carry(ch, argument)) != NULL)
      return obj;

   if ((obj = get_obj_wear(ch, argument)) != NULL)
      return obj;

   return NULL;
}

/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int number;
   int count;

   if ((obj = get_obj_here(ch, argument)) != NULL)
      return obj;

   number = number_argument(argument, arg);
   count = 0;
   for (obj = first_obj; obj != NULL; obj = obj->next)
   {
      if (can_see_obj(ch, obj) && is_name(arg, obj->name))
      {
         if (++count == number)
            return obj;
      }
   }

   return NULL;
}

/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money(int amount)
{
   OBJ_DATA *obj;

   if (amount <= 0)
   {
      sprintf(buf, "create_money: %d provided as amount.", amount);
      monitor_chan(buf, MONITOR_OBJ);

      bug("Create_money: zero or negative money %d.", amount);
      amount = 1;
   }

   if (amount == 1)
   {
      obj = create_object(get_obj_index(OBJ_VNUM_MONEY_ONE), 0);
   }
   else
   {
      obj = create_object(get_obj_index(OBJ_VNUM_MONEY_SOME), 0);
      sprintf(buf, obj->short_descr, amount);
      free_string(obj->short_descr);
      obj->short_descr = str_dup(buf);
      obj->value[0] = amount;
   }

   return obj;
}

/*
 * Return # of objects which an object counts as.
 * Thanks to Tony Chamberlain for the correct recursive code here.
 */
int get_obj_number(OBJ_DATA *obj)
{
   int number;
   /*
    * OBJ_DATA *vobj;
    */

   number = 1; /*set to one since bag will count as 1 item */
               /*    if ( obj->item_type == ITEM_CONTAINER )
                   {
                      for ( vobj = obj->first_in_carry_list; vobj != NULL; vobj = vobj->next_in_carry_list )
                      {
                         number = number - 1;
                      }
                   }
            
               */
               /* containers should count as one item!
                   if ( obj->item_type == ITEM_CONTAINER )
                     for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
                  number += get_obj_number( obj );
                   else
                  number = 1;
               Zen */
   return number;
}

/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight(OBJ_DATA *obj)
{
   int weight;

   if (obj->item_type == ITEM_MONEY)
   {
      weight = obj->value[0] / 100000;
      return weight;
   }
   weight = obj->weight;
   for (obj = obj->first_in_carry_list; obj != NULL; obj = obj->next_in_carry_list)
      weight += get_obj_weight(obj);

   return weight;
}
