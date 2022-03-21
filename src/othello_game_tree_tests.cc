#include <gtest/gtest.h>

#include "othello_game_tree_t.hh"

TEST(OthelloGameTreeTest, NodeCreate)
{
    OthelloBoard b = othello_board_create();
    Node n = node_create(b);
    ASSERT_TRUE(n != NULL);
}

TEST(OthelloGameTreeTest, NodeAddGetChild)
{
    OthelloBoard b = othello_board_create();
    Node n = node_create(b);
    OthelloBoard c = othello_board_create();
    Node child = node_create(c);
    node_add_child(child, n);
    ASSERT_TRUE(node_get_children(n)[0] == child);
}

TEST(OthelloGameTreeTest, NodeGetBoard)
{
    OthelloBoard b = othello_board_create();
    Node n = node_create(b);
    ASSERT_TRUE(node_get_board(n) == b);
}

TEST(OthelloGameTreeTest, NodeGetNumChild)
{
    OthelloBoard b = othello_board_create();
    Node n = node_create(b);
    OthelloBoard c = othello_board_create();
    Node child = node_create(c);
    node_add_child(child, n);
    ASSERT_TRUE(node_get_num_children(n) == 1);
}

TEST(OthelloGameTreeTest, NodeGetPosScore)
{
    OthelloBoard b = othello_board_create();
    Node n = node_create(b);
    ASSERT_TRUE(node_get_pos_score(n, BLACK) == 0);
    ASSERT_TRUE(node_get_pos_score(n, WHITE) == 0);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}