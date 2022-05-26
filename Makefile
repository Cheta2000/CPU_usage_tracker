RM := rm -rf

SDIR := ./src
ADIR := ./app
IDIR := ./inc
TDIR := ./test

SRC := $(wildcard $(SDIR)/*.c)
ASRC := $(SRC) $(wildcard $(ADIR)/*.c)
TSRC := $(SRC) $(wildcard $(TDIR)/*.c)
EXEC := main.out
TEST := test.out

CC := gcc
CC_STD := -std=c99
CC_FLAGS := -Wall -Wextra

ifeq ($(CC),clang)
	CC_FLAGS := -Weverything
endif

ifeq ("$(origin O)", "command line")
	OPT := -O$(O)
else
	OPT := -O3
endif

CC_FLAGS += $(OPT) 

all: $(EXEC)

$(EXEC): $(ASRC)
	$(CC) $(CC_STD) $(CC_FLAGS) $(ASRC) -o $@ 

test : $(TEST)

$(TEST): $(TSRC)
	$(CC) $(CC_STD) $(CC_FLAGS) $(TSRC) -o $@ 

clean:
	$(RM) $(EXEC)
	$(RM) $(TEST)
