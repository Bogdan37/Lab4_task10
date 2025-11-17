cmake_minimum_required(VERSION 3.10.0)



project(Lab4_task10 LANGUAGES CXX)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)





find_package(Threads REQUIRED)



if (MSVC)

add_compile_options(/W3 /EHsc /O2 /utf-8)
else()

add_compile_options(-Wall -fexceptions -O3)
endif()





add_executable(Lab4_task10
main.cpp
OptimizedDataStructure.cpp
TestRunner.cpp
        TestRunner.h
        TestRunner.cpp
)



target_link_libraries(Lab4_task10 PRIVATE Threads::Threads)