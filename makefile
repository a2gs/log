CC = cc
AR = ar
RANLIB = ranlib

SRCDIR = ./src
INCDIR = ./inc
BINDIR = ./bin

CFLAGS_OPTIMIZATION = -g
#CFLAGS_OPTIMIZATION = -O3
CFLAGS_VERSION = -std=c11
CFLAGS_WARNINGS = -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-parameter -Wno-unused-result
CFLAGS_DEFINES = -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L -D_POSIX_SOURCE=1 -D_DEFAULT_SOURCE=1 -D_GNU_SOURCE=1
CFLAGS = $(CFLAGS_OPTIMIZATION) $(CFLAGS_VERSION) $(CFLAGS_WARNINGS) $(CFLAGS_DEFINES)

RM = rm -rf

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
