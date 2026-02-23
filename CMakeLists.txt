# CMakeList.txt : CMake project for RootTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

project ("PowerGraphs" LANGUAGES CXX)

# Find ROOT installation
# Ensure ROOT is set up in your environment (e.g., b "LinkDef.h"y sourcing thisroot.sh)
find_package(ROOT REQUIRED) # Add components as needed

# Output ROOT found successfuly
message(STATUS "ROOT ${ROOT_VERSION} found at ${ROOT_BINDIR}")

# Sets up settings and variables defined for the ROOT library in `ROOT_USE_FILE.cmake`
include(${ROOT_USE_FILE})

set(CMAKE_CXX_STANDARD ${ROOT_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# https://patorjk.com/software/taag/#p=display&f=RubiFont&t=Files&x=none&v=4&h=4&w=80&we=false

# ▗▄▄▄▖▗▄▄▄▖▗▖   ▗▄▄▄▖ ▗▄▄▖
# ▐▌     █  ▐▌   ▐▌   ▐▌   
# ▐▛▀▀▘  █  ▐▌   ▐▛▀▀▘ ▝▀▚▖
# ▐▌   ▗▄█▄▖▐▙▄▄▖▐▙▄▄▖▗▄▄▞▘

# Define source files
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
)

# Define source files (for dictionary generation only)
set(SOURCES_FOR_DICT
    ${CMAKE_CURRENT_SOURCE_DIR}/MyMainFrame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MyFrame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CanvasHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DateHelper.cpp
)

# Define header files (both - for dictionary generation & other program headers they depend on)
set(HEADERS_FOR_DICT
    ${CMAKE_CURRENT_SOURCE_DIR}/MyMainFrame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MyFrame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CanvasHelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DateHelper.h
)

# Need to specify include directories for custome headers (
# include_directories(${ROOT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/)

# Generate dictionary "G__${PROJECT_NAME}.cxx" - from headers with ClassDef and LinkDef.h
root_generate_dictionary(
    G__${PROJECT_NAME} 
    ${HEADERS_FOR_DICT}
    MODULE ${PROJECT_NAME} 
    LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h
)

# ▗▖   ▗▄▄▄▖▗▄▄▖ ▗▄▄▖  ▗▄▖ ▗▄▄▖▗▖  ▗▖
# ▐▌     █  ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▝▚▞▘ 
# ▐▌     █  ▐▛▀▚▖▐▛▀▚▖▐▛▀▜▌▐▛▀▚▖ ▐▌  
# ▐▙▄▄▖▗▄█▄▖▐▙▄▞▘▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌ ▐▌  

# Our shared library source files include headers (DateHelper.h), we need to tell CMake where to find them
# https://runebook.dev/en/articles/cmake/command/add_library
include_directories(./)

# Create shared library. CMake adds "lib" prefix automatically
add_library(${PROJECT_NAME}Lib SHARED ${SOURCES_FOR_DICT} G__${PROJECT_NAME}.cxx)


# If portable build and install - relative Run Path for shared library (relative to executable location)
# Tip: CMake applies different run paths for built shared libraries and for installed shared libraries. 
if(PORTABLE_INSTALL)
    message(STATUS "PORTABLE_INSTALL is defined")
    # Set rpath to use for installed targets (here - shared libraries) $ORIGIN - on Linux, @rpath on macOS (former @loader_path)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        # TODO: try dependency walker on linux
        SET(CMAKE_INSTALL_RPATH "$ORIGIN/")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        # On macOS, default RPATHs will be used in the build tree, but cleared when installing the targets
        # With "Dependency Walker" for macOS it is possible to inspect build targets' RPATH and assign same RPATHs to installed items:
        SET(CMAKE_INSTALL_RPATH "${ROOT_LIBRARY_DIR};@executable_path") # for executable
        SET_TARGET_PROPERTIES(${PROJECT_NAME}Lib PROPERTIES
            INSTALL_RPATH "${ROOT_LIBRARY_DIR}"                         # for shared library
        )        
    else()
        # On Windows, relative .dll search path is always included in executable, no need to set relative run path
    endif()
    # Use relative paths in the build tree (optional, for convenience during development)
    # SET(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
endif()

# Compose list of ROOT libraries with "ROOT::" prefix - need to link them to the shared library
set(LIB_NAMES "")
#list(APPEND LIB_NAMES "ROOT::Core")
list(APPEND LIB_NAMES "ROOT::Gui")
list(APPEND LIB_NAMES "ROOT::Gpad")
#list(APPEND LIB_NAMES "ROOT::Graf3d")
list(APPEND LIB_NAMES "ROOT::Graf")
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
#list(APPEND LIB_NAMES  "ROOT::XMLParser")

# Link ROOT libraries to generated shared library
target_link_libraries(${PROJECT_NAME}Lib ${LIB_NAMES})

# Depend shared library on dict generation to rebuld automatically
# add_dependencies(lib${PROJECT_NAME} G__${PROJECT_NAME})

# Marks all the symbols in the shared library for export
if(MSVC)
   set_target_properties(${PROJECT_NAME}Lib PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

# This should go last
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link executable to the shared library
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}Lib)

