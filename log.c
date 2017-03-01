#include "log.h"
/*
 * This write log to the logfile 
 */

/*
 *  Open logfile
 */
FILE *open_logfile(const char *path)
{
    FILE *logfile;

    logfile = fopen(path, "w");
    if ( logfile == NULL )
    {
        printf("Error opening logfile. \n");
        exit(EXIT_FAILURE);
    }
    // set logfile to line buffering
    setvbuf(logfile, NULL, _IOLBF, 0);
    return logfile;
}

/*
 *  Write Log to logfile
 */
void Log(FILE* logfile, char *message)
{
    time_t ltime = time(NULL);
    struct tm* time = localtime(&ltime);

    // fprintf(logfile, "%s: %s",time, message);
    fprintf(logfile, "[%04d-%02d-%02d %02d:%02d:%02d]: %s",
       time->tm_year+1900,
       time->tm_mon+1,
       time->tm_mday,
       time->tm_hour,
       time->tm_min,
       time->tm_sec,
       message
       );
    printf("%s", message);
}

/*
 *  unit test 
 */
// void main()
// {
//     FILE* logfile = open_logfile("./logfile.log");
//     Log(logfile, "passed unit test: Log()");
//     fclose(logfile);
//     exit(0);
// }
