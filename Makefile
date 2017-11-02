
SOURCES = $(wildcard src/*.cpp)

include ../../plugin.mk

LDFLAGS += -rdynamic \
	-lpthread -ldl \
	-L../../dep/lib -ljansson

dist: all
	mkdir -p dist/Fundamental
	cp LICENSE* dist/Fundamental/
	cp $(TARGET) dist/Fundamental/
	cp -R res dist/Fundamental/

main.o: main.cpp 
	$(CXX) -g -o main.o -c -Isrc -I../../include main.cpp -std=c++11 -I ../../dep/include/ -fPIC

test: all main.o
	$(CXX) main.o $(OBJECTS) -o test $(LDFLAGS)
