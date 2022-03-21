#include <stdlib.h>
#include <stdio.h>
#include "othello_board_t.h"
#include "othello_game_tree_t.h"
#include "othello_npc.h"
int main(int argc, char *argv[])
{
    printf("\e[1;1H\e[2J");
    othello_board_select_gamemode();
}