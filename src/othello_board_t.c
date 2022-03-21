#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "othello_board_t.h"
#include "othello_game_tree_t.h"
#include "othello_npc.h"
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

const char *get_piece_name(enum piece p)
{
    switch (p)
    {
    case 0:
        return "BLACK";
        break;
    case 1:
        return "WHITE";
        break;
    case 2:
        return "NONE";
        break;
    default:
        break;
    }
}

//Timer callback
void timeup_cb()
{
    //time up
}

//Timer starter
static void *g_start_countdown(void *args)
{
    void (*callback_pointer)();

    int seconds = *((int *)args);

    callback_pointer = timeup_cb;
    sleep(seconds);
    (*callback_pointer)();

    pthread_exit(NULL);
}
//erase a line: printf("%c[2K", 27);

//Tile struct - Makes up one playable position on the board, holds the position of the tile and what is present on the tile
typedef struct tile
{
    int x;
    int y;
    enum piece p;
};

//Othello board - Comprised of 64 tiles, the othello board is the structure holding the current games state, as well as a struct for conducting hypothetical moves and evaluating
//This is the workhorse of the project
typedef struct othello_board
{
    //double pointer
    struct tile **board;
    enum boardstate state;
    enum piece current_player;
};

//Function that allocates memory for tiles and sets board pointer values to those tiles. Value of each tile is default "NONE"
void empty_board(OthelloBoard ob)
{
    for (int i = 0; i < 8; i++)
    {
        for (int k = 0; k < 8; k++)
        {
            //Using the tiles to keep track of coordinates
            struct tile *t = malloc(sizeof(*t));
            t->x = i;
            t->y = k;
            t->p = NONE;
            //k+i*8 is the formula to display the 2d othello grid in a 1d array
            ob->board[k + (i * 8)] = t;
        }
    }
}

//Function that sets up the 4 starter pieces of a standard othello game, this would usually be done after initializing and emptying the board
void initialize_pieces(OthelloBoard ob)
{
    othello_board_set_point(ob, 3, 3, BLACK);
    othello_board_set_point(ob, 4, 4, BLACK);
    othello_board_set_point(ob, 3, 4, WHITE);
    othello_board_set_point(ob, 4, 3, WHITE);
}

//The helper function of othello_board_create. Parameter empty allows for the creation of an empty or an initialized piece board.
OthelloBoard othello_board_init(bool empty)
{
    OthelloBoard ob;
    ob = malloc(sizeof(*ob));
    //Board is 8x8=64
    ob->board = malloc(sizeof(struct tile *) * 64);
    empty_board(ob);
    if (!empty)
    {
        initialize_pieces(ob);
    }
    ob->state = INITIALIZED;
    ob->current_player = BLACK;
    return ob;
}

//Wrapper function to initialize and return an Othelloboard
OthelloBoard othello_board_create(void)
{
    return othello_board_init(false);
}

//Function that returns true if the board is empty
bool check_board_empty(OthelloBoard ob)
{
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            if (othello_board_get_point(ob, row, column) != NONE)
            {
                return false;
            }
        }
    }
    return true;
}

//Function that return true if the player 'who' has a valid move
bool player_has_moves(OthelloBoard ob, enum piece who)
{
    OthelloBoard possibleMoves = othello_board_calculate_legal_moves(ob, who);
    if (check_board_empty(possibleMoves))
    {
        return false;
    }
    othello_board_destroy(possibleMoves);
    return true;
}

//Function that updated the OthelloBoard state if the game is over
void check_gameover(OthelloBoard ob)
{
    if (!(player_has_moves(ob, WHITE) && player_has_moves(ob, BLACK)))
    {
        ob->state = GAMEOVER;
    }
}

//Returns a 'piece' representing the current player of the board
enum piece othello_board_get_current_player(OthelloBoard ob)
{
    return ob->current_player;
}

//Changes Othelloboard->current_player to the opposite of the current
void othello_board_change_player(OthelloBoard ob)
{
    if (ob->current_player == WHITE)
    {
        ob->current_player = BLACK;
    }
    else if (ob->current_player == BLACK)
    {
        ob->current_player = WHITE;
    }
}

