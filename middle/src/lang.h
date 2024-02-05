#ifndef LANGlib
#define LANGlib

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <ctype.h>
#include <float.h>
#include <math.h>

#include "tree.h"

const size_t MAX_NUM_VARS   = 10;
const size_t MAX_NUM_TOKENS = 50;
const size_t MAX_NUM_TABLES = 10;
const size_t MAX_NUM_NAMES  = 10;
const size_t MAX_NUM_FUNCTIONS = 10;

enum MorseAlhabet
{
    MORSE_A = 65,
    MORSE_B,
    MORSE_C,
    MORSE_D, 
    MORSE_E,
    MORSE_F,
    MORSE_G,
    MORSE_H,
    MORSE_I,
    MORSE_J,
    MORSE_K,
    MORSE_L,
    MORSE_M,
    MORSE_N,
    MORSE_O,
    MORSE_P,
    MORSE_Q,
    MORSE_R, 
    MORSE_S, 
    MORSE_T, 
    MORSE_U,
    MORSE_V,
    MORSE_W,     
    MORSE_X,
    MORSE_Y,
    MORSE_Z,
    MORSE_0 = 0, 
    MORSE_1, 
    MORSE_2, 
    MORSE_3, 
    MORSE_4, 
    MORSE_5, 
    MORSE_6, 
    MORSE_7, 
    MORSE_8, 
    MORSE_9,  
    MORSE_POINT, 
    MORSE_COMMA, 
    MORSE_COLON,
    MORSE_SEMICOLON, 
    MORSE_BRACKET, 
    MORSE_APOSTROPHE,
    MORSE_QUOTES,
    MORSE_DASH, //-
    MORSE_SLASH,
    MORSE_QUESTION_MARK,
    MORSE_EXPOINT
};

enum LangError
{
    NO_ERROR_LANG,
    SYNTAX_ERROR,
    ALLOC_ERROR_LANG
};

struct MorseAlpha
{
    MorseAlhabet value;
    char  encoding[10];
};

const size_t NUM_SYMBOLS = 47;
const size_t NUM_ALPHA = 26;
const size_t NUM_DIGIT = 10;
const size_t NUM_SPECIAL_SYMBOLS = 11;
const size_t NUM_ALNUM = 36;

const MorseAlpha morse_alpha[NUM_ALPHA] = \
   {{MORSE_A, ".-"  }, {MORSE_B, "-..."}, {MORSE_W, ".--" }, {MORSE_G, "--." }, {MORSE_D, "-.." }, {MORSE_E, "."   },\
    {MORSE_V, "...-"}, {MORSE_Z, "--.."}, {MORSE_I, ".."  }, {MORSE_J, ".---"}, {MORSE_K, "-.-" }, {MORSE_L, ".-.."},\
    {MORSE_M, "--"  }, {MORSE_N, "-."  }, {MORSE_O, "---" }, {MORSE_P, ".--."}, {MORSE_R, ".-." }, {MORSE_S, "..." },\
    {MORSE_T, "-"   }, {MORSE_U, "..-" }, {MORSE_F, "..-."}, {MORSE_H, "...."}, {MORSE_C, "-.-."}, {MORSE_Q, "--.-"},\
    {MORSE_Y, "-..-"}, {MORSE_X, "-.--"}};
    
const MorseAlpha morse_digit[NUM_DIGIT] = \
   {{MORSE_0, "-----"}, {MORSE_1, ".----"}, {MORSE_2, "..---"}, {MORSE_3, "...--"}, {MORSE_4, "....-"}, {MORSE_5, "....."},\
    {MORSE_6, "-...."}, {MORSE_7, "--..."}, {MORSE_8, "---.."}, {MORSE_9, "----."}};
    
const MorseAlpha morse_alnum[NUM_ALNUM] = \
   {{MORSE_A, ".-"  }, {MORSE_B, "-..."}, {MORSE_W, ".--" }, {MORSE_G, "--." }, {MORSE_D, "-.." }, {MORSE_E, "."   },\
    {MORSE_V, "...-"}, {MORSE_Z, "--.."}, {MORSE_I, ".."  }, {MORSE_J, ".---"}, {MORSE_K, "-.-" }, {MORSE_L, ".-.."},\
    {MORSE_M, "--"  }, {MORSE_N, "-."  }, {MORSE_O, "---" }, {MORSE_P, ".--."}, {MORSE_R, ".-." }, {MORSE_S, "..." },\
    {MORSE_T, "-"   }, {MORSE_U, "..-" }, {MORSE_F, "..-."}, {MORSE_H, "...."}, {MORSE_C, "-.-."}, {MORSE_Q, "--.-"},\
    {MORSE_Y, "-..-"}, {MORSE_X, "-.--"},\
    {MORSE_1, ".----"}, {MORSE_2, "..---"}, {MORSE_3, "...--"}, {MORSE_4, "....-"}, {MORSE_5, "....."},\
    {MORSE_6, "-...."}, {MORSE_7, "--..."}, {MORSE_8, "---.."}, {MORSE_9, "----."}, {MORSE_0, "-----"}};
    

const MorseAlpha morse_symbols[NUM_SPECIAL_SYMBOLS] = \
   {{MORSE_POINT,         "......"},\
    {MORSE_COMMA,         "-.-.-."},\
    {MORSE_COLON,         "---..."},\
    {MORSE_SEMICOLON,     "-.-.-" },\
    {MORSE_BRACKET,       "-.--.-"},\
    {MORSE_APOSTROPHE,    ".----."},\
    {MORSE_QUOTES,        ".-..-."},\
    {MORSE_DASH,          "-....-"},\
    {MORSE_SLASH,         "-..-." },\
    {MORSE_QUESTION_MARK, "..--.."},\
    {MORSE_EXPOINT,       "--..--"}};


#endif