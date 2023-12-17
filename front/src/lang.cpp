#include "lang.h"
#include "operators.h"

LangError DestructorIterator(Iterator* func_it)
{
    for (size_t i = 0; i < func_it->size; i++)
    {
        //DestructorTree(&(func_it->funcs[i].tree));

        DeleteNames(&(func_it->funcs[i].vars));
        //free(func_it->funcs[i].vars);

        //DeleteNames(&(func_it->funcs[i]));
    }

    return NO_ERROR_LANG;
}

LangError CreateNames(Name** name)
{
    (*name) = (Name*) calloc(MAX_NUM_NAMES, sizeof(Name));

    return NO_ERROR_LANG;
}

LangError DeleteNames(Name** name)
{
    free(*name);

    return NO_ERROR_LANG;
}



void DeleteToken(Node* node)
{
    free(node);

    return;
}

LangError ConstructorTokens(Tokens* tkns, Text* buf)
{
    tkns->size = buf->size_buffer;
    tkns->position = 0;
    tkns->tokens = (Node**) calloc(tkns->size, sizeof(Node*));
    if (!tkns->tokens) return ALLOC_ERROR_LANG;
    
    return NO_ERROR_LANG;
}

LangError DestructorTokens(Tokens* tkns)
{
    for (size_t i = 0; i < tkns->size; i++)
        DeleteToken(tkns->tokens[i]);

    tkns->size = (size_t) INT_MAX;
    tkns->position = (size_t) INT_MAX;
    free(tkns->tokens);
    
    return NO_ERROR_LANG;
}

void SkipSpaces(Text* buf)
{
    while (isspace(buf->str[buf->position]))
        buf->position++;
}

LangError CreateTokens(Tokens* tkns, Text* buf)
{
    while (buf->str[buf->position] != '\0')
    {
        SkipSpaces(buf);
        
        if (buf->position == buf->size_buffer)
            return NO_ERROR_LANG;
        
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
    if (ptr == NULL) {return ALLOC_ERROR_LANG;}

    tkns->tokens = ptr;
    tkns->position = 0;

    return NO_ERROR_LANG;
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


LangError ParseFunction(Tokens* tkns, Text* buf)
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
                //MakeTokenWithName(tkns, buf, id, CreateSomething);
                buf->position++;
                
                MorseAlhabet morse_word[SIZE_SYMBOLS] = {};
                size_t i_vars = 0;
                size_t text_pos = buf->position - 1;
                ReadMorseCode(morse_word, &i_vars, buf);

                char name_fun[MAX_SIZE_NAME] = {};
                size_t size_name = 0;
                TranslateMorseCode(name_fun, morse_word, &size_name);

                size_t id_fun = 0;

                //MatchNamesTable(names, name_fun, &id_var);
                
                tkns->tokens[tkns->position] = CreateFunction(id_fun, name_fun, NULL, NULL);
                tkns->tokens[tkns->position]->text_pos = text_pos;

                (tkns->position)++;
                
                return NO_ERROR_LANG;
            }
            //else
            // return ERROR
        }
    }

    return NO_ERROR_LANG;
}

LangError ParseNumOrVar(Tokens* tkns, Text* buf)
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
            
            return NO_ERROR_LANG;
        }

        for (size_t i = 0; i < i_vars; i++)
            value = 10 * value + (double)(morse_word[i] - MORSE_0);
        //if ((morse_word[i] - MORSE_0) > 10) ERROR

        tkns->tokens[tkns->position] = CreateNumber(value, NULL, NULL);
        tkns->tokens[tkns->position]->text_pos = text_pos;
        tkns->position++;

        return NO_ERROR_LANG;
    }

    return NO_ERROR_LANG;
}

LangError ReadMorseCode(MorseAlhabet* id_value, size_t* i_vars, Text* buf)
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

    return NO_ERROR_LANG;
}

LangError MatchFuncNamesTable(Iterator* func_it, char* name_var, size_t* id_fun)
{
    bool match = false;

    for (size_t i_func = 0; i_func < func_it->size; i_func++)
    {
        if (strncmp(name_var, func_it->table_funcs[i_func].name, func_it->table_funcs[i_func].name_size) == 0)
        {
            match = true;
            (*id_fun) = func_it->table_funcs[i_func].id;
            return NO_ERROR_LANG;
        }
    }
    
    if (match == false)
    {
        size_t size_name = strlen(name_var);

        (*id_fun) = func_it->size;

        func_it->table_funcs[func_it->size].name_size = size_name;
        func_it->table_funcs[func_it->size].id = (*id_fun);

        func_it->table_funcs[func_it->size].type = FUNCTION;

        memcpy(func_it->table_funcs[func_it->size].name, name_var, size_name);
        //(func_it->size)++;
    }

    return NO_ERROR_LANG;
}

