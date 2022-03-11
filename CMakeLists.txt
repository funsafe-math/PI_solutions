cmake_minimum_required(VERSION 3.20.0)
project(programowanie_imperatywne VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
endif()


# Rescan for new tests and executables every compile
add_custom_target(dummy ALL)
add_custom_command(
    TARGET dummy
    PRE_BUILD
    COMMAND "${CMAKE_COMMAND}" "${CMAKE_BINARY_DIR}"
)

# Search for solution files
FILE(GLOB SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.c*)
foreach(src ${SRC_FILES})
    get_filename_component(FILENAME_NO_EXTENSION ${src} NAME_WLE )
    add_executable(${FILENAME_NO_EXTENSION} ${src})
endforeach()


add_library(tester tester/tester.cpp)
include_directories(.)
include_directories(template)

# Tester files
# include(CTest)
enable_testing()

FILE(GLOB TESTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp*)
foreach(src ${TESTS})
    get_filename_component(FILENAME_NO_EXTENSION  ${src} NAME_WLE )
    add_executable(${FILENAME_NO_EXTENSION} ${src})
    target_link_libraries(${FILENAME_NO_EXTENSION} tester)
    add_test(${FILENAME_NO_EXTENSION} ${FILENAME_NO_EXTENSION})
endforeach()



# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
