set( _glfw3_HEADER_SEARCH_DIRS
"${CMAKE_SOURCE_DIR}/includes"
"${CMAKE_SOURCE_DIR}/third_party/glfw/include"
"C:/Program Files (x86)/glfw/include" )
set( _glfw3_LIB_SEARCH_DIRS
"${CMAKE_SOURCE_DIR}/lib"
"${CMAKE_SOURCE_DIR}/third_party/glfw/install/lib" )


# Search for the header
FIND_PATH(GLFW3_INCLUDE_DIR "GLFW/glfw3.h" PATHS ${_glfw3_HEADER_SEARCH_DIRS} )

# Search for the library
FIND_LIBRARY(GLFW3_LIBRARY NAMES glfw3 glfw PATHS ${_glfw3_LIB_SEARCH_DIRS} )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)

include_directories(${GLFW3_INCLUDE_DIR})