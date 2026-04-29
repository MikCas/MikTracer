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
    src/Vec3.cpp
    src/ImageBuffer.cpp
    src/Scene.cpp
    src/Renderer.cpp)

target_include_directories(miktracer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(scenes)

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.5.2 # Always use a specific version for stability
    )
    FetchContent_MakeAvailable(Catch2)

    add_subdirectory(tests)
endif()




