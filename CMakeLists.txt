cmake_minimum_required(VERSION 3.14)

project(MikTracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(miktracer_lib
    src/Camera.cpp
    src/Interval.cpp 
    src/Material.cpp 
    src/Object.cpp
    src/Ray.cpp
    src/Vec3.cpp)

target_include_directories(miktracer_lib PUBLIC include)

add_executable(miktracer src/main.cpp)

target_link_libraries(miktracer PRIVATE miktracer_lib)