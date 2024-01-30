cmake_minimum_required(VERSION 3.10)

project(clang_graphviz C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

set(PROJECT_NAME clang_graphviz)
set(SOURSE_NAME clang_graphviz.c)

set(SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(DIRECTORY_TEST1 ${TEST_DIRECTORY}/test1)
set(NAME_EXAMPLE1 example_1.c)
set(DIRECTORY_TEST2 ${TEST_DIRECTORY}/test2)
set(NAME_EXAMPLE2 example_2.c)

set(DOT_FILE_NAME call_graph.dot)

set(GRAPH_FILE_NAME call_graph.png)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

add_executable(${PROJECT_NAME} ${SOURCE_DIRECTORY}/${SOURSE_NAME})

target_link_directories(${PROJECT_NAME} PRIVATE ${LLVM_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LLVM_LIBRARIES} -lclang)

add_custom_target(test1
    COMMAND ${PROJECT_NAME} ${DIRECTORY_TEST1}/${NAME_EXAMPLE1}
)

add_custom_target(test2
    COMMAND ${PROJECT_NAME} ${DIRECTORY_TEST2}/${NAME_EXAMPLE2}
)

add_custom_target(graph
    COMMAND dot -Gsize=11,8.5 -Grankdir=LR -Tpng ${CMAKE_CURRENT_SOURCE_DIR}/${DOT_FILE_NAME} -o ${CMAKE_CURRENT_SOURCE_DIR}/${GRAPH_FILE_NAME}
  COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/${DOT_FILE_NAME}
  COMMAND eog ${CMAKE_CURRENT_SOURCE_DIR}/${GRAPH_FILE_NAME}
)

add_custom_target(clean_all
    COMMAND rm -f ${CMAKE_CURRENT_SOURCE_DIR}/${DOT_FILE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${GRAPH_FILE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}
	COMMAND rm -f ${CMAKE_CURRENT_SOURCE_DIR}/cmake_install.cmake ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCache.txt ${CMAKE_CURRENT_SOURCE_DIR}/Makefile 
	COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles
)
