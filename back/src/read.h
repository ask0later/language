#ifndef READlib
#define READlib
#include "tree.h"

const size_t SIZE_FUN_ = 4;
const size_t SIZE_VAR_ = 4;

TreeError CreateTree(Text* buf, Tree** array_tree, size_t* num_trees);


TreeError ReadNode(Text* buf, Tree* tree, Node** node, Order order_value);
TreeError SkipSpaces(Text* buf);
TreeError PasteObject(char* source, Node** node);
TreeError ReadObject(char* source, Text* buf);

#endif