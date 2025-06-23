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


#include <stdio.h>
#include <stdlib.h>  /* For div_t, div() */
#include <string.h>
#include <time.h>
#include "globals.h"
#include "magic.h"
#include "tables.h"
#include <math.h>

extern POL_DATA politics_data;
extern CHAR_DATA *quest_target;
extern CHAR_DATA *quest_mob;

/*
 * Local functions.
 */
bool check_avoidance args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool check_counter args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void check_killer args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
bool check_skills args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void dam_message args( ( CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt, bool critical ) );
void death_message args( ( CHAR_DATA * ch, CHAR_DATA * victim, int dt, int max_dt ) );
void death_cry args( ( CHAR_DATA * ch ) );
void group_gain args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void do_knee args( ( CHAR_DATA *ch, char *argument ) );
bool do_lifesteal args( ( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *wield, bool dual, int dam ) );

bool is_safe args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void make_corpse args( ( CHAR_DATA * ch, char *argument ) );
void one_hit args( ( CHAR_DATA * ch, CHAR_DATA * victim, int dt ) );
void raw_kill args( ( CHAR_DATA * victim, char *argument ) );
void set_fighting args( ( CHAR_DATA * ch, CHAR_DATA * victim, bool check ) );
void disarm args( ( CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj ) );
void trip args( ( CHAR_DATA * ch, CHAR_DATA * victim ) );
void check_adrenaline args( ( CHAR_DATA * ch, sh_int damage ) );
void obj_damage args( ( OBJ_DATA * obj, CHAR_DATA * victim, int dam ) );

int do_damage args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, bool critical, int element) );
int damage args( ( CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt ) );

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;
   CHAR_DATA *victim;
   CHAR_DATA *rch;
   CHAR_DATA *rch_next;
   AFFECT_DATA *paf;

   /*
    * CHAR_DATA *check_char;   
    */
