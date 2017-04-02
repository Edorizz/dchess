/* C library */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/* Chess header file */
#include <dchess.h>

int
main(int argc, char **argv)
{
	chess_state game = { 0 };
	char input_buffer[MAX_INPUT], *data, *new_line;
	int i;

	init_board(&game);
	while (!(game.flags & BIT(QUIT))) {
		do {
			printf("> ");

			fgets(input_buffer, MAX_INPUT, stdin);
			if ((new_line = strchr(input_buffer, '\n')) == NULL) {
				printf("Input too large\n\n");
				/* Clear input */
				while (getchar() != '\n')
					;

				continue;
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
		if (i == COMMAND_COUNT)
			move(&game, data);
	}

	return 0;
}
