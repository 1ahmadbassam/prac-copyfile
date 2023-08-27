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
	@mkdir -p $(ODIR)
	$(CC) $(CPPFLAGS) -c -o $@ $< $(CFLAGS)

copyfile.exe: $(OBJ)
	$(CC) $(CPPFLAGS) -o $@ $^ $(CFLAGS)

.PHONY: clean output

clean:
	rm -f $(ODIR)/*.obj *~ core $(INCDIR)/*~ 
