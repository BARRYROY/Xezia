CC = gcc
LDFLAGS = -L/opt/homebrew/Cellar/sdl2/2.30.8/lib -lSDL2
CFLAGS = -Wall -Wextra -std=c11 -pedantic -I/opt/homebrew/Cellar/sdl2/2.30.8/include 
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
OUTPUT = xezia 


# rules for compiling .c to xezia
$(OUTPUT): $(SOURCE)
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
