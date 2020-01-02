SRIPATH ?= /mnt/c/Users/ChexterWang/Documents/ForGitProjects/dsp/hw3/srilm
MACHINE_TYPE ?= i686-m64
INC_PATH ?= inc
SRC_PATH ?= src

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -I$(INC_PATH) -Wall --std=c++11
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)
vpath %.c $(SRC_PATH)
vpath %.cpp $(SRC_PATH)

TARGET = mydisambig
SRC = $(SRC_PATH)/mydisambig.cpp
OBJ = $(SRC_PATH)/mydisambig.o
FROM ?= Big5-ZhuYin.map
TO ?= ZhuYin-Big5.map
.PHONY: all clean map

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	@echo 'Linking object files (mydisambig, liboolm, libdstruct, libmisc) ...'
	@$(CXX) -lpthread -fopenmp -lm -lz -o $@ $^
	@echo 'Complete!'

%.o: %.cpp
	@echo 'Compiling src/mydisambig.cpp...'
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

all: $(TARGET)

map:
	@echo 'Compiling src/map.cpp and generating executable...'
	@g++ -o map $(SRC_PATH)/map.cpp
	@echo 'Generating map file...'
	@./map $(FROM) $(TO)
	@echo 'Complete!'

clean:
	@echo 'Removing executables...'
	@$(RM) $(TARGET) ./map
	@echo 'Removing object files...'
	@$(RM) $(SRC_PATH)/*.o
