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
const size_t MAX_SIZE_NAME  = 10;
const size_t NUM_COMMANDS   = 11;
const size_t NUM_COMMANDS_T =  7;
const size_t MAX_NUM_VARS   = 10;
const size_t MAX_NUM_TOKENS = 10;

typedef char* Elem_t;

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
    OP_ADD = 1,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    FUN_SIN,
    FUN_COS,
    FUN_POW,
    FUN_SQRT,
    FUN_LN,
    OP_LEFT_P,
    OP_RIGHT_P
};

union tag_data
{
    double         value;
    char*       variable;
    Operators   value_op;
};

struct Token
{
    Type type;
    tag_data data;
};


struct Command
{
    char name[MAX_SIZE_NAME];
    size_t   size_name;
    Type          type;
    int          value;
    UnaryorBinary num_args;
};

struct Var
{
    char name[MAX_SIZE_NAME];
    double             value;
    size_t         name_size;
};


const size_t SIZE_ADD  = 1;
const size_t SIZE_SUB  = 1;
const size_t SIZE_MUL  = 1;
const size_t SIZE_DIV  = 1;
const size_t SIZE_SIN  = 3;
const size_t SIZE_COS  = 3;
const size_t SIZE_POW  = 1;
const size_t SIZE_SQRT = 4;
const size_t SIZE_LN   = 2;

const Command cmds[NUM_COMMANDS] = {{"(",    1,  OPERATOR, OP_LEFT_P  , BINARY},\
                                    {")",    1,  OPERATOR, OP_LEFT_P  , BINARY},
                                    {"+",    SIZE_ADD,  OPERATOR, OP_ADD  , BINARY},\
                                    {"-",    SIZE_SUB,  OPERATOR, OP_SUB  , BINARY},\
                                    {"*",    SIZE_MUL,  OPERATOR, OP_MUL  , BINARY},\
                                    {"/",    SIZE_DIV,  OPERATOR, OP_DIV  , BINARY},\
                                    {"sin",  SIZE_SIN,  OPERATOR, FUN_SIN , UNARY },\
                                    {"cos",  SIZE_COS,  OPERATOR, FUN_COS , UNARY },\
                                    {"^",    SIZE_POW,  OPERATOR, FUN_POW , BINARY},\
                                    {"sqrt", SIZE_SQRT, OPERATOR, FUN_SQRT, UNARY },\
                                    {"ln",   SIZE_LN,   OPERATOR, FUN_LN  , UNARY } };

const Command cmdsT[NUM_COMMANDS] = {{"*",    SIZE_MUL,  OPERATOR, OP_MUL  , BINARY},\
                                     {"/",    SIZE_DIV,  OPERATOR, OP_DIV  , BINARY},\
                                     {"sin",  SIZE_SIN,  OPERATOR, FUN_SIN , UNARY },\
                                     {"cos",  SIZE_COS,  OPERATOR, FUN_COS , UNARY },\
                                     {"^",    SIZE_POW,  OPERATOR, FUN_POW , BINARY},\
                                     {"sqrt", SIZE_SQRT, OPERATOR, FUN_SQRT, UNARY },\
                                     {"ln",   SIZE_LN,   OPERATOR, FUN_LN  , UNARY } };

enum Order
{
    PRE_ORDER  = 1,
    IN_ORDER   = 2,
    POST_ORDER = 3,
};


enum TreeError
{
    NO_ERROR,
    ERROR_LOOP,
    ERROR_ALLOCATION,
    ERROR_CONST,
    FILE_NOT_OPEN,
    ERROR_POSITIONING_FUNC,
    DEFINE_IS_NULL,
    ELEMENT_NOT_FOUND,
    READER_ERROR
};



struct Node
{
    UnaryorBinary  num_args;
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

TreeError ConstructorTree(Tree* tree);
void       DestructorTree(Tree* tree);


Node* CreateNode(Type type, void* value, Node* left, Node* right);

Node* CreateVariable(char* value, Node* left, Node* right);
Node* CreateNumber(double value, Node* left, Node* right);
Node* CreateOperator(Operators value, Node* left, Node* right);
void DeleteNode(Node* node);


void Tokenization(Token** tokens, size_t token_i, Text* buf);

Node* GetG(Text* buf, Token* tokens, size_t token_i);
Node* GetE(Text* buf, Token* tokens, size_t token_i);
Node* GetT(Text* buf, Token* tokens, size_t token_i);
Node* GetP(Text* buf, Token* tokens, size_t token_i);
Node* GetC(Text* buf, Token* tokens, size_t token_i);
Node* GetN(Text* buf, Token* tokens, size_t token_i);

void syntax_assert(bool x, Text* buf);




TreeError  PrintNode(Node* node, FILE* To, Order order_value);
void PrintObject(Node* node, FILE* To);
void PrintOperator(Operators value_Operators, FILE* TO);



#endif