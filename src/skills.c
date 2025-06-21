#include "globals.h"

bool can_hit_skill(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);
bool do_poison(CHAR_DATA *ch, char *argument, int gsn);
bool combo(CHAR_DATA *ch, CHAR_DATA *victim, int gsn);

void do_backstab( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int cnt;
   int mult;
   int dam;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You cannot do that while in this form.\n\r", ch );
      return;
   }

   if( !can_use_skill(ch, "backstab") )
      return;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Backstab whom?\n\r", ch );
      return;
   }

   backstab(ch, get_char_room( ch, arg ), TRUE);
}

void do_circle( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int cnt;
   int mult;
   int chance;
   int dam;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You cannot do that while in this form.\n\r", ch );
      return;
   }

   if( !can_use_skill(ch, "circle") )
      return;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      strcpy(arg, "enemy");
   }

   backstab(ch, get_char_room(ch, arg), FALSE);
}

void backstab(CHAR_DATA *ch, CHAR_DATA *victim, bool backstab)
{
   OBJ_DATA *obj;
   int dam;
   int chance;
   int mult;

   if( victim == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( ( victim == NULL ) || ( victim->is_free != FALSE ) )
      return;

   if( IS_NPC( victim ) && IS_SET( victim->act, ACT_NO_BODY ) )
   {
      act( "$N has no body to backstab!", ch, NULL, victim, TO_CHAR );
      return;
   }

   if (!backstab && !can_hit_skill(victim, ch, gsn_circle))
   {
      act( "$n tries to circle $N, but misses!", ch, NULL, victim, TO_NOTVICT );
      act( "You try to circle $N, but miss!", ch, NULL, victim, TO_CHAR );
      act( "$N tries to circle you, but misses!", victim, NULL, ch, TO_CHAR );
      WAIT_STATE(ch, skill_table[gsn_circle].beats);
      return;
   }

   if( victim == ch )
   {
      send_to_char( "How can you sneak up on yourself?\n\r", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( ( obj = get_eq_char( ch, WEAR_HOLD_HAND_R ) ) == NULL || obj->value[3] != 11 )
      if( ( obj = get_eq_char( ch, WEAR_HOLD_HAND_L ) ) == NULL || obj->value[3] != 11 )
      {
         send_to_char( "You need to wield a piercing weapon.\n\r", ch );
         return;
      }

   if(backstab && victim->fighting != NULL)
   {
      send_to_char( "You can't backstab a fighting person.\n\r", ch );
      return;
   }

   /*
    * Work out damage 
    */
   dam = number_range( obj->value[1], obj->value[2] );
   dam += number_range( get_psuedo_level(ch) / 2, get_psuedo_level(ch) * 2 ) + GET_DAMROLL( ch ) / 2;
   dam += dam * ch->lvl[CLASS_THI] / 100;
   dam += dam * ch->remort[CLASS_ASS] / 100;
   dam += dam * ch->remort[CLASS_WLK] / 100 * .75;
   check_killer( ch, victim );

   if(backstab && IS_NPC(victim) && IS_AFFECTED( victim, AFF_SANCTUARY ) && (ch->remort[CLASS_ASS] > 0 || ch->remort[CLASS_WLK] > 0) && ( number_percent(  ) > 50 ) )
   {
      send_to_char("Critical Success! Your advanced training has succeeded!\n\r", ch);
      act( "The white aura around $n fades.", victim, NULL, NULL, TO_ROOM );
      send_to_char( "The white aura around you fades.\n\r", victim );
      REMOVE_BIT( victim->affected_by, AFF_SANCTUARY );
   }


   char actbuf[MSL];
   sprintf( actbuf, "$n places $p into the back of $N!!");
   act( actbuf, ch, obj, victim, TO_NOTVICT );
   sprintf( actbuf, "You place $p into the back of $N!!");
   act( actbuf, ch, obj, victim, TO_CHAR );
   sprintf( actbuf, "$N places $p into your back!!");
   act( actbuf, victim, obj, ch, TO_CHAR );


   if(!IS_NPC(ch) && ch->lvl[CLASS_THI] > 0 && number_percent(  ) == 1)
   {
      send_to_room("You hear a large CRACK!\n\r", ch->in_room);
      dam *= 2;
   }

   if (backstab)
      swing(ch, victim, dam, gsn_backstab);
   else
      swing(ch, victim, dam*0.8, gsn_circle);

   if (backstab)
      WAIT_STATE(ch, skill_table[gsn_backstab].beats);
   else
      WAIT_STATE(ch, skill_table[gsn_circle].beats);

   if (is_affected( victim, skill_lookup( "poison:quinine" )))
   {
      sprintf( actbuf, "$N screams as the quinine in their veins is consumed!");
      act( actbuf, ch, obj, victim, TO_NOTVICT );
      sprintf( actbuf, "$N screams as the quinine in their veins is consumed!");
      act( actbuf, ch, obj, victim, TO_CHAR );
      sprintf( actbuf, "You scream as the quinine in your veins is consumed!");
      act( actbuf, ch, obj, victim, TO_VICT );
      swing(ch, victim, number_range(dam * 1.1, dam*1.25), gsn_poison_quinine);
      affect_strip( victim, skill_lookup( "poison:quinine" ) );
   }

   if (is_affected( victim, skill_lookup( "poison:arsenic" )))
   {
      sprintf( actbuf, "$N screams as the arsenic in their veins is consumed!");
      act( actbuf, ch, obj, victim, TO_NOTVICT );
      sprintf( actbuf, "$N screams as the arsenic in their veins is consumed!");
      act( actbuf, ch, obj, victim, TO_CHAR );
      sprintf( actbuf, "You scream as the arsenic in your veins is consumed!");
      act( actbuf, ch, obj, victim, TO_VICT );
      swing(ch, victim, number_range(dam * 1.1, dam*1.25), gsn_poison_arsenic);
      affect_strip( victim, skill_lookup( "poison:arsenic" ) );
   }
}

void do_poison_quinine(CHAR_DATA *ch, char *argument)
{
   do_poison(ch, argument, gsn_poison_quinine);
}

void do_poison_arsenic(CHAR_DATA *ch, char *argument)
{
   do_poison(ch, argument, gsn_poison_arsenic);
}

bool do_poison(CHAR_DATA *ch, char *argument, int gsn)
{
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   AFFECT_DATA af;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You cannot do that while in this form.\n\r", ch );
      return FALSE;
   }

   if( !can_use_skill(ch, skill_table[gsn].name) )
      return FALSE;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Poison whom?\n\r", ch );
      return;
   }

   victim = get_char_room(ch, arg);

   if (victim == NULL)
   {
      send_to_char("Poison whom?\n\r", ch);
      return;
   }

   if (is_affected(ch, skill_table[gsn].name) )
   {
      send_to_char("Your victim is already inflicted with that.\n\r", ch);
      return;
   }

   WAIT_STATE(ch, skill_table[gsn].beats);

   check_killer(ch, victim);
   set_fighting(victim, ch);

   if (!can_hit_skill(ch, victim, gsn))
   {
      sprintf(buf, "You attempt to inflict $N with %s, but fail.", skill_table[gsn].name);
      act(buf, ch, NULL, victim, TO_CHAR);
      sprintf(buf, "$n attempts to inflict you with %s, but fails.", skill_table[gsn].name);
      act(buf, ch, NULL, victim, TO_VICT);
      sprintf(buf, "$n attempts to inflict $N with %s, but fails.", skill_table[gsn].name);
      act(buf, ch, NULL, victim, TO_ROOM);
      return FALSE;
   }

   sprintf(buf, "You inflict $N with %s!", skill_table[gsn].name);
   act(buf, ch, NULL, victim, TO_CHAR);
   sprintf(buf, "$n inflictS you with %s!", skill_table[gsn].name);
   act(buf, ch, NULL, victim, TO_VICT);
   sprintf(buf, "$n inflictS $N with %s!", skill_table[gsn].name);
   act(buf, ch, NULL, victim, TO_ROOM);
   af.type = gsn;
   af.duration = 2;
   af.location = APPLY_DOT;
   af.modifier = get_psuedo_level(ch)/2;
   af.bitvector = 0;
   af.caster = ch;
   affect_to_char( victim, &af );
   return TRUE;
}

void do_rescue( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   CHAR_DATA *fch;
   int best;
   int cnt;

   best = -1;

   if( !IS_NPC( ch ) )
   {
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->lvl[cnt] >= skill_table[gsn_rescue].skill_level[cnt] && ch->lvl[cnt] >= best )
            best = cnt;
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know how to rescue!!\n\r", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Rescue whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "What about fleeing instead?\n\r", ch );
      return;
   }

   if( !IS_NPC( ch ) && IS_NPC( victim ) )
   {
      send_to_char( "Doesn't need your help!\n\r", ch );
      return;
   }

   if( ch->fighting == victim )
   {
      send_to_char( "Too late.\n\r", ch );
      return;
   }

   if( ( fch = victim->fighting ) == NULL )
   {
      send_to_char( "That person is not fighting right now.\n\r", ch );
      return;
   }

   WAIT_STATE( ch, skill_table[gsn_rescue].beats );
   if( !IS_NPC( ch ) && number_percent(  ) > ch->pcdata->learned[gsn_rescue] )
   {
      send_to_char( "You fail the rescue.\n\r", ch );
      return;
   }

   act( "You rescue $N!", ch, NULL, victim, TO_CHAR );
   act( "$n rescues you!", ch, NULL, victim, TO_VICT );
   act( "$n rescues $N!", ch, NULL, victim, TO_NOTVICT );

   stop_fighting( fch, FALSE );
   stop_fighting( victim, FALSE );

   set_fighting( ch, fch, TRUE );
   set_fighting( fch, ch, FALSE );
   return;
}

