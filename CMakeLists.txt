cmake_minimum_required(VERSION 3.17)
project(da_proj)

set(GCC_FLAGS_OPTIMIZE "-Ofast -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops")
set(GCC_FLAGS_PARANOID "-pthread -g -O -Wall -pedantic -Wunused-result -pedantic-errors -Wextra -Wcast-align -Wcast-qual -Wchar-subscripts -Wcomment -Wconversion -Wdisabled-optimization \
    -Wfloat-equal  -Wformat  -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport  -Winit-self  -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wmissing-braces \
    -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wpacked -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type \
    -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs  -Wuninitialized \
    -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  -Wvariadic-macros \
    -Wvolatile-register-var  -Wwrite-strings") # -Weffc++ -Wpadded

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("lib/GraphViewerCpp")
include_directories("lib/GraphViewerCpp/include")

include_directories("include" "algorithms/include" "structures/include" "utils/include" "ui/include")
file(GLOB_RECURSE SRC "algorithms/src/*.cpp" "structures/src/*.cpp" "utils/src/*.cpp")
file(GLOB_RECURSE SRC_UI "ui/src/*.cpp")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Detected GNU GCC, adding some compilation flags for da_proj algorithms")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS_OPTIMIZE}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS_PARANOID}")
endif ()

# ################ MAIN ################
add_executable(da_proj ${SRC} ${SRC_UI} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} graphviewer)

# ################ TESTS ################
add_subdirectory("test/Catch2")
include_directories("test/Catch2/src")
add_executable(tests test/test.cpp ${SRC})
target_link_libraries(tests PRIVATE Catch2WithMain)
include(CTest)
include(test/Catch2/extras/Catch.cmake)
catch_discover_tests(tests)

# ################ EVAL ################
add_executable(eval ${SRC} "src/eval.cpp")
