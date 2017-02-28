/*
** selectserver.c -- a cheezy multiperson chat server
*/

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


#define PORT "9034"   // port we're listening on
 char* STATUS_200 = "200_OK\n";
 char* STATUS_404 = "404_NOT_FOUND\n";
 char* STATUS_411 = "411_LENGTH_REQUIRED\n";
 char* STATUS_500 = "500_INTERNAL_SERVER_ERROR\n";
 char* STATUS_501 = "501_NOT_IMPLEMENTED\n";
 char* STATUS_503 = "503_SERVICE_UNAVAILABLE\n";
 char* STATUS_505 = "505_HTTP_VERSION_NOT_SUPPORTED\n";

 void processRequest(char*, int, char* );
 void respond_HTTP_ver(char* , char* );

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[256];    // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    //int i, j, rv;
    int i, rv;

    struct addrinfo hints, *ai, *p;

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

	// get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
      fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
      exit(1);
  }

  for(p = ai; p != NULL; p = p->ai_next) {
     listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
     if (listener < 0) { 
       continue;
   }

		// lose the pesky "address already in use" error message
   setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

   if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
       close(listener);
       continue;
   }

   break;
}

	// if we got here, it means we didn't get bound
if (p == NULL) {
  fprintf(stderr, "selectserver: failed to bind\n");
  exit(2);
}

	freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    // main loop
    for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                      (struct sockaddr *)&remoteaddr,
                      &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the max
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                    }
                } else {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        // we got some data from a client
                        //Parse the buffer to the parse function. You will need to pass the socket fd and the buffer would need to
                        //be read from that fd
                        printf("%s", buf);
                        printf("%d", nbytes);

                        char* response = malloc(4096);
                        processRequest(buf, nbytes,response);

                        printf("%s\n", response); 

                        strcat(response, "\r\n");

                        printf("size of response: %d\n", strlen(response));
    
                        if (send(i, response, strlen(response), 0) == -1) {
                            printf("Error sending");
                        }else{
                            printf("SENT RESPONSE! :D");
                            // printf(response);
                        }
                         free(response);
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
    return 0;
}

void append(char*s, size_t size, char c) {
 int len = strlen(s);
 s[len] = c;
 s[len+1] = '\0';
}

void processRequest(char* buf, int nbytes, char* response) {
    Request *request = parse(buf,nbytes);
    //Just printing everything
    printf("Http Method %s\n",request->http_method);
    printf("Http Version %s\n",request->http_version);
    printf("Http Uri %s\n",request->http_uri);
    for(int index = 0;index < request->header_count;index++){
        printf("Request Header\n");
        printf("Header name %s Header Value %s\n",request->headers[index].header_name,request->headers[index].header_value);
    }
     // strcpy(response, "");
    // memset(response, 0, sizeof(response));

    // char *header = malloc(4096);
    // //put a space infront of the header
    // strcpy(header, " ");
    respond_HTTP_ver(request->http_version, response);
    strcat(response, " ");

    if (!strcmp(request->http_method, "HEAD")  || !strcmp(request->http_method, "GET")) {
        respond_get_or_head(request, response);
        printf(response);
    }


//DELETEEEE THIS 
    // char path[1024];
    // if (!strcmp(request->http_method, "HEAD")  || !strcmp(request->http_method, "GET") ) {

    //     if (getcwd(path, sizeof(path)) != NULL) {
    //         fprintf(stdout, "Current working dir: %s\n", path);


    //         strcat(path,request->http_uri);
    //         fprintf(stdout, "Full path is: %s\n", path);

    //         int result = access(path, F_OK); // F_OK tests existence also (R_OK,W_OK,X_OK).
    //                                           //            for readable, writeable, executable
    //         if ( result == 0 ) {
    //            printf("%s exists!!\n",path);
    //            strcat(response, STATUS_200);
    //            strcat(response, "\r\n");
    //            }
    //         else  {
    //                printf("ERROR: %s doesn't exist!\n",path); 
    //                strcat(response, STATUS_404);
    //                strcat(response, "\r\n");
    //            }

    //        // printf("Printing METHID %s\n",request->http_method);
    //        if (!strcmp(request->http_method, "GET")){
    //              printf("line 227");  

    //             int fd_in = open(path, O_RDONLY);
    //             char file_buf[8192];
    //             if(fd_in < 0) {
    //                 printf("Error 501: Failed to open the file\n"); 
    //                 strcat(response, STATUS_501);                    
    //             }
    //             int content_length = read(fd_in,file_buf,8192);
    //             strcat(response, file_buf);
    //             //append(response, strlen(response), '\r\n');
    //             //strcat(response, "\r\n");
    //             printf("%s\n",response); 
    //        }

    //     }
    //     else {
    //             perror("getcwd() error");
    //         }
    // }

//DELETE ABOVE
    
    // strcpy(response, file_buf);
    // append(response, strlen(response), '\n');
}


