#include "othello_board_t.h"
#include "othello_game_tree_t.h"
#include <stdlib.h>
#include <stdio.h>
#include "othello_npc.h"
#include <stdbool.h>

//Basic Tree ADT
//Extended to the othello game, as each node contains an instance of an othelloboard and the score of each player
//When implemented fully, nodes will have n children where n is the number of possible moves you can make from the current othelloboard with a specified player
// and each node represents that hypothetical move, until a specified depth is reached.
typedef struct node
{
    struct node **children;
    struct node *parent;
    int num_children;
    OthelloBoard ob;
    float pos_score_white;
    float pos_score_black;
};

//Node memory allocation and initialization
Node node_init(OthelloBoard ob)
{
    Node nd;
    nd = malloc(sizeof(*nd));
    nd->children = malloc(sizeof(struct node *));
    nd->num_children = 0;
    nd->ob = ob;
    nd->pos_score_white = eval_board(ob, WHITE);
    nd->pos_score_black = eval_board(ob, BLACK);
}
//Wrapper function to return a node containing 'ob'
Node node_create(OthelloBoard ob)
{
    return node_init(ob);
}

//Destroy node recursively
void node_destroy(Node toDestroy)
{
    if (toDestroy == NULL)
    {
        return;
    }
    if (toDestroy->ob != NULL)
    {
        othello_board_destroy(toDestroy->ob);
        toDestroy->ob = NULL;
    }
    if (toDestroy->num_children > 0)
    {
        for (int i = 0; i < toDestroy->num_children; i++)
        {
            if (toDestroy->children[i] != NULL)
            {
                node_destroy(toDestroy->children[i]);
                toDestroy->children[i] = NULL;
            }
        }
    }
    free(toDestroy);
}

//Adds a child to a node 'parent'
void node_add_child(Node child, Node parent)
{
    parent->num_children++;
    parent->children = realloc(parent->children, sizeof(struct node *) * parent->num_children);
    parent->children[parent->num_children - 1] = child;
    child->parent = parent;
}

//returns the list of children of a given node
struct node **node_get_children(Node nd)
{
    return nd->children;
}

//returns the board of a given node
OthelloBoard node_get_board(Node nd)
{
    return nd->ob;
}

//returns the number of children of a given node
int node_get_num_children(Node nd)
{
    return nd->num_children;
}

//return the score of 'nd' given 'who'
float node_get_pos_score(Node nd, enum piece who)
{
    if (who == BLACK)
        return nd->pos_score_black;
    else if (who == WHITE)
        return nd->pos_score_white;
}

//Print the node (DEBUG ONLY)
void node_print(Node nd, bool printChildren)
{
    printf("Parent: (B)%f, (W)%f\n", nd->pos_score_black, nd->pos_score_white);
    if (printChildren)
    {
        if (nd->num_children > 0)
        {
            printf("Children: ");
        }
        for (int i = 0; i < nd->num_children; i++)
        {
            node_print(nd->children[i], true);
        }
    }
    printf("\n");
}