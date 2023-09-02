cmake_minimum_required(VERSION 3.0)

project(MetalRT)

set(CMAKE_CXX_STANDARD 17)
find_package(SDL2 REQUIRED)

add_subdirectory(metal-cpp)

include_directories(
    metal-cpp
    ${SDL2_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
    main.cpp
    Application.cpp
    BVH.cpp
    Camera.cpp
    Timer.cpp
)

target_link_libraries( ${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    MetalCPP
    "-framework Metal"
    "-framework QuartzCore"
    "-framework Foundation"
)
