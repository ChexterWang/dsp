.PHONY: all clean run
CC=g++
CFLAGS=-g -Wall -std=c++11
LDFLAGS=-lm
TARGET=train test

all: $(TARGET)

train: src/train.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -Iinc

test: src/test.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -Iinc

clean:
	rm -f $(TARGET)
