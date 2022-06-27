CC = gcc
DEBUG := -DPM_DEBUG
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
EXEC = bin/passman

all: update $(EXEC)

$(OBJ) : obj/%.o : src/%.c
	$(CC) $(DEBUG) -c $? -o $@

$(EXEC) : $(OBJ)
	$(CC) -o $@ $?
	@echo "Finished compilation!"

clean:
	rm -rf $(EXEC) obj/*.o
	@echo "Cleaned directory!"

update:
	@perl help_gen.pl
	@echo "Updated help.c"

