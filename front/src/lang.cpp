#include "lang.h"
#include "operators.h"
#include "error_allocator.h"


int CreateNames(Name** name)
{
    (*name) = (Name*) calloc(MAX_NUM_NAMES, sizeof(Name));

    return 0;
}

int DeleteNames(Name** name)
{
    free(*name);

    return 0;
}

void DeleteToken(Node* node)
{
    free(node);
    return;
}

int ConstructorTokens(Tokens* tkns, Text* buf, err_allocator* err_alloc)
{
    tkns->size = buf->size_buffer;
    tkns->position = 0;
    tkns->tokens = (Node**) calloc(tkns->size, sizeof(Node*));
    if (!tkns->tokens)
    {
        ERROR_INSERT("dynamic allocation is fault");
        return 1;
    }
    
    return 0;
}

int DestructorTokens(Tokens* tkns)
{
    for (size_t i = 0; i < tkns->size; i++)
        DeleteToken(tkns->tokens[i]);

    tkns->size = (size_t) INT_MAX;
    tkns->position = (size_t) INT_MAX;
    free(tkns->tokens);
    
    return 0;
}

void SkipSpaces(Text* buf)
{
    while (isspace(buf->str[buf->position]))
        buf->position++;
}



int CreateTokens(Tokens* tkns, Text* buf, err_allocator* err_alloc)
{
    while (buf->str[buf->position] != '\0')
    {
        SkipSpaces(buf);
        
        if (buf->position == buf->size_buffer)
            return 0;

        ParseOperator(tkns, buf);
        
        ParseNameOrNumber(tkns, buf);
    }

    tkns->tokens[tkns->position] = CreateOperator(END, NULL, NULL);
    tkns->tokens[tkns->position]->text_pos = tkns->position;

    tkns->position++;

    tkns->size = tkns->position;

    Node** ptr = (Node**) realloc(tkns->tokens, tkns->size * sizeof(Node*));

    if (ptr == NULL) 
    {
        ERROR_INSERT("dynamic allocation is fault (realloc)");
        return 1;
    }

    tkns->tokens = ptr;
    tkns->position = 0;

    return 0;
}   


int ParseOperator(Tokens* tkns, Text* buf)
{
    for (size_t i = 0; i < COMMAND_NUM; i++)
    {
        if (strncmp(buf->str + buf->position, language_cmds[i].name, language_cmds[i].size_name) == 0)
        {
            tkns->tokens[tkns->position] = CreateOperator(language_cmds[i].id, NULL, NULL);
            tkns->tokens[tkns->position]->text_pos = buf->position;
        
            tkns->position++;
            buf->position += language_cmds[i].size_name;

            return 0;
        }
    }
    return 0;
}


int ParseNameOrNumber(Tokens* tkns, Text* buf)
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

            if (tkns->tokens[tkns->position - 1]->data.id_op == DEFINE)
            {
                tkns->tokens[tkns->position] = CreateFunction(id_name, name_var, NULL, NULL);
            }
            else
            {
                tkns->tokens[tkns->position] = CreateVariable(id_name, name_var, NULL, NULL);
            }

            tkns->tokens[tkns->position]->text_pos = text_pos;
            tkns->position++;
            
            return 0;
        }

        for (size_t i = 0; i < i_vars; i++)
        {
            if ((morse_word[i] - MORSE_0) > 10)
            {
                //ERROR_INSERT("syntax error");
                return 1;
            }
            value = 10 * value + (double)(morse_word[i] - MORSE_0);
        }

        tkns->tokens[tkns->position] = CreateNumber(value, NULL, NULL);
        tkns->tokens[tkns->position]->text_pos = text_pos;
        tkns->position++;

        return 0;
    }

    return 0;


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


int ReadMorseCode(MorseAlhabet* id_value, size_t* i_vars, Text* buf)
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

    return 0;
}


int TranslateMorseCode(char* name_var, MorseAlhabet* var, size_t* size_name)
{
    size_t i = 0;
    while (var[i] != 0)
    {
        name_var[i] = var[i];
        i++;
    }
    (*size_name) = i;

    return 0;
}








// int MatchFuncNamesTable(Function* funcs, char* name_var, size_t* id_fun)
// {
//     bool match = false;

