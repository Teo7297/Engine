set( _freetype_HEADER_SEARCH_DIRS
"${CMAKE_SOURCE_DIR}/includes"
"${CMAKE_SOURCE_DIR}/third_party/freetype"
"${CMAKE_SOURCE_DIR}/submodules/Engine/third_party/freetype"
"C:/Program Files (x86)/freetype" )
set( _freetype_LIB_SEARCH_DIRS
"${CMAKE_SOURCE_DIR}/lib"
"${CMAKE_SOURCE_DIR}/third_party/freetype" 
"${CMAKE_SOURCE_DIR}/submodules/Engine/third_party/freetype" 
)


# Search for the header
FIND_PATH(FREETYPE_INCLUDE_DIR "." PATHS ${_freetype_HEADER_SEARCH_DIRS} )

# Search for the library
FIND_LIBRARY(FREETYPE_LIBRARY NAMES freetype PATHS ${_freetype_LIB_SEARCH_DIRS} )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREETYPE DEFAULT_MSG FREETYPE_LIBRARY FREETYPE_INCLUDE_DIR)