#include "globals.h"

char *get_wear_name(OBJ_DATA *obj);
void set_obj_stat_auto(OBJ_DATA *obj);
void set_aff_to_obj(OBJ_DATA *obj, int location, int modifier);
char *get_suffix(OBJ_DATA *obj);
bool is_jewelry(OBJ_DATA *obj);
char *get_prefix(OBJ_DATA *obj);

OBJ_DATA *generate_item(int level)
{
   char buf[MSL];
   OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_MUSHROOM), 0);
   obj->level = level;
   if (obj->level > 150)
      obj->level = 150;
   if (obj->level < 1)
      obj->level = 1;
   obj->weight = number_range(1, 15);
   SET_BIT(obj->extra_flags, ITEM_GENERATED);
   SET_BIT(obj->extra_flags, ITEM_BIND_EQUIP);
   obj->item_type = ITEM_ARMOR;

   switch (number_range(1, 26))
   {
   case 1:
      SET_BIT(obj->wear_flags, ITEM_WEAR_HALO);
      break;
   case 2:
      SET_BIT(obj->wear_flags, ITEM_WEAR_AURA);
      break;
   case 3:
      SET_BIT(obj->wear_flags, ITEM_WEAR_HORNS);
      break;
   case 4:
      SET_BIT(obj->wear_flags, ITEM_WEAR_HEAD);
      break;
   case 5:
      SET_BIT(obj->wear_flags, ITEM_WEAR_FACE);
      break;
   case 6:
      SET_BIT(obj->wear_flags, ITEM_WEAR_BEAK);
      break;
   case 7:
      SET_BIT(obj->wear_flags, ITEM_WEAR_EAR);
      break;
   case 8:
      SET_BIT(obj->wear_flags, ITEM_WEAR_NECK);
      break;
   case 9:
      SET_BIT(obj->wear_flags, ITEM_WEAR_WINGS);
      break;
   case 10:
      SET_BIT(obj->wear_flags, ITEM_WEAR_SHOULDERS);
      break;
   case 11:
      SET_BIT(obj->wear_flags, ITEM_WEAR_ARMS);
      break;
   case 12:
      SET_BIT(obj->wear_flags, ITEM_WEAR_WRIST);
      break;
   case 13:
      SET_BIT(obj->wear_flags, ITEM_WEAR_HANDS);
      break;
   case 14:
      SET_BIT(obj->wear_flags, ITEM_WEAR_FINGER);
      break;
   case 15:
      SET_BIT(obj->wear_flags, ITEM_WEAR_CLAWS);
      break;
   case 17:
      SET_BIT(obj->wear_flags, ITEM_WEAR_ABOUT);
      break;
   case 18:
      SET_BIT(obj->wear_flags, ITEM_WEAR_WAIST);
      break;
   case 19:
      SET_BIT(obj->wear_flags, ITEM_WEAR_BODY);
      break;
   case 20:
      SET_BIT(obj->wear_flags, ITEM_WEAR_TAIL);
      break;
   case 21:
      SET_BIT(obj->wear_flags, ITEM_WEAR_LEGS);
      break;
   case 22:
      SET_BIT(obj->wear_flags, ITEM_WEAR_FEET);
      break;
   case 23:
      SET_BIT(obj->wear_flags, ITEM_WEAR_HOOVES);
      break;
   case 16:
   default:
      SET_BIT(obj->wear_flags, ITEM_WEAR_HOLD_HAND);
      int chance = number_percent();
      if (chance <= 20)
      {
         obj->item_type = ITEM_LIGHT;
      }
      else if (obj->level > 100 && chance <= 30)
      {
         SET_BIT(obj->extra_flags, ITEM_BUCKLER);
      }
      else if (obj->level > 100 && chance <= 40)
      {
         SET_BIT(obj->extra_flags, ITEM_EXTRA_WAND);
         obj->item_type = ITEM_WEAPON;
      }
      else if (obj->level > 100 && chance <= 50)
      {
         SET_BIT(obj->extra_flags, ITEM_FIST);
         obj->item_type = ITEM_WEAPON;
      }
      else if (obj->level > 100 && chance <= 70)
      {
         obj->item_type = ITEM_WEAPON;
         obj->value[1] = obj->level / 10 + 1;
         obj->value[2] = obj->level / 5 + 2;
         obj->value[3] = number_range(1, 13);
      }
      break;
   }

   sprintf(buf, "%s%s %s", get_prefix(obj), get_wear_name(obj), get_suffix(obj));
   obj->name = str_dup(buf);
   obj->short_descr = str_dup(buf);
   sprintf(buf, "%s lies here", obj->short_descr);
   obj->description = str_dup(buf);

   set_obj_stat_auto(obj);

   if (obj->weight > 14)
      obj->weight = 14;
   else if (obj->weight == 11)
      obj->weight = 12;
   else if (obj->weight == 9)
      obj->weight = 8;
   else if (obj->weight == 5)
      obj->weight = 6;
   else if (obj->weight < 2)
      obj->weight = 2;

   return obj;
}

