#ifndef DEC_SPECIAL_H
#define DEC_SPECIAL_H 1

DECLARE_SPEC_FUN(spec_breath_any);
DECLARE_SPEC_FUN(spec_breath_acid);
DECLARE_SPEC_FUN(spec_breath_fire);
DECLARE_SPEC_FUN(spec_breath_frost);
DECLARE_SPEC_FUN(spec_breath_gas);
DECLARE_SPEC_FUN(spec_breath_lightning);
DECLARE_SPEC_FUN(spec_cast_adept);
DECLARE_SPEC_FUN(spec_cast_cleric);
DECLARE_SPEC_FUN(spec_cast_judge);
DECLARE_SPEC_FUN(spec_cast_mage);
DECLARE_SPEC_FUN(spec_cast_undead);
DECLARE_SPEC_FUN(spec_executioner);
DECLARE_SPEC_FUN(spec_fido);
DECLARE_SPEC_FUN(spec_guard);
DECLARE_SPEC_FUN(spec_janitor);
DECLARE_SPEC_FUN(spec_mayor);
DECLARE_SPEC_FUN(spec_poison);
DECLARE_SPEC_FUN(spec_thief);
DECLARE_SPEC_FUN(spec_policeman);
DECLARE_SPEC_FUN(spec_cast_cadaver);
DECLARE_SPEC_FUN(spec_undead);
DECLARE_SPEC_FUN(spec_stephen);
DECLARE_SPEC_FUN(spec_rewield);
DECLARE_SPEC_FUN(spec_sylai_priest);
DECLARE_SPEC_FUN(spec_cast_bigtime);
DECLARE_SPEC_FUN(spec_sage);
DECLARE_SPEC_FUN(spec_wizardofoz);
DECLARE_SPEC_FUN(spec_vamp_hunter);
DECLARE_SPEC_FUN(spec_mino_guard);
DECLARE_SPEC_FUN(spec_tax_man);
DECLARE_SPEC_FUN(spec_keep_physical_captain);
DECLARE_SPEC_FUN(spec_summon_water);
DECLARE_SPEC_FUN(spec_summon_fire);
DECLARE_SPEC_FUN(spec_summon_earth);
DECLARE_SPEC_FUN(spec_summon_undead);
DECLARE_SPEC_FUN(spec_summon_holy);
DECLARE_SPEC_FUN(spec_summon_shadow);
DECLARE_SPEC_FUN(spec_summon_metal);
DECLARE_SPEC_FUN(spec_summon_animate);
DECLARE_SPEC_FUN(spec_summon_thought);
DECLARE_SPEC_FUN(spec_lamplighter);
DECLARE_SPEC_FUN(spec_warden);
DECLARE_SPEC_FUN(spec_vendor);
DECLARE_SPEC_FUN(spec_lay_sister);
DECLARE_SPEC_FUN(spec_laborer);
DECLARE_SPEC_FUN(spec_city_messenger);
DECLARE_SPEC_FUN(spec_park_keeper);
DECLARE_SPEC_FUN(spec_records_keeper);
DECLARE_SPEC_FUN(spec_posting_clerk);
DECLARE_SPEC_FUN(spec_harbor_hand);
DECLARE_SPEC_FUN(spec_kiess_shopkeeper);
DECLARE_SPEC_FUN(spec_kiess_innkeeper);
DECLARE_SPEC_FUN(spec_kiess_scout);
DECLARE_SPEC_FUN(spec_kiess_orator);
DECLARE_SPEC_FUN(spec_kiess_wall_officer);
DECLARE_SPEC_FUN(spec_kowloon_shopkeeper);
DECLARE_SPEC_FUN(spec_kowloon_gate_captain);
DECLARE_SPEC_FUN(spec_kowloon_courier);
DECLARE_SPEC_FUN(spec_kowloon_innkeeper);
DECLARE_SPEC_FUN(spec_kowloon_corsair);
DECLARE_SPEC_FUN(spec_kowloon_sweeper);
DECLARE_SPEC_FUN(spec_kowloon_magistracy);
DECLARE_SPEC_FUN(spec_kowloon_shrine);
DECLARE_SPEC_FUN(spec_kowloon_vendor);
DECLARE_SPEC_FUN(spec_kowloon_laborer);
DECLARE_SPEC_FUN(spec_gnf_road_warden);
DECLARE_SPEC_FUN(spec_gnf_lamplighter);
DECLARE_SPEC_FUN(spec_gnf_peddler);
DECLARE_SPEC_FUN(spec_gnf_mire_speaker);
DECLARE_SPEC_FUN(spec_gnf_everkeeper);
DECLARE_SPEC_FUN(spec_gnf_joint_scout);
DECLARE_SPEC_FUN(spec_gnf_toll_collector);
DECLARE_SPEC_FUN(spec_gnf_customs);
DECLARE_SPEC_FUN(spec_gnf_courier);
DECLARE_SPEC_FUN(spec_rr_road_clerk);
DECLARE_SPEC_FUN(spec_rr_warden_captain);
DECLARE_SPEC_FUN(spec_rr_charter_keeper);
DECLARE_SPEC_FUN(spec_rr_convoy_marshal);
DECLARE_SPEC_FUN(spec_rr_peddler);
DECLARE_SPEC_FUN(spec_rr_shrine_keeper);
DECLARE_SPEC_FUN(spec_rr_ferryman);
DECLARE_SPEC_FUN(spec_rr_camp_cook);
DECLARE_SPEC_FUN(spec_rr_pilgrim);
DECLARE_SPEC_FUN(spec_rr_ruin_scavenger);
DECLARE_SPEC_FUN(spec_pyramid_black_sun_shard);
DECLARE_SPEC_FUN(spec_keep_elemental_captain);
DECLARE_SPEC_FUN(spec_hermit_archon);
DECLARE_SPEC_FUN(spec_sanctum_keeper);
DECLARE_SPEC_FUN(spec_ss_cinder_broker);
DECLARE_SPEC_FUN(spec_ss_manifest_warden);
DECLARE_SPEC_FUN(spec_ss_kiln_overseer);

SPEC_FUN *spec_lookup(const char *name);
char *rev_spec_lookup(void *func);
void print_spec_lookup(char *buf);
bool is_player_summon_special(SPEC_FUN *spec_fun);

/* Damage dispatch — call from do_damage() for any NPC with a spec_fun */
void spec_handle_damage(CHAR_DATA *victim, int element, int dam);

/* Death dispatch — call from group_gain() when an NPC dies */
void spec_death_handler(CHAR_DATA *victim, CHAR_DATA *killer);

/* Superboss tracking — returns index into superboss_kills[], or -1 */
int spec_superboss_index(CHAR_DATA *mob);

/* Heal chance calculation for summons */
int summon_master_heal_chance(int master_hit, int master_max_hp, int thematic_bonus);

/* Per-spec damage callbacks */
void spec_keep_elemental_captain_damage(CHAR_DATA *ch, int element, int dam);
void spec_hermit_archon_damage(CHAR_DATA *ch, int element, int dam);
void spec_sanctum_keeper_damage(CHAR_DATA *ch, int element, int dam);

#endif
