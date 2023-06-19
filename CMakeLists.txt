cmake_minimum_required(VERSION 3.26)

project(RPN_COMPLEX_C)

set(CMAKE_C_STANDARD 11)

include_directories(head)

add_library(node node.c)
add_library(list list.c)
add_library(selector selector.c)
add_library(stack stack.c)

target_link_libraries(list node)
target_link_libraries(stack node)
target_link_libraries(selector node)
add_executable(main RPNc.c)
target_link_libraries(main list stack selector node -lm)