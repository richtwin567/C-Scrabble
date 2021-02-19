/**
 * @file game.h
 * @author Anakai Richards
 * @brief 
 * @version 0.1
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

struct Board
{
    int size;       // The size of the board grid
    int rowCount;   // The number of array rows for a size by size grid
    int lineLength; // The number of characters in each row
    char **grid;    // The game board array (pointer implementation)
};

struct BoardCoord
{
    int x; // The x coordinate (row)
    int y; // the y coordinate (column)
};

struct Move
{
    struct BoardCoord coords; // The coordinates for the move
    char letter;              // The letter to be placed at coords
};

struct Game
{
    int turn;        // The game turn count
    int p1score;     // The score for player 1
    int p2score;     // The score for player 2
    char p1name[30]; // Player 1's name
    char p2name[30]; // Player 2's name
};
