IDIR=../include
ODIR=obj

CC=clang
CFLAGS=-I$(IDIR) -g

_DEPS = util.h obj.h read.h eval.h print.h hash.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = util.o obj.o read.o eval.o print.o cxiv.o hash.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_H_OBJ = util.o obj.o hash.o thash.o
H_OBJ = $(patsubst %,$(ODIR)/%,$(_H_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o cxiv $^ $(CFLAGS)

hash: $(H_OBJ)
	$(CC) -o hash $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~
