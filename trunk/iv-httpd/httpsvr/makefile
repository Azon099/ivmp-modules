# Makefile for HTTP-Server in IV:MP
# Linux: run "make" to compile it

OUT=../httpsvr.so

SDK=../SDK/
MONGOOSE=$(SDK)mongoose/
INCZ=-I$(SDK) -I$(MONGOOSE) -I.
FLAGS=-c -O3 -w -DLINUX $(INCZ)
#-Wall

all:
	gcc $(FLAGS) -D_POSIX_SOURCE -D_BSD_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE $(MONGOOSE)*.c
	g++ $(FLAGS) $(SDK)*.cpp
	g++ $(FLAGS) *.cpp
	g++ -O2 -fshort-wchar -shared -L$(MONGOOSE)lib/ -lmongoose $(MONGOOSE)lib/libmongoose.a -o $(OUT) *.o
	rm *.o
	@echo "Done.\n"
