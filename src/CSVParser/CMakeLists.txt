project(DistanceCalculator)


set(PROJECT_SOURCES
        "CSVParser.h"
        "CSVParser.cc"
        "CSVCell.cc"
        "CSVRow.cc")

add_library(CSVParserLib STATIC ${PROJECT_SOURCES})
target_include_directories(CSVParserLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CSVParserLib PUBLIC dbgh_asserts_lib)

target_compile_features(CSVParserLib PUBLIC cxx_std_20)
