# CMakeList.txt : CMake project for UniversalUI, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("UniversalUI")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

# Collect header and source files
file(GLOB SOURCE_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB TEST_FILES ${TEST_DIR}/*.cpp)

# Add source to this project's executable.
add_executable (UniversalUI ${SOURCE_FILES} ${TEST_FILES})

# Include the ./include directory for header files
target_include_directories(UniversalUI PUBLIC ${CMAKE_SOURCE_DIR}/include/UniversalUI/ ${CMAKE_SOURCE_DIR}/include/)
