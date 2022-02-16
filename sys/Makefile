SRC := src/st2.c   \
       src/debug.c \
       src/heap.c

include arch/x86_64/arch.mk

OBJ = $(patsubst %.c, %.o, $(patsubst %.s, %.o, $(SRC)))

TARG = kernel

INCLUDE += -Iinclude

CFLAGS := -Wall                                 \
          -Wextra                               \
          -Werror=implicit-function-declaration \
          -Werror=return-type                   \
          -fno-pic                              \
          -fpie                                 \
          -mno-red-zone                         \
          -nostdinc                             \
          -ffreestanding                        \
          -nostdlib                             \
          -g                                    \
          $(INCLUDE)

LDFLAGS = -Tlinker.ld    \
          -nostdlib      \
          -static        \
          -pie

LD = ld
CC = gcc

.PHONY: clean all

all: $(TARG)

$(TARG): $(OBJ)
	@echo "    LD $@"
	@$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@echo "    CC $@"
	@$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.s
	@echo "    AS $@"
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJ) $(TARG)
