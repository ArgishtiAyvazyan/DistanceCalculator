/**
 * @file        MathUtils.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration the utilities for MathEngine.
 * @date        01-11-2020
 * @copyright   Copyright (c) 2020
 */


#pragma once

#include <cmath>

#include "IMathKernel.h"
#include <DBGHAssert.h>


namespace math::util
{

/**
 * @brief           Computes the distance between two vectors using L1 metric.
 *
 * @tparam T        The value type.
 * @param first     The first vector.
 * @param second    The second vector.
 * @return          The computed distance.
 */
template<typename T>
T computeL1Distance(
        const typename IMathKernel<T>::Matrix::value_type& first,
        const typename IMathKernel<T>::Matrix::value_type& second)
{
    ASSERT_ERROR(std::size(first) == std::size(second)
                 , "The vector sizes is not equal, distance computation is impossible.");
    const auto vectorSize = std::size(first);
    T distance = 0;
    for (std::size_t vecIdx = 0; vecIdx < vectorSize; ++vecIdx)
    {
        distance += std::abs(first[vecIdx] - second[vecIdx]);
    }
    return distance;
}

/**
 * @brief           Computes the distance between two vectors using L2 metric.
 *
 * @tparam T        The value type.
 * @param first     The first vector.
 * @param second    The second vector.
 * @return          The computed distance.
 */
template<typename T>
T computeL2Distance(
        const typename IMathKernel<T>::Matrix::value_type& first,
        const typename IMathKernel<T>::Matrix::value_type& second)
{
    ASSERT_ERROR(std::size(first) == std::size(second)
                 , "The vector sizes is not equal, distance computation is impossible.");

    const auto vectorSize = std::size(first);
    long double distance = 0;
    for (std::size_t vecIdx = 0; vecIdx < vectorSize; ++vecIdx)
    {
        const auto diff = (first[vecIdx] - second[vecIdx]);
        distance += static_cast<long double>(diff * diff);
    }
    return static_cast<T>(std::sqrt(distance));
}

/**
 * @brief           Computes the distance between two vectors using Hamming metric.
 *
 * @tparam T        The value type.
 * @param first     The first vector.
 * @param second    The second vector.
 * @return          The computed distance.
 */
template<typename T>
T computeHammingDistance(
        const typename IMathKernel<T>::Matrix::value_type& first,
        const typename IMathKernel<T>::Matrix::value_type& second)
{
    ASSERT_ERROR(std::size(first) == std::size(second)
                 , "The vector sizes is not equal, distance computation is impossible.");
    const auto vectorSize = std::size(first);
    T distance = 0;
    for (std::size_t vecIdx = 0; vecIdx < vectorSize; ++vecIdx)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            static constexpr auto allowedError = static_cast<T>(.000001L);
            if (std::fabs(first[vecIdx] - second[vecIdx]) > allowedError)
            {
                ++distance;
            }
        }
        else
        {
            if (first[vecIdx] != second[vecIdx])
            {
                ++distance;
            }
        }
    }
    return distance;
}

} // namespace math::util

