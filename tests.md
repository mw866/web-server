## Testing Methods
The server is tested with different sample request using the following command
`nc localhost 9034 < test_request.txt`

Below are the test scenarios.

Request `Method` | Request `Path` | Response `Status-Code` | Response `Content-Type`
---|---|---|---
GET, HEAD | \ | 200 OK | text/html
GET, HEAD | \non_existent_file | 404 NOT FOUND | N/A
GET, HEAD | \index.html | 200 OK | text/html
GET, HEAD | \logo.jpg | 200 OK | image/jpeg
POST      | \non_existent_file | 204 NO CONTENT | N/A
POST      | \index.html | 200 OK  | text/html
UNEXISTENT_METHOD| \ | 501 NOT IMPLEMENTED  | N/A

## Vulnerability

* Additional non-standard  request header fails parser
Currently, the parser only takes in up to 7 fields in the request header. In the case of additional non-standard headers, the parser errors will be thrown.

* Partial response will 'hang' the socket.
Currently, the timeout mechanism is not implemented to handle partial requests. DoS attacks can exploit this by taking finite sockets.

* Buffer Overflow
Currently, standard library functions like `strcpy()` and `strcat()` are frequently used, which are not aware of the size of the string or buffer. This may cause accidental read from or write outside the intended memory space.

* Insufficient Memory
Currently, the memory sizes for various buffers are fixed. If the content is larger than the allocated buffer size, the exceeded content will be dropped.
