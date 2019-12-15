# Specifies the CMake version we require
cmake_minimum_required (VERSION 3.9)

set(IO_LIBRARY "PSI")
set(IO_TEST "IO_Test")

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/Output)

###########################
# Import / Export Library #
###########################
project (${IO_LIBRARY} LANGUAGES C CXX)

# Set our Includes folder as the place to look for library includes
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/Includes/)
# Set the source directory as an include folder for relative includes
include_directories(${CMAKE_SOURCE_DIR}/Source)

# Add source subdirectory which contains the source files
add_subdirectory(Source)

add_library(${IO_LIBRARY} STATIC
    ${IO_LIBRARY_SOURCES}
)

set_target_properties(${IO_LIBRARY} PROPERTIES PUBLIC_HEADER "${IO_LIBRARY_PUBLIC_HEADERS}")

install(TARGETS ${IO_LIBRARY}
    CONFIGURATIONS Debug
    ARCHIVE DESTINATION ${OUTPUT_DIR}/lib/Debug
    PUBLIC_HEADER DESTINATION ${OUTPUT_DIR}/include
    RUNTIME DESTINATION ${OUTPUT_DIR}/bin
)
install(TARGETS ${IO_LIBRARY}
    CONFIGURATIONS Release
    ARCHIVE DESTINATION ${OUTPUT_DIR}/lib/Release
    PUBLIC_HEADER DESTINATION ${OUTPUT_DIR}/include
    RUNTIME DESTINATION ${OUTPUT_DIR}/bin
)

#############################
# Import / Export Unit Test #
#############################
project (${IO_TEST} LANGUAGES C CXX)

# Specify the name of the project executable and which sources should be used in the project
add_executable(${IO_TEST}
    ${IO_TEST_SOURCES}
)

add_dependencies(${IO_TEST} ${IO_LIBRARY})

# Specify the libraries to use when linking the executable
target_link_libraries (${IO_TEST} ${IO_LIBRARY})

install(TARGETS ${IO_TEST}
    RUNTIME DESTINATION ${OUTPUT_DIR}/tests
)
