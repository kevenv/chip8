CC := gcc
CFLAGS := \
	-Wall -Wextra -Wconversion -Werror \
	-Wno-sign-conversion -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter \
	-std=c99
CFLAGS_DEBUG := -O0 -g
CFLAGS_RELEASE := -O3 -s
LDFLAGS := -lSDL2
BUILD_DIR := _build
EXE := chip8

SRC := \
	src/main.c \
	src/app.c \
	src/chip8.c \
	src/display.c \
	src/keypad.c \
	src/rom.c \
	src/speaker.c

all: $(BUILD_DIR) $(EXE)

$(EXE): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $(LDFLAGS) $(SRC) -o $(BUILD_DIR)/$(EXE)

release: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) $(LDFLAGS) $(SRC) -o $(BUILD_DIR)/$(EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