/*    extern CHAR_DATA *violence_marker;
    CHAR_DATA	*marker;  */
   bool has_cast = FALSE;

   CREF( ch_next, CHAR_NEXT );
   for( ch = first_char; ch; ch = ch_next )
   {
      ch_next = ch->next;

      /*
       * For stunning during combat
       * -Damane-    4/26/96 
       */

      if( ch->position == POS_STUNNED )
      {
         if( ch->wait > 0 )
         {
            ch->wait -= 1;
            continue;
         }
         else
         {
            ch->position = POS_STANDING;
            send_to_char( "You are no longer stunned.\n\r", ch );
            continue;
         }
      }
      has_cast = FALSE;

/* Healing rapidly for raged wolves  */

      if( !IS_NPC( ch ) && IS_WOLF( ch ) && IS_RAGED( ch ) )
      {
         if( !is_affected( ch, skill_lookup( "Enraged" ) ) )
            REMOVE_BIT( ch->pcdata->pflags, PFLAG_RAGED );
         ch->hit = ( UMIN( ch->max_hit, ( ch->hit + ch->max_hit / 150 ) ) );

      }

      // Hots and dots
      for( paf = ch->first_affect; paf != NULL; paf = paf->next )
      {
         if (paf->location == APPLY_HOT && ch->hit < ch->max_hit && is_same_room(ch, paf->caster))
         {
            heal_character(paf->caster, ch, paf->modifier, paf->type, TRUE);
         }
         if (paf->location == APPLY_DOT && ch->hit > 0 && is_same_room(ch, paf->caster))
         {
            do_damage(paf->caster, ch, paf->modifier, paf->type, FALSE, REALM_IMPACT);
         }
      }

/* slight damage for players in a speeded stance, simulates fatigue */

      if( !IS_NPC( ch ) && ( stance_app[ch->stance].speed_mod > 1 ) )
      {
         ch->hit =
            UMAX( 10, ch->hit - number_range( get_psuedo_level( ch ) * 5 / 1000, get_psuedo_level( ch ) * 10 / 1000 ) );
      }

      if( ch->hit > 0
          && ch->in_room != NULL && get_room_index( ch->in_room->vnum ) != NULL && item_has_apply( ch, ITEM_APPLY_HEATED ) )
      {
         OBJ_DATA *heated_item;
         int heat_damage = 0;

         for( heated_item = ch->first_carry; heated_item != NULL; heated_item = heated_item->next_in_carry_list )
         {
            if( IS_SET( heated_item->item_apply, ITEM_APPLY_HEATED ) && heated_item->wear_loc != WEAR_NONE )
            {
               heat_damage = heated_item->level;
               obj_damage( heated_item, ch, heat_damage );
               act( "@@W   $p@@N you are wearing are @@eBURNING@@N you!!!", ch, heated_item, NULL, TO_CHAR );
               act( "@@W   $p worn by $n is @@eBURNING@@N!!!", ch, heated_item, NULL, TO_ROOM );
               if( IS_NPC( ch ) )
                  do_remove( ch, heated_item->name );
            }
         }
      }

      if( ch->stunTimer > 0 )
      {
         ch->stunTimer--;
         continue;
      }

      if( ( ch->is_free == FALSE ) && ( IS_NPC( ch ) ) && IS_SET( ch->act, ACT_SOLO ) && ch->hit > 0 )
      {
         if( ( ch->hit < ch->max_hit * 3 / 4 ) && ( ch->mana > mana_cost( ch, skill_lookup( "heal" ) ) ) )
         {
            do_cast( ch, "heal self" );
         }
      }
      else if( IS_NPC( ch ) && ch->hit < 0 )
      {
         ch->position = POS_DEAD;
         if( ch->fighting == NULL )
            if( ch->in_room != NULL )
               act( "Suddenly, $n is enveloped in a @@mBeam of light@@N, and is gone!", ch, NULL, NULL, TO_ROOM );
         stop_fighting( ch, TRUE );
         extract_char( ch, TRUE );
         continue;
      }

      area_resetting_global = TRUE;
      if( ( ch->is_free == FALSE ) && ( IS_NPC( ch ) ) && ( !IS_SET( ch->def, DEF_NONE ) ) && ch->hit > 0 )
      {
         if( ch->hit < ch->max_hit * 2 / 3 )
         {
            if( IS_SET( ch->def, DEF_CURE_LIGHT ) )
            {
               if( ch->mana > mana_cost( ch, skill_lookup( "cure light" ) ) )
               {
                  do_cast( ch, "\'cure light\' self" );
                  has_cast = TRUE;
               }
            }
            if( IS_SET( ch->def, DEF_CURE_SERIOUS ) )
            {
               if( ch->mana > mana_cost( ch, skill_lookup( "cure serious" ) ) )
               {
                  do_cast( ch, "\'cure serious\' self" );
                  has_cast = TRUE;
               }
            }
            if( IS_SET( ch->def, DEF_CURE_CRITIC ) )
            {
               if( ch->mana > mana_cost( ch, skill_lookup( "cure critical" ) ) )
               {
                  do_cast( ch, "\'cure critical\' self" );
                  has_cast = TRUE;
               }
            }
            if( IS_SET( ch->def, DEF_CURE_HEAL ) )
            {
               if( ch->mana > mana_cost( ch, skill_lookup( "heal" ) ) )
               {
                  do_cast( ch, "heal self" );
                  has_cast = TRUE;
               }
            }
         }
      }
      else if( IS_NPC( ch ) && ch->hit < 0 )
      {
         ch->position = POS_DEAD;
         if( ch->fighting == NULL )
            if( ch->in_room != NULL )
               act( "Suddenly, $n is enveloped in a @@mBeam of light@@N, and is gone!", ch, NULL, NULL, TO_ROOM );
         extract_char( ch, TRUE );
         continue;
      }

      if( ( ch->is_free == FALSE )
          && ( IS_NPC( ch ) )
          && ( !IS_SET( ch->def, DEF_NONE ) )
          && ( ch->hit > 0 ) && ( ch->first_shield == NULL ) && ( !has_cast ) && ( ch->fighting == NULL ) )
      {
         if( ( IS_SET( ch->def, DEF_SHIELD_FIRE ) )
             && ( !is_affected( ch, skill_lookup( "fireshield" ) ) )
             && ( ch->mana > mana_cost( ch, skill_lookup( "fireshield" ) ) ) )
         {
            do_cast( ch, "fireshield" );
            has_cast = TRUE;
         }
         else
            if( ( IS_SET( ch->def, DEF_SHIELD_ICE ) )
                && ( !is_affected( ch, skill_lookup( "iceshield" ) ) )
                && ( ch->mana > mana_cost( ch, skill_lookup( "iceshield" ) ) ) )
         {
            do_cast( ch, "iceshield" );
            has_cast = TRUE;
         }
         else
            if( ( IS_SET( ch->def, DEF_SHIELD_SHOCK ) )
                && ( !is_affected( ch, skill_lookup( "shockshield" ) ) )
                && ( ch->mana > mana_cost( ch, skill_lookup( "shockshield" ) ) ) )
         {
            do_cast( ch, "shockshield" );
            has_cast = TRUE;
         }
      }
      area_resetting_global = FALSE;
/* Offensive spell handler, only use when actually fighting.. */

      if( ( IS_NPC( ch ) )
          && ( ch->is_free == FALSE )
          && ( ch->cast > 1 )
          && ( !has_cast )
          && ( ch->position > POS_RESTING )
          && ( ch->fighting != NULL )
          && ( ch->fighting->is_free != TRUE )
          && ( ch->in_room != NULL ) && ( ch->hit > 1 ) && ( ch->position == POS_FIGHTING ) )

      {
         sh_int cast_frequency;
         sh_int index;

         cast_frequency = get_psuedo_level( ch ) / 2; /* maybe set in olc later? */
         if( ( number_range( 0, 99 ) < cast_frequency ) && ( ch->mana >= ( 40 * ch->max_mana / 100 ) ) )
         {
            for( index = 1; index < 32; index++ )
            {
               if( ( IS_SET( ch->cast, ( 1 << index ) ) )
                   && ( number_range( 0, 99 ) < ( index * 3 + number_range( 0, 25 ) ) )
                   && ( ch->mana > mana_cost( ch, skill_lookup( rev_table_lookup( tab_cast_name, ( 1 << index ) ) ) ) ) )
               {
                  char cast_name[MSL];
                  sprintf( cast_name, "%s %s", rev_table_lookup( tab_cast_name, ( 1 << index ) ), ch->fighting->name );
                  do_cast( ch, cast_name );
                  has_cast = TRUE;
                  break;
               }
            }
         }
      }
      if( ( IS_NPC( ch ) )
          && ( ch->is_free == FALSE )
          && ( ch->position > POS_RESTING )
          && ( ch->fighting != NULL )
          && ( ch->fighting->is_free == FALSE )
          && ( ch->in_room != NULL ) && ( ch->hit > 1 ) && ( ch->position == POS_FIGHTING ) )
      {
         check_skills(ch, ch->fighting);
      }
      /*
       * Hunting mobs.
       * -S- Mod: use flags to work out what to do.... 
       */
      if( IS_NPC( ch ) && ch->fighting == NULL && IS_AWAKE( ch ) )
      {
         mob_hunt( ch );
         continue;
      }
      if( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
      {
         continue;
      }

      if( ( IS_AWAKE( ch ) )
          && ( ch->is_free == FALSE )
          && ( victim->is_free == FALSE )
          && ( ch->in_room != NULL ) && ( victim->in_room != NULL ) && ( ch->in_room == victim->in_room ) )
         multi_hit( ch, victim, TYPE_UNDEFINED );
      else
         stop_fighting( ch, FALSE );

      if( ( victim = ch->fighting ) == NULL )
         continue;

      mprog_hitprcnt_trigger( ch, victim );
      mprog_fight_trigger( ch, victim );

      /*
       * Fun for the whole family!   RCH is a non-fighting mob
       */
      if( IS_NPC( victim ) && ( get_psuedo_level( victim ) > 15 ) )
      {
         CREF( rch_next, CHAR_NEXTROOM );
         for( rch = ch->in_room->first_person; rch != NULL; rch = rch_next )
         {
            rch_next = rch->next_in_room;
            if( !IS_NPC( rch ) )
               continue;

            if( IS_AWAKE( rch ) && rch->fighting == NULL )
            {
               /*
                * NPC's assist NPC's of same type or 45% chance regardless.
                */
               if( !IS_AFFECTED( rch, AFF_CHARM ) && !IS_SET( rch->act, ACT_NOASSIST ) )
               {
                  if( ( rch->pIndexData == victim->pIndexData )   /* is it the same as a target here?  */
                      || ( ( number_percent(  ) < 20 )
                           && ( abs( get_psuedo_level( rch ) - get_psuedo_level( victim ) ) < 35 ) ) )
                  {
                     CHAR_DATA *vch;
                     CHAR_DATA *target;
                     int number;

                     target = NULL;
                     number = 0;

                     /*
                      * vch is the target of the lazy mob...a player 
                      */
                     for( vch = ch->in_room->first_person; vch; vch = vch->next )
                     {
                        if( ( can_see( rch, vch ) ) && ( !IS_NPC( vch ) ) )
                        {
                           target = vch;
                           number++;
                        }
                     }

                     if( target != NULL )
                     {
                        if( abs( target->level - rch->level ) < 40 )
                           multi_hit( rch, target, TYPE_UNDEFINED );
                     }
                  }
               }
            }
         }
         CUREF( rch_next );
      }
   }
   CUREF( ch_next );
   return;
}

