CC = gcc
DEBUG := -DPM_DEBUG
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
EXEC = bin/passman

all: $(EXEC)

$(OBJ) : obj/%.o : src/%.c
	$(CC) $(DEBUG) -c $? -o $@

$(EXEC) : $(OBJ)
	$(CC) -o $@ $?

clean:
	rm -rf $(EXEC) obj/*.o