char *get_suffix(OBJ_DATA *obj)
{
   if (obj->weight > 10)
      return "of the Defender";
   else if (obj->weight > 5)
      return "of the Fighter";
   else
      return "of the Arcanist";
}

char *get_prefix(OBJ_DATA *obj)
{
   switch (obj->weight)
   {
   case 2:
      return "Magus ";
   case 4:
      return "Tricksters ";
   case 6:
      return "Guardians ";
   case 8:
      return "Berserkers ";
   case 12:
      return "Protectors ";
   case 14:
      return "Conquerors ";
   default:
      return "";
   }
}

char *get_wear_name(OBJ_DATA *obj)
{
   if (obj->item_type == ITEM_WEAPON)
      return "Weapon";

   if (obj->item_type == ITEM_LIGHT)
   {
      obj->value[2] = -1;
      if (obj->level > 100)
         return "Lantern";

      return "Torch";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_HALO))
   {
      if (obj->level > 100)
         return "Soulstone";

      return "Halo";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_AURA))
   {
      if (obj->level > 100)
         return "Soul";

      return "Aura";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_HORNS))
      return "Horns";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_HEAD))
   {
      if (obj->weight == 3)
         return "Helmet";
      else if (obj->weight == 2)
         return "Helm";

      return "Hat";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_BEAK))
      return "Beak";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_FACE))
   {
      if (obj->weight == 3)
         return "Visor";
      else if (obj->weight == 2)
         return "Faceguard";
      else
         return "Visage";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_EAR))
      return "Earring";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_NECK))
      return "Necklace";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_WINGS))
      return "Wings";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_SHOULDERS))
   {
      if (obj->weight == 3)
         return "Pauldrons";
      else if (obj->weight == 2)
         return "Spaulders";
      else
         return "Epaulets";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_ARMS))
   {
      if (obj->weight == 3)
         return "Plate Vambraces";
      else if (obj->weight == 2)
         return "Leather Vambraces";
      else
         return "Sleeves";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_WRIST))
   {
      if (obj->weight == 3)
         return "Plate Bracers";
      else if (obj->weight == 2)
         return "Leather Bracers";
      else
         return "Bracers";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_HANDS))
   {
      if (obj->weight == 3)
         return "Plate Gauntlets";
      else if (obj->weight == 2)
         return "Leather Rondel";
      else
         return "Gloves";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_FINGER))
      return "Ring";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_CLAWS))
      return "Claws";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_ABOUT))
      return "Cape";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_WAIST))
      return "Belt";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_BODY))
   {
      if (obj->weight == 3)
         return "Platemail Armor";
      else if (obj->weight == 2)
         return "Hauberk";
      else
         return "Robes";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_TAIL))
      return "Tailguard";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_LEGS))
   {
      if (obj->weight == 3)
         return "Plate Greaves";
      else if (obj->weight == 2)
         return "Leather Greaves";
      else
         return "Leggings";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_FEET))
   {
      if (obj->weight == 3)
         return "Plate Boots";
      else if (obj->weight == 2)
         return "Leather Boots";
      else
         return "Boots";
   }

   if (IS_SET(obj->wear_flags, ITEM_WEAR_HOOVES))
      return "Hooves";

   if (IS_SET(obj->wear_flags, ITEM_WEAR_HOLD_HAND))
   {
      if (IS_SET(obj->extra_flags, ITEM_BUCKLER))
         return "Buckler";

      return "Shield";
   }

   return "null";
}

