# include the AGE specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# add the AGE sources path
include_directories(${CPROJECT_SOURCE_DIR}/src)

# set the output directory for AGE libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# set the source directory for include and source files
set(INCROOT ${PROJECT_SOURCE_DIR}/include/AGE)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/AGE)

# AGE needs to export symbols when build so define the export flag
add_definitions(-DAGE_EXPORTS)

# add the modules subdirectories
add_subdirectory(Core)

# install Config header include file
install(FILES ${INCROOT}/Config.hpp
        DESTINATION include/AGE
        COMPONENT dev)
