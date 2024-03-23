cmake_minimum_required(VERSION 3.10)
project(EngineDll VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add your source files
set(SOURCES
    EngineDll.cpp
)

# Add the DLL target
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Define ENGINE_DLL_EXPORTS when building the DLL
target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_DLL_EXPORTS)

# Define your test source files
set(TEST_SOURCES
    EngineDllTest.cpp
)

# Add an executable target for your test
add_executable(EngineDllTest ${TEST_SOURCES})

# Link your test executable with the library you want to test
target_link_libraries(EngineDllTest PRIVATE ${PROJECT_NAME})

# Optionally, you can use CTest to automate running your tests
enable_testing()
add_test(NAME EngineDllTest COMMAND EngineDllTest)