//     for (size_t i_func = 0; i_func < func_it->size; i_func++)
//     {
//         if (strncmp(name_var, func_it->table_funcs[i_func].name, func_it->table_funcs[i_func].name_size) == 0)
//         {
//             match = true;
//             (*id_fun) = func_it->table_funcs[i_func].id;
//             return 0;
//         }
//     }
    
//     if (match == false)
//     {
//         size_t size_name = strlen(name_var);

//         (*id_fun) = func_it->size;

//         func_it->table_funcs[func_it->size].name_size = size_name;
//         func_it->table_funcs[func_it->size].id = (*id_fun);

//         func_it->table_funcs[func_it->size].type = FUNCTION;

//         memcpy(func_it->table_funcs[func_it->size].name, name_var, size_name);
//     }

//     return 0;
// }

// int MatchNamesTable(Function* funcs, char* name_var, size_t* id_var)
// {
//     bool match = false;

//     Function* func = &(func_it->funcs[func_it->size]);

//     for (size_t i_var = 0; i_var < func->num_names; i_var++)
//     {
//         if (strncmp(name_var, func->vars[i_var].name, func->vars[i_var].name_size) == 0)
//         {
//             match = true;
//             (*id_var) = func->vars[i_var].id;
//             return 0;
//         }
//     }

//     if (func_it->size == 0)
//     {
//         func_it->funcs[func_it->size].offset = 0;
//     }
//     if (func_it->size != 0)
//     {
//         func_it->funcs[func_it->size].offset = func_it->funcs[func_it->size - 1].num_names + func_it->funcs[func_it->size - 1].offset;
//     }

//     if (match == false)
//     {
//         size_t size_name = strlen(name_var);

//         (*id_var) = func->num_names + func_it->funcs[func_it->size].offset;

//         func->vars[func->num_names].id = (*id_var);
//         func->vars[func->num_names].type = VARIABLE;
//         func->vars[func->num_names].name_size = size_name;

//         memcpy(func->vars[func->num_names].name, name_var, size_name);
//         (func->num_names)++;
//     }

//     return 0;
// }





Node* GetGrammar(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    size_t i_funcs= 0;

    while (tkns->tokens[tkns->position]->data.id_op == DEFINE)
    {
        (tkns->position)++;
        
        // CreateNames((funcs[i_funcs].vars));

        funcs[i_funcs].tree.root = GetFunction(tkns, funcs, err_alloc);
        i_funcs++;
    }

    return funcs[i_funcs - 1].tree.root;
}



Node* GetFunction(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* value_1 = NULL;

    if (tkns->tokens[tkns->position]->type == FUNCTION)
    {
        value_1 = tkns->tokens[tkns->position];
        size_t id = 0;

        // MatchFuncNamesTable(funcs, value_1->data.name, &id);
        
        value_1->data.id_var = id;
        
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected function");
        return NULL;
    }

    value_1->left = GetArgument(tkns, funcs, err_alloc);

    // if (value_1->left == NULL)
    // {
    //     ERROR_INSERT("arguments is bad");
    //     return NULL;
    // }

    if (tkns->tokens[tkns->position]->data.id_op == L_CURLY_BRACKET)
    {
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected <{>");
        return NULL;
    }

    value_1->right = GetOperators(tkns, funcs, err_alloc);

// if (value_1->right == NULL)
    // {
    //     ERROR_INSERT("error in operators");
    //     return NULL;
    // }    

    if (tkns->tokens[tkns->position]->data.id_op == R_CURLY_BRACKET)
    {
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected <}>");
        return NULL;
    }

    return value_1;
}


Node* GetOperators(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* node_operator = NULL;

    Node* current_token = tkns->tokens[tkns->position];

    if (current_token->type == OPERATOR)
    {
        if ((tkns->tokens[tkns->position]->data.id_op == OP_CONDITION) || (tkns->tokens[tkns->position]->data.id_op == OP_LOOP))
        {
            node_operator = GetWhileOrIf(tkns, funcs, err_alloc);
        }
        else if (tkns->tokens[tkns->position]->data.id_op == RET)
        {
            node_operator = GetReturn(tkns, funcs, err_alloc);
        }
        else if ((tkns->tokens[tkns->position]->data.id_op == INPUT) || (tkns->tokens[tkns->position]->data.id_op == OUTPUT))
        {
            node_operator = GetInOutput(tkns, funcs, err_alloc);
        }
    }
    else if (tkns->tokens[tkns->position]->type == VARIABLE)
    {
        node_operator = GetAssign(tkns, funcs, err_alloc);
    }
    
    if (node_operator)
    {
        
        node_operator->right = GetOperators(tkns, funcs, err_alloc);

        // value_1 = GetOperators(tkns, funcs, err_alloc);
    }

    return node_operator;
}

