#include "tree.h"
#include "readFile.h"
#include "graphic.h"



int main()
{
    struct Text buf = {};
    struct Tree test_tree = {};

    CreateBuffer(&buf, "file.txt");
    
    Iterator func_it = {};

    printf("Input text = <%s>\n", buf.str);

    Tokens tkns = {};
    ConstructorTokens(&tkns, &buf);

    CreateTokens(&tkns, &buf);
    
    DumpTokens(&tkns, NULL);
    
    TreeError error = NO_ERROR;

    test_tree.root = GetGrammar(&tkns, &func_it, &error);
    // printf("_____%lu_____\n", tkns.position);
    // if (error != NO_ERROR)
    // {
    //     DumpErrors(error, &tkns, &buf);
    //     DestructorTokens(&tkns);
    //     DeleteBuffer(&buf);
    //     return 1;
    // }

    //PrintNode(test_tree.root, stdout, &names, IN_ORDER);
    
    GraphicDump(&(func_it.funcs[0].tree), &(func_it.funcs[1].tree));

    //DeleteNode(test_tree.root);
    
    DestructorTokens(&tkns);
    //printf("____________________________");
    //DestructorTables(&tables);
    DestructorIterator(&func_it);

    DeleteBuffer(&buf);
}