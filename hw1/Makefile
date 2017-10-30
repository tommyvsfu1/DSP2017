.PHONY: all clean

CC=g++
CFLAGS=-O3 -Wall
LDFLAGS+=-lm     # link to math library

TARGET=train test

all: $(TARGET)

train: train.cpp
	$(CC) $(CFLAGS) $< -o $@

test: test.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGET)