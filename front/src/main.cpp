#include "tree.h"
#include "readFile.h"
#include "graphic.h"
#include "dump.h"
#include "lang.h"
#include "print.h"

int main(const int argc, const char* argv[])
{
    if (argc != 3)
    {
        printf("Введите текстовый файл с программой\n");
        return 1;
    }

    const char* lang_file = argv[1];
    const char* tree_file = argv[2];

    struct Text buf = {};
    struct Tree main_tree = {};

    CreateBuffer(&buf, lang_file);
    
    Iterator func_it = {};

    printf("Input text = <%s>\n", buf.str);

    Tokens tkns = {};
    ConstructorTokens(&tkns, &buf);

    CreateTokens(&tkns, &buf);
    
    DumpTokens(&tkns);

    CreateNames(&(func_it.table_funcs));

    LangError error = NO_ERROR_LANG;
    
    main_tree.root = GetGrammar(&tkns, &func_it, &error);

    DumpNamesTables(&func_it);

    FILE* output = fopen(tree_file, "w");
    
    if (output == NULL)
    {
        DestructorIterator(&func_it);
        DestructorTokens(&tkns);
        DeleteBuffer(&buf);
        DeleteNames(&(func_it.table_funcs));
        printf("error: file not open\n");
        return 1;
    }

    PrintTrees(&func_it, output);

    fclose(output);
    
    GraphicDump(func_it.size, &(func_it.funcs[0].tree), &(func_it.funcs[1].tree), &(func_it.funcs[2].tree), &(func_it.funcs[3].tree), &(func_it.funcs[4].tree));// , &(func_it.funcs[5].tree));

    DestructorIterator(&func_it);
    DestructorTokens(&tkns);
    DeleteNames(&(func_it.table_funcs));
    DeleteBuffer(&buf);
}