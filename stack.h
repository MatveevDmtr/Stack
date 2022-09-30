#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "StackConfig.h"

// two headers: interface internal

typedef unsigned long long ULL;

typedef unsigned long long bird_t;

typedef struct stack_info
{
    bird_t             StructLeftCannary;
    elem_t*            Ptr;
    size_t             Size;
    size_t             Capacity;
    bool               DeadInside;
    ULL StackHashSum;
    ULL StructHashSum;

    struct  calling_inf
    {
        const char* Orig_name;
        const char* Func_calling;
        const char* File_calling;
        size_t      Line_created;
    } debug_info;

    bird_t StructRightCannary;
}
stack_t;

stack_t StructureStackInit(const char* name,
                           const char* func,
                           const char* file,
                           int line);

int LogError(int errcode);

int FuckingDump(stack_t* stk,
                const char* funcname,
                const char* filename,
                int line);

int StackPush(stack_t* stk, elem_t elem);

int StackCtor(stack_t* stk);

int StackDtor(stack_t* stk);

static int StackResize(stack_t* stk, size_t new_capacity);

int StackPush(stack_t* stk, elem_t elem);

elem_t StackPop(stack_t* stk);

ULL CalculateGNUHash(void* start_ptr, int num_bytes);

ULL StackHash  (stack_t* stk);

ULL StructHash (stack_t* stk);

int CheckLeftCannary  (stack_t* stk);

int CheckRightCannary (stack_t* stk);


#endif
