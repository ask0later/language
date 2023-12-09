#include "tree.h"

Node* CreateVariable(int* var, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = VAR;

    memcpy(node->data.var, var, SIZE_MORSE_SYMBOL);

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
        //node->data.variable = strdup((const char*) value);
        break;

    default:
        break;
    }

    return node;
}


void DeleteToken(Node* node)
{
    // if (node->type == VAR)
    //     //free(node->data.variable);
    free(node);

    return;
}

void DeleteNode(Node* node)
{
    if (!node) return;

    // if (node->type == VAR)
    //     free(node->data.variable);
    
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
        
        ParseMathOperators(tkns, buf);

        ParseBoolOperators(tkns, buf);

        ParseLanguageOperators(tkns, buf);

        ParseNumOrVar(tkns, buf);
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

bool FindCommand(Text* buf, const Command* cmds, const size_t num_commands, Operators* id)//, size_t* offset)
{
    for (size_t i = 0; i < num_commands; i++)
    {
        if (strncmp(&(buf->str[buf->position]), cmds[i].name, cmds[i].size_name) == 0)
        {
            buf->position += cmds[i].size_name;
            (*id) = cmds[i].id;

            return true;
        }
    }

    return false;
}

bool FindSymbol(char* source, const MorseAlpha* morse_symbol, const size_t num_symbols, MorseAlhabet* id)
{
    for (size_t i = 0; i < num_symbols; i++)
    {
        if (strcmp(source, morse_symbol[i].encoding) == 0)
        {
            (*id) = morse_symbol[i].value;

            return true;
        }
    }
    
    return false;
}

TreeError ParseNumOrVar(Tokens* tkns, Text* buf)
{
    if (buf->str[buf->position] == '|')
    {
        (buf->position)++;

        ParseMorseCode(tkns, buf);
    }

    return NO_ERROR;
}

TreeError ReadingMorseCode(MorseAlhabet* id_value, size_t* i_vars, Text* buf)
{
    char var[SIZE_MORSE_SYMBOL] = {};

    size_t i_var  = 0;
   
    MorseAlhabet id = MORSE_0;

    while (buf->str[buf->position] != '|')
    {
        var[i_var] = buf->str[buf->position];
        buf->position++;
        i_var++;

        if (buf->str[buf->position] == ' ')
        {
            FindSymbol(var, morse_alnum, NUM_ALNUM, &id);
    
            id_value[*i_vars] = id;

            (*i_vars)++;
            memset(var, 0, SIZE_MORSE_SYMBOL);
            i_var = 0;
            buf->position++;
        }
    }

    if (i_var != 0)
    {
        FindSymbol(var, morse_alnum, NUM_ALNUM, &id);
        id_value[*i_vars] = id;
        (*i_vars)++;
    }

    buf->position++;

    return NO_ERROR;
}

TreeError ParseMorseCode(Tokens* tkns, Text* buf)
{
    MorseAlhabet id_value[SIZE_SYMBOLS] = {};
    size_t i_vars = 0;

    ReadingMorseCode(id_value, &i_vars, buf);
    
    double value = 0;

    if ((id_value[0] - MORSE_0) > (MORSE_9 - MORSE_0))
    {
        tkns->tokens[tkns->position] = CreateVariable((int*) id_value, NULL, NULL);
        tkns->position++;
        
        return NO_ERROR;
    }

    for (size_t i = 0; i < i_vars; i++)
        value = 10 * value + (double)(id_value[i] - MORSE_0);

    tkns->tokens[tkns->position] = CreateNumber(value, NULL, NULL);
    tkns->position++;

    return NO_ERROR;
}

TreeError ParseLanguageOperators(Tokens* tkns, Text* buf)
{
    Operators id = NO_OP;

    if (FindCommand(buf, cmds_lang, NUM_LANG_COMMANDS, &id) == true)
    {
        tkns->tokens[tkns->position] = CreateOperator(id, NULL, NULL);
        tkns->position++;
    }

    return NO_ERROR;
}

TreeError  ParseBoolOperators(Tokens* tkns, Text* buf)
{
    Operators id = NO_OP;

    if (FindCommand(buf, cmds_compare, NUM_COMPARE_COMMANDS, &id) == true)
    {
        tkns->tokens[tkns->position] = CreateOperator(id, NULL, NULL);
        tkns->position++;
    }

    return NO_ERROR;
}

TreeError ParseMathOperators(Tokens* tkns, Text* buf)
{
    Operators id = NO_OP;

    if (FindCommand(buf, math_cmds, NUM_MATH_COMMANDS, &id) == true)
    {
        tkns->tokens[tkns->position] = CreateOperator(id, NULL, NULL);
        tkns->position++;
    }

    return NO_ERROR;
}

Node* GetGrammar(Tokens* tkns)
{
    Node* current = GetOperators(tkns);
    
    return current;
}

Node* GetOperators(Tokens* tkns)
{
    Node* value_1 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if (tkns->tokens[tkns->position]->data.value_op == OP_CONDITION)
        {
            value_1 = GetIf(tkns);
        }
        else if (tkns->tokens[tkns->position]->data.value_op == OP_LOOP)
        {
            value_1 = GetLoop(tkns); 
        }
    }
    else if (tkns->tokens[tkns->position]->type == VAR)
    {
        value_1 = GetAssign(tkns);
    }
    
    if (value_1)
    {
        value_1->right = GetOperators(tkns);
    }

    return value_1;
}

