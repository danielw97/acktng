#include "globals.h"

bool check_skills( CHAR_DATA *ch, CHAR_DATA *victim );

void ai_update()
{
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;
   for( ch = first_char; ch; ch = ch_next )
   {
      ch_next = ch->next;

      if (IS_NPC(ch) && ch->fighting != NULL)
      {
         check_skills(ch, ch->fighting);
      }
   }

}

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
#define MOB_NONE                        BIT_1
#define MOB_SECOND                      BIT_2
#define MOB_THIRD                       BIT_3
#define MOB_FOURTH                      BIT_4
#define MOB_FIFTH                       BIT_5
#define MOB_SIXTH                       BIT_6
#define MOB_NODISARM                    BIT_7
#define MOB_NOTRIP                      BIT_8
#define MOB_PUNCH                       BIT_9
#define MOB_HEADBUTT                    BIT_10
#define MOB_KNEE                        BIT_11
#define MOB_DISARM                      BIT_12
#define MOB_TRIP                        BIT_13
#define MOB_NODISARM                    BIT_14
#define MOB_NOTRIP                      BIT_15
#define MOB_DODGE                       BIT_16
#define MOB_PARRY                       BIT_17
#define MOB_MARTIAL                     BIT_18
#define MOB_ENHANCED                    BIT_19
#define MOB_DUALWIELD                   BIT_20
#define MOB_DIRT                        BIT_21
#define MOB_FIFTH                       BIT_22
#define MOB_SIXTH                       BIT_23
#define MOB_CHARGE                      BIT_24
#define MOB_COUNTER                     BIT_25
#define MOB_KICK                        BIT_26
*/

/*
#define CAST_NONE                       BIT_1
#define CAST_MAGIC_MISSILE              BIT_2
#define CAST_SHOCKING_GRASP             BIT_3
#define CAST_BURNING_HANDS              BIT_4
#define CAST_COLOR_SPRAY                BIT_5
#define CAST_FIREBALL                   BIT_6
#define CAST_HELLSPAWN                  BIT_7
#define CAST_ACID_BLAST                 BIT_8
#define CAST_CHAIN_LIGHTNING            BIT_9
#define CAST_FLARE                      BIT_10
#define CAST_FLAMESTRIKE                BIT_11
#define CAST_EARTHQUAKE                 BIT_12
#define CAST_MIND_FLAIL                 BIT_13
#define CAST_PLANERGY                   BIT_14
#define CAST_PHOBIA                     BIT_15
#define CAST_MIND_BOLT                  BIT_16
#define CAST_STATIC                     BIT_17
#define CAST_EGO_WHIP                   BIT_18
#define CAST_BLOODY_TEARS               BIT_19
#define CAST_MINDFLAME                  BIT_20
#define CAST_SUFFOCATE                  BIT_21
#define CAST_NERVE_FIRE                 BIT_22
#define CAST_LIGHTNING_BOLT             BIT_23
#define CAST_HEAT_ARMOR                 BIT_24
#define CAST_LAVA_BURST                 BIT_25
*/

bool reset_skills(CHAR_DATA *ch)
{
   if (!IS_NPC(ch))
      return FALSE;

   return TRUE;
}

