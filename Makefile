# Makefile for Snake Game

# Define the compiler
CC = emcc

# Define the source files
SOURCES = main.cc

# Define the output file
OUTPUT = snake_game.html

# Define raylib path (using web-compiled version) I had compiled at ../raylib for web.
RAYLIB_INCLUDE = ../raylib/src
RAYLIB_LIB = ../raylib/src

# Define the flags
CFLAGS = -I$(RAYLIB_INCLUDE) -L$(RAYLIB_LIB) -lraylib.web -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 -DPLATFORM_WEB

# Default target
all: $(OUTPUT)

# Rule to build the web version
$(OUTPUT): $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o $(OUTPUT)

# Clean rule
clean:
	rm -f snake_game.html snake_game.js snake_game.wasm

# Serve the game locally (requires Python)
serve:
	python3 -m http.server 8000

.PHONY: all clean serve