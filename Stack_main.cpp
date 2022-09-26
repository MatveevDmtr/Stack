#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "StackConfig.h"
#include "macroses.h"
#include "stack.h"
#include "GetPoison.h"

const int MIN_LEN_STACK = 10;

const unsigned long long START_HASH = 88005553535;

enum ERRCODES
{
     OK             , // 0
     SEGFAULT       , // 1
     ZOMBIE         , // 2
     NULLPTR        , // 3
     PTRPOISONED    , // 4
     NEGSIZE        , // 5
     SIZEPOISONED   , // 6
     NEGCAP         , // 7
     CAPPOISONED    , // 8
     LBIRDSTACK     , // 9
     RBIRDSTACK     , // 10
     LBIRDSTRUCT    , // 11
     RBIRDSTRUCT    , // 12
     INVSTACKHASH   , // 13
     INVSTRUCTHASH  , // 14
     FILEERROR      , // 15
     DEBUGINFOERROR   // 16
};

int main()
{
    FILE* log_file = open_log();

    log(log_file, "Log file opened");

    stack_t stk1 = StructStackInit();

    stk1.debug_info.Orig_name    = "stk1";
    stk1.debug_info.Func_calling = __PRETTY_FUNCTION__;
    stk1.debug_info.File_calling = __FILE__;
    stk1.debug_info.Line_created = __LINE__;

    StackDump(&stk1, log_file);

    log_simple(log_file, "Stk1 initialized");

    StackCtor(&stk1);

    log_simple(log_file, "Stk1 created");

    for (size_t i = 0; i < 100; i++)
    {
        StackPush(log_file, &stk1, i);

        if (i % 10 == 0)
        {
            StackDump(&stk1, log_file);
        }
    }

    for (size_t i = 0; i < 100; i++)
    {
        StackPop(log_file, &stk1);

        if (i % 10 == 0)
        {
            StackDump(&stk1, log_file);
        }
    }

    StackDump(&stk1, log_file);

    close_log(log_file);
}

stack_t StructStackInit()
{
    elem_t*            Ptr;
    size_t             Size;
    size_t             Capacity;
    return {getPoison(Ptr),
            getPoison(Size),
            getPoison(Capacity),
            1,
            NULL,
            NULL};
}

int FindErrors(stack_t* stk)
{
    unsigned long long sum_errcodes = 0;

    if (stk == NULL || stk == getPoison(stk))       sum_errcodes += 1 << SEGFAULT;
    if (stk->DeadInside)                            sum_errcodes += 1 << ZOMBIE;
    if (stk->Ptr <= NULL)                           sum_errcodes += 1 << NULLPTR;
    if (stk->Ptr == (elem_t*)getPoison(stk->Ptr))   sum_errcodes += 1 << PTRPOISONED;
    if (stk->Size < 0)                              sum_errcodes += 1 << NEGSIZE;
    if (stk->Size == getPoison(stk->Size))          sum_errcodes += 1 << SIZEPOISONED;
    if (stk->Capacity <= 0)                         sum_errcodes += 1 << NEGCAP;
    if (stk->Capacity == getPoison(stk->Capacity))  sum_errcodes += 1 << CAPPOISONED;
    if (!CheckLeftCannary(stk))                     sum_errcodes += 1 << LBIRDSTACK;
    if (!CheckRightCannary)                         sum_errcodes += 1 << RBIRDSTACK;

    unsigned long long stack_hash_sum = CalculateGNUHash(stk->Ptr, stk->Capacity * sizeof(elem_t));

    if (stack_hash_sum != stk->StackHashSum)
    {
        sum_errcodes += 1 << INVSTACKHASH;
    }

    stk->StackHashSum = stack_hash_sum;

    unsigned long long struct_hash_sum      = 0;
    unsigned long long prev_struct_hash_sum = stk->StructHashSum;

    stk->StructHashSum                      = 0;

    if ((struct_hash_sum = CalculateGNUHash(stk, sizeof(stack_t))) != prev_struct_hash_sum)
    {
        sum_errcodes += 1 << INVSTRUCTHASH;
    }

    stk->StructHashSum = struct_hash_sum;

    if (sum_errcodes == 0)
         sum_errcodes++;

    return sum_errcodes;
}