/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA * ch, CHAR_DATA * victim, int dt )
{
   int chance;
   OBJ_DATA *wield1 = NULL;
   OBJ_DATA *wield2 = NULL;
   int dual_chance = 0;
   bool multi_hit = FALSE;

   if( IS_SET(stance_app[ch->stance].specials, STANCE_NO_HIT ) )
      return;

   if( ch->position == POS_RIDING )
   {
      if( ch->riding && ( ch->riding->in_room == ch->in_room ) )
      {
         do_dismount( ch, "" );
      }
      else
      {
         ch->position = POS_FIGHTING;
      }
   }
   if( ( ( ( wield1 = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) != NULL )
         && ( wield1->item_type == ITEM_WEAPON ) )
       && ( ( ( wield2 = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) != NULL ) && ( wield2->item_type == ITEM_WEAPON ) ) )
      dual_chance = 15;

   int hits = 0;

   if (IS_NPC(ch))
   {
      if (IS_SET( ch->skills, MOB_SECOND ))
         hits++;
      if (IS_SET( ch->skills, MOB_THIRD ))
         hits++;
      if (IS_SET( ch->skills, MOB_FOURTH ))
         hits++;
      if (IS_SET( ch->skills, MOB_FIFTH ))
         hits++;
      if (IS_SET( ch->skills, MOB_SIXTH ))
         hits++;
   }
   else
   {
      // Remort hits
      if (ch->remort[CLASS_PAL] > 0)
         hits++;
      if (ch->remort[CLASS_ASS] > 0)
         hits++;
      if (ch->remort[CLASS_WLK] > 0)
         hits++;
      if (ch->remort[CLASS_KNI] > 0)
         hits++;
      if (ch->remort[CLASS_SWO] > 0)
         hits++;

      // Mort hits
      for(int i = 0; i < MAX_CLASS; i++)
      {
         if (class_table[ch->pcdata->order[i]].attr_prime == APPLY_INT ||
             class_table[ch->pcdata->order[i]].attr_prime == APPLY_WIS)
             continue;

         if (i == 0 && ch->lvl[ch->pcdata->order[i]] > 10)
            hits++;
         if (i == 1 && ch->lvl[ch->pcdata->order[i]] > 20)
            hits++;
         if (i == 2 && ch->lvl[ch->pcdata->order[i]] > 40)
            hits++;
         if (i == 3 && ch->lvl[ch->pcdata->order[i]] > 80)
            hits++;
      }
   }

   // First hit
   one_hit( ch, victim, dt );

   if (ch->fighting != victim)
      return;

   for(int i = 0; i < hits; i++)
   {
      chance = 80 - (i*10);
      chance += stance_app[ch->stance].speed_mod;
      chance += dual_chance;

      int calc_chance = number_percent();

      if (chance > calc_chance)
      {
         one_hit( ch, victim, dt );

         if (ch->fighting != victim)
            break;
      }
   }

   if( !IS_NPC( ch ) && ch->stance > 0 && ( ( IS_SET(stance_app[victim->stance].specials, STANCE_NINJA ) ) ) )
   {
      send_to_char( "You step out of the shadows.\n\r", ch );
      ch->stance = 0;
      act( "$n steps out of the Shadows!", ch, NULL, NULL, TO_ROOM );
   }
   return;
}

#define ZERO_RATIO 20
#define ONE_STEPS 100l
#define MAX_IX 120
#define MAX_DAM_MOD 1.4

