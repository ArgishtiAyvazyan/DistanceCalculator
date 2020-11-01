project(DistanceCalculator)

set(PROJECT_SOURCES
        MathKernel.h
        IMathKernel.cc
        DistanceCalculator.h)

add_library(MathEngineLib STATIC ${PROJECT_SOURCES})
target_include_directories(MathEngineLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MathEngineLib PUBLIC dbgh_asserts_lib)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using GCC
    target_link_libraries(MathEngineLib PRIVATE pthread tbb)
endif()
