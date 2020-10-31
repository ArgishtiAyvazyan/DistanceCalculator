/**
 * @file        CSVUtils.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration the utilities for CSVParser.
 * @date        31-10-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <vector>

namespace io
{
class CSVParser;
} // namespace io


namespace io::util
{

template <typename T>
using Table = std::vector<std::vector<T>>;

/**
 * @brief The execution policy type.
 */
enum class Execution
{
    /**
     * @brief Execution policy type used to indicate that an algorithm’s execution not parallelized.
     */
    Seq,

    /**
     * @brief Execution policy type used to indicate that an algorithm’s execution may be parallelized.
     */
    Par,

    /**
     * @brief Execution policy type used to indicate that an algorithm’s execution parallelized.
     */
    Par2
};

/**
 * @brief   Helper function for quickly reading CSV files that contain data of the same type.
 *
 * @tparam  T The CSV file data type.
 * @param   parser The source parser.
 * @param   exec The execution policy.
 * @return  The table contains the CSV file data.
 */
template <typename T>
Table<T> loadFlatCSV(const io::CSVParser& parser, io::util::Execution exec);

} // namespace io::util
