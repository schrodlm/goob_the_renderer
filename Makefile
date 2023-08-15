CXX = g++
FLAGS = -pg -fms-extensions -g
BASIC_FLAGS = -Wall -pedantic
LIBS = -lm


TARGET = main

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, build/%.o, ${SOURCES})
DEPS = $(patsubst src/%.cpp, build/%.dep, ${SOURCES})

# makes it obvious for make that clean isnt a file
.PHONY: all clean compile

all: compile

compile: ${OBJECTS}
	@mkdir -p build/
	${CXX} ${BASIC_FLAGS} ${FLAGS} $^

build/%.o: src/%.cpp
	@mkdir -p build/
	${CXX} ${BASIC_FLAGS} ${FLAGS} -c $< -o $@

clean:
	-rm -rf build
	-rm -f *.tga

build/%.dep: src/%.cpp src/*
	@mkdir -p build/
	${CXX} -MM -MT $(patsubst src/%.cpp, build/%.o, $<) $< > $@

include ${DEPS}