void respond_HTTP_ver(char* http_ver, char* response){
    strcpy(response, http_ver);
    if (strcmp(http_ver , "HTTP/1.1"))
        strcpy(response, STATUS_505);
} 

const char *file_ext(const char *filename) {
    const char *period = strrchr(filename, '.');
    if(!period || period == filename){
      return "";  
    }     
    return period + 1;
}

void respond_get_or_head(Request * request, char * response) {

  char header[4096];
  char path[1024];

  if (getcwd(path, sizeof(path)) != NULL) {
      fprintf(stdout, "Current working dir: %s\n", path);
      strcat(path,request->http_uri);
      fprintf(stdout, "Full path is: %s\n", path);

      int result = access(path, F_OK);
      // if(access(path, F_OK ) != -1 ) {
      if (result != -1) {
          printf("I am here!! ");
          strcpy(header, STATUS_200);

          strcat(header, "Server: select_server/1.0\n");

          strcat(header, "Connection: keep-alive\r\n");

          // Get file length
          FILE * fp = fopen(request->http_uri, "rb");
          int prev=ftell(fp);
          fseek(fp, 0L, SEEK_END);

          int sz=ftell(fp);
          fseek(fp,prev,SEEK_SET);
          char length[200];
          sprintf(length, "%d", sz);
          strcat(header, "Content-length: ");
          strcat(header, length);
          strcat(header,"\n");

          // Content type
          char ext[1024];

          
         //TODO: USE SWITCH
        if (file_ext(request->http_uri) == "html") {
            strcpy(ext, "text/html");
        }
        else if (file_ext(request->http_uri) == "css") {
            strcpy(ext, "text/css");
        }
        else if (file_ext(request->http_uri) == "txt") {
            strcpy(ext, "text/plain");
        }
        else { // default
            strcpy(ext, "application/octet-stream");
        }

          strcat(header, "Content-Type: ");
          strcat(header, ext);
          strcat(header,"\n");

          // Get Date
          char get_time[500];
          time_t now = time(0);
          struct tm tm = *gmtime(&now);
          strftime(get_time, sizeof get_time, "%a, %d %b %Y %H:%M:%S %Z", &tm);
          strcat(header, "Date: ");
          strcat(header, get_time);
          strcat(header, "\n");

          // Get Last date modified of file
          struct stat attr;
          stat(request->http_uri, &attr);
          char buf_LM[500];
          // NEED TO FORMAT THIS STRING LIKE ABOVE ONE
          // sprintf(buf_LM, "%a, %d %b %Y %H:%M:%S %Z", ctime(&attr.st_mtime));
          strcat(header, "Last-Modified: ");
          strcat(header, ctime(&attr.st_mtime));
          strcat(header, "\n");

      } else { // File doesn't exist: Respond with an error
          printf("FILE DOES NOT EXIST");
      }
    }
    else{
        strcat(header, STATUS_500);
        printf("Internal ERROR");
    }

  strcat(header, "\r\n");
  strcat(response, header);

}








