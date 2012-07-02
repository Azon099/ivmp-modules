# Makefile for HTTP-Server in IV:MP
# Linux: run "make" to compile it

OUT=../../Binary/modules/iv-httpd.so

SDK=../../SDK/
MONGOOSE=../mongoose/
INCZ=-I$(SDK) -I$(MONGOOSE) -I.
FLAGS=-c -O2 -w -DLINUX -D_LINUX $(INCZ)
#-Wall

all:
	gcc $(FLAGS) -D_POSIX_SOURCE -D_BSD_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE $(MONGOOSE)*.c
	g++ $(FLAGS) $(SDK)*.cpp
	g++ $(FLAGS) *.cpp
	g++ -O2 -fshort-wchar -shared -o $(OUT) *.o
	rm *.o
	@echo "Done.\n"
