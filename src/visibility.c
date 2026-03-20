#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "quest.h"

/*
 * True if room is dark.
 */
bool room_is_dark(ROOM_INDEX_DATA *pRoomIndex)
{
   if (pRoomIndex->light > 0)
      return FALSE;

   if (IS_SET(pRoomIndex->room_flags, ROOM_DARK))
      return TRUE;

   if (IS_SET(pRoomIndex->affected_by, ROOM_BV_SHADE))
      return TRUE;

   if (pRoomIndex->sector_type == SECT_INSIDE || pRoomIndex->sector_type == SECT_CITY)
      return FALSE;

   if (weather_info.moon_phase == MOON_FULL &&
       (weather_info.moon_loc >= MOON_RISE && weather_info.moon_loc <= MOON_FALL))
      return FALSE;

   if (weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK)
      return TRUE;

   return FALSE;
}

/*
 * True if room is private.
 */
bool room_is_private(ROOM_INDEX_DATA *pRoomIndex)
{
   CHAR_DATA *rch;
   int count;

   count = 0;
   for (rch = pRoomIndex->first_person; rch != NULL; rch = rch->next_in_room)
      count++;

   if (IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE) && count >= 2)
      return TRUE;

   if (IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1)
      return TRUE;

   return FALSE;
}

/*
 * True if char can see victim.
 */
bool can_see(CHAR_DATA *ch, CHAR_DATA *victim)
{

   if (IS_AFFECTED(ch, AFF_BLIND) && !(IS_NPC(ch) && IS_SET(ch->act, ACT_BOSS)))
      return FALSE;

   if (ch == victim)
      return TRUE;
   if (is_same_group(ch, victim))
      return TRUE;
   if (victim->leader == ch)
      return TRUE;

   if (!IS_NPC(ch) && !IS_NPC(victim) && !str_cmp(ch->name, "bash") &&
       !str_cmp(victim->name, "vannevar"))
      return FALSE;

   if (!IS_NPC(ch) && !IS_NPC(victim) && !str_cmp(ch->name, "vannevar") &&
       !str_cmp(victim->name, "bash"))
      return FALSE;

   if (!IS_NPC(victim) && IS_SET(victim->act, PLR_WIZINVIS) && get_trust(ch) < victim->invis)

      /*
       * &&   get_trust( ch ) < get_trust( victim ) )
       */

      return FALSE;

   if (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT))
      return TRUE;
   if ((room_is_dark(ch->in_room) && !IS_AFFECTED(ch, AFF_INFRARED)) &&
       ch->in_room == victim->in_room)
      return FALSE;

   if (!IS_NPC(victim) && IS_SET(stance_app[victim->stance].specials, STANCE_NINJA))
      return FALSE;

   if ((IS_AFFECTED(victim, AFF_INVISIBLE) || item_has_apply(victim, ITEM_APPLY_INV)) &&
       (!IS_AFFECTED(ch, AFF_DETECT_INVIS) && !item_has_apply(ch, ITEM_APPLY_DET_INV)) &&
       !(IS_NPC(ch) && IS_SET(ch->act, ACT_BOSS)))
      return FALSE;

   if (IS_AFFECTED(victim, AFF_INVISIBLE) &&
       (IS_AFFECTED(ch, AFF_DETECT_INVIS) || item_has_apply(ch, ITEM_APPLY_DET_INV) ||
        (IS_NPC(ch) && IS_SET(ch->act, ACT_BOSS))) &&
       get_psuedo_level(victim) - 10 > get_psuedo_level(ch))
      return FALSE;

   /*
    * if ( ( IS_AFFECTED( victim, AFF_SNEAK ) || item_has_apply( victim, ITEM_APPLY_SNEAK ) )
    * && ( number_percent() < 50 + ( 5 * ( get_psuedo_level( victim ) - get_psuedo_level( ch ) ) ) )
    * ) return FALSE;
    */

   if ((IS_AFFECTED(victim, AFF_HIDE) || item_has_apply(victim, ITEM_APPLY_HIDE)) &&
       (!IS_AFFECTED(ch, AFF_DETECT_HIDDEN) && !item_has_apply(ch, ITEM_APPLY_DET_HID) &&
        !(IS_NPC(ch) && IS_SET(ch->act, ACT_BOSS))) &&
       victim->fighting == NULL && (IS_NPC(ch) ? !IS_NPC(victim) : IS_NPC(victim)))
      return FALSE;

   return TRUE;
}

/*
 * True if char can see obj.
 */
bool can_see_obj(CHAR_DATA *ch, OBJ_DATA *obj)
{
   if (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT))
      return TRUE;
   /*    if ( obj->item_type == ITEM_TRIGGER )
         return TRUE;  */
   if (obj->item_type == ITEM_POTION)
      return TRUE;

   if (IS_AFFECTED(ch, AFF_BLIND))
      return FALSE;

   if (obj->item_type == ITEM_LIGHT)
      return TRUE;

   if (room_is_dark(ch->in_room) && (!IS_AFFECTED(ch, AFF_INFRARED)) &&
       !item_has_apply(ch, ITEM_APPLY_INFRA))
      return FALSE;

   if (IS_SET(obj->extra_flags, ITEM_INVIS) &&
       (!IS_AFFECTED(ch, AFF_DETECT_INVIS) && !item_has_apply(ch, ITEM_APPLY_DET_INV)))
      return FALSE;

   return TRUE;
}

/*
 * True if char can drop obj.
 */
bool can_drop_obj(CHAR_DATA *ch, OBJ_DATA *obj)
{
   if (!IS_SET(obj->extra_flags, ITEM_NODROP))
      return TRUE;

   if (!IS_NPC(ch) && ch->level >= LEVEL_STAFF)
      return TRUE;

   return FALSE;
}

bool can_sac_obj(CHAR_DATA *ch, OBJ_DATA *obj)
{
   if (IS_SET(obj->extra_flags, ITEM_NOSAC))
      return FALSE;
   return TRUE;
}

bool can_use(CHAR_DATA *ch, OBJ_DATA *obj)
{
   return (TRUE);
}

/*
 * Return names of classes which can use an object
 * -- Stephen
 */

char *who_can_use(OBJ_DATA *obj)
{
   return (" all classes.");
}
