#
# **************************************************************
# *                Simple C++ Makefile Template                *
# *                                                            *
# * Author: Arash Partow (2003)                                *
# * URL: http://www.partow.net/programming/makefile/index.html *
# *                                                            *
# * Copyright notice:                                          *
# * Free use of this C++ Makefile template is permitted under  *
# * the guidelines and in accordance with the the MIT License  *
# * http://www.opensource.org/licenses/MIT                     *
# *                                                            *
# **************************************************************
#
#
# Modifications of the original file are licenced under MIT Licence
# 
#(c) Ludovic 'Archivist' Lagouardette 2018
#


CXX      := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -Werror -O3 -std=c++14
# -DCOMPAT_TLS
# ^ Enable this flag if your compiler ABI have issues with thread local storage
LDFLAGS  := -L/usr/lib -lstdc++ -lm -lpthread
BUILD    := build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := tests.cpp
INCLUDE  := -Iinclude/
SRC      :=                      \
	src/FastRandom/memory_randomize.cpp         \
	src/FastRandom/uuid.cpp         \
	src/FastRandom/base_prng.cpp         \

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
TARGETNAME := $(TARGET:%.cpp=%)

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGETNAME) src/$(TARGET) $(OBJECTS)

.PHONY: all build clean

ASTYLE_FLAGS= --style=stroustrup --align-reference=type --align-pointer=type --break-blocks \
              --indent-namespaces --indent=tab --add-brackets 
format:
	astyle $(ASTYLE_FLAGS) include/FastRandom/*
	astyle $(ASTYLE_FLAGS) src/FastRandom/*

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf build/*
	rm -rf src/FastRandom/*.orig
	rm -rf include/FastRandom/*.orig