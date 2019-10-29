.PHONY: all clean run
CC=g++
CFLAGS=-Ofast -std=c++11
DFLAGS=-g -Wall -std=c++11
LDFLAGS=-lm
TARGET=train test accuracy

all: $(TARGET)

train: src/train.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -Iinc

test: src/test.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -Iinc

accuracy: src/accuracy.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -Iinc

clean:
	rm -f $(TARGET)
