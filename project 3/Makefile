#
# Makefile for CMINUS
# Gnu C Version
# K. Louden 2/3/98
#

CC = gcc

CFLAGS = 

OBJS = main.o util.o scan.o

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

analyze.o: global.h
	$(CC) $(CFLAGS) -c analyze.c

code.o: global.h
	$(CC) $(CFLAGS) -c code.c

cgen.o: global.h
	$(CC) $(CFLAGS) -c cgen.c

clean:
	-rm cminus
	#-rm tm
	-rm $(OBJS)

#tm: tm.c
	#$(CC) $(CFLAGS) tm.c -o tm

all: cminus

