#include "globals.h"

char *get_wear_name( OBJ_DATA *obj );
void set_obj_stat_auto( OBJ_DATA *obj );
void set_aff_to_obj(OBJ_DATA *obj, int location, int modifier);

OBJ_DATA *generate_item( int level )
{
    char buf[MSL];
    OBJ_DATA *obj = create_object( get_obj_index(OBJ_VNUM_MUSHROOM), level );
    return obj;
    obj->level = level;
    if (obj->level > 150)
       obj->level = 150;
    if (obj->level < 1)
       obj->level = 1;
    obj->weight = number_range(1,25);
    SET_BIT(obj->extra_flags, ITEM_GENERATED);
    SET_BIT(obj->extra_flags, ITEM_BIND_EQUIP);

    switch(number_range(1,23))
    {
        case 1:
           SET_BIT(obj->wear_flags, ITEM_WEAR_HALO);
           obj->item_type = ITEM_ARMOR;
        break;
        case 2:
           SET_BIT(obj->wear_flags, ITEM_WEAR_AURA);
           obj->item_type = ITEM_ARMOR;
        break;
        case 3:
           SET_BIT(obj->wear_flags, ITEM_WEAR_HORNS);
           obj->item_type = ITEM_ARMOR;
        break;
        case 4:
           SET_BIT(obj->wear_flags, ITEM_WEAR_HEAD);
           obj->item_type = ITEM_ARMOR;
        break;
        case 5:
           SET_BIT(obj->wear_flags, ITEM_WEAR_FACE);
           obj->item_type = ITEM_ARMOR;
        break;
        case 6:
           SET_BIT(obj->wear_flags, ITEM_WEAR_BEAK);
           obj->item_type = ITEM_ARMOR;
        break;
        case 7:
           SET_BIT(obj->wear_flags, ITEM_WEAR_EAR);
           obj->item_type = ITEM_ARMOR;
        break;
        case 8:
           SET_BIT(obj->wear_flags, ITEM_WEAR_NECK);
           obj->item_type = ITEM_ARMOR;
        break;
        case 9:
           SET_BIT(obj->wear_flags, ITEM_WEAR_WINGS);
           obj->item_type = ITEM_ARMOR;
        break;
        case 10:
           SET_BIT(obj->wear_flags, ITEM_WEAR_SHOULDERS);
           obj->item_type = ITEM_ARMOR;
        break;
        case 11:
           SET_BIT(obj->wear_flags, ITEM_WEAR_ARMS);
           obj->item_type = ITEM_ARMOR;
        break;
        case 12:
           SET_BIT(obj->wear_flags, ITEM_WEAR_WRIST);
           obj->item_type = ITEM_ARMOR;
        break;
        case 13:
           SET_BIT(obj->wear_flags, ITEM_WEAR_HANDS);
           obj->item_type = ITEM_ARMOR;
        break;
        case 14:
           SET_BIT(obj->wear_flags, ITEM_WEAR_FINGER);
           obj->item_type = ITEM_ARMOR;
        break;
        case 15:
           SET_BIT(obj->wear_flags, ITEM_WEAR_CLAWS);
           obj->item_type = ITEM_ARMOR;
        break;
        case 17:
           SET_BIT(obj->wear_flags, ITEM_WEAR_ABOUT);
           obj->item_type = ITEM_ARMOR;
        break;
        case 18:
           SET_BIT(obj->wear_flags, ITEM_WEAR_WAIST);
           obj->item_type = ITEM_ARMOR;
        break;
        case 19:
           SET_BIT(obj->wear_flags, ITEM_WEAR_BODY);
           obj->item_type = ITEM_ARMOR;
        break;
        case 20:
           SET_BIT(obj->wear_flags, ITEM_WEAR_TAIL);
           obj->item_type = ITEM_ARMOR;
        break;
        case 21:
           SET_BIT(obj->wear_flags, ITEM_WEAR_LEGS);
           obj->item_type = ITEM_ARMOR;
        break;
        case 22:
           SET_BIT(obj->wear_flags, ITEM_WEAR_FEET);
           obj->item_type = ITEM_ARMOR;
        break;
        case 23:
           SET_BIT(obj->wear_flags, ITEM_WEAR_HOOVES);
           obj->item_type = ITEM_ARMOR;
        break;
        case 16:
        default:
           SET_BIT(obj->wear_flags, ITEM_WEAR_HOLD_HAND);
           obj->item_type = ITEM_ARMOR;
        break;
    }

    
    if (obj->weight < 10)
        sprintf(buf, "Caster Caster's %s", get_wear_name(obj));
    else if (obj->weight < 20)
        sprintf(buf, "Melee Melee's %s", get_wear_name(obj));
    else
        sprintf(buf, "Tank Tank's %s", get_wear_name(obj));
    obj->name = str_dup( buf );
    
    if (obj->weight < 10)
        sprintf(buf, "Caster's %s", get_wear_name(obj));
    else if (obj->weight < 20)
        sprintf(buf, "Melee's %s", get_wear_name(obj));
    else
        sprintf(buf, "Tank's %s", get_wear_name(obj));
    obj->short_descr = str_dup( buf );
    
    if (obj->weight < 10)
        sprintf(buf, "A Caster's %s lies here", get_wear_name(obj));
    else if (obj->weight < 20)
        sprintf(buf, "A Melee's %s lies here", get_wear_name(obj));
    else
        sprintf(buf, "A Tank's %s lies here", get_wear_name(obj));
    obj->description = str_dup( buf );

    set_obj_stat_auto(obj);

    return obj;
}

