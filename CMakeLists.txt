cmake_minimum_required(VERSION 3.16)
project(terragl VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
)
FetchContent_MakeAvailable(glad)
add_subdirectory(${glad_SOURCE_DIR}/cmake glad_build)

file(GLOB SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/**/*.cpp)
add_executable(terragl ${SRC_FILES})

target_link_libraries(terragl PRIVATE
    glad
    glfw
    glm::glm
)

if(UNIX AND NOT APPLE)
    target_link_libraries(terragl PRIVATE dl pthread)
endif()
