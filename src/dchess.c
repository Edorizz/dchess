/* Header file */
#include <dchess.h>
/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Globals */
const char pieces[MAX_PIECES - 1] = { 'P', 'R', 'N', 'B', 'Q', 'K' };
const command commands[COMMAND_COUNT] = { { "exit", cmd_exit }, { "quit", cmd_exit },
					  { "help", cmd_help },
					  { "status", cmd_status } };

void
init_board(chess_state *game)
{
	/* Clear board */
	memset(game->board, 0, sizeof(game->board));

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

	game->flags = BIT(WHITE_TURN);
}

void
print_block(BYTE block)
{
	BYTE l = LEFT_PIECE(block);
	BYTE r = RIGHT_PIECE(block);

	if (l) {
		printf(" %s%c%s",
		       l & BIT(WHITE_PIECE) ? BLUE : RED,
		       pieces[(l & 0x7) - 1],
		       RESET);
	} else {
		printf(" x");
	}

	if (r) {
		printf(" %s%c%s",
		       r & BIT(WHITE_PIECE) ? BLUE : RED,
		       pieces[(r & 0x7) - 1],
		       RESET);
	} else {
		printf(" x");
	}
}

void
print_board(chess_state *game)
{
	printf("     a b c d e f g h\n\n");
	for (int i = 0; i != BOARD_HEIGHT; ++i) {
		printf(" %c  ", '8' - i);
		for (int j = 0; j != BOARD_WIDTH / 2; ++j)
			print_block(game->board[i][j]);
		printf("  %c\n", '8' - i);
	}
	printf("\n     a b c d e f g h\n");

	putchar('\n');
}

BYTE
read_at(chess_state *game, int row, int col)
{
	BYTE block = game->board[row][col / 2];

	if (col % 2)
		return RIGHT_PIECE(block);
	else
		return LEFT_PIECE(block);
}

void
write_at(chess_state *game, int row, int col, BYTE piece)
{
	BYTE *block = &game->board[row][col / 2];

	if (col % 2)
		*block = BLOCK(LEFT_PIECE(*block), piece);
	else
		*block = BLOCK(piece, RIGHT_PIECE(*block));
}

int
relationship(BYTE p1, BYTE p2)
{
	if (p1 == 0 || p2 == 0)
		return NONE;

	if ((p1 ^ p2) & BIT(WHITE_PIECE))
		return ENEMY;

	return FRIENDLY;
}

int
line_test(chess_state *game, int crow, int ccol, int trow, int tcol)
{
	int i, j;
	
	if (crow == trow) {
		if (ccol > tcol)
			return line_test(game, trow, tcol, crow, ccol);

		while (++ccol < tcol) {
			if (read_at(game, crow, ccol))
				return 0;
		}
	} else if (ccol == tcol) {
		if (crow > trow)
			return line_test(game, trow, tcol, crow, ccol);

		while (++crow < trow) {
			if (read_at(game, crow, ccol))
				return 0;
		}
	} else if (abs(crow - trow) == abs(ccol - tcol)) {
		i = (trow - crow) / abs(trow - crow);
		j = (tcol - ccol) / abs(tcol - ccol);

		while ((crow += i) != trow) {
			if (read_at(game, crow, ccol += j))
				return 0;
		}
	} else {
		return 0;
	}

	return 1;
}

int
valid_movement(chess_state *game, int crow, int ccol, int trow, int tcol)
{
	BYTE current = read_at(game, crow, ccol);
	BYTE target = read_at(game, trow, tcol);
	int rel, rdiff, cdiff;

	if (((current & BIT(WHITE_PIECE)) >> WHITE_PIECE) ^
	    ((game->flags & BIT(WHITE_TURN)) >> WHITE_TURN))
		return 0;

	if ((rel = relationship(current, target)) == FRIENDLY)
		return 0;

	rdiff = crow - trow;
	cdiff = ccol - tcol;

	switch (current & 0x7) {
	case PAWN:
		if (rel == ENEMY) {
			if (current & BIT(WHITE_PIECE)) {
				if (rdiff != 1 || abs(cdiff) != 1)
					return 0;
			} else if (rdiff != -1 || abs(cdiff) != 1) {
					return 0;
			}
		} else {
			if (current & BIT(WHITE_PIECE)) {
				if (ccol != tcol || rdiff != 1)
					return 0;
			} else if (ccol != tcol || rdiff != -1) {
					return 0;
			}
		}

		break;
	case ROOK:
		if ((rdiff && cdiff) || !line_test(game, crow, ccol, trow, tcol))
			return 0;

		break;
	case KNIGHT:
		if ((abs(rdiff) + abs(cdiff) != 3) || (abs(rdiff) != 1 && abs(cdiff) != 1))
			return 0;

		break;
	case BISHOP:
		if (abs(rdiff) != abs(cdiff) || !line_test(game, crow, ccol, trow, tcol))
			return 0;

		break;
	case QUEEN:
		if (((rdiff && cdiff) || !line_test(game, crow, ccol, trow, tcol)) &&
		    (abs(rdiff) != abs(cdiff) || !line_test(game, crow, ccol, trow, tcol)))
			return 0;

		break;
	case KING:
		if (abs(rdiff) > 1 || abs(cdiff) > 1)
			return 0;

		break;
	default:
		printf("Invalid chess piece detected\n");
		return 0;
	}

	return 1;
}

void
move_piece(chess_state *game, int crow, int ccol, int trow, int tcol)
{
	/* Copy piece to new location */
	write_at(game, trow, tcol, read_at(game, crow, ccol));
	/* Delete old piece */
	write_at(game, crow, ccol, 0);
}

void
move(chess_state *game, const char *input)
{
	int crow, ccol, trow, tcol; 
	if (strlen(input) != 4) {
		printf("Invalid input\n\n");
		return;
	}

	/* Get current position */
	ccol = *input++ - 'a';
	crow = '8' - *input++;
	/* Get target position */
	tcol = *input++ - 'a';
	trow = '8' - *input++;

	if (ccol < 0 || ccol >= 8 || crow < 0 || crow >= 8 ||
	    tcol < 0 || tcol >= 8 || trow < 0 || trow >= 8) {
		printf("Invalid movement (out of range)\n\n");
		return;
	}

	if (read_at(game, crow, ccol) == 0) {
		printf("Can't move empty square\n\n");
		return;
	}

	if (valid_movement(game, crow, ccol, trow, tcol)) {
		move_piece(game, crow, ccol, trow, tcol);

		print_board(game);
		game->flags ^= BIT(WHITE_TURN);
	} else {
		printf("Invalid movement\n\n");
	}
}

void
cmd_exit(chess_state *game)
{
	printf("Please come back!\n");

	game->flags |= BIT(QUIT);
}

void
cmd_help(chess_state *game)
{
	printf("Beep boop, my name is Darien, the chess master, and here are the commands\nmy master allowed me to execute:\n"
	       "\thelp: display this\n"
	       "\texit/quit: quits the program :(\n"
	       "\tstatus: prints the current board state\n"
	       "\n");
}

void
cmd_status(chess_state *game)
{
	print_board(game);
}

