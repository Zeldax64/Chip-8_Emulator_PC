# Chip-8 Makefile
PROG = chip-8
CC = gcc
CPPFLAGS = -Wall -O2 -I./src 
LIBS = -lGL -lGLU -lglut

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))
all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LIBS)

# Compilation wildcard
%.o: %.cpp %.h
	$(CC) $(CPPFLAGS) -o $@ -c $<

.PHONY: clean help

clean:
	rm -f $(PROG) $(OBJS)

help:
	echo $(SRCS)
	echo $(OBJS)