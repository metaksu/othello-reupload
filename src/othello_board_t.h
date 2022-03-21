#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_
#include <stdbool.h>

typedef struct othello_board *OthelloBoard;

//Used to indicate the current player of the game as well as the color of piece on a tile in the othelloboard
enum piece 
{
    BLACK, //0
    WHITE, //1
    NONE   //2
};

//Used to store the state of the board to provide an easy way of deciding if the board is ready to be played on or if the game is over
enum boardstate
{
    INITIALIZED,
    PLAYING,
    GAMEOVER
};

//public functions
extern OthelloBoard othello_board_create(void);
extern void othello_board_select_gamemode(void);
extern void othello_board_start_new_2p_game(OthelloBoard ob);
extern void othello_board_start_new_npc_game(OthelloBoard ob, int depth);
extern void othello_board_start_new_npc_npc_game(OthelloBoard ob, int depth1, int depth2);
extern int othello_board_get_point(OthelloBoard ob, int row, int column);
extern void othello_board_set_point(OthelloBoard ob, int row, int column, enum piece pi);
extern void othello_board_print_board(OthelloBoard ob);
extern void othello_board_print_score(OthelloBoard ob);
extern OthelloBoard othello_board_calculate_legal_moves(OthelloBoard ob, enum piece who);
extern bool othello_board_make_move(OthelloBoard ob, int row, int column, enum piece who);
extern void othello_board_destroy(OthelloBoard ob);
extern OthelloBoard othello_board_create_copy(OthelloBoard toCopy);
extern int othello_board_get_score(OthelloBoard ob, enum piece who);
extern enum piece othello_board_get_current_player(OthelloBoard ob);
extern void othello_board_change_player(OthelloBoard ob);
extern void othello_board_set_current_player(OthelloBoard ob, enum piece player);

#endif