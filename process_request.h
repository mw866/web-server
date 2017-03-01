#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parse.h"
#include <time.h>
#include "log.h"

void processRequest(char* ,int , char* );
void respond_get_or_head(Request* , char*);
void respond_post(Request*, char*);
