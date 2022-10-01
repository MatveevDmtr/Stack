#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define Assert(condition)                                  \
{                                                          \
    if(condition)                                          \
    {                                                      \
        printf("ERROR in line %d in \n%s \nFound (%s).\n", \
                __LINE__, __FILE__, #condition);           \
    }                                                      \
}


#endif
