#include "tree.h"

// TreeError ConstructorTables(Tables* tables)
// {
//     tables->i_table = 0;
//     tables->tables = (Names*) calloc(MAX_NUM_TABLES, sizeof(Names));
//     if (!tables->tables) return ALLOC_ERROR;

//     return NO_ERROR; 
// }

// TreeError DestructorTables(Tables* tables)
// {
//     DeleteNames(&(tables->tables));

//     free(tables->tables);
// }

TreeError DestructorIterator(Iterator* func_it)
{
    size_t i = 0; 
    while (i < MAX_NUM_FUNCTIONS)
    {
        i++;
        // DestructorTree(&(func_it->funcs[i].tree));
        // DeleteNames(&(func_it->funcs[i]));
    }

    return NO_ERROR;
}

TreeError CreateNames(Function* funcs)
{
    funcs->num_names = 0;

    funcs->names = (_Name*) calloc(MAX_NUM_NAMES, sizeof(_Name));

    return NO_ERROR;
}

TreeError DeleteNames(Function* funcs)
{
    free(funcs->names);

    return NO_ERROR;
}

Node* CreateVariable(size_t id_var, char* name, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = VARIABLE;

    memcpy(node->data.name, name, strlen(name));
    //node->data.id_var = id_var;

    return node;
}

Node* CreateFunction(size_t id_fun, char* name, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = FUNCTION;

    memcpy(node->data.name, name, strlen(name));

    //node->data.id_fun = id_fun;

    return node;
}

Node* CreateNumber(double value, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = NUMBER;

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

    node->data.id_op = value;

    return node;
}

Node* CreateNode(Type type, void* value, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = type;


    switch (node->type)
    {
    case NUMBER:
        node->data.value = *((double*) value);
        break;
    case OPERATOR:
        node->data.id_op  = *((Operators*) value);
        break;
    case VARIABLE:
        //node->data.variable = strdup((const char*) value);
        break;
    case FUNCTION:
    case KEY_WORD:
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

        ParseFunction(tkns, buf);
    }

    tkns->tokens[tkns->position] = CreateOperator(END, NULL, NULL);
    tkns->tokens[tkns->position]->text_pos = tkns->position;

    tkns->position++;

    tkns->size = tkns->position;

    Node** ptr = (Node**) realloc(tkns->tokens, tkns->size * sizeof(Node*));
    if (ptr == NULL) {return ALLOC_ERROR;}

    tkns->tokens = ptr;
    tkns->position = 0;

    return NO_ERROR;
}   