void do_disarm( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;
   int percent;
   int best;
   int cnt;

   best = -1;

   if( !IS_NPC( ch ) && IS_WOLF( ch ) && ( IS_SHIFTED( ch ) || IS_RAGED( ch ) ) )
   {
      send_to_char( "You cannot do that while in this form.\n\r", ch );
      return;
   }


   if( !IS_NPC( ch ) )
   {
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( ch->lvl[cnt] >= skill_table[gsn_disarm].skill_level[cnt] && ch->lvl[cnt] >= best )
            best = cnt;
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know how to disarm!\n\r", ch );
      return;
   }

   if( ( !IS_WEAPON( get_eq_char( ch, WEAR_HOLD_HAND_L ) ) ) && ( !IS_WEAPON( get_eq_char( ch, WEAR_HOLD_HAND_R ) ) ) )
   {
      send_to_char( "You must wield a weapon to disarm.\n\r", ch );
      return;
   }

   if( ( victim = ch->fighting ) == NULL )
   {
      send_to_char( "You aren't fighting anyone.\n\r", ch );
      return;
   }
   if( !IS_NPC( victim ) && IS_WOLF( victim ) && ( IS_SHIFTED( victim ) || IS_RAGED( victim ) ) )
   {
      send_to_char( "Try slicing off their claws instead!\n\r", ch );
      return;
   }

   if( ( ( obj = get_eq_char( victim, WEAR_HOLD_HAND_L ) ) == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      if( ( ( obj = get_eq_char( victim, WEAR_HOLD_HAND_R ) ) == NULL ) || ( obj->item_type != ITEM_WEAPON ) )
      {
         send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
         return;
      }
   if( IS_SET( obj->extra_flags, ITEM_NODISARM ) )
      return;


   WAIT_STATE( ch, skill_table[gsn_disarm].beats );
   percent = number_percent(  ) + victim->level - ch->level;
   if( IS_NPC( ch ) || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3 )
      disarm( ch, victim, obj );
   else
      send_to_char( "You failed.\n\r", ch );
   return;
}

void do_trip( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int best;
/*    int cnt;  */

   best = -1;

   if( !IS_NPC( ch ) )
   {
      /*
       * for ( cnt = 0; cnt < MAX_CLASS; cnt++ )
       * if ( ch->lvl[cnt] >= skill_table[gsn_trip].skill_level[cnt] 
       * && ch->lvl[cnt] >= best )
       * best = cnt;  
       */
      if( ch->pcdata->learned[gsn_trip] > 75 )
         best = UMAX( 79, get_psuedo_level( ch ) );
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know of such a skill.\n\r", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Trip whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "That wouldn't even be funny.\n\r", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( victim->fighting == NULL )
   {
      send_to_char( "You can't trip someone who isn't fighting.\n\r", ch );
      return;
   }

   check_killer( ch, victim );

   if( number_percent(  ) < ( IS_NPC( ch ) ? 50 : 2 * ch->pcdata->learned[gsn_trip] ) )
   {
      check_killer( ch, victim );
      trip( ch, victim );
      WAIT_STATE( ch, skill_table[gsn_trip].beats );

   }

   return;
}

void do_dirt( CHAR_DATA * ch, char *argument )
{
   AFFECT_DATA af;
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int best;
   /*
    * int cnt;   
    */

   best = -1;

   if( !IS_NPC( ch ) )
   {
      /*
       * for ( cnt = 0; cnt < MAX_CLASS; cnt++ )
       * if ( ch->lvl[cnt] >= skill_table[gsn_dirt].skill_level[cnt] 
       * && ch->lvl[cnt] >= best )
       * best = cnt;  
       */
      if( ch->pcdata->learned[gsn_dirt] > 75 )
         best = UMAX( 79, get_psuedo_level( ch ) );
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know of such a skill.\n\r", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' && ch->fighting == NULL )
   {
      send_to_char( "Kick dirt at whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL && ch->fighting == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( victim == NULL )
      victim = ch->fighting;

   if( victim == ch )
   {
      send_to_char( "That wouldn't be too smart, would it??.\n\r", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( victim->fighting == NULL )
   {
      send_to_char( "Try doing this to them, when fighting...\n\r", ch );
      return;
   }

   if( IS_AFFECTED( victim, AFF_BLIND ) )
      return;

   WAIT_STATE( ch, skill_table[gsn_dirt].beats );

   if( number_percent(  ) > ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_dirt] ) )
   {
      act( "You kick dirt at $M but miss!", ch, NULL, victim, TO_CHAR );
      act( "$n kicks dirt at you but misses!", ch, NULL, victim, TO_VICT );
      act( "$n kicks dirt at $N but misses!", ch, NULL, victim, TO_NOTVICT );
      return;
   }
   else
   {
      act( "You kick dirt at $S eyes!", ch, NULL, victim, TO_CHAR );
      act( "$n kicks dirt in your eyes!", ch, NULL, victim, TO_VICT );
      act( "$n kicks dirt at $N's eyes!", ch, NULL, victim, TO_NOTVICT );

      af.type = skill_lookup( "blindness" );
      af.location = APPLY_HITROLL;
      af.modifier = -2;
      af.duration = 1;
      af.bitvector = AFF_BLIND;
      affect_to_char( victim, &af );
   }
   return;
}

void do_bash( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int best;
/*    int cnt;  */

   best = -1;

   if( !IS_NPC( ch ) )
   {
      /*
       * for ( cnt = 0; cnt < MAX_CLASS; cnt++ )
       * if ( ch->lvl[cnt] >= skill_table[gsn_dirt].skill_level[cnt] 
       * && ch->lvl[cnt] >= best )
       * best = cnt;  
       */
      if( ch->pcdata->learned[gsn_bash] > 75 )
         best = UMAX( 79, get_psuedo_level( ch ) );
   }
   else
      best = ch->level;

   if( best == -1 )
   {
      send_to_char( "You don't know of such a skill.\n\r", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Bash whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Forget it!\n\r", ch );
      return;
   }

   if( is_safe( ch, victim ) )
      return;

   if( victim->fighting == NULL )
   {
      send_to_char( "It might help if you were fighting.....\n\r", ch );
      return;
   }

   WAIT_STATE( ch, skill_table[gsn_bash].beats );

   if( ( IS_NPC( ch ) && ( number_percent(  ) > 75 + ( ch->level ) / 2 ) )
       || ( !IS_NPC( ch ) && ( 2 * number_percent(  ) > ch->pcdata->learned[gsn_bash] ) ) )
   {
      act( "Your bash misses $M, and you fall!", ch, NULL, victim, TO_CHAR );
      act( "$n trys to bash you, misses, and falls!", ch, NULL, victim, TO_VICT );
      act( "$n trys to bash $N but misses, and falls!", ch, NULL, victim, TO_NOTVICT );
      return;
   }
   else
   {
      act( "Your bash sends $M flying!", ch, NULL, victim, TO_CHAR );
      act( "$n bashes you, sending you flying!", ch, NULL, victim, TO_VICT );
      act( "$n's bash sends $N's flying!", ch, NULL, victim, TO_NOTVICT );

      /*
       * If victim very weak, set pos to stunned, stop fighting 
       */
      if( victim->hit < ( victim->max_hit / 50 ) + 1 )
      {
         act( "$N stays on the floor.", ch, NULL, victim, TO_CHAR );
         act( "You are unable to get up.", ch, NULL, victim, TO_VICT );
         act( "$N stays on the floor.", ch, NULL, victim, TO_NOTVICT );
         stop_fighting( victim, TRUE );   /* MAG: might del this? -S- */
         victim->position = POS_RESTING;
         update_pos( victim );
      }
      else
      {
         /*
          * Do some damage instead... 
          */
         damage( ch, victim, ( best + 12 ) * 2, -1 );
      }

   }

   return;
}

void do_beserk( CHAR_DATA * ch, char *argument )
{
   AFFECT_DATA af;
   int best;
   int level;

   bool prime;

   prime = FALSE;
   best = -1;

   if( IS_NPC( ch ) )
      return;

   if( ch->pcdata->order[0] == 2 )
      prime = TRUE;

   level = ch->lvl[2];

   if( ch->fighting == NULL )
   {
      send_to_char( "You can only go beserk when fighting!\n\r", ch );
      return;
   }

   if( is_affected( ch, gsn_beserk ) )
   {
      send_to_char( "You are already beserk!!\n\r", ch );
      return;
   }

   if( ch->pcdata->learned[gsn_beserk] == 0 )
   {
      send_to_char( "You don't know how to use this skill!\n\r", ch );
      return;
   }

   WAIT_STATE( ch, skill_table[gsn_beserk].beats );

   if( number_percent(  ) < ( ch->pcdata->learned[gsn_beserk] / 2 ) )
   {
      /*
       * Failure 
       */
      act( "$n calls on the Dark Powers, but they don't answer!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You call on the Dark Powers.  They don't answer!", ch );
      return;
   }

   af.type = gsn_beserk;
   af.duration = -1;
   af.location = APPLY_AC;
   af.modifier = ( best == 2 ) ? level : ( level * 2 );
   af.bitvector = 0;
   affect_to_char( ch, &af );

   af.location = APPLY_HITROLL;
   af.modifier = ( prime )
      ? ( 10 + ( level >= 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 80 ) )
      : ( 5 + ( level >= 25 ) + ( level >= 50 ) );
   affect_to_char( ch, &af );

   af.location = APPLY_DAMROLL;
   af.modifier = ( prime )
      ? ( 12 + ( level >= 20 ) + ( level >= 40 ) + ( level >= 60 ) + ( level >= 80 ) )
      : ( 2 + ( level >= 25 ) + ( level >= 50 ) );
   affect_to_char( ch, &af );


   af.location = APPLY_SAVING_SPELL;
   af.modifier = ( prime )
      ? ( 10 - ( level >= 20 ) - ( level >= 40 ) - ( level >= 60 ) - ( level >= 80 ) )
      : ( 20 - ( level >= 25 ) - ( level >= 50 ) );
   affect_to_char( ch, &af );

   act( "$n calls on the Dark Powers, who answer!!!", ch, NULL, NULL, TO_ROOM );
   send_to_char( "You call on the Dark Powers.  They answer!!!\n\r", ch );
   return;
}

void do_punch( CHAR_DATA * ch, char *argument )
{
   war_attack(ch, argument, gsn_punch);
}

void war_attack( CHAR_DATA * ch, char *argument, int gsn )
{
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   char actbuf[MAX_STRING_LENGTH];
   int dam;
   int chance;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      strcpy(arg, "enemy");
   }

   if( !can_use_skill(ch, skill_table[gsn].name) )
      return;

   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\n\r", ch );
      return;
   }

   if( victim == NULL )
      victim = ch->fighting;

   dam = number_range(get_psuedo_level(ch) * get_curr_str(ch) / 10, get_psuedo_level(ch) * get_curr_str(ch) / 5)

   dam += get_damroll(ch)/2;

   dam += dam * ch->lvl[CLASS_WAR] / 100;

   if (ch->remort[CLASS_KNI] > 0)
      dam += dam * ch->remort[CLASS_KNI] / 50;
   else if (ch->remort[CLASS_SWO] > 0)
      dam += dam * ch->remort[CLASS_SWO] / 50;
   else if (ch->remort[CLASS_MON] > 0)
      dam += dam * ch->remort[CLASS_MON] / 50;
   else if (ch->remort[CLASS_PAL] > 0)
      dam += dam * ch->remort[CLASS_PAL] / 50 * .75;

   WAIT_STATE( ch, skill_table[gsn].beats );

   check_killer( ch, victim );
   if( can_hit_skill(ch, victim, gsn ) )
   {
      char actbuf[MSL];
      sprintf( actbuf, "$n %ss $N!!", skill_table[gsn].name, dam );
      act( actbuf, ch, NULL, victim, TO_NOTVICT );
      sprintf( actbuf, "$N %ss you really hard!!", skill_table[gsn].name, dam );
      act( actbuf, ch, NULL, victim, TO_VICT );
      sprintf( actbuf, "You %s $N!!", skill_table[gsn].name, dam );
      act( actbuf, ch, NULL, victim, TO_CHAR );

      swing(ch, victim, dam, gsn);
      combo(ch, victim, gsn);
   }
   else
   {
      sprintf( actbuf, "$n tries to %s $N, but misses!", skill_table[gsn].name );
      act( actbuf, ch, NULL, victim, TO_NOTVICT );
      sprintf( actbuf, "$N tries to %s you, but misses!", skill_table[gsn].name );
      act( actbuf, ch, victim, NULL, TO_VICT );
      sprintf( actbuf, "You try to %s $N, but miss!", skill_table[gsn].name );
      act( actbuf, ch, NULL, victim, TO_CHAR );
   }
}

void do_headbutt( CHAR_DATA * ch, char *argument )
{
   war_attack(ch, argument, gsn_headbutt);
}

void do_charge( CHAR_DATA * ch, char *argument )
{

   CHAR_DATA *victim;
   int dam;
   bool prime;
   int chance;

   prime = FALSE;


   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_charge] == 0 )
   {
      send_to_char( "You are not trained in this skill!\n\r", ch );
      return;
   }

   if( ( IS_NPC( ch ) ) && ( get_psuedo_level( ch ) < 80 ) )
      return;

   if( ( ( victim = get_char_room( ch, argument ) ) == NULL ) && ch->fighting == NULL )
   {
      send_to_char( "No such victim!\n\r", ch );
      return;
   }


   if( victim == NULL )
      victim = ch->fighting;

   if( victim == ch )
   {
      send_to_char( "You can't reach!\n\r", ch );
      return;
   }

   if( !IS_NPC( ch ) && ch->pcdata->order[0] == 3 )
      prime = TRUE;


   dam = number_range( get_psuedo_level( ch ), get_psuedo_level( ch ) * 3 );

   if( !IS_NPC( ch ) )
      chance = ch->pcdata->learned[gsn_charge] / 2;
   else
      chance = 50;

   chance += ( ( get_psuedo_level( ch ) - ( get_psuedo_level( victim ) - 30 ) ) / 2 );


   WAIT_STATE( ch, skill_table[gsn_charge].beats );

   check_killer( ch, victim );

   if( number_percent(  ) < chance )
   {
      /*
       * HIT 
       */

         /*
          * HIT! 
          */
         char actbuf[MSL];
         sprintf( actbuf, "@@a$n @@acharges $N@@a, and knocks them over!@@N @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_NOTVICT );
         sprintf( actbuf, "@@a$N @@acharges right into you!@@N @@l(@@W%d@@l)@@N", dam );
         act( actbuf, victim, NULL, ch, TO_CHAR );
         sprintf( actbuf, "@@aYou charge right into $N@@a, and knock him over!@@N @@l(@@W%d@@l)@@N", dam );
         act( actbuf, ch, NULL, victim, TO_CHAR );

      damage( ch, victim, dam, -1 );
   }
   else
   {
      /*
       * MISS 
       */
      act( "$n@@b charges $N@@b, but runs right past!@@b", ch, NULL, victim, TO_NOTVICT );
      act( "$N @@bcharges you, but runs right past!@@N", victim, NULL, ch, TO_CHAR );
      act( "@@bYou try to charge $N@@b, but run past him. DOH!!@@N", ch, NULL, victim, TO_CHAR );
      damage( ch, victim, 0, -1 );
   }
   return;
}

void do_kick( CHAR_DATA * ch, char *argument )
{
   war_attack(ch, argument, gsn_kick);
}

void do_warcry( CHAR_DATA * ch, char *argument )
/* --Stephen 
 * FIXME: make this a warrior skill, check for bless.
 */
{
   AFFECT_DATA af;
   int best;
   int level;

   best = -1;

   if( IS_NPC( ch ) )
      return;

   level = ch->lvl[CLASS_WAR];

   if( ch->fighting == NULL )
   {
      send_to_char( "You can only give a warcry when fighting!\n\r", ch );
      return;
   }

   if( is_affected( ch, gsn_warcry ) )
   {
      send_to_char( "You already have given a warcry!!\n\r", ch );
      return;
   }

   if( ch->pcdata->learned[gsn_warcry] == 0 )
   {
      send_to_char( "You don't know how to use this skill!\n\r", ch );
      return;
   }

   WAIT_STATE( ch, skill_table[gsn_warcry].beats );

   af.type = gsn_warcry;
   af.duration = -1;
   af.location = APPLY_HITROLL;
   af.modifier = level / 16;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   af.location = APPLY_SAVING_SPELL;
   af.modifier = 0 - level / 16;
   affect_to_char( ch, &af );

   send_to_char( "You scream a great warcry!\n\r", ch );
   act( "$n screams a great warcry!", ch, NULL, NULL, TO_ROOM );
   return TRUE;
}

void do_knee( CHAR_DATA * ch, char *argument )
{
   war_attack(ch, argument, gsn_knee);
}


void do_detox( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   int dam;
   int chance;

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

void do_morale( CHAR_DATA * ch, char *argument )
{
   AFFECT_DATA af;
   CHAR_DATA *gch;

   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_morale] == 0 )
   {
      send_to_char( "You are not trained in this skill!\n\r", ch );
      return;
   }

   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      if( is_affected( gch, gsn_morale ) || !is_same_group( ch, gch ) )
         continue;

      act( "$n seems much more willing to fight.", gch, NULL, NULL, TO_ROOM );
      send_to_char( "You are inspired to fight better!\n\r", gch );
      af.type = gsn_morale;
      if (ch == gch)
         af.duration = -1;
      else
         af.duration = 4 + ( get_psuedo_level(ch) / 5 );
      af.location = APPLY_DAMROLL;
      af.modifier = get_psuedo_level( ch ) / 10;
      af.bitvector = 0;
      affect_to_char( gch, &af );
   }
   send_to_char( "You inspire the troops!\n\r", ch );

   return TRUE;
}

void do_leadership( CHAR_DATA * ch, char *argument )
{
   AFFECT_DATA af;
   CHAR_DATA *gch;

   if( !IS_NPC( ch ) && ch->pcdata->learned[gsn_leadership] == 0 )
   {
      send_to_char( "You are not trained in this skill!\n\r", ch );
      return;
   }

   for( gch = ch->in_room->first_person; gch != NULL; gch = gch->next_in_room )
   {
      if( is_affected( gch, gsn_leadership ) || !is_same_group( ch, gch ) )
         continue;

      act( "$n looks more courageous!", gch, NULL, NULL, TO_ROOM );
      send_to_char( "You feel courage wash over you!\n\r", gch );
      af.type = gsn_leadership;
      if (ch == gch)
         af.duration = -1;
      else
         af.duration = 4 + ( get_psuedo_level(ch) / 5 );
      af.location = APPLY_HITROLL;
      af.modifier = get_psuedo_level( ch ) / 10;
      af.bitvector = 0;
      affect_to_char( gch, &af );
   }
   send_to_char( "You inspire the troops!\n\r", ch );

   return TRUE;
}

bool can_hit_skill(CHAR_DATA *ch, CHAR_DATA *victim, int gsn)
{
   int chance = 70;

   if (gsn == gsn_poison_quinine || gsn == gsn_poison_arsenic)
   {
      // Can never hit immune poison with poison
      if (IS_SET(race_table[victim->race].race_flags, RACE_MOD_IMMUNE_POISON))
         return FALSE;
   }

   if( !IS_AWAKE(victim) )
      chance += 75;

   if( IS_AFFECTED(ch, AFF_SNEAK) || item_has_apply( ch, ITEM_APPLY_SNEAK ) )
      chance += 10;
      
   if (!can_see(victim,ch))
      chance += 20;

   chance += get_psuedo_level( ch ) - get_psuedo_level( victim );

   if (chance < number_percent())
      return FALSE;

   return TRUE;
}

bool combo(CHAR_DATA *ch, CHAR_DATA *victim, int gsn)
{
   char buf[MAX_STRING_LENGTH];
   int i;

   for(i = 1; i < MAX_COMBO; i++)
   {
      /* Reverse index */
      ch->combo[MAX_COMBO-i] = ch->combo[MAX_COMBO-i-1];
   }

   ch->combo[0] = gsn;

   if (ch->combo[0] == gsn_knee && ch->combo[1] == gsn_headbutt && ch->combo[2] == gsn_punch && ch->combo[3] == gsn_kick)
   {
     send_to_char("@@yCombo triggered!@@N\n\r",ch);
     act("You begin a combo attack!", ch, NULL, victim, TO_CHAR);
     act("$n begins a combo attack!", ch, NULL, victim, TO_ROOM);
     do_knee(ch, victim->name);
     do_kick(ch, victim->name);

     for(int i = 0; i < MAX_COMBO; i++)
        ch->combo[i] = -1;
   }
}
