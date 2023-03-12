cmake_minimum_required(VERSION 3.25)

set(This FixedPointMath)
set(ThisShort FpmQ)
project(${This} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)


enable_testing()

add_subdirectory(googletest)
add_subdirectory(test)

set(Headers
    src/fpm/fpm.hpp
    src/fpm/q.hpp
    src/fpm/sq.hpp
)

set(Sources
    src/fpm/fpm.cpp
)


add_library(${ThisShort} STATIC ${Headers} ${Sources})
set_target_properties(${ThisShort} PROPERTIES LINKER_LANGUAGE CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
