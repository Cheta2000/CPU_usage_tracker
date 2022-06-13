RM := rm -rf

SDIR := ./src
ADIR := ./app
IDIR := ./inc
TDIR := ./test

SRC := $(wildcard $(SDIR)/*.c)
ASRC := $(SRC) $(wildcard $(ADIR)/*.c)
TSRC := $(SRC) $(wildcard $(TDIR)/*.c)

AOBJ := $(ASRC:%.c=%.o)
TOBJ := $(TSRC:%.c=%.o)
OBJ := $(AOBJ) $(TOBJ) 

EXEC := main.out
TEST := test.out

H_INC := $(foreach d, $(IDIR), -I$d)

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

$(EXEC): $(AOBJ)
	$(CC) $(CC_STD) $(CC_FLAGS) $(AOBJ) -o $@ -lpthread -lm

%.o:%.c
	$(CC) $(CC_STD) $(CC_FLAGS) $(H_INC) -c $< -o $@

test : $(TEST)

$(TEST): $(TOBJ)
	$(CC) $(CC_STD) $(CC_FLAGS) $(TOBJ) -o $@ -lpthread -lm

clean:
	$(RM) $(EXEC)
	$(RM) $(OBJ)
	$(RM) $(TEST)
