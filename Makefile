CC := gcc
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
CFLAGS := -c -std=c99 -pedantic -Wall -Iinclude/
LDFLAGS :=
RM := rm -f
NAME := dchess

$(NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) obj/*.o $(NAME)