/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA * ch, CHAR_DATA * victim, int dt )
{
   OBJ_DATA *wield;
   OBJ_DATA *dualwield = NULL;
   OBJ_DATA *shield;
   int victim_ac;
   int remort_bonus;
   int dam;
   int diceroll;
   int ix;
   float dam_mod;
   extern const float hr_damTable[121];
   /*
    * Can't beat a dead char!
    * Guard against weird room-leavings.
    */
   if( victim->position == POS_DEAD || ch->in_room != victim->in_room )
      return;

   /*
    * Figure out the type of damage message.
    */
   wield = get_eq_char( ch, WEAR_HOLD_HAND_L );
   if( !IS_WEAPON( wield ) )
      wield = get_eq_char( ch, WEAR_HOLD_HAND_R );
   if( IS_WEAPON(wield) )
      dualwield = get_eq_char( ch, WEAR_HOLD_HAND_R );
   if( !IS_WEAPON( wield ) )
      wield = NULL;
   if( dt == TYPE_UNDEFINED )
   {
      dt = TYPE_HIT;
      if( wield != NULL && wield->item_type == ITEM_WEAPON && !IS_SET(wield->extra_flags, ITEM_FIST))
         dt += wield->value[3];
   }

   if ( dt == TYPE_HIT && (wield == NULL || IS_SET(wield->extra_flags, ITEM_FIST) ) )
   {
      int chance = 0;

      if ( IS_NPC(ch) && IS_SET(ch->skills, MOB_MARTIAL) )
         chance = 75;

      if ( !IS_NPC(ch) && can_use_skill(ch, gsn_martial_arts) )
         chance = 50;

      if (number_percent() < chance)
         dt = TYPE_MARTIAL;
   }

   victim_ac = get_ac( victim );
   if( !can_see( ch, victim ) )
      victim_ac -= 200;

   if( dt == gsn_backstab || dt == gsn_circle )
      victim_ac += 300;

   diceroll = number_range( ( get_psuedo_level( ch ) * 5 ), ( get_psuedo_level( ch ) * 21 ) ) + get_hitroll( ch );

   if( !IS_NPC( ch ) )
      diceroll += number_range( get_psuedo_level( ch ), ( get_psuedo_level( ch ) * 1 ) );
   if( ( remort_bonus = get_psuedo_level( ch ) > 100 ) )
      diceroll += remort_bonus * 1;

   if( IS_NPC( ch ) )
   {
      diceroll += ( get_psuedo_level( ch ) * 5 );
      if( IS_SET( ch->act, ACT_SOLO ) )
         diceroll += ( get_psuedo_level( ch ) * 5 );
   }
   if( IS_AFFECTED( ch, AFF_CLOAK_ADEPT ) )
      diceroll += get_psuedo_level( ch ) * 2;

/* Player vs player bonus, to handle unbalanced hitroll vs ac */
   if( !IS_NPC( ch ) && !IS_NPC( victim ) && get_psuedo_level( ch ) > 80 && get_psuedo_level( victim ) > 80 )
      diceroll += number_range( 1000, 2000 );
   if( IS_NPC( ch ) && victim_ac < -3000 && get_psuedo_level( ch ) > 110 && ( number_range( 0, 100 ) < 10 ) )
      diceroll += 3000;

   if( victim_ac > -100 )
   {
      if( diceroll + victim_ac < 0 )
         ix = -1;
      else
         ix = ZERO_RATIO;
   }
   else
   {
      /*
       * This finds the ratio of excess hit roll to AC, and
       * breaks it into steps as defined by constants above,
       * for use in damage modifier lookup table.
       * Long int calculation avoids overflow problems. 
       */
      ix = -( ONE_STEPS * ( long int )( diceroll + victim_ac ) ) / ( long int )victim_ac;
      ix += ZERO_RATIO;
   }

   if( ix < 0 )
   {
      dam_mod = 0.0;

      damage( ch, victim, 0, dt );
      tail_chain(  );
      return;
   }
   else if( ix <= MAX_IX )
      dam_mod = hr_damTable[ix];
   else
      dam_mod = MAX_DAM_MOD;

   dam = dam * dam_mod;
   /*
    * Hit.
    * Calc damage.
    * Tried to make it easy for players to hit mobs... --Stephen
    */
   if( IS_NPC( ch ) )
   {
      if( wield )
         dam = number_range( wield->value[1], wield->value[2] );
      else
         dam = number_range( ch->level / 3, ch->level / 2 );
      if( IS_SET( ch->act, ACT_SOLO ) )
         dam *= 1.5;

   }
   else
   {
      if( wield != NULL )
         dam = number_range( wield->value[1], wield->value[2] );
      else
         dam = UMAX( number_range( 2, 4 ), ch->level / 4 );
   }

   /*
    * Bonuses.
    */
   dam += number_range( get_damroll( ch ) * 13 / 20, get_damroll( ch ) * 15 / 20 );

   if (can_use_skill(ch, gsn_enhanced_damage) )
      dam += dam * 0.6;
   else if( IS_NPC( ch ) && IS_SET(ch->skills, MOB_ENHANCED) )
      dam += dam * 0.2;
   else if (item_has_apply(ch, ITEM_APPLY_ENHANCED) )
      dam += dam * 0.4;

   dam += dam * ch->remort[CLASS_KNI]/100;
   dam += dam * ch->remort[CLASS_SWO]/100;
   dam += dam * ch->remort[CLASS_PAL]/100 * .75;

   if ( !IS_NPC(ch) && wield && wield->value[3] == 3 && can_use_skill(ch, gsn_enhanced_sword) )
   {
      dam += dam * number_range(20,40)/100;
   }

   if( !IS_AWAKE( victim ) )
      dam *= 1.5;
   /*
    * extra damage from martial arts 
    */
   if( dt == TYPE_MARTIAL )
      dam = ( dam * 4 ) / 3;

   if (!IS_NPC(ch) && can_use_skill(ch, gsn_bare_hand) )
   {
      wield = get_eq_char(ch, WEAR_HOLD_HAND_L);
      if ( wield == NULL ||
           (wield->value[3] == 0 && can_use_skill(ch, gsn_equip_fist) && IS_SET(wield->extra_flags, ITEM_FIST) ) )
      {
         wield = get_eq_char(ch, WEAR_HOLD_HAND_R);

         if (wield == NULL ||
            (wield->value[3] == 0 && can_use_skill(ch, gsn_equip_fist) && IS_SET(wield->extra_flags, ITEM_FIST) ) )
         {
            if (ch->remort[CLASS_MON] > 0)
               dam += dam * ch->remort[CLASS_MON] / 100;
            else if (ch->remort[CLASS_BRA] > 0)
               dam += dam * ch->remort[CLASS_BRA] / 100 * .75;
         }
      }
   }

   dam = swing(ch, victim, dam, dt);

   bool stole_life = FALSE;
   if( ( wield ) && ( dam > 0 ) && ( ( IS_OBJ_STAT( wield, ITEM_LIFESTEALER ) ) ) )
      stole_life = do_lifesteal(ch, victim, wield, FALSE, dam);
   if ( !stole_life && dualwield && dam > 0 && IS_OBJ_STAT(dualwield, ITEM_LIFESTEALER))
      do_lifesteal(ch, victim, dualwield, TRUE, dam);

}

int swing(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt)
{
   bool critical = FALSE;

   if (dam > 200 && (dt >= TYPE_HIT || dt < 0))
   {
      dam -= 200;
      dam /= 2;
      dam += 200;
   }

   if (number_range(0,100) < get_crit( ch ))
   {
      int crit_mult = get_crit_mult(ch);

      dam += dam * crit_mult / 100;
      critical = TRUE;
   }

   if( dt == gsn_backstab )
      dam *= 1.4;
   if( dt == gsn_circle )
      dam *= 1.1;
   if( IS_AFFECTED( ch, AFF_CLOAK_ADEPT ) )
      dam *= 1.2;
   if( dam <= 0 )
      dam = 1;

   dam -= dam * get_curr_con(ch) / 100;

   int skin_mods;
   if( !IS_NPC( victim ) )
      skin_mods = race_table[victim->race].race_flags;
   else
      skin_mods = ( victim->race == 0 ? victim->pIndexData->race_mods : race_table[victim->race].race_flags );

   if( IS_SET( skin_mods, RACE_MOD_TOUGH_SKIN ) )
      dam = dam * 0.9;

   if( IS_SET( skin_mods, RACE_MOD_STONE_SKIN ) )
      dam = dam * 0.8;

   if( IS_SET( skin_mods, RACE_MOD_IRON_SKIN ) )
      dam = dam * 0.7;

   if( IS_AFFECTED( victim, AFF_SANCTUARY ) || item_has_apply( victim, ITEM_APPLY_SANC ) )
      dam /= 2;

   if( ( IS_AFFECTED( victim, AFF_PROTECT ) || item_has_apply( ch, ITEM_APPLY_PROT ) ) && IS_EVIL( ch ) )
      dam -= dam / 4;

   int return_val = do_damage( ch, victim, dam, dt, critical, REALM_PHYSICAL );

   tail_chain( );

   return return_val;
}

/*
 * Inflict damage from a hit.
 */
int damage( CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt )
{
   return do_damage(ch, victim, dam, dt, FALSE, REALM_PHYSICAL);
}

