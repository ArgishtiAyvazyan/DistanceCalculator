/**
 * @file        CSVParserTest.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Tests implementation for CLIParser.
 * @date        03-11-2020
 * @copyright   Copyright (c) 2020
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "CLIParser.h"

TEST_CASE( "Check cli::Parser for float", "[cli::Parser]" )
{
    std::string strValue;

     std::string arr[] { "executable", "-str", "stringValue", "-bool", "-intRequired", "12", "-callArg", "callValue", "-call" };

    char** argc = new char*[std::size(arr)];

    for (std::size_t i = 0; i < std::size(arr); ++i)
    {
        argc[i] = arr[i].data();
    }

    io::cli::Parser parser (std::size(arr), argc);


    parser.addOption("-str", strValue, "default", "This is string.");

    bool boolValue = false;
    parser.addOption("-bool", boolValue, false, "This is bool.");

    int intValue;
    parser.addOption("-int", intValue, 12, "This is int.");

    int intValueRequired;
    parser.addOption("-intRequired", intValueRequired, io::cli::required, "This is int2.");


    bool calledArg = false;
    parser.addOption("-callArg", [&calledArg](const auto& str)
        {
            REQUIRE(str == "callValue");
            calledArg = true;
        }
        , io::cli::required, "This is call back.");

    bool called = false;
    parser.addOption("-call", [&called]()
        {
            called = true;
        }
        , io::cli::required, "This is call back.");


    parser.parse();

    REQUIRE(strValue == "stringValue");
    REQUIRE(boolValue == true);
    REQUIRE(intValue == 12);
    REQUIRE(intValueRequired == 12);
    REQUIRE(calledArg == true);
    REQUIRE(called == true);
}


