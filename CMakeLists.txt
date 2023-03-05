cmake_minimum_required(VERSION 3.1.0)
project(FixedPointMath VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(FpmQ src/main.cpp)

target_compile_features(FpmQ PUBLIC cxx_std_23)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
