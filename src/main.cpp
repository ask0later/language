#include "tree.h"
#include "readFile.h"
#include "graphic.h"



int main()
{
    struct Text buf = {};
    struct Tree test_tree = {};

    CreateBuffer(&buf, "file.txt");

    printf("%s\n", buf.str);

    Tokens tkns = {};
    ConstructorTokens(&tkns, &buf);

    CreateTokens(&tkns, &buf);
    DumpTokens(&tkns);
    test_tree.root = GetGrammar(&tkns);
    
    GraphicDump(&test_tree, NULL);

    //DeleteNode(test_tree.root);
    
    DestructorTokens(&tkns);

    DeleteBuffer(&buf);
}