LangError MatchNamesTable(Iterator* func_it, char* name_var, size_t* id_var)
{
    bool match = false;

    Function* func = &(func_it->funcs[func_it->size]);

    for (size_t i_var = 0; i_var < func->num_names; i_var++)
    {
        if (strncmp(name_var, func->vars[i_var].name, func->vars[i_var].name_size) == 0)
        {
            match = true;
            (*id_var) = func->vars[i_var].id;
            return NO_ERROR_LANG;
        }
    }

    if (func_it->size == 0)
    {
        func_it->funcs[func_it->size].offset = 0;
    }
    if (func_it->size != 0)
    {
        func_it->funcs[func_it->size].offset = func_it->funcs[func_it->size - 1].num_names + func_it->funcs[func_it->size - 1].offset;
    }
        

    if (match == false)
    {
        size_t size_name = strlen(name_var);

        (*id_var) = func->num_names + func_it->funcs[func_it->size].offset;

        func->vars[func->num_names].id = (*id_var);
        func->vars[func->num_names].type = VARIABLE;
        func->vars[func->num_names].name_size = size_name;

        memcpy(func->vars[func->num_names].name, name_var, size_name);
        (func->num_names)++;
    }

    return NO_ERROR_LANG;
}

LangError TranslateMorseCode(char* name_var, MorseAlhabet* var, size_t* size_name)
{
    size_t i = 0;
    while (var[i] != 0)
    {
        name_var[i] = var[i];
        i++;
    }
    (*size_name) = i;

    return NO_ERROR_LANG;
}


LangError ParseLanguageOperators(Tokens* tkns, Text* buf)
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

    return NO_ERROR_LANG;
}

LangError ParseBoolOperators(Tokens* tkns, Text* buf)
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

    return NO_ERROR_LANG;
}

LangError ParseMathOperators(Tokens* tkns, Text* buf)
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

    return NO_ERROR_LANG;
}

Node* GetGrammar(Tokens* tkns, Iterator* func_it, LangError* error)
{
    while (tkns->tokens[tkns->position]->data.id_op == DEFINE)
    {
        (tkns->position)++;
        CreateNames(&(func_it->funcs[func_it->size].vars));

        func_it->funcs[func_it->size].tree.root = GetFunction(tkns, func_it, error);
        (func_it->size)++;
    }


    return func_it->funcs[func_it->size - 1].tree.root;
}

