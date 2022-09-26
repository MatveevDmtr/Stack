#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "StackConfig.h"

typedef struct stack_info
{
    elem_t*            Ptr;
    size_t             Size;
    size_t             Capacity;
    bool               DeadInside;
    unsigned long long StackHashSum;
    unsigned long long StructHashSum;

    struct  calling_inf
    {
        const char* Orig_name;
        const char* Func_calling;
        const char* File_calling;
        int         Line_created;    // size_t
    } debug_info;
}
stack_t;

int close_log(FILE* file);

FILE* open_log();

stack_t StructStackInit();

int LogError(FILE* log_file, int errcode);

int FuckingDump(stack_t* stk,
              FILE* log_file,
              const char* funcname,
              const char* filename,
              int line);

int StackPush(FILE* log_file, stack_t* stk, elem_t elem);

int StackCtor(stack_t* stk);

int StackDtor(stack_t* stk);

static int StackIncrease(FILE* log_file, stack_t* stk);

static int StackDecrease(FILE* log_file, stack_t* stk);

int StackPush(FILE* log_file, stack_t* stk, elem_t elem);

elem_t StackPop(FILE* log_file, stack_t* stk);

unsigned long long CalculateGNUHash(void* start_ptr, int num_bytes);

unsigned long long StackHash  (stack_t* stk);

unsigned long long StructHash (stack_t* stk);

int CheckLeftCannary  (stack_t* stk);

int CheckRightCannary (stack_t* stk);


#endif
