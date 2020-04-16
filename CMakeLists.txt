cmake_minimum_required(VERSION 3.10)
project(unclone C)

# Get packages
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(GD REQUIRED)

# C standard
set(CMAKE_C_STANDARD 99)
# set sources
set(SOURCES src/main.c)

# set other cmake vars
set(GD_USE_STATIC_LIBS OFF)


# Compile and link
if(GD_FOUND)
    include_directories(${GD_INCLUDE_DIRS})
    add_executable(unclone ${SOURCES})
    target_link_libraries(unclone ${GD_LIBRARIES})
endif()


