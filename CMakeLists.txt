cmake_minimum_required(VERSION 3.25)

set(This FixedPointMath)
set(ThisShortLib FpmQ)
set(Playground Pg)
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
    test/playground.cpp
)

set(Include
    src
)

add_compile_options(-Wall -Wextra -pedantic -ffunction-sections -fdata-sections)  # -v for verbose
#add_link_options(-Wl,-static -Wl,--gc-sections -Wl,-s)  # no debug info
add_link_options(-Wl,-static)  # with debug info

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
add_compile_options(-fconcepts-diagnostics-depth=2)
endif()

add_library(${ThisShortLib} STATIC ${Headers})
set_target_properties(${ThisShortLib} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(${ThisShortLib} PRIVATE -g -Oz)  # optimize for size

add_executable(${Playground} ${Headers} ${Sources})
set_target_properties(${Playground} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${Playground} PRIVATE ${Include})
target_link_options(${Playground} PUBLIC LINKER:-Map=${Playground}.map)
target_compile_options(${Playground} PRIVATE -Oz)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# size analysis
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
list(TRANSFORM Sources PREPEND $(CMAKE_BINARY_DIR)/CMakeFiles/${Playground}.dir/ OUTPUT_VARIABLE PgOut)
list(TRANSFORM PgOut APPEND .obj OUTPUT_VARIABLE PgOutObj)
# add_custom_command(TARGET ${Playground} POST_BUILD
#                    COMMAND llvm-size --radix=10 --format=sysv ${PgOutObj}
#                    COMMENT "Running llvm-size on ${Playground}")
add_custom_command(TARGET ${Playground} POST_BUILD
                   COMMAND llvm-size --radix=10 --format=berkeley ${PgOutObj}
                   COMMENT "Running llvm-size on ${Playground}")
endif()
