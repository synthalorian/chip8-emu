CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs)

TARGET = chip8
SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/chip8.c \
          $(SRC_DIR)/opcodes.c \
          $(SRC_DIR)/display.c \
          $(SRC_DIR)/input.c \
          $(SRC_DIR)/audio.c

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(SDL_LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
