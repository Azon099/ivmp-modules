CC=g++
CFLAGS=-c -Wall -D_LINUX -w -ISDK/ -Ipawn/linux/ -Ipawn/amx/ -I. -DAMX_ANSIONLY
SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard ../SDK/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../Binary/modules/iv-pawn.so

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(CFLAGS) pawn/amx/amx.c pawn/amx/amxaux.c pawn/amx/amxcons.c pawn/amx/amxcore.c pawn/amx/amxstring.c pawn/amx/amxtime.c pawn/amx/fixed.c pawn/amx/float.c
	g++ $(OBJECTS) amx*.o fixed.o float.o -lpthread -ldl -shared -o $@ 
	mkdir -p ../Binary/sdk/
	cp -R pawn_sdk ../Binary/sdk/

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf $(OBJECTS) $(EXECUTABLE)
