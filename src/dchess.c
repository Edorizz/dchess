/* Header file */
#include <dchess.h>
/* C library */
#include <stdio.h>
#include <string.h>

/* Globals */
const char pieces[MAX_PIECES - 1] = { 'P', 'R', 'N', 'B', 'Q', 'K' };
const command commands[COMMAND_COUNT] = { { "exit", cmd_exit }, { "quit", cmd_exit } };

void
init_board(chess_state *game)
{
	/* First black row */
	game->board[0][0] = BLOCK(ROOK, KNIGHT);
	game->board[0][1] = BLOCK(BISHOP, QUEEN);
	game->board[0][2] = BLOCK(KING, BISHOP);
	game->board[0][3] = BLOCK(KNIGHT, ROOK);

	/* Black pawn row */
	for (int i = 0; i != BOARD_WIDTH / 2; ++i)
		game->board[1][i] = BLOCK(PAWN, PAWN);

	/* White pawn row */
	for (int i = 0; i != BOARD_WIDTH / 2; ++i)
		game->board[BOARD_HEIGHT - 2][i] = WHITE_BLOCK(PAWN, PAWN);

	/* First white row */
	game->board[BOARD_HEIGHT - 1][0] = WHITE_BLOCK(ROOK, KNIGHT);
	game->board[BOARD_HEIGHT - 1][1] = WHITE_BLOCK(BISHOP, QUEEN);
	game->board[BOARD_HEIGHT - 1][2] = WHITE_BLOCK(KING, BISHOP);
	game->board[BOARD_HEIGHT - 1][3] = WHITE_BLOCK(KNIGHT, ROOK);
}

void
print_block(BYTE block)
{
	BYTE l = LEFT_PIECE(block);
	BYTE r = RIGHT_PIECE(block);

	printf(" %c %c",
	       l ? pieces[(l & 7) - 1] : ' ',
	       r ? pieces[(r & 7) - 1] : ' ');
}

void
print_board(chess_state *game)
{
	for (int i = 0; i != BOARD_HEIGHT; ++i) {
		for (int j = 0; j != BOARD_WIDTH / 2; ++j)
			print_block(game->board[i][j]);
		putchar('\n');
	}
}

void
cmd_exit(chess_state *game)
{
	game->flags |= BIT(QUIT);
}