# ▗▄▄▄▖▗▖  ▗▖ ▗▄▄▖▗▄▄▄▖▗▄▖ ▗▖   ▗▖   
#   █  ▐▛▚▖▐▌▐▌     █ ▐▌ ▐▌▐▌   ▐▌   
#   █  ▐▌ ▝▜▌ ▝▀▚▖  █ ▐▛▀▜▌▐▌   ▐▌   
# ▗▄█▄▖▐▌  ▐▌▗▄▄▞▘  █ ▐▌ ▐▌▐▙▄▄▖▐▙▄▄▖

# Get user's HOME directory location (for installation path)
IF(UNIX OR APPLE)
    SET(USER_HOME_DIR "$ENV{HOME}")
ELSEIF(WIN32)
    SET(USER_HOME_DIR "$ENV{USERPROFILE}")
ENDIF()
message(STATUS "User Home Directory: ${USER_HOME_DIR}")

# Specify install paths (prefix) for different platforms
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/.local/bin" CACHE PATH "linux install path" FORCE)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CMAKE_INSTALL_PREFIX "/Applications/${PROJECT_NAME}.app/Contents" CACHE PATH "macOS install path" FORCE)
    elseif(WIN32)
        set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/AppData/Local/${PROJECT_NAME}" CACHE PATH "Windows install path" FORCE)
    endif()
    message(STSTUS "Install path is set to ${CMAKE_INSTALL_PREFIX}")
endif()

# Install shared library
# DESTINATION below is is relative to CMAKE_INSTALL_PREFIX
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS ${PROJECT_NAME}Lib DESTINATION "./")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    install(TARGETS ${PROJECT_NAME}Lib DESTINATION MacOS)
elseif(WIN32)
    install(TARGETS ${PROJECT_NAME}Lib DESTINATION "./")
endif()

# Install ROOT pre-compiled modules (*.pcm files)
# DESTINATION below is is relative to CMAKE_INSTALL_PREFIX
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(CODE "file(GLOB _pcms \"${CMAKE_CURRENT_BINARY_DIR}/*.pcm\")"
            CODE "file(INSTALL \${_pcms} DESTINATION \"\${CMAKE_INSTALL_PREFIX}\")")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    install(CODE "file(GLOB _pcms \"${CMAKE_CURRENT_BINARY_DIR}/*.pcm\")"
            CODE "file(INSTALL \${_pcms} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/MacOS\")")
elseif(WIN32)
    install(CODE "file(GLOB _pcms \"${CMAKE_CURRENT_BINARY_DIR}/*.pcm\")"
            CODE "file(INSTALL \${_pcms} DESTINATION \"\${CMAKE_INSTALL_PREFIX}\")")
endif()

# Install executable
# DESTINATION below is is relative to CMAKE_INSTALL_PREFIX
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    install(TARGETS ${PROJECT_NAME} DESTINATION "./")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    install(TARGETS ${PROJECT_NAME} DESTINATION "MacOS")
elseif(WIN32)
    install(TARGETS ${PROJECT_NAME} DESTINATION "./")
endif()

# Install iapplication icon and launcher
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Install icon
    install(CODE "MESSAGE(STATUS \"Installing icon\")")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.png" DESTINATION "$ENV{HOME}/.local/share/icons/hicolor/256x256/apps")
    # Update icon cache
    install(CODE "MESSAGE(STATUS \"Updating icon cache\")")
    set (update_icon_cache_cmd "gtk-update-icon-cache")
    install(CODE "execute_process(COMMAND \"${update_icon_cache_cmd}\")")
    # Install launcher
    install(CODE "MESSAGE(STATUS \"Installing launcher\")")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.desktop" DESTINATION "$ENV{HOME}/.local/share/applications")
    # Update launcher cache
    install(CODE "MESSAGE(STATUS \"Updating launcher cache\")")
    set (update_launcher_cache_cmd "xdg-desktop-menu")
    set (update_launcher_cache_args "forceupdate")
    install(CODE "execute_process(COMMAND \"${update_launcher_cache_cmd}\" \"${update_launcher_cache_args}\")")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Install icon
    install(CODE "MESSAGE(STATUS \"Installing icon\")")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.icns" DESTINATION Resources)
    # Install launcher
    install(CODE "MESSAGE(STATUS \"Installing Info.plist\")")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist" DESTINATION "./")
elseif(WIN32)
    # Install icon
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.ico" DESTINATION "./")
    # Desktop shortcut is created in the "install.bat"
endif()