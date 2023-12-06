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


void DeleteToken(Node* node)
{
    if (node->type == VAR)
        free(node->data.variable);

    free(node);

    return;
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

void SkipSpaces(Text* buf)
{
    while (isspace(buf->str[buf->position]))
        buf->position++;
}

TreeError ConstructorTokens(Tokens* tkns, Text* buf)
{
    tkns->size = buf->size_buffer;
    tkns->position = 0;
    tkns->tokens = (Node**) calloc(tkns->size, sizeof(Node*));
    if (!tkns->tokens) return ALLOC_ERROR;
    
    return NO_ERROR;
}

TreeError DestructorTokens(Tokens* tkns)
{
    for (size_t i = 0; i < tkns->size; i++)
        DeleteToken(tkns->tokens[i]);

    tkns->size = (size_t) INT_MAX;
    tkns->position = (size_t) INT_MAX;
    free(tkns->tokens);
    
    return NO_ERROR;
}


TreeError CreateTokens(Tokens* tkns, Text* buf)
{
    while (buf->str[buf->position] != '\0')
    {
        SkipSpaces(buf);
        
        if (buf->position == buf->size_buffer)
            return NO_ERROR;

        ParseNumber(tkns, buf);
        
        ParseMathOperators(tkns, buf);

        ParseVariable(tkns, buf);
    }

    tkns->tokens[tkns->position] = CreateOperator(END, NULL, NULL);

    tkns->position++;

    tkns->size = tkns->position;

    Node** ptr = (Node**) realloc(tkns->tokens, tkns->size * sizeof(Node*));
    if (ptr == NULL) {return ALLOC_ERROR;}

    tkns->tokens = ptr;
    tkns->position = 0;

    return NO_ERROR;
}   

TreeError ParseVariable(Tokens* tkns, Text* buf)
{
    if (isalpha(buf->str[buf->position]))
    {
        char var[MAX_SIZE_NAME] = {};
        size_t i_var = 0;
        var[i_var] = buf->str[buf->position];
        i_var++;
        buf->position++;
        while (isalnum(buf->str[buf->position]) || buf->str[buf->position] == '_')
        {
            var[i_var] = buf->str[buf->position];
            buf->position++;
            i_var++;
        }
        tkns->tokens[tkns->position] = CreateVariable(var, NULL, NULL);
        tkns->position++;
    }

    return NO_ERROR;
}

TreeError ParseMathOperators(Tokens* tkns, Text* buf)
{
    for (size_t i = 0; i < NUM_MATH_COMMANDS; i++)
    {
        if (strncmp(&(buf->str[buf->position]), math_cmds[i].name, math_cmds[i].size_name) == 0)
        {
            buf->position += math_cmds[i].size_name;
     
            tkns->tokens[tkns->position] = CreateOperator(math_cmds[i].value, NULL, NULL);
            tkns->position++;
        }
    }

    return NO_ERROR;
}

TreeError ParseNumber(Tokens* tkns, Text* buf)
{
    int val = 0;
    while (isdigit(buf->str[buf->position]))
    {
        val = 10 * val + buf->str[buf->position] - '0';
        buf->position++;
    }
    if (val != 0)
    {
        tkns->tokens[tkns->position] = CreateNumber(val, NULL, NULL);
        tkns->position++;    
    }
    
    return NO_ERROR;
}

Node* GetG(Tokens* tkns)
{
    Node* current = GetExpression(tkns);
    return current;
}

Node* GetExpression(Tokens* tkns)
{
    
    Node* value_1 = GetTerm(tkns);
    Node* value_3 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        while ((tkns->tokens[tkns->position]->data.value_op == OP_ADD) || (tkns->tokens[tkns->position]->data.value_op == OP_SUB))
        {
            value_3 = tkns->tokens[tkns->position];
            tkns->position++;
            
            Node* value_2 = GetTerm(tkns);

            value_3->left = value_1;
            value_3->right = value_2;

            value_1 = value_3;
        }
    }
    
    return value_3;
}


Node* GetTerm(Tokens* tkns)
{
    Node* value_1 = GetUnary(tkns);
    
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        for (size_t i = 0; i < NUM_COMMANDS_T; i++)
        {
            if (cmdsT[i].value == tkns->tokens[tkns->position]->data.value_op)
            {
                Node* value_3 = tkns->tokens[tkns->position];
                tkns->position++;
                
                Node* value_2 = GetUnary(tkns);
                value_3->left = value_1;
                value_3->right = value_2;

                value_1 = value_3;
            }
        }
    }
    
    return value_1;
}

Node* GetUnary(Tokens* tkns)
{
    Node* value_1 = NULL;
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        for (size_t i = 0; i < NUM_COMMANDS_U; i++)
        {
            if (cmdsU[i].value == tkns->tokens[tkns->position]->data.value_op)
            {
                value_1 = tkns->tokens[tkns->position];
                tkns->position++;
                Node* value_2 = GetPrimaryExpression(tkns);

                value_1->right = value_2;
            }
        }
    }
    
    if (value_1 == NULL)
        value_1 = GetPrimaryExpression(tkns);

    return value_1;
}

Node* GetPrimaryExpression(Tokens* tkns)
{
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if (tkns->tokens[tkns->position]->data.value_op == L_BRACKET)
        {
            tkns->position++;
            Node* val = GetExpression(tkns);
            if (tkns->tokens[tkns->position]->data.value_op == R_BRACKET)
                tkns->position++;
            else
                return NULL;

            return val;
        }
    }

    return GetC(tkns);
}

Node* GetC(Tokens* tkns)
{
    if (tkns->tokens[tkns->position]->type == VAR)
    {
        Node* var = tkns->tokens[tkns->position];
        tkns->position++;
        return var;
    }
    return GetN(tkns);
}

Node* GetN(Tokens* tkns)
{

    if (tkns->tokens[tkns->position]->type == NUM)
    {
        Node* num = tkns->tokens[tkns->position];
        tkns->position++;
        return num;
    }

    return NULL;
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
        case L_BRACKET:
        case R_BRACKET:
        case END:
        default:
            printf("extra");
            break;
    }
}