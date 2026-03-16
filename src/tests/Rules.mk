$(OBJDIR)/tests/%.o: tests/%.c headers/ack.h
	@mkdir -p $(dir $@)
	$(CC) -c $(C_FLAGS) -I. -o $@ $<

unit-test-sha256: $(OBJDIR)/tests/test_sha256.o $(OBJDIR)/sha256.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-sha256
	$(CC) -o tests/unit-test-sha256 $(OBJDIR)/tests/test_sha256.o $(OBJDIR)/sha256.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/update.unit-test.o: update.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_UPDATE -o $(OBJDIR)/update.unit-test.o update.c

unit-test-update: $(OBJDIR)/tests/test_update.o $(OBJDIR)/update.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-update
	$(CC) -o tests/unit-test-update $(OBJDIR)/tests/test_update.o $(OBJDIR)/update.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)


$(OBJDIR)/comm.unit-test.o: comm.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_COMM -o $(OBJDIR)/comm.unit-test.o comm.c

unit-test-comm: $(OBJDIR)/tests/test_comm.o $(OBJDIR)/comm.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-comm
	$(CC) -o tests/unit-test-comm $(OBJDIR)/tests/test_comm.o $(OBJDIR)/comm.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

unit-test-websocket-validation: $(OBJDIR)/tests/test_websocket_validation.o
	rm -f tests/unit-test-websocket-validation
	$(CC) -o tests/unit-test-websocket-validation $(OBJDIR)/tests/test_websocket_validation.o $(L_FLAGS)

$(OBJDIR)/fight.unit-test.o: fight.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/fight.unit-test.o fight.c

unit-test-fight: $(OBJDIR)/tests/test_fight.o $(OBJDIR)/fight.unit-test.o $(OBJDIR)/cloak.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-fight
	$(CC) -Wl,--gc-sections -o tests/unit-test-fight $(OBJDIR)/tests/test_fight.o $(OBJDIR)/fight.unit-test.o $(OBJDIR)/cloak.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/act_info.unit-test.o: act_info.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/act_info.unit-test.o act_info.c

unit-test-act-info: $(OBJDIR)/tests/test_act_info.o $(OBJDIR)/act_info.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-act-info
	$(CC) -Wl,--gc-sections -o tests/unit-test-act-info $(OBJDIR)/tests/test_act_info.o $(OBJDIR)/act_info.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)


$(OBJDIR)/act_move.unit-test.o: act_move.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/act_move.unit-test.o act_move.c

unit-test-act-move: $(OBJDIR)/tests/test_act_move.o $(OBJDIR)/act_move.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-act-move
	$(CC) -Wl,--gc-sections -o tests/unit-test-act-move $(OBJDIR)/tests/test_act_move.o $(OBJDIR)/act_move.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)


$(OBJDIR)/cloak.unit-test.o: cloak.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_CLOAK -o $(OBJDIR)/cloak.unit-test.o cloak.c

unit-test-cloak: $(OBJDIR)/tests/test_cloak.o $(OBJDIR)/cloak.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-cloak
	$(CC) -o tests/unit-test-cloak $(OBJDIR)/tests/test_cloak.o $(OBJDIR)/cloak.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/spendqp.unit-test.o: spendqp.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SPENDQP -o $(OBJDIR)/spendqp.unit-test.o spendqp.c

unit-test-spendqp: $(OBJDIR)/tests/test_spendqp.o $(OBJDIR)/spendqp.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-spendqp
	$(CC) -o tests/unit-test-spendqp $(OBJDIR)/tests/test_spendqp.o $(OBJDIR)/spendqp.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/skills_chi.unit-test.o: skills_chi.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/skills_chi.unit-test.o skills_chi.c

unit-test-skills-chi: $(OBJDIR)/tests/test_skills_chi.o $(OBJDIR)/skills_chi.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-skills-chi
	$(CC) -Wl,--gc-sections -o tests/unit-test-skills-chi $(OBJDIR)/tests/test_skills_chi.o $(OBJDIR)/skills_chi.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/spell_dam.unit-test.o: spell_dam.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/spell_dam.unit-test.o spell_dam.c

unit-test-spell-dam: $(OBJDIR)/tests/test_spell_dam.o $(OBJDIR)/spell_dam.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-spell-dam
	$(CC) -Wl,--gc-sections -o tests/unit-test-spell-dam $(OBJDIR)/tests/test_spell_dam.o $(OBJDIR)/spell_dam.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)


