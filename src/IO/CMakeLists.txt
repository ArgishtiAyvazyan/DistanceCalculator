project(DistanceCalculator)

set(PROJECT_SOURCES CLIParser.cc Msg.cpp)

add_library(InputOutputLib STATIC ${PROJECT_SOURCES})
target_include_directories(InputOutputLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(InputOutputLib PUBLIC dbgh_asserts_lib)

