#ifndef _OTHELLO_GAME_TREE_H_
#define _OTHELLO_GAME_TREE_H_

#include <stdbool.h>
#include "othello_board_t.h"

typedef struct node *Node;

extern Node node_create(OthelloBoard ob);
extern void node_add_child(Node child, Node parent);
extern struct node **node_get_children(Node nd);
extern OthelloBoard node_get_board(Node nd);
extern int node_get_num_children(Node nd);
extern float node_get_pos_score(Node nd, enum piece who);
extern void node_print(Node nd, bool printChildren);
extern void node_destroy(Node toDestroy);

#endif