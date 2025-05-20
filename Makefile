CC = gcc
SDL = sdl-config
LDFLAGS = -L/opt/homebrew/Cellar/sdl2/2.32.4/lib -lSDL2\
		  -L/opt/homebrew/Cellar/sdl2_ttf/2.24.0/lib -lSDL2_ttf
CFLAGS = -g -Wall -Wextra -std=c11 -pedantic -I/opt/homebrew/Cellar/sdl2/2.32.4/include \
			-I/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2
SRC_DIR = . src
#SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
SRC_FILES = $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
OUTPUT = xezia 


# rules for compiling .c to xezia
$(OUTPUT): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC_FILES) $(LDFLAGS)
# rules for compiling .c to .o
.c.o: 
#should also work fine 
#$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
# call this to run the game
run: $(OUTPUT)
	./xezia
# call this to clean
clean: 
	rm -f $(OBJ_FILES) $(OUTPUT)
