/* C library */
#include <stdio.h>
#include <string.h>
/* Chess header file */
#include <dchess.h>

int
main(int argc, char **argv)
{
	chess_state game = { 0 };
	char input_buffer[MAX_INPUT], *new_line;

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

		print_board(&game);
	}

	return 0;
}
