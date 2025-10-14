# Project Name: Threads
# Author: Chandler Scott
# Description: Threads example


# Definition of variables
BUILD_TOOL = gcc
BUILD_DIR = ./build/

T_SOURCES = ./src/threads.c
T_TARGET = ./build/threads

D_SOURCES = ./src/data_race.c
D_TARGET = ./build/data_race

.PHONY: all clean setup

# Default target
all: setup $(T_TARGET) $(D_TARGET)

setup:
	mkdir -p $(BUILD_DIR)

$(T_TARGET): $(T_SOURCES) 
	$(BUILD_TOOL) $(T_SOURCES) -pthread -o $(T_TARGET)

$(D_TARGET): $(D_SOURCES) 
	$(BUILD_TOOL) $(D_SOURCES) -pthread -o $(D_TARGET)

# Clean target to remove build artifacts
clean:  
	rm -rf $(BUILD_DIR)
