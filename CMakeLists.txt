# Project master CMakeList.txt file
#
# Reference(s):
# http://www.itk.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file
# http://www.sfml-dev.org/
# @date 20110421 RQL Modified to match example in reference(s) above

# Make sure they are using a recent version of CMake
cmake_minimum_required(VERSION 2.6)

# set a default build type if none was provided
# this has to be done before the project() instruction!
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

# project name
project(AGE)

# Include the macros file for this project
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# project options
if(MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11 -Wall -pedantic")
endif()

set_option(BUILD_STATIC_STD_LIBS FALSE BOOL "Set to TRUE to statically link to the standard libraries, FALSE to use them as DLLs")
set_option(BUILD_SHARED_LIBS FALSE BOOL "Set to FALSE to build static libraries")
set_option(INSTALL_DOC TRUE BOOL "Set to FALSE to skip build/install Documentation")
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set_option(SFML_STATIC_LIBRARIES TRUE BOOL "Set to TRUE to statically link SFML libraries to AGE")
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set_option(SFML_STATIC_LIBRARIES FALSE BOOL "Set to TRUE to statically link SFML libraries to AGE")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set_option(SFMLDIR ${PROJECT_SOURCE_DIR}/extlibs/headers PATH "SFML include directory")

# setup version numbers
set(AGE_VERSION_MAJOR 0)
set(AGE_VERSION_MINOR 1)
set(AGE_VERSION_PATCH 0)
set(AGE_VERSION "${AGE_VERSION_MAJOR}.${AGE_VERSION_MINOR}.${AGE_VERSION_PATCH}")

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules/)

# include the configuration file after the project options are defined
include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/extlibs/headers")

# disable the rpath stuff
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# add the AGE subdirectory
add_subdirectory(src/AGE)

if(INSTALL_DOC)
    add_subdirectory(doc)
endif()

# Add all targets to the build-tree export set
#export(TARGETS age-core age-entities
#  FILE "${PROJECT_BINARY_DIR}/AGE_LibraryDepends.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
#export(PACKAGE AGE)

# Create a AGE_Config.cmake file for the use from the build tree
set(AGE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
set(AGE_LIB_DIR "${PROJECT_BINARY_DIR}")
set(AGE_CMAKE_DIR "${PROJECT_BINARY_DIR}")
configure_file(age-config.cmake.in
  "${PROJECT_BINARY_DIR}/age-config.cmake" @ONLY)
configure_file(age-configversion.cmake.in
  "${PROJECT_BINARY_DIR}/age-configversion.cmake" @ONLY)

# Install the export set for use with the install-tree
#install(EXPORT AGE_LibraryDepends DESTINATION
#  "cmake"
#  COMPONENT dev)

# Create a AGE_Config.cmake file for the use from the install tree
# and install it
set(AGE_INCLUDE_DIRS include)
set(AGE_LIB_DIR lib)
set(AGE_CMAKE_DIR cmake)
configure_file(age-config.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/age-config.cmake" @ONLY)
configure_file(age-configversion.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/age-configversion.cmake" @ONLY)

# install our config and config version files
install(FILES
  "${PROJECT_BINARY_DIR}/InstallFiles/age-config.cmake"
  "${PROJECT_BINARY_DIR}/InstallFiles/age-configversion.cmake"
  DESTINATION "${AGE_CMAKE_DIR}" COMPONENT dev)

# install our FindAGE cmake module into the Cmake Modules directory
install(FILES ${PROJECT_SOURCE_DIR}/cmake/Modules/FindAGE.cmake DESTINATION cmake/Modules)

# install our license file into our INSTALL_DATA_DIR
install(FILES license.txt DESTINATION .)

if(WINDOWS)
  if(COMPILER_GCC)
    if(ARCH_32BITS)
      if(EXISTS ${SFML_INCLUDE_DIR}/../bin-mingw/x86/libsndfile-1.dll)
        install(FILES extlibs/bin-mingw/x86/libsndfile-1.dll DESTINATION bin)
      endif()
      if(EXISTS ${SFML_INCLUDE_DIR}/../bin-mingw/x86/openal32.dll)
        install(FILES ${SFML_INCLUDE_DIR}/..bin-mingw/x86/openal32.dll DESTINATION bin)
      endif()
    else()
      if(EXISTS ${SFML_INCLUDE_DIR}/../bin-mingw/x64/libsndfile-1.dll)
        install(FILES ${SFML_INCLUDE_DIR}/../bin-mingw/x64/libsndfile-1.dll DESTINATION bin)
      endif()
      if(EXISTS ${SFML_INCLUDE_DIR}/../bin-mingw/x64/openal32.dll)
        install(FILES ${SFML_INCLUDE_DIR}/../bin-mingw/x64/openal32.dll DESTINATION bin)
      endif()
    endif()
  elseif(COMPILER_MSVC)
    if(ARCH_32BITS)
      if(EXISTS ${SFML_INCLUDE_DIR}/../bin-msvc/x86/libsndfile-1.dll)
        install(FILES ${SFML_INCLUDE_DIR}/../bin-msvc/x86/libsndfile-1.dll DESTINATION bin)
      endif()
      if(EXISTS ${SFML_INCLUDE_DIR}/../bin-msvc/x86/openal32.dll)
        install(FILES ${SFML_INCLUDE_DIR}/../bin-msvc/x86/openal32.dll DESTINATION bin)
      endif()
    else()
      if(EXISTS ${SFML_INCLUDE_DIR}/../bin-msvc/x64/libsndfile-1.dll)
        install(FILES ${SFML_INCLUDE_DIR}/../bin-msvc/x64/libsndfile-1.dll DESTINATION bin)
      endif()
      if(EXISTS ${PROJECT_SOURCE_DIR}/../bin-msvc/x64/openal32.dll)
        install(FILES ${SFML_INCLUDE_DIR}/../bin-msvc/x64/openal32.dll DESTINATION bin)
      endif()
    endif()
  endif()
elseif(MACOSX)
  if(EXISTS ${SFML_INCLUDE_DIR}/../libs-osx/Frameworks/sndfile.framework)
    install(DIRECTORY ${SFML_INCLUDE_DIR}/../libs-osx/Frameworks/sndfile.framework DESTINATION /Library/Frameworks PATTERN ".hg" EXCLUDE)
  endif()
endif()


message( "  COMPILER_FLAGS = ${CMAKE_CXX_FLAGS}" )
