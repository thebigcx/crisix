SRC = st2.c
OBJ = $(patsubst %.c, %.o, $(SRC))

TARG = kernel

CFLAGS =  -Wall          \
          -Wextra        \
          -fno-pic       \
          -fpie          \
          -mno-red-zone  \
          -nostdinc      \
          -ffreestanding \
          -nostdlib		 \
		  -Iinclude

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

clean:	
	rm -f $(OBJ) $(TARG)    
