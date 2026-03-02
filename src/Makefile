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
	  item_generation.o keep.o lists.o macros.o magic.o magic2.o magic3.o \
	  magic4.o mapper.o mob_commands.o mob_prog.o mount.o obj_fun.o \
	  pdelete.o proposition.o quest.o reincarnate.o rulers.o save.o \
	  scheck.o skills.o skills_chi.o skills_combo.o skills_obj.o \
	  skills_poison.o social-edit.o special.o spell_dam.o spendqp.o ssm.o \
	  stance.o strfuns.o sysdata.o tables.o trigger.o update.o vampyre.o \
	  werewolf.o wizutil.o write.o
O_FILES = ai.o act_clan.o act_comm.o act_info.o act_mob.o act_move.o act_obj.o act_wiz.o \
	  areachk.o areasave.o board.o build.o buildare.o buildtab.o clutch.o comm.o \
	  const.o damage.o cloak.o death.o db.o email.o enchant.o fight.o handler.o hash.o hunt.o \
	  interp.o invasion.o item_generation.o keep.o lists.o macros.o magic.o magic2.o magic3.o magic4.o mapper.o \
	  mob_commands.o mob_prog.o mount.o obj_fun.o pdelete.o proposition.o quest.o reincarnate.o rulers.o \
	  save.o scheck.o skills.o skills_chi.o skills_combo.o skills_obj.o skills_poison.o social-edit.o special.o spell_dam.o spendqp.o ssm.o \
	  stance.o strfuns.o sysdata.o tables.o trigger.o update.o vampyre.o werewolf.o wizutil.o \
	  write.o

.PHONY: all clean purge unit-tests

all:
	$(MAKE) clean
	$(MAKE) ack
	$(MAKE) unit-tests

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
	rm -f *.o ack ack.exe ../area/core *~

purge:
	rm -f *.o ack ack.exe ../area/core *~

%.o: %.c ack.h
	$(CC) -c $(C_FLAGS) $<

scan: scan.o
	rm -f scan
	$(CC) $(L_FLAGS) -o scan scan.o


unit-test-sha256: test_sha256.o sha256.o
	rm -f unit-test-sha256
	$(CC) -o unit-test-sha256 test_sha256.o sha256.o $(L_FLAGS)

update.unit-test.o: update.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_UPDATE -o update.unit-test.o update.c

unit-test-update: test_update.o update.unit-test.o
	rm -f unit-test-update
	$(CC) -o unit-test-update test_update.o update.unit-test.o $(L_FLAGS)


comm.unit-test.o: comm.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_COMM -o comm.unit-test.o comm.c

unit-test-comm: test_comm.o comm.unit-test.o
	rm -f unit-test-comm
	$(CC) -o unit-test-comm test_comm.o comm.unit-test.o $(L_FLAGS)

fight.unit-test.o: fight.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o fight.unit-test.o fight.c

unit-test-fight: test_fight.o fight.unit-test.o cloak.unit-test.o
	rm -f unit-test-fight
	$(CC) -Wl,--gc-sections -o unit-test-fight test_fight.o fight.unit-test.o cloak.unit-test.o $(L_FLAGS)

act_info.unit-test.o: act_info.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o act_info.unit-test.o act_info.c

unit-test-act-info: test_act_info.o act_info.unit-test.o
	rm -f unit-test-act-info
	$(CC) -Wl,--gc-sections -o unit-test-act-info test_act_info.o act_info.unit-test.o $(L_FLAGS)


cloak.unit-test.o: cloak.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_CLOAK -o cloak.unit-test.o cloak.c

unit-test-cloak: test_cloak.o cloak.unit-test.o
	rm -f unit-test-cloak
	$(CC) -o unit-test-cloak test_cloak.o cloak.unit-test.o $(L_FLAGS)

spendqp.unit-test.o: spendqp.c
	$(CC) -c $(C_FLAGS) -DUNIT_TEST_SPENDQP -ffunction-sections -fdata-sections -o spendqp.unit-test.o spendqp.c

unit-test-spendqp: test_spendqp.o spendqp.unit-test.o
	rm -f unit-test-spendqp
	$(CC) -Wl,--gc-sections -o unit-test-spendqp test_spendqp.o spendqp.unit-test.o $(L_FLAGS)

spell_dam.unit-test.o: spell_dam.c
	$(CC) -c $(C_FLAGS) -ffunction-sections -fdata-sections -o spell_dam.unit-test.o spell_dam.c

unit-test-spell-dam: test_spell_dam.o spell_dam.unit-test.o
	rm -f unit-test-spell-dam
	$(CC) -Wl,--gc-sections -o unit-test-spell-dam test_spell_dam.o spell_dam.unit-test.o $(L_FLAGS)

unit-tests: unit-test-sha256 unit-test-update unit-test-comm unit-test-fight unit-test-act-info unit-test-cloak unit-test-spendqp unit-test-spell-dam
	./unit-test-sha256
	./unit-test-update
	./unit-test-comm
	./unit-test-fight
	./unit-test-act-info
	./unit-test-cloak
	./unit-test-spendqp
	./unit-test-spell-dam
