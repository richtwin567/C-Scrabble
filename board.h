#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

static int isOnBoard(struct Board board, int x, int y);
static int getColumnIndex(int userColumnInput);
static int getRowIndex(int userRowInput);
static int getLogicalRow(int index);
static int getLogicalCol(int index);
static void initGrid(struct Board board);
static void calcRowsAndLines(int const LOGICAL_SIZE, int *ROW_COUNT, int *LINE_LENGTH);
struct Board getNewBoard(struct Board board, int const LOGICAL_SIZE);
void drawBoard(struct Board board);
int makePlay(struct Board board, struct Move move);
static void startBoard(struct Board board, int x);
static void setPosition(struct Board board, struct Move move);
char getPosition(struct Board board, struct BoardCoord);
static int isPositionEmpty(struct Board board, struct BoardCoord);

/* int main()
{
    struct Board board;              // the game board
    int const LOGICAL_GRID_SIZE = 8; // the size of the game board

    board = getNewBoard(board, LOGICAL_GRID_SIZE);

    drawBoard(board);
    return 0;
} */

static int isOnBoard(struct Board board, int row, int col)
{
    return (row <= board.size && row >= 1) && (col <= board.size && col >= 1);
}

static int getColumnIndex(int userColumnInput)
{
    // Calculate the accurate column index by expressing the column
    // as the nth value in an arithmetic series (nth = a+(n-1)d)
    return 4 + (4 * (userColumnInput - 1));
}

static int getRowIndex(int userRowInput)
{
    // calculate the accurate row index by expressing the row
    // as the nth value in an arithmetic series  (nth = a+(n-1)d)
    return 2 + (2 * (userRowInput - 1));
}

static int getLogicalRow(int index)
{
    // The reverse of getRowIndex
    // Uses the array index to get the row number as seen by the user
    return ((index - 2) / 2) + 1;
}

static int getLogicalCol(int index)
{
    // The reverse of getLogicalCol.
    // Uses the array index to the column number as seen by the user
    return ((index - 4) / 4) + 1;
}

static void initGrid(struct Board board)
{
    char rowsep[board.lineLength];
    char columnlines[board.lineLength];
    char colheadings[board.lineLength];

    // build row separator string
    // build column lines
    // build column headers

    rowsep[0] = ' ';
    rowsep[1] = ' ';
    columnlines[0] = ' ';
    columnlines[1] = ' ';
    colheadings[0] = ' ';
    colheadings[1] = ' ';

    int i;
    for (i = 2; i < board.lineLength - 2; i += 4)
    {
        rowsep[i] = '+';
        rowsep[i + 1] = '-';
        rowsep[i + 2] = '-';
        rowsep[i + 3] = '-';
        columnlines[i] = '|';
        columnlines[i + 1] = ' ';
        columnlines[i + 2] = ' ';
        columnlines[i + 3] = ' ';
        colheadings[i] = ' ';
        colheadings[i + 1] = ' ';
        colheadings[i + 2] = getLogicalCol(i + 2) + '0';
        colheadings[i + 3] = ' ';
    }
    rowsep[i] = '+';
    rowsep[i + 1] = '\0';
    columnlines[i] = '|';
    columnlines[i + 1] = '\0';
    colheadings[i] = ' ';
    colheadings[i + 1] = '\0';

    // Copy the empty grid lines and labels into the grid
    strcpy(board.grid[0], colheadings);
    for (i = 1; i < board.rowCount - 1; i += 2)
    {
        strcpy(board.grid[i], rowsep);
        columnlines[0] = getLogicalRow(i + 1) + '0';
        strcpy(board.grid[i + 1], columnlines);
    }
    strcpy(board.grid[i], rowsep);

    return;
}

static void calcRowsAndLines(int const LOGICAL_SIZE, int *ROW_COUNT, int *LINE_LENGTH)
{
    *ROW_COUNT = (LOGICAL_SIZE + 1) * 2;
    *LINE_LENGTH = 4 * (LOGICAL_SIZE + 1);
}

struct Board getNewBoard(struct Board board, int const LOGICAL_SIZE)
{
    //creates a brand new blank board. Returns a pointer to the array
    int ROW_COUNT;
    int LINE_LENGTH;
    calcRowsAndLines(LOGICAL_SIZE, &ROW_COUNT, &LINE_LENGTH);
    board.size = LOGICAL_SIZE;
    board.rowCount = ROW_COUNT;
    board.lineLength = LINE_LENGTH;

    // allocate the necessary memory
    board.grid = calloc(board.rowCount, sizeof *board.grid);
    for (int i = 0; i < board.rowCount; i++)
    {
        board.grid[i] = calloc(board.lineLength, sizeof *(board.grid[i]));
    }
    initGrid(board);
    startBoard(board, 10);

    return board;
} //EndFunction getNewBoard

void drawBoard(struct Board board)
{
    //This function prints out the board that was passed. Returns void
    for (int i = 0; i < board.rowCount; i++)
    {
        printf("%s\n", board.grid[i]);
    }
    return;
} //EndFunction drawBoard

/**
 * @brief Place the character at the specified (x,y) location
 * 
 * @param board the board to make the play on
 * @param x the row
 * @param y the column
 * @param c the character to place
 * @return int - Returns 1 if the play was made succesfully. Returns 0 if the requested position is not empty. Returns -1 if the requested position does not exist.
 */
int makePlay(struct Board board, struct Move move)
{
    if (isOnBoard(board, move.coords.x, move.coords.y))
    {
        if (isPositionEmpty(board, move.coords))
        {
            setPosition(board, move);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }

    return 1;
}

static void startBoard(struct Board board, int times)
{
    srand(time(0));
    struct Move move;
    for (int i = 0; i < times; i++)
    {
        move.letter = (char)(rand() % ('z' + 1 - 'a')) + 'a';
        do
        {
            move.coords.x = (rand() % board.size) + 1;
            move.coords.y = (rand() % board.size) + 1;
        } while (makePlay(board, move) == 0);
    }
}

static int isPositionEmpty(struct Board board, struct BoardCoord coords)
{

    return isblank(getPosition(board, coords));
}

static void setPosition(struct Board board, struct Move move)
{
    board.grid[getRowIndex(move.coords.x)][getColumnIndex(move.coords.y)] = move.letter;
}

char getPosition(struct Board board, struct BoardCoord coords)
{
    return board.grid[getRowIndex(coords.x)][getColumnIndex(coords.y)];
}