cmake_minimum_required(VERSION 3.25)

set(This FpmTests)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)
#[[set(CMAKE_BUILD_TYPE RelWithDebInfo)]]

set(Sources
    Fpm.test.cpp
    Q.test.cpp
    Sq.test.cpp
)
set(Headers
)

add_executable(${This} ${Sources} ${Headers})
target_link_libraries(${This} PUBLIC ${CMAKE_THREAD_LIBS_INIT}
    gtest_main
)

target_include_directories(${This} PUBLIC ../src ../googletest/googletest/include)
