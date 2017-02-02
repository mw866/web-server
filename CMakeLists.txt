cmake_minimum_required(VERSION 3.6)
project(web_server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        echo_client.c
        echo_server.c
        Makefile
        README)

add_executable(web_server ${SOURCE_FILES})