void set_obj_stat_auto(OBJ_DATA *obj)
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

   // Fuck this number.
   if (obj->level == 88)
      obj->level = 87;

   if (obj->level > MAX_MORTAL)
   {
      ilevel += (obj->level - MAX_MORTAL) * 3;
   }

   if (obj->level > 124)
   {
      ilevel += (obj->level - 124) * 4;
   }

   if (obj->level == 150)
   {
      ilevel += 50;
   }

   if (IS_SET(obj->extra_flags, ITEM_GENERATED))
      ilevel *= 0.8;

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

   if (IS_SET(obj->extra_flags, ITEM_TWO_HANDED) && obj->item_type == ITEM_WEAPON)
      ilevel *= 1.25;

   if (IS_SET(obj->extra_flags, ITEM_LOOT))
      ilevel *= 1.2;

   /* Jewelry */
   if (is_jewelry(obj))
   {
      hr_div = 15;
      dr_div = 15;
      ac_div = 20;
      hp_div = 2;
      mana_div = 2;
      move_div = 2;
      ac_bonus = -5;
      hrdr_bonus = 1;
      stat_bonus = 5;

      if (obj->weight > 10)
      {
         ac_div -= 5;
         hp_div /= 2;
         mana_div *= 2;
         move_div *= 2;
      }
      else if (obj->weight > 5)
      {
         /* Do nothing for now */
      }
      else
      {
         /* Caster */
         dr_div *= 2;
         move_div *= 2;

         spellpower_div = 12;
      }
   }
   else if (obj->item_type == ITEM_WEAPON)
   {
      hrdr_bonus = 10;
      ac_bonus = -5;
      stat_bonus = 5;
      hr_div = 5;
      dr_div = 5;
      ac_div = 20;
      hp_div = 10;
      move_div = 10;
      mana_div = 10;

      if (obj->weight > 10)
      {
         ac_div -= 5;
         hp_div /= 2;
         mana_div *= 2;
         move_div *= 2;
      }
      else if (obj->weight > 5)
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
         ac_bonus = 0;
         hrdr_bonus = 0;
         ac_div = 0;
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
      hr_div = 10;
      dr_div = 10;
      ac_div = 2;
      hp_div = 5;
      mana_div = 5;
      move_div = 5;

      if (obj->weight > 10)
      {
         ac_div -= 1;
         hp_div /= 2;
         mana_div *= 2;
         move_div *= 2;
      }
      else if (obj->weight > 5)
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

      if (obj->weight > 10)
      {
         ac_div -= 5;
         hp_div /= 2;
         mana_div *= 2;
         move_div *= 2;
      }
      else if (obj->weight > 5)
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
      ac_val -= (ilevel * 2 / ac_div);
   if (ac_val == -88)
      ac_val = -87;
   int hr_val = hrdr_bonus;
   if (hr_div != 0)
      hr_val += (ilevel / hr_div);
   if (hr_val == 88)
      hr_val = 87;
   int dr_val = hrdr_bonus;
   if (dr_div != 0)
      dr_val += (ilevel / dr_div);
   if (dr_val == 88)
      dr_val = 87;
   int hp_val = stat_bonus;
   if (hp_div != 0)
      hp_val += (ilevel / hp_div);
   if (hp_val == 88)
      hp_val = 87;
   int mana_val = stat_bonus;
   if (mana_div != 0)
      mana_val += (ilevel / mana_div);
   if (mana_val == 88)
      mana_val = 87;
   int move_val = stat_bonus;
   if (move_div != 0)
      move_val += (ilevel / move_div);
   if (move_val == 88)
      move_val = 87;
   int spellpower_val = 0;
   if (spellpower_div > 0)
      spellpower_val = (ilevel / spellpower_div);
   if (spellpower_val == 88)
      spellpower_val = 87;

   if (ac_val != 0)
      set_aff_to_obj(obj, APPLY_AC, ac_val);
   if (hr_val != 0)
      set_aff_to_obj(obj, APPLY_HITROLL, hr_val);
   if (dr_val != 0)
      set_aff_to_obj(obj, APPLY_DAMROLL, dr_val);
   if (hp_val != 0)
      set_aff_to_obj(obj, APPLY_HIT, hp_val);
   if (mana_val != 0)
      set_aff_to_obj(obj, APPLY_MANA, mana_val);
   if (move_val != 0)
      set_aff_to_obj(obj, APPLY_MOVE, move_val);
   if (spellpower_val > 0)
      set_aff_to_obj(obj, APPLY_SPELLPOWER, spellpower_val);

   if (is_jewelry(obj) && (IS_SET(obj->extra_flags, ITEM_RARE) || IS_SET(obj->extra_flags, ITEM_MYTHIC)))
   {
      int rare_mod = 1;

      if (IS_SET(obj->wear_flags, ITEM_WEAR_NECK))
      {
         if (obj->weight == 1)
            set_aff_to_obj(obj, APPLY_WIS, rare_mod);
         else
            set_aff_to_obj(obj, APPLY_DEX, rare_mod);
      }
      else if (IS_SET(obj->wear_flags, ITEM_WEAR_FINGER))
      {
         if (obj->weight == 1)
            set_aff_to_obj(obj, APPLY_INT, rare_mod);
         else
            set_aff_to_obj(obj, APPLY_STR, rare_mod);
      }
      else
      {
         set_aff_to_obj(obj, APPLY_CON, rare_mod);
      }
   }

   if (is_jewelry(obj) && IS_SET(obj->extra_flags, ITEM_LEGENDARY))
   {
      int legendary_mod = 2;

      if (IS_SET(obj->wear_flags, ITEM_WEAR_NECK))
      {
         if (obj->weight == 1)
            set_aff_to_obj(obj, APPLY_WIS, legendary_mod);
         else
            set_aff_to_obj(obj, APPLY_DEX, legendary_mod);
      }
      else if (IS_SET(obj->wear_flags, ITEM_WEAR_FINGER))
      {
         if (obj->weight == 1)
            set_aff_to_obj(obj, APPLY_INT, legendary_mod);
         else
            set_aff_to_obj(obj, APPLY_STR, legendary_mod);
      }
      else
      {
         set_aff_to_obj(obj, APPLY_CON, legendary_mod);
      }
   }

   if (IS_SET(obj->extra_flags, ITEM_MYTHIC))
      set_aff_to_obj(obj, APPLY_DAMCAP, 20);
   else if (IS_SET(obj->extra_flags, ITEM_LEGENDARY))
      set_aff_to_obj(obj, APPLY_DAMCAP, 40);
}

