/**
 * @file        CSVParser.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for CSVParser.
 * @date        30-10-2020
 * @copyright   Copyright (c) 2020
 */

#include "CSVParser.h"

#include <fstream>


namespace io
{
    CSVParser::CSVParser(std::string strCSVFileName)
        : m_strCSVFileName(std::move(strCSVFileName))
    {
        ASSERT_DEBUG(!m_strCSVFileName.empty(), "The CSV file name cannot be empty, please input valid filename.");

        loadCSVFile();
    }

    void CSVParser::loadCSVFile()
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


} // namespace io
