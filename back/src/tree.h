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
const size_t MAX_SIZE_NAME  = 10;
const size_t MAX_SIZE_ARG   = 20;

const size_t NUM_TREE = 10;

enum Order
{
    PRE_ORDER  = 1,
    IN_ORDER   = 2,
    POST_ORDER = 3,
};

enum TreeError
{
    NO_ERROR_TREE,
    LOOP_ERROR,
    ALLOC_ERROR_TREE,
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
    NUMBER = 1,
    OPERATOR,
    VARIABLE,
    FUNCTION,
    KEY_WORD
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
    COMMA,
    MORSE_PARTITION,
    OP_CONDITION,
    INPUT,
    OUTPUT,
    OP_LOOP,
    OP_ASSIGN,
    OP_ABOVE,
    OP_BELOW,
    OP_EQUAL,
    OP_NO_EQUAL,
    DEFINE,
    RET,
    END
};

union tag_data
{
    double         value;
    char name[MAX_SIZE_NAME];
    size_t        id_fun;
    size_t        id_var;
    Operators      id_op;
};

struct Node
{
    tag_data    data;
    Type        type;
    Node*       left;
    Node*      right;
    size_t  text_pos;
};

struct Tree
{
    Node* root;
    size_t size;
    size_t num_vars;
    size_t changes_num;
};

Tree* ConstructorTree();
void       DestructorTree(Tree* tree);

void DestructorTrees(Tree** trees, size_t num_trees);


Node* CreateNode(Type type, void* value, Node* left, Node* right);
Node* Copynator(Node* node);

Node* CreateVariable(size_t id_var, char* name, Node* left, Node* right);
Node* CreateFunction(size_t id_fun, char* name, Node* left, Node* right);
Node* CreateNumber(double value, Node* left, Node* right);
Node* CreateOperator(Operators value, Node* left, Node* right);

void DeleteNode(Node* node);

///////////////////

#endif