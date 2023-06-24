cmake_minimum_required(VERSION 3.26)

project(RPN_COMPLEX_C)

set(CMAKE_C_STANDARD 11)

# Инклуд директория для всех модулей
include_directories(include)

# Для тестов, библиотека Criterion
include_directories(criterion/include)
link_directories(criterion/lib)

# Модули
add_library(node src/node.c)
add_library(list src/list.c)
add_library(stack src/stack.c)
add_library(RPN src/RPN.c)
add_library(utils src/utils.c)

target_link_libraries(list node)
target_link_libraries(stack node)

target_link_libraries(RPN utils)

# Основная программа
add_executable(main src/main.c)
target_link_libraries(main list stack node -lm)

# Тесты
# Список
add_executable(list_test src/test/list_test.c)
target_link_libraries(list_test list node -lcriterion)
# Узел
add_executable(node_test src/test/node_test.c)
target_link_libraries(node_test node -lcriterion)