//Overrides the current player of 'ob' to the parameter 'player'
void othello_board_set_current_player(OthelloBoard ob, enum piece player)
{
    ob->current_player = player;
}

//Future helper function for the main menu
void othello_board_select_gamemode(void)
{
    int choice;
    int d1, d2;
    printf("Othello in C by Metin Aksu\nPlease pick your desired gamemode by typing in the number of the option you wish to choose:\n");
    OthelloBoard ob = othello_board_create();
    bool valid = true;
    do
    {
        printf("1) 2-Player on the same device\n2) 1-Player vs. Easy Difficulty NPC\n3) 1-Player vs. Medium Difficulty NPC\n4) 1-Player vs. Hard Difficulty NPC\n5) NPC vs NPC (user-defined-depth)\n");
        if (scanf("%d", &choice) == 1)
        {
            printf("\e[1;1H\e[2J");
            switch (choice)
            {
            case 1:
                othello_board_start_new_2p_game(ob);
                break;
            case 2:
                othello_board_start_new_npc_game(ob, 1);
                break;
            case 3:
                othello_board_start_new_npc_game(ob, 3);
                break;
            case 4:
                othello_board_start_new_npc_game(ob, 5);
                break;
            case 5:
                printf("Enter depth values separated by space (1-5) ex: 3 5\n");
                scanf("%i%i", &d1, &d2);
                othello_board_start_new_npc_npc_game(ob, d1, d2);
                break;
            default:
                printf("Not a valid option. Please try again.\n");
                valid = false;
                break;
            }
        }
        else
        {
            printf("Non-numeric input detected. Restart your game.\n");
            return 0;
        }
    } while (!valid);
    printf("\e[1;1H\e[2J");
}

//Helper random num generator function l = lower bound, r = upper
int generate_random(int l, int r)
{ //this will generate random number in range l and r
    int rand_num = (rand() % (r - l + 1)) + l;
    return rand_num;
}

void othello_board_start_new_npc_npc_game(OthelloBoard ob, int depth1, int depth2)
{
    Node top;
    enum piece npc1;
    printf("Creating new npc vs npc game; npc1 depth = %i, npc2 depth = %i.\n", depth1, depth2);
    printf("Colors are automatically assigned.\nQuit at any time with Ctrl+C.\n");
    othello_board_print_board(ob);
    ob->state = PLAYING;
    srand(time(NULL));
    int rand = generate_random(0, 100);
    if (rand > 50)
    {
        npc1 = BLACK;
        printf("NPC1 is BLACK (depth: %i)\n", depth1);
    }
    else
    {
        npc1 = WHITE;
        printf("NPC1 is WHITE (depth: %i)\n", depth2);
    }
    while (ob->state == PLAYING)
    {
        printf("%s's turn.\n", get_piece_name(ob->current_player));
        if (!player_has_moves(ob, ob->current_player))
        {
            printf("%s has no moves available. Skipping turn...\n", get_piece_name(ob->current_player));
            othello_board_change_player(ob);
            continue;
        }

        int row, col;
        bool legal = false;
        if (ob->current_player == npc1)
        {
            top = node_create(othello_board_create_copy(ob));
            generate_decision_tree_from_node(top, depth1);
            enum piece cplayer = ob->current_player;
            ob = othello_board_create_copy(find_best_move_from_tree(top, cplayer));
            ob->current_player = cplayer;
        }
        else
        {
            top = node_create(othello_board_create_copy(ob));
            generate_decision_tree_from_node(top, depth2);
            enum piece cplayer = ob->current_player;
            ob = othello_board_create_copy(find_best_move_from_tree(top, cplayer));
            ob->current_player = cplayer;
        }
        othello_board_print_board(ob);
        othello_board_print_score(ob);
        othello_board_change_player(ob);
        check_gameover(ob);
        node_destroy(top);
        sleep(1);
    }
}

