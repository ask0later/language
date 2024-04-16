#include "error_allocator.h"
#include "read_file.h"
#include "tree.h"
#include "graphic.h"
#include "read_tree.h"
#include "printasm.h"



int main(const int argc, const char* argv[])
{
    if (argc != 3)
    {
        printf("Введите текстовый файл с деревом\n");
        return 1;
    }

    struct err_allocator err_alloc = {};
    CtorErrorAllocator(&err_alloc);

    const char* tree_file = argv[1];
    const char* asm_file  = argv[2];

    struct Text buf = {};

    

    CtorBuffer(&buf, tree_file, &err_alloc);
    if (err_alloc.need_call == true)
    {
        INSERT_ERROR_NODE(&err_alloc, "file failed to open");
        ERR_ALLOC_TERMINATE(&err_alloc);
        DtorBuffer(&buf);
    }
    

    Tree* trees[NUM_TREE] = {};
    size_t trees_num = 0;

    CreateTree(&buf, trees, &trees_num, &err_alloc);
    if (err_alloc.need_call == true)
    {
        INSERT_ERROR_NODE(&err_alloc, "invalid executing CreateTree");
        ERR_ALLOC_TERMINATE(&err_alloc);
        DestructorTrees(trees, trees_num);
        DtorBuffer(&buf);
        return 1;
    }
    
    
    GraphicDump(trees_num, trees[0], trees[1], trees[2], trees[3], trees[4]);

    FILE* output = fopen(asm_file, "w");
    if (output == NULL)
    {
        INSERT_ERROR_NODE(&err_alloc, "file failed to open");
        ERR_ALLOC_TERMINATE(&err_alloc);
        DestructorTrees(trees, trees_num);
        DtorBuffer(&buf);
        return 1;
    }

    PrintToAssembler(trees, trees_num, output);
    
    fclose(output);


    DtorBuffer(&buf);
    DtorErrorAllocator(&err_alloc);
    DestructorTrees(trees, trees_num);
}