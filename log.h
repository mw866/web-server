#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

FILE *open_logfile(const char *path);

void Log(FILE* logfile, char *message);