bool is_safe( CHAR_DATA * ch, CHAR_DATA * victim )
{
   if( deathmatch )
      return FALSE;  /* Deathmatch? Anything goes!! */


/*we are going to have safe rooms no attacks at all  SRZ 2 Jul 96
    if ( IS_NPC(ch) || IS_NPC(victim) )
	return FALSE;
*/


   /*
    * No PC vs. PS attacks in safe rooms 
    */
   if( ( victim->in_room != NULL ) && IS_SET( victim->in_room->room_flags, ROOM_SAFE ) )
   {
      send_to_char( "Not a chance!  This is a safe room.\n\r", ch );
      return TRUE;
   }

   if( IS_SET( victim->act, PLR_KILLER ) )
      return FALSE;
   if( IS_SET( victim->act, PLR_THIEF ) )
      return FALSE;
   /*
    * Vampires are considered PKOK 
    */

   if( !IS_NPC( ch ) && !IS_NPC( victim )
       && ( IS_SET( victim->pcdata->pflags, PFLAG_PKOK )
            || IS_SET( victim->pcdata->pflags, PFLAG_VAMP ) )
       && ( IS_SET( ch->pcdata->pflags, PFLAG_PKOK ) || IS_SET( ch->pcdata->pflags, PFLAG_VAMP ) ) )
      return FALSE;

   if( ( ( victim->level < 10 ) || ( victim->level + 20 < ch->level ) ) && ( !IS_NPC( victim ) ) && ( !IS_NPC( ch ) ) )
   {
      send_to_char( "The Gods prevent your foul deed.\n\r", ch );
      return TRUE;
   }

   return FALSE;
}

struct hunt_mobs_tp
{
   int mob_vnum;
   int room_vnum;
   int min_level;
   char *name;
}

hunt_mobs[] =
{
   {
   3062, 3001, 0, "Bug"},
   {
   3561, 3001, 40, "Shadow Dragon"},
   {
   18302, 3001, 80, "Bounty Hunter"},
   {
   18301, 3001, 100, "The Avenger"},
   {
   18306, 3001, 240, "Blayze"},
   {
   -1, -1, -1, " "}
};

/*
 * See if an attack justifies a KILLER flag.
 */
void check_killer( CHAR_DATA * ch, CHAR_DATA * victim )
{
   /*
    * Follow charm thread to responsible character.
    * Attacking someone's charmed char is hostile!
    * -S- Mod:  Set sentence according to difference in levels -
    * this is no. times the player must die before the killer flag
    * will be removed.
    */

   int diff;
   int a;
   MOB_INDEX_DATA *pMobIndex;
   ROOM_INDEX_DATA *pRoomIndex;
   CHAR_DATA *hunter;

   if( ( ch->fighting == victim ) )
      return;
/*
   if ( ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL ) 
      && ( ch->fighting != victim )  )
	victim = victim->master;   */

   /*
    * NPC's are fair game.
    * So are killers and thieves.
    */
   if( IS_NPC( victim ) || IS_SET( victim->act, PLR_KILLER ) || IS_SET( victim->in_room->room_flags, ROOM_PK ) /* -S- Mod */
       || IS_SET( victim->act, PLR_THIEF ) || IS_VAMP( victim ) || IS_WOLF( victim )   /*
                                                                                        * || ( ch->fighting == victim )  */
       || ( ch == victim ) )
      return;

   /*
    * if in deathmatch, anything goes 
    */
   if( deathmatch )
      return;
   if( ch->sentence > 5000 )
      return;

   /*
    * Check to see if ch & victim are in clans, and enemies 
    */

   if( !IS_NPC( ch ) && !IS_NPC( victim ) )
   {

      if( ( ch->pcdata->clan != 0 ) && ( victim->pcdata->clan != 0 )
          && ( politics_data.diplomacy[ch->pcdata->clan][victim->pcdata->clan] < -450 ) )
         return;

      if( IS_SET( ch->pcdata->pflags, PFLAG_PKOK ) && IS_SET( victim->pcdata->pflags, PFLAG_PKOK ) )
         return;
   }

   /*
    * NPC's are cool of course (as long as not charmed).
    * Hitting yourself is cool too (bleeding).
    * So is being immortal (Alander's idea).
    * BAH.  Imms get flagged too now, unless pkok.
    * And current killers stay as they are.
    */
   if( IS_NPC( ch ) || ch == victim )
      return;

   send_to_char( "*** You are a PLAYER KILLER!! ***\n\r", ch );

   {
      char buf[MAX_STRING_LENGTH];

      sprintf( buf, "%s flagged as a KILLER for attack on %s.", ch->name, victim->name );
      monitor_chan( buf, MONITOR_COMBAT );
   }
   diff = 3;
   if( get_psuedo_level( ch ) > get_psuedo_level( victim ) )
   {
      diff += ( get_psuedo_level( ch ) - get_psuedo_level( victim ) ) / 7;
      if( diff > 5 )
         diff = 5;
   }
   ch->sentence += diff * get_psuedo_level( ch ) * 3; /* Magic # - Ramias */
   if( ch->adept_level > 0 )
      ch->sentence += diff * get_psuedo_level( ch ) * 2;

   SET_BIT( ch->act, PLR_KILLER );
   save_char_obj( ch );


   /*
    * MAG Create a hunter for the person 
    */
   diff = get_psuedo_level( ch );

   /*
    * Added if check back... meant to penalize for attacking lower
    * * level players -S-
    */

   if( get_psuedo_level( ch ) > get_psuedo_level( victim ) )
      diff += get_psuedo_level( ch ) - get_psuedo_level( victim );
/*     
    if (diff > MAX_LEVEL)
     diff=MAX_LEVEL;
                       SRZ just set the level of the hunter to the level of the player :) */

   /*
    * Fixed for loop so it can't become infinite -S- 
    */
   for( a = 0; hunt_mobs[a].min_level != -1; a++ )
   {
      if( diff <= hunt_mobs[a].min_level )
         break;
   }

   if( hunt_mobs[a].mob_vnum == -1 )
   {
      notify( "No killer hunter found in check_killer!", 83 );
      return;
   }

   if( ( pMobIndex = get_mob_index( hunt_mobs[a].mob_vnum ) ) == NULL )
   {
      bug( "check_killer: No such mob %i.", hunt_mobs[a].mob_vnum );
      return;
   }

   if( ( pRoomIndex = get_room_index( hunt_mobs[a].room_vnum ) ) == NULL )
   {
      bug( "check_killer: No such room %i.", hunt_mobs[a].room_vnum );
      return;
   }

   hunter = create_mobile( pMobIndex );
   char_to_room( hunter, pRoomIndex );
   /*
    * do_gossip( hunter, "Well, someone's gonna get it!" ); 
    */
   set_hunt( hunter, NULL, ch, NULL, HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR | HUNT_INFORM,
             HUNT_CR | HUNT_MERC );

   return;
}

