cmake_minimum_required(VERSION 3.25)

set(This FpmTests)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Sources
    fpm.test.cpp
    q.test.cpp
    sq.test.cpp
)
set(Headers
)


add_compile_options(-Wall -Wextra -pedantic -static)  # -v for verbose

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
add_compile_options(-fconcepts-diagnostics-depth=4)
endif()

add_executable(${This} ${Sources} ${Headers})
target_link_libraries(${This} PUBLIC ${CMAKE_THREAD_LIBS_INIT}
    gtest_main
)
target_link_options(${This} PUBLIC LINKER:-Map=${This}.map -static)  # -v for verbose

target_include_directories(${This} PUBLIC ../src ../googletest/googletest/include)
