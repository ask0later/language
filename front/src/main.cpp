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
    
    FunctionShell func_shell = {};

    printf("Input text = <%s>\n", buf.str);

    Tokens tkns = {};
    ConstructorTokens(&tkns, &buf);

    CreateTokens(&tkns, &buf);
    
    DumpTokens(&tkns);

    LangError error = NO_ERROR_LANG;
    
    main_tree.root = GetGrammar(&tkns, &func_shell, &error);

    DumpNamesTables(&func_shell);

    FILE* output = fopen(tree_file, "w");
    
    if (output == NULL)
    {
        DestructorIterator(&func_shell);
        DestructorTokens(&tkns);
        DeleteBuffer(&buf);
        printf("error: file not open\n");
        return 1;
    }

    PrintTrees(&func_shell, output);

    fclose(output);
    
    GraphicDump(func_shell.size, &(func_shell.funcs[0].tree), &(func_shell.funcs[1].tree), &(func_shell.funcs[2].tree), &(func_shell.funcs[3].tree), &(func_shell.funcs[4].tree), &(func_shell.funcs[5].tree));

    DestructorIterator(&func_shell);
    DestructorTokens(&tkns);
    DeleteBuffer(&buf);
}