CC := gcc

ODIR := ./build
EXE := test.out
# ALL_C := $(wildcard CR/*.c) $(wildcard *.c)
# OBJS := $(subst CR/,,$(patsubst %.c,$(ODIR)/%.c.o,$(ALL_C)))
CR_SRC := ./CR
TEST_SRC := ./sample
CR_C := $(wildcard $(CR_SRC)/*.c)
TEST_C := $(wildcard $(TEST_SRC)/*.c)
CR_OBJS := $(CR_C:$(CR_SRC)/%=$(ODIR)/%.o)
TEST_OBJS := $(TEST_C:$(TEST_SRC)/%=$(ODIR)/%.o)




$(ODIR)/$(EXE):$(CR_OBJS) $(TEST_OBJS)
	$(CC) $(CR_OBJS) $(TEST_OBJS) -o $(ODIR)/$(EXE)

$(ODIR)/%.c.o:$(CR_SRC)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(ODIR)/%.c.o:$(TEST_SRC)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

#test.out:$(OBJS)\
	$(CC) $(OBJS) -o $(ODIR)/test.out\
\
$(OBJS):$(ALL_C)\
	mkdir -p $(ODIR)\
	$(foreach n, $(ALL_C), $(CC) $(n) -c $(CFLAGS) -o $(subst CR/,,$(patsubst %.c,$(ODIR)/%.c.o,$(n)));)

.PHONY: all clean run
all:clean run

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.out

run:$(ODIR)/$(EXE)
	./$(ODIR)/$(EXE)
