#include "letters.h"
#include "board.h"
#include "dictionary.h"

struct Game
{
    int turn;
    int p1score;
    int p2score;
};

struct Move getPlayerMove(struct Game);
int calculateScore(char *word);
char *findWordLeft(struct Board board, struct BoardCoord coords, struct Dictionary dict);
char *findWordRight(struct Board board, struct BoardCoord coords, struct Dictionary dict);
char *findWordUp(struct Board board, struct BoardCoord coords, struct Dictionary dict);
char *findWordDown(struct Board board, struct BoardCoord coords, struct Dictionary dict);
char **findWordsFromMove(struct Board board, struct BoardCoord coords, struct Dictionary dict);
void updateScore(struct Board board, struct BoardCoord coords, struct Dictionary dict, struct Game *gameData);
int isBoardFull(struct Board board);
void announceWinner(struct Game gameData);


int main(int argc, char const *argv[])
{
    struct Board board;                        // the game board
    int const LOGICAL_GRID_SIZE = 8;           // the size of the game board
    struct Dictionary dict = loadDictionary(); // dictionary of valid words
    struct Game gameData;                      // keep track the game

    // initialize game
    gameData.p1score = 0;
    gameData.p2score = 0;
    gameData.turn = 0;
    board = getNewBoard(LOGICAL_GRID_SIZE);
    drawBoard(board);

    while (1)
    {
        printf("\n[Player %d]", (gameData.turn % 2) + 1);
        struct Move nextMove = getPlayerMove(gameData);
        switch (makePlay(board, nextMove))
        {
        case 0:
            printf("\nThe requested coordinates (%d,%d) are not empty\nPress enter to try again...\n", nextMove.coords.x, nextMove.coords.y);
            gameData.turn--;
            getchar();
            break;
        case -1:
            printf("\nThe requested coordinates (%d,%d) do not exist on a %dx%d board.\nPress enter to try again...\n", nextMove.coords.x, nextMove.coords.y, board.size, board.size);
            gameData.turn--;
            getchar();
            break;
        default:
            updateScore(board, nextMove.coords, dict, &gameData);
            gameData.turn++;
            printf("\nPlayer %d is next... press enter to continue\n", (gameData.turn % 2) + 1);
            getchar();
            break;
        }
        if (isBoardFull(board))
        {
            printf("\nThe board is full! No more moves available.\n");
            announceWinner(gameData);
            exit(0);
        }
        drawBoard(board);
        
    }

    return 0;
}
struct Move getPlayerMove(struct Game gameData)
{
    char *choice = calloc(5, sizeof *choice);
    struct Move move;
    int readCount;
    do
    {

        printf("\nPlease enter the coordinates of your move in xy (x is the row, y is the column) format or 'quit' to quit: ");
        fgets(choice, 5, stdin);
        if (strcmp(choice, "quit") == 0)
        {
            announceWinner(gameData);
            exit(0);
            //return ;
        }

        readCount = sscanf(choice, "%1d%1d", &move.coords.x, &move.coords.y);
        //printf("\nsuccess %d %d %d\n", success, move.coords.x, move.coords.y);
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
        return 0;
    }

    return total;
}

char *findWordLeft(struct Board board, struct BoardCoord coords, struct Dictionary dict)
{
    char *word = calloc(1, sizeof *word);
    int i = 0;
    for (i = 0; !isPositionEmpty(board, coords); i++)
    {
        word[i] = getPosition(board, coords);
        word = realloc(word, (i + 1) * sizeof *word);
        coords.y--;
        if (!isOnBoard(board, coords.x, coords.y))
        {
            word[i + 1] = '\0';
            break;
        }
    }
    reverseWord(word);
    return word;
}

char *findWordRight(struct Board board, struct BoardCoord coords, struct Dictionary dict)
{
    char *word = calloc(1, sizeof *word);
    int i = 0;
    for (i = 0; !isPositionEmpty(board, coords); i++)
    {
        word[i] = getPosition(board, coords);
        word = realloc(word, (i + 1) * sizeof *word);
        coords.y++;
        if (!isOnBoard(board, coords.x, coords.y))
        {
            word[i + 1] = '\0';
            break;
        }
    }
    return word;
}

char *findWordUp(struct Board board, struct BoardCoord coords, struct Dictionary dict)
{
    char *word = calloc(1, sizeof *word);
    int i = 0;
    for (i = 0; !isPositionEmpty(board, coords); i++)
    {
        word[i] = getPosition(board, coords);
        word = realloc(word, (i + 1) * sizeof *word);
        coords.x--;
        if (!isOnBoard(board, coords.x, coords.y))
        {
            word[i + 1] = '\0';
            break;
        }
    }
    reverseWord(word);
    return word;
}

char *findWordDown(struct Board board, struct BoardCoord coords, struct Dictionary dict)
{
    char *word = calloc(1, sizeof *word);
    int i = 0;
    for (i = 0; !isPositionEmpty(board, coords); i++)
    {
        word[i] = getPosition(board, coords);
        word = realloc(word, (i + 1) * sizeof *word);
        coords.x++;
        if (!isOnBoard(board, coords.x, coords.y))
        {
            word[i + 1] = '\0';
            break;
        }
    }
    return word;
}

char **findWordsFromMove(struct Board board, struct BoardCoord coords, struct Dictionary dict)
{
    char **words = calloc(4, sizeof *words);
    words[0] = findWordLeft(board, coords, dict);
    words[1] = findWordRight(board, coords, dict);
    words[2] = findWordDown(board, coords, dict);
    words[3] = findWordUp(board, coords, dict);
    return words;
}

void updateScore(struct Board board, struct BoardCoord coords, struct Dictionary dict, struct Game *gameData)
{
    char **words = findWordsFromMove(board, coords, dict);
    int turnTotal = 0;
    printf("\nYour words this round:\t");
    for (int i = 0; i < 4; i++)
    {
        //printf("\n%s\n", words[i]);
        if (isValidWord(dict, words[i]))
        {
            int wordScore = calculateScore(words[i]);
            turnTotal += wordScore;
            printf("%s(%d pts) ", words[i], wordScore);
        }
    }
    printf("\nYou scored %d points!", turnTotal);

    if (gameData->turn % 2 == 1)
    {
        gameData->p2score += turnTotal;
        printf(" You now have %d points.\n\n", gameData->p2score);
    }
    else
    {
        gameData->p1score += turnTotal;
        printf(" You now have %d points.\n\n", gameData->p1score);
    }
}

void announceWinner(struct Game gameData)
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

int isBoardFull(struct Board board)
{
    struct BoardCoord coords;
    for (int i = 1; i <= board.size; i++)
    {
        for (int x = 1; x <= board.size; x++)
        {
            coords.x = i;
            coords.y = x;
            if (isPositionEmpty(board, coords))
            {
                return 0;
            }
        }
    }
    return 1;
}