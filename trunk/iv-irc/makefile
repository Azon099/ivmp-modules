CXX = g++
CXXFLAGS = -O3 -fPIC -Wall -I./Libraries/
LDFLAGS = -s -shared -L./Libraries/boost/ -lboost_system -lboost_thread -lssl -lcrypto

OUT = ./irc.so ./*.o
SRC = ./SDK/*.cpp ./Source/*.cpp ./*.cpp

all:
	$(CXX) -c $(SRC) $(CXXFLAGS)
	$(CXX) -o $(OUT) $(LDFLAGS)
	-rm -f ./*.o
