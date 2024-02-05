#include "read.h"
#include "operators.h"

TreeError CreateTree(Text* buf, Tree** array_tree, size_t* num_trees)
{
    size_t i = 0;

    while (buf->position != buf->size_buffer - 1)
    {
        array_tree[i] = ConstructorTree();

        ReadNode(buf, array_tree[i], &(array_tree[i]->root), PRE_ORDER);
        i++;
    }

    (*num_trees) = i;

    return NO_ERROR_TREE;
}

TreeError ReadNode(Text* buf, Tree* tree, Node** node, Order order_value)
{
    TreeError error = NO_ERROR_TREE;
    
    SkipSpaces(buf);

    if (buf->str[buf->position] == '(')
    {
        *node = CreateNode((Type) 0, 0, NULL, NULL);

        tree->size++;
        (buf->position)++;

        char source[MAX_SIZE_NAME];
        SkipSpaces(buf);
        ReadObject(source, buf);
        PasteObject(source, node);

        error =  ReadNode(buf, tree, &((*node)->left), order_value);
        if (error != NO_ERROR_TREE)
            return error;

        error = ReadNode(buf, tree, &((*node)->right), order_value);
        if (error != NO_ERROR_TREE)
            return error;

        
        SkipSpaces(buf);

        if (buf->str[buf->position] == ')')
            (buf->position)++;
    }
    else if (buf->str[buf->position] == ')')
    {
        return NO_ERROR_TREE;
    }

    return NO_ERROR_TREE;
}

TreeError SkipSpaces(Text* buf)
{
    while (isspace(buf->str[buf->position]))
        (buf->position)++;

    return NO_ERROR_TREE;
}
    

TreeError PasteObject(char* source, Node** node)
{
    double value = 0;

    for (size_t i = 0; i < NUM_OPERATORS; i++)
    {
        if (strncmp(source, operators[i].name, operators[i].size_name) == 0)
        {
            (*node)->type = OPERATOR;
            (*node)->data.id_op  = operators[i].id;
            return NO_ERROR_TREE;
        }
    }

    if (sscanf(source, "%lg", &value) == 1)
    {
        (*node)->type  = NUMBER;
        (*node)->data.value = value;
    }
    else if (strncmp(source, "func_", SIZE_FUNC_) == 0)
    {
        (*node)->type = FUNCTION;
        
        size_t i = SIZE_FUNC_;
        int id = 0;
        while (source[i] != '\0')
        { 
            id = id * 10 + source[i] - '0';
            i++;
        }
        (*node)->data.id_fun = (size_t) id;
    }
    else if (strncmp(source, "var_", SIZE_VAR_) == 0)
    {
        (*node)->type = VARIABLE;

        size_t i = SIZE_VAR_;
        int id = 0;
        while (source[i] != '\0')
        { 
            id = id * 10 + source[i] - '0';
            i++;
        }
        (*node)->data.id_fun = (size_t) id;
    }

    return NO_ERROR_TREE;
}


TreeError ReadObject(char* source, Text* buf)
{
    size_t i = 0;

    while ((buf->str[buf->position] != ')') && (buf->str[buf->position] != '('))
    {
        source[i] = buf->str[buf->position];
        (buf->position)++;
        i++;
    }

    source[i] = '\0';

    return NO_ERROR_TREE;
}