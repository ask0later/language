#include "dump.h"
#include "print.h" 

void DumpTokens(Tokens* tkns)
{
    for (size_t i = 0; i < tkns->size; i++)
    {
        printf("tokens[%2lu]: ", i);
        DumpToken(tkns->tokens[i]);
        printf("    (text_position =%lu)\n", tkns->tokens[i]->text_pos);
        printf("\n");
    }
}

void DumpToken(Node* current)
{
    if (current->type == OPERATOR)
    {
        printf("OPERATOR = ");
        PrintOperator(current->data.id_op, stdout);
    }
    else if (current->type == NUMBER)
    {
        printf("NUMBER   = ");
        printf(" %lg ", current->data.value);
    }
    else if (current->type == VARIABLE)
    {
        printf("VARIABLE = ");
        printf("<%s>;", current->data.name);
    }
    else if (current->type == FUNCTION)
    {
        printf("FUNCTION = ");
        printf("<%s>;", current->data.name);
    }
}

void DumpNamesTables(Iterator* func_it)
{
    for (size_t i_vars = 0; i_vars < func_it->size; i_vars++)
    {
        DumpNamesTable(func_it->funcs + i_vars);
        printf("\n");
    }
    for (size_t i_func = 0; i_func < func_it->size; i_func++)
    {
        printf("function[%lu] = %s; type = %d; id = %lu\n", i_func, func_it->table_funcs[i_func].name, func_it->table_funcs[i_func].type, func_it->table_funcs[i_func].id);
    }
}

void DumpNamesTable(Function* func)
{
    for (size_t i = 0; i < func->num_names; i++)
    {
        printf("name[%lu] = %s; type = %d; id = %lu\n", i, func->vars[i].name, func->vars[i].type, func->vars[i].id);
    }
}