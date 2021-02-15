/**
 * @file board.c
 * @author Anakai Richards
 * @brief 
 * @version 0.1
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// TODO - game instructions, player names, confirm quit

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "dictionary.h"
#include "letters.h"

int main()
{

    struct Board board;                        // the game board
    int const LOGICAL_SIZE = 8;                // the size of the game board
    struct Dictionary dict = loadDictionary(); // dictionary of valid words
    struct Game gameData;                      // keeps track of the game
    struct Move move;                          // Stores moves made in the game
    struct BoardCoord coords;                  // Stores specific board coordinates

    int isOnBoard(int row, int col)
    {
        return (row <= board.size && row >= 1) && (col <= board.size && col >= 1);
    }

    int getColumnIndex(int userColumnInput)
    {
        // Calculate the accurate column index by expressing the column
        // as the nth value in an arithmetic series (nth = a+(n-1)d)
        return 4 + (4 * (userColumnInput - 1));
    }

    int getRowIndex(int userRowInput)
    {
        // calculate the accurate row index by expressing the row
        // as the nth value in an arithmetic series  (nth = a+(n-1)d)
        return 2 + (2 * (userRowInput - 1));
    }

    int getVirtualRow(int index)
    {
        // The reverse of getRowIndex
        // Uses the array index to get the row number as seen by the user
        return ((index - 2) / 2) + 1;
    }

    int getVirtualCol(int index)
    {
        // The reverse of getVirtualCol.
        // Uses the array index to the column number as seen by the user
        return ((index - 4) / 4) + 1;
    }

    void initGrid()
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
            colheadings[i + 2] = getVirtualCol(i + 2) + '0';
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
            columnlines[0] = getVirtualRow(i + 1) + '0';
            strcpy(board.grid[i + 1], columnlines);
        }
        strcpy(board.grid[i], rowsep);

        return;
    }

    void calcRowsAndCols()
    {
        board.rowCount = (LOGICAL_SIZE + 1) * 2;
        board.lineLength = 4 * (LOGICAL_SIZE + 1);
    }
    char getPosition(struct BoardCoord coords)
    {
        return board.grid[getRowIndex(coords.x)][getColumnIndex(coords.y)];
    }

    int isPositionEmpty(struct BoardCoord coords)
    {

        return isblank(getPosition(coords));
    }

    void setPosition(struct Move move)
    {
        board.grid[getRowIndex(move.coords.x)][getColumnIndex(move.coords.y)] = move.letter;
    }

    /**
 * @brief Place the character at the specified (x,y) location
 * 
 * @param board the board to make the play on
 * @param x the row
 * @param y the column
 * @param c the character to place
 * @return int - Returns 1 if the play was made succesfully. Returns 0 if the requested position is not empty. Returns -1 if the requested position does not exist.
 */
    int makePlay(struct Move move)
    {
        if (isOnBoard(move.coords.x, move.coords.y))
        {
            if (isPositionEmpty(move.coords))
            {
                setPosition(move);
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

    void startBoard()
    {
        srand(time(0));
        struct Move randmove;
        for (int i = 0; i < 10; i++)
        {
            randmove.letter = (char)(rand() % ('z' + 1 - 'a')) + 'a';
            do
            {
                randmove.coords.x = (rand() % board.size) + 1;
                randmove.coords.y = (rand() % board.size) + 1;
            } while (makePlay(randmove) == 0);
        }
    }

    void getNewBoard()
    {
        //creates a brand new blank board. Returns a pointer to the array

        calcRowsAndCols();
        board.size = LOGICAL_SIZE;

        // allocate the necessary memory
        board.grid = calloc(board.rowCount, sizeof *board.grid);
        for (int i = 0; i < board.rowCount; i++)
        {
            board.grid[i] = calloc(board.lineLength, sizeof *(board.grid[i]));
        }
        initGrid();
        startBoard();

    } //EndFunction getNewBoard

    void drawBoard()
    {
        //This function prints out the board that was passed. Returns void
        for (int i = 0; i < board.rowCount; i++)
        {
            printf("%s\n", board.grid[i]);
        }
        return;
    } //EndFunction drawBoard

    void announceWinner()
    {
        printf("\n============================\n");
        printf("\nThe Scores are: ");
        printf("\nPlayer 1:\t%d", gameData.p1score);
        printf("\nPlayer 2:\t%d\n", gameData.p2score);
        if (gameData.p1score > gameData.p2score)
        {
            printf("\nThe winner is Player 1!");
        }
        else if (gameData.p1score < gameData.p2score)
        {
            printf("\nThe winner is Player 2!");
        }
        else if (gameData.p1score == gameData.p2score)
        {
            printf("\nIt's a tie!");
        }
        printf("\nThank you for playing this game\n");
    }

    void getPlayerMove()
    {
        char *choice = calloc(5, sizeof *choice);
        int readCount;
        do
        {

            printf("\nPlease enter the coordinates of your move in xy (x is the row, y is the column) format or 'quit' to quit: ");
            fgets(choice, 5, stdin);
            if (strcmp(choice, "quit") == 0)
            {
                announceWinner();
                exit(0);
                //return ;
            }

            readCount = sscanf(choice, "%1d%1d", &move.coords.x, &move.coords.y);
            //printf("\nsuccess %d %d %d\n", readCount, move.coords.x, move.coords.y);
            if (readCount != 2)
            {
                puts("\nInvalid input for the coordinates. Please re-enter\n");
            }

        } while (readCount != 2);

        do
        {
            printf("\nPlease enter the letter you would like to place at the coordinates: ");

            move.letter = fgetc(stdin);
            fgetc(stdin);
            if (!isLetter(move.letter))
            {
                printf("\n%c is not a letter. Please re-enter", move.letter);
            }

        } while (!isLetter(move.letter));
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
            return 0;
        }

        return total;
    }

    char *findWordWest(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            last.y--;
            if (!isOnBoard(last.x, last.y))
            {
                word[i + 1] = '\0';
                break;
            }
        }
        return word;
    }

    char *findWordEast(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            last.y++;
            if (!isOnBoard(last.x, last.y))
            {
                word[i + 1] = '\0';
                break;
            }
        }
        return word;
    }

    char *findWordNorth(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            last.x--;
            if (!isOnBoard(last.x, last.y))
            {
                word[i + 1] = '\0';
                break;
            }
        }
        return word;
    }

    char *findWordSouth(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            last.x++;
            if (!isOnBoard(last.x, last.y))
            {
                word[i + 1] = '\0';
                break;
            }
        }
        return word;
    }

    char *findWordNorthEast(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            last.y++;
            last.x--;
            if (!isOnBoard(last.x, last.y))
            {
                word[i + 1] = '\0';
                break;
            }
        }
        return word;
    }

    char *findWordNorthWest(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            last.x--;
            last.y--;
            if (!isOnBoard(last.x, last.y))
            {
                word[i + 1] = '\0';
                break;
            }
        }
        return word;
    }

    char *findWordSouthEast(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            last.x++;
            last.y++;
            if (!isOnBoard(last.x, last.y))
            {
                word[i + 1] = '\0';
                break;
            }
        }
        return word;
    }

    char *findWordSouthWest(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            last.x++;
            last.y--;
            if (!isOnBoard(last.x, last.y))
            {
                word[i + 1] = '\0';
                break;
            }
        }
        return word;
    }

    char **findWordsFromMove()
    {
        char **words = calloc(8, sizeof *words);
        words[0] = findWordWest(move.coords);
        words[1] = findWordEast(move.coords);
        words[2] = findWordSouth(move.coords);
        words[3] = findWordNorth(move.coords);
        words[4] = findWordNorthEast(move.coords);
        words[5] = findWordNorthWest(move.coords);
        words[6] = findWordSouthEast(move.coords);
        words[7] = findWordSouthWest(move.coords);
        return words;
    }

    void updateScore()
    {
        char **words = findWordsFromMove();
        int turnTotal = 0;
        printf("\nYour words this round:\t");
        for (int i = 0; i < 8; i++)
        {
            if (isValidWord(dict, words[i]))
            {
                int wordScore = calculateScore(words[i]);
                turnTotal += wordScore;
                printf("%s(%d pts) ", words[i], wordScore);
            }
        }
        printf("\nYou scored %d points!", turnTotal);

        if (gameData.turn % 2 == 1)
        {
            gameData.p2score += turnTotal;
            printf(" You now have %d points.\n\n", gameData.p2score);
        }
        else
        {
            gameData.p1score += turnTotal;
            printf(" You now have %d points.\n\n", gameData.p1score);
        }
        // free the memory taken up by checking the words for this round
        for (int i = 0; i < 8; i++)
        {
            free(words[i]);
        }
        free(words);
    }

    int isBoardFull()
    {
        struct BoardCoord tempCoords;
        for (int i = 1; i <= board.size; i++)
        {
            for (int x = 1; x <= board.size; x++)
            {
                tempCoords.x = i;
                tempCoords.y = x;
                if (isPositionEmpty(tempCoords))
                {
                    return 0;
                }
            }
        }
        return 1;
    }

    // initialize game
    gameData.p1score = 0;
    gameData.p2score = 0;
    gameData.turn = 0;
    move.coords.x = 0;
    move.coords.y = 0;
    getNewBoard();
    drawBoard();

    while (1)
    {
        printf("\n[Player %d]", (gameData.turn % 2) + 1);
        getPlayerMove();
        switch (makePlay(move))
        {
        case 0:
            printf("\nThe requested coordinates (%d,%d) are not empty\nPress enter to try again...\n", move.coords.x, move.coords.y);
            getchar();
            break;
        case -1:
            printf("\nThe requested coordinates (%d,%d) do not exist on a %dx%d board.\nPress enter to try again...\n", move.coords.x, move.coords.y, board.size, board.size);
            getchar();
            break;
        default:
            updateScore();
            gameData.turn++;
            printf("\nPlayer %d is next... press enter to continue\n", (gameData.turn % 2) + 1);
            getchar();
            break;
        }
        if (isBoardFull())
        {
            printf("\nThe board is full! No more moves available.\n");
            announceWinner();
            exit(0);
        }
        drawBoard();
    }

    return 0;
}
