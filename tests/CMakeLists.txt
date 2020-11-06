cmake_minimum_required(VERSION 3.1)
project(CSVParserTests)


add_executable(CSVParserTests CSVParserTest.cc)
add_executable(CSVParserBenchmark CSVParserBenchmark.cc)
add_executable(MathEngineTests MathEngineTest.cc)
add_executable(MathEngineBenchmark MathEngineBenchmark.cc)
add_executable(CLIParserTests CLIParserTests.cc)

target_link_libraries(CSVParserTests PRIVATE Catch2 CSVParserLib)
target_link_libraries(CSVParserBenchmark PRIVATE Catch2 CSVParserLib)
target_link_libraries(MathEngineTests PRIVATE Catch2 MathEngineLib)
target_link_libraries(MathEngineBenchmark PRIVATE Catch2 MathEngineLib)
target_link_libraries(CLIParserTests PRIVATE Catch2 InputOutputLib)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(CSVParserTests PRIVATE pthread tbb)
endif()

target_compile_features(CSVParserTests PUBLIC cxx_std_17)