bool check_avoidance( CHAR_DATA *ch, CHAR_DATA *victim )
{
   int max_avoidance = 75;
   int chance = number_percent();

   // Can't avoid when stunned!
   if (ch->position <= POS_STUNNED)
      return FALSE;

   if (IS_NPC(ch) && IS_SET(ch->act, ACT_SOLO))
      max_avoidance += 10;

   int parry = get_parry(victim);
   if (parry > max_avoidance)
   {
      parry = max_avoidance;
      max_avoidance = 0;
   }
   else
      max_avoidance -= parry;

   if (chance < parry + ( (get_psuedo_level(victim) - get_psuedo_level(ch)) / 2 ))
   {
      act( "You parry $n's attack.",  ch, NULL, victim, TO_VICT    );
      act( "$N parries your attack.", ch, NULL, victim, TO_CHAR    );
      act( "$N parries $n's attack.", ch, NULL, victim, TO_NOTVICT );

      check_counter(ch, victim);

      return TRUE;
   }

   int block = get_block(victim);
   if (block > max_avoidance)
   {
      block = max_avoidance;
      max_avoidance = 0;
   }
   else
      max_avoidance -= block;

   if (chance < parry+block + ( (get_psuedo_level(victim) - get_psuedo_level(ch)) / 2 ))
   {
      act( "You block $n's attack.",  ch, NULL, victim, TO_VICT    );
      act( "$N blocks your attack.", ch, NULL, victim, TO_CHAR    );
      act( "$N blocks $n's attack.", ch, NULL, victim, TO_NOTVICT );

      check_counter(ch, victim);

      return TRUE;
   }

   int dodge = get_dodge(victim);
   if (dodge > max_avoidance)
   {
      dodge = max_avoidance;
      max_avoidance = 0;
   }
   else
      max_avoidance -= dodge;
      
   if (chance < parry+block+dodge + ( (get_psuedo_level(victim) - get_psuedo_level(ch)) / 2 ))
   {
      act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT    );
      act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR    );
      act( "$N dodges $n's attack.", ch, NULL, victim, TO_NOTVICT );

      check_counter(ch, victim);

      return TRUE;
   }

   return FALSE;
}

/*
 * Check for parry.
 */
