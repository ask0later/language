#include "tree.h"
#include "readFile.h"
#include "graphic.h"
#include "dump.h"
#include "lang.h"
#include "print.h"
#include "error_allocator.h"


int main(const int argc, const char* argv[])
{
    if (argc != 3)
    {
        printf("Введите текстовый файл с программой\n");
        return 1;
    }
    
    struct err_allocator err_alloc = {};
    error_allocator_Ctor(&err_alloc);

    const char* lang_file = argv[1];
    const char* tree_file = argv[2];

    struct Text buf = {};
    struct Tree main_tree = {};

    if (CreateBuffer(&buf, lang_file, &err_alloc) == 1)
    {
        dump_error(&err_alloc);

        error_allocator_Dtor(&err_alloc);
        DeleteBuffer(&buf);
        return 1;
    }
    
    Tokens tkns = {};

    if (ConstructorTokens(&tkns, &buf, &err_alloc) == 1)
    {
        dump_error(&err_alloc);

        error_allocator_Dtor(&err_alloc);
        DestructorTokens(&tkns);
        DeleteBuffer(&buf);
        return 1;
    }

    if (CreateTokens(&tkns, &buf, &err_alloc) == 1)
    {
        dump_error(&err_alloc);

        error_allocator_Dtor(&err_alloc);
        DestructorTokens(&tkns);
        DeleteBuffer(&buf);
        return 1;
    }
    
    DumpTokens(&tkns);

    Function funcs[10] = {};
    
    main_tree.root = GetGrammar(&tkns, funcs, &err_alloc);
    
    if (main_tree.root == NULL)
    {
        dump_error(&err_alloc);

        error_allocator_Dtor(&err_alloc);
        DestructorTokens(&tkns);
        DeleteBuffer(&buf);
        return 1;
    }

    // DumpNamesTables(&func_shell);





    // FILE* output = fopen(tree_file, "w");
    
    // if (output == NULL)
    // {
    //     DestructorIterator(&func_shell);
    //     DestructorTokens(&tkns);
    //     DeleteBuffer(&buf);
    //     printf("error: file not open\n");
    //     return 1;
    // }


    //PrintTrees(&func_shell, output);

    // fclose(output);
    
    GraphicDump(5, &(funcs[0].tree), &(funcs[1].tree), &(funcs[2].tree), &(funcs[3].tree), &(funcs[4].tree), &(funcs[5].tree));

    //DestructorIterator(&func_shell);
    DestructorTokens(&tkns);
    DeleteBuffer(&buf);
}