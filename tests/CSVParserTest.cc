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
#include <random>
#include <thread>
#include <execution>


#include "CSVParser.h"
#include "CSVUtils.h"

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "CSVParserTestUtil.h"

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

    io::CSVParser parser (tmpFile.string());

    // Start testing.
    const auto matrix = io::util::loadFlatCSV<value_type>(parser, io::util::Execution::Par);

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

TEST_CASE( "Check CSVParser utils loadFlatCSV (Seq).", "[CSVParser]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    using value_type = float;

    static constexpr std::size_t rowCount = 1'00;
    static constexpr auto columnCount = rowCount;
    static constexpr auto tmpFileName = "tmpCSV.csv";
    static const std::filesystem::path tmpFile = tmpFileName;

    const auto target = createCSVFile<value_type>(tmpFile, ',', rowCount, columnCount);

    RAIIDeleter d { tmpFile };

    io::CSVParser parser { tmpFile.string() };

    const auto matrix = io::util::loadFlatCSV<value_type> (parser, io::util::Execution::Seq);;
    checkMatrix(matrix, target);
}

TEST_CASE( "Check CSVParser utils loadFlatCSV (Par).", "[CSVParser]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    using value_type = float;

    static constexpr std::size_t rowCount = 1'00;
    static constexpr auto columnCount = rowCount;
    static constexpr auto tmpFileName = "tmpCSV.csv";
    static const std::filesystem::path tmpFile = tmpFileName;

    const auto target = createCSVFile<value_type>(tmpFile, ',', rowCount, columnCount);

    RAIIDeleter d { tmpFile };

    io::CSVParser parser { tmpFile.string() };

    const auto matrix = io::util::loadFlatCSV<value_type> (parser, io::util::Execution::Par);;
    checkMatrix(matrix, target);
}

TEST_CASE( "Check CSVParser utils loadFlatCSV (Par2).", "[CSVParser]" )
{
    dbgh::CAssertConfig::Get().EnableAsserts(dbgh::EAssertLevel::Fatal);

    using value_type = float;

    static constexpr std::size_t rowCount = 1'00;
    static constexpr auto columnCount = rowCount;
    static constexpr auto tmpFileName = "tmpCSV.csv";
    static const std::filesystem::path tmpFile = tmpFileName;

    const auto target = createCSVFile<value_type>(tmpFile, ',', rowCount, columnCount);

    RAIIDeleter d { tmpFile };

    io::CSVParser parser { tmpFile.string() };

    const auto matrix = io::util::loadFlatCSV<value_type> (parser, io::util::Execution::Par2);;
    checkMatrix(matrix, target);
}
