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

    /**
     * @brief Checks if the specified coordinates exist on the board
     * 
     * @param row the row to be checked
     * @param col the column to be checked
     * @return int a non-zero value to indicate true or 0 to indicate false
     */
    int isOnBoard(int row, int col)
    {
        return (row <= board.size && row >= 1) && (col <= board.size && col >= 1);
    } // end function isOnBoard

    /**
     * @brief Converts the column as the user enters it to an index to be used to access that specify location in the string
     * 
     * @param userColumnInput the column number (1-8)
     * @return int the index of the column
     */
    int getColumnIndex(int userColumnInput)
    {
        // Calculate the accurate column index by expressing the column
        // as the nth value in an arithmetic series (nth = a+(n-1)d)
        return 4 + (4 * (userColumnInput - 1));
    } // end function getColumnIndex

    /**
     * @brief Converts the row as the user enters it to an index to be used to access that specify location in the string
     * 
     * @param userRowInput the row number (1-8)
     * @return int the index of the row
     */
    int getRowIndex(int userRowInput)
    {
        // calculate the accurate row index by expressing the row
        // as the nth value in an arithmetic series  (nth = a+(n-1)d)
        return 2 + (2 * (userRowInput - 1));
    } // end function getRowIndex

    /**
     * @brief Get the board row based on the index given
     * 
     * @param index the index in the string
     * @return int the board row (1-8)
     */
    int getVirtualRow(int index)
    {
        // The reverse of getRowIndex
        // Uses the array index to get the row number as seen by the user
        return ((index - 2) / 2) + 1;
    } //end function getVirtualRow

    /**
     * @brief Get the board column based on the index given
     * 
     * @param index the index in the string
     * @return int the board column (1-8)
     */
    int getVirtualCol(int index)
    {
        // The reverse of getVirtualCol.
        // Uses the array index to the column number as seen by the user
        return ((index - 4) / 4) + 1;
    }

    /**
     * @brief Build the empty grid rows and column lines with the appropriate number labels.
     * 
     */
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
    } // end function initGrid

    /**
     * @brief calculate the number of characters for each row and the number of rows needed based on the grid size.
     * 
     */
    void calcRowsAndCols()
    {
        board.rowCount = (LOGICAL_SIZE + 1) * 2;
        board.lineLength = 4 * (LOGICAL_SIZE + 1);
    } //end function calRowsAndCols

    /**
     * @brief Get the character at the specified coordinates.
     * 
     * @param coords the coordinates 
     * @return char the character at the coordinates
     */
    char getPosition(struct BoardCoord coords)
    {
        return board.grid[getRowIndex(coords.x)][getColumnIndex(coords.y)];
    } // end function getPosition

    /**
     * @brief Checks if the position specified is empty
     * 
     * @param coords the position
     * @return int 1 if the positon is empty or 0 if it is not
     */
    int isPositionEmpty(struct BoardCoord coords)
    {

        return isblank(getPosition(coords));
    } // end function isPositionEmpty

    /**
     * @brief Place a character specified by the move at the positon specified by the move
     * 
     * @param move the character to place, and the position to place it at.
     */
    void setPosition(struct Move move)
    {
        board.grid[getRowIndex(move.coords.x)][getColumnIndex(move.coords.y)] = move.letter;
    } // end function setPosition

    /**
     * @brief Place the character at the specified (x,y) location
     * 
     * @param move the row and the column and the character to place
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
    } // end function makePlay

    /**
     * @brief Places 10 characters at random locatinos on the board
     * 
     */
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
    } // end function startBoard

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

    /**
     * @brief Prints the scores and the winner at the end of the game
     * 
     */
    void announceWinner()
    {
        printf("\n============================\n");
        printf("\nThe Scores are: ");
        printf("\n%s:\t%d",gameData.p1name, gameData.p1score);
        printf("\n%s:\t%d\n", gameData.p2name,gameData.p2score);
        if (gameData.p1score > gameData.p2score)
        {
            printf("\nThe winner is %s!", gameData.p1name);
        }
        else if (gameData.p1score < gameData.p2score)
        {
            printf("\nThe winner is %s!", gameData.p2name);
        }
        else if (gameData.p1score == gameData.p2score)
        {
            printf("\nIt's a tie!");
        }
        printf("\nThank you for playing this game\n");
    } // end function announce winner

    /**
     * @brief Checks if the player is sure they would like to quit the game
     * 
     */
    int confirmQuit()
    {
        char confirm;
        int check = 1;
        int read;
        do
        {
            printf("\nAre you sure you want to quit? (y/N): ");
            if (scanf("%c", &confirm) == 0)
            {
                check = 1;
                printf("\n2Invalid answer. You must enter y for yes or n for no. Please re-enter: \n");
            }
            else
            {
                check = 0;
            }
            fgetc(stdin);
            if (check == 0)
            {
                confirm = tolower(confirm);
                if (confirm != 'y' && confirm != 'n')
                {
                    check = 1;
                    printf("\nInvalid answer. You must enter y for yes or n for no. Please re-enter: \n");
                }
                else
                {
                    check = 0;
                }
            }

        } while (check == 1);
        if (confirm == 'y')
        {
            announceWinner();
            exit(0);
        }
        else
        {
            printf("\nQuit cancelled\n");
            return 1;
        }
    } // end function confirmQuit

    /**
     * @brief Get the Player Move. Tmove can be xy coordinates on the board or quit. 
     * 
     */
    void getPlayerMove()
    {
        char *choice = calloc(5, sizeof *choice);
        int readCount;
        do
        {
            int shouldReprint = 0;

            printf("\nPlease enter the coordinates of your move in xy (x is the row, y is the column) format or 'quit' to quit: ");
            fgets(choice, 5, stdin);
            if (strcmp(choice, "quit") == 0)
            {
                fgetc(stdin);
                shouldReprint = confirmQuit();
            }
            // parse the string entered by the player above to extract the x and y coordinates
            readCount = sscanf(choice, "%1d%1d", &move.coords.x, &move.coords.y);
            if (readCount != 2)
            {
                if (!shouldReprint)
                {
                    puts("\nInvalid input for the coordinates. Please re-enter\n");
                }
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
    } // end function getPlayerMove

    /**
     * @brief Calculates the sore of a valid word by summing each letter's value. The word must be 4 letters long at least to be worth any points.
     * 
     * @param word the word to calculate the score for
     * @return int the sore for that word
     */
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
    } // end function calculateScore

    /**
     * @brief Search the board for a word/string going to the left from the last move position.
     * 
     * @param last the coordinates of the last move
     * @return char* the string found
     */
    char *findWordWest(struct BoardCoord last)
    {
        char *word = calloc(1, sizeof *word);
        int i = 0;
        // this function only builds string of alphanumeric characters so once the next positon is empty, stop
        for (i = 0; !isPositionEmpty(last); i++)
        {
            word[i] = getPosition(last);
            word = realloc(word, (i + 1) * sizeof *word);
            //got to the next position
            last.y--;
            // if the next position does not exist on the board, the end of the string has been reached.
            if (!isOnBoard(last.x, last.y))
            {
                i++;
                break;
            }
        }
        word[i] = '\0';
        return word;
    } // end function findWordWest

    /**
     * @brief Search the board for a word/string going to the right from the last move position.
     * 
     * @param last the coordinates of the last move
     * @return char* the string found
     */
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
                i++;
                break;
            }
        }
        word[i] = '\0';
        return word;
    } // end function findWordEast

    /**
     * @brief Search the board for a word/string going up from the last move position.
     * 
     * @param last the coordinates of the last move
     * @return char* the string found
     */
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
                i++;
                break;
            }
        }
        word[i] = '\0';
        return word;
    } // end function findWordNorth

    /**
     * @brief Search the board for a word/string going down from the last move position.
     * 
     * @param last the coordinates of the last move
     * @return char* the string found
     */
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
                i++;
                break;
            }
        }
        word[i] = '\0';
        return word;
    } // end function findWordSouth

    /**
     * @brief Search the board for a word/string on the upper right diagonal from the last move position.
     * 
     * @param last the coordinates of the last move
     * @return char* the string found
     */
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
                i++;
                break;
            }
        }
        word[i] = '\0';
        return word;
    } // end function findWordNorthEast

    /**
     * @brief Search the board for a word/string on the upper left diagonal from the last move position.
     * 
     * @param last the coordinates of the last move
     * @return char* the string found
     */
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
                i++;
                break;
            }
        }
        word[i] = '\0';
        return word;
    } // end functon findWordNorthWest

    /**
     * @brief Search the board for a word/string on the lower right diagonal from the last move position.
     * 
     * @param last the coordinates of the last move
     * @return char* the string found
     */
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
                i++;
                break;
            }
        }
        word[i] = '\0';
        return word;
    } // end function findWordSouthEast

    /**
     * @brief Search the board for a word/string on the lower left diagonal from the last move position.
     * 
     * @param last the coordinates of the last move
     * @return char* the string found
     */
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
                i++;
                break;
            }
        }
        word[i] = '\0';
        return word;
    } // end function findWordSouthWest

    /**
     * @brief Finds all the words created for the round in all directions starting from the positon where the last move was made.
     * 
     * @return char** The words found
     */
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

    /**
     * @brief Updates the player score for the round after they make a move
     * 
     */
    void updateScore()
    {
        // find all potential words for this round
        char **words = findWordsFromMove();
        int turnTotal = 0;
        printf("\nYour words this round:\t");
        for (int i = 0; i < 8; i++)
        {
            // if the word is valid calculate and save the score
            if (isValidWord(dict, words[i]))
            {
                int wordScore = calculateScore(words[i]);
                turnTotal += wordScore;
                printf("%s(%d pts) ", words[i], wordScore);
            }
        }
        // print the current point standing
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

    /**
     * @brief Checks is there are any free spaces left on the board
     * 
     * @return int 1, if the board is full or 0 if it is not full.
     */
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

    /**
     * @brief Print the instructions for the players
     * 
     */
    void printInstructions()
    {
        printf("\nWelcome!\nThis Scrabble game is a two player game played on a 8x8 board. The aim of this game is to create four letter words or longer\n");
        printf("\n=====\tRULES\t=====");
        printf("\n1.\tPlayers alternate turns placing a single character at a specific position.");
        printf("\n2.\tOn their turn a player can either quit the game or make a move");
        printf("\n3.\tThe game will automatically end when the board is full");
        printf("\n4.\tAt the end of the game the socres and the winner will be displayed.");
        printf("\n\n=====\tSCORING\t=====");
        printf("\n1.\tWords made for a round will be checked in all directions starting from the last letter played");
        printf("\n2.\tWords will be checked against this game's dictionary for validity");
        printf("\n3.\tA player can only score points for valid words that are 4 letters or more.");
        printf("\nPress enter to continue...\n");
        getchar();
    }

    /**
     * @brief Get the Player Names
     * 
     */
    void getPlayerNames()
    {
        printf("\nPlayer 1, please enter your name: ");
        scanf("%s", gameData.p1name);
        fgetc(stdin);
        printf("\nPlayer 2, please enter your name: ");
        scanf("%s", gameData.p2name);
        fgetc(stdin);
        printf("\nPress enter to start...\n");
        getchar();
    }

    printInstructions();
    getPlayerNames();
    // initialize game
    gameData.p1score = 0;
    gameData.p2score = 0;
    gameData.turn = 0;
    move.coords.x = 0;
    move.coords.y = 0;
    getNewBoard();
    drawBoard();

    // primary loop to control the game
    while (1)
    {
        int player = (gameData.turn % 2) + 1;
        if (player == 1)
        {
            printf("\n[It's %s's turn]", gameData.p1name);
        }
        else
        {
            printf("\n[It's %s's turn]", gameData.p2name);
        }
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
            printf("\n%s is next... press enter to continue\n", player == 1 ? gameData.p2name : gameData.p1name);
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
