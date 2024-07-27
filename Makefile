CC := gcc
CFLAGS := \
	-Wall -Wextra -Wconversion -Werror \
	-Wno-sign-conversion -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter \
	-std=c99 \
	-O0 -g
LDFLAGS := -lSDL2
BUILD_DIR := _build
EXE := chip8

SRC := \
	src/main.c \
	src/app.c \
	src/chip8.c \
	src/display.c \
	src/keypad.c \
	src/rom.c

all: $(BUILD_DIR) $(EXE)

$(EXE): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(BUILD_DIR)/$(EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
