cmake_minimum_required(VERSION 3.23)
project(Formal_Languges)

set(CMAKE_CXX_STANDARD 17)
SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-coverage -lgcov")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(MAIN_SOURCE main.cpp)
set(MAIN_SOURCE_LIB src/Includes.h
        src/NFA.cpp src/NFA.h
        src/NFA_from_regex.cpp src/NFA_from_regex.h
        src/NFA_from_regex_builder.cpp src/NFA_from_regex_builder.h
        src/FDFA.cpp src/FDFA.h
        src/Task_3_Solver.cpp src/Task_3_Solver.h)

add_library(lib STATIC ${MAIN_SOURCE_LIB})

add_executable(main ${MAIN_SOURCE})
target_link_libraries(main lib)


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_SOURCE main_test.cpp)

add_executable(main_test ${TEST_SOURCE})
target_link_libraries(main_test ${GTEST_LIBRARIES})
target_link_libraries(main_test lib)

enable_testing()

add_test(TEST ${PROJECT_SOURCE_DIR}/bin/main_test)