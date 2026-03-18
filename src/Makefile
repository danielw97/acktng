CC      = gcc
PROF    =
.DEFAULT_GOAL := all

# Debugging flags possible:  DEBUG_MEM DEBUG_MEM_CORRUPT DEBUG_MEM_DUP_FREE
# DEBUG_MEM is most basic, just checks magic numbers when freeing
# DEBUG_MEM_CORRUPT checks every free block to see if magic numbers ok, every
#                   call to alloc_mem
#		    Also makes calls to void check_free_mem(void) check for
#		    corrupted free memory blocks.
# DEBUG_MEM_DUP_FREE checks to see if freed blocks are overlapping every call
# 		     to mem_alloc.
#

#-DDEBUG_MEM -DDEBUG_MEM_CORRUPT -DDEBUG_MEM_DUP_FREE
#-DDEBUG_MEM -DDEBUG_MEM_CORRUPT

#Uncomment to compile in Cygwin
#CYGWIN = -DCYGWIN

# Uncomment the two lines below if compiling on a Solaris box
#SOLARIS_FLAG = -Dsun -DSYSV -Wno-char-subscripts
#SOLARIS_LINK = -lnsl -lsocket -lresolv

C_FLAGS = -O -g2 -Wall -DACK_43 $(PROF) $(SOLARIS_FLAG) -Iheaders -Iai/headers -Itests/headers -Iquests
L_FLAGS = -O -g2 -lcrypt $(PROF) $(SOLARIS_LINK)

OBJDIR = build/obj

include ai/Rules.mk
include tests/Rules.mk

MAIN_OBJS = $(addprefix $(OBJDIR)/,$(O_FILES))

