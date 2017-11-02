CPP_SRCS = $(wildcard src/*.cpp)
CPP_SRCS := $(filter-out src/render.cpp, $(CPP_SRCS))
OBJECTS = $(addprefix build/src/,$(notdir $(CPP_SRCS:.cpp=.o)))

#include ../../plugin.mk
#include ../../compile.mk

CXXFLAGS = -std=c++11 -Isrc -g -O0 -I../../include -I../../dep/include -DRACK_ARM -DARCH_LIN
LDFLAGS += -rdynamic \
	-lpthread -ldl \
	-L../../dep/lib -ljansson -lsamplerate

dist: all
	mkdir -p dist/Fundamental
	cp LICENSE* dist/Fundamental/
	cp $(TARGET) dist/Fundamental/
	cp -R res dist/Fundamental/

build/src/%.o: ./src/%.cpp
	$(AT) $(CXX) $(SYNTAX_FLAG) $(INCLUDES) $(CXXFLAGS) -Wall -c -fmessage-length=0 -U_FORTIFY_SOURCE -MMD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<" $(CPPFLAGS) -fPIC -Wno-unused-function

build/main.o: main.cpp
	$(AT) $(CXX) $(SYNTAX_FLAG) $(INCLUDES) $(CXXFLAGS) -Wall -c -fmessage-length=0 -U_FORTIFY_SOURCE -MMD -MP -MF"$(@:%.o=%.d)" -o "$@" "$<" $(CPPFLAGS) -fPIC -Wno-unused-function

test: $(OBJECTS) build/main.o
	$(CXX) $(OBJECTS) -o test $(LDFLAGS) build/main.o

clean:
	rm build/src/*

