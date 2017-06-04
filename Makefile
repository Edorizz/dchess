CC := gcc
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
CFLAGS := -c -std=c99 -pedantic -Wall
LDFLAGS :=
RM := rm -f
NAME := dchess

$(NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

obj/main.o: src/main.c src/dchess.h
	$(CC) $(CFLAGS) src/main.c -o $@

obj/dchess.o: src/dchess.c src/dchess.h
	$(CC) $(CFLAGS) src/dchess.c -o $@

clean:
	$(RM) obj/*.o $(NAME)

