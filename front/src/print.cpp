#include "print.h"


TreeError PrintTrees(Iterator* func_it, FILE* To)
{
    for (size_t i = 0; i < func_it->size; i++)
    {
        PrintNode(func_it->funcs[i].tree.root, To, PRE_ORDER);
    }

    return NO_ERROR_TREE;
}

TreeError PrintNode(Node* node, FILE* To, Order order_value)
{
    if (!node) {return NO_ERROR_TREE;}
    if (To == NULL) {return FILE_NOT_OPEN;}
    
    TreeError error = NO_ERROR_TREE;

    fprintf(To, "(");

    if (order_value == PRE_ORDER)
        PrintObject(node, To);

    if (node->left)
    {
        error = PrintNode(node->left, To, order_value);
        if (error != NO_ERROR_TREE)
            return error;
    }
    
    if (order_value == IN_ORDER)
        PrintObject(node, To);
    
    if (node->right)
    {
        error = PrintNode(node->right, To, order_value);
        if (error != NO_ERROR_TREE)
            return error;
    }
    
    if (order_value == POST_ORDER)
        PrintObject(node, To);

    fprintf(To, ")");

    return NO_ERROR_TREE;
}

void PrintObject(Node* node, FILE* To)
{
    if (node->type == NUMBER)
        fprintf(To, "%lg", node->data.value);
    else if (node->type == OPERATOR)
    {
        PrintOperator(node->data.id_op, To);
    }
    else if (node->type == VARIABLE)
    {
        fprintf(To, "var_%lu", node->data.id_var);
    }
    else if (node->type == FUNCTION)
    {
        fprintf(To, "func_%lu", node->data.id_fun);
    }
}

void PrintOperator(Operators value_Operators, FILE* To)
{
    switch(value_Operators)
    {
        case OP_ADD:
            fprintf(To, " + ");
            break;
        case OP_SUB:
            fprintf(To, " - ");
            break;
        case OP_MUL:
            fprintf(To, " * ");
            break;
        case OP_DIV:
            fprintf(To, " / ");
            break;
        case FUN_SIN:
            fprintf(To, " sin ");
            break;
        case FUN_COS:
            fprintf(To, " cos ");
            break;
        case FUN_POW:
            fprintf(To, " ^ ");
            break;
        case FUN_SQRT:
            fprintf(To, " sqrt ");
            break;
        case FUN_LN:
            fprintf(To, " ln ");
            break;
        case OP_CONDITION:
            fprintf(To, " if ");
            break;
        case OP_LOOP:
            fprintf(To, " while ");
            break;
        case OP_ASSIGN:
            fprintf(To, " = ");
            break;
        case OP_UN_SUB:
            fprintf(To, " - ");
            break;
        case SEMICOLON:
            fprintf(To, " ; ");
            break;
        case COMMA:
            fprintf(To, " , ");
            break;
        case OP_ABOVE:
            fprintf(To, " > ");
            break;
        case OP_BELOW:
            fprintf(To, " < ");
            break;
        case OP_EQUAL:
            fprintf(To, " == ");
            break;
        case OP_NO_EQUAL:
            fprintf(To, " != ");
            break;
        case L_BRACKET:
            fprintf(To, " ( ");
            break;
        case R_BRACKET:
            fprintf(To, " ) ");
            break;
        case L_CURLY_BRACKET:
            fprintf(To, " { ");
            break;
        case R_CURLY_BRACKET:
            fprintf(To, " } ");
            break;
        case DEFINE:
            fprintf(To, " define ");
            break;
        case RET:
            fprintf(To, " return ");
            break;    
        case END:
            fprintf(To, " END ");
            break;
        case INPUT:
            fprintf(To, " input ");
            break;
        case OUTPUT:
            fprintf(To, " output ");
            break;
        case NO_OP:
        case MORSE_PARTITION:
        default:
            printf("extra_dump");
            break;
    }
}