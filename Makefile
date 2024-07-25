CC := gcc
CFLAGS := \
	-Wall -Wextra -Wconversion -Werror \
	-Wno-sign-conversion -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter \
	-std=c99 \
	-O0 -g
LDFLAGS := -lSDL2
BUILD_DIR := _build

SRC := \
	main.c

all: $(BUILD_DIR) chip8

chip8: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(BUILD_DIR)/chip8

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
