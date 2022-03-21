#ifndef _OTHELLO_AI_H_
#define _OTHELLO_AI_H_
#include <stdbool.h>
#include <pthread.h>
#include "othello_board_t.h"
#include "othello_game_tree_t.h"

float coin_parity_heuristic(OthelloBoard ob, enum piece who);
int mobility_heuristic(OthelloBoard ob, enum piece who);
int corner_heuristic(OthelloBoard ob, enum piece who);
int stability_heuristic(OthelloBoard ob, enum piece who);
extern float eval_board(OthelloBoard ob, enum piece who);
extern void generate_decision_tree_from_node(Node nd, int depth);
extern OthelloBoard find_best_move_from_tree(Node tree, enum piece who);

#endif