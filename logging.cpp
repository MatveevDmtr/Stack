#include "macroses.h"

FILE* open_log()
{
    FILE* log_file = fopen(LOG_NAME, "w");

    if (log_file == NULL)
    {
        fprintf(stderr, "ERROR: log file not found while opening\n");

        return NULL;
    }

    fprintf(log_file, "\n\n\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"
                      "Start program\n");

    return log_file;
}

int close_log(FILE* log_file)
{
    fprintf(log_file, "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"
                      "Finish program\n");

    fclose(log_file);

    /*if (log_file == NULL)
    {
        fprintf(stderr, "ERROR: log file not found while closing\n");
    }*/

    return 0;
}