bool FindCommand(Text* buf, const Command* cmds, const size_t num_commands, Operators* id, size_t* offset)
{
    for (size_t i = 0; i < num_commands; i++)
    {
        if (strncmp(&(buf->str[buf->position]), cmds[i].name, cmds[i].size_name) == 0)
        {
            (*offset) = cmds[i].size_name;
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


TreeError ParseFunction(Tokens* tkns, Text* buf)
{
    Operators id = NO_OP;
    size_t offset = 0;

    if (FindCommand(buf, function, NUM_COMMANDS_FUNCTION, &id, &offset) == true)
    {
        tkns->tokens[tkns->position] = CreateOperator(id, NULL, NULL);
        tkns->tokens[tkns->position]->text_pos = buf->position;
        tkns->position++;

        buf->position += offset;

        if (id == DEFINE)
        {
            SkipSpaces(buf);

            if (buf->str[buf->position] == '|')
            {
                buf->position++;

                MorseAlhabet morse_word[SIZE_SYMBOLS] = {};
                size_t i_vars = 0;
                size_t text_pos = buf->position - 1;
                ReadMorseCode(morse_word, &i_vars, buf);
                
                double value = 0;

                char name_fun[MAX_SIZE_NAME] = {};
                size_t size_name = 0;
                TranslateMorseCode(name_fun, morse_word, &size_name);

                size_t id_fun = 0;

                //MatchNamesTable(names, name_fun, &id_var);
                
                tkns->tokens[tkns->position] = CreateFunction(id_fun, name_fun, NULL, NULL);
                tkns->tokens[tkns->position]->text_pos = text_pos;

                (tkns->position)++;
                
                return NO_ERROR;
            }
        }
    }

    return NO_ERROR;
}

//ReadMorseCode

TreeError ParseNumOrVar(Tokens* tkns, Text* buf)
{
    if (buf->str[buf->position] == '|')
    {
        (buf->position)++;

        MorseAlhabet morse_word[SIZE_SYMBOLS] = {};

        size_t i_vars = 0;
        size_t text_pos = buf->position - 1;
        
        ReadMorseCode(morse_word, &i_vars, buf);
        
        double value = 0;

        if (((morse_word[0]) - MORSE_0) > (MORSE_9 - MORSE_0))
        {
            char name_var[MAX_SIZE_NAME] = {};
            size_t size_name = 0;
            TranslateMorseCode(name_var, morse_word, &size_name);

            size_t id_name = 0;

            //MatchNamesTable(names, name_var, &id_name);

            tkns->tokens[tkns->position] = CreateVariable(id_name, name_var, NULL, NULL);
            tkns->tokens[tkns->position]->text_pos = text_pos;
            tkns->position++;
            
            return NO_ERROR;
        }

        for (size_t i = 0; i < i_vars; i++)
            value = 10 * value + (double)(morse_word[i] - MORSE_0);
        //if ((morse_word[i] - MORSE_0) > 10) ERROR

        tkns->tokens[tkns->position] = CreateNumber(value, NULL, NULL);
        tkns->tokens[tkns->position]->text_pos = text_pos;
        tkns->position++;

        return NO_ERROR;
    }

    return NO_ERROR;
}

TreeError ReadMorseCode(MorseAlhabet* id_value, size_t* i_vars, Text* buf)
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


TreeError MatchNamesTable(Table* names, char* name_var, size_t* id_var)
{
    bool match = false;

    for (size_t i_var = 0; i_var < names->num_var; i_var++)
    {
        if (strncmp(name_var, names->vars[i_var].name, names->vars[i_var].name_size) == 0)
        {
            match = true;
            (*id_var) = names->vars[i_var].id_name;
            return NO_ERROR;
        }
    }

    if (match == false)
    {
        size_t size_name = strlen(name_var);

        (*id_var) = names->num_var;
        names->vars[names->num_var].name_size = size_name;
        memcpy(names->vars[names->num_var].name, name_var, size_name);
        names->num_var++;
    }

    return NO_ERROR;
}

TreeError TranslateMorseCode(char* name_var, MorseAlhabet* var, size_t* size_name)
{
    size_t i = 0;
    while (var[i] != 0)
    {
        name_var[i] = var[i];
        i++;
    }
    (*size_name) = i;

    return NO_ERROR;
}


TreeError ParseLanguageOperators(Tokens* tkns, Text* buf)
{
    Operators id = NO_OP;
    size_t offset = 0;

    if (FindCommand(buf, cmds_lang, NUM_LANG_COMMANDS, &id, &offset) == true)
    {
        tkns->tokens[tkns->position] = CreateOperator(id, NULL, NULL);
        tkns->tokens[tkns->position]->text_pos = buf->position;
        
        tkns->position++;
        buf->position += offset;
    }

    return NO_ERROR;
}

TreeError ParseBoolOperators(Tokens* tkns, Text* buf)
{
    Operators id = NO_OP;
    size_t offset = 0;

    if (FindCommand(buf, cmds_compare, NUM_COMPARE_COMMANDS, &id, &offset) == true)
    {
        tkns->tokens[tkns->position] = CreateOperator(id, NULL, NULL);
        tkns->tokens[tkns->position]->text_pos = buf->position;
        
        tkns->position++;
        buf->position += offset;
    }

    return NO_ERROR;
}

TreeError ParseMathOperators(Tokens* tkns, Text* buf)
{
    Operators id = NO_OP;
    size_t offset = 0;

    if (FindCommand(buf, math_cmds, NUM_MATH_COMMANDS, &id, &offset) == true)
    {
        tkns->tokens[tkns->position] = CreateOperator(id, NULL, NULL);
        tkns->tokens[tkns->position]->text_pos = buf->position;
        
        tkns->position++;
        buf->position += offset;
    }

    return NO_ERROR;
}

Node* GetGrammar(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    while (tkns->tokens[tkns->position]->data.id_op == DEFINE)
    {
        printf("____________________________\n");
        (tkns->position)++;
        func_it->funcs[func_it->i_func].tree.root = GetFunction(tkns, func_it, error);
        (func_it->i_func)++;
    }

    return func_it->funcs[func_it->i_func - 1].tree.root;
}

Node* GetOperators(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    Node* value_1 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if ((tkns->tokens[tkns->position]->data.id_op == OP_CONDITION) ||\
            (tkns->tokens[tkns->position]->data.id_op == OP_LOOP))
            value_1 = GetWhileOrIf(tkns, func_it, error);
    }
    else if (tkns->tokens[tkns->position]->type == VARIABLE)
    {
        value_1 = GetAssign(tkns, func_it, error);
    }
    
    if (value_1)
    {
        value_1->right = GetOperators(tkns, func_it, error);
    }

    return value_1;
}

Node* GetFunction(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    Node* value_1 = NULL;

    if (tkns->tokens[tkns->position]->type == FUNCTION)
    {
        value_1 = tkns->tokens[tkns->position];
        printf("AAA");
        (tkns->position)++;
    }
    else
    {
        return NULL;
    }

    if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
    {
        printf("BB");
        (tkns->position)++;
    }
    else
    {
        return NULL;
    }

    Node* value_2 = NULL;
    Node* value_3 = NULL;

    do 
    {
        value_2 = tkns->tokens[tkns->position];
        (tkns->position)++;
        
        if (tkns->tokens[tkns->position]->data.id_op == COMMA)
        {
            value_3 = tkns->tokens[tkns->position];
            (tkns->position)++;

            value_3->left = value_2;
            value_3->right = tkns->tokens[tkns->position];;
            
            (tkns->position)++;
            value_2 = value_3;
        }
        
    } while ((tkns->tokens[tkns->position]->data.id_op != R_BRACKET));

    value_1->left = value_2;

    (tkns->position)++;

    if (tkns->tokens[tkns->position]->data.id_op == L_CURLY_BRACKET)
    {
        (tkns->position)++;
    }
    else
    {
        return NULL;
    }

    value_1->right = GetOperators(tkns, func_it, error);

    if (tkns->tokens[tkns->position]->data.id_op == R_CURLY_BRACKET)
    {
        (tkns->position)++;
    }
    else
    {
        return NULL;
    }

    if (tkns->tokens[tkns->position]->data.id_op == SEMICOLON)
    {
        (tkns->position)++;
    }
    else
    {
        return NULL;
    }

    return value_1;
}

Node* GetWhileOrIf(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    Node* value_1 = NULL;
    Node* value_2 = NULL;
    
    if ((tkns->tokens[tkns->position]->data.id_op == OP_CONDITION) || (tkns->tokens[tkns->position]->data.id_op == OP_LOOP))
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        (*error) = ERROR_IF_WHILE;
        return NULL;
    }

    value_1->left = GetBoolPrimaryExpression(tkns, func_it, error);
    
    if (tkns->tokens[tkns->position]->data.id_op == L_CURLY_BRACKET)
    {
        (tkns->position)++;
        
        value_1->right = GetOperators(tkns, func_it, error);

        if (tkns->tokens[tkns->position]->data.id_op == R_CURLY_BRACKET)
        {
            (tkns->position)++;
        }
        else
        {
            (*error) = ERROR_BRACKET;
            return NULL;
        }

        if (tkns->tokens[tkns->position]->data.id_op == SEMICOLON)
        {
            value_2 = tkns->tokens[tkns->position];
            value_2->left = value_1;
            (tkns->position)++;
        }
        else
        {
            (*error) = ERROR_SEMICOLON;
            return NULL;
        }
    }
    else 
    {
        (*error) = ERROR_BRACKET;
        return NULL;
    }

    return value_2;
}

Node* GetAssign(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    Node* left = GetVariable(tkns, func_it, error);
    Node* value_1 = NULL;
    
    if (tkns->tokens[tkns->position]->data.id_op == OP_ASSIGN)
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        (*error) = ERROR_ASSIGN;
        return NULL;
    }

    Node* right = GetExpression(tkns, func_it, error);

    value_1->left = left;
    value_1->right = right;

    Node* value_2 = NULL;

    if (tkns->tokens[tkns->position]->data.id_op == SEMICOLON)
    {
        value_2 = tkns->tokens[tkns->position];
        (tkns->position)++;
        value_2->left = value_1;
    }
    else
    {
        (*error) = ERROR_SEMICOLON;
        return NULL;
    }

    return value_2;
}

