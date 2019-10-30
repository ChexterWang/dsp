.PHONY: all clean run
CC=g++
CFLAGS=-Ofast -std=c++11
DFLAGS=-g -Wall -std=c++11
LDFLAGS=-lm
TARGET=train test accuracy

all: $(TARGET)

train: src/train.c
	$(CC) -o bin/$@ $^ $(CFLAGS) $(LDFLAGS) -Iinc

test: src/test.c
	$(CC) -o bin/$@ $^ $(CFLAGS) $(LDFLAGS) -Iinc

accuracy: util/accuracy.c
	$(CC) -o bin/$@ $^ $(CFLAGS) $(LDFLAGS) -Iinc

clean:
	rm -f bin/$(TARGET)
