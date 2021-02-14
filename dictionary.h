#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Dictionary loadDictionary();
int isValidWord(struct Dictionary dict, char *word);

struct Dictionary
{
    int size;
    char **words;
};

/* int main()
{
    struct Dictionary dict;
    dict = loadDictionary();
    int v = isValidWord(dict, "apple");
    printf("\n%d", v);
    return 0;
} */

struct Dictionary loadDictionary()
{
    struct Dictionary dict;      // The dictionary to be filled
    int const LONGEST_WORD = 46; // The length of the longest word
    FILE *fp;                    // The file pointer for words.txt
    int i = 0;                   // Keeps track of the size of the array

    fp = fopen("words.txt", "r");

    // initialize words array
    dict.words = calloc(1, sizeof *dict.words);
    dict.words[i] = calloc(LONGEST_WORD, sizeof *(dict.words[i]));
    fscanf(fp, "%s", dict.words[i]);

    // read all words provided in word.txt line by line
    while (!feof(fp))
    {
        i++;
        dict.words = realloc(dict.words, (i + 1) * sizeof *dict.words);
        dict.words[i] = calloc(LONGEST_WORD, sizeof *(dict.words[i]));
        fscanf(fp, "%s", dict.words[i]);
        //printf("\n%d %s", i, dict.words[i]);
    }

    // set size
    dict.size = i;

    // close file
    fclose(fp);

    return dict;
}

int isValidWord(struct Dictionary dict, char *word)
{
    // ensure that word is in uppercase
    char *searchWord = calloc(strlen(word), sizeof *searchWord);
    for (int x = 0; word[x] != '\0'; x++)
    {
        char c = toupper(word[x]);
        searchWord[x] = c;
    }

    // Check if word exists in dictionary by binary search
    int first = 0;
    int last = dict.size - 1;
    while (first <= last)
    {
        int mid = (first + last) / 2;
        int res = strcmp(dict.words[mid], searchWord);
        printf("%s vs %s\n", dict.words[mid], searchWord);
        if (res == 0)
        {
            free(searchWord);
            return 1;
        }
        else if (res < 0)
        {
            first = mid + 1;
        }
        else
        {
            last = mid - 1;
        }
    }
    free(searchWord);
    return 0;
}