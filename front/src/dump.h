#ifndef DUMPlib
#define DUMPlib

#include "lang.h"

void DumpTokens(Tokens* tkns);
void DumpToken(Node* current);
void DumpErrors(TreeError error, Tokens* tkns, Text* buf);
void DumpNamesTables(Iterator* func_it);
void DumpNamesTable(Function* func);

#endif