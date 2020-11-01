cmake_minimum_required (VERSION 3.1)

project(DistanceCalculator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

########################################################################################################################
# Main sources.
########################################################################################################################

add_subdirectory ("src")
add_subdirectory ("thirdparty/DBGH_ASSERTS")


########################################################################################################################
# Tests sources.
########################################################################################################################

add_subdirectory ("tests")
add_subdirectory ("thirdparty/Catch2")
