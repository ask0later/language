#include "readFile.h"
#include "tree.h"
#include "graphic.h"
#include "read.h"
#include "simple.h"
#include "print.h"



int main()
{
    struct Text buf = {};

    Tree* trees[NUM_TREE] = {};

    CreateBuffer(&buf, "../tree_text.txt");
    printf("Input text = <%s>\n", buf.str);

    size_t num_trees = 0;
    CreateTree(&buf, trees, &num_trees);

    SimplificationTrees(trees, num_trees);
    GraphicDump(num_trees, trees[0], trees[1], trees[2], trees[3], trees[4]);

    FILE* output = fopen("../simple_tree_text.txt", "w");
    if (output == NULL)
    {
        DestructorTrees(trees, num_trees);
        DeleteBuffer(&buf);
        printf("error: file not open\n");
        return 1;
    }

    PrintTrees(trees, num_trees, output);

    fclose(output);
    
    DestructorTrees(trees, num_trees);

    DeleteBuffer(&buf);
}