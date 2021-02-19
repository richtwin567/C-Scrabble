/**
 * @file letters.h
 * @author Anakai Richards
 * @brief 
 * @version 0.1
 * @date 2021-02-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <ctype.h>

int isLetter(char c);
int determineLetterValue(char c);

/**
 * @brief Stores all the letter values as specified by the instructions
 * 
 */
enum LetterValues
{
    A = 1,
    B = 3,
    C = 3,
    D = 2,
    E = 1,
    F = 4,
    G = 2,
    H = 4,
    I = 1,
    J = 8,
    K = 5,
    L = 1,
    M = 3,
    N = 1,
    O = 1,
    P = 3,
    Q = 10,
    R = 1,
    S = 1,
    T = 1,
    U = 1,
    V = 4,
    W = 4,
    X = 8,
    Y = 4,
    Z = 10

};

/**
 * @brief Checks if the character passed is a letter
 * 
 * @param c the character to check
 * @return int a non-zero integer if c is a letter or 0 otherwise
 */
int isLetter(char c)
{
    return isalpha(c);
}

/**
 * @brief Returns the value of the character given
 * 
 * @param c a letter
 * @return int the value of the letter
 */
int determineLetterValue(char c)
{
    int val = 0;
    if (!isLetter(c)){
        return val;
    }
    c = toupper(c);
    switch (c)
    {
    case 'A':
        val = A;
        break;
    case 'B':
        val = B;
        break;
    case 'C':
        val = C;
        break;
    case 'D':
        val = D;
        break;
    case 'E':
        val = E;
        break;
    case 'F':
        val = F;
        break;
    case 'G':
        val = G;
        break;
    case 'H':
        val = H;
        break;
    case 'I':
        val = I;
        break;
    case 'J':
        val = J;
        break;
    case 'K':
        val = K;
        break;
    case 'L':
        val = L;
        break;
    case 'M':
        val = M;
        break;
    case 'N':
        val = N;
        break;
    case 'O':
        val = O;
        break;
    case 'P':
        val = P;
        break;
    case 'Q':
        val = Q;
        break;
    case 'R':
        val = R;
        break;
    case 'S':
        val = S;
        break;
    case 'T':
        val = T;
        break;
    case 'U':
        val = U;
        break;
    case 'V':
        val = V;
        break;
    case 'W':
        val = W;
        break;
    case 'X':
        val = X;
        break;
    case 'Z':
        val = Z;
        break;
    default:
        break;
    }
    return val;
}