Node* GetOperators(Tokens* tkns, Iterator* func_it, LangError* error)
{
    Node* value_1 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if ((tkns->tokens[tkns->position]->data.id_op == OP_CONDITION) ||\
            (tkns->tokens[tkns->position]->data.id_op == OP_LOOP))
            value_1 = GetWhileOrIf(tkns, func_it, error);
        else if (tkns->tokens[tkns->position]->data.id_op == RET)
            value_1 = GetReturn(tkns, func_it, error);
        else if ((tkns->tokens[tkns->position]->data.id_op == INPUT) || (tkns->tokens[tkns->position]->data.id_op == OUTPUT))
        {
            value_1 = GetInOutput(tkns, func_it, error);
        }
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

Node* GetInOutput(Tokens* tkns, Iterator* func_it, LangError* error)
{
    Node* value_1 = NULL;

    if ((tkns->tokens[tkns->position]->data.id_op == INPUT) || (tkns->tokens[tkns->position]->data.id_op == OUTPUT))
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
    {
        tkns->position++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }
    
    value_1->left = GetVariable(tkns, func_it, error);

    if (tkns->tokens[tkns->position]->data.id_op == R_BRACKET)
    {
        tkns->position++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    Node* value_2 = NULL;

    if (tkns->tokens[tkns->position]->data.id_op == SEMICOLON)
    {
        value_2 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    value_2->left = value_1;

    return value_2;
}

Node* GetArgument(Tokens* tkns, Iterator* func_it, LangError* error)
{
    if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
    {
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
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
            value_2 = GetExpression(tkns, func_it, error);
        }
        else if (tkns->tokens[tkns->position]->type == OPERATOR)
        {
            if (tkns->tokens[tkns->position]->data.id_op != COMMA)
            {
                value_2 = GetExpression(tkns, func_it, error);
            }
        }
        
        if (tkns->tokens[tkns->position]->data.id_op == COMMA)
        {
            value_3 = tkns->tokens[tkns->position];
            (tkns->position)++;

            value_3->left = value_2;
            value_3->right = GetExpression(tkns, func_it, error);
            
            value_2 = value_3;
        }
        
    } while ((tkns->tokens[tkns->position]->data.id_op != R_BRACKET));

    (tkns->position)++;

    return value_2;
}

Node* GetFunction(Tokens* tkns, Iterator* func_it, LangError* error)
{
    Node* value_1 = NULL;

    if (tkns->tokens[tkns->position]->type == FUNCTION)
    {
        value_1 = tkns->tokens[tkns->position];
        size_t id = 0;
        MatchFuncNamesTable(func_it, value_1->data.name, &id);
        
        value_1->data.id_var = id;
        
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    value_1->left = GetArgument(tkns, func_it, error);

    if (tkns->tokens[tkns->position]->data.id_op == L_CURLY_BRACKET)
    {
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    value_1->right = GetOperators(tkns, func_it, error);

    if (tkns->tokens[tkns->position]->data.id_op == R_CURLY_BRACKET)
    {
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    if (tkns->tokens[tkns->position]->data.id_op == SEMICOLON)
    {
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    return value_1;
}

Node* GetReturn(Tokens* tkns, Iterator* func_it, LangError* error)
{
    Node* value_1 = NULL;

    if (tkns->tokens[tkns->position]->data.id_op == RET)
    {
        value_1 = tkns->tokens[tkns->position];
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    value_1->left = GetExpression(tkns, func_it, error); 

    if (tkns->tokens[tkns->position]->data.id_op == SEMICOLON)
    {
        (tkns->position)++;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    return value_1;
}

Node* GetWhileOrIf(Tokens* tkns, Iterator* func_it, LangError* error)
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
        (*error) = SYNTAX_ERROR;
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
            (*error) = SYNTAX_ERROR;
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
            (*error) = SYNTAX_ERROR;
            return NULL;
        }
    }
    else 
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    return value_2;
}

Node* GetAssign(Tokens* tkns, Iterator* func_it, LangError* error)
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
        (*error) = SYNTAX_ERROR;
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
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    return value_2;
}

Node* GetExpression(Tokens* tkns, Iterator* func_it, LangError* error)
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

Node* GetTerm(Tokens* tkns, Iterator* func_it, LangError* error)
{
    Node* value_1 = GetUnary(tkns, func_it, error);
    
    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        while ((tkns->tokens[tkns->position]->data.id_op == OP_MUL) || (tkns->tokens[tkns->position]->data.id_op == OP_DIV))
        {
            Node* value_3 = tkns->tokens[tkns->position];
            tkns->position++;
            
            Node* value_2 = GetUnary(tkns, func_it, error);
            value_3->left = value_1;
            value_3->right = value_2;
            
            value_1 = value_3;
        }
    }
    
    return value_1;
}

Node* GetUnary(Tokens* tkns, Iterator* func_it, LangError* error)
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
    {
        value_1 = GetPrimaryExpression(tkns, func_it, error);
    }
    return value_1;
}

Node* GetBoolingExpression(Tokens* tkns, Iterator* func_it, LangError* error)
{
    Node* value_1 = GetExpression(tkns, func_it, error);
    Node* value_3 = NULL;

    if (tkns->tokens[tkns->position]->type == OPERATOR)
    {
        if ((tkns->tokens[tkns->position]->data.id_op == OP_ABOVE) || (tkns->tokens[tkns->position]->data.id_op == OP_BELOW) || (tkns->tokens[tkns->position]->data.id_op == OP_EQUAL) || (tkns->tokens[tkns->position]->data.id_op == OP_NO_EQUAL))
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

Node* GetBoolPrimaryExpression(Tokens* tkns, Iterator* func_it, LangError* error)
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

Node* GetPrimaryExpression(Tokens* tkns, Iterator* func_it, LangError* error)
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

Node* GetVariable(Tokens* tkns, Iterator* func_it, LangError* error)
{
    if (tkns->tokens[tkns->position]->type == VARIABLE)
    {
        Node* var = tkns->tokens[tkns->position];
        (tkns->position)++;

        size_t id = 0;

        if (tkns->tokens[tkns->position]->data.id_op == L_BRACKET)
        {   
            var->type = FUNCTION;
            MatchFuncNamesTable(func_it, var->data.name, &id);
            var->data.id_fun = id;
        }
        else
        {
            MatchNamesTable(func_it, var->data.name, &id);
            var->data.id_var = id;
        }

        var->left = GetArgument(tkns, func_it, error);

        return var;
    }
    
    return GetNumber(tkns, error);
}

Node* GetNumber(Tokens* tkns, LangError* error)
{
    if (tkns->tokens[tkns->position]->type == NUMBER)
    {
        Node* num = tkns->tokens[tkns->position];
        tkns->position++;
        return num;
    }
    else
    {
        (*error) = SYNTAX_ERROR;
        return NULL;
    }

    return NULL;
}