cmake_minimum_required(VERSION 3.2.2)

project(CommandLine)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if (NOT TARGET gtest_main)
    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                     ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)
endif()

# Utilities submodule
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/utility EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

add_library(commandline STATIC
    src/commandline.cc
)

target_include_directories(commandline PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(commandline
    superstring
)

# -----------------------------------------------------------------------------

add_executable(commandline-ut
    test/commandline_ut.cc
    test/main.cc
)

target_link_libraries(commandline-ut
    gtest_main
    commandline
)
