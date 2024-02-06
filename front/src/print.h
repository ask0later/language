#ifndef PRINTlib
#define PRINTlib

#include "tree.h"
#include "lang.h"

TreeError PrintTrees(FunctionShell* func_shell, FILE* To);

TreeError PrintNode(Node* node, FILE* To, Order order_value);
void PrintObject(Node* node, FILE* To);
void PrintOperator(Operators value_Operators, FILE* TO);

#endif