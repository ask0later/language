#include "tree.h"

Tree* ConstructorTree()
{
    Tree* new_tree = (Tree*) calloc(1, sizeof(Tree));
    new_tree->root = NULL;
    new_tree->size = 0;
    new_tree->changes_num = 0;
    return new_tree;
}

Node* Copynator(Node* node)
{
    if (!node) return NULL;

    Node* new_node;

    if (node->type == NUMBER)
        new_node = CreateNumber(node->data.value, Copynator(node->left), Copynator(node->right));
    else if (node->type == OPERATOR)
        new_node = CreateOperator(node->data.id_op, Copynator(node->left), Copynator(node->right));
    else if (node->type == VARIABLE)
        new_node = CreateVariable(node->data.id_var, 0, Copynator(node->left), Copynator(node->right));
    else if (node->type == FUNCTION)
        new_node = CreateVariable(node->data.id_fun, 0, Copynator(node->left), Copynator(node->right));    

    return new_node;
}

void DestructorTree(Tree* tree)
{
    DeleteNode(tree->root);
    tree->size = INT_MAX;
    free(tree);
}

void DestructorTrees(Tree** trees, size_t num_trees)
{
    for (size_t i = 0; i < num_trees; i++)
    {
        DestructorTree(trees[i]);
    }
}

Node* CreateVariable(size_t id_var, char* name, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = VARIABLE;
    
    if (name != 0)
        memcpy(node->data.name, name, strlen(name));
    else
        node->data.id_var = id_var;

    return node;
}

Node* CreateFunction(size_t id_fun, char* name, Node* left, Node* right)
{
    Node* node = (Node*) calloc(1, sizeof(Node));
    if (!node) {return 0;}

    node->left   = left;
    node->right  = right;

    node->type = FUNCTION;

    if (name != 0)
        memcpy(node->data.name, name, strlen(name));
    else
        node->data.id_fun = id_fun;

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