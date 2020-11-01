/**
 * @file        MathKernel.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for MathKernel.
 * @date        1-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <DBGHAssert.h>

#include "IMathKernel.h"
#include "MathUtils.h"


namespace math
{

/**
 * @class       MathKernel
 * @brief       The implementation for \ref IMathKernel.
 *
 * @details     The all algorithm execute sequential.
 * @tparam T    The value type.
 */
template<typename T>
class MathKernel : public IMathKernel<T>
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
auto MathKernel<T>::computeDistance(MatrixView query, MatrixView dataSet, EDistanceMetric metric) const -> Matrix
{
    switch (metric)
    {
        case EDistanceMetric::L1:
            return computeDistance_L1(query, dataSet);
        case EDistanceMetric::L2:
            return computeDistance_L2(query, dataSet);
        case EDistanceMetric::Hamming:
            return computeDistance_Hamming(query, dataSet);
    }
    ASSERT_FATAL(false, "Unexpected metric type.");
    return { };
}

template<typename T>
auto MathKernel<T>::computeDistance_L1(MatrixView query, MatrixView dataSet) const -> Matrix
{
    return computeDistance(query, dataSet, util::computeL1Distance<T>);
}

template<typename T>
auto MathKernel<T>::computeDistance_L2(MatrixView query, MatrixView dataSet) const -> Matrix
{
    return computeDistance(query, dataSet, util::computeL2Distance<T>);
}

template<typename T>
auto MathKernel<T>::computeDistance_Hamming(MatrixView query, MatrixView dataSet) const -> Matrix
{
    return computeDistance(query, dataSet, util::computeHammingDistance<T>);
}

template<typename T>
template<typename Fn>
auto MathKernel<T>::computeDistance(MatrixView query, MatrixView dataSet, Fn&& distance) -> Matrix
{
    const auto querySize = std::size(query);
    const auto dataSetSize = std::size(dataSet);
    auto result = IMathKernel<T>::createMatrix(querySize, dataSetSize);

    for (std::size_t queryIdx = 0; queryIdx < querySize; ++queryIdx)
    {
        for (std::size_t dataSetIdx = 0; dataSetIdx < dataSetSize; ++dataSetIdx)
        {
            ASSERT_ERROR(std::size(query[queryIdx]) == std::size(dataSet[dataSetIdx])
                         , "The vector sizes is not equal, distance computation is impossible.");

            const auto& queryVec = query[queryIdx];
            const auto& dataSetVec = dataSet[dataSetIdx];
            result[queryIdx][dataSetIdx] = distance(queryVec, dataSetVec);
        }
    }

    return result;
}

} // namespace math
