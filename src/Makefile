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

C_FLAGS = -O -g2 -Wall -DACK_43 $(PROF) $(SOLARIS_FLAG) -Iheaders -Iai/headers -Itests/headers
L_FLAGS = -O -g2 -lcrypt $(PROF) $(SOLARIS_LINK)

OBJDIR = build/obj

include ai/Rules.mk
include tests/Rules.mk

MAIN_OBJS = $(addprefix $(OBJDIR)/,$(O_FILES))

O_FILES = ai.o act_clan.o act_comm.o act_info.o act_mob.o act_move.o act_obj.o act_wiz.o \
	  areachk.o areasave.o board.o build.o buildare.o buildtab.o caravan.o clutch.o comm.o \
	  const.o damage.o cloak.o death.o db.o email.o enchant.o fight.o handler.o hash.o hunt.o \
	  interp.o invasion.o item_generation.o item_generation_tables.o keep.o lists.o macros.o magic.o magic2.o magic3.o magic4.o mapper.o \
	  obj_fun.o pdelete.o quest.o crusade.o reincarnate.o rulers.o \
	  save.o scheck.o skills.o skills_chi.o skills_combo.o skills_obj.o skills_poison.o social-edit.o special.o spell_dam.o spendqp.o ssm.o \
	  stance.o strfuns.o sysdata.o tables.o trigger.o update.o vampyre.o werewolf.o wizutil.o \
	  write.o $(AI_SPEC_SRCS)

.PHONY: all clean purge unit-tests integration-test integration-test-telnet web

all:
	$(MAKE) clean
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