int get_parry( CHAR_DATA * ch )
{
   int chance = 0;

   if( !IS_AWAKE( ch ) )
      return chance;

   if( IS_NPC( ch ) && !IS_SET( ch->skills, MOB_PARRY ) )
      return chance;

   if( IS_NPC( ch ) )
   {
      chance = get_psuedo_level( ch ) / 3.2 + get_curr_str( ch ) * 2 / 5;
      if( IS_SET( ch->act, ACT_SOLO ) )
         chance += 15;
   }
   else
   {
      OBJ_DATA *weapon;
      if( ( weapon = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL || !IS_WEAPON(weapon))
         weapon = get_eq_char( ch, WEAR_HOLD_HAND_L );
      if( ( weapon == NULL ) || ( !IS_WEAPON( weapon ) ) )
      {
         return FALSE;
      }
      if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
         return FALSE;

      chance = ( ch->pcdata->learned[gsn_parry] / 3.5 ) + get_curr_str( ch ) * 3 / 5;
   }
   if( IS_AFFECTED( ch, AFF_CLOAK_ADEPT ) )
      chance += 5;

   return chance;
}

/*
 * Check for dodge.
 */
int get_dodge( CHAR_DATA * ch )
{
   int chance = 0;

   if( !IS_AWAKE( ch ) )
      return chance;

   if( IS_NPC( ch ) && !IS_SET( ch->skills, MOB_DODGE ) )
      return chance;

   if( IS_NPC( ch ) )
   {
      /*
       * Tuan was here.  :) 
       */
      chance = get_psuedo_level( ch ) / 3.1 + get_curr_dex( ch ) * 2 / 5;
      if( IS_SET( ch->act, ACT_SOLO ) )
         chance += 15;
   }
   else
   {
      chance = ( ch->pcdata->learned[gsn_dodge] / 3.5 ) + get_curr_dex( ch ) * 3 / 5;
      if( ch->remort[CLASS_MON] > 0 )   /* Monk  */
         chance += ch->remort[CLASS_MON] / 8;
   }
   if( IS_AFFECTED( ch, AFF_CLOAK_ADEPT ) )
      chance += 5;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
      chance += 20;

   return chance;
}

int get_block( CHAR_DATA * ch )
{
   int chance = 0;

   if( !IS_AWAKE( ch ) )
      return chance;

   if( IS_NPC( ch ) && !IS_SET( ch->skills, MOB_PARRY ) )
      return chance;

   if( IS_NPC( ch ) )
   {
      chance = get_psuedo_level( ch ) / 3.2 + get_curr_str( ch ) * 2 / 5;
      if( IS_SET( ch->act, ACT_SOLO ) )
         chance += 15;
   }
   else
   {
      OBJ_DATA *shield;
      shield = get_eq_char( ch, WEAR_HOLD_HAND_R);
      if (shield == NULL || shield->item_type != ITEM_ARMOR)
         shield = get_eq_char( ch, WEAR_HOLD_HAND_L);
      if (shield == NULL || shield->item_type != ITEM_ARMOR)
         return 0;
      if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
         return 0;

      chance = ( ch->pcdata->learned[gsn_shield_block] / 3.5 ) + get_curr_str( ch ) * 3 / 5;
   }

   if( IS_AFFECTED( ch, AFF_CLOAK_ADEPT ) )
      chance += 5;

   return chance;
}

/* Check for counter */
bool check_counter( CHAR_DATA * ch, CHAR_DATA * victim )
{
   int chance = 0;

   if( !IS_AWAKE( victim ) )
      return FALSE;

   if( IS_NPC( victim ) && !IS_SET( victim->skills, MOB_COUNTER ) )
      return FALSE;

   if (!IS_NPC(victim) && victim->pcdata->learned[gsn_counter] < 1)
      return FALSE;

   if( IS_NPC( victim ) )
   {
      chance = get_psuedo_level( victim ) / 3.1 + get_curr_dex( victim ) * 2 / 5;
      if( IS_SET( victim->act, ACT_SOLO ) )
         chance += 15;
   }
   else
   {
      chance = ( victim->pcdata->learned[gsn_counter] / 3.5 ) + get_curr_dex( victim ) * 3 / 5;
      if( victim->remort[CLASS_MON] > 0 )   /* Monk  */
         chance += victim->remort[CLASS_MON] / 8;
   }
   if( IS_AFFECTED( victim, AFF_CLOAK_ADEPT ) )
      chance += 5;

   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
      chance += 20;


   if( number_percent(  ) < ( chance + ( get_psuedo_level( victim ) - get_psuedo_level( ch ) ) / 2 ) )
   {
      one_hit(victim, ch, gsn_counter);
      return TRUE;
   }
   return FALSE;
}

/*
 * Check_skills : if IS_NPC(ch) then check ch->skills to see if there are
 * any extra attack skills available for use --Stephen
 */

bool check_skills( CHAR_DATA * ch, CHAR_DATA * victim )
{
   int cnt, check;

   if( !IS_NPC( ch ) )
      return FALSE;

   /*
    * Count how many of the attack skills are available 
    */

   cnt = 0;
   if( IS_SET( ch->skills, MOB_PUNCH ) )
      cnt++;
   if( IS_SET( ch->skills, MOB_HEADBUTT ) )
      cnt++;
   if( IS_SET( ch->skills, MOB_KNEE ) )
      cnt++;
   if( IS_SET( ch->skills, MOB_KICK ) )
      cnt++;
   if( IS_SET( ch->skills, MOB_DIRT ) )
      cnt++;
   if( IS_SET( ch->skills, MOB_CHARGE ) )
      cnt++;

   if( cnt == 0 )
      return FALSE;  /* There were no attack skills set */

   check = number_range( 1, cnt );

   cnt = 0;
   if( IS_SET( ch->skills, MOB_KICK ) && ( ++cnt == check ) )
   {
      do_kick( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->skills, MOB_PUNCH ) && ( ++cnt == check ) )
   {
      do_punch( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->skills, MOB_HEADBUTT ) && ( ++cnt == check ) )
   {
      do_headbutt( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->skills, MOB_KNEE ) && ( ++cnt == check ) )
   {
      do_knee( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->skills, MOB_DIRT ) && ( ++cnt == check ) )
   {
      do_dirt( ch, "" );
      return TRUE;
   }
   if( IS_SET( ch->skills, MOB_CHARGE ) && ( ++cnt == check ) )
   {
      do_charge( ch, "" );
      return TRUE;
   }

   return FALSE;
}

/*
 * Start fights.
 */
void set_fighting( CHAR_DATA * ch, CHAR_DATA * victim, bool check )
{
   if( ch->fighting != NULL )
   {
      /*
       * bug( "Set_fighting: already fighting", 0 );  
       */
      return;
   }

   /*
    * Check here for killer flag 
    */
   if( check )
      check_killer( ch, victim );


/*    if ( IS_NPC(victim) && IS_SET(victim->act, ACT_HUNTER ) )
                make_hunt( victim, ch );*//*
    * fun fun FUN! 
    */


   if( IS_AFFECTED( ch, AFF_SLEEP ) )
      affect_strip( ch, gsn_sleep );

   ch->fighting = victim;
   ch->position = POS_FIGHTING;

   /*
    * Check if mob has ACT_REMEMBER (ch to attack) SET 
    */

   if( IS_NPC( victim ) && !IS_NPC( ch ) && IS_SET( victim->act, ACT_REMEMBER ) )
   {
      /*
       * Then set victim->target to player's name... 
       */
      if( victim->target != NULL )
         free_string( victim->target );
      if( ch != NULL && ch->is_free == FALSE )
         victim->target = str_dup( ch->name );
   }

   return;
}

/*
 * Stop fights.
 */
void stop_fighting( CHAR_DATA * ch, bool fBoth )
{
   CHAR_DATA *fch;
   CHAR_DATA *fch_next;

   ch->fighting = NULL;
   ch->position = POS_STANDING;
   update_pos( ch );

   if( !fBoth )
      return;

   CREF( fch_next, CHAR_NEXT );
   for( fch = first_char; fch != NULL; fch = fch_next )
   {
      fch_next = fch->next;
      if( fch->fighting == ch )
      {
         fch->fighting = NULL;
         fch->position = POS_STANDING;
         update_pos( fch );
      }
   }
   CUREF( fch_next );
   return;
}

void do_kill( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Kill whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( !IS_NPC( victim ) && !( deathmatch ) )
   {
      if( !IS_SET( victim->act, PLR_KILLER ) && !IS_SET( victim->act, PLR_THIEF ) )
      {
         send_to_char( "You must MURDER a player.\n\r", ch );
         return;
      }
   }
   else
   {
      if( IS_AFFECTED( victim, AFF_CHARM ) && victim->master != NULL )
      {
         send_to_char( "You must MURDER a charmed creature.\n\r", ch );
         return;
      }
   }

   if( victim == ch )
   {
      send_to_char( "You hit yourself.  Ouch!\n\r", ch );
      multi_hit( ch, ch, TYPE_UNDEFINED );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
   {
      act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( ch->position == POS_FIGHTING )
   {
      send_to_char( "You do the best you can!\n\r", ch );
      return;
   }

   WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
   check_killer( ch, victim );
   multi_hit( ch, victim, TYPE_UNDEFINED );
   return;
}

void do_murde( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to MURDER, spell it out.\n\r", ch );
   return;
}

void do_murder( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Murder whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Suicide is a mortal sin.\n\r", ch );
      return;
   }

   if( is_safe( ch, victim ) )
   {
      send_to_char( "Not here.\n\r", ch );
      return;
   }

   if( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
   {
      act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( ch->position == POS_FIGHTING )
   {
      send_to_char( "You do the best you can!\n\r", ch );
      return;
   }

   WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
   sprintf( buf, "%s attacked by %s.\n\r", victim->name, ch->name );
   notify( buf, MAX_LEVEL - 2 );

   if( IS_NPC( ch ) || IS_NPC( victim )
       || !IS_SET( ch->pcdata->pflags, PFLAG_PKOK ) || !IS_SET( victim->pcdata->pflags, PFLAG_PKOK ) )
   {
      /*
       * If not pkok people, do yell. 
       */
      sprintf( buf, "Help! I'M BEING ATTACKED!!! ARRRGGGHHHHHH!" );
      do_yell( victim, buf );
   }
   check_killer( ch, victim );
   multi_hit( ch, victim, TYPE_UNDEFINED );
   return;
}

void do_flee( CHAR_DATA * ch, char *argument )
{
   ROOM_INDEX_DATA *was_in;
   ROOM_INDEX_DATA *now_in;
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   int attempt;
   int cost;   /* xp cost for a flee */

   if( ( victim = ch->fighting ) == NULL )
   {
      if( ch->position == POS_FIGHTING )
         ch->position = POS_STANDING;
      send_to_char( "You aren't fighting anyone.\n\r", ch );
      return;
   }

   /*
    * Check if mob will "allow" ch to flee... 
    */

   if( IS_SET( victim->act, ACT_NO_FLEE ) && !IS_NPC( ch ) && IS_NPC( victim ) )
   {
      send_to_char( "You attempt to flee from battle, but fail!\n\r", ch );
      sprintf( buf, "%s tells you 'No way will you escape ME!!'\n\r", victim->short_descr );
      send_to_char( buf, ch );
      return;
   }
   if( deathmatch )
   {
      if( ch->hit < 200 )
      {
         send_to_char( "@@eYou are in too much pain!!@@N\n\r", ch );
         return;
      }
      else
      {
         send_to_char( "@@eYour enemy nails you one more time!@@N\n\r", ch );
         ch->hit -= 150;
      }
   }

   was_in = ch->in_room;
   for( attempt = 0; attempt < 6; attempt++ )
   {
      EXIT_DATA *pexit;
      int door;

      door = number_door(  );
      if( ( pexit = was_in->exit[door] ) == 0
          || pexit->to_room == NULL
          || IS_SET( pexit->exit_info, EX_CLOSED )
          || ( IS_NPC( ch )
               && ( IS_SET( pexit->to_room->room_flags, ROOM_NO_MOB )
                    || ( IS_SET( ch->act, ACT_STAY_AREA ) && pexit->to_room->area != ch->in_room->area ) ) ) )
         continue;

      move_char( ch, door );
      if( ( now_in = ch->in_room ) == was_in )
         continue;

      ch->in_room = was_in;
      act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
      ch->in_room = now_in;

      if( !IS_NPC( ch ) )
      {
         cost = number_range( ch->exp / 15, ch->exp / 10 );
         if( ch->adept_level > 0 )
            cost /= 1000;
         cost = UMIN( cost, ch->exp );
         sprintf( buf, "You flee from combat!  You lose %d exps.\n\r", cost );
         send_to_char( buf, ch );
         gain_exp( ch, ( 0 - cost ) );
      }
      if( ( ch->fighting != NULL ) && ( AI_MOB( ch->fighting ) ) )
      {
         ch->fighting->ngroup->state = GRP_STATE_HUNTING;
         ch->fighting->ngroup->leader->hunting = ch;
      }
      stop_fighting( ch, TRUE );
      /*
       * 75% chance that mobs will hunt fleeing people. -- Alty 
       */
      if( IS_NPC( victim ) && !IS_SET( victim->act, ACT_SENTINEL ) && number_bits( 2 ) > 0 )
         set_hunt( victim, NULL, ch, NULL, HUNT_WORLD | HUNT_INFORM | HUNT_OPENDOOR, HUNT_MERC | HUNT_CR );
      return;
   }

   cost = get_psuedo_level( ch ) * 3;
   if( ch->adept_level > 0 )
      cost = 0;
   cost = UMIN( cost, ch->exp );
   sprintf( buf, "You failed!  You lose %d exps.\n\r", cost );
   send_to_char( buf, ch );
   gain_exp( ch, ( 0 - cost ) );
   return;
}

void do_sla( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
   return;
}

void do_slay( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Slay whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( ch == victim )
   {
      send_to_char( "Suicide is a mortal sin.\n\r", ch );
      return;
   }


   if( IS_HERO( victim ) )
   {
      send_to_char( "Not on other Immortal / Adept players!\n\r", ch );
      return;
   }

   if( !IS_NPC( victim ) && victim->level >= ch->level )
   {
      send_to_char( "You failed.\n\r", ch );
      return;
   }

   act( "You suck the life energy out of $M!", ch, NULL, victim, TO_CHAR );
   act( "$n sucks out your life energy!", ch, NULL, victim, TO_VICT );
   act( "$n sucks out $N's life energy!", ch, NULL, victim, TO_NOTVICT );
   raw_kill( victim, "" );
   return;
}

void do_assist( CHAR_DATA * ch, char *argument )
{
   /*
    * Allow players to join in fight, by typing assist, 
    * * or assist <name>.  Will only ever allow players to
    * * assist a group member  -- Stephen
    */

   CHAR_DATA *assist = NULL;
   CHAR_DATA *ppl;
   char actbuf[MSL];

   if( argument[0] != '\0' )  /* then check for assist <name> */
   {
      if( ( assist = get_char_room( ch, argument ) ) == NULL )
      {
         send_to_char( "They don't seem to be here right now...\n\r", ch );
         return;
      }
      if( assist == ch )
      {
         send_to_char( "You sure need help!\n\r", ch );
         return;
      }
      if( !is_same_group( ch, assist ) )
      {
         act( "Sorry, $N isn't in your group...", ch, NULL, assist, TO_CHAR );
         return;
      }
      if( ( assist->fighting != NULL ) && ( ch->fighting == NULL ) )
      {
         sprintf( actbuf, "$n screams, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
         act( actbuf, ch, NULL, assist, TO_ROOM );
         sprintf( actbuf, "You scream, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
         act( actbuf, ch, NULL, assist, TO_CHAR );
         set_fighting( ch, assist->fighting, TRUE );
         return;
      }
      send_to_char( "Doesn't look like anyone needs your help right now...\n\r", ch );
      return;
   }

   /*
    * Check if any group members in room AND fighting 
    */

   for( ppl = ch->in_room->first_person; ppl != NULL; ppl = ppl->next_in_room )
      if( ( ppl != ch ) && ( is_same_group( ch, ppl ) ) )
         break;

   if( ppl == NULL )
   {
      send_to_char( "Doesn't look like anyone needs your help right now...\n\r", ch );
      return;
   }

   /*
    * Assisting leader is main priority 
    */
   if( ( ch->leader != NULL )
       && ( ch->leader->in_room == ch->in_room )
       && ( ch->leader->fighting != NULL ) && ( ch->fighting == NULL ) && ( ch->leader != ch ) )
   {
      sprintf( actbuf, "$n screams, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
      act( actbuf, ch, NULL, ch->leader, TO_ROOM );
      sprintf( actbuf, "You scream, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
      act( actbuf, ch, NULL, ch->leader, TO_CHAR );
      set_fighting( ch, ch->leader->fighting, TRUE );
      return;
   }

   /*
    * Ok, so no leader to help... lets look for other group members 
    */
   for( ppl = ch->in_room->first_person; ppl != NULL; ppl = ppl->next_in_room )
      if( ( is_same_group( ch, ppl ) ) && ( ppl != ch ) && ( ppl->fighting != NULL ) && ( ch->fighting == NULL ) )
      {
         sprintf( actbuf, "$n screams, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
         act( actbuf, ch, NULL, ppl, TO_ROOM );
         sprintf( actbuf, "You scream, '%s'", IS_NPC( ch ) ? "BANZAI!! $N must be assisted!!" : ch->pcdata->assist_msg );
         act( actbuf, ch, NULL, ppl, TO_CHAR );
         set_fighting( ch, ppl->fighting, TRUE );
         return;
      }


   send_to_char( "Doesn't look like anyone needs your help right now...\n\r", ch );
   return;
}

void check_adrenaline( CHAR_DATA * ch, sh_int damage )
{
   AFFECT_DATA af;

   if( damage > 200 && can_use_skill(ch, gsn_adrenaline) )
   {

      af.type = skill_lookup( "adrenaline bonus" );
      af.duration = 1;
      af.location = APPLY_DAMROLL;
      af.modifier = 1;
      af.bitvector = 0;
      affect_join( ch, &af );
      send_to_char( "You feel a surge of @@eadrenaline@@N!!!\n\r", ch );
   }
   return;
}
