CC := gcc

CFLAGS := -g -std=gnu99 -pedantic -Wall -Wextra

BDIR := bin
DDIR := doc
IDIR := include
SDIR := src
TDIR := test

VPATH := $(SDIR):$(IDIR):$(ODIR):$(BDIR):$(TDIR)

TESTOBJECTS := $(patsubst %.c, %.o, $(wildcard $(SDIR)/test_*.c))
OBJECTS := $(filter-out $(SDIR)/minicas.o $(TESTOBJECTS), $(patsubst %.c, %.o, $(wildcard $(SDIR)/*.c)))
TESTS := $(patsubst $(SDIR)/%.o, $(BDIR)/%, $(TESTOBJECTS))

%.o: %.c
	$(CC) $(CFLAGS) -I$(IDIR) -c $< -o $@

$(BDIR)/%: $(SDIR)/%.o $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

all: $(BDIR)/minicas $(TESTS)

clean:
	@rm $(SDIR)/*.o $(BDIR)/minicas $(TESTS)


.PHONY: all clean