char *get_wear_name( OBJ_DATA *obj )
{
    if (IS_SET(obj->wear_flags, ITEM_WEAR_HALO))
       return "Halo";
    
    if (IS_SET(obj->wear_flags, ITEM_WEAR_AURA))
       return "Aura";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_HORNS))
       return "Horns";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_HEAD))
       return "Helm";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_BEAK))
       return "Beak";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_FACE))
       return "Faceguard";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_EAR))
       return "Earring";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_NECK))
       return "Necklace";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_WINGS))
       return "Wings";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_SHOULDERS))
       return "Pauldrons";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_ARMS))
       return "Armguards";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_WRIST))
       return "Bracers";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_HANDS))
       return "Gauntlets";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_FINGER))
       return "Ring";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_HANDS))
       return "Claws";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_FINGER))
       return "Cape";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_WAIST))
       return "Belt";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_BODY))
       return "Hauberk";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_TAIL))
       return "Tailguard";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_LEGS))
       return "Legguards";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_FEET))
       return "Boots";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_HOOVES))
       return "Hooves";

    if (IS_SET(obj->wear_flags, ITEM_WEAR_HOLD_HAND))
       return "Shield";

    return "null";
}

void set_obj_stat_auto( OBJ_DATA *obj )
{
   int ilevel = obj->level;
   int hr_div = 0;
   int dr_div = 0;
   int ac_div = 0;
   int hp_div = 0;
   int move_div = 0;
   int mana_div = 0;
   int spellpower_div = 0;
   int hrdr_bonus = 0;
   int ac_bonus = 0;
   int stat_bonus = 0;

   if (IS_SET(obj->extra_flags, ITEM_GENERATED))
   {
    ilevel *= 0.8;
   }
   if (IS_SET(obj->extra_flags, ITEM_MAGIC))
   {
      ilevel *= 1.2;
   }
   else if (IS_SET(obj->extra_flags, ITEM_RARE))
   {
      ilevel *= 1.5;
   }
   else if (IS_SET(obj->extra_flags, ITEM_MYTHIC))
   {
      ilevel *= 1.75;
   }
   else if (IS_SET(obj->extra_flags, ITEM_LEGENDARY))
   {
      ilevel *= 2;
   }

   if (obj->level > MAX_MORTAL)
   {
      ilevel += (obj->level - MAX_MORTAL)*3;
   }

   if (obj->level > 124)
   {
      ilevel += (obj->level - 124)*4;
   }

   if (obj->level == 150)
   {
      ilevel += 50;
   }

   /* Small bonus for higher weights within itemization class */
   ilevel += obj->level * (obj->weight%10) * 2 / 100;

   /* Jewelry */
   if (IS_SET(obj->wear_flags, ITEM_WEAR_HALO) || IS_SET(obj->wear_flags, ITEM_WEAR_AURA) ||
       IS_SET(obj->wear_flags, ITEM_WEAR_NECK) || IS_SET(obj->wear_flags, ITEM_WEAR_FINGER) ||
       IS_SET(obj->wear_flags, ITEM_WEAR_HALO) || IS_SET(obj->wear_flags, ITEM_WEAR_AURA) )
   {
      hr_div = 10;
      dr_div = 10;
      ac_div = 20;
      hp_div = 2;
      mana_div = 2;
      move_div = 2;
      ac_bonus = -5;
      hrdr_bonus = 1;
      stat_bonus = 5;

      if (obj->weight > 19)
      {
         ac_div -= 5;
         hp_div /= 2;
         mana_div *= 2;
         move_div *= 2;
      }
      else if (obj->weight > 9)
      {
         /* Do nothing for now */
      }
      else
      {
         /* Caster */
         dr_div *= 2;
         move_div *= 2;

         spellpower_div = 15;
      }
   }
   else if (obj->item_type == ITEM_WEAPON)
   {
      hrdr_bonus = 5;
      ac_bonus = -5;
      stat_bonus = 5;
      hr_div = 6;
      dr_div = 6;
      ac_div = 20;
      hp_div = 10;
      move_div = 10;
      mana_div = 10;

      if (obj->weight > 19)
      {
         ac_div -= 5;
         hp_div /= 2;
         mana_div *= 2;
         move_div *= 2;
      }
      else if (obj->weight > 9)
      {
         /* Do nothing for now */
      }
      else
      {
         /* Caster */
         dr_div *= 2;
         move_div *= 2;

         spellpower_div = 8;
      }
      if (IS_SET(obj->extra_flags, ITEM_EXTRA_WAND))
      {
         // This is all in for caster stats
         spellpower_div = 2;
         hrdr_bonus = 0;
         hr_div = 0;
         dr_div = 0;
         move_div = 0;
      }
   }
   else if (obj->item_type == ITEM_ARMOR)
   {
      ac_bonus = -10;
      hrdr_bonus = 1;
      stat_bonus = 5;
      hr_div = 8;
      dr_div = 8;
      ac_div = 2;
      hp_div = 5;
      mana_div = 5;
      move_div = 5;
      
      if (obj->weight > 19)
      {
         ac_div -= 1;
         hp_div /= 2;
         mana_div *= 2;
         move_div *= 2;
      }
      else if (obj->weight > 9)
      {
         /* Do nothing for now */
      }
      else
      {
         /* Caster */
         dr_div *= 2;
         move_div *= 2;

         spellpower_div = 15;
      }

      // Bucklers get poor stats
      if (IS_SET(obj->extra_flags, ITEM_BUCKLER))
      {
         ac_bonus = 0;
         hr_div *= 2;
         dr_div *= 2;
         ac_div *= 2;
         hp_div *= 2;
         mana_div *= 2;
         move_div *= 2;
         spellpower_div *= 2;
      }
   }
   else
   {
      /* Well, shit stats since it doesn't follow the standards */
      hr_div = 15;
      dr_div = 15;
      ac_div = 20;
      hp_div = 10;
      mana_div = 10;
      move_div = 10;
      ac_bonus = -5;
      stat_bonus = 5;
      hrdr_bonus = 1;

      if (obj->weight > 19)
      {
         ac_div -= 5;
         hp_div /= 2;
         mana_div *= 2;
         move_div *= 2;
      }
      else if (obj->weight > 9)
      {
         /* Do nothing for now */
      }
      else
      {
         /* Caster */
         dr_div *= 2;
         move_div *= 2;

         spellpower_div = 15;
      }
   }

   int ac_val = ac_bonus;
   if (ac_div != 0)
      ac_val -= (ilevel*2 / ac_div);
   int hr_val = hrdr_bonus;
   if (hr_div != 0)
      hr_val += (ilevel / hr_div);
   int dr_val  = hrdr_bonus;
   if (dr_div != 0)
      dr_val += (ilevel / dr_div);
   int hp_val = stat_bonus;
   if (hp_div != 0)
      hp_val += (ilevel / hp_div) + stat_bonus;
   int mana_val = stat_bonus;
   if (mana_div != 0)
      mana_val += (ilevel / mana_div);
   int move_val = stat_bonus;
   if (move_div != 0)
      move_val += (ilevel / move_div);
   int spellpower_val = 0;
   if (spellpower_div > 0)
      spellpower_val = (ilevel / spellpower_div);

   if (ac_val != 0)
      set_aff_to_obj( obj, APPLY_AC, ac_val );
   if (hr_val != 0)
      set_aff_to_obj( obj, APPLY_HITROLL, hr_val );
   if (dr_val != 0)
      set_aff_to_obj( obj, APPLY_DAMROLL, dr_val );
   if (hp_val != 0)
      set_aff_to_obj( obj, APPLY_HIT, hp_val );
   if (mana_val != 0)
      set_aff_to_obj( obj, APPLY_MANA, mana_val );
   if (move_val != 0)
      set_aff_to_obj( obj, APPLY_MOVE, move_val );
   if (spellpower_val > 0)
      set_aff_to_obj( obj, APPLY_SPELLPOWER, spellpower_val);
}

void set_aff_to_obj(OBJ_DATA *obj, int location, int modifier)
{
   AFFECT_DATA *new_af;

   GET_FREE( new_af, affect_free );
   new_af->type = -1;
   new_af->duration = -1;
   new_af->location = location;
   new_af->modifier = modifier;
   new_af->bitvector = 0;
   new_af->caster = NULL;
   LINK( new_af, obj->first_apply, obj->last_apply, next, prev );
}
