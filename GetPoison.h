#ifndef GETPOISON_H_INCLUDED
#define GETPOISON_H_INCLUDED

#include "logging.h"
#include "StackConfig.h"


const size_t BAAD_FOOD = 0xBAADF00D;


//start prototypes
stack_t* getPoison (stack_t* /* value */);

int getPoison (int /* value */);

size_t getPoison (size_t /* value */);

float getPoison (float /* value */);

double getPoison (double /* value */);

char getPoison (char /* value */);

char* getPoison (char* /* value */);

int* getPoison (int* /* value */);

double* getPoison (double* /* value */);

char** getPoison (char** /* value */);

void* getPoison (void* /* value */);

unsigned long long* getPoison (unsigned long long* /* value */);

unsigned long long getPoison (unsigned long long /* value */);
//finish prototypes


// const bad food

stack_t* getPoison (stack_t* /* value */)
{
    return (stack_t*) BAAD_FOOD;
}

int getPoison (int /* value */)
{
    return (int) BAAD_FOOD;
}

size_t getPoison (size_t /* value */)
{
    return (size_t) BAAD_FOOD;
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
    return (char) 1;
}

char* getPoison (char* /* value */)
{
    return (char*)BAAD_FOOD;
}

int* getPoison (int* /* value */)
{
    return (int*)BAAD_FOOD;
}

double* getPoison (double* /* value */)
{
    return (double*)BAAD_FOOD;
}

char** getPoison (char** /* value */)
{
    return (char**)BAAD_FOOD;
}

void* getPoison (void* /* value */)
{
    return (void*)BAAD_FOOD;
}

unsigned long long* getPoison (unsigned long long* /* value */)
{
    return (unsigned long long*)BAAD_FOOD;
}

unsigned long long getPoison (unsigned long long /* value */)
{
    return (unsigned long long)BAAD_FOOD;
}

#endif