Node* GetIf(Tokens* tkns)
{   
    Node* value_1 = NULL;
    Node* value_2 = NULL;
    
    if (tkns->tokens[tkns->position]->data.value_op == OP_CONDITION)
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }

    value_1->left = GetBoolPrimaryExpression(tkns);
    
    if (tkns->tokens[tkns->position]->data.value_op == L_CURLY_BRACKET)
    {
        (tkns->position)++;
        
        value_1->right = GetOperators(tkns);

        if (tkns->tokens[tkns->position]->data.value_op == R_CURLY_BRACKET)
        {
            (tkns->position)++;
        }

        if (tkns->tokens[tkns->position]->data.value_op == SEMICOLON)
        {
            value_2 = tkns->tokens[tkns->position];
            value_2->left = value_1;
            (tkns->position)++;
        }
    }
    else 
    {
        return NULL;
    }

    return value_2;
}

Node* GetLoop(Tokens* tkns)
{
    Node* value_1 = NULL;
    Node* value_2 = NULL;
    
    if (tkns->tokens[tkns->position]->data.value_op == OP_LOOP)
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    
    value_1->left = GetBoolPrimaryExpression(tkns);
    
    if (tkns->tokens[tkns->position]->data.value_op == L_CURLY_BRACKET)
    {
        (tkns->position)++;
        
        value_1->right = GetOperators(tkns);
        
        if (tkns->tokens[tkns->position]->data.value_op == R_CURLY_BRACKET)
        {
            (tkns->position)++;
        }
    }
    else 
    {
        return NULL;
    }
    
    return value_1;
}


Node* GetAssign(Tokens* tkns)
{
    Node* left = GetVariable(tkns);
    Node* value_1 = NULL;
    
    if (tkns->tokens[tkns->position]->data.value_op == OP_ASSIGN)
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        return NULL;
    }

    Node* right = GetExpression(tkns);

    value_1->left = left;
    value_1->right = right;

    Node* value_2 = NULL;

    if (tkns->tokens[tkns->position]->data.value_op == SEMICOLON)
    {
        value_2 = tkns->tokens[tkns->position];
        (tkns->position)++;
        value_2->left = value_1;
    }
    else
    {
        return NULL;
    }

    return value_2;
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
    
    return value_1;
}

Node* GetBoolingExpression(Tokens* tkns)
{
    Node* value_1 = GetExpression(tkns);
    Node* value_3 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if ((tkns->tokens[tkns->position]->data.value_op == OP_ABOVE) || (tkns->tokens[tkns->position]->data.value_op == OP_BELOW))
        {
            value_3 = tkns->tokens[tkns->position];
            tkns->position++;
            
            Node* value_2 = GetExpression(tkns);
            
            value_3->left = value_1;
            value_3->right = value_2;

            value_1 = value_3;
        }
    }
    
    return value_1;
}


Node* GetTerm(Tokens* tkns)
{
    Node* value_1 = GetUnary(tkns);
    
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        for (size_t i = 0; i < NUM_COMMANDS_T; i++)
        {
            if (cmdsT[i].id == tkns->tokens[tkns->position]->data.value_op)
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
            if (cmdsU[i].id == tkns->tokens[tkns->position]->data.value_op)
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

Node* GetBoolPrimaryExpression(Tokens* tkns)
{
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if (tkns->tokens[tkns->position]->data.value_op == L_BRACKET)
        {
            tkns->position++;
            Node* val = GetBoolingExpression(tkns);
            if (tkns->tokens[tkns->position]->data.value_op == R_BRACKET)
                tkns->position++;
            else
                return NULL;

            return val;
        }
    }

    return GetVariable(tkns);
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

    return GetVariable(tkns);
}

Node* GetVariable(Tokens* tkns)
{
    if (tkns->tokens[tkns->position]->type == VAR)
    {
        Node* var = tkns->tokens[tkns->position];
        tkns->position++;
        return var;
    }
    return GetNumber(tkns);
}

Node* GetNumber(Tokens* tkns)
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
        fprintf(To, "%d %d %d", node->data.var[0], node->data.var[1], node->data.var[2]);
        //fprintf(To, "%s", node->data.variable);
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
        case OP_ABOVE:
            fprintf(To, " > ");
            break;
        case OP_BELOW:
            fprintf(To, " < ");
            break;
        case NO_OP:
        case L_CURLY_BRACKET:
        case R_CURLY_BRACKET:
        case MORSE_PARTITION:
        case L_BRACKET:
        case R_BRACKET:
        case END:
        default:
            printf("extra");
            break;
    }
}


void DumpTokens(Tokens* tkns)
{
    for (size_t i = 0; i < tkns->size; i++)
    {
        if (tkns->tokens[i]->type == OPERATOR)
        {
            printf("operator = %d;\n", tkns->tokens[i]->data.value_op);
        }
        else if (tkns->tokens[i]->type == NUM)
        {
            printf("num = %lg;\n", tkns->tokens[i]->data.value);
        }
        else if (tkns->tokens[i]->type == VAR)
        {
            printf("var = <%d %d %d>;\n", tkns->tokens[i]->data.var[0], tkns->tokens[i]->data.var[1], tkns->tokens[i]->data.var[2]);
            //printf("var = <%s>;\n", tkns->tokens[i]->data.variable);
        }
    }
}