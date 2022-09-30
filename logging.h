#ifndef LOG_NAME

    #define LOG_NAME "log.txt"

#endif


#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

#include <stdarg.h>
#include <stdlib.h>

#include "macroses.h"

enum LOG_MODES
{
    SIMPLE           = 0,
    FATAL_ERROR      = 1,
    N_LINE           = 2,
    FUNC_N_LINE      = 3,
    FILE_FUNC_N_LINE = 4
};

/*#define log_with_path(text_file, action)                       \
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
} */

#define print_log(mode, text)                                     \
{                                                                 \
    switch (mode)                                                 \
    {                                                             \
        case SIMPLE:                                              \
            fprintf_log(mode, text);                              \
            break;                                                \
                                                                  \
        case FATAL_ERROR:                                          \
            fprintf_log(mode, text, __PRETTY_FUNCTION__);         \
            break;                                                \
                                                                  \
        case N_LINE:                                               \
            fprintf_log(mode, text, __LINE__);                    \
            break;                                                \
                                                                  \
        case FUNC_N_LINE:                                           \
            fprintf_log(mode, text, __PRETTY_FUNCTION__,          \
                                  __LINE__);                      \
            break;                                                \
                                                                  \
        case FILE_FUNC_N_LINE:                                       \
            fprintf_log(mode, text, __FILE__,                     \
                                  __PRETTY_FUNCTION__,            \
                                  __LINE__);                      \
            break;                                                \
                                                                  \
        default:                                                  \
            fprintf_log(FATAL_ERROR,                               \
                      "LOGERROR: Invalid Mode of logging\n",      \
                      __PRETTY_FUNCTION__);                       \
    }                                                             \
}

//start prototypes
FILE* open_log();

void close_log();

int fprintf_log(size_t mode, char* text, ...);

void PrintFatalError(char* func, char* text);

void log(const char* format, ...);
//end prototypes


#endif
