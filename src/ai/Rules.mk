AI_SPEC_SRCS = \
	  ai/spec_summon_water.o ai/spec_summon_fire.o ai/spec_summon_earth.o \
	  ai/spec_summon_undead.o ai/spec_summon_holy.o ai/spec_summon_shadow.o \
	  ai/spec_summon_metal.o ai/spec_summon_animate.o ai/spec_summon_thought.o \
	  ai/spec_summon_revenant.o \
	  ai/spec_breath_any.o ai/spec_breath_acid.o ai/spec_breath_fire.o \
	  ai/spec_breath_frost.o ai/spec_breath_gas.o ai/spec_breath_lightning.o \
	  ai/spec_cast_adept.o ai/spec_cast_cleric.o ai/spec_cast_judge.o \
	  ai/spec_cast_mage.o ai/spec_cast_undead.o ai/spec_executioner.o \
	  ai/spec_mino_guard.o ai/spec_fido.o ai/spec_guard.o ai/spec_janitor.o \
	  ai/spec_mayor.o ai/spec_poison.o ai/spec_thief.o ai/spec_policeman.o \
	  ai/spec_cast_cadaver.o ai/spec_undead.o ai/spec_stephen.o ai/spec_rewield.o \
	  ai/spec_sylai_priest.o ai/spec_cast_bigtime.o ai/spec_sage.o \
	  ai/spec_wizardofoz.o ai/spec_vamp_hunter.o ai/spec_tax_man.o \
	  ai/spec_lamplighter.o ai/spec_warden.o ai/spec_vendor.o \
	  ai/spec_lay_sister.o ai/spec_laborer.o \
	  ai/spec_city_messenger.o ai/spec_park_keeper.o ai/spec_records_keeper.o \
	  ai/spec_posting_clerk.o ai/spec_harbor_hand.o \
	  ai/spec_keep_physical_captain.o \
	  ai/spec_kiess_shopkeeper.o ai/spec_kiess_innkeeper.o ai/spec_kiess_scout.o \
	  ai/spec_kiess_orator.o ai/spec_kiess_wall_officer.o \
	  ai/spec_kowloon_shopkeeper.o ai/spec_kowloon_gate_captain.o \
	  ai/spec_kowloon_courier.o ai/spec_kowloon_innkeeper.o \
	  ai/spec_kowloon_corsair.o ai/spec_kowloon_sweeper.o \
	  ai/spec_kowloon_magistracy.o ai/spec_kowloon_shrine.o \
	  ai/spec_kowloon_vendor.o ai/spec_kowloon_laborer.o \
	  ai/spec_gnf_road_warden.o ai/spec_gnf_lamplighter.o ai/spec_gnf_peddler.o \
	  ai/spec_gnf_mire_speaker.o ai/spec_gnf_everkeeper.o \
	  ai/spec_gnf_joint_scout.o ai/spec_gnf_toll_collector.o \
	  ai/spec_gnf_customs.o ai/spec_gnf_courier.o \
	  ai/spec_rr_road_clerk.o ai/spec_rr_warden_captain.o \
	  ai/spec_rr_charter_keeper.o ai/spec_rr_convoy_marshal.o \
	  ai/spec_rr_peddler.o ai/spec_rr_shrine_keeper.o ai/spec_rr_ferryman.o \
	  ai/spec_rr_camp_cook.o ai/spec_rr_pilgrim.o ai/spec_rr_ruin_scavenger.o \
	  ai/spec_rr_byways_warden.o ai/spec_rr_byways_hermit.o \
	  ai/spec_rr_byways_innkeeper.o ai/spec_rr_byways_ferryman.o \
	  ai/spec_rr_byways_scholar.o ai/spec_rr_byways_market.o \
	  ai/spec_rr_byways_kiess_clerk.o ai/spec_rr_byways_midgaard_guard.o \
	  ai/spec_reach_warden.o ai/spec_reach_guide.o ai/spec_reach_assessor.o \
	  ai/spec_reach_outrider.o ai/spec_reach_scavenger.o ai/spec_reach_tidewright.o \
	  ai/spec_reach_carter.o ai/spec_reach_smuggler.o \
	  ai/spec_pyramid_black_sun_shard.o \
	  ai/spec_keep_elemental_captain.o \
	  ai/spec_hermit_archon.o \
	  ai/spec_sanctum_keeper.o \
	  ai/spec_ss_cinder_broker.o ai/spec_ss_manifest_warden.o \
	  ai/spec_ss_kiln_overseer.o \
	  ai/spec_midgaard_gate_warden.o ai/spec_midgaard_caravan_master.o \
	  ai/spec_midgaard_pilgrim.o ai/spec_midgaard_city_guard.o \
	  ai/spec_midgaard_executioner.o ai/spec_midgaard_temple_guardian.o \
	  ai/spec_midgaard_healer.o ai/spec_midgaard_beggar.o \
	  ai/spec_midgaard_street_vendor.o ai/spec_midgaard_temple_priest.o \
	  ai/spec_midgaard_shopkeeper.o ai/spec_midgaard_postmaster.o \
	  ai/spec_midgaard_quartermaster.o ai/spec_midgaard_invasion_warden.o \
	  ai/spec_cinderteeth_warlord.o ai/spec_cinderteeth_warden_cmd.o \
	  ai/spec_cinderteeth_sulfur_colossus.o ai/spec_cinderteeth_anchor.o \
	  ai/spec_cinderteeth_ventspeaker.o ai/spec_cinderteeth_oracle.o \
	  ai/spec_cinderteeth_caldera_watcher.o ai/spec_cinderteeth_patriarch.o \
	  ai/spec_mudschool_guide.o

AI_SUMMON_SRCS = ai/spec_summon_water.o ai/spec_summon_fire.o ai/spec_summon_earth.o \
	  ai/spec_summon_undead.o ai/spec_summon_holy.o ai/spec_summon_shadow.o \
	  ai/spec_summon_metal.o ai/spec_summon_animate.o ai/spec_summon_thought.o \
	  ai/spec_summon_revenant.o

AI_SUMMON_OBJS = $(addprefix $(OBJDIR)/,$(AI_SUMMON_SRCS))

$(OBJDIR)/ai/%.o: ai/%.c headers/ack.h
	@mkdir -p $(dir $@)
	$(CC) -c $(C_FLAGS) -I. -o $@ $<
