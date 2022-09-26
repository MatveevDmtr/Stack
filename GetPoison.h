#ifndef GETPOISON_H_INCLUDED
#define GETPOISON_H_INCLUDED


stack_t* getPoison (stack_t* /* value */)
{
    return (stack_t*) 0xBAADF00D;
}

int getPoison (int /* value */)
{
    return 0xBAADF00D;
}

size_t getPoison (size_t /* value */)
{
    return 0xBAADF00D;
}

float getPoison (float /* value */)
{
    return NAN;
}

double getPoison (double /* value */)
{
    return NAN;
}

char getPoison (char /* value */)
{
    return 'z' + 1;
}

char* getPoison (char* /* value */)
{
    return (char*)0xBAADF00D;
}

int* getPoison (int* /* value */)
{
    return (int*)0xBAADF00D;
}

double* getPoison (double* /* value */)
{
    return (double*)0xBAADF00D;
}

char** getPoison (char** /* value */)
{
    return (char**)0xBAADF00D;
}

#endif
