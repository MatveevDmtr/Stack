#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <TXLib.h>

#include "logging.h"
#include "stack.h"
#include "StackConfig.h"
#include "macroses.h"
#include "GetPoison.h"

#define HASH 1
#define CANARY 1
#define VERIFIER 0

const int MIN_LEN_STACK = 10;

const bird_t LEFT_CANNARY  = 0xDEEDFEE1BAD;

const bird_t RIGHT_CANNARY = 0xDEEDFEE15AD;

const ULL START_HASH = 88005553535;

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
     DEBUGINFOERROR , // 16
     REALLOCERROR   , // 17
     STACKOVERFLOW  , // 18
     CONSTR_ERROR   , // 19
     DESTR_ERROR      // 20
};

int main()
{
    log("Log file opened\n");

    fprintf(stderr, "Startuem\n");

    stack_t stk1 = StructStackInit(stk1);

    StackDump(&stk1);

    log("Stk1 initialized\n");

    StackCtor(&stk1);

    //log_simple(LOG_FILE, "Stk1 created");

    for (size_t i = 0; i < 100; i++)
    {
        //StackPush(LOG_FILE, &stk1, i);

        if (i % 10 == 0)
        {
            //StackDump(&stk1, LOG_FILE);
        }
    }

    for (size_t i = 0; i < 100; i++)
    {
        //StackPop(LOG_FILE, &stk1);

        if (i % 10 == 0)
        {
            //StackDump(&stk1, LOG_FILE);
        }
    }

    //StackDump(&stk1, LOG_FILE);
}

stack_t StructureStackInit(const char* name,
                           const char* func,
                           const char* file,
                           int line)
{
    elem_t*            Ptr;
    size_t             Size;
    size_t             Capacity;

    stack_t temp_stk = {LEFT_CANNARY,
                        getPoison(Ptr),
                        getPoison(Size),
                        getPoison(Capacity),
                        1,
                        NULL,
                        NULL};

    temp_stk.debug_info.Orig_name    = name;
    temp_stk.debug_info.Func_calling = func;
    temp_stk.debug_info.File_calling = file;
    temp_stk.debug_info.Line_created = line;

    temp_stk.StructRightCannary  = RIGHT_CANNARY;

    return temp_stk;
}

int FindErrors(stack_t* stk)
{
    ULL sum_errcodes = 0;

    if (stk == NULL || stk == getPoison(stk))       sum_errcodes += 1 << SEGFAULT;
    if (stk->DeadInside)                            sum_errcodes += 1 << ZOMBIE;
    if (stk->Ptr <= NULL)                           sum_errcodes += 1 << NULLPTR;
    if (stk->Ptr == (elem_t*)getPoison(stk->Ptr))   sum_errcodes += 1 << PTRPOISONED;
    if (stk->Size < 0)                              sum_errcodes += 1 << NEGSIZE;
    if (stk->Size == getPoison(stk->Size))          sum_errcodes += 1 << SIZEPOISONED;
    if (stk->Capacity <= 0)                         sum_errcodes += 1 << NEGCAP;
    if (stk->Capacity == getPoison(stk->Capacity))  sum_errcodes += 1 << CAPPOISONED;
    if (stk->StructLeftCannary  != LEFT_CANNARY)    sum_errcodes += 1 << LBIRDSTRUCT;
    if (stk->StructRightCannary != RIGHT_CANNARY)   sum_errcodes += 1 << RBIRDSTRUCT;
    //if (!CheckRightCannary)                       sum_errcodes += 1 << RBIRDSTACK;
    if (stk->Size >= stk->Capacity)                 sum_errcodes += 1 << STACKOVERFLOW;

    ULL stack_hash_sum = CalculateGNUHash(stk->Ptr, stk->Capacity * sizeof(elem_t)); // add cannaries

    if (stack_hash_sum != stk->StackHashSum)
    {
        sum_errcodes += 1 << INVSTACKHASH;
    }

    stk->StackHashSum = stack_hash_sum;

    ULL struct_hash_sum      = 0;
    ULL prev_struct_hash_sum = stk->StructHashSum;

    stk->StructHashSum       = 0;
    // off rejime: hash, canary, verifier
    if ((struct_hash_sum = CalculateGNUHash(stk, sizeof(stack_t))) != prev_struct_hash_sum)
    {
        sum_errcodes += 1 << INVSTRUCTHASH;
    }

    stk->StructHashSum = struct_hash_sum;

    if (sum_errcodes == 0)
    {
         sum_errcodes = (1 << OK);
    }

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

int DecodeErrors(ULL sum_errcodes)
{
    size_t max_errcode = floor(log2 (sum_errcodes));

    for (size_t errcode = 0; errcode <= max_errcode; errcode++)
    {
        if (sum_errcodes & (1 << errcode))
        {
            LogError(errcode);
        }
    }

    return 0;
}

int LogError(int errcode)
{
    switch (errcode)
    {
        case OK:
            //log_res_of_check(log_file, "Everything OK: Stack is all right and can be used");
            break;

        case SEGFAULT:
            //log_res_of_check(log_file, "SEGMENTATION FAULT: Invalid Pointer to Structure of Stack");
            break;

        case ZOMBIE:
            //log_res_of_check(log_file, "DEADINSIDE ERROR: Stack doesn't exist");
            break;

        case NULLPTR:
            //log_res_of_check(log_file, "POINTER ERROR: Stack Pointer (pointer to buffer) is NULL");
            break;

        case PTRPOISONED:
            //log_res_of_check(log_file, "POINTER ERROR: Invalid Stack Pointer (pointer to buffer)");
            break;

        case NEGSIZE:
            //log_res_of_check(log_file, "SIZE ERROR: Stack Size has a Negative Value");
            break;

        case SIZEPOISONED:
            //log_res_of_check(log_file, "SIZE ERROR: Stack Size is poisoned");
            break;

        case NEGCAP:
            //log_res_of_check(log_file, "CAPACITY ERROR: Stack Capacity has a Negative Value");
            break;

        case CAPPOISONED:
            //log_res_of_check(log_file, "CAPACITY ERROR: Stack Capacity is poisoned");
            break;

        default:
            //log_res_of_check(log_file, "DECODE ERROR: Unexpected Error Code");
            return 1;
    }

    return 0;
}

int StackVerify(stack_t* stk)
{
    ULL sum_errcodes = FindErrors(stk);

    log("Sum_errcodes: %d\n", sum_errcodes);

    if (sum_errcodes < 1)
    {
        print_log(FATAL_ERROR, "SEARCH OF ERRORS ERROR: Invalid Sum of Error Codes");

        return -1;
    }

    DecodeErrors(sum_errcodes);

    return 0;
}

void DumpExit()
{
    log("\n---------- EMERGENCY FINISH DUMP ----------\n\n");
}

int FuckingDump(stack_t* stk,
                const char* funcname,
                const char* filename,
                int         line)
{
    log("\n\n++++++++++ Start Dump ++++++++++\n");

    if (stk == NULL)
    {
        LogError(SEGFAULT);

        DumpExit();

        return SEGFAULT;
    }

    if (funcname == NULL || filename == NULL || line == NULL)
    {
        log("STACKDUMP CALL ERROR: StackDump can't recognize parameters of call\n");
    }
    else
    {
        log("\n%s at %s (line %d):\n", funcname, filename, line);
    }

    log("Stack [0x%p] ", stk->Ptr);

    ULL sum_errcodes = StackVerify(stk);

    if (sum_errcodes == 1)
    {
        log("OK\n");
    }
    else
    {
        log("ERROR\n");

        DecodeErrors(sum_errcodes);
    }

    log("Originally '%s' from %s at %s (line %d): \n\n",
                       stk->debug_info.Orig_name,
                       stk->debug_info.Func_calling,
                       stk->debug_info.File_calling,
                       stk->debug_info.Line_created);

    log("Pointer:          %p\n"
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

    if (stk->Capacity <= 0 || stk->Capacity == getPoison(stk->Capacity))
    {
        LogError(NEGCAP);

        DumpExit();

        return CAPPOISONED;
    }

    if (stk->Ptr == NULL || stk->Ptr == getPoison(stk->Ptr))
    {
        log("POINTER ERROR: Bad Stack Pointer (pointer to buffer)");

        DumpExit();

        return PTRPOISONED;
    }

    log("{\n");

    int numbers_in_capacity = ceil(log10 ((double) stk->Capacity));

    for (size_t i = 0; i < stk->Capacity; i++)
    {
        log("\t data[%0*d] = %p = %d\n", numbers_in_capacity, i, (stk->Ptr)[i]);
    }

    log("}\n\n");

    log("\n---------- Finish Dump ----------\n\n");

    return 0;
};

ULL CalculateGNUHash(void* start_ptr, int num_bytes)
{
    ULL hash_sum = START_HASH;
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

/*int CheckLeftStructCannary(stack_t* stk)
{
    if (stk->StructLeftCannary == LEFT_CANNARY)
        return true;
    return false;
}

int CheckRightStructCannary(stack_t* stk)
{
    return true;
}
*/

/*ULL StackHash(stack_t* stk)
{
    return CalculateGNUHash(stk->Ptr, stk->Size * sizeof(elem_t));
}

ULL StructHash(stack_t* stk)
{
    return CalculateGNUHash(stk, sizeof(stk));
}*/

static int StackResize(stack_t* stk, size_t new_capacity)
{
    StackVerify(stk);

    char* temp_ptr = (char*) realloc(stk->Ptr, new_capacity * sizeof(elem_t) + 2 * sizeof(ULL)); // Canary type

    if (temp_ptr == NULL)
    {
        print_log(FATAL_ERROR, "REALLOCATION ERROR: Can't Find Such Amount of Dynamic Memory");

        return REALLOCERROR;
    }

    bird_t* ptr_left_bird  = (bird_t*) temp_ptr;

    stk->Ptr            = (elem_t*)(temp_ptr + sizeof(bird_t));

    bird_t* ptr_right_bird = (bird_t*)(temp_ptr + sizeof(bird_t) + new_capacity * sizeof(elem_t));

    if (new_capacity > stk->Capacity)
    {
        for (size_t i = stk->Size; i < new_capacity; i++)
        {
            stk->Ptr[i] = getPoison(stk->Ptr[0]);
        }
    }

    stk->Capacity = new_capacity;

    UpdateHash(stk);
}

/*static int StackDecrease(FILE* log_file, stack_t* stk)
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
}*/

// #define DEBUG
// #if DEBUG
//#ifndef DEBUG

int StackCtor(stack_t* stk)
{
    if (stk == NULL || stk == getPoison(stk))
    {
        LogError(SEGFAULT);

        return SEGFAULT;
    }

    if (!(stk->DeadInside))
    {
        print_log(FATAL_ERROR, "CONSTRUCTION ERROR: Trying to Reconstruct an Existing Stack");

        return CONSTR_ERROR;
    }

    char* temp_ptr = (char*) calloc(MIN_LEN_STACK * sizeof(elem_t) + 2 * sizeof(bird_t), sizeof(char));

    if (temp_ptr == NULL)
    {
        print_log(FATAL_ERROR, "REALLOCATION ERROR: Can't Find Such Amount of Dynamic Memory");

        return REALLOCERROR;
    }

    bird_t* ptr_left_bird  = (bird_t*) temp_ptr;

    stk->Ptr            = (elem_t*)(temp_ptr + sizeof(bird_t));

    bird_t* ptr_right_bird = (bird_t*)(temp_ptr + sizeof(bird_t) + MIN_LEN_STACK * sizeof(elem_t));

    stk->Ptr = (elem_t*) temp_ptr;

    *ptr_left_bird = LEFT_CANNARY;

    stk->DeadInside       = 0;
    stk->Size             = 0;
    stk->Capacity         = MIN_LEN_STACK;

    *ptr_right_bird = LEFT_CANNARY;

    UpdateHash(stk);
}

int StackDtor(stack_t* stk)
{
    if (stk->DeadInside)
    {
        print_log(FATAL_ERROR, "DESTRUCTION ERROR: Trying to Destruct a Dead Stack");

        return DESTR_ERROR;
    }

    elem_t poison_for_buf = getPoison(stk->Ptr[0]);

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

    free(stk->Ptr);

    UpdateHash(stk);
}

int StackPush(stack_t* stk, elem_t elem)
{
    #if VERIFIER
        StackVerify(stk);
    #endif

    if (stk->Size >= stk->Capacity)
    {
        StackResize(stk, stk->Capacity * 2);
    }
    stk->Ptr[stk->Size] = elem;

    stk->Size++;

    UpdateHash(stk);

    return 0;
}

elem_t StackPop(stack_t* stk)
{   // verify
    elem_t elem = stk->Ptr[stk->Size - 1];

    stk->Ptr[stk->Size - 1] = getPoison(stk->Ptr[0]);

    stk->Size--;

    if (stk->Size >= MIN_LEN_STACK && stk->Size <= stk->Capacity / 4)
    {
        StackResize(stk, stk->Capacity / 2);
    }

    UpdateHash(stk);

    return elem;
}
