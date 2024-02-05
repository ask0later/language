#include "readFile.h"
#include "tree.h"
#include "graphic.h"
#include "read.h"
#include "printasm.h"



int main(const int argc, const char* argv[])
{
    if (argc != 3)
    {
        printf("Введите текстовый файл с деревом\n");
        return 1;
    }

    const char* tree_file = argv[1];
    const char* asm_file  = argv[2];

    struct Text buf = {};

    Tree* trees[NUM_TREE] = {};

    CreateBuffer(&buf, tree_file);
    printf("Input text = <%s>\n", buf.str);

    size_t num_trees = 0;
    CreateTree(&buf, trees, &num_trees);

    GraphicDump(num_trees, trees[0], trees[1], trees[2], trees[3], trees[4]);

    FILE* output = fopen(asm_file, "w");
    if (output == NULL)
    {
        DestructorTrees(trees, num_trees);
        DeleteBuffer(&buf);
        printf("error: file not open\n");
        return 1;
    }

    PrintToAssembler(trees, num_trees, output);
    
    fclose(output);
    DestructorTrees(trees, num_trees);

    DeleteBuffer(&buf);
}