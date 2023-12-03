#include "tree.h"

Node* CreateVariable(char* value, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = VAR;

    node->data.variable = strdup(value);

    return node;
}

Node* CreateNumber(double value, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = NUM;

    node->data.value = value;

    return node;
}

Node* CreateOperator(Operators value, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = OPERATOR;

    node->data.value_op = value;

    return node;
}


Node* CreateNode(Type type, void* value, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = type;

    // node->data.value_op  = NO_OP;
    // node->data.value_fun = NO_FUN;
    // node->data.variable  = NULL;

    switch (node->type)
    {
    case NUM:
        node->data.value = *((double*) value);
        break;
    case OPERATOR:
        node->data.value_op  = *((Operators*) value);
        break;
    case VAR:
        node->data.variable = strdup((const char*) value);
        break;

    default:
        break;
    }

    return node;
}

void DeleteNode(Node* node)
{
    if (!node) return;

    if (node->type == VAR)
        free(node->data.variable);
    
    if (node->left)
        DeleteNode(node->left);
    if (node->right)
        DeleteNode(node->right);
    
    free(node);
    
    return;
}

void Tokenization(Token** tokens, size_t token_i, Text* buf)
{   
    if (buf->position == buf->size_buffer)
        return;
    // while (isspace(buf->str[buf->position]))
    //     buf->position++;
        
    printf("!%c!", buf->str[buf->position]);

    int val = 0;

    while (isdigit(buf->str[buf->position]))
    {
        val = 10 * val + buf->str[buf->position] - '0';
        buf->position++;
    }
    if (val != 0)
    {
        (*tokens)[token_i].type = NUM;
        printf("AXAXAX");
        (*tokens)[token_i].data.value = val;
        token_i++;
        return Tokenization(tokens, token_i, buf);
    }

    char op[MAX_SIZE_NAME] = {};
    
    if (buf->position < buf->size_buffer - 2)
    {
        op[0] = buf->str[buf->position];
        op[1] = buf->str[buf->position + 1];
        op[2] = buf->str[buf->position + 2];
    }
    else if (buf->position < buf->size_buffer - 1)
    {
        op[0] = buf->str[buf->position];
        op[1] = buf->str[buf->position + 1];
    }
    else 
    {
        op[0] = buf->str[buf->position];
    }

    for (size_t i = 0; i < NUM_COMMANDS; i++)
    {
        printf("<%s> and <%s> \n", op, cmds[i].name);
        if (strncmp(cmds[i].name, op, cmds[i].size_name) == 0)
        {
            printf("MMM");
            buf->position += cmds[i].size_name;
            (*tokens)[token_i].type = OPERATOR;
            (*tokens)[token_i].data.value_op = (Operators) cmds[i].value;
            token_i++;
            return Tokenization(tokens, token_i, buf);
        }
    }

    

    // if (isalpha(buf->str[buf->position]))
    // {
    //     char var[MAX_SIZE_NAME] = {};
    //     size_t i_var = 0;
    //     var[i] = buf->str[buf->position];
    //     i_var++;
    //     buf->position++;
    //     while (isalnum(buf->str[buf->position]) || buf->str[buf->position] == '_')
    //     {
    //         var[i] = buf->str[buf->position];
    //         buf->position++;
    //         i_var++;
    //     }
    //     tokens[i] =
    //     i++;
    // }
    //return Tokenization(tokens, token_i, buf);
}   

Node* GetG(Text* buf, Token* tokens, size_t token_i)
{
    Node* current = GetE(buf, tokens, token_i);
    //syntax_assert(buf->str[buf->position] == '\0', buf);
    return current;
}

Node* GetE(Text* buf, Token* tokens, size_t token_i)
{
    Node* val = GetT(buf, tokens, token_i);

    while((buf->str[buf->position] == '+') || (buf->str[buf->position] == '-'))
    {
        char op = buf->str[buf->position];
        buf->position++;
        Node* val2 = GetT(buf, tokens);
        switch (op)
        {
            case '+': val = CreateOperator(OP_ADD, val, val2); break;
            case '-': val = CreateOperator(OP_SUB, val, val2); break;
            default: printf("extra"); syntax_assert(false, buf);
                break;
        }
    }
    //syntax_assert(buf->str[buf->position] != '\0', buf);
    return val;
}


