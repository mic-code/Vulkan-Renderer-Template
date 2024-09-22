cmake_minimum_required(VERSION 3.26)
project(Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan)

message(STATUS "Found Vulkan in ${Vulkan_INCLUDE_DIRS}")
message(STATUS "Found Vulkan libs in ${Vulkan_LIBRARIES}")


set(TINYGLTF_INCLUDE_DIR)
set(GLM_INCLUDE_DIR)
set(GLFW3_INCLUDE_DIR)
set(JSON_INCLUDE_DIR)


include_directories("${Vulkan_INCLUDE_DIRS}")

file(GLOB_RECURSE)

add_executable(Vulkan_Engine_Template)

target_sources(Vulkan_Engine_Template)
