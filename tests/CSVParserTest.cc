/**
 * @file        CSVParserTest.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for CSVParser.
 * @date        31-10-2020
 * @copyright   Copyright (c) 2020
 */

#include <algorithm>
#include <fstream>
#include <vector>
#include <filesystem>
#include <random>
#include <thread>

#include "CSVParser.h"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

namespace
{

/**
 * @struct RAIIDeleter
 * @brief The helper class for save remove temporary files.
 */
struct RAIIDeleter
{
    ~RAIIDeleter()
    {
        if (std::filesystem::exists(m_filePath))
        {
            std::filesystem::remove(m_filePath);
        }
    }

    /**
     * @brief The file path.
     */
    std::filesystem::path m_filePath;
};

}


template<typename T>
auto createCSVFile (const std::filesystem::path& CSVFile,
                    const char delimiter,
                    size_t rowCount,
                    size_t columnCount
                    ) -> std::vector<std::vector<T>>
{
    using value_type = T;

    std::vector<std::vector<value_type>> target (rowCount, std::vector<value_type> (columnCount));

    std::fstream CSVOut { CSVFile, std::ios_base::app };
    std::default_random_engine re;
    for (auto& arrRow : target)
    {
        for (auto& cell : arrRow)
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                constexpr value_type lower_bound = -1'000'000;
                constexpr value_type upper_bound = 1'000'000;
                std::uniform_real_distribution<T> unif { lower_bound, upper_bound };
                cell = unif(re);
            }
            else
            {
                static constexpr auto lower_bound = std::numeric_limits<T>::min();
                static constexpr auto upper_bound = std::numeric_limits<T>::max();
                std::uniform_int_distribution<T> unif { lower_bound, upper_bound };
                cell = unif(re);
            }

            CSVOut << cell << delimiter;
        }
        CSVOut << std::endl;
    }
    CSVOut.close();

    return target;
}

template <typename T>
auto readCSV (const std::filesystem::path& CSVFile) -> std::vector<std::vector<T>>
{
    using value_type = T;

    io::CSVParser parser (CSVFile.string());

    std::vector<std::vector<value_type>> matrix;
    std::transform(std::cbegin(parser), std::cend(parser), std::back_inserter(matrix)
                   , [](const io::CSVRow& row)
            {
                typename decltype(matrix)::value_type arrRow;
                std::transform(std::cbegin(row), std::cend(row), std::back_inserter(arrRow)
                               , [](const io::CSVCell& cell)
                        {
                            return cell.get<value_type>();
                        });
                return arrRow;
            });

    return matrix;
}

template <typename T>
void checkMatrix(const std::vector<std::vector<T>>& matrix, const std::vector<std::vector<T>>& target)
{
    // TODO investigate big error reason.
    static constexpr double allowedError = 1.0f;

    using value_type = T;
    REQUIRE(matrix.size() == target.size());
    const auto rowCount = std::size(matrix);
    if (0 == rowCount)
    {
        return;
    }

    for (std::size_t row = 0; row < rowCount; ++row)
    {
        REQUIRE(matrix[row].size() == target[row].size());
        const auto columnCount = std::size(matrix[row]);
        for (std::size_t col = 0; col < columnCount; ++col)
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                REQUIRE(std::abs(matrix[row][col] - target[row][col]) < allowedError);
            }
            else
            {
                REQUIRE(matrix[row][col] == target[row][col]);
            }
        }
    }
}

template <typename T>
void readTestBody(const char delimiter = ',')
{
    static constexpr std::size_t rowCount = 1'0;
    static constexpr auto columnCount = rowCount;
    static constexpr auto tmpFileName = "tmpCSV.csv";
    static const std::filesystem::path tmpFile = tmpFileName;


    if (std::filesystem::exists(tmpFile))
    {
        std::filesystem::remove(tmpFile);
    }

    RAIIDeleter raii { tmpFileName };

    using value_type = T;

    const auto target = createCSVFile<value_type>(tmpFile, delimiter, rowCount, columnCount);

    // Start testing.
    const auto matrix = readCSV<value_type>(tmpFile);

    checkMatrix(matrix, target);
}


TEST_CASE( "Check CSVParser for float", "[CSVParser]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    readTestBody<float>(' ');
    readTestBody<float>(',');
    readTestBody<float>('\t');

}

TEST_CASE( "Check CSVParser for double", "[CSVParser]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    readTestBody<double>(' ');
    readTestBody<double>(',');
    readTestBody<double>('\t');
}

TEST_CASE( "Check CSVParser for int", "[CSVParser]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    readTestBody<int>(' ');
    readTestBody<int>(',');
    readTestBody<int>('\t');
}

TEST_CASE( "Check CSVParser for size_t", "[CSVParser]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    readTestBody<size_t>(' ');
    readTestBody<size_t>(',');
    readTestBody<size_t>('\t');
}


template <typename T>
auto readCSV_MT (const std::filesystem::path& CSVFile) -> std::vector<std::vector<T>>
{
    using value_type = T;
    io::CSVParser parser { CSVFile.string() };

    const auto length = std::size(parser);

    const uint32_t min_per_thread = 25;
    const uint32_t max_threads = ( length + min_per_thread - 1 ) / min_per_thread;

    const uint32_t hardware_threads = std::thread::hardware_concurrency();

    const uint32_t num_threads = std::min( hardware_threads != 0 ? hardware_threads : 2, max_threads );

    const auto block_size = length / num_threads;

    std::vector<std::vector<value_type>> arrResults( length );
    std::vector<std::thread>  threads( num_threads - 1 );

    auto block_start = std::cbegin(parser);

    auto readCSVRows = [&arrResults, &parser] (
            io::CSVParser::const_iterator first,
            io::CSVParser::const_iterator last)
    {
        auto rowIndex = std::distance (std::begin (parser), first);
        for (auto it = first; it != last; ++it)
        {
            const io::CSVRow& row = *it;
            for (const io::CSVCell& cell : row)
            {
                arrResults[rowIndex].push_back(cell.get<value_type>());
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

    std::for_each( std::begin(threads), std::end(threads),
            std::mem_fn( &std::thread::join ) );

    return arrResults;
}

TEST_CASE( "Check CSVParser for concurrent reading.", "[CSVParser]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    using value_type = float;

    static constexpr std::size_t rowCount = 1'000;
    static constexpr auto columnCount = rowCount;
    static constexpr auto tmpFileName = "tmpCSV.csv";
    static const std::filesystem::path tmpFile = tmpFileName;

    const auto target = createCSVFile<value_type>(tmpFile, ',', rowCount, columnCount);

    RAIIDeleter d { tmpFile };

    const auto matrix = readCSV_MT<value_type> (tmpFile);

    checkMatrix(matrix, target);
}
