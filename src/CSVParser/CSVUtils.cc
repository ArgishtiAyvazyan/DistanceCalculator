/**
 * @file        CSVUtils.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation of the utilities for CSVParser.
 * @date        31-10-2020
 * @copyright   Copyright (c) 2020
 */

#include <algorithm>
#include <execution>
#include <iterator>
#include <functional>
#include <thread>

#include <DBGHAssert.h>

#include "CSVUtils.h"
#include "CSVParser.h"

namespace
{

/**
 * @internal
 * @brief Reads and returns CSV file data.
 *
 * @tparam  T The CSV file data type.
 * @param   parser The source parser.
 * @return  The table contains the CSV file data.
 */
template <typename T>
auto readCSV (const io::CSVParser& parser) -> io::util::Table<T>
{
    using value_type = T;

    io::util::Table<T> table;
    std::transform(std::cbegin(parser), std::cend(parser), std::back_inserter(table)
                   , [](const io::CSVRow& row)
            {
                typename decltype(table)::value_type arrRow;
                std::transform(std::cbegin(row), std::cend(row), std::back_inserter(arrRow)
                               , [](const io::CSVCell& cell)
                        {
                            return cell.get<value_type>();
                        });
                return arrRow;
            });

    return table;
}

/**
 * @internal
 * @brief Reads and returns CSV file data. (execution may be parallelized)
 *
 * @tparam  T The CSV file data type.
 * @param   parser The source parser.
 * @return  The table contains the CSV file data.
 */
template <typename T>
auto readCSV_MT (const io::CSVParser& parser) -> io::util::Table<T>
{
    using value_type = T;

    io::util::Table<T> table (std::size(parser));

    std::for_each(std::execution::par, std::begin(parser), std::end(parser)
                  , [&](const io::CSVRow& row)
            {
                const std::size_t index = (&row - &*std::begin(parser));
                for (const auto& cell : row)
                {
                    table[index].push_back(cell.get<value_type>());
                }
            });

    return table;
}

/**
 * @internal
 * @brief Reads and returns CSV file data. (execution may be parallelized)
 *
 * @tparam  T The CSV file data type.
 * @param   parser The source parser.
 * @return  The table contains the CSV file data.
 */
template <typename T>
auto readCSV_MT2 (const io::CSVParser& parser) -> io::util::Table<T>
{
    using value_type = T;

    const auto length = std::size(parser);

    const uint32_t min_per_thread = 25;
    const uint32_t max_threads = ( length + min_per_thread - 1 ) / min_per_thread;
    const uint32_t hardware_threads = std::thread::hardware_concurrency();
    const uint32_t num_threads = std::min( hardware_threads != 0 ? hardware_threads : 2, max_threads );
    const auto block_size = length / num_threads;

    io::util::Table<value_type> table (length);
    std::vector<std::thread>  threads( num_threads - 1 );

    auto block_start = std::cbegin(parser);

    auto readCSVRows = [&table, &parser] (
            io::CSVParser::const_iterator first,
            io::CSVParser::const_iterator last)
    {
        auto rowIndex = std::distance (std::begin (parser), first);
        for (auto it = first; it != last; ++it)
        {
            const io::CSVRow& row = *it;
            for (const io::CSVCell& cell : row)
            {
                table[rowIndex].push_back(cell.get<value_type>());
            }
            ++rowIndex;
        }
    };

    for ( unsigned long i = 0; i < ( num_threads - 1 ); ++i )
    {
        auto block_end = block_start;
        std::advance( block_end, block_size );
        threads[i] = std::thread( readCSVRows, block_start, block_end );
        block_start = block_end;
    }

    readCSVRows( block_start, std::end (parser));

    std::for_each( std::begin(threads), std::end(threads)
            , std::mem_fn( &std::thread::join ) );

    return table;
}

} // unnamed namespace


namespace io::util
{
template <typename T>
[[maybe_unused]] auto loadFlatCSV(const CSVParser& parser, const Execution exec) -> Table<T>
{
    switch (exec)
    {
        case Execution::Seq:
            return readCSV<T> (parser);
        case Execution::Par:
            return readCSV_MT<T> (parser);
        case Execution::Par2:
            return readCSV_MT2<T>(parser);
        default:
            ASSERT_DEBUG(false, "An unexpected enumeration type.");
    }
    return { };
}

template Table<short> loadFlatCSV<short>(const io::CSVParser&, const Execution);
template Table<unsigned short> loadFlatCSV<unsigned short>(const io::CSVParser&, const Execution);
template Table<int> loadFlatCSV<int>(const io::CSVParser&, const Execution);
template Table<unsigned> loadFlatCSV<unsigned>(const io::CSVParser&, const Execution);
template Table<long> loadFlatCSV<long>(const io::CSVParser&, const Execution);
template Table<unsigned long> loadFlatCSV<unsigned long>(const io::CSVParser&, const Execution);
template Table<long long> loadFlatCSV<long long>(const io::CSVParser&, const Execution);
template Table<unsigned long long> loadFlatCSV<unsigned long long>(const io::CSVParser&, const Execution);
template Table<float> loadFlatCSV<float>(const io::CSVParser&, const Execution);
template Table<double> loadFlatCSV<double>(const io::CSVParser&, const Execution);
template Table<long double> loadFlatCSV<long double>(const io::CSVParser&, const Execution);
template Table<std::string> loadFlatCSV<std::string>(const io::CSVParser&, const Execution);
template Table<std::string_view> loadFlatCSV<std::string_view>(const io::CSVParser&, const Execution);

} // namespace io
