cmake_minimum_required(VERSION 3.28)
project(EngineDll VERSION 1.0)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add your source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "third_party/imgui/*.cpp") # TODO find imgui

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(ENGINE_FONTS_DIR ${CMAKE_SOURCE_DIR}/fonts)
set(ENGINE_SHADERS_DIR ${CMAKE_SOURCE_DIR}/fonts)
if(NOT EXISTS ${ENGINE_FONTS_DIR}) # if this does not exists FOR NOW it means we are in a submodule.. FIX THIS!
    set(ENGINE_FONTS_DIR ${CMAKE_SOURCE_DIR}/submodules/Engine/fonts)
    set(ENGINE_SHADERS_DIR ${CMAKE_SOURCE_DIR}/submodules/Engine/shaders)
endif()

file(COPY ${ENGINE_FONTS_DIR} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${ENGINE_SHADERS_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Add the DLL target
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Define ENGINE_DLL_EXPORTS when building the DLL
target_compile_definitions(${PROJECT_NAME} PRIVATE ENGINE_DLL_EXPORTS)

# Find GLEW package
# set(GLEW_USE_STATIC_LIBS ON) # remove this line for dynamic linking
set(CMAKE_PREFIX_PATH "third_party\\glew")
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

find_package(GLFW3 REQUIRED) # statically linked

find_package(FREETYPE REQUIRED)

target_include_directories( ${PROJECT_NAME} PUBLIC
    include
    ${FREETYPE_INCLUDE_DIR}
    ${GLFW3_INCLUDE_DIR}
    "third_party/freetype"
    "third_party/imgui"
    "third_party"
    )

target_link_options(${PROJECT_NAME} PRIVATE "/NODEFAULTLIB:MSVCRT")
target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${GLFW3_LIBRARY}    
        GLEW::GLEW
        OpenGL::GL
        ${FREETYPE_LIBRARY}
        "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64\\opengl32.lib"
    )


# This copy is for dll (dynamic linking)
# Set source directory
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/glew/bin/Release/x64/glew32.dll")
if(NOT EXISTS ${SOURCE_DIR})
    set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/submodules/Engine/third_party/glew/bin/Release/x64/glew32.dll")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
${SOURCE_DIR}
$<TARGET_FILE_DIR:${PROJECT_NAME}>)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/freetype/bin/freetype.dll")
if(NOT EXISTS ${SOURCE_DIR})
    set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/submodules/Engine/third_party/freetype/bin/freetype.dll")
endif()
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SOURCE_DIR}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

################################################## TESTS #########################################################

# Define your test source files
# file(GLOB_RECURSE TEST_SOURCES "test/EngineDllTest.cpp")

# Add an executable target for your test
add_executable(TextureTest "test/TextureTest.cpp")
add_test(NAME TextureTest COMMAND TextureTest)
target_link_libraries(TextureTest PRIVATE ${PROJECT_NAME})

add_executable(EngineDllTest "test/EngineDllTest.cpp")
add_test(NAME EngineDllTest COMMAND EngineDllTest)
target_link_libraries(EngineDllTest PRIVATE ${PROJECT_NAME})

add_executable(TextRenderingTest "test/TextRenderingTest.cpp")
add_test(NAME TextRenderingTest COMMAND TextRenderingTest)
target_link_libraries(TextRenderingTest PRIVATE ${PROJECT_NAME})


enable_testing()
################################################## TESTS #########################################################
