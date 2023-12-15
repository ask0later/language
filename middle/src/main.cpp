#include "readFile.h"
#include "tree.h"
#include "graphic.h"
#include "read.h"
#include "simple.h"
#include "print.h"



int main()
{
    struct Text buf = {};

    Tree* array_tree[NUM_TREE] = {};

    CreateBuffer(&buf, "../tree_text.txt");
    printf("Input text = <%s>\n", buf.str);

    size_t num_trees = 0;
    CreateTree(&buf, array_tree, &num_trees);

    SimplificationTrees(array_tree, num_trees);
    GraphicDump(num_trees, array_tree[0], array_tree[1]);

    FILE* output = fopen("../simple_tree_text.txt", "w");
    if (output == NULL)
    {
        DestructorTrees(array_tree, num_trees);
        DeleteBuffer(&buf);
        printf("error: file not open\n");
        return 1;
    }

    PrintTrees(array_tree, num_trees, output);

    fclose(output);
    
    DestructorTrees(array_tree, num_trees);

    DeleteBuffer(&buf);
}