int CheckFile(FILE* log_file)
{
    if (log_file == NULL)
    {
        fprintf(stderr, "FILE ERROR: Invalid log file\n");

        return FILEERROR;
    }

    return 0;
}

int DecodeErrors(FILE* log_file, unsigned long long sum_errcodes)
{
    if (CheckFile(log_file))
        return FILEERROR;

    size_t max_errcode = floor(log2 (sum_errcodes));

    for (size_t errcode = 0; errcode <= max_errcode; errcode++)
    {
        log_int(log_file, "Errcode", errcode);
        log_int(log_file, "Bit Move of Errcode", 1 << errcode);
        log_int(log_file, "Result of Bit Comparison", sum_errcodes & (1 << errcode));

        if (sum_errcodes & (1 << errcode))
        {
            log(log_file, "Try to Log Error");

            LogError(log_file, errcode);
        }
    }

    return 0;
}

int LogError(FILE* log_file, int errcode)
{
    if (CheckFile(log_file))
        return FILEERROR;

    log_int(log_file, "errcode", errcode);

    switch (errcode)
    {
        case OK:
            log_res_of_check(log_file, "Everything OK: Stack is all right and can be used");
            break;

        case SEGFAULT:
            log_res_of_check(log_file, "SEGMENTATION FAULT: Invalid Pointer to Structure of Stack");
            break;

        case ZOMBIE:
            log_res_of_check(log_file, "DEADINSIDE ERROR: Stack doesn't exist");
            break;

        case NULLPTR:
            log_res_of_check(log_file, "POINTER ERROR: Stack Pointer (pointer to buffer) is NULL");
            break;

        case PTRPOISONED:
            log_res_of_check(log_file, "POINTER ERROR: Invalid Stack Pointer (pointer to buffer)");
            break;

        case NEGSIZE:
            log_res_of_check(log_file, "SIZE ERROR: Stack Size has a Negative Value");
            break;

        case SIZEPOISONED:
            log_res_of_check(log_file, "SIZE ERROR: Stack Size is poisoned");
            break;

        case NEGCAP:
            log_res_of_check(log_file, "CAPACITY ERROR: Stack Capacity has a Negative Value");
            break;

        case CAPPOISONED:
            log_res_of_check(log_file, "CAPACITY ERROR: Stack Capacity is poisoned");
            break;

        default:
            log_res_of_check(log_file, "DECODE ERROR: Unexpected Error Code");
            return 1;
    }

    return 0;
}

int StackVerify(FILE* log_file, stack_t* stk)
{
    unsigned long long sum_errcodes = FindErrors(stk);

    log_int(log_file, "Sum_errcodes", sum_errcodes);

    if (sum_errcodes < 1) //<0
    {
        log(log_file, "SEARCH OF ERRORS ERROR: Invalid Sum of Error Codes");

        return -1;
    }

    DecodeErrors(log_file, sum_errcodes);

    return 0;
}

