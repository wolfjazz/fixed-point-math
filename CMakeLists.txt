cmake_minimum_required(VERSION 3.25)

set(This FixedPointMath)
set(ThisShortLib FpmQ)
set(ThisShort FpmQPg)
project(${This} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE MinSizeRel)

enable_testing()

add_subdirectory(googletest)
add_subdirectory(test)

set(Headers
    src/fpm.hpp
    src/fpm/fpm.hpp
    src/fpm/q.hpp
    src/fpm/sq.hpp
)

set(Sources
    src/fpm.cpp
)

# GCC options
add_compile_options(-Wall -Wextra -pedantic -fconcepts-diagnostics-depth=2)

# Clang options
#add_compile_options(-Wall -Wextra -pedantic)

add_library(${ThisShortLib} STATIC ${Headers})
set_target_properties(${ThisShortLib} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(${ThisShortLib} PRIVATE -g -Og)  # optimize for debug

add_executable(${ThisShort} ${Headers} ${Sources})
set_target_properties(${ThisShort} PROPERTIES LINKER_LANGUAGE CXX)
target_link_options(${ThisShort} PUBLIC LINKER:-Map=${ThisShort}.map)
target_compile_options(${ThisShort} PRIVATE -Os)  # optimize for size

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
