cmake_minimum_required(VERSION 3.10)

project(clang_graphviz C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -O0")

set(PROJECT_NAME clang_graphviz)

set(SOURCE_DIRECTORY ../src)
set(TEST_DIRECTORY ../tests)

set(DOT_DIRECTORY ../output_graphviz)
set(DOT_FILE_NAME call_graph.dot)

set(GRAPH_DIDECTORY ../graph_image)
set(GRAPH_FILE_NAME call_graph.png)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(${PROJECT_NAME} ${SOURCE_DIRECTORY}/clang_graphviz.c)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LLVM_LIBRARIES} -lclang)

add_custom_target(test1
    COMMAND ${PROJECT_NAME} ${TEST_DIRECTORY}/test1/example_1.c
	COMMAND clang -cc1 -ast-dump ${TEST_DIRECTORY}/test1/example_1.c > ${TEST_DIRECTORY}/test1/ast_dump_example_1.txt
)

add_custom_target(test2
    COMMAND ${PROJECT_NAME} ${TEST_DIRECTORY}/test2/example_2.c
	COMMAND clang -cc1 -ast-dump ${TEST_DIRECTORY}/test2/example_2.c > ${TEST_DIRECTORY}/test2/ast_dump_example_2.txt
)

add_custom_target(graph
    COMMAND dot -Gsize=11,8.5 -Grankdir=LR -Tpng ${DOT_DIRECTORY}/${DOT_FILE_NAME} -o ${GRAPH_DIDECTORY}/${GRAPH_FILE_NAME}
	COMMAND cat ${DOT_DIRECTORY}/${DOT_FILE_NAME}
	COMMAND eog ${GRAPH_DIDECTORY}/${GRAPH_FILE_NAME}
)

add_custom_target(clean_all
    COMMAND rm -f ${DOT_DIRECTORY}/${DOT_FILE_NAME} ${GRAPH_DIDECTORY}/${GRAPH_FILE_NAME}
	COMMAND rm -f ${TEST_DIRECTORY}/test1/ast_dump_example_1.txt ${TEST_DIRECTORY}/test2/ast_dump_example_2.txt ${TEST_DIRECTORY}/test3/ast_dump_example_3.txt
	COMMAND rm -rf ./*
)
