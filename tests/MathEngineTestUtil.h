/**
 * @file        MathEngineTestUtil.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration the utilities for MathEngine testing.
 * @date        2-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <catch2/catch.hpp>

#include "DistanceCalculator.h"
#include "MathKernel.h"

template <typename T>
auto crateTestQuery(size_t size) -> typename math::IMathKernel<T>::Matrix
{
    using value_type = T;
    auto result = math::IMathKernel<value_type>::createMatrix(size, size);

    std::default_random_engine re;
    for (auto& row : result)
    {
        for (auto& item : row)
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                constexpr value_type lower_bound = -1'000'000;
                constexpr value_type upper_bound = 1'000'000;
                std::uniform_real_distribution<T> unif { lower_bound, upper_bound };
                item = unif(re);
            }
            else
            {
                static constexpr auto lower_bound = std::numeric_limits<T>::min();
                static constexpr auto upper_bound = std::numeric_limits<T>::max();
                std::uniform_int_distribution<T> unif { lower_bound, upper_bound };
                item = unif(re);
            }
        }
    }

    return result;
}


template <typename T>
void checkDistanceBetweenEqualVectors(math::EDistanceMetric metric)
{
    using value_type = T;
    using Matrix = typename math::IMathKernel<value_type>::Matrix;

    const Matrix matrix {
            { 1, 1, 1 },
            { 1, 1, 1 },
            { 1, 1, 1 },
    };

    math::DistanceCalculator<value_type> calculator { std::make_unique<math::MathKernel<value_type>>() };

    Matrix result = calculator.computeDistance(matrix, matrix, metric);

    // Matrix is symmetric.
    const auto size = std::size(result);
    for (size_t r = 0; r < size; ++r)
    {
        REQUIRE(std::size(result) == size);
        for (size_t c = 0; c < size; ++c)
        {
            REQUIRE(0 == result[c][r]);
        }
    }

    // Diagonal line is zero.
    for (size_t i = 0; i < size; ++i)
    {
        REQUIRE(0 == result[i][i]);
    }
}


template <typename T>
void checkDistanceBetweenEqualMatrices(math::EDistanceMetric metric)
{
    using value_type = T;
    using Matrix = typename math::IMathKernel<value_type>::Matrix;

    const auto matrix = crateTestQuery<value_type>(153);

    math::DistanceCalculator<value_type> calculator { std::make_unique<math::MathKernel<value_type>>() };

    Matrix result = calculator.computeDistance(matrix, matrix, metric);

    // Matrix is symmetric.
    const auto size = std::size(result);
    for (size_t r = 0; r < size; ++r)
    {
        REQUIRE(std::size(result) == size);
        for (size_t c = r + 1; c < size; ++c)
        {
            REQUIRE(result[r][c] == result[c][r]);
        }
    }

    // Diagonal line is zero.
    for (size_t i = 0; i < size; ++i)
    {
        REQUIRE(0 == result[i][i]);
    }
}
