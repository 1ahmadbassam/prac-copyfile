IDIR=includes
CC=gcc
CPPFLAGS=-Wall
CFLAGS=-I$(IDIR)
VPATH=src
_DEPS= string.h includes.h
_OBJ = string.obj main.obj 

ODIR=output
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.obj: %.c $(DEPS)
	$(CC) $(CPPFLAGS) -c -o $@ $< $(CFLAGS)

copyfile.exe: $(OBJ)
	$(CC) $(CPPFLAGS) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.obj *~ core $(INCDIR)/*~ 
