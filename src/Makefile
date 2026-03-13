CC      = gcc
PROF    = 

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

C_FLAGS = -O -g2 -Wall -DACK_43 $(PROF) $(SOLARIS_FLAG)
L_FLAGS = -O -g2 -lcrypt $(PROF) $(SOLARIS_LINK)

O_FILES = act_clan.o act_comm.o act_info.o act_mob.o act_move.o act_obj.o \
	  act_wiz.o ai.o areachk.o areasave.o board.o build.o buildare.o \
	  buildtab.o cloak.o clutch.o comm.o const.o damage.o db.o death.o \
	  email.o enchant.o fight.o handler.o hash.o hunt.o interp.o invasion.o \
	  item_generation.o item_generation_tables.o keep.o lists.o macros.o magic.o magic2.o magic3.o \
	  magic4.o mapper.o mob_commands.o mob_prog.o obj_fun.o \
	  pdelete.o quest.o crusade.o reincarnate.o rulers.o save.o \
	  scheck.o skills.o skills_chi.o skills_combo.o skills_obj.o \
	  skills_poison.o social-edit.o special.o spell_dam.o spendqp.o ssm.o \
	  stance.o strfuns.o sysdata.o tables.o trigger.o update.o vampyre.o \
	  werewolf.o wizutil.o write.o
AI_SPEC_OBJS = \
	  ai/spec_summon_water.o ai/spec_summon_fire.o ai/spec_summon_earth.o \
	  ai/spec_summon_undead.o ai/spec_summon_holy.o ai/spec_summon_shadow.o \
	  ai/spec_summon_metal.o ai/spec_summon_animate.o ai/spec_summon_thought.o \
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
	  ai/spec_lay_sister.o ai/spec_laborer.o ai/spec_keep_physical_captain.o \
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
	  ai/spec_rr_camp_cook.o ai/spec_rr_pilgrim.o ai/spec_rr_ruin_scavenger.o

O_FILES = ai.o act_clan.o act_comm.o act_info.o act_mob.o act_move.o act_obj.o act_wiz.o \
	  areachk.o areasave.o board.o build.o buildare.o buildtab.o caravan.o clutch.o comm.o \
	  const.o damage.o cloak.o death.o db.o email.o enchant.o fight.o handler.o hash.o hunt.o \
	  interp.o invasion.o item_generation.o item_generation_tables.o keep.o lists.o macros.o magic.o magic2.o magic3.o magic4.o mapper.o \
	  mob_commands.o mob_prog.o obj_fun.o pdelete.o quest.o crusade.o reincarnate.o rulers.o \
	  save.o scheck.o skills.o skills_chi.o skills_combo.o skills_obj.o skills_poison.o social-edit.o special.o spell_dam.o spendqp.o ssm.o \
	  stance.o strfuns.o sysdata.o tables.o trigger.o update.o vampyre.o werewolf.o wizutil.o \
	  write.o $(AI_SPEC_OBJS)

.PHONY: all clean purge unit-tests integration-test web

all:
	$(MAKE) clean
	$(MAKE) web
	$(MAKE) ack
	$(MAKE) unit-tests

integration-test: ack
	cd .. && ./integration-test.sh

web:
	$(MAKE) -C ../web

ifdef CYGWIN
ack: $(O_FILES)
	rm -f ack.exe
	$(CC) -o ack.exe $(O_FILES) $(L_FLAGS)
	chmod g+w ack.exe
	chmod a+x ack.exe
	chmod g+w $(O_FILES)
else
ack: $(O_FILES)
	rm -f ack
	$(CC) -o ack $(O_FILES) $(L_FLAGS)
	chmod g+w ack
	chmod a+x ack
	chmod g+w $(O_FILES)
endif