Node* GetExpression(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    Node* value_1 = GetTerm(tkns, func_it, error);
    Node* value_3 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        while ((tkns->tokens[tkns->position]->data.id_op == OP_ADD) || (tkns->tokens[tkns->position]->data.id_op == OP_SUB))
        {
            value_3 = tkns->tokens[tkns->position];
            tkns->position++;
            
            Node* value_2 = GetTerm(tkns, func_it, error);
            
            value_3->left = value_1;
            value_3->right = value_2;

            value_1 = value_3;
        }
    }
    
    return value_1;
}

Node* GetBoolingExpression(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    Node* value_1 = GetExpression(tkns, func_it, error);
    Node* value_3 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if ((tkns->tokens[tkns->position]->data.id_op == OP_ABOVE) || (tkns->tokens[tkns->position]->data.id_op == OP_BELOW))
        {
            value_3 = tkns->tokens[tkns->position];
            tkns->position++;
            
            Node* value_2 = GetExpression(tkns, func_it, error);
            
            value_3->left = value_1;
            value_3->right = value_2;

            value_1 = value_3;
        }
    }
    
    return value_1;
}


Node* GetTerm(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    Node* value_1 = GetUnary(tkns, func_it, error);
    
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        for (size_t i = 0; i < NUM_COMMANDS_T; i++)
        {
            if (cmdsT[i].id == tkns->tokens[tkns->position]->data.id_op)
            {
                Node* value_3 = tkns->tokens[tkns->position];
                tkns->position++;
                
                Node* value_2 = GetUnary(tkns, func_it, error);
                value_3->left = value_1;
                value_3->right = value_2;

                value_1 = value_3;
            }
        }
    }
    
    return value_1;
}

