#include "readFile.h"



ReaderError CreateBuffer(Text* buf, const char* input_file)
{
    buf->position = 0;
    
    buf->size_buffer = GetSizeFile(input_file);
    
    
    buf->str = (char*) calloc(buf->size_buffer, sizeof(char));
    if (buf->str == NULL) {return ReaderErrorALLOC_ERROR;}

    ReadFile(buf, input_file);
    

    return ReaderErrorNO_ERROR;
}

void DeleteBuffer(Text* buf)
{
    buf->size_buffer = INT_MAX;
    buf->position    = INT_MAX;

    free(buf->str);
}


void ReadFile(Text* buf, const char* input_file)
{
    FILE* fname = fopen(input_file, "r");

    fread(buf->str, sizeof(char), buf->size_buffer, fname);
    printf("<%s>", buf->str);
    
    fclose(fname);
}


size_t GetSizeFile(const char* input_file)
{
    struct stat    buff;
    stat(input_file, &buff);
    
    return (size_t) (buff.st_size + 1);
}