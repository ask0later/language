#include "read_file.h"

int CtorBuffer(Text* buf, const char* input_file, err_allocator* err_alloc)
{
    buf->position = 0;

    buf->size_buffer = GetSizeFile(input_file, err_alloc);
    
    if (buf->size_buffer == 0)
    {
        INSERT_ERROR_NODE(err_alloc, "error getting size of file");
        err_alloc->need_call = true;
        return 1;
    }
    
    buf->str = (char*) calloc(buf->size_buffer, sizeof(char));
    if (buf->str == NULL)
    {
        INSERT_ERROR_NODE(err_alloc, "dynamic allocation is fault");
        err_alloc->need_call = true;
        return 1;
    }

    if (ReadFile(buf, input_file, err_alloc) == 1)
    {
        INSERT_ERROR_NODE(err_alloc, "error reading file");
        err_alloc->need_call = true;
        return 1;       
    }
    
    buf->str[buf->size_buffer - 1] = '\0';

    return 0;
}

void DtorBuffer(Text* buf)
{
    buf->size_buffer = INT_MAX;
    buf->position    = INT_MAX;

    free(buf->str);
}


int ReadFile(Text* buf, const char* input_file, err_allocator* err_alloc)
{
    FILE* fname = fopen(input_file, "r");
    
    if (!fname)
    {
        INSERT_ERROR_NODE(err_alloc, "file failed to open");
        err_alloc->need_call = true;
        return 1;
    }

    if (fread(buf->str, sizeof(char), buf->size_buffer, fname) == 0)
    {
        INSERT_ERROR_NODE(err_alloc, "no characters were counted");
        err_alloc->need_call = true;
        return 1;
    }

    return 0;
}


size_t GetSizeFile(const char* input_file, err_allocator* err_alloc)
{
    struct stat buff;

    if (stat(input_file, &buff) == 1)
    {
        INSERT_ERROR_NODE(err_alloc, "unsuccessful reading");
        return 0;
    }
    
    return (size_t) (buff.st_size + 1);
}