Node* GetUnary(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    Node* value_1 = NULL;
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        for (size_t i = 0; i < NUM_COMMANDS_U; i++)
        {
            if (cmdsU[i].id == tkns->tokens[tkns->position]->data.id_op)
            {
                value_1 = tkns->tokens[tkns->position];
                tkns->position++;
                Node* value_2 = GetPrimaryExpression(tkns, func_it, error);

                value_1->right = value_2;
            }
        }
    }
    
    if (value_1 == NULL)
        value_1 = GetPrimaryExpression(tkns, func_it, error);

    return value_1;
}

Node* GetBoolPrimaryExpression(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
        {
            tkns->position++;
            Node* val = GetBoolingExpression(tkns, func_it, error);
            if (tkns->tokens[tkns->position]->data.id_op == R_BRACKET)
                tkns->position++;
            else
                return NULL;

            return val;
        }
    }

    return GetVariable(tkns, func_it, error);
}

Node* GetPrimaryExpression(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
        {
            tkns->position++;
            Node* val = GetExpression(tkns, func_it, error);
            if (tkns->tokens[tkns->position]->data.id_op == R_BRACKET)
                tkns->position++;
            else
                return NULL;

            return val;
        }
    }

    return GetVariable(tkns, func_it, error);
}

Node* GetVariable(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    if (tkns->tokens[tkns->position]->type == VARIABLE)
    {
        Node* var = tkns->tokens[tkns->position];
        tkns->position++;
        return var;
    }
    return GetNumber(tkns, func_it, error);
}

Node* GetNumber(Tokens* tkns, Iterator* func_it, TreeError* error)
{
    if (tkns->tokens[tkns->position]->type == NUMBER)
    {
        Node* num = tkns->tokens[tkns->position];
        tkns->position++;
        return num;
    }

    return NULL;
}

