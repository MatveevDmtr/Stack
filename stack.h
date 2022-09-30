#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "StackConfig.h"

// two headers: interface internal

typedef unsigned long long UnsignedLL;

typedef unsigned long long bird_t;

typedef struct stack_info
{
    bird_t             StructLeftCannary;
    elem_t*            Ptr;
    size_t             Size;
    size_t             Capacity;
    bool               DeadInside;
    bird_t*            PtrStackLeftBird;
    bird_t*            PtrStackRightBird;
    UnsignedLL                StackHashSum;
    UnsignedLL                StructHashSum;

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

UnsignedLL FindErrors(stack_t* stk);

int DecodeErrors(UnsignedLL sum_errcodes);

int LogCritError(int errcode, const char* func, int line);

UnsignedLL StackVerify(stack_t* stk);

void DumpEmExit();

int FuckingDump(stack_t* stk,
                const char* funcname,
                const char* filename,
                int line);

int StackCtor(stack_t* stk);

int StackDtor(stack_t* stk);

static int StackResize(stack_t* stk, size_t new_capacity);

int StackPush(stack_t* stk, elem_t elem);

elem_t StackPop(stack_t* stk);

UnsignedLL CalculateGNUHash(void* start_ptr, size_t num_bytes);

static int UpdateHash(stack_t* stk);

int CheckLeftCannary  (stack_t* stk);

int CheckRightCannary (stack_t* stk);


#endif
