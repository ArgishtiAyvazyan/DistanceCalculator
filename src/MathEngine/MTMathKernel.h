/**
 * @file        MTMathKernel.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for MTMathKernel.
 * @date        1-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <algorithm>
#include <execution>

#include <DBGHAssert.h>

#include "IMathKernel.h"
#include "MathUtils.h"


namespace math
{

/**
 * @class       MTMathKernel
 * @brief       The implementation for \ref IMathKernel.
 *
 * @details     The all algorithm execute parallel.
 * @tparam T    The value type.
 */
template<typename T>
class MTMathKernel : public IMathKernel<T>
{
public:
    using Matrix = typename IMathKernel<T>::Matrix;
    using MatrixView = typename IMathKernel<T>::MatrixView;

public:

    /**
     * @brief           Calculates and returns the distances between the specified vector sets.
     *
     * @param query     The first vector set.
     * @param dataSet   The second vector set.
     * @param metric    The distance metric type, \ref math::EDistanceMetric instance.
     * @return          Distances matrix, containing distances of each query vector from every dataSet vector.
     */
    Matrix computeDistance(MatrixView query, MatrixView dataSet, EDistanceMetric metric) const override;


private:
    Matrix computeDistance_L1(MatrixView query, MatrixView dataSet) const;

    Matrix computeDistance_L2(MatrixView query, MatrixView dataSet) const;

    Matrix computeDistance_Hamming(MatrixView query, MatrixView dataSet) const;


    /**
     * @internal
     * @brief           Genetic algorithm for computing distances using the given predicate.
     *
     * @details         The algorithm may be executed parallel.
     *
     * @tparam Fn       The predicate type.
     * @param query     The set of vectors.
     * @param dataSet   The set of vectors.
     * @param distance  The callable object for computing distance between given vectors. (predicate)
     * @return          Distances matrix, containing distances of each query vector from every dataSet vector.
     */
    template<typename Fn>
    static Matrix computeDistance(MatrixView query, MatrixView dataSet, Fn&& distance);
};

template<typename T>
auto MTMathKernel<T>::computeDistance(MatrixView query, MatrixView dataSet, EDistanceMetric metric) const -> Matrix
{
    switch (metric)
    {
        case EDistanceMetric::L1:
            return computeDistance_L1(query, dataSet);
        case EDistanceMetric::L2:
            return computeDistance_L2(query, dataSet);
        case EDistanceMetric::Hamming:
            return computeDistance_Hamming(query, dataSet);
        default:
            break;
    }
    ASSERT_FATAL(false, "Unexpected metric type.");
    return { };
}

template<typename T>
auto MTMathKernel<T>::computeDistance_L1(MatrixView query, MatrixView dataSet) const -> Matrix
{
    return computeDistance(query, dataSet, util::computeL1Distance<T>);
}

template<typename T>
auto MTMathKernel<T>::computeDistance_L2(MatrixView query, MatrixView dataSet) const -> Matrix
{
    return computeDistance(query, dataSet, util::computeL2Distance<T>);
}

template<typename T>
auto MTMathKernel<T>::computeDistance_Hamming(MatrixView query, MatrixView dataSet) const -> Matrix
{
    return computeDistance(query, dataSet, util::computeHammingDistance<T>);
}

template<typename T>
template<typename Fn>
auto MTMathKernel<T>::computeDistance(MatrixView query, MatrixView dataSet, Fn&& distance) -> Matrix
{
    const auto querySize = std::size(query);
    const auto dataSetSize = std::size(dataSet);
    auto result = IMathKernel<T>::createMatrix(querySize, dataSetSize);

    bool dimensionalError = false;
    auto handleQueryVector = [&](const auto& queryVec)
    {
        if (dimensionalError)
        {
            // Skip computing.
            return;
        }

        const auto queryIdx = static_cast<std::size_t>(&queryVec - &*std::begin(query));
        for (std::size_t dataSetIdx = 0; dataSetIdx < dataSetSize; ++dataSetIdx)
        {
            if (std::size(queryVec) != std::size(dataSet[dataSetIdx]))
            {
                dimensionalError = true;
                return;
            }

            const auto& dataSetVec = dataSet[dataSetIdx];
            result[queryIdx][dataSetIdx] = distance(queryVec, dataSetVec);
        }
    };

    std::for_each(std::execution::par, std::begin(query), std::end(query), handleQueryVector);

    ASSERT_ERROR(!dimensionalError, "The vector sizes is not equal, distance computation is impossible.");
    return result;
}

} // namespace math
