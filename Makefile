CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = program.out

# Directory setup
SRC_DIR = source
BUILD_DIR = build
HEADER_DIR = header

# Get all .c files from src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Transform: src/main.c → build/main.o
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ -lm

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)  # Create build directory if needed
	$(CC) $(CFLAGS) -I./header -c $< -o $@

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

compile-run: all run

.PHONY: all clean run