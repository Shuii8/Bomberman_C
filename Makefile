# Directories
S_DIR=source
B_DIR=.

# Files
S_FILES=$(S_DIR)/demo.c

# Output
EXEC=$(B_DIR)/demo

# Build settings
CC=gcc
# SDL options
CC_SDL= `sdl2-config --cflags --libs`


all:Build

Build:
	$(CC) $(S_FILES) -o $(EXEC) $(CC_SDL)

build_run:Build
	$(EXEC)

clean:
	rm -f demo
	rm -f source/.*.swp
