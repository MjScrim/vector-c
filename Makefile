CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -g
INCLUDES = -I./include
FLAG_TEST = -fsanitize=address

SRC = src/vector.c examples/main.c
SRC_TEST = src/*c
TEST = tests/test_vector.c

TARGET = build/main
TARGET_TEST = build/test

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

test:
	$(CC) $(FLAG_TEST) $(INCLUDES) $(SRC_TEST) $(TEST) -o $(TARGET_TEST)
	./$(TARGET_TEST)

clean:
	rm -rf build
