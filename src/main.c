/* C library */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/* Chess header file */
#include "dchess.h"

#define MAX_INPUT	256

int
main(int argc, char **argv)
{
	chess_state game = { 0 };
	char input_buffer[MAX_INPUT], *data, *new_line;
	int i;

	init_board(&game);
	while (!(game.flags & BIT(QUIT))) {
		do {
			printf("black(%d,%d) | white(%d,%d)\n",
			       game.black_king.y, game.black_king.x,
			       game.white_king.y, game.white_king.x);
			printf("(%c | %.2x): ", game.flags & BIT(WHITE_TURN) ? 'w' : 'b', game.flags);

			fgets(input_buffer, MAX_INPUT, stdin);
			if ((new_line = strchr(input_buffer, '\n')) == NULL) {
				printf("Input too large\n\n");
				/* Clear input */
				while (getchar() != '\n')
					;
			}
		} while (!new_line);
		*new_line = '\0';

		/* Skip spaces in input */
		data = input_buffer;
		while (!isgraph(*data))
			++data;

		/* Skip everyting after the first space */
		new_line = data;
		while (isgraph(*new_line))
			++new_line;
		*new_line = '\0';

		printf("Your input was \"%s\"\n", data);

		/* Check if input is a command */
		for (i = 0; i != COMMAND_COUNT; ++i) {
			if (strcmp(commands[i].name, data) == 0) {
				commands[i].cmd(&game);
				break;
			}
		}

		/* If it isn't a command, interpret it as a piece movement */
		if (i == COMMAND_COUNT) {
			move(&game, data);
		}

		if (game.flags & BIT(WHITE_CHECK)) {
			printf(BLUE "White is on check!\n" RESET);
		} else if (game.flags & BIT(BLACK_CHECK)) {
			printf(RED "Red is on check!\n" RESET);
		}
	}

	return 0;
}