Node* GetT(Text* buf, Token* tokens)
{
    Node* val = GetP(buf, tokens);
    
    char op[MAX_SIZE_NAME]= {};

    if (buf->position < buf->size_buffer - 2)
    {
        op[0] = buf->str[buf->position];
        op[1] = buf->str[buf->position + 1];
        op[2] = buf->str[buf->position + 2];
    }

    bool x = true;
    do 
    {
        for (size_t i = 0; i < NUM_COMMANDS_T; i++)
        {
            if (strncmp(cmdsT[i].name, op, cmdsT[i].size_name) == 0)
            {
                fprintf(stderr, "<%s> and <%s> size = %lu\n", op, cmds[i].name, cmds[i].size_name);
                buf->position += cmds[i].size_name;
                Node* val2 = GetP(buf, tokens);
                val = CreateOperator((Operators) cmdsT[i].value, val, val2);
                x = false;
            }
        }
        if (x == false)
            x = true;
        if (x == true)
            x = false;
    } while (x == true);

    // while((buf->str[buf->position] == '*') || (buf->str[buf->position] == '/'))
    // {
    //     char op = buf->str[buf->position];
    //     buf->position++;
    //     Node* val2 = GetP(buf);
    //     switch (op)
    //     {
    //         case '*': val = CreateOperator(OP_MUL, val, val2); break;
    //         case '/': val = CreateOperator(OP_DIV, val, val2); break;
    //         default: printf("extra"); syntax_assert(false, buf);
    //             break;
    //     }
    // }
    
    //syntax_assert(buf->str[buf->position] != '\0', buf);
    return val;
}

Node* GetP(Text* buf, Token* tokens)
{
    if (buf->str[buf->position] == '(')
    {
        Node* val = 0;
        buf->position++;
        val = GetE(buf, tokens);
        syntax_assert(buf->str[buf->position] == ')', buf);
        buf->position++;

        return val;
    }

    return GetC(buf, tokens);
}

Node* GetC(Text* buf, Token* tokens)
{
    if (isalpha(buf->str[buf->position]))
    {
        char val[MAX_SIZE_NAME] = {};
        size_t i = 0;
        val[i] = buf->str[buf->position];
        i++;
        buf->position++;
        while (isalnum(buf->str[buf->position]) || buf->str[buf->position] == '_')
        {
            val[i] = buf->str[buf->position];
            buf->position++;
            i++;
        }
        return CreateVariable(val, NULL, NULL);
    }
    return GetN(buf, tokens);   
}

Node* GetN(Text* buf, Token* tokens)
{
    int val = 0;
    size_t old_position = buf->position;
    while(isdigit(buf->str[buf->position]))
    {
        val = val * 10 + buf->str[buf->position] - '0';
        buf->position++;
    }

    syntax_assert(buf->position > old_position, buf);

    return CreateNumber(val, NULL, NULL);
}

void syntax_assert(bool x, Text* buf)
{
    if (x == false)
    {
        printf("syntax error: %s\n", buf->str);
        printf("              ");
        for (size_t i = 0; i < buf->position; i++)
        {
            printf(" ");
        }
        printf("^\n");


        exit(1);
    }
}

TreeError PrintNode(Node* node, FILE* To, Order order_value)
{
    if (!node) {return NO_ERROR;}
    if (To == NULL) {return FILE_NOT_OPEN;}
    
    TreeError error = NO_ERROR;

    fprintf(To, "( ");

    if (order_value == PRE_ORDER)
        PrintObject(node, To);

    if (node->left)
    {
        error = PrintNode(node->left, To, order_value);
        if (error != NO_ERROR)
            return error;
    }
    
    if (order_value == IN_ORDER)
        PrintObject(node, To);
    
    if (node->right)
    {
        error = PrintNode(node->right, To, order_value);
        if (error != NO_ERROR)
            return error;
    }
    
    if (order_value == POST_ORDER)
        PrintObject(node, To);

    fprintf(To, " ) ");

    return NO_ERROR;
}

void PrintObject(Node* node, FILE* To)
{
    if (node->type == NUM)
        fprintf(To, "%lg", node->data.value);
    else if (node->type == OPERATOR)
    {
        PrintOperator(node->data.value_op, To);
    }
    else if (node->type == VAR)
    {
        fprintf(To, "%s", node->data.variable);
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
        default:
            printf("extra");
            break;
    }
}