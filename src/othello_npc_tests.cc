#include <gtest/gtest.h>

#include "othello_npc.hh"

TEST(OthelloNPCTest, CoinParityHeuristic)
{
    OthelloBoard b = othello_board_create();
    ASSERT_TRUE(coin_parity_heuristic(b, BLACK) == 0);
    othello_board_make_move(b, 4, 2, BLACK);
    ASSERT_TRUE(coin_parity_heuristic(b, BLACK) == 60);
}

TEST(OthelloNPCTest, GenerateDecisionTreeFromNode)
{
    OthelloBoard b = othello_board_create();
    Node n = node_create(b);
    ASSERT_TRUE(node_get_num_children(n) == 0);
    generate_decision_tree_from_node(n, 2);
    ASSERT_TRUE(node_get_num_children(n) > 0);
    ASSERT_TRUE(node_get_num_children(node_get_children(n)[0]) > 0);
    ASSERT_TRUE(node_get_num_children(node_get_children(node_get_children(n)[0])[0]) > 0);
}

//BestMoveFromTree is tested by putting different depth tree npc's against another, the npc with higher depth searches always wins.


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}