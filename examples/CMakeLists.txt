# Build options
set(OUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
    set(DEFAULT_CXX_FLAGS "/W4 /O2")
else()
    set(DEFAULT_CXX_FLAGS "-Wall -Wextra -Wcast-align -Wconversion -Wunreachable-code -Wuninitialized -pedantic -g -O2")
endif()

if (NOT CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS}" CACHE STRING "Default C++ compiler flags" FORCE)
endif()

message(STATUS "Examples Build options:")
message(STATUS "[OUT_DIR = ${OUT_DIR}]")
message(STATUS "[CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}]")

# Helper macro to register an example project
macro(make_example PROJECT_NAME TARGET_NAME)
    add_executable(${TARGET_NAME} main.cpp)
    set_target_properties(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME "run" # don't do that - go up a dir and create the binary with the example name
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )
    target_link_libraries(${TARGET_NAME} PRIVATE cpp-argon)
endmacro()

# Build projects
add_subdirectory(argon_git)
add_subdirectory(file_merger)
add_subdirectory(logging_mode)
add_subdirectory(message_logger)
add_subdirectory(numbers_converter)
add_subdirectory(power_calculator)
add_subdirectory(verbosity)
