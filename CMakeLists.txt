cmake_minimum_required(VERSION 3.26)
project(UniversalUI)

set(CMAKE_CXX_STANDARD 14)

# Find the Vulkan package
find_package(Vulkan REQUIRED)

add_executable(UniversalUI
        Test/main.cpp
        Windowing/source/uWindow.cpp
        Core/source/UniversalUI.cpp
        Core/source/uApplication.cpp
        Windowing/source/uWindowManager.cpp
        Rendering/source/uRenderManager.cpp
        Rendering/include/uRenderManager.h
)

target_include_directories(UniversalUI PUBLIC "./Core/include")
target_include_directories(UniversalUI PUBLIC "./Windowing/include")
target_include_directories(UniversalUI PUBLIC "./Rendering/include")

# Link Vulkan with your target
target_link_libraries(UniversalUI Vulkan::Vulkan)