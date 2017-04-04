#ifndef DCHESS_H
#define DCHESS_H

/* Board size */
#define BOARD_WIDTH	8
#define BOARD_HEIGHT	8

/* Extra */
#define COMMAND_COUNT	4

/* Colors */
#define RESET		"\x1B[0m"
#define RED		"\x1B[31m"
#define GREEN		"\x1B[32m"
#define BLUE		"\x1B[34m"

/* Flags */
#define NONE		0
#define FRIENDLY	1
#define ENEMY		2

/* Bit flags */
#define WHITE_PIECE	3
#define WHITE_TURN	0
#define QUIT		1

/* Bit manipluation */
#define BIT(n)			(1 << (n))

/* Byte manipulation */
#define BLOCK(l, r)		(((l) << 4) | (r))
#define WHITE_BLOCK(l, r)	(BLOCK(((l) | BIT(WHITE_PIECE)), ((r) | BIT(WHITE_PIECE))))
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

extern const command commands[COMMAND_COUNT];

/* Game functions */
void init_board(chess_state *game);
void print_block(BYTE block);
void print_board(chess_state *game);
BYTE read_at(chess_state *game, int row, int col);
void write_at(chess_state *game, int row, int col, BYTE piece);
int  relationship(BYTE p1, BYTE p2);
int  valid_movement(chess_state *game, int crow, int ccol, int trow, int tcol);
void move_piece(chess_state *game, int crow, int ccol, int trow, int tcol);
void move(chess_state *game, const char *input);
/* Commands */
void cmd_exit(chess_state *game);
void cmd_help(chess_state *game);
void cmd_status(chess_state *game);

#endif /* DCHESS_H */

