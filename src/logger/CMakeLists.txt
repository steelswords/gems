cmake_minimum_required(VERSION 3.16)

project(GemsLogger)

add_library(GemsLogger
    logger.cpp
    logger.hpp
)

if (${BUILD_TESTING})
    include(CTest)

    add_executable(LoggerTest tests/logger_test.cpp)
    target_link_libraries(LoggerTest GemsLogger)
    #target_include_directories(LoggerTest ${CMAKE_CURRENT_LIST_DIR})

    set(loggerTestNames
        BasicDebugLog
    )


    foreach(testName ${loggerTestNames})
        add_test(
            NAME LoggerTest.${testName}
            COMMAND LoggerTest ${testName}
        )
    endforeach()
endif()
