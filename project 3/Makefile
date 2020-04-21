#
# Makefile for CMINUS
# Gnu C Version
# K. Louden 2/3/98
#

CC = gcc

CFLAGS = 

OBJS = main.o util.o scan.o parse.o

cminus: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cminus

main.o: main.c global.h util.h scan.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h global.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h global.h
	$(CC) $(CFLAGS) -c scan.c

parse.o: scan.h global.h util.h
	$(CC) $(CFLAGS) -c parse.c

clean:
	-rm cminus
	-rm $(OBJS)

all: cminus
