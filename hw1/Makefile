.PHONY: all clean

CFLAGS+=
LDFLAGS+=-lm     # link to math library

TARGET=train

all: $(TARGET)
# type make/make all to compile test_hmm
	g++ -O3 -Wall train.cpp -o train
	g++ -O3 -Wall test.cpp -o test

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
