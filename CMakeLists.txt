# CMakeList.txt : CMake project for RootTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("PowerGraphs" LANGUAGES CXX)

# Find ROOT installation
# Ensure ROOT is set up in your environment (e.g., b "LinkDef.h"y sourcing thisroot.sh)
find_package(ROOT REQUIRED) # Add components as needed

# Output ROOT found successfuly
message(STATUS "ROOT ${ROOT_VERSION} found at ${ROOT_BINDIR}")

# Sets up global settings
include(${ROOT_USE_FILE})

set(CMAKE_CXX_STANDARD ${ROOT_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define source files
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DateHelper.cpp
)

set(HEDERS
    ${CMAKE_CURRENT_SOURCE_DIR}/DateHelper.h
)

# Define source files (for dictionary generation only)
set(SOURCES_FOR_DICT
    ${CMAKE_CURRENT_SOURCE_DIR}/MyMainFrame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MyFrame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CanvasHelper.cpp
)

# Define header files (both - for dictionary generation & other program headers they depend on)
set(HEADERS_FOR_DICT
    ${CMAKE_CURRENT_SOURCE_DIR}/MyMainFrame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MyFrame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CanvasHelper.h
)

# Need to specify include directories for custome headers (
# include_directories(${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/)

# Generate dictionary "G__${PROJECT_NAME}.cxx" - from headers with ClassDef and LinkDef.h
root_generate_dictionary(
    G__${PROJECT_NAME} 
    ${HEADERS_FOR_DICT}
    MODULE lib${PROJECT_NAME} 
    LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h
)

# Our shared library source files include headers (DateHelper.h), we need to tell CMake where to find them
# https://runebook.dev/en/articles/cmake/command/add_library
include_directories(./)

# Create shared library but first specify.CMake adds "lib" prefix automatically
add_library(${PROJECT_NAME}Lib SHARED ${SOURCES_FOR_DICT} G__${PROJECT_NAME}.cxx)

# Compose list of ROOT libraries with "ROOT::" prefix - need to link them to the shared library
set(LIB_NAMES "")
list(APPEND LIB_NAMES "ROOT::Core")
list(APPEND LIB_NAMES "ROOT::Gui")
list(APPEND LIB_NAMES "ROOT::Gpad")
#list(APPEND LIB_NAMES "ROOT::Graf3d")
#list(APPEND LIB_NAMES "ROOT::Graf")
list(APPEND LIB_NAMES "ROOT::Hist")
#list(APPEND LIB_NAMES "ROOT::Imt")
#list(APPEND LIB_NAMES "ROOT::MathCore")
#list(APPEND LIB_NAMES "ROOT::Matrix")
#list(APPEND LIB_NAMES "ROOT::Net")
#list(APPEND LIB_NAMES "ROOT::Physics")
#list(APPEND LIB_NAMES "ROOT::Postscript")
#list(APPEND LIB_NAMES "ROOT::RIO")
#list(APPEND LIB_NAMES "ROOT::ROOTDataFrame")
#list(APPEND LIB_NAMES "ROOT::Rint")
#list(APPEND LIB_NAMES "ROOT::Tree")
list(APPEND LIB_NAMES "ROOT::XMLIO")
list(APPEND LIB_NAMES  "ROOT::XMLParser")

# Link ROOT libraries to generated shared library
target_link_libraries(${PROJECT_NAME}Lib ${LIB_NAMES})

# Depend shared library on dict generation to rebuld automatically
# add_dependencies(lib${PROJECT_NAME} G__${PROJECT_NAME})

# This is required for successful linking on Windows
if(MSVC)
   set_target_properties(${PROJECT_NAME}Lib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

# This should go last
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link executable to the shared library
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}Lib)

