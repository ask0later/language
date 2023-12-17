#ifndef OPERlib
#define OPERlib

#include "lang.h"

const size_t SIZE_SYMBOLS      = 10;
const size_t SIZE_MORSE_SYMBOL = 10;

const size_t NUM_OPERATORS =  22;
const size_t NUM_COMMANDS_T =  2;
const size_t NUM_COMMANDS_U =  6;

const size_t NUM_LANG_COMMANDS = 11;
const size_t NUM_MATH_COMMANDS =  9;

const size_t NUM_COMPARE_COMMANDS   = 4;
const size_t NUM_COMMANDS_FUNCTION  = 2;
const size_t NUM_OPERATORS_COMMANDS = 2;


// struct Command
// {
//     char name[MAX_SIZE_ARG];
//     size_t   size_name;
//     Type          type;
//     Operators       id;
// };

const size_t SIZE_LN = 2;
const size_t SIZE_SIN = 3;
const size_t SIZE_COS = 3;
const size_t SIZE_SQRT = 4;
const size_t SIZE_RETURN = 6;
const size_t SIZE_INPUT  = 5;
const size_t SIZE_OUTPUT = 6;

const size_t SIZE_WHILE = 5;
const size_t SIZE_IF = 2;
const size_t SIZE_SYMBOL = 1;


const Command operators[NUM_OPERATORS] = \
   {{"+",    SIZE_SYMBOL,  OPERATOR, OP_ADD  },\
    {"-",    SIZE_SYMBOL,  OPERATOR, OP_SUB  },\
    {"*",    SIZE_SYMBOL,  OPERATOR, OP_MUL  },\
    {"/",    SIZE_SYMBOL,  OPERATOR, OP_DIV  },\
    {"sin",  SIZE_SIN,  OPERATOR, FUN_SIN },\
    {"cos",  SIZE_COS,  OPERATOR, FUN_COS },\
    {"^",    SIZE_SYMBOL,  OPERATOR, FUN_POW },\
    {"sqrt", SIZE_SQRT, OPERATOR, FUN_SQRT},\
    {"ln",   SIZE_LN,   OPERATOR, FUN_LN  },\
    {"==",  2 * SIZE_SYMBOL, OPERATOR, OP_EQUAL},\
    {"!=",  2 * SIZE_SYMBOL, OPERATOR, OP_NO_EQUAL},\
    {"=", SIZE_SYMBOL,    OPERATOR, OP_ASSIGN},\
    {">", SIZE_SYMBOL, OPERATOR, OP_ABOVE},\
    {"<",  SIZE_SYMBOL, OPERATOR, OP_BELOW},\
    {",", SIZE_SYMBOL, OPERATOR, COMMA},\
    {";", SIZE_SYMBOL, OPERATOR, SEMICOLON},\
    {"while", SIZE_WHILE,      OPERATOR, OP_LOOP},\
    {"if",    SIZE_IF, OPERATOR, OP_CONDITION},\
    {"-",    SIZE_SYMBOL,  OPERATOR, OP_UN_SUB},\
    {"return",  SIZE_RETURN,  OPERATOR, RET},
    {"input",  SIZE_INPUT,  OPERATOR, INPUT},
    {"output",  SIZE_OUTPUT,  OPERATOR, OUTPUT}};

const Command math_cmds[NUM_MATH_COMMANDS] = \
   {{"+",    SIZE_SYMBOL,  OPERATOR, OP_ADD  },\
    {"-",    SIZE_SYMBOL,  OPERATOR, OP_SUB  },\
    {"*",    SIZE_SYMBOL,  OPERATOR, OP_MUL  },\
    {"/",    SIZE_SYMBOL,  OPERATOR, OP_DIV  },\
    {"sin",  SIZE_SIN,  OPERATOR, FUN_SIN },\
    {"cos",  SIZE_COS,  OPERATOR, FUN_COS },\
    {"^",    SIZE_SYMBOL,  OPERATOR, FUN_POW },\
    {"sqrt", SIZE_SQRT, OPERATOR, FUN_SQRT},\
    {"ln",   SIZE_LN,   OPERATOR, FUN_LN  }};

const size_t SIZE_CALL = 19;
const size_t SIZE_OUT  =  3;

const Command function[NUM_COMMANDS_FUNCTION] = \
   {{"handing over to you", SIZE_CALL, OPERATOR, DEFINE},
    {"out",                 SIZE_OUT,  OPERATOR, RET   }};

const Command cmdsT[NUM_COMMANDS_T] = \
   {{"*",    SIZE_SYMBOL,  OPERATOR, OP_MUL},\
    {"/",    SIZE_SYMBOL,  OPERATOR, OP_DIV},};


const size_t SIZE_ABOVE = 8;
const size_t SIZE_BELOW = 7;
const size_t SIZE_EQUAL = 5;
const size_t SIZE_NOEQUAL = 7;

const Command cmds_compare[NUM_COMPARE_COMMANDS] = \
   {{"overflow", SIZE_ABOVE, OPERATOR, OP_ABOVE},\
    {"lacking",  SIZE_BELOW, OPERATOR, OP_BELOW},
    {"match",    SIZE_EQUAL, OPERATOR, OP_EQUAL},
    {"diverse",  SIZE_NOEQUAL, OPERATOR, OP_NO_EQUAL}};

const size_t SIZE_LANG_INPUT     = 13;
const size_t SIZE_LANG_OUTPUT    = 15;

const size_t SIZE_LOOP      = 15;
const size_t SIZE_ASSIGN    = 16;
const size_t SIZE_CONDITION = 12;


const Command cmds_lang[NUM_LANG_COMMANDS] = \
   {{"(",    SIZE_SYMBOL,  OPERATOR, L_BRACKET},\
    {")",    SIZE_SYMBOL,  OPERATOR, R_BRACKET},\
    {"{",    SIZE_SYMBOL,  OPERATOR, L_CURLY_BRACKET},\
    {"}",    SIZE_SYMBOL,  OPERATOR, R_CURLY_BRACKET},\
    {"encoding matches", SIZE_ASSIGN,    OPERATOR, OP_ASSIGN},
    {"reseption while",   SIZE_LOOP,      OPERATOR, OP_LOOP},
    {"reseption if",    SIZE_CONDITION, OPERATOR, OP_CONDITION},
    {",", SIZE_SYMBOL, OPERATOR, COMMA},
    {";", SIZE_SYMBOL, OPERATOR, SEMICOLON},
    {"give a signal", SIZE_LANG_INPUT, OPERATOR, INPUT},
    {"get the message", SIZE_LANG_OUTPUT, OPERATOR, OUTPUT}};

const Command cmds_oper[NUM_OPERATORS_COMMANDS] = \
   {{"reseprion while", SIZE_LOOP,      OPERATOR, OP_LOOP},
    {"reseption if",    SIZE_CONDITION, OPERATOR, OP_CONDITION}};

const Command cmdsU[NUM_COMMANDS_U] = \
   {{"sin",  SIZE_SIN,  OPERATOR, FUN_SIN  },\
    {"cos",  SIZE_COS,  OPERATOR, FUN_COS  },\
    {"sqrt", SIZE_SQRT, OPERATOR, FUN_SQRT },\
    {"ln",   SIZE_LN,   OPERATOR, FUN_LN   },\
    {"-",    SIZE_SYMBOL,  OPERATOR, OP_UN_SUB},
    {"out",  SIZE_OUT,  OPERATOR, RET      }};

#endif