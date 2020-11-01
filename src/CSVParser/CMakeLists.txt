project(DistanceCalculator)


set(PROJECT_SOURCES
        "CSVParser.h"
        "CSVParser.cc"
        "CSVCell.cc"
        "CSVRow.cc"
        "CSVUtils.h"
        "CSVUtils.cc")

add_library(CSVParserLib STATIC ${PROJECT_SOURCES})
target_include_directories(CSVParserLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CSVParserLib PUBLIC dbgh_asserts_lib)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(CSVParserLib PRIVATE pthread tbb)
endif()
