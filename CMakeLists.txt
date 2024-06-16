cmake_minimum_required(VERSION 3.16)
project(gems)

if (${BUILD_TESTING})
    include(CTest)
endif()
add_subdirectory(src/logger)
# TODO: Others here too