Node* GetInOutput(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* node_in_out = NULL;

    if ((tkns->tokens[tkns->position]->data.id_op == INPUT) || (tkns->tokens[tkns->position]->data.id_op == OUTPUT))
    {
        node_in_out = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected in/out put");
        return NULL;
    }

    if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
    {
        tkns->position++;
    }
    else
    {
        ERROR_INSERT("expected <(>");
        return NULL;
    }
    
    node_in_out->left = GetVariable(tkns, funcs, err_alloc);

    if (tkns->tokens[tkns->position]->data.id_op == R_BRACKET)
    {
        tkns->position++;
    }
    else
    {
        ERROR_INSERT("expected <)>");
        return NULL;
    }

    Node* current = NULL;

    if (tkns->tokens[tkns->position]->data.id_op == SEMICOLON)
    {
        current = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected <;>");
        return NULL;
    }

    current->left = node_in_out;

    return current;
}

Node* GetArgument(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
    {
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected <(>");
        return NULL;
    }

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        (tkns->position)++;
        return NULL;
    }

    Node* value_2 = NULL;
    Node* value_3 = NULL;

    do 
    {
        if (tkns->tokens[tkns->position]->type == VARIABLE)
        {
            value_2 = GetExpression(tkns, funcs, err_alloc);
        }
        else if (tkns->tokens[tkns->position]->type == OPERATOR)
        {
            if (tkns->tokens[tkns->position]->data.id_op != COMMA)
                value_2 = GetExpression(tkns, funcs, err_alloc);
        }
        if (tkns->tokens[tkns->position]->data.id_op == COMMA)
        {
            value_3 = tkns->tokens[tkns->position];
            (tkns->position)++;

            value_3->left = value_2;
            value_3->right = GetExpression(tkns, funcs, err_alloc);
            
            value_2 = value_3;
        }
        
    } while ((tkns->tokens[tkns->position]->data.id_op != R_BRACKET));

    (tkns->position)++;

    return value_2;
}

Node* GetReturn(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* value_1 = NULL;

    if (tkns->tokens[tkns->position]->data.id_op == RET)
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected ret");
        return NULL;
    }

    value_1->left = GetExpression(tkns, funcs, err_alloc); 

    if (tkns->tokens[tkns->position]->data.id_op == SEMICOLON)
    {
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected <;>");
        return NULL;
    }

    return value_1;
}

Node* GetWhileOrIf(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* value_1 = NULL;
    
    if ((tkns->tokens[tkns->position]->data.id_op == OP_CONDITION) || (tkns->tokens[tkns->position]->data.id_op == OP_LOOP))
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected if or while");
        return NULL;
    }

    value_1->left = GetBoolPrimaryExpression(tkns, funcs, err_alloc);
    
    if (tkns->tokens[tkns->position]->data.id_op == L_CURLY_BRACKET)
    {
        (tkns->position)++;
        
        value_1->right = GetOperators(tkns, funcs, err_alloc);

        if (tkns->tokens[tkns->position]->data.id_op == R_CURLY_BRACKET)
        {
            (tkns->position)++;
        }
        else
        {
            ERROR_INSERT("expected <}>");
            return NULL;
        }
    }
    else 
    {
        ERROR_INSERT("expected <{>");
        return NULL;
    }

    return value_1;
}

Node* GetAssign(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* left = GetVariable(tkns, funcs, err_alloc);
    Node* value_1 = NULL;
    
    if (tkns->tokens[tkns->position]->data.id_op == OP_ASSIGN)
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        ERROR_INSERT("expected <=>");
        return NULL;
    }

    Node* right = GetExpression(tkns, funcs, err_alloc);
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
        ERROR_INSERT("expected <;>");
        return NULL;
    }

    return value_2;
}

