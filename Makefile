CC = gcc
CMAKE = cmake

CFLAGS = -Wall -Wextra -Werror -std=c11 -g
INCLUDES = -I./include
FLAG_TEST = -fsanitize=address

SRC = src/vector.c examples/main.c
SRC_TEST = src/*c
TEST = tests/test_vector.c

TARGET = build/main
TARGET_TEST = build/test

setup:
	$(CMAKE) -S . -B build

build-cmake:
	$(CMAKE) --build build

run-cmake: build-cmake
	$(CMAKE) --build build --target run

ctest:
	cd build && ctest --output-on-failure

check: build-cmake ctest

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

format:
	find src include tests -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -style=file -i {} +

doc:
	doxygen Doxyfile

.PHONY: all run test clean cmake-build ctest check setup format doc
