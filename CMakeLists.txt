cmake_minimum_required(VERSION 3.28)
project(EngineDll VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add your source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the DLL target
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Define ENGINE_DLL_EXPORTS when building the DLL
target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_DLL_EXPORTS)

# Include directory
include_directories(include)

# Include GLFW
include_directories(third_party)


find_library(GLFW_LIBRARY glfw3 HINTS ${CMAKE_SOURCE_DIR}/libs ${CMAKE_SOURCE_DIR}/submodules/Engine/libs REQUIRED)
find_library(GLEW_LIBRARY glew32s HINTS ${CMAKE_SOURCE_DIR}/libs ${CMAKE_SOURCE_DIR}/submodules/Engine/libs REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    OpenGL::GL
)

# Define your test source files
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

# Add an executable target for your test
add_executable(EngineDllTest ${TEST_SOURCES})

# Link your test executable with the library you want to test
target_link_libraries(EngineDllTest PRIVATE ${PROJECT_NAME})

# Optionally, you can use CTest to automate running your tests
enable_testing()
add_test(NAME EngineDllTest COMMAND EngineDllTest)