Node* GetExpression(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* value_1 = GetTerm(tkns, funcs, err_alloc);
    Node* value_3 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        while ((tkns->tokens[tkns->position]->data.id_op == OP_ADD) || (tkns->tokens[tkns->position]->data.id_op == OP_SUB))
        {
            value_3 = tkns->tokens[tkns->position];
            tkns->position++;
            
            Node* value_2 = GetTerm(tkns, funcs, err_alloc);
            
            value_3->left = value_1;
            value_3->right = value_2;

            value_1 = value_3;
        }
    }
    
    return value_1;
}

Node* GetTerm(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* value_1 = GetUnary(tkns, funcs, err_alloc);
    
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        while ((tkns->tokens[tkns->position]->data.id_op == OP_MUL) || (tkns->tokens[tkns->position]->data.id_op == OP_DIV))
        {
            Node* value_3 = tkns->tokens[tkns->position];
            tkns->position++;
            
            Node* value_2 = GetUnary(tkns, funcs, err_alloc);
            value_3->left = value_1;
            value_3->right = value_2;
            
            value_1 = value_3;
        }
    }
    
    return value_1;
}

Node* GetUnary(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
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
                Node* value_2 = GetPrimaryExpression(tkns, funcs, err_alloc);

                value_1->right = value_2;
            }
        }
    }
    
    if (value_1 == NULL)
    {
        value_1 = GetPrimaryExpression(tkns, funcs, err_alloc);
    }
    return value_1;
}

Node* GetBoolingExpression(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    Node* value_1 = GetExpression(tkns, funcs, err_alloc);
    Node* value_3 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if ((tkns->tokens[tkns->position]->data.id_op == OP_ABOVE) || (tkns->tokens[tkns->position]->data.id_op == OP_BELOW) || (tkns->tokens[tkns->position]->data.id_op == OP_EQUAL) || (tkns->tokens[tkns->position]->data.id_op == OP_NO_EQUAL))
        {
            value_3 = tkns->tokens[tkns->position];
            tkns->position++;
            
            Node* value_2 = GetExpression(tkns, funcs, err_alloc);
            
            value_3->left = value_1;
            value_3->right = value_2;

            value_1 = value_3;
        }
    }
    
    return value_1;
}

Node* GetBoolPrimaryExpression(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
        {
            tkns->position++;
            Node* val = GetBoolingExpression(tkns, funcs, err_alloc);
            if (tkns->tokens[tkns->position]->data.id_op == R_BRACKET)
                tkns->position++;
            else
                return NULL;

            return val;
        }
    }

    return GetVariable(tkns, funcs, err_alloc);
}

Node* GetPrimaryExpression(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
        {
            tkns->position++;
            Node* val = GetExpression(tkns, funcs, err_alloc);
            if (tkns->tokens[tkns->position]->data.id_op == R_BRACKET)
                tkns->position++;
            else
                return NULL;

            return val;
        }
    }
    return GetVariable(tkns, funcs, err_alloc);
}

Node* GetVariable(Tokens* tkns, Function* funcs, err_allocator* err_alloc)
{
    if (tkns->tokens[tkns->position]->type == VARIABLE)
    {
        Node* var = tkns->tokens[tkns->position];
        (tkns->position)++;

        // size_t id = 0;

        if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
        {
            var->type = FUNCTION;
            var->left = GetArgument(tkns, funcs, err_alloc);
        }

            // MatchFuncNamesTable(func_it, var->data.name, &id);
            // var->data.id_fun = id;
            // }
        // else
        // {
        //     // MatchNamesTable(func_it, var->data.name, &id);
        //     // var->data.id_var = id;
        // }

        // var->left = GetArgument(tkns, funcs, err_alloc);

        return var;
    }
    
    return GetNumber(tkns, err_alloc);
}

Node* GetNumber(Tokens* tkns, err_allocator* err_alloc)
{
    if (tkns->tokens[tkns->position]->type == NUMBER)
    {
        Node* num = tkns->tokens[tkns->position];
        tkns->position++;
        return num;
    }
    else
    {
        ERROR_INSERT("expected number");
        return NULL;
    }

    return NULL;
}