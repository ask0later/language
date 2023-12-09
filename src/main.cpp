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
    
    // printf("<%s>\n", &(buf.str[12]));
    // printf("<%s>\n", &(buf.str[13]));
    // printf("<%s>\n", &(buf.str[14]));
    // printf("<%s>\n", &(buf.str[15]));
    // printf("<%s>\n", &(buf.str[16]));
    // printf("<%s>\n", &(buf.str[17]));
    // printf("<%s>\n", &(buf.str[18]));
    // printf("___%lu\n%s\n", buf.size_buffer, buf.str);

    CreateTokens(&tkns, &buf);
    DumpTokens(&tkns);
    //test_tree.root = GetG(&tkns);
    
    GraphicDump(&test_tree, NULL);

    //DeleteNode(test_tree.root);
    
    DestructorTokens(&tkns);

    DeleteBuffer(&buf);
}

