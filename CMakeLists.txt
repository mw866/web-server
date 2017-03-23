cmake_minimum_required(VERSION 3.6)
project(lisod)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_USE_RELATIVE_PATHS ON)


find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c)
FLEX_TARGET(MyScanner lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
#add_executable(LexYacc
#        ${BISON_MyParser_OUTPUTS}
#        ${FLEX_MyScanner_OUTPUTS})

#target_link_libraries(lisod ${FLEX_LIBRARIES} ${BISON_LIBRARIES})


set(SOURCE_FILES
        lisod.c
        log.c
        log.h
        parse.c
        parse.h
        process_request.c
        process_request.h
        lex.yy.c
        y.tab.c
        y.tab.h
        )

add_executable(lisod ${SOURCE_FILES} ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS})

