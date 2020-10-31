cmake_minimum_required(VERSION 3.1)
project(CSVParserTests)


add_executable(CSVParserTests CSVParserTest.cc)
target_link_libraries(CSVParserTests PRIVATE Catch2 CSVParserLib)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(CSVParserTests PRIVATE pthread)
endif()

target_compile_features(CSVParserTests PUBLIC cxx_std_17)