// TreeError ReadMorseVariables(Tokens* tkns, Table* names)
// {
//     for (size_t i_tkns = 0; i_tkns < tkns->size; i_tkns++)
//     {
//         if (tkns->tokens[i_tkns]->type == VARIABLE)
//         {
//             char name_var[MAX_SIZE_NAME] = {};
//             size_t size_name = 0;
//             ReadMorseWord(name_var, tkns->tokens[i_tkns], &size_name);

//             bool match = false;
//             for (size_t i_var = 0; i_var < names->num_var; i_var++)
//             {
//                 if (strncmp(name_var, names->vars[i_var].name, names->vars[i_var].name_size) == 0)
//                 {
//                     match = true;
//                     tkns->tokens[i_tkns]->data.id_var = names->vars[i_var].id_var;
//                 }
//             }

//             if (match == false)
//             {
//                 tkns->tokens[i_tkns]->data.id_var = names->num_var;
//                 names->vars[names->num_var].name_size = size_name;
//                 memcpy(names->vars[names->num_var].name, name_var, size_name);
//                 names->num_var++;
//             }
//         }
//     }

//     return NO_ERROR;
// }

// TreeError ReadMorseWord(char* name_var, Node* current, size_t* size_name)
// {
//     size_t i = 0;
//     while (current->data.var[i] != 0)
//     {
//         name_var[i] = current->data.var[i];
//         i++;
//     }
//     (*size_name) = i;

//     return NO_ERROR;
// }


TreeError PrintNode(Node* node, FILE* To, Table* names, Order order_value)
{
    if (!node) {return NO_ERROR;}
    if (To == NULL) {return FILE_NOT_OPEN;}
    
    TreeError error = NO_ERROR;

    printf("(");

    if (order_value == PRE_ORDER)
        PrintObject(node, To, names);

    if (node->left)
    {
        error = PrintNode(node->left, To, names, order_value);
        if (error != NO_ERROR)
            return error;
    }
    
    if (order_value == IN_ORDER)
        PrintObject(node, To, names);
    
    if (node->right)
    {
        error = PrintNode(node->right, To, names, order_value);
        if (error != NO_ERROR)
            return error;
    }
    
    if (order_value == POST_ORDER)
        PrintObject(node, To, names);

    printf(")");

    return NO_ERROR;
}

void PrintObject(Node* node, FILE* To, Table* names)
{
    if (node->type == NUMBER)
        fprintf(To, "%lg", node->data.value);
    else if (node->type == OPERATOR)
    {
        PrintOperator(node->data.id_op, To);
    }
    else if (node->type == VARIABLE)
    {
        fprintf(To, "%s", node->data.name);
        //fprintf(To, "%s", names->vars[node->data.id_var].name);
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
        case NO_OP:
        case MORSE_PARTITION:
        default:
            printf("extra");
            break;
    }
}

void DumpTokens(Tokens* tkns, Table* names)
{
    for (size_t i = 0; i < tkns->size; i++)
    {
        printf("tokens[%2lu] = ", i);
        DumpToken(tkns->tokens[i], names);
        printf("(указывает на строчку с позиции %lu\n", tkns->tokens[i]->text_pos);
        //printf("\n");
    }
}

void DumpToken(Node* current, Table* names)
{
    if (current->type == OPERATOR)
    {
        PrintOperator(current->data.id_op, stdout);
        printf("_%d_", current->data.id_op);
    }
    else if (current->type == NUMBER)
    {
        printf(" %lg ", current->data.value);
    }
    else if (current->type == VARIABLE)
    {
        printf("name = <%s>;", current->data.name);
        //printf("id_var = %lu; name = <%s>;", current->data.id_var, names->vars[current->data.id_var].name);
    }
    else if (current->type == FUNCTION)
    {
        printf("function name = <%s>;", current->data.name);
    }
}

void DumpErrors(TreeError error, Tokens* tkns, Text* buf)
{
    printf("\n\n%s\n", &buf->str[tkns->tokens[tkns->position]->text_pos]);
    printf("после этого оператора ожидалось");

}