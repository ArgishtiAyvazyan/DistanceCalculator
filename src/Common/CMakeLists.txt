project(DistanceCalculator)

set(PROJECT_SOURCES "Task.cc")

add_library(CommonLib STATIC ${PROJECT_SOURCES})
target_include_directories(CommonLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CommonLib PUBLIC InputOutputLib)