int FuckingDump(stack_t* stk,
                FILE* log_file,
                const char* funcname,
                const char* filename,
                int   line)
{
    if (CheckFile(log_file))
        return FILEERROR;

    fprintf(log_file, "\n\n++++++++++ Start Dump ++++++++++\n");

    fflush(log_file);

    if (stk == NULL)
    {
        LogError(log_file, SEGFAULT);

        fprintf(log_file, "\n---------- EMERGENCY FINISH DUMP ----------\n\n");

        fflush(log_file);

        return SEGFAULT;
    }

    if (funcname == NULL || filename == NULL || line == NULL)
    {
        log(log_file, "DEBUG INFO ERROR: Structure of Stack has invalid debug info");

        fprintf(log_file, "\n---------- EMERGENCY FINISH DUMP ----------\n\n");

        fflush(log_file);

        return DEBUGINFOERROR;
    }

    fprintf(log_file, "\n%s at %s (line %d):\n",
                       funcname,
                       filename,
                       line);

    fflush(log_file);

    fprintf(log_file, "Stack [0x%p] ", stk->Ptr);

    fflush(log_file);

    int err_code = 0;

    if (!err_code) //verify_stack
    {
        fprintf(log_file, "OK\n");
    }
    else
    {
        fprintf(log_file, "ERROR\n");
    }

    fflush(log_file);

    fprintf(log_file, "Originally '%s' from %s at %s (line %d): \n\n",
                       stk->debug_info.Orig_name,
                       stk->debug_info.Func_calling,
                       stk->debug_info.File_calling,
                       stk->debug_info.Line_created);

    fflush(log_file);

    fprintf(log_file, "Pointer:          %p\n"
                      "Size:             %d\n"
                      "Capacity:         %d\n"
                      "DeadInside:       %d\n"
                      "StackHashSum:     %d\n"
                      "StructHashSum:    %d\n\n",
                      stk->Ptr,
                      stk->Size,
                      stk->Capacity,
                      stk->DeadInside,
                      stk->StackHashSum,
                      stk->StructHashSum);

    fflush(log_file);

    if (stk->Capacity <= 0 || stk->Capacity == getPoison(stk->Capacity))
    {
        LogError(log_file, NEGCAP);

        fprintf(log_file, "\n---------- EMERGENCY FINISH DUMP ----------\n\n");

        fflush(log_file);

        return CAPPOISONED;
    }

    if (stk->Ptr == NULL || stk->Ptr == getPoison(stk->Ptr))
    {
        log(log_file, "POINTER ERROR: Bad Stack Pointer (pointer to buffer)");

        fprintf(log_file, "\n---------- EMERGENCY FINISH DUMP ----------\n\n");

        fflush(log_file);

        return PTRPOISONED;
    }

    fprintf(log_file, "{\n");

    fflush(log_file);

    int numbers_in_capacity = ceil(log10 ((double) stk->Capacity));

    for (size_t i = 0; i < stk->Capacity; i++)
    {
        fprintf(log_file, "\t data[%0*d] = %p = %d\n", numbers_in_capacity, i, (stk->Ptr)[i]);

        fflush(log_file);
    }

    fprintf(log_file, "}\n\n");

    fflush(log_file);

    fprintf(log_file, "\n---------- Finish Dump ----------\n\n");

    fflush(log_file);

    return 0;
};

unsigned long long CalculateGNUHash(void* start_ptr, int num_bytes)
{
    unsigned long long hash_sum = START_HASH;
    for (size_t i = 0; i < num_bytes; i++)
    {
        hash_sum = hash_sum * 33 + ((char*)start_ptr)[i];
    }

    return hash_sum;
}

int UpdateHash(stack_t* stk)
{
    stk->StackHashSum     = CalculateGNUHash(stk->Ptr, stk->Capacity * sizeof(elem_t));
    stk->StructHashSum    = 0;
    stk->StructHashSum    = CalculateGNUHash(stk, sizeof(stack_t));
}

int CheckLeftCannary(stack_t* stk)
{
    return true;
}

int CheckRightCannary(stack_t* stk)
{
    return true;
}

/*unsigned long long StackHash(stack_t* stk)
{
    return CalculateGNUHash(stk->Ptr, stk->Size * sizeof(elem_t));
}

unsigned long long StructHash(stack_t* stk)
{
    return CalculateGNUHash(stk, sizeof(stk));
}*/

