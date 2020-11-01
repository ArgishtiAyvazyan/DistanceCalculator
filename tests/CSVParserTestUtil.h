/**
 * @file        CSVParserTestUtil.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration the utilities for csv::Parser testing.
 * @date        31-10-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <filesystem>
#include <vector>
#include <fstream>

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
    std::string strRow;
    for (auto& arrRow : target)
    {
        strRow.reserve(1024);

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

            strRow += std::to_string(cell) + delimiter;
        }
        CSVOut << strRow << std::endl;
        strRow.clear();
    }
    CSVOut.close();

    return target;
}

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
