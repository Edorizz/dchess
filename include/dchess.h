#ifndef DCHESS_H
#define DCHESS_H

/* Board size */
#define BOARD_WIDTH	8
#define BOARD_HEIGHT	8

/* Extra */
#define MAX_INPUT	10
#define COMMAND_COUNT	2

/* Bit flags */
#define WHITE_PIECE	3
#define WHITE_TURN	0
#define QUIT		1

/* Bit manipluation */
#define BIT(n)			(1 << (n))

/* Byte manipulation */
#define BLOCK(l, r)		(((l) << 4) | (r))
#define WHITE_BLOCK(l, r)	(BLOCK(((l) | BIT(WHITE_PIECE)), ((r) | BIT(WHITE_PIECE)))) /* ONLY USED FOR INITIALIZATION */
#define LEFT_PIECE(b)		(((b) & 0xF0) >> 4)
#define RIGHT_PIECE(b)		((b) & 0x0F)

typedef unsigned char BYTE;

enum chess_pieces { PAWN = 1, ROOK, KNIGHT, BISHOP, QUEEN, KING, MAX_PIECES };

typedef struct {
	BYTE board[BOARD_HEIGHT][BOARD_WIDTH / 2];
	BYTE flags;
} chess_state;

typedef struct {
	char *name;
	void(*cmd)(chess_state*);
} command;

/* Game functions */
void init_board(chess_state *game);
void print_block(BYTE block);
void print_board(chess_state *game);
/* Commands */
void cmd_exit(chess_state *game);

#endif /* DCHESS_H */

