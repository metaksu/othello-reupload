#include <gtest/gtest.h>

#include "othello_board_t.hh"

TEST(OthelloBoardTest, OthelloBoardCreate)
{
    OthelloBoard b = othello_board_create();
    ASSERT_TRUE(b != NULL);
}

TEST(OthelloBoardTest, OthelloBoardGetPoint)
{
    OthelloBoard b = othello_board_create();
    ASSERT_TRUE(othello_board_get_point(b, 3, 3) == BLACK);
}

TEST(OthelloBoardTest, OthelloBoardSetPoint)
{
    OthelloBoard b = othello_board_create();
    othello_board_set_point(b, 3, 3, WHITE);
    ASSERT_TRUE(othello_board_get_point(b, 3, 3) == WHITE);
}

TEST(OthelloBoardTest, OthelloBoardCalculateLegalMoves)
{
    OthelloBoard b = othello_board_create();
    OthelloBoard blackMoves = othello_board_calculate_legal_moves(b, BLACK);
    OthelloBoard whiteMoves = othello_board_calculate_legal_moves(b, WHITE);
    ASSERT_TRUE(othello_board_get_point(blackMoves, 4, 2) == BLACK);
    ASSERT_TRUE(othello_board_get_point(blackMoves, 5, 3) == BLACK);
    ASSERT_TRUE(othello_board_get_point(blackMoves, 2, 4) == BLACK);
    ASSERT_TRUE(othello_board_get_point(blackMoves, 3, 5) == BLACK);
    ASSERT_TRUE(othello_board_get_point(whiteMoves, 3, 2) == WHITE);
    ASSERT_TRUE(othello_board_get_point(whiteMoves, 2, 3) == WHITE);
    ASSERT_TRUE(othello_board_get_point(whiteMoves, 5, 4) == WHITE);
    ASSERT_TRUE(othello_board_get_point(whiteMoves, 4, 5) == WHITE);
}

TEST(OthelloBoardTest, OthelloBoardMakeMove)
{
    OthelloBoard b = othello_board_create();
    othello_board_make_move(b, 4, 2, BLACK);
    ASSERT_TRUE(othello_board_get_point(b, 4, 2) == BLACK);
    ASSERT_TRUE(othello_board_get_point(b, 4, 3) == BLACK);
}

//Forgot you can't tell whether or not a pointer has been freed. Some people set it to null after freeing but that doesn't change the fact that I can only tell if its freeing memory with valgrind.
// TEST(OthelloBoardTest, OthelloBoardDestroy)
// {
//     OthelloBoard b = othello_board_create();
//     othello_board_destroy(b);
//     ASSERT_TRUE(b == NULL);
// }

TEST(OthelloBoardTest, OthelloBoardCreateCopy)
{
    //Have to test deep copy
    OthelloBoard b = othello_board_create();
    OthelloBoard c = othello_board_create_copy(b);
    othello_board_set_point(c, 0, 0, BLACK);
    ASSERT_TRUE((othello_board_get_point(c, 0, 0) != othello_board_get_point(b, 0, 0)) && (othello_board_get_point(c, 3, 3) == (othello_board_get_point(b, 3, 3))));
}

TEST(OthelloBoardTest, OthelloBoardGetScore)
{
    OthelloBoard b = othello_board_create();
    int bScore = othello_board_get_score(b, BLACK);
    int wScore = othello_board_get_score(b, WHITE);
    ASSERT_TRUE(bScore == 2);
    ASSERT_TRUE(wScore == 2);
    othello_board_make_move(b, 4, 2, BLACK);
    bScore = othello_board_get_score(b, BLACK);
    wScore = othello_board_get_score(b, WHITE);
    ASSERT_TRUE(bScore == 4);
    ASSERT_TRUE(wScore == 1);
}

TEST(OthelloBoardTest, OthelloBoardGetCurrentPlayer)
{
    OthelloBoard b = othello_board_create();
    ASSERT_TRUE(othello_board_get_current_player(b) == BLACK);
}

TEST(OthelloBoardTest, OthelloBoardChangeCurrentPlayer)
{
    OthelloBoard b = othello_board_create();
    ASSERT_TRUE(othello_board_get_current_player(b) == BLACK);
    othello_board_change_player(b);
    ASSERT_TRUE(othello_board_get_current_player(b) == WHITE);
}

TEST(OthelloBoardTest, OthelloBoardSetCurrentPlayer)
{
    OthelloBoard b = othello_board_create();
    ASSERT_TRUE(othello_board_get_current_player(b) == BLACK);
    othello_board_set_current_player(b, WHITE);
    ASSERT_TRUE(othello_board_get_current_player(b) == WHITE);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}