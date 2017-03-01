## Instructions
* Server: `./select_server`

    
* Client: `nc localhost 9034 < test_request.txt`


## References
* Beej's Guide to Network Programming: https://beej.us/guide/bgnet/

* IETF RFC 2616: https://www.ietf.org/rfc/rfc2616.txt

* FAQs: https://pages.github.coecis.cornell.edu/cs5450/website/assignments/p1/P1_FAQ.txt

* Tutorial of development environment: https://pages.github.coecis.cornell.edu/cs5450/website/assignments/p1/docker.html

* Annotated Excerpted RFC 2616 Text: https://pages.github.coecis.cornell.edu/cs5450/website/assignments/p1/rfc.txt

* CMU Liso Projects: 
	* https://github.com/wshen0123/cmu-15641-lisod
	
	* https://github.com/zhuansunxt/Liso
	
	* https://github.com/mindbergh/Liso
	
	* https://github.com/mydagobah/Lisod

* Makefile Tutorial: http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/


## Troubleshooting

### Error: Cannot load symbol file

In case you’d like to debug Linux target from macOS host, you have to use GDB version compiled with `--target=x86_64-linux-gnu` flag. Get the binutils-gdb sources from the Git repository, switch branch to gdb-7.11.1-release and build like this:


    ./configure --disable-werror --target=x86_64-linux-gnu --prefix=/Users/kylin1989/Downloads/binutils-gdb-linux
    make
    make install
Then select this debugger in the remote GDB configuration.

https://blog.jetbrains.com/clion/2016/07/clion-2016-2-eap-remote-gdb-debug/

### Error: 'sbrk' has been explicitly marked deprecated here" when compiling binutils-gdb

Use argument when running ./configure :
        
        --disable-werror

http://stackoverflow.com/questions/20426213/error-building-ellcc

### Error: 'Segmentation Errror'

Add 2 lines at the end of the reuqests.