//Begins new game against cpu opponent using heuristic analysis to make moves. 'depth' indicates the depth of the search tree the cpu can utilize to inform its algorithm.
void othello_board_start_new_npc_game(OthelloBoard ob, int depth)
{
    Node top;
    enum piece playerColor;
    printf("Creating new npc game; depth = %i.\n", depth);
    printf("Please decide your color.\nPlay a piece by entering the desired row followed by the desired column by pressing enter after each input or by putting a space between them.\nQuit at any time with Ctrl+C.\n");
    othello_board_print_board(ob);
    ob->state = PLAYING;
    srand(time(NULL));
    int rand = generate_random(0, 100);
    if (rand > 50)
    {
        playerColor = BLACK;
        printf("You are black\n");
    }
    else
    {
        playerColor = WHITE;
        printf("You are white\n");
    }
    while (ob->state == PLAYING)
    {
        top = NULL;
        printf("%s's turn.\n", get_piece_name(ob->current_player));
        if (!player_has_moves(ob, ob->current_player))
        {
            printf("%s has no moves available. Skipping turn...\n", get_piece_name(ob->current_player));
            othello_board_change_player(ob);
            continue;
        }

        int row, col;
        bool legal = false;
        if (ob->current_player == playerColor)
        {
            do
            {
                scanf("%i%i", &row, &col);
                legal = othello_board_make_move(ob, row, col, ob->current_player);
                if (!legal)
                {
                    printf("Illegal move, try again.\n");
                    othello_board_print_board(ob);
                }
            } while (!legal);
        }
        else
        {
            top = node_create(othello_board_create_copy(ob));
            generate_decision_tree_from_node(top, depth);
            enum piece cplayer = ob->current_player;
            ob = othello_board_create_copy(find_best_move_from_tree(top, cplayer));
            ob->current_player = cplayer;
        }
        othello_board_print_board(ob);
        othello_board_print_score(ob);
        othello_board_change_player(ob);
        check_gameover(ob);
        if (top != NULL)
        {
            node_destroy(top);
        }
    }
}

//Begins new game with 2 human players. Controls all aspects of the game and utilizes the games state to decide what should be shown
void othello_board_start_new_2p_game(OthelloBoard ob)
{
    //By default, 2 player local game
    //Black goes first
    printf("Creating new two-player game...\n");
    othello_board_print_board(ob);
    printf("Please decide your color.\nPlay a piece by entering the desired row followed by the desired column by pressing enter after each input or by putting a space between them.\nQuit at any time with Ctrl+C.\n");
    //printf("Please enter the amount of seconds per turn: \n");
    ob->state = PLAYING;
    while (ob->state == PLAYING)
    {
        printf("%s's turn.\n", get_piece_name(ob->current_player));
        if (!player_has_moves(ob, ob->current_player))
        {
            printf("%s has no moves available. Skipping turn...\n", get_piece_name(ob->current_player));
            othello_board_change_player(ob);
            continue;
        }

        int row, col;
        bool legal = false;
        do
        {
            scanf("%i%i", &row, &col);
            legal = othello_board_make_move(ob, row, col, ob->current_player);
            if (!legal)
            {
                printf("Illegal move, try again.\n");
                othello_board_print_board(ob);
            }
        } while (!legal);

        othello_board_print_board(ob);
        othello_board_print_score(ob);
        othello_board_change_player(ob);
        check_gameover(ob);
    }
}

//Returns the point present at a row and column of a board
int othello_board_get_point(OthelloBoard ob, int row, int column)
{
    return ob->board[(row * 8) + column]->p;
}

//Hard sets the point at a row and column of a board to 'pi'
void othello_board_set_point(OthelloBoard ob, int row, int column, enum piece pi)
{
    ob->board[(row * 8) + column]->p = pi;
}

//Check if there is a color WHO on the line starting at row, column or anywhere further by adding dir_row, dir_col to row,column (recursive)
bool check_line_end_match(OthelloBoard ob, int dir_row, int dir_col, int row, int column, enum piece who)
{
    if (othello_board_get_point(ob, row, column) == who)
    {
        return true;
    }
    if (othello_board_get_point(ob, row, column) == NONE)
    {
        return false;
    }
    if ((row + dir_row < 0) || (row + dir_row > 7))
    {
        return false;
    }
    if ((column + dir_col < 0) || (column + dir_col > 7))
    {
        return false;
    }
    return check_line_end_match(ob, dir_row, dir_col, row + dir_row, column + dir_col, who);
}

