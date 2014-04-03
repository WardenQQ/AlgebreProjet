CC := gcc

CFLAGS := -g -std=gnu99 -pedantic -Wall -Wextra

DDIR := doc

IDIR := include

SDIR := src

VPATH := $(SDIR):$(IDIR):$(ODIR):$(BDIR)

OBJECTS := $(patsubst %.c, %.o, $(wildcard $(SDIR)/*.c))


%.o: %.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $< -o $@

all: minicas

clean:
	@rm $(SDIR)/*.o minicas

minicas: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all clean
