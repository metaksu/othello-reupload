#include "othello_npc.h"

//helper function to quickly return the opposite player of 'who'
enum piece get_other(enum piece who)
{
    if (who == WHITE)
    {
        return BLACK;
    }
    else if (who == BLACK)
    {
        return WHITE;
    }
}

//difference in coins between the max (player of interest) and min player
float coin_parity_heuristic(OthelloBoard ob, enum piece who)
{
    int max_player_score = othello_board_get_score(ob, who);
    int min_player_score = othello_board_get_score(ob, get_other(who));
    return (100 * (max_player_score - min_player_score) / (max_player_score + min_player_score));
}

//eval_board will use all the individual heuristic functions and eventually take 'weights' as a parameter to customize the effect of any one heuristic on the overall evaluation
float eval_board(OthelloBoard ob, enum piece who)
{
    return coin_parity_heuristic(ob, who);
}

//uses a starting node containing an othelloboard to generate 'depth' levels of possible decisions of the game represented by their own node containing a hypothethical othelloboard that can be evaluated.
void generate_decision_tree_from_node(Node nd, int depth)
{
    OthelloBoard possibleMoves = othello_board_calculate_legal_moves(node_get_board(nd), othello_board_get_current_player(node_get_board(nd)));
    for (int row = 0; row < 8; row++)
    {
        for (int column = 0; column < 8; column++)
        {
            if (othello_board_get_point(possibleMoves, row, column) != NONE)
            {
                OthelloBoard copy = othello_board_create_copy(node_get_board(nd));
                othello_board_make_move(copy, row, column, othello_board_get_point(possibleMoves, row, column));
                Node child = node_create(copy);
                node_add_child(child, nd);
                othello_board_change_player(copy);
            }
        }
    }
    if (depth > 0)
    {
        struct node **children = node_get_children(nd);
        for (int i = 0; i < node_get_num_children(nd); i++)
        {
            generate_decision_tree_from_node((Node)children[i], depth - 1);
        }
    }
}

//Recursive function to search for the highest score in the subtree for player 'who'.
float highest_score_from_subtree(Node tree, float highest, enum piece who)
{
    struct node **children = node_get_children(tree);
    if (node_get_num_children(tree) > 0)
    {
        for (int i = 0; i < node_get_num_children(tree); i++)
        {

            float high = highest_score_from_subtree(children[i], node_get_pos_score(children[i], who), who);
            if (high > highest)
            {
                return high;
            }
            else
            {
                return highest;
            }
        }
    }
    else
    {
        if (node_get_pos_score(tree, who) > highest)
        {
            return node_get_pos_score(tree, who);
        }
        return highest;
    }
}

//Separates the tree into n many subtrees where n is the number of moves 'who' can make from the starting othelloboard contained in 'tree'
//Then finds the subtree that contains the node with the highest score for 'who'
OthelloBoard find_best_move_from_tree(Node tree, enum piece who)
{
    int tree_with_highest_score = 0;
    float highest_score = 0;
    struct node **children = node_get_children(tree);
    float highScores[node_get_num_children(tree)];
    for (int i = 0; i < node_get_num_children(tree); i++)
    {
        float high = highest_score_from_subtree(children[i], node_get_pos_score(children[i], who), who);
        if (high > highest_score)
        {
            tree_with_highest_score = i;
            highest_score = high;
        }
    }
    return node_get_board(children[tree_with_highest_score]);
}