//Check if the position at row,column contains the opposite of 'who' on the board and if the line indicated by a direction ends with the current players color.
bool evaluate_move_with_direction(OthelloBoard ob, int row, int column, int dir_row, int dir_col, enum piece who)
{
    enum piece other;
    if (who == BLACK)
    {
        other = WHITE;
    }
    else if (who == WHITE)
    {
        other = BLACK;
    }
    else
    {
        //error
        return false;
    }
    //Walking off the board
    if ((row + dir_row < 0) || (row + dir_row > 7))
    {
        return false;
    }
    if ((column + dir_col < 0) || (column + dir_col > 7))
    {
        return false;
    }
    //adjacent piece is not the other player
    if (othello_board_get_point(ob, row + dir_row, column + dir_col) != other)
    {
        return false;
    }
    //adjacent piece is an edge or corner
    if ((row + dir_row + dir_row < 0) || (row + dir_row + dir_row > 7))
    {
        return false;
    }
    if ((column + dir_col + dir_col < 0) || (column + dir_col + dir_col > 7))
    {
        return false;
    }
    if (check_line_end_match(ob, dir_row, dir_col, row + dir_row + dir_row, column + dir_col + dir_col, who))
    {
        return true;
    }
}

//Return true if the hypothetical move indicated by row, column, and who is a legal move for who
bool move_is_legal(OthelloBoard ob, int row, int column, enum piece who)
{
    if (othello_board_get_point(ob, row, column) == NONE)
    {
        bool nw = evaluate_move_with_direction(ob, row, column, -1, -1, who);
        bool nn = evaluate_move_with_direction(ob, row, column, -1, 0, who);
        bool ne = evaluate_move_with_direction(ob, row, column, -1, 1, who);

        bool ww = evaluate_move_with_direction(ob, row, column, 0, -1, who);
        bool ee = evaluate_move_with_direction(ob, row, column, 0, 1, who);

        bool sw = evaluate_move_with_direction(ob, row, column, 1, -1, who);
        bool ss = evaluate_move_with_direction(ob, row, column, 1, 0, who);
        bool se = evaluate_move_with_direction(ob, row, column, 1, 1, who);

        if (nw || nn || ne || ww || ee || sw || ss || se)
        {
            return true;
        }
    }
    return false;
}

//Returns an OthelloBoard representing all the legal moves 'who' can make on 'ob'
OthelloBoard othello_board_calculate_legal_moves(OthelloBoard ob, enum piece who)
{
    OthelloBoard legalmoves = othello_board_init(true);
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            if (move_is_legal(ob, row, column, who))
            {
                othello_board_set_point(legalmoves, row, column, who);
            }
        }
    }
    return legalmoves;
}

//recursively flips line until end point under assumption that the move was legal
bool flip_line_with_direction(OthelloBoard ob, int row, int column, int dir_row, int dir_col, enum piece who)
{
    enum piece other;
    if (who == BLACK)
    {
        other = WHITE;
    }
    else if (who == WHITE)
    {
        other = BLACK;
    }
    else
    {
        //error
        return false;
    }
    if (othello_board_get_point(ob, row + dir_row, column + dir_col) == other)
    {
        othello_board_set_point(ob, row + dir_row, column + dir_col, who);
        flip_line_with_direction(ob, row + dir_row, column + dir_col, dir_row, dir_col, who);
    }
    else if (othello_board_get_point(ob, row + dir_row, column + dir_col) == who)
    {
        return true;
    }
    else if (othello_board_get_point(ob, row + dir_row, column + dir_col) == NONE)
    {
        //error
        return false;
    }
}

