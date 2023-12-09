#ifndef TREElib
#define TREElib

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <ctype.h>
#include <float.h>
#include <math.h>

#include "readFile.h"


const size_t MAX_SIZE_TREE  = 64;
const size_t MAX_SIZE_ARG   = 64;
const size_t MAX_SIZE_NAME  = 50;
const size_t MAX_NUM_VARS   = 10;
const size_t MAX_NUM_TOKENS = 50;
const size_t SIZE_SYMBOLS      = 10;
const size_t SIZE_MORSE_SYMBOL = 10;

const size_t NUM_MATH_COMMANDS = 9;
const size_t NUM_COMMANDS_T =  2;
const size_t NUM_COMMANDS_U =  5;
const size_t NUM_COMPARE_COMMANDS = 2;
const size_t NUM_LANG_COMMANDS = 8;
const size_t NUM_OPERATORS_COMMANDS = 2;

typedef char* Elem_t;

enum Order
{
    PRE_ORDER  = 1,
    IN_ORDER   = 2,
    POST_ORDER = 3,
};

enum TreeError
{
    NO_ERROR,
    LOOP_ERROR,
    ALLOC_ERROR,
    ERROR_CONST,
    FILE_NOT_OPEN,
    ERROR_POSITIONING_FUNC,
    DEFINE_IS_NULL,
    ELEMENT_NOT_FOUND,
    READER_ERROR
};

enum UnaryorBinary
{
    UNARY  = 1,
    BINARY = 2
};

enum Type
{
    NUM = 1,
    OPERATOR,
    VAR
};

enum Operators
{
    NO_OP = 0,
    OP_ADD = 1,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    FUN_SIN,
    FUN_COS,
    FUN_POW,
    FUN_SQRT,
    FUN_LN,
    OP_UN_SUB,
    L_BRACKET,
    R_BRACKET,
    L_CURLY_BRACKET,
    R_CURLY_BRACKET, 
    SEMICOLON,
    MORSE_PARTITION,
    OP_CONDITION,
    OP_LOOP,
    OP_ASSIGN,
    OP_ABOVE,
    OP_BELOW,
    END
};

enum MorseAlhabet
{
    MORSE_A = 300,
    MORSE_B,
    MORSE_C,
    MORSE_D, 
    MORSE_E,
    MORSE_F,
    MORSE_G,
    MORSE_H,
    MORSE_W,
    MORSE_V,
    MORSE_I,
    MORSE_J,
    MORSE_K,
    MORSE_L,
    MORSE_M, 
    MORSE_N,
    MORSE_O,
    MORSE_P, 
    MORSE_R, 
    MORSE_S, 
    MORSE_T, 
    MORSE_U, 
    MORSE_Q, 
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
    MORSE_EXPOINT //!
};

union tag_data
{
    double         value;
    MorseAlhabet var[SIZE_SYMBOLS];
    Operators   value_op;
};

struct Node
{
    tag_data  data;
    Type      type;
    Node*     left;
    Node*    right;
};

struct Tree
{
    Node* root;
    size_t size;
    size_t num_vars;
    size_t changes_num;
};

struct Tokens
{
    Node**    tokens;
    size_t  position;
    size_t      size;
};

struct Command
{
    char name[MAX_SIZE_NAME];
    size_t   size_name;
    Type          type;
    Operators       id;
};

struct Var
{
    char name[MAX_SIZE_NAME];
    double             value;
    size_t         name_size;
};

const size_t SIZE_SYMBOL  = 1;
const size_t SIZE_ADD  = 1;
const size_t SIZE_SUB  = 1;
const size_t SIZE_MUL  = 1;
const size_t SIZE_DIV  = 1;
const size_t SIZE_SIN  = 3;
const size_t SIZE_COS  = 3;
const size_t SIZE_POW  = 1;
const size_t SIZE_SQRT = 4;
const size_t SIZE_LN   = 2;

const Command math_cmds[NUM_MATH_COMMANDS] = \
   {{"+",    SIZE_ADD,  OPERATOR, OP_ADD  },\
    {"-",    SIZE_SUB,  OPERATOR, OP_SUB  },\
    {"*",    SIZE_MUL,  OPERATOR, OP_MUL  },\
    {"/",    SIZE_DIV,  OPERATOR, OP_DIV  },\
    {"sin",  SIZE_SIN,  OPERATOR, FUN_SIN },\
    {"cos",  SIZE_COS,  OPERATOR, FUN_COS },\
    {"^",    SIZE_POW,  OPERATOR, FUN_POW },\
    {"sqrt", SIZE_SQRT, OPERATOR, FUN_SQRT},\
    {"ln",   SIZE_LN,   OPERATOR, FUN_LN  }};


