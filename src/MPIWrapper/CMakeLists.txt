project(DistanceCalculator)

set(PROJECT_SOURCES MPIWrapper.cc)

add_library(MPIWrapperLib ${PROJECT_SOURCES})
target_include_directories(MPIWrapperLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MPI_INCLUDE_PATH "C:/Program Files (x86)/Microsoft SDKs/MPI/Include/")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bits
        set(MPI_LIBRARIES "C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x64/msmpi.lib")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32 bits
        set(MPI_LIBRARIES "C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x86/msmpi.lib")
endif()

    include_directories(${MPI_INCLUDE_PATH})
    set(CMAKE_C_FLAGS "${CMAKE_FLAGS} ${MPI_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MPI_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MPI_EXE_LINKER_FLAGS}")
    target_link_libraries(MPIWrapperLib PUBLIC dbgh_asserts_lib MathEngineLib ${MPI_LIBRARIES} )
else()
    find_package(MPI REQUIRED)
    target_link_libraries(MPIWrapperLib PUBLIC dbgh_asserts_lib MathEngineLib MPI::MPI_CXX)
endif()
