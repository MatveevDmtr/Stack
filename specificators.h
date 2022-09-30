#include "StackConfig.h"

//start prototypes
const char* specificator1 (int /* value*/);

const char* specificator2 (int /* value*/);
//finish prototypes


const char* specificator1 (int /* value*/)
{
    return "%d";
}

const char* specificator2 (int /* value*/)
{
    return "%p";
}