const Command cmdsT[NUM_COMMANDS_T] = \
   {{"*",    SIZE_MUL,  OPERATOR, OP_MUL},\
    {"/",    SIZE_DIV,  OPERATOR, OP_DIV},};


const size_t SIZE_ABOVE = 8;
const size_t SIZE_BELOW = 7;

const Command cmds_compare[NUM_COMPARE_COMMANDS] = \
   {{"overflow", SIZE_ABOVE, OPERATOR, OP_ABOVE},\
    {"lacking",  SIZE_BELOW, OPERATOR, OP_BELOW}};

const size_t SIZE_ASSIGN = 16;
const size_t SIZE_LOOP = 15;
const size_t SIZE_CONDITION = 12;

const Command cmds_lang[NUM_LANG_COMMANDS] = \
   {{"(",    SIZE_SYMBOL,  OPERATOR, L_BRACKET},\
    {")",    SIZE_SYMBOL,  OPERATOR, R_BRACKET},\
    {"{",    SIZE_SYMBOL,  OPERATOR, L_CURLY_BRACKET},\
    {"}",    SIZE_SYMBOL,  OPERATOR, R_CURLY_BRACKET},\
    {"encoding matches", SIZE_ASSIGN,    OPERATOR, OP_ASSIGN},
    {"reseption while",   SIZE_LOOP,      OPERATOR, OP_LOOP},
    {"reseption if",    SIZE_CONDITION, OPERATOR, OP_CONDITION},
    {";", SIZE_SYMBOL, OPERATOR, SEMICOLON}};

const Command cmds_oper[NUM_OPERATORS_COMMANDS] = \
   {{"reseprion while", SIZE_LOOP,      OPERATOR, OP_LOOP},
    {"reseption if",    SIZE_CONDITION, OPERATOR, OP_CONDITION}};


const Command cmdsU[NUM_COMMANDS_U] = \
   {{"sin",  SIZE_SIN,  OPERATOR, FUN_SIN  },\
    {"cos",  SIZE_COS,  OPERATOR, FUN_COS  },\
    {"sqrt", SIZE_SQRT, OPERATOR, FUN_SQRT },\
    {"ln",   SIZE_LN,   OPERATOR, FUN_LN   },\
    {"-",    SIZE_SUB,  OPERATOR, OP_UN_SUB}};


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


TreeError ConstructorTree(Tree* tree);
void       DestructorTree(Tree* tree);


Node* CreateNode(Type type, void* value, Node* left, Node* right);

Node* CreateVariable(int* var, Node* left, Node* right);
Node* CreateNumber(double value, Node* left, Node* right);
Node* CreateOperator(Operators value, Node* left, Node* right);

void DeleteNode(Node* node);
void DeleteToken(Node* node);

TreeError ConstructorTokens(Tokens* tkns, Text* buf);
TreeError DestructorTokens(Tokens* tkns);

TreeError CreateTokens(Tokens* tkns, Text* buf);
TreeError DeleteTokens(Tokens* tkns, Text* buf);

void SkipSpaces(Text* buf);

TreeError          ParseNumOrVar(Tokens* tkns, Text* buf);
TreeError         ParseMorseCode(Tokens* tkns, Text* buf);

TreeError     ParseMathOperators(Tokens* tkns, Text* buf);
TreeError     ParseBoolOperators(Tokens* tkns, Text* buf);
TreeError ParseLanguageOperators(Tokens* tkns, Text* buf);

bool FindCommand(Text* buf, const Command* cmds, const size_t num_commands, Operators* id);
bool FindSymbol(char* source, const MorseAlpha* morse_symbol, const size_t num_symbols, MorseAlhabet* id);
TreeError ReadingMorseCode(MorseAlhabet* id_value, size_t* i_vars, Text* buf);


Node* GetGrammar(Tokens* tkns);
Node* GetOperators(Tokens* tkns);

Node* GetExpression(Tokens* tkns);
Node* GetBoolingExpression(Tokens* tkns);

Node* GetTerm(Tokens* tkns);
Node* GetUnary(Tokens* tkns);

Node* GetPrimaryExpression(Tokens* tkns);
Node* GetBoolPrimaryExpression(Tokens* tkns);
Node* GetVariable(Tokens* tkns);
Node* GetNumber(Tokens* tkns);

Node* GetIf(Tokens* tkns);
Node* GetLoop(Tokens* tkns);
Node* GetAssign(Tokens* tkns);

void syntax_assert(bool x, Text* buf);




TreeError  PrintNode(Node* node, FILE* To, Order order_value);
void PrintObject(Node* node, FILE* To);
void PrintOperator(Operators value_Operators, FILE* TO);
void DumpTokens(Tokens* tkns);


#endif