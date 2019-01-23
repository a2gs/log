CC = cc
AR = ar
RANLIB = ranlib

SRCDIR = ./src
INCDIR = ./inc
BINDIR = ./bin

CFLAGS = -g -Wall

RM = /bin/rm -f

OBJ = $(BINDIR)/liba2gs_Log.o
LIB = $(BINDIR)/liba2gs_Log.a
SOURCES = $(SRCDIR)/log.c

all: $(OBJ)

$(OBJ): $(SOURCES)
	$(CC) -c -o$(OBJ) -I$(INCDIR) $(SOURCES) $(CFLAGS)
	$(AR) rc $(LIB) $(OBJ)
	$(RANLIB) $(LIB)

clean:
	$(RM) $(BINDIR)/*
