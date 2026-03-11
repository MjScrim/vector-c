CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -g
INCLUDES = -I./include

SRC = src/vector.c examples/main.c

TARGET = build/main

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build
