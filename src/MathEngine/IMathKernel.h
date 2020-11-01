/**
 * @file        IMathKernel.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for IMathKernel.
 * @date        1-11-2020
 * @copyright   Copyright (c) 2020
 */


#pragma once

#include <span>
#include <vector>


namespace math
{

/**
 * @brief The available type of distance metrics.
 */
enum class EDistanceMetric
{
    /**
     * @brief Compute Taxicab geometry distance.
     *
     * @details The distance between two points is the sum of the absolute differences
     *          of their Cartesian coordinates
     *          https://en.wikipedia.org/wiki/Taxicab_geometry
     */
    L1,

    /**
     * @brief Compute Euclidean distance distance.
     *
     * @details The Euclidean distance between two points in Euclidean space is a number
     *          , the length of a line segment between the two points.
     *          https://en.wikipedia.org/wiki/Euclidean_distance
     */
    L2,

    /**
     * @brief   Compute Hamming distance.
     *
     * @details In information theory, the Hamming distance between two strings of equal
     *          length is the number of positions at which the corresponding symbols are different.
     *          https://en.wikipedia.org/wiki/Hamming_distance
     */
    Hamming
};

/**
 * @class IMathKernel
 * @brief The math kernel interface for distance computing.
 *
 * @tparam T The value type.
 */
template <typename T>
class IMathKernel
{
public:

    using Matrix = std::vector<std::vector<T>>;
//    using MatrixView = std::span<std::span<T>>;
    using MatrixView = const Matrix&;

public:

    IMathKernel() = default;
    IMathKernel(const IMathKernel<T>&) = default;
    IMathKernel(IMathKernel<T>&&) noexcept = default;
    IMathKernel& operator=(const IMathKernel<T>&) = default;
    IMathKernel& operator=(IMathKernel<T>&&) noexcept = default;
    virtual ~IMathKernel() = default;

public:

    /**
     * @brief               Creates a matrix with the specified dimensions.
     *
     * @param rowCount      The row count.
     * @param columnCount   The column count.
     * @return              The created matrix.
     */
    static Matrix createMatrix (std::size_t rowCount, std::size_t columnCount);

    /**
     * @brief           Calculates and returns the distances between the specified vector set.
     *
     * @param query     The first vector set.
     * @param dataSet   The second vector set.
     * @param metric    The distance metric type, \ref math::EDistanceMetric instance.
     * @return          Distances matrix, containing distances of each query vector from every dataSet vector.
     */
    virtual Matrix computeDistance(MatrixView query, MatrixView dataSet, EDistanceMetric metric) const = 0;

}; // class IDistanceCalculator

template<typename T>
auto IMathKernel<T>::createMatrix(std::size_t rowCount, std::size_t columnCount) -> Matrix
{
    return Matrix(rowCount, typename Matrix::value_type (columnCount));
}


} // namespace math
