#include "graphic.h"
#include "Dotter.h"

TreeError   GraphicDump(size_t num_trees, ...)
{
    dtBegin("Tree.dot");                        // Начало dot-описания графа

    dtNodeStyle ().shape        ("box");
    dtNodeStyle ().style         ("filled");
    dtNodeStyle ().fontcolor      ("black");

    va_list one_tree;
    va_start(one_tree, num_trees);
    
    for (size_t i = 0; i < num_trees; i++)
    {
        GraphicDumpTree(va_arg(one_tree, Tree*));
    }

    va_end(one_tree);

    dtEnd();                                   // Конец dot-описания графа

    dtRender("Tree.dot", "Tree.jpg", "jpg", false);

    return NO_ERROR_TREE;
}

TreeError GraphicDumpTree(Tree* tree)
{
    if (tree)
        GraphicDumpNode(tree->root);
    
    return NO_ERROR_TREE;
}


TreeError GraphicDumpNode(Node* node)
{
    if (!node) return NO_ERROR_TREE;

    if (!node) {return NO_ERROR_TREE;}

    char str[MAX_SIZE_ARG] = {};
    if (node->type == NUMBER)
    {
        dtNodeStyle().fillcolor("#7BF2DA");//HEX_TIFFANY
        sprintf(str, "%lg", node->data.value);
    }
    else if (node->type == OPERATOR)
    {
        dtNodeStyle().fillcolor("#EE204D");//HEX_RED
        switch(node->data.id_op)
        {
            case OP_ADD:
                sprintf(str, " + ");
                break;
            case OP_SUB:
                sprintf(str, " - ");
                break;
            case OP_MUL:
                sprintf(str, " * ");
                break;
            case OP_DIV:
                sprintf(str, " / ");
                break;
            case FUN_SIN:
                sprintf(str, " sin ");
                break;
            case FUN_COS:
                sprintf(str, " cos ");
                break;
            case FUN_POW:
                sprintf(str, " ^ ");
                break;
            case FUN_SQRT:
                sprintf(str, " sqrt ");
                break;
            case FUN_LN:
                sprintf(str, " ln ");
                break;
            case OP_ABOVE:
                sprintf(str, " > ");
                break;
            case OP_BELOW:
                sprintf(str, " < ");
                break;
            case OP_ASSIGN:
                sprintf(str, " = ");
                break;
            case OP_EQUAL:
                sprintf(str, " == ");
                break;
            case OP_NO_EQUAL:
                sprintf(str, " != ");
                break;
            case OP_CONDITION:
                sprintf(str, " if ");
                break;
            case OP_LOOP:
                sprintf(str, " while ");
                break;
            case OP_UN_SUB:
                sprintf(str, " - ");
                break;
            case SEMICOLON:
                sprintf(str, " ; ");
                break;
            case COMMA:
                sprintf(str, " , ");
                break;
            case RET:
                sprintf(str, " return ");
                break;
            case INPUT:
                sprintf(str, " input ");
                break;
            case OUTPUT:
                sprintf(str, " output ");
                break;
            case NO_OP:
            case L_CURLY_BRACKET:
            case R_CURLY_BRACKET:
            case MORSE_PARTITION:
            case L_BRACKET:
            case R_BRACKET:
            case END:
            case DEFINE:
            default:
                printf("extra_graphic");
                break;
        }
    }
    else if (node->type == VARIABLE)
    {
        dtNodeStyle().fillcolor("#21C912");//HEX_GREEN
        sprintf(str, "var_%lu", node->data.id_var);
    }
    else if (node->type == FUNCTION)
    {
        dtNodeStyle().fillcolor("#735499");//HEX_PURPLE
        sprintf(str, "func_%lu", node->data.id_fun);
    }
    
    
    dtNode((int)(size_t) node, str);

    if (node->left)
    {
        GraphicDumpNode(node->left);
        dtLink((int)(size_t) node, (int)(size_t)(node->left), "");
    }

    if (node->right)
    {
        GraphicDumpNode(node->right);
        dtLink((int)(size_t) node, (int)(size_t)(node->right), "");
    }

    return NO_ERROR_TREE;
}