//wrapper function for a player to perform moves and for the board to flip pieces accordingly under the premise the move was legal.
void make_move(OthelloBoard ob, int row, int column, enum piece who)
{
    othello_board_set_point(ob, row, column, who);
    bool nw = evaluate_move_with_direction(ob, row, column, -1, -1, who);
    if (nw)
    {
        flip_line_with_direction(ob, row, column, -1, -1, who);
    }
    bool nn = evaluate_move_with_direction(ob, row, column, -1, 0, who);
    if (nn)
    {
        flip_line_with_direction(ob, row, column, -1, 0, who);
    }
    bool ne = evaluate_move_with_direction(ob, row, column, -1, 1, who);
    if (ne)
    {
        flip_line_with_direction(ob, row, column, -1, 1, who);
    }

    bool ww = evaluate_move_with_direction(ob, row, column, 0, -1, who);
    if (ww)
    {
        flip_line_with_direction(ob, row, column, 0, -1, who);
    }
    bool ee = evaluate_move_with_direction(ob, row, column, 0, 1, who);
    if (ee)
    {
        flip_line_with_direction(ob, row, column, 0, 1, who);
    }

    bool sw = evaluate_move_with_direction(ob, row, column, 1, -1, who);
    if (sw)
    {
        flip_line_with_direction(ob, row, column, 1, -1, who);
    }
    bool ss = evaluate_move_with_direction(ob, row, column, 1, 0, who);
    if (ss)
    {
        flip_line_with_direction(ob, row, column, 1, 0, who);
    }
    bool se = evaluate_move_with_direction(ob, row, column, 1, 1, who);
    if (se)
    {
        flip_line_with_direction(ob, row, column, 1, 1, who);
    }
}

//Highest level wrapper function to make a move, return true if the move was declared legal, returns false otherwise. If the move was legal, this will make all the required changes to the board.
bool othello_board_make_move(OthelloBoard ob, int row, int column, enum piece who)
{
    if (move_is_legal(ob, row, column, who))
    {
        make_move(ob, row, column, who);
        return true;
    }

    return false;
}

//Returns the score based on the number of pieces owned by 'who'
int othello_board_get_score(OthelloBoard ob, enum piece who)
{
    int score = 0;
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            if (othello_board_get_point(ob, row, column) == who)
            {
                score++;
            }
        }
    }
    return score;
}

//Prints the score of both players in a fashion that fits the board print
void othello_board_print_score(OthelloBoard ob)
{
    printf("Black: %i           White: %i\n", othello_board_get_score(ob, BLACK), othello_board_get_score(ob, WHITE));
}

//clean up boards
void othello_board_destroy(OthelloBoard ob)
{
    int i;
    struct tile *t;
    if (ob != NULL)
    {
        for (i = 0; i < 64; i++)
        {
            if (t != NULL)
            {
                t = ob->board[i];
                free(t);
                t = NULL;
            }
        }
        if (ob->board != NULL)
        {
            free(ob->board);
            ob->board = NULL;
        }
        free(ob);
        ob = NULL;
    }
}

//print board in neat matter
void othello_board_print_board(OthelloBoard ob)
{
    printf("[ ][0][1][2][3][4][5][6][7]\n");
    for (int row = 0; row < 8; row++)
    {
        printf("[%i]", row);
        for (int column = 0; column < 8; column++)
        {
            printf("[");
            enum piece pi = othello_board_get_point(ob, row, column);
            if (pi == NONE)
            {
                printf(" ");
            }
            else if (pi == BLACK)
            {
                printf("B");
            }
            else if (pi == WHITE)
            {
                printf("W");
            }
            printf("]");
        }
        printf("\n");
    }
}

//Deep copying to a pre-initialized board
void othello_board_copy(OthelloBoard copy, OthelloBoard original)
{
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            othello_board_set_point(copy, row, column, othello_board_get_point(original, row, column));
        }
    }
    switch (original->state)
    {
    case INITIALIZED:
        copy->state = INITIALIZED;
        break;
    case PLAYING:
        copy->state = PLAYING;
        break;
    case GAMEOVER:
        copy->state = GAMEOVER;
        break;
    default:
        break;
    }
    switch (original->current_player)
    {
    case WHITE:
        copy->current_player = WHITE;
        break;
    case BLACK:
        copy->current_player = BLACK;
        break;
    case NONE:
        copy->current_player = NONE;
        break;
    default:
        break;
    }
}

//Wrapper function for create_copy, returns a deep copy of 'toCopy'
OthelloBoard othello_board_create_copy(OthelloBoard toCopy)
{
    OthelloBoard copy = othello_board_create();
    othello_board_copy(copy, toCopy);
    return copy;
}