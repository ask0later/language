#include "tree.h"
#include "readFile.h"
#include "graphic.h"



int main()
{
    struct Text buf = {};
    struct Tree test_tree = {};

    CreateBuffer(&buf, "file.txt");

    printf("%s\n", buf.str);

    Token tokens[MAX_NUM_TOKENS] = {};
    Token* tkn = tokens;
    printf("%c\n", buf.str[buf.position]);
    printf("%c\n", buf.str[buf.position + 1]);
    Tokenization(&tkn, 0, &buf);

    printf("%d", tokens[0].type);
    printf("%d", tokens[1].type);
    printf("%d", tokens[2].type);
    printf("%d", tokens[3].type);
    test_tree.root = GetG(&buf, tokens, 0);
    
    //PrintNode(test_tree.root, stdout, IN_ORDER);
    GraphicDump(&test_tree, NULL);

    DeleteNode(test_tree.root);

    DeleteBuffer(&buf);
    //Node* current = GetG(Parse* parse);
}