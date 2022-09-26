#include <stdio.h>
#include <math.h>
#include <string.h>

#if !defined (LOG_NAME)

    #define LOG_NAME "log.txt"

#endif

#define Assert(condition)                                  \
{                                                          \
    if(condition)                                          \
    {                                                      \
        printf("ERROR in line %d in \n%s \nFound (%s).\n", \
                __LINE__, __FILE__, #condition);           \
    }                                                      \
}

#define log_with_path(text_file, action)                       \
{                                                              \
    fprintf(text_file, "\nIn file %s: \n"                      \
                       "\tin %s: \n"                           \
                       "\t\tline %d: %s\n\n",                  \
                        __FILE__,                              \
                        __PRETTY_FUNCTION__,                   \
                        __LINE__,                              \
                        #action);                              \
                                                               \
    fflush(text_file);                                         \
}

#define log(text_file, action)                             \
{                                                          \
                                                           \
    fprintf(text_file, "\nIn %s: \n"                       \
                       "\tline %d: %s\n\n",                \
                        __PRETTY_FUNCTION__,               \
                        __LINE__,                          \
                        #action);                          \
                                                           \
    fflush(text_file);                                     \
}

#define log_simple(text_file, action)                      \
{                                                          \
    fprintf(text_file, "\nline %d: %s\n\n",                \
                        __LINE__,                          \
                        #action);                          \
                                                           \
    fflush(text_file);                                     \
}

#define log_int(text_file, var, val)                       \
{                                                          \
    fprintf(text_file, "\tline %d: counted %s = %d\n\n",   \
                        __LINE__,                          \
                        #var, val);                        \
                                                           \
    fflush(text_file);                                     \
}

#define log_res_of_check(text_file, error)                 \
{                                                          \
    int len_text = strlen(#error);                         \
                                                           \
    fprintf(text_file, "\nIn %s: \n", __PRETTY_FUNCTION__);\
                                                           \
    for (size_t i = 0; i < len_text + 4; i++)              \
    {                                                      \
        fputc('-', log_file);                              \
    }                                                      \
                                                           \
    fputc('\n', log_file);                                 \
                                                           \
    fprintf(text_file, "| %s |\n", #error);                \
                                                           \
    for (size_t i = 0; i < len_text + 4; i++)              \
    {                                                      \
        fputc('-', log_file);                              \
    }                                                      \
                                                           \
    fputc('\n', log_file);                                 \
                                                           \
    fflush(text_file);                                     \
}

#define StackDump(stk, log_file);                          \
        FuckingDump(stk, log_file,                         \
                  __PRETTY_FUNCTION__,                     \
                  __FILE__,                                \
                  __LINE__);

/*#define StructStackInit()                                  \
({                                                         \
    elem_t*            Ptr;                                \
    size_t             Size;                               \
    size_t             Capacity;                           \
    {getPoison(Ptr),                                       \
     getPoison(Size),                                      \
     getPoison(Capacity),                                  \
     1,                                                    \
     NULL,                                                 \
     NULL};                                                \
})*/
