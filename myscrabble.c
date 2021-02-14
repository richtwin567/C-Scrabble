#include "letters.h"
#include "board.h"
#include "dictionary.h"

struct Move getPlayerMove();

int main(int argc, char const *argv[])
{
    struct Board board;              // the game board
    int const LOGICAL_GRID_SIZE = 8; // the size of the game board

    board = getNewBoard(board, LOGICAL_GRID_SIZE);
    drawBoard(board);

    while (1)
    {
        struct Move nextMove = getPlayerMove();
        switch (makePlay(board, nextMove))
        {
        case 0:
            printf("The requested coordinates (%d,%d) are not empty", nextMove.coords.x, nextMove.coords.y);
            break;
        case -1:
            printf("The requested coordinates (%d,%d) do not exist on a %dx%d board.", nextMove.coords.x, nextMove.coords.y, board.size, board.size);
            break;
        default:
            break;
        }
        drawBoard(board);
    }

    return 0;
}

struct Move getPlayerMove()
{
    static int turn = 0;
    char *choice = calloc(5, sizeof *choice);
    struct BoardCoord coords;
    struct Move move;
    printf("\n[Player %d]\nPlease enter the coordinates of your move in xy (x is the row, y is the column) format or 'quit' to quit: ", (turn % 2) + 1);
    fgets(choice, 5, stdin);
    if (strcmp(choice, "quit") == 0)
    {
        printf("Thank you for playing this game");
        exit(0);
    }

    int success = sscanf(choice, "%1d%1d", &move.coords.x, &move.coords.y);
    printf("\nsuccess %d %d %d\n", success, move.coords.x, move.coords.y);
    do
    {
        printf("\nPlease the letter you would like to place at the coordinates: ");

        move.letter=fgetc(stdin);
        fgetc(stdin);
    } while (!isLetter(move.letter));
    turn++;

    return move;
}

int calculateScore(char *word)
{
    int total = 0;
    int i;
    for (i = 0; word[i] != '\0'; i++)
    {
        total += determineLetterValue(word[i]);
    }
    if (i < 4)
    {
        return -1;
    }

    return total;
}