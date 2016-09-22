CC=gcc
CCFLAGS=-Wall -Wpedantic -Werror -O5 -mtune=native -std=c11
LDFLAGS=-lcrypto -lm

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=pbkdf2_test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

.PHONY: clean
clean:
	rm $(OBJECTS) $(TARGET)
