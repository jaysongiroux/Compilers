#
# Makefile for TINY
# Gnu C Version
# K. Louden 2/3/98
#

CC = gcc

CFLAGS = 

OBJS = main.o util.o scan.o parse.o symtab.o analyze.o 

cminus: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cminus

main.o: main.c globals.h util.h scan.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c scan.c

parse.o: parse.c parse.h scan.h globals.h util.h
	$(CC) $(CFLAGS) -c parse.c

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c

analyze.o: analyze.c globals.h symtab.h analyze.h
	$(CC) $(CFLAGS) -c analyze.c


clean:
	-rm cminus
	-rm cm
	-rm $(OBJS)

cm: cm.c
	$(CC) $(CFLAGS) cm.c -o cm

all: cminus cm

