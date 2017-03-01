#include "process_request.h"

//Process Request from the client and send the client an appropriate response

char* STATUS_200 = "200_OK\n";
char* STATUS_204 = "204_NO_CONTENT\n";  
char* STATUS_404 = "404_NOT_FOUND\n";
char* STATUS_411 = "411_LENGTH_REQUIRED\n";
char* STATUS_500 = "500_INTERNAL_SERVER_ERROR\n";
char* STATUS_501 = "501_NOT_IMPLEMENTED\n";
char* STATUS_503 = "503_SERVICE_UNAVAILABLE\n";
char* STATUS_505 = "505_HTTP_VERSION_NOT_SUPPORTED\n";

//function to process request and edied the response
 void processRequest(char* buf, int nbytes, char* response) {
  Request *request = parse(buf,nbytes);
  //calling respond_HTTP_ver function
  // respond_HTTP_ver(request->http_version, response);
  // strcat(response, " ");
  if (!strcmp(request->http_version , "HTTP/1.1")) {
    strcpy(response, request->http_version);
    strcat(response, " ");

    if (!strcmp(request->http_method, "HEAD")  || !strcmp(request->http_method, "GET")) {
      respond_get_or_head(request, response);
    } 
    else if (!strcmp(request->http_method, "POST")) {
      respond_post(request, response);        
    }
    else {
      strcat(response, STATUS_501);  
    }
  }
  else {
    strcpy(response, request->http_version);
    strcat(response, " ");
    strcat(response, STATUS_505);
  }
}


//find the file extension, code modified from (http://stackoverflow.com/questions/5309471/getting-file-extension-in-c)
char *file_ext(const char *filename) {
  char *period = strrchr(filename, '.');
  if(!period || period == filename){
    return "";  
  }     
  return period + 1;
}

//Response when HTTP request was either head or get
void respond_get_or_head(Request * request, char * response) {

  char header[4096];
  char body[1000000];

  //check if the file exists
  if(access(request->http_uri, F_OK ) != -1 ) {
    strcpy(header, STATUS_200);
    strcat(header, "Server: select_server/1.0\n");
    strcat(header, "Connection: keep-alive\r\n");

    //read from file 
    int fd_in = open(request->http_uri, O_RDONLY);
    char file_buf[1000000];
    strcpy(file_buf, " ");
    if(fd_in < 0) {
      printf("Error 501: Failed to open the file\n"); 
      strcat(response, STATUS_501);
      exit(-1);                   
    }
    //read from the file
    int content_length = read(fd_in,file_buf,sizeof(file_buf));
    printf("File Buf is!! : \n%s\n", file_buf);
    printf("************\n");

    char content_length_str[100];
    sprintf(content_length_str, "%d", content_length);
    strcat(header, "Content-length: ");
    strcat(header, content_length_str);
    strcat(header,"\r\n");

    //if its head, read from the file buffer
    if(!strcmp(request->http_method, "GET")){
      strcpy(body, file_buf);
      // free(file_buf);
    }

    char ext[1024];

    //check if the file extension is html, image or text
    if (!strcmp(file_ext(request->http_uri), "html")) {
      strcpy(ext, "text/html");
    }
    else if (!strcmp(file_ext(request->http_uri), "png")) {
      strcpy(ext, "image/png");
    }
    else if (!strcmp(file_ext(request->http_uri), "css")) {
      strcpy(ext, "text/css");
    }
    else if (!strcmp(file_ext(request->http_uri), "jpg")) {
      strcpy(ext, "image/jpeg");
    }
    else if (!strcmp(file_ext(request->http_uri), "txt")) {
      strcpy(ext, "text/plain");
    }
    else if (!strcmp(file_ext(request->http_uri), "gif")) {
      strcpy(ext, "image/gif");
    }
    else { 
      strcpy(ext, "application/octet-stream");
    }

    strcat(header, "Content-Type: ");
    strcat(header, ext);
    strcat(header,"\r\n");

    // Get Date, Time
    char get_date_time[500];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(get_date_time, sizeof get_date_time, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    strcat(header, "Date: ");
    strcat(header, get_date_time);
    strcat(header, "\r\n");

    // Get the latest time (code modified from: http://stackoverflow.com/questions/10446526/get-last-modified-time-of-file-in-linux)
    struct stat attr;
    stat(request->http_uri, &attr);
    strcat(header, "Last-Modified: ");
    strcat(header, ctime(&attr.st_mtime));
    strcat(header, "\r\n");

  }
  else{
    strcat(header, STATUS_404);
    printf("File was not found");
  }

  strcat(header, "\r\n");
  strcat(response, header);

  if(!strcmp(request->http_method, "GET")){
    strcat(response, body);
  }
}

//response when the http request is POST
void respond_post(Request * request, char * response)  {
  if(access(request->http_uri, F_OK ) != -1 ) 
    strcat(response, STATUS_200);    
  else
    strcat(response, STATUS_204);
}