SPELL_OBJS = \
	spells/spell_acid_blast.o spells/spell_acid_breath.o spells/spell_animate.o \
	spells/spell_arc_lightning.o spells/spell_armor.o spells/spell_badbreath.o \
	spells/spell_bark_skin.o spells/spell_barrier.o spells/spell_beacon.o \
	spells/spell_black_curse.o spells/spell_black_hand.o spells/spell_bless.o \
	spells/spell_blindness.o spells/spell_bloody_tears.o spells/spell_burning_hands.o \
	spells/spell_cage.o spells/spell_call_lightning.o spells/spell_calm.o \
	spells/spell_cause_critical.o spells/spell_cause_light.o spells/spell_cause_serious.o \
	spells/spell_chain_lightning.o spells/spell_change_sex.o spells/spell_charm_person.o \
	spells/spell_chill_touch.o spells/spell_cloak_absorb.o spells/spell_cloak_adept.o \
	spells/spell_cloak_drain.o spells/spell_cloak_elements.o spells/spell_cloak_flaming.o \
	spells/spell_cloak_iron.o spells/spell_cloak_mental.o spells/spell_cloak_misery.o \
	spells/spell_cloak_oathbreaker.o spells/spell_cloak_reflect.o \
	spells/spell_cloak_transcendence.o spells/spell_color_spray.o \
	spells/spell_condense_soul.o spells/spell_cone_cold.o spells/spell_continual_light.o \
	spells/spell_control_weather.o spells/spell_corrupt_bond.o spells/spell_create_food.o \
	spells/spell_create_spring.o spells/spell_create_water.o spells/spell_creature_bond.o \
	spells/spell_cure_blindness.o spells/spell_cure_critical.o spells/spell_cure_light.o \
	spells/spell_cure_poison.o spells/spell_cure_serious.o spells/spell_curse.o \
	spells/spell_deflect_weapon.o spells/spell_detect_evil.o spells/spell_detect_hidden.o \
	spells/spell_detect_invis.o spells/spell_detect_magic.o spells/spell_detect_poison.o \
	spells/spell_detect_undead.o spells/spell_detection.o spells/spell_diamond_golem.o \
	spells/spell_dimension_blade.o spells/spell_dispel_evil.o spells/spell_dispel_magic.o \
	spells/spell_dispel_object.o spells/spell_divine_intervention.o \
	spells/spell_earth_shatter.o spells/spell_earthelem.o spells/spell_earthquake.o \
	spells/spell_ego_whip.o spells/spell_elemental_deluge.o spells/spell_elemental_inferno.o \
	spells/spell_elemental_rupture.o spells/spell_elemental_shock.o \
	spells/spell_enchant_weapon.o spells/spell_encumber.o spells/spell_energy_drain.o \
	spells/spell_enhance_weapon.o spells/spell_faerie_fire.o spells/spell_faerie_fog.o \
	spells/spell_feeble_body.o spells/spell_feeble_mind.o spells/spell_fighting_trance.o \
	spells/spell_fire_breath.o spells/spell_fireball.o spells/spell_fireblast.o \
	spells/spell_fireelem.o spells/spell_fireshield.o spells/spell_flamestrike.o \
	spells/spell_flare.o spells/spell_fly.o spells/spell_frost_breath.o \
	spells/spell_gas_breath.o spells/spell_gate.o spells/spell_general_purpose.o \
	spells/spell_giant_strength.o spells/spell_group_heal.o spells/spell_harm.o \
	spells/spell_haste.o spells/spell_heal.o spells/spell_healing_light.o \
	spells/spell_heat_armor.o spells/spell_hellspawn.o spells/spell_high_explosive.o \
	spells/spell_holy_armor.o spells/spell_holy_avenger.o spells/spell_holy_light.o \
	spells/spell_holy_wrath.o spells/spell_hypnosis.o spells/spell_ice_bolt.o \
	spells/spell_iceshield.o spells/spell_identify.o spells/spell_influx.o \
	spells/spell_infravision.o spells/spell_infuse.o spells/spell_invis.o \
	spells/spell_iron_golem.o spells/spell_kinetic_reversion.o \
	spells/spell_know_alignment.o spells/spell_know_critical.o spells/spell_know_item.o \
	spells/spell_know_weakness.o spells/spell_laserbolt.o spells/spell_lava_burst.o \
	spells/spell_lightning_bolt.o spells/spell_lightning_breath.o \
	spells/spell_locate_object.o spells/spell_magic_missile.o \
	spells/spell_magical_supremacy.o spells/spell_mana_drain.o spells/spell_mana_flare.o \
	spells/spell_mass_invis.o spells/spell_mind_bolt.o spells/spell_mind_flail.o \
	spells/spell_mindflame.o spells/spell_mystic_armor.o spells/spell_mystical_focus.o \
	spells/spell_nerve_fire.o spells/spell_night_vision.o spells/spell_null.o \
	spells/spell_overdrive.o spells/spell_pass_door.o spells/spell_phase.o \
	spells/spell_phobia.o spells/spell_phoenix_flare.o spells/spell_physic_crush.o \
	spells/spell_physic_thrust.o spells/spell_planergy.o spells/spell_poison.o \
	spells/spell_poison_arsenic.o spells/spell_poison_quinine.o \
	spells/spell_poison_weapon.o spells/spell_portal.o spells/spell_produce_food.o \
	spells/spell_protection.o spells/spell_psionic_recovery.o spells/spell_purify.o \
	spells/spell_refresh.o spells/spell_refresh_mana.o spells/spell_refuge.o \
	spells/spell_regen.o spells/spell_remove_curse.o spells/spell_restoration.o \
	spells/spell_retri_strike.o spells/spell_rictus_curse.o spells/spell_room_dispel.o \
	spells/spell_rune_fire.o spells/spell_rune_poison.o spells/spell_rune_shock.o \
	spells/spell_sanctuary.o spells/spell_seal_room.o spells/spell_see_magic.o \
	spells/spell_sense_evil.o spells/spell_shadowshield.o spells/spell_shield.o \
	spells/spell_shocking_grasp.o spells/spell_shockshield.o spells/spell_shockstorm.o \
	spells/spell_skeleton.o spells/spell_sleep.o spells/spell_slow.o \
	spells/spell_sonic_blast.o spells/spell_soul_net.o spells/spell_soul_thief.o \
	spells/spell_spirit_curse.o spells/spell_stalker.o spells/spell_static.o \
	spells/spell_stone_skin.o spells/spell_suffocate.o spells/spell_summon.o \
	spells/spell_summon_beast.o spells/spell_summon_devourer.o \
	spells/spell_summon_nightmare.o spells/spell_summon_pegasus.o \
	spells/spell_summon_shadow.o spells/spell_summon_thought_devourer.o \
	spells/spell_teleport.o spells/spell_terra_rend.o spells/spell_thought_vise.o \
	spells/spell_thoughtshield.o spells/spell_throw_needle.o spells/spell_throw_star.o \
	spells/spell_tidal_lash.o spells/spell_travel.o spells/spell_ventriloquate.o \
	spells/spell_visit.o spells/spell_waterelem.o spells/spell_weaken.o \
	spells/spell_window.o spells/spell_wither_shadow.o spells/spell_word_of_recall.o \
	spells/spell_wraith_touch.o