void set_aff_to_obj(OBJ_DATA *obj, int location, int modifier)
{
   AFFECT_DATA *new_af;

   GET_FREE(new_af, affect_free);
   new_af->type = -1;
   new_af->duration = -1;
   new_af->location = location;
   new_af->modifier = modifier;
   new_af->bitvector = 0;
   new_af->caster = NULL;
   LINK(new_af, obj->first_apply, obj->last_apply, next, prev);
}

bool is_jewelry(OBJ_DATA *obj)
{
   if (IS_SET(obj->wear_flags, ITEM_WEAR_HALO) || IS_SET(obj->wear_flags, ITEM_WEAR_AURA) ||
       IS_SET(obj->wear_flags, ITEM_WEAR_NECK) || IS_SET(obj->wear_flags, ITEM_WEAR_FINGER) ||
       IS_SET(obj->wear_flags, ITEM_WEAR_EAR))
      return TRUE;

   return FALSE;
}

bool create_loot(CHAR_DATA *ch, OBJ_DATA *corpse)
{
   char buf[MSL];
   int total = ch->loot_amount / 100;
   int created = 0;

   if (ch->loot_amount % 100 > 0)
   {
      if (number_percent() < ch->loot_amount)
         total++;
   }

   //   sprintf(buf, "create_loot loot total is %d", total);
   // bug(buf,0);
   if (total == 0)
      return FALSE;

   while (created < total)
   {
      int chance = 0;
      bool viable = FALSE;

      int roll = number_percent();
      for (int i = 0; i < MAX_LOOT; i++)
      {
         //       sprintf(buf, "Chloot %d Chlootchance %d chance %d", ch->loot[i], ch->loot_chance[i], chance);
         //         bug(buf,0);
         if (ch->loot[i] > 0 && ch->loot_chance[i] > 0 && roll <= ch->loot_chance[i] + chance)
         {
            viable = TRUE;
            OBJ_DATA *obj = create_object(get_obj_index(ch->loot[i]), 0);

            if (obj != NULL)
            {
               obj_to_obj(obj, corpse);
            }
            created++;
            break;
         }
         chance += ch->loot_chance[i];
      }
      if (!viable)
      {
         sprintf(buf, "%s did not create a viable loot table", ch->short_descr);
         bug(buf, 0);
         return FALSE;
      }
   }

   return TRUE;
}
