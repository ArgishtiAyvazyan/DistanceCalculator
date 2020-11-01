/**
 * @file        CSVParserTest.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for Parser.
 * @date        31-10-2020
 * @copyright   Copyright (c) 2020
 */

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch.hpp>

#include "CSVParser.h"
#include "CSVUtils.h"

#include "CSVParserTestUtil.h"

TEST_CASE("csv::Parser performance testing.", "[Parser]")
{
    dbgh::CAssertConfig::Get().DisableAsserts(dbgh::EAssertLevel::Fatal);

    using value_type = float;

    static constexpr std::size_t rowCount = 1'00;
    static constexpr auto columnCount = rowCount;
    static constexpr auto tmpFileName = "tmpCSV.csv";
    static const std::filesystem::path tmpFile = tmpFileName;

    const auto target = createCSVFile<value_type>(tmpFile, ',', rowCount, columnCount);

    RAIIDeleter d { tmpFile };

    csv::Parser parser { tmpFile.string() };

    BENCHMARK("readCSV (Seq)")
    {
        return csv::util::loadFlatCSV<value_type> (parser, csv::util::Execution::Seq);
    };

    BENCHMARK("readCSV (Par)")
    {
        return csv::util::loadFlatCSV<value_type> (parser, csv::util::Execution::Par);
    };

    BENCHMARK("readCSV (Par2)")
    {
        return csv::util::loadFlatCSV<value_type> (parser, csv::util::Execution::Par2);
    };

}