SKILL_OBJS = \
	skills/do_anti_magic_shell.o skills/do_aurabolt.o skills/do_backstab.o \
	skills/do_bash.o skills/do_beserk.o skills/do_chakra.o skills/do_charge.o \
	skills/do_chiblock.o skills/do_circle.o skills/do_cripple.o skills/do_detox.o \
	skills/do_dirt.o skills/do_disarm.o skills/do_disguise.o skills/do_fleche.o \
	skills/do_flurry.o skills/do_frenzy.o skills/do_headbutt.o skills/do_holystrike.o \
	skills/do_kick.o skills/do_knee.o skills/do_leadership.o skills/do_mindoverbody.o \
	skills/do_morale.o skills/do_palmstrike.o skills/do_phantomfist.o skills/do_pick.o \
	skills/do_poison_arsenic.o skills/do_poison_nightshade.o skills/do_poison_quinine.o \
	skills/do_pummel.o skills/do_punch.o skills/do_rescue.o skills/do_riposte.o \
	skills/do_shieldblock.o skills/do_smash.o skills/do_spinfist.o skills/do_stun.o \
	skills/do_target.o skills/do_trip.o skills/do_warcry.o

QUEST_OBJS = quests/template.o quests/state.o quests/cartography.o quests/notify.o quests/commands.o quests/crusade.o

O_FILES = affect.o ai.o act_clan.o act_comm.o act_info.o act_mob.o act_move.o act_obj.o act_wiz.o \
	  areachk.o board.o build.o buildare.o buildtab.o caravan.o clutch.o comm.o socket.o prompt.o \
	  const.o damage.o cloak.o death.o db.o email.o enchant.o fight.o handler.o hash.o hunt.o \
	  heal.o interp.o invasion.o item_generation.o item_generation_tables.o keep.o lists.o lookup.o macros.o magic.o mapper.o \
	  obj_fun.o pdelete.o $(QUEST_OBJS) reincarnate.o \
	  save/save.o save/save_players.o save/save_objects.o save/save_mobs.o save/save_areas.o \
	  save/save_area_files.o save/save_rulers.o save/save_socials.o save/save_sysdata.o save/save_notes.o \
	  scheck.o skills.o skills_chi.o skills_combo.o skills_obj.o skills_poison.o special.o spell_dam.o spendqp.o ssm.o \
	  stance.o strfuns.o tables.o trigger.o update.o visibility.o wizutil.o \
	  write.o $(AI_SPEC_SRCS) $(SPELL_OBJS) $(SKILL_OBJS)

# Source files for linting (all .c and .h excluding third-party sha256 and build artifacts)
LINT_SOURCES = $(shell find . -name '*.c' -o -name '*.h' | grep -v sha256 | grep -v './build/' | sort)

.PHONY: all clean purge unit-tests integration-test integration-test-telnet web lint format

all:
	$(MAKE) clean
	$(MAKE) format
	$(MAKE) lint
	$(MAKE) web
	$(MAKE) ack
	$(MAKE) unit-tests

integration-test: ack
	cd .. && ./integration-test.sh

integration-test-telnet: ack
	cd .. && ./integration-test-telnet.sh

web:
	$(MAKE) -C ../web

ifdef CYGWIN
ack: $(MAIN_OBJS)
	rm -f ack.exe
	$(CC) -o ack.exe $(MAIN_OBJS) $(L_FLAGS)
	chmod g+w ack.exe
	chmod a+x ack.exe
	chmod g+w $(MAIN_OBJS)
else
ack: $(MAIN_OBJS)
	rm -f ack
	$(CC) -o ack $(MAIN_OBJS) $(L_FLAGS)
	chmod g+w ack
	chmod a+x ack
	chmod g+w $(MAIN_OBJS)
endif

clean:
	rm -rf $(OBJDIR) tests/unit-test-* ack ack.exe ../area/core *~
	$(MAKE) -C ../web clean

purge:
	rm -rf $(OBJDIR) tests/unit-test-* ack ack.exe ../area/core *~
	$(MAKE) -C ../web clean

$(OBJDIR)/%.o: %.c headers/ack.h
	@mkdir -p $(dir $@)
	$(CC) -c $(C_FLAGS) -o $@ $<

scan: $(OBJDIR)/scan.o
	rm -f scan
	$(CC) $(L_FLAGS) -o scan $(OBJDIR)/scan.o

# Check that all source files match the .clang-format style (fails if any differ)
lint:
	clang-format --dry-run --Werror $(LINT_SOURCES)

# Auto-apply the .clang-format style to all source files
format:
	clang-format -i $(LINT_SOURCES)
