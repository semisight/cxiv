IDIR=../include
ODIR=obj

CC=clang
CFLAGS=-I$(IDIR) -g

_DEPS = util.h obj.h read.h eval.h print.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = util.o obj.o read.o eval.o print.o cxiv.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o cxiv $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~
