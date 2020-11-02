/**
 * @file        MathEngineBenchmark.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for MathEngine.
 * @date        02-11-2020
 * @copyright   Copyright (c) 2020
 */


#include <map>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch.hpp>

#include <DBGHAssert.h>
#include "DistanceCalculator.h"
#include "MathKernel.h"
#include "MTMathKernel.h"

#include "MathEngineTestUtil.h"

namespace
{
const std::string& to_string(math::EDistanceMetric metric)
{
    static std::map<math::EDistanceMetric, std::string> s_enumToName
            {
                    { math::EDistanceMetric::L1, "L1" },
                    { math::EDistanceMetric::L2, "L2" },
                    { math::EDistanceMetric::Hamming, "Hamming" },
            };

    return s_enumToName.at(metric);
}
}

template <typename T>
void compareMathKernelMTMathKernel(math::EDistanceMetric metric)
{
    using value_type = T;

    const auto query = crateTestQuery<value_type>(128);
    const auto dataSet = crateTestQuery<value_type>(128);

    math::DistanceCalculator<value_type> sequentialCalculator { std::make_unique<math::MathKernel<value_type>>() };
    math::DistanceCalculator<value_type> parallelCalculator { std::make_unique<math::MTMathKernel<value_type>>() };

    BENCHMARK((std::string {"Sequential computing "} + to_string(metric) + " distance.").c_str())
                {
                    return sequentialCalculator.computeDistance(query, dataSet, metric);
                };

    BENCHMARK((std::string {"Parallel computing "} + to_string(metric) + " distance.").c_str())
                {
                    return parallelCalculator.computeDistance(query, dataSet, metric);
                };
}


TEST_CASE("MathEngine performance testing. (L1)", "[MathEngine]")
{
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Fatal);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Warning);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Debug);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Error);

    compareMathKernelMTMathKernel<int>(math::EDistanceMetric::L1);
}

TEST_CASE("MathEngine performance testing. (L2)", "[MathEngine]")
{
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Fatal);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Warning);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Debug);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Error);

    compareMathKernelMTMathKernel<int>(math::EDistanceMetric::L2);
}

TEST_CASE("MathEngine performance testing. (Hamming)", "[MathEngine]")
{
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Fatal);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Warning);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Debug);
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Error);

    compareMathKernelMTMathKernel<int>(math::EDistanceMetric::Hamming);
}
