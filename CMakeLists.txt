cmake_minimum_required(VERSION 3.16)
project(terragl VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c23-extensions -fsanitize=address")

include(FetchContent)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    SOURCE_SUBDIR cmake
)
FetchContent_MakeAvailable(glad)

add_executable(terragl)

file(GLOB SRC_FILES CONFIGURE_DEPENDS src/*.cpp src/**/*.cpp)
target_sources(terragl PRIVATE ${SRC_FILES})

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Freetype REQUIRED)

glad_add_library(glad_gl_core_46 REPRODUCIBLE API gl:core=4.6)

target_link_libraries(terragl PRIVATE
    glad_gl_core_46
    glfw
    glm
    Freetype::Freetype
    ${OpenGL_LIBRARIES}
    -fsanitize=address
)
