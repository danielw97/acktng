#include "globals.h"

void do_knee( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   int dam;
   bool prime;
   int chance;

   prime = FALSE;

   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_knee] == 0 )
   {
      send_to_char( "You are not trained in this skill!\n\r", ch );
      return;
   }

   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\n\r", ch );
      return;
   }

   if( victim == NULL )
      victim = ch->fighting;

   if( IS_NPC( victim ) && IS_SET( victim->act, ACT_NO_BODY ) )
   {
      act( "$N doesn't have a definable body to knee!", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->order[0] == 3 )
      prime = TRUE;

   if( IS_NPC( ch ) )
      dam = number_range( ch->level / 3, ch->level / 2 );
   else
      dam = number_range( ch->lvl[3] / 2, ch->lvl[3] * ( prime ? 2 : 1 ) );

   chance = ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_knee] / 2 );

   chance += ( ch->lvl[3] - victim->level );


   WAIT_STATE( ch, skill_table[gsn_knee].beats );

   check_killer( ch, victim );

   if( number_percent(  ) < chance )
   {
      /*
       * HIT
       */
         /*
          * HIT!
          */

         dam = damage(ch, victim, (dam * 3) / 5, -1 );
         char actbuf[MSL];
         sprintf( actbuf, "$n grabs $N and knees $M in the groin! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_NOTVICT );
         sprintf( actbuf, "$N grabs you, and knees you in the groin! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, victim, NULL, ch, TO_CHAR );
         sprintf( actbuf, "You grab $M and knee $M in the groin! @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_CHAR );
   }
   else
   {
      /*
       * MISS
       */
      act( "$n tries to grab $N, but $E twists free!", ch, NULL, victim, TO_NOTVICT );
      act( "$N tries to grab you, but you twist free!", victim, NULL, ch, TO_CHAR );
      act( "You try to grab $N, but $E twists free!", ch, NULL, victim, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   return;
}


void do_detox(CHAR_DATA * ch, char *argument )
{

   CHAR_DATA *victim;
   int dam;
   bool prime;
   int chance;

   prime = FALSE;


   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_detox] == 0 )
   {
      send_to_char( "You are not trained in this skill!\n\r", ch );
      return;
   }



   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\n\r", ch );
      return;
   }

   if( IS_NPC( victim ) && IS_SET( victim->act, ACT_NO_BODY ) )
   {
      act( "$N doesn't have a definable body to detox!", ch, NULL, victim, TO_CHAR );
      return;
   }

   if( is_affected( victim, gsn_poison ) )
   {
      affect_strip( victim, gsn_poison );
      act( "$n detoxes $N, and $N looks better.", ch, NULL, victim, TO_NOTVICT );
      act( "$n detoxes you, and you feel better.", ch, NULL, victim, TO_VICT );
      act( "You detox $N, and $N looks noticably better.", ch, NULL, victim, TO_CHAR);
   }
   else
      act( "$N is not poisoned!", ch, NULL, victim, TO_CHAR );
   return TRUE;
}

