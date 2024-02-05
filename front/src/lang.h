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

struct Tokens
{
    Node**    tokens;
    size_t  position;
    size_t      size;
};

struct Name
{
    Type                type;
    size_t                id;
    size_t         name_size;
    char name[MAX_SIZE_NAME];
};

struct Function
{
    Tree         tree;
    Name*        vars;
    size_t  num_names;
    size_t     offset;
};

struct Iterator
{   
    Function funcs[MAX_NUM_FUNCTIONS];
    Name*   table_funcs;
    size_t   size;
};

struct Command
{
    char name[MAX_SIZE_ARG];
    size_t   size_name;
    Type          type;
    Operators       id;
};

//------------------------------------------------------------------------

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
   {{MORSE_1, ".----"}, {MORSE_2, "..---"}, {MORSE_3, "...--"}, {MORSE_4, "....-"}, {MORSE_5, "....."},\
    {MORSE_6, "-...."}, {MORSE_7, "--..."}, {MORSE_8, "---.."}, {MORSE_9, "----."}, {MORSE_0, "-----"}};
    
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



LangError ConstructorTokens(Tokens* tkns, Text* buf);
LangError DestructorTokens(Tokens* tkns);

LangError CreateTokens(Tokens* tkns, Text* buf);
LangError DeleteTokens(Tokens* tkns, Text* buf);

void SkipSpaces(Text* buf);

LangError          ParseFunction(Tokens* tkns, Text* buf);
LangError          ParseNumOrVar(Tokens* tkns, Text* buf);
LangError         ParseMorseCode(Tokens* tkns, Text* buf);

LangError     ParseMathOperators(Tokens* tkns, Text* buf);
LangError     ParseBoolOperators(Tokens* tkns, Text* buf);
LangError ParseLanguageOperators(Tokens* tkns, Text* buf);

bool FindCommand(Text* buf, const Command* cmds, const size_t num_commands, Operators* id, size_t* offset);
bool FindSymbol(char* source, const MorseAlpha* morse_symbol, const size_t num_symbols, MorseAlhabet* id);
LangError ReadMorseCode(MorseAlhabet* id_value, size_t* i_vars, Text* buf);

LangError MakeTokenWithName(Tokens* tkns, Text* buf, Operators id_op, Node* (*CreateSomething)(size_t, char*, Node*, Node*));


LangError MatchNamesTable(Iterator* func_it, char* name_var, size_t* id_var);
LangError MatchFuncNamesTable(Iterator* func_it, char* name_var, size_t* id_fun);

Node* GetGrammar(Tokens* tkns, Iterator* func_it, LangError* error);
Node* GetOperators(Tokens* tkns, Iterator* func_it, LangError* error);

Node* GetExpression(Tokens* tkns, Iterator* func_it, LangError* error);
Node* GetBoolingExpression(Tokens* tkns, Iterator* func_it, LangError* error);

Node* GetTerm(Tokens* tkns, Iterator* func_it, LangError* error);
Node* GetUnary(Tokens* tkns, Iterator* func_it, LangError* error);

Node* GetPrimaryExpression(Tokens* tkns, Iterator* func_it, LangError* error);
Node* GetBoolPrimaryExpression(Tokens* tkns, Iterator* func_it, LangError* error);
Node* GetVariable(Tokens* tkns, Iterator* func_it, LangError* error);
Node* GetNumber(Tokens* tkns, LangError* error);
Node* GetArgument(Tokens* tkns, Iterator* func_it, LangError* error);

Node*  GetFunction(Tokens* tkns, Iterator* func_it, LangError* error);
Node* GetWhileOrIf(Tokens* tkns, Iterator* func_it, LangError* error);
Node*    GetAssign(Tokens* tkns, Iterator* func_it, LangError* error);

Node*  GetInOutput(Tokens* tkns, Iterator* func_it, LangError* error);
Node*    GetReturn(Tokens* tkns, Iterator* func_it, LangError* error);


LangError TranslateMorseCode(char* name_var, MorseAlhabet* var, size_t* size_name);

LangError DestructorIterator(Iterator* func_it);
LangError CreateNames(Name** name);
LangError DeleteNames(Name** name);
void DeleteToken(Node* node);



#endif