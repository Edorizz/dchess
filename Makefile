CC := gcc
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
CFLAGS := -c -std=c99 -pedantic -Wall -Iinclude/
LDFLAGS :=
RM := rm -f
NAME := dchess

$(NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

obj/main.o: src/main.c include/dchess.h
	$(CC) $(CFLAGS) src/main.c -o $@

obj/dchess.o: src/dchess.c include/dchess.h
	$(CC) $(CFLAGS) src/dchess.c -o $@

clean:
	$(RM) obj/*.o $(NAME)

