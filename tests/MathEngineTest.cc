/**
 * @file        MathEngineTest.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for MathEngine.
 * @date        01-11-2020
 * @copyright   Copyright (c) 2020
 */

#include <algorithm>
#include <vector>
#include <thread>

#include <DBGHAssert.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "DistanceCalculator.h"
#include "MathKernel.h"
#include "MTMathKernel.h"

#include "MathEngineTestUtil.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// L1 Testing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


TEST_CASE( "Check MathEngine simple test (L1)", "[MathEngine]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    using value_type = float;
    using Matrix = typename math::IMathKernel<value_type>::Matrix;

    Matrix query   { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 } };

    Matrix dataSet { { 3, 4, 5 }, { 2, 3, 4 }, { 1, 2, 3 } };

    Matrix target { { 6, 3, 0 }, { 3, 0, 3 }, { 0, 3, 6 } };

    math::DistanceCalculator<value_type> calculator { std::make_unique<math::MathKernel<value_type>>() };

    Matrix result = calculator.computeDistance(query, dataSet, math::EDistanceMetric::L1);

    REQUIRE(result == target);
}

TEST_CASE( "Check MathEngine, Distance calculation between equal matrices. (L1)", "[MathEngine]" )
{
    checkDistanceBetweenEqualMatrices<int>(math::EDistanceMetric::L1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// L2 Testing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE( "Check MathEngine simple test (L2)", "[MathEngine]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    using value_type = float;
    using Matrix = typename math::IMathKernel<value_type>::Matrix;

    Matrix query   { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 } };

    Matrix dataSet { { 3, 4, 5 }, { 2, 3, 4 }, { 1, 2, 3 } };

    Matrix target { { 3.4641f, 1.73205f, 0.0f }, { 1.73205f, 0.0f, 1.73205f }, { 0.0f, 1.73205f, 3.4641f } };

    math::DistanceCalculator<value_type> calculator { std::make_unique<math::MathKernel<value_type>>() };

    Matrix result = calculator.computeDistance(query, dataSet, math::EDistanceMetric::L2);

    REQUIRE(std::size(target) == std::size(result));
    for (size_t r = 0; r < std::size(target); ++r)
    {
        REQUIRE(std::size(target[r]) == std::size(result[r]));
        for (size_t c = 0; c < std::size(target[r]); ++c)
        {
            REQUIRE(target[r][c] == Approx(result[r][c]));
        }
    }
}

TEST_CASE( "Check MathEngine, Distance calculation between equal matrices. (L2)", "[MathEngine]" )
{
    checkDistanceBetweenEqualMatrices<int>(math::EDistanceMetric::L2);
}

TEST_CASE( "Check MathEngine, Distance equal vectors. (L2)", "[MathEngine]" )
{
    checkDistanceBetweenEqualVectors<int>(math::EDistanceMetric::L2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hamming Testing
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE( "Check MathEngine simple test (Hamming)", "[MathEngine]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    using value_type = float;
    using Matrix = typename math::IMathKernel<value_type>::Matrix;

    Matrix query   { { 1, 2, 3 }, { 2, 3, 4 }, { 3, 4, 5 } };

    Matrix dataSet { { 3, 4, 5 }, { 2, 3, 4 }, { 1, 2, 3 } };

    Matrix target { { 3, 3, 0 }, { 3, 0, 3 }, { 0, 3, 3 } };

    math::DistanceCalculator<value_type> calculator { std::make_unique<math::MathKernel<value_type>>() };

    Matrix result = calculator.computeDistance(query, dataSet, math::EDistanceMetric::Hamming);

    REQUIRE(result == target);
}

TEST_CASE( "Check MathEngine, Distance calculation between equal matrices. (Hamming)", "[MathEngine]" )
{
    checkDistanceBetweenEqualMatrices<int>(math::EDistanceMetric::Hamming);
}

TEST_CASE( "Check MathEngine, Distance equal vectors. (Hamming)", "[MathEngine]" )
{
    checkDistanceBetweenEqualVectors<int>(math::EDistanceMetric::Hamming);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MTMathKernel tests.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void compareMathKernelMTMathKernel(math::EDistanceMetric metric)
{
    using value_type = T;
    using Matrix = typename math::IMathKernel<value_type>::Matrix;

    const auto query = crateTestQuery<value_type>(128);
    const auto dataSet = crateTestQuery<value_type>(128);

    math::DistanceCalculator<value_type> sequentialCalculator { std::make_unique<math::MathKernel<value_type>>() };
    math::DistanceCalculator<value_type> parallelCalculator { std::make_unique<math::MTMathKernel<value_type>>() };


    Matrix seResult = sequentialCalculator.computeDistance(query, dataSet, metric);
    Matrix mtResult = parallelCalculator.computeDistance(query, dataSet, metric);

    REQUIRE(seResult == mtResult);
}

TEST_CASE( "Check MTMathEngine, Compare MathKernel and MTMathKernel results. (L1)", "[MathEngine]" )
{
    compareMathKernelMTMathKernel<int>(math::EDistanceMetric::L1);
    compareMathKernelMTMathKernel<float>(math::EDistanceMetric::L1);
    compareMathKernelMTMathKernel<double>(math::EDistanceMetric::L1);
}

TEST_CASE( "Check MTMathEngine, Compare MathKernel and MTMathKernel results. (L2)", "[MathEngine]" )
{
    compareMathKernelMTMathKernel<int>(math::EDistanceMetric::L2);
    compareMathKernelMTMathKernel<float>(math::EDistanceMetric::L2);
    compareMathKernelMTMathKernel<double>(math::EDistanceMetric::L2);
}

TEST_CASE( "Check MTMathEngine, Compare MathKernel and MTMathKernel results. (Hamming)", "[MathEngine]" )
{
    compareMathKernelMTMathKernel<int>(math::EDistanceMetric::Hamming);
    compareMathKernelMTMathKernel<float>(math::EDistanceMetric::Hamming);
    compareMathKernelMTMathKernel<double>(math::EDistanceMetric::Hamming);
}