clean:
	rm -f *.o ai/*.o tests/*.o tests/unit-test-* ack ack.exe ../area/core *~
	$(MAKE) -C ../web clean

purge:
	rm -f *.o ai/*.o tests/*.o tests/unit-test-* ack ack.exe ../area/core *~
	$(MAKE) -C ../web clean

%.o: %.c ack.h
	$(CC) -c $(C_FLAGS) $<

ai/%.o: ai/%.c ack.h
	$(CC) -c $(C_FLAGS) -I. -o $@ $<

tests/%.o: tests/%.c ack.h
	$(CC) -c $(C_FLAGS) -I. -o $@ $<

scan: scan.o
	rm -f scan
	$(CC) $(L_FLAGS) -o scan scan.o


unit-test-sha256: tests/test_sha256.o sha256.o tests/test_is_fighting.o
	rm -f tests/unit-test-sha256
	$(CC) -o tests/unit-test-sha256 tests/test_sha256.o sha256.o tests/test_is_fighting.o $(L_FLAGS)

update.unit-test.o: update.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_UPDATE -o update.unit-test.o update.c

unit-test-update: tests/test_update.o update.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-update
	$(CC) -o tests/unit-test-update tests/test_update.o update.unit-test.o tests/test_is_fighting.o $(L_FLAGS)


comm.unit-test.o: comm.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_COMM -o comm.unit-test.o comm.c

unit-test-comm: tests/test_comm.o comm.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-comm
	$(CC) -o tests/unit-test-comm tests/test_comm.o comm.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

unit-test-websocket-validation: tests/test_websocket_validation.o
	rm -f tests/unit-test-websocket-validation
	$(CC) -o tests/unit-test-websocket-validation tests/test_websocket_validation.o $(L_FLAGS)

fight.unit-test.o: fight.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o fight.unit-test.o fight.c

unit-test-fight: tests/test_fight.o fight.unit-test.o cloak.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-fight
	$(CC) -Wl,--gc-sections -o tests/unit-test-fight tests/test_fight.o fight.unit-test.o cloak.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

act_info.unit-test.o: act_info.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o act_info.unit-test.o act_info.c

unit-test-act-info: tests/test_act_info.o act_info.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-act-info
	$(CC) -Wl,--gc-sections -o tests/unit-test-act-info tests/test_act_info.o act_info.unit-test.o tests/test_is_fighting.o $(L_FLAGS)


act_move.unit-test.o: act_move.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o act_move.unit-test.o act_move.c

unit-test-act-move: tests/test_act_move.o act_move.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-act-move
	$(CC) -Wl,--gc-sections -o tests/unit-test-act-move tests/test_act_move.o act_move.unit-test.o tests/test_is_fighting.o $(L_FLAGS)


cloak.unit-test.o: cloak.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_CLOAK -o cloak.unit-test.o cloak.c

unit-test-cloak: tests/test_cloak.o cloak.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-cloak
	$(CC) -o tests/unit-test-cloak tests/test_cloak.o cloak.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

spendqp.unit-test.o: spendqp.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SPENDQP -o spendqp.unit-test.o spendqp.c

unit-test-spendqp: tests/test_spendqp.o spendqp.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-spendqp
	$(CC) -o tests/unit-test-spendqp tests/test_spendqp.o spendqp.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

skills_chi.unit-test.o: skills_chi.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o skills_chi.unit-test.o skills_chi.c

unit-test-skills-chi: tests/test_skills_chi.o skills_chi.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-skills-chi
	$(CC) -Wl,--gc-sections -o tests/unit-test-skills-chi tests/test_skills_chi.o skills_chi.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

spell_dam.unit-test.o: spell_dam.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o spell_dam.unit-test.o spell_dam.c

unit-test-spell-dam: tests/test_spell_dam.o spell_dam.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-spell-dam
	$(CC) -Wl,--gc-sections -o tests/unit-test-spell-dam tests/test_spell_dam.o spell_dam.unit-test.o tests/test_is_fighting.o $(L_FLAGS)


pdelete.unit-test.o: pdelete.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o pdelete.unit-test.o pdelete.c

unit-test-pdelete: tests/test_pdelete.o pdelete.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-pdelete
	$(CC) -Wl,--gc-sections -o tests/unit-test-pdelete tests/test_pdelete.o pdelete.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

email.unit-test.o: email.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_EMAIL -o email.unit-test.o email.c

unit-test-email: tests/test_email.o email.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-email
	$(CC) -o tests/unit-test-email tests/test_email.o email.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

rulers.unit-test.o: rulers.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_RULERS -o rulers.unit-test.o rulers.c

unit-test-rulers: tests/test_rulers.o rulers.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-rulers
	$(CC) -o tests/unit-test-rulers tests/test_rulers.o rulers.unit-test.o tests/test_is_fighting.o $(L_FLAGS)


save.unit-test.o: save.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SAVE -ffunction-sections -fdata-sections -o save.unit-test.o save.c

unit-test-save: tests/test_save.o save.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-save
	$(CC) -Wl,--gc-sections -o tests/unit-test-save tests/test_save.o save.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

skills_obj.unit-test.o: skills_obj.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o skills_obj.unit-test.o skills_obj.c

unit-test-skills-obj: tests/test_skills_obj.o skills_obj.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-skills-obj
	$(CC) -Wl,--gc-sections -o tests/unit-test-skills-obj tests/test_skills_obj.o skills_obj.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

skills_combo.unit-test.o: skills_combo.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o skills_combo.unit-test.o skills_combo.c

unit-test-skills-combo: tests/test_skills_combo.o skills_combo.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-skills-combo
	$(CC) -Wl,--gc-sections -o tests/unit-test-skills-combo tests/test_skills_combo.o skills_combo.unit-test.o tests/test_is_fighting.o $(L_FLAGS)


reincarnate.unit-test.o: reincarnate.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o reincarnate.unit-test.o reincarnate.c

unit-test-reincarnate: tests/test_reincarnate.o reincarnate.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-reincarnate
	$(CC) -Wl,--gc-sections -o tests/unit-test-reincarnate tests/test_reincarnate.o reincarnate.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

mob_commands.unit-test.o: mob_commands.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o mob_commands.unit-test.o mob_commands.c

unit-test-mob-commands: tests/test_mob_commands.o mob_commands.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-mob-commands
	$(CC) -Wl,--gc-sections -o tests/unit-test-mob-commands tests/test_mob_commands.o mob_commands.unit-test.o tests/test_is_fighting.o $(L_FLAGS)




ssm.unit-test.o: ssm.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SSM -o ssm.unit-test.o ssm.c

unit-test-ssm: tests/test_ssm.o ssm.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-ssm
	$(CC) -o tests/unit-test-ssm tests/test_ssm.o ssm.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

db.unit-test.o: db.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_DB -ffunction-sections -fdata-sections -o db.unit-test.o db.c

unit-test-db: tests/test_db.o db.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-db
	$(CC) -Wl,--gc-sections -o tests/unit-test-db tests/test_db.o db.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

magic.unit-test.o: magic.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_MAGIC -ffunction-sections -fdata-sections -o magic.unit-test.o magic.c

unit-test-magic: tests/test_magic.o magic.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-magic
	$(CC) -Wl,--gc-sections -o tests/unit-test-magic tests/test_magic.o magic.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

magic4.unit-test.o: magic4.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o magic4.unit-test.o magic4.c

unit-test-magic4: tests/test_magic4.o magic4.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-magic4
	$(CC) -Wl,--gc-sections -o tests/unit-test-magic4 tests/test_magic4.o magic4.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

mapper.unit-test.o: mapper.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_MAPPER -ffunction-sections -fdata-sections -o mapper.unit-test.o mapper.c

unit-test-mapper: tests/test_mapper.o mapper.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-mapper
	$(CC) -Wl,--gc-sections -o tests/unit-test-mapper tests/test_mapper.o mapper.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

damage.unit-test.o: damage.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_DAMAGE -ffunction-sections -fdata-sections -o damage.unit-test.o damage.c

unit-test-damage: tests/test_damage.o damage.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-damage
	$(CC) -Wl,--gc-sections -o tests/unit-test-damage tests/test_damage.o damage.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

buildare.unit-test.o: buildare.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_BUILDARE -ffunction-sections -fdata-sections -o buildare.unit-test.o buildare.c

unit-test-buildare: tests/test_buildare.o buildare.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-buildare
	$(CC) -Wl,--gc-sections -o tests/unit-test-buildare tests/test_buildare.o buildare.unit-test.o tests/test_is_fighting.o $(L_FLAGS)


keep.unit-test.o: keep.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_KEEP -ffunction-sections -fdata-sections -o keep.unit-test.o keep.c

unit-test-keep: tests/test_keep.o keep.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-keep
	$(CC) -Wl,--gc-sections -o tests/unit-test-keep tests/test_keep.o keep.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

act_obj.unit-test.o: act_obj.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_ACT_OBJ -ffunction-sections -fdata-sections -o act_obj.unit-test.o act_obj.c

unit-test-act-obj: tests/test_act_obj.o act_obj.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-act-obj
	$(CC) -Wl,--gc-sections -o tests/unit-test-act-obj tests/test_act_obj.o act_obj.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

quest.unit-test.o: quest.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_QUEST -ffunction-sections -fdata-sections -o quest.unit-test.o quest.c

unit-test-quest: tests/test_quest.o quest.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-quest
	$(CC) -Wl,--gc-sections -o tests/unit-test-quest tests/test_quest.o quest.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

build.unit-test.o: build.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_BUILD -ffunction-sections -fdata-sections -o build.unit-test.o build.c

unit-test-build: tests/test_build.o build.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-build
	$(CC) -Wl,--gc-sections -o tests/unit-test-build tests/test_build.o build.unit-test.o tests/test_is_fighting.o $(L_FLAGS)


invasion.unit-test.o: invasion.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_INVASION -ffunction-sections -fdata-sections -o invasion.unit-test.o invasion.c

unit-test-invasion: tests/test_invasion.o invasion.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-invasion
	$(CC) -Wl,--gc-sections -o tests/unit-test-invasion tests/test_invasion.o invasion.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

AI_SUMMON_OBJS = ai/spec_summon_water.o ai/spec_summon_fire.o ai/spec_summon_earth.o \
	  ai/spec_summon_undead.o ai/spec_summon_holy.o ai/spec_summon_shadow.o \
	  ai/spec_summon_metal.o ai/spec_summon_animate.o ai/spec_summon_thought.o

special.unit-test.o: special.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SPECIAL -ffunction-sections -fdata-sections -o special.unit-test.o special.c

unit-test-special: tests/test_special.o special.unit-test.o $(AI_SUMMON_OBJS) tests/test_is_fighting.o
	rm -f tests/unit-test-special
	$(CC) -Wl,--gc-sections -o tests/unit-test-special tests/test_special.o special.unit-test.o $(AI_SUMMON_OBJS) tests/test_is_fighting.o $(L_FLAGS)



item_generation.unit-test.o: item_generation.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_ITEM_GENERATION -ffunction-sections -fdata-sections -o item_generation.unit-test.o item_generation.c

unit-test-item-generation: tests/test_item_generation.o item_generation.unit-test.o item_generation_tables.o tests/test_is_fighting.o
	rm -f tests/unit-test-item-generation
	$(CC) -Wl,--gc-sections -o tests/unit-test-item-generation tests/test_item_generation.o item_generation.unit-test.o item_generation_tables.o tests/test_is_fighting.o $(L_FLAGS)

unit-test-act-flags: tests/test_act_flags.o
	rm -f tests/unit-test-act-flags
	$(CC) -o tests/unit-test-act-flags tests/test_act_flags.o $(L_FLAGS)

unit-test-area-format: tests/test_area_format.o
	rm -f tests/unit-test-area-format
	$(CC) -o tests/unit-test-area-format tests/test_area_format.o $(L_FLAGS)

unit-test-help-format: tests/test_help_format.o
	rm -f tests/unit-test-help-format
	$(CC) -o tests/unit-test-help-format tests/test_help_format.o $(L_FLAGS)

interp.unit-test.o: interp.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o interp.unit-test.o interp.c

unit-test-interp: tests/test_interp.o interp.unit-test.o
	rm -f tests/unit-test-interp
	$(CC) -Wl,--gc-sections -Wl,--unresolved-symbols=ignore-all -o tests/unit-test-interp tests/test_interp.o interp.unit-test.o $(L_FLAGS)

unit-tests: unit-test-act-flags unit-test-area-format unit-test-help-format unit-test-sha256 unit-test-update unit-test-comm unit-test-websocket-validation unit-test-fight unit-test-act-info unit-test-act-move unit-test-cloak unit-test-spendqp unit-test-spell-dam unit-test-email unit-test-pdelete unit-test-rulers unit-test-save unit-test-skills-obj unit-test-skills-combo unit-test-reincarnate unit-test-mob-commands unit-test-db unit-test-magic unit-test-magic4 unit-test-mapper unit-test-damage unit-test-buildare unit-test-build unit-test-invasion unit-test-quest unit-test-keep unit-test-act-obj unit-test-ssm unit-test-special unit-test-crusade unit-test-death unit-test-item-generation unit-test-interp
	./tests/unit-test-act-flags
	./tests/unit-test-area-format
	./tests/unit-test-help-format
	./tests/unit-test-sha256
	./tests/unit-test-update
	./tests/unit-test-comm
	./tests/unit-test-websocket-validation
	./tests/unit-test-fight
	./tests/unit-test-act-info
	./tests/unit-test-act-move
	./tests/unit-test-cloak
	./tests/unit-test-spendqp
	./tests/unit-test-spell-dam
	./tests/unit-test-email
	./tests/unit-test-pdelete
	./tests/unit-test-rulers
	./tests/unit-test-save
	./tests/unit-test-skills-obj
	./tests/unit-test-skills-combo
	./tests/unit-test-reincarnate
	./tests/unit-test-mob-commands
	./tests/unit-test-db
	./tests/unit-test-magic
	./tests/unit-test-magic4
	./tests/unit-test-mapper
	./tests/unit-test-damage
	./tests/unit-test-buildare
	./tests/unit-test-build
	./tests/unit-test-invasion
	./tests/unit-test-quest
	./tests/unit-test-keep
	./tests/unit-test-act-obj
	./tests/unit-test-ssm
	./tests/unit-test-special
	./tests/unit-test-crusade
	./tests/unit-test-death
	./tests/unit-test-item-generation
	./tests/unit-test-interp
	$(MAKE) integration-test


crusade.unit-test.o: crusade.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o crusade.unit-test.o crusade.c

unit-test-crusade: tests/test_crusade.o crusade.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-crusade
	$(CC) -Wl,--gc-sections -o tests/unit-test-crusade tests/test_crusade.o crusade.unit-test.o tests/test_is_fighting.o $(L_FLAGS)

death.unit-test.o: death.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o death.unit-test.o death.c

unit-test-death: tests/test_death.o death.unit-test.o tests/test_is_fighting.o
	rm -f tests/unit-test-death
	$(CC) -Wl,--gc-sections -o tests/unit-test-death tests/test_death.o death.unit-test.o tests/test_is_fighting.o $(L_FLAGS)
