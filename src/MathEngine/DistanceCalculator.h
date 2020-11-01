/**
 * @file        DistanceCalculator.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for DistanceCalculator.
 * @date        1-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <memory>
#include <DBGHAssert.h>

#include "IMathKernel.h"


namespace math
{

/**
 * @class       DistanceCalculator
 * @brief       The math engine for computing distances between two vector sets.
 *
 * @tparam T    The value type.
 */
template <typename T>
class DistanceCalculator
{
    using MathKernel = IMathKernel<T>;

public:
    using Matrix = typename MathKernel::Matrix;
    using MatrixView = typename MathKernel::MatrixView;

public:
    DistanceCalculator() = delete;
    DistanceCalculator(const DistanceCalculator&) = delete;
    DistanceCalculator& operator=(const DistanceCalculator&) = delete;

    DistanceCalculator(DistanceCalculator&&) noexcept = default;
    DistanceCalculator& operator=(DistanceCalculator&&) noexcept = default;
    ~DistanceCalculator() = default;

    /**
     * @brief           Constructs and initializes new engine.
     *
     * @param kernel    The math engine kernel unique pointer.
     */
    explicit DistanceCalculator(std::unique_ptr<MathKernel>&& kernel)
        : m_pMathKernel(std::move(kernel))
    { }

    /**
     * @brief           Calculates and returns the distances between the specified vector sets.
     *
     * @param query     The first vector set.
     * @param dataSet   The second vector set.
     * @param metric    The distance metric type, \ref math::EDistanceMetric instance.
     * @return          Distances matrix, containing distances of each query vector from every dataSet vector.
     */
    Matrix computeDistance(MatrixView query, MatrixView dataset, EDistanceMetric metric) const;
private:

    /**
     * @brief The pointer to the engine kernel.
     */
    std::unique_ptr<MathKernel> m_pMathKernel;
}; // class DistanceCalculator

template<typename T>
auto DistanceCalculator<T>::computeDistance(MatrixView query, MatrixView dataset, EDistanceMetric metric) const ->Matrix
{
    ASSERT_DEBUG(nullptr != m_pMathKernel, "Math kernel can not be null.");
    return m_pMathKernel->computeDistance(query, dataset, metric);
}

} // namespace math
