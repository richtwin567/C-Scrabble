/**
 * @file dictionary.h
 * @author Anakai Richards
 * @brief 
 * @version 0.1
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Dictionary loadDictionary();
int isValidWord(struct Dictionary dict, char *word);

/**
 * @brief A representaion of a dictionary 
 * 
 */
struct Dictionary
{
    int size;
    char **words;
};

/**
 * @brief Loads the words from words.txt into a dictionary
 * 
 * @return struct Dictionary the dictionary
 */
struct Dictionary loadDictionary()
{
    struct Dictionary dict;      // The dictionary to be filled
    int const LONGEST_WORD = 46; // The length of the longest word
    FILE *fp;                    // The file pointer for words.txt
    int i = 0;                   // Keeps track of the size of the array

    fp = fopen("words.txt", "r");
    if (fp != NULL)
    {
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
        }

        // set size
        dict.size = i;

        // close file
        fclose(fp);

        return dict;
    }
    else
    {
        fprintf(stderr, "\nCan't find the dictionary.");
        exit(1);
    }
}

/**
 * @brief Searches the dictionary to find the given word. The word is valid once it is found.
 * 
 * @param dict the dictionary to search in 
 * @param word the word to search for
 * @return int 1 if the word is valid or 0 if the word is invalid
 */
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
}// end function isValidWord