$(OBJDIR)/pdelete.unit-test.o: pdelete.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/pdelete.unit-test.o pdelete.c

unit-test-pdelete: $(OBJDIR)/tests/test_pdelete.o $(OBJDIR)/pdelete.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-pdelete
	$(CC) -Wl,--gc-sections -o tests/unit-test-pdelete $(OBJDIR)/tests/test_pdelete.o $(OBJDIR)/pdelete.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/email.unit-test.o: email.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_EMAIL -o $(OBJDIR)/email.unit-test.o email.c

unit-test-email: $(OBJDIR)/tests/test_email.o $(OBJDIR)/email.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-email
	$(CC) -o tests/unit-test-email $(OBJDIR)/tests/test_email.o $(OBJDIR)/email.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/rulers.unit-test.o: rulers.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_RULERS -o $(OBJDIR)/rulers.unit-test.o rulers.c

unit-test-rulers: $(OBJDIR)/tests/test_rulers.o $(OBJDIR)/rulers.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-rulers
	$(CC) -o tests/unit-test-rulers $(OBJDIR)/tests/test_rulers.o $(OBJDIR)/rulers.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)


$(OBJDIR)/save.unit-test.o: save.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SAVE -ffunction-sections -fdata-sections -o $(OBJDIR)/save.unit-test.o save.c

unit-test-save: $(OBJDIR)/tests/test_save.o $(OBJDIR)/save.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-save
	$(CC) -Wl,--gc-sections -o tests/unit-test-save $(OBJDIR)/tests/test_save.o $(OBJDIR)/save.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/skills_obj.unit-test.o: skills_obj.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/skills_obj.unit-test.o skills_obj.c

unit-test-skills-obj: $(OBJDIR)/tests/test_skills_obj.o $(OBJDIR)/skills_obj.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-skills-obj
	$(CC) -Wl,--gc-sections -o tests/unit-test-skills-obj $(OBJDIR)/tests/test_skills_obj.o $(OBJDIR)/skills_obj.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/skills_combo.unit-test.o: skills_combo.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/skills_combo.unit-test.o skills_combo.c

unit-test-skills-combo: $(OBJDIR)/tests/test_skills_combo.o $(OBJDIR)/skills_combo.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-skills-combo
	$(CC) -Wl,--gc-sections -o tests/unit-test-skills-combo $(OBJDIR)/tests/test_skills_combo.o $(OBJDIR)/skills_combo.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)


$(OBJDIR)/reincarnate.unit-test.o: reincarnate.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/reincarnate.unit-test.o reincarnate.c

unit-test-reincarnate: $(OBJDIR)/tests/test_reincarnate.o $(OBJDIR)/reincarnate.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-reincarnate
	$(CC) -Wl,--gc-sections -o tests/unit-test-reincarnate $(OBJDIR)/tests/test_reincarnate.o $(OBJDIR)/reincarnate.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/ssm.unit-test.o: ssm.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SSM -o $(OBJDIR)/ssm.unit-test.o ssm.c

unit-test-ssm: $(OBJDIR)/tests/test_ssm.o $(OBJDIR)/ssm.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-ssm
	$(CC) -o tests/unit-test-ssm $(OBJDIR)/tests/test_ssm.o $(OBJDIR)/ssm.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/db.unit-test.o: db.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_DB -ffunction-sections -fdata-sections -o $(OBJDIR)/db.unit-test.o db.c

unit-test-db: $(OBJDIR)/tests/test_db.o $(OBJDIR)/db.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-db
	$(CC) -Wl,--gc-sections -o tests/unit-test-db $(OBJDIR)/tests/test_db.o $(OBJDIR)/db.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/magic.unit-test.o: magic.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_MAGIC -ffunction-sections -fdata-sections -o $(OBJDIR)/magic.unit-test.o magic.c

unit-test-magic: $(OBJDIR)/tests/test_magic.o $(OBJDIR)/magic.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-magic
	$(CC) -Wl,--gc-sections -o tests/unit-test-magic $(OBJDIR)/tests/test_magic.o $(OBJDIR)/magic.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/magic2.unit-test.o: magic2.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/magic2.unit-test.o magic2.c

unit-test-magic2: $(OBJDIR)/tests/test_magic2.o $(OBJDIR)/magic2.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-magic2
	$(CC) -Wl,--gc-sections -o tests/unit-test-magic2 $(OBJDIR)/tests/test_magic2.o $(OBJDIR)/magic2.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/magic4.unit-test.o: magic4.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/magic4.unit-test.o magic4.c

unit-test-magic4: $(OBJDIR)/tests/test_magic4.o $(OBJDIR)/magic4.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-magic4
	$(CC) -Wl,--gc-sections -o tests/unit-test-magic4 $(OBJDIR)/tests/test_magic4.o $(OBJDIR)/magic4.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/mapper.unit-test.o: mapper.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_MAPPER -ffunction-sections -fdata-sections -o $(OBJDIR)/mapper.unit-test.o mapper.c

unit-test-mapper: $(OBJDIR)/tests/test_mapper.o $(OBJDIR)/mapper.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-mapper
	$(CC) -Wl,--gc-sections -o tests/unit-test-mapper $(OBJDIR)/tests/test_mapper.o $(OBJDIR)/mapper.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/damage.unit-test.o: damage.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_DAMAGE -ffunction-sections -fdata-sections -o $(OBJDIR)/damage.unit-test.o damage.c

unit-test-damage: $(OBJDIR)/tests/test_damage.o $(OBJDIR)/damage.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-damage
	$(CC) -Wl,--gc-sections -o tests/unit-test-damage $(OBJDIR)/tests/test_damage.o $(OBJDIR)/damage.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/buildare.unit-test.o: buildare.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_BUILDARE -ffunction-sections -fdata-sections -o $(OBJDIR)/buildare.unit-test.o buildare.c

unit-test-buildare: $(OBJDIR)/tests/test_buildare.o $(OBJDIR)/buildare.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-buildare
	$(CC) -Wl,--gc-sections -o tests/unit-test-buildare $(OBJDIR)/tests/test_buildare.o $(OBJDIR)/buildare.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)


$(OBJDIR)/keep.unit-test.o: keep.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_KEEP -ffunction-sections -fdata-sections -o $(OBJDIR)/keep.unit-test.o keep.c

unit-test-keep: $(OBJDIR)/tests/test_keep.o $(OBJDIR)/keep.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-keep
	$(CC) -Wl,--gc-sections -o tests/unit-test-keep $(OBJDIR)/tests/test_keep.o $(OBJDIR)/keep.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/act_obj.unit-test.o: act_obj.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_ACT_OBJ -ffunction-sections -fdata-sections -o $(OBJDIR)/act_obj.unit-test.o act_obj.c

unit-test-act-obj: $(OBJDIR)/tests/test_act_obj.o $(OBJDIR)/act_obj.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-act-obj
	$(CC) -Wl,--gc-sections -o tests/unit-test-act-obj $(OBJDIR)/tests/test_act_obj.o $(OBJDIR)/act_obj.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/quest.unit-test.o: quest.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_QUEST -ffunction-sections -fdata-sections -o $(OBJDIR)/quest.unit-test.o quest.c

unit-test-quest: $(OBJDIR)/tests/test_quest.o $(OBJDIR)/quest.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-quest
	$(CC) -Wl,--gc-sections -o tests/unit-test-quest $(OBJDIR)/tests/test_quest.o $(OBJDIR)/quest.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/build.unit-test.o: build.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_BUILD -ffunction-sections -fdata-sections -o $(OBJDIR)/build.unit-test.o build.c

unit-test-build: $(OBJDIR)/tests/test_build.o $(OBJDIR)/build.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-build
	$(CC) -Wl,--gc-sections -o tests/unit-test-build $(OBJDIR)/tests/test_build.o $(OBJDIR)/build.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)


$(OBJDIR)/invasion.unit-test.o: invasion.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_INVASION -ffunction-sections -fdata-sections -o $(OBJDIR)/invasion.unit-test.o invasion.c

unit-test-invasion: $(OBJDIR)/tests/test_invasion.o $(OBJDIR)/invasion.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-invasion
	$(CC) -Wl,--gc-sections -o tests/unit-test-invasion $(OBJDIR)/tests/test_invasion.o $(OBJDIR)/invasion.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/special.unit-test.o: special.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SPECIAL -ffunction-sections -fdata-sections -o $(OBJDIR)/special.unit-test.o special.c

unit-test-special: $(OBJDIR)/tests/test_special.o $(OBJDIR)/special.unit-test.o $(AI_SUMMON_OBJS) $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-special
	$(CC) -Wl,--gc-sections -o tests/unit-test-special $(OBJDIR)/tests/test_special.o $(OBJDIR)/special.unit-test.o $(AI_SUMMON_OBJS) $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)



$(OBJDIR)/item_generation.unit-test.o: item_generation.c headers/ack.h
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_ITEM_GENERATION -ffunction-sections -fdata-sections -o $(OBJDIR)/item_generation.unit-test.o item_generation.c

unit-test-item-generation: $(OBJDIR)/tests/test_item_generation.o $(OBJDIR)/item_generation.unit-test.o $(OBJDIR)/item_generation_tables.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-item-generation
	$(CC) -Wl,--gc-sections -o tests/unit-test-item-generation $(OBJDIR)/tests/test_item_generation.o $(OBJDIR)/item_generation.unit-test.o $(OBJDIR)/item_generation_tables.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

unit-test-act-flags: $(OBJDIR)/tests/test_act_flags.o
	rm -f tests/unit-test-act-flags
	$(CC) -o tests/unit-test-act-flags $(OBJDIR)/tests/test_act_flags.o $(L_FLAGS)

unit-test-area-format: $(OBJDIR)/tests/test_area_format.o
	rm -f tests/unit-test-area-format
	$(CC) -o tests/unit-test-area-format $(OBJDIR)/tests/test_area_format.o $(L_FLAGS)

unit-test-help-format: $(OBJDIR)/tests/test_help_format.o
	rm -f tests/unit-test-help-format
	$(CC) -o tests/unit-test-help-format $(OBJDIR)/tests/test_help_format.o $(L_FLAGS)

$(OBJDIR)/interp.unit-test.o: interp.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/interp.unit-test.o interp.c

unit-test-interp: $(OBJDIR)/tests/test_interp.o $(OBJDIR)/interp.unit-test.o
	rm -f tests/unit-test-interp
	$(CC) -Wl,--gc-sections -Wl,--unresolved-symbols=ignore-all -o tests/unit-test-interp $(OBJDIR)/tests/test_interp.o $(OBJDIR)/interp.unit-test.o $(L_FLAGS)

$(OBJDIR)/crusade.unit-test.o: crusade.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/crusade.unit-test.o crusade.c

unit-test-crusade: $(OBJDIR)/tests/test_crusade.o $(OBJDIR)/crusade.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-crusade
	$(CC) -Wl,--gc-sections -o tests/unit-test-crusade $(OBJDIR)/tests/test_crusade.o $(OBJDIR)/crusade.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/death.unit-test.o: death.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/death.unit-test.o death.c

unit-test-death: $(OBJDIR)/tests/test_death.o $(OBJDIR)/death.unit-test.o $(OBJDIR)/tests/test_is_fighting.o
	rm -f tests/unit-test-death
	$(CC) -Wl,--gc-sections -o tests/unit-test-death $(OBJDIR)/tests/test_death.o $(OBJDIR)/death.unit-test.o $(OBJDIR)/tests/test_is_fighting.o $(L_FLAGS)

$(OBJDIR)/strfuns.unit-test.o: strfuns.c headers/ack.h
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o $(OBJDIR)/strfuns.unit-test.o strfuns.c

unit-test-strfuns: $(OBJDIR)/tests/test_strfuns.o $(OBJDIR)/strfuns.unit-test.o
	rm -f tests/unit-test-strfuns
	$(CC) -Wl,--gc-sections -o tests/unit-test-strfuns $(OBJDIR)/tests/test_strfuns.o $(OBJDIR)/strfuns.unit-test.o $(L_FLAGS)

unit-tests: unit-test-act-flags unit-test-area-format unit-test-help-format unit-test-sha256 unit-test-update unit-test-comm unit-test-websocket-validation unit-test-fight unit-test-act-info unit-test-act-move unit-test-cloak unit-test-spendqp unit-test-spell-dam unit-test-email unit-test-pdelete unit-test-rulers unit-test-save unit-test-skills-obj unit-test-skills-combo unit-test-reincarnate unit-test-db unit-test-magic unit-test-magic2 unit-test-magic4 unit-test-mapper unit-test-damage unit-test-buildare unit-test-build unit-test-invasion unit-test-quest unit-test-keep unit-test-act-obj unit-test-ssm unit-test-special unit-test-crusade unit-test-death unit-test-item-generation unit-test-interp unit-test-strfuns
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
	./tests/unit-test-db
	./tests/unit-test-magic
	./tests/unit-test-magic2
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
	./tests/unit-test-strfuns
	$(MAKE) integration-test
	$(MAKE) integration-test-telnet
