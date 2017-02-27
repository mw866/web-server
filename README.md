## Response Header Format
### RFC2616 


       Response      = Status-Line               ; Section 6.1
                       *(( general-header        ; Section 4.5
                        | response-header        ; Section 6.2
                        | entity-header ) CRLF)  ; Section 7.1
                       CRLF
                       [ message-body ]          ; Section 7.2

### Example Response Header 


        HTTP/1.1 200 OK
        access-control-allow-origin: *
        cache-control: max-age=60
        content-security-policy: default-src 'self' blob: https://*.cnn.com:* http://*.cnn.com:* *.cnn.io:* *.cnn.net:* *.turner.com:* *.turner.io:* *.ugdturner.com:* *.vgtf.net:*; script-src 'unsafe-eval' 'unsafe-inline' 'self' *; style-src 'unsafe-inline' 'self' blob: *; child-src 'self' blob: *; frame-src 'self' *; object-src 'self' *; img-src 'self' data: blob: *; media-src 'self' blob: *; font-src 'self' data: *; connect-src 'self' *;
        Content-Type: text/html; charset=utf-8
        x-content-type-options: nosniff
        x-servedByHost: ::ffff:10.61.5.146
        x-xss-protection: 1; mode=block
        Via: 1.1 varnish
        Fastly-Debug-Digest: 1e206303e0672a50569b0c0a29903ca81f3ef5033de74682ce90ec9d13686981
        Content-Length: 131114
        Accept-Ranges: bytes
        Date: Mon, 27 Feb 2017 20:18:07 GMT
        Via: 1.1 varnish
        Age: 205
        Connection: keep-alive
        Set-Cookie: countryCode=US; Domain=.cnn.com; Path=/
        Set-Cookie: geoData=Ithaca|NY|14853|US|NA; Domain=.cnn.com; Path=/
        X-Served-By: cache-iad2142-IAD, cache-bos8233-BOS
        X-Cache: HIT, HIT
        X-Cache-Hits: 2, 5
        X-Timer: S1488226687.252677,VS0,VE0
        Vary: Accept-Encoding


## Instructions

* Create Docker container initially


    docker run -d -p 3022:22 -p 7777:7777 -p 9999:9999 -h server.cornell.edu   --security-opt seccomp:unconfined  --name server ebagdasa/cs5450_p1:latest 

* Start stopped Docker container:


    docker start [CONTAINER_NAME | CONTAINER_ID]

* Log into the Docker container:


    ssh -p 3022 root@127.0.0.1

## References

* FAQs: https://pages.github.coecis.cornell.edu/cs5450/website/assignments/p1/P1_FAQ.txt

* Tutorial of development environment: https://pages.github.coecis.cornell.edu/cs5450/website/assignments/p1/docker.html

* Explanation of software compilation: https://help.ubuntu.com/community/CompilingSoftware

* Annotated Excerpted RFC 2616 Text: https://pages.github.coecis.cornell.edu/cs5450/website/assignments/p1/rfc.txt

* CMU Liso Projects: 
** https://github.com/wshen0123/cmu-15641-lisod

** https://github.com/zhuansunxt/Liso

** https://github.com/mindbergh/Liso

** https://github.com/mydagobah/Lisod

* Makefile Tutorial: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/


## Troubleshooting

### Error: Cannot load symbol file

In case you’d like to debug Linux target from macOS host, you have to use GDB version compiled with --target=x86_64-linux-gnu flag. Get the binutils-gdb sources from the Git repository, switch branch to gdb-7.11.1-release and build like this:


    ./configure --disable-werror --target=x86_64-linux-gnu --prefix=/Users/kylin1989/Downloads/binutils-gdb-linux
    make
    make install
Then select this debugger in the remote GDB configuration.

https://blog.jetbrains.com/clion/2016/07/clion-2016-2-eap-remote-gdb-debug/

### Error: 'sbrk' has been explicitly marked deprecated here" when compiling binutils-gdb

Use argument when running ./configure :
        
        --disable-werror

http://stackoverflow.com/questions/20426213/error-building-ellcc


