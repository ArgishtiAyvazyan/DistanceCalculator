/**
 * @file        Parser.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for Parser.
 * @date        30-10-2020
 * @copyright   Copyright (c) 2020
 */

#include "CSVParser.h"

#include <fstream>


namespace csv
{
Parser::Parser(std::string strCSVFileName)
    : m_strCSVFileName(std::move(strCSVFileName))
{
    ASSERT_DEBUG(!m_strCSVFileName.empty(), "The CSV file name cannot be empty, please input valid filename.");

    loadCSVFile();
}

std::size_t Parser::size() const noexcept
{
    return std::size(m_arrData);
}


void Parser::loadCSVFile()
{
    std::ifstream inputCSVFile{ m_strCSVFileName };
    ASSERT_ERROR(inputCSVFile.is_open(), "CSV file not exists.");


    std::string strRow;
    while (std::getline(inputCSVFile, strRow))
    {
        m_arrData.emplace_back( std::move(strRow) );
        strRow.clear();
    }
}

} // namespace csv