static int StackIncrease(FILE* log_file, stack_t* stk)
{
    if (CheckFile(log_file))
        return FILEERROR;

    elem_t* temp_ptr = (elem_t*)realloc(stk->Ptr, stk->Capacity * 2 * sizeof(elem_t));

    if (temp_ptr == NULL)
    {
        log_res_of_check(log_file, "REALLOCATION ERROR: Can't Find Such Amount of Dynamic Memory");
    }

    stk->Ptr = temp_ptr;

    stk->Capacity = stk->Capacity * 2;

    for (size_t i = stk->Size; i < stk->Capacity; i++)
    {
        stk->Ptr[i] = getPoison(stk->Ptr[0]);
    }

    Assert(stk->Ptr == NULL);

    UpdateHash(stk);
}

static int StackDecrease(FILE* log_file, stack_t* stk)
{
    if (CheckFile(log_file))
        return FILEERROR;

    elem_t* temp_ptr = (elem_t*)realloc(stk->Ptr, stk->Capacity * sizeof(elem_t) / 2);

    if (temp_ptr == NULL) //include to LogError
    {
        log_res_of_check(log_file, "REALLOCATION ERROR: Can't Find Such Amount of Dynamic Memory");

        return -1;
    }

    stk->Ptr = temp_ptr;

    stk->Capacity = stk->Capacity / 2;

    UpdateHash(stk);
}

int StackCtor(stack_t* stk)
{
    if (stk == NULL || stk == getPoison(stk))
    {
        LogError(log_file, SEGFAULT);

        return SEGFAULT;
    }

    if (!(stk->DeadInside)) // include to LogError
    {
        log_res_of_check(log_file, "CONSTRUCTION ERROR: Trying to Reconstruct an Existing Stack");

        return -1;
    }

    elem_t* temp_ptr = (elem_t*) calloc(MIN_LEN_STACK, sizeof(elem_t)); //include to LogError

    if (temp_ptr == NULL)
    {
        log_res_of_check(log_file, "REALLOCATION ERROR: Can't Find Such Amount of Dynamic Memory");

        return -1;
    }

    stk->Ptr = temp_ptr;

    stk->DeadInside       = 0;
    stk->Size             = 0;
    stk->Capacity         = MIN_LEN_STACK;

    UpdateHash(stk);

    return 0;
}

int StackDtor(stack_t* stk)
{
    elem_t poison_for_buf = getPoison(stk->Ptr[0]); // Destroy the existing Stack check

    for (size_t i = 0; i < stk->Capacity; i++)
    {
        (stk->Ptr)[i] = poison_for_buf;
    }

    stk->Ptr           = getPoison(stk->Ptr);
    stk->Size          = getPoison(stk->Size);
    stk->Capacity      = getPoison(stk->Capacity);
    stk->DeadInside    = 1;
    stk->StackHashSum  = CalculateGNUHash(stk->Ptr, stk->Size * sizeof(elem_t));
    stk->StructHashSum = 0;
    stk->StructHashSum = CalculateGNUHash(stk, sizeof(stk));

    UpdateHash(stk);
}

int StackPush(FILE* log_file, stack_t* stk, elem_t elem)
{
    if (CheckFile(log_file))
        return FILEERROR;

    StackVerify(log_file, stk);

    if (stk->Size >= stk->Capacity)
    {
        StackIncrease(log_file, stk);
    }
    stk->Ptr[stk->Size] = elem;

    stk->Size++;

    UpdateHash(stk);

    return 0;
}

elem_t StackPop(FILE* log_file, stack_t* stk)
{
    if (CheckFile(log_file))
        return FILEERROR;

    elem_t elem = stk->Ptr[stk->Size - 1];

    stk->Ptr[stk->Size - 1] = getPoison(stk->Ptr[0]);

    stk->Size--;

    if (stk->Size >= MIN_LEN_STACK && stk->Size <= stk->Capacity / 4)
    {
        StackDecrease(log_file, stk);
    }

    UpdateHash(stk);

    return elem;
}
