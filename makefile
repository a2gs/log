CC = cc
AR = ar
RANLIB = ranlib

SRCDIR = ./src
INCDIR = ./inc
BINDIR = ./bin

CFLAGS = -g -Wall -std=c11 -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -D_POSIX_SOURCE=1 -D_DEFAULT_SOURCE=1 -D_GNU_SOURCE=1

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
