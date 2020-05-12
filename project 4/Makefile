#
# Makefile for CMINUS
# Gnu C Version
# Lauren Ramirez & Isaac Martin 2/3/98
#

CC = gcc

CFLAGS = 

OBJS = main.o util.o scan.o parse.o symtab.o analyze.o

cminus: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cminus

main.o: main.c global.h util.h scan.h parse.h analyze.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h global.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h global.h
	$(CC) $(CFLAGS) -c scan.c

parse.o: parse.c parse.h scan.h global.h util.h
	$(CC) $(CFLAGS) -c parse.c

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c

analyze.o: analyze.c global.h symtab.h analyze.h
	$(CC) $(CFLAGS) -c analyze.c


clean:
	-rm cminus
	-rm $(OBJS)

all: cminus

