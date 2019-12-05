SRIPATH ?= /mnt/c/Users/ChexterWang/Documents/ForGitProjects/dsp/hw3/srilm
MACHINE_TYPE ?= i686-m64
INC_PATH ?= inc
SRC_PATH ?= src

CXX = g++
CXXFLAGS = -O2 -I$(SRIPATH)/include -I$(INC_PATH)
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)
vpath %.c $(SRC_PATH)
vpath %.cpp $(SRC_PATH)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
FROM ?= Big5-ZhuYin.map
TO ?= ZhuYin-Big5.map
.PHONY: all clean map

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

all: $(TARGET)

map:
	@echo 'Compiling src/map.cpp and generating executable...'
	@g++ -o $(SRC_PATH)/map $(SRC_PATH)/map.cpp
	@echo 'Generating map file...'
	@$(SRC_PATH)/map $(FROM) $(TO)
	@echo 'Complete!'

clean:
	$(RM) $(OBJ) $(TARGET)
