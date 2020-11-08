project(DistanceCalculator)


set(PROJECT_SOURCES Main.cc MainApplication.cc)

add_executable(distance_calculator_exe ${PROJECT_SOURCES})
target_include_directories(distance_calculator_exe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(distance_calculator_exe PUBLIC InputOutputLib CSVParserLib MathEngineLib CommonLib)
