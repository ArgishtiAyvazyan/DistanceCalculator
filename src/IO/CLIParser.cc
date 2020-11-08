/**
 * @file        CLIParser.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for cli::Parser.
 * @date        03-11-2020
 * @copyright   Copyright (c) 2020
 */

#include "CLIParser.h"

#include <sstream>
#include <iostream>
#include <string_view>


using namespace std::string_view_literals;

namespace
{
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
} // unnamed namespace

io::cli::Parser::Parser() noexcept
        : m_argc(0),
        m_argv(nullptr)
{ }

io::cli::Parser::Parser(int argc, char** argv, std::string description) noexcept
        : m_argc(argc),
        m_argv(argv),
        m_strApplicationDescription(std::move(description))
{ }

void io::cli::Parser::addOption(
        std::string optionName,
        const TValueRef& valueRef,
        TValue defaultValue, std::string description)
{
    ASSERT_ERROR(((callBackIndex == valueRef.index())
                            || (callBackWithArgumentIndex == valueRef.index())
                            || (requireIndex == defaultValue.index())
                            || (valueRef.index() == defaultValue.index()))
                 , "The value type is not equal to the default value type.");
    std::optional<TValue> defValue = std::nullopt;
    if (requireIndex != defaultValue.index())
    {
        defValue.emplace(std::move(defaultValue));
    }
    [[maybe_unused]] auto [option, inserted] = m_mapOptionNameToValueDefinition.emplace(std::move(optionName)
            , ValueDefinition { valueRef, std::move(defValue), std::move(description) });

    ASSERT_ERROR(inserted, ("The option is already registered: "s + option->first).c_str());
}

bool io::cli::Parser::parse()
{
    return parse(m_argc, m_argv);
}

bool io::cli::Parser::parse(int argc, char** argv)
{
    if ((2 == argc) && ("-help"sv == std::string_view {argv[1]}))
    {
        auto helpMessage = makeHelpMessage();
        std::cout << helpMessage << std::endl;

        // Exit
        return false;
    }

    for (int i = 1; i < argc; ++i)
    {
        auto search = m_mapOptionNameToValueDefinition.find(argv[i]);
        ASSERT_ERROR (search != std::end(m_mapOptionNameToValueDefinition),
                ("Invalid option: "s + argv[i]).c_str());

        ASSERT_ERROR (search->second.initialized == false
                      , ("The option is duplicated: "s + argv[i]).c_str());
        search->second.initialized = true;
        std::visit(overloaded {
                [&i, &argv](std::reference_wrapper<std::string>& out) { ++i; stringToValue (out, argv[i]); },
                [](std::reference_wrapper<bool>& out) { out.get() = true; },
                [&i, &argv](std::reference_wrapper<int>& out) { ++i; stringToValue (out, argv[i]); },
                [&i, &argv](std::reference_wrapper<double>& out) { ++i; stringToValue (out, argv[i]); },
                [](TCallBack& callBack) { callBack(); },
                [&i, &argv](TCallBackWithArgument& callBack) { ++i; callBack(argv[i]); },
        }, search->second.valueRef);

        search->second.initialized = true;
    }

    for (auto& [optionName, optionDefinition] : m_mapOptionNameToValueDefinition)
    {
        if (optionDefinition.initialized)
        {
            continue;
        }

        ASSERT_ERROR (optionDefinition.defaultValue.has_value()
                      , ("The "s + optionName + " option value is required."s).c_str());

        std::visit(overloaded {
            [def = &optionDefinition](std::reference_wrapper<std::string>& out) {
                    out.get() = std::get<std::string>(def->defaultValue.value());
                },
            [def = &optionDefinition](std::reference_wrapper<bool>& out) {
                    out.get() = std::get<bool>(def->defaultValue.value());
                },
            [def = &optionDefinition](std::reference_wrapper<int>& out) {
                    out.get() = std::get<int>(def->defaultValue.value());
                },
            [def = &optionDefinition](std::reference_wrapper<double>& out) {
                    out.get() = std::get<double>(def->defaultValue.value());
                },
            []( [[maybe_unused]] TCallBack& callBack) { },
            [def = &optionDefinition](TCallBackWithArgument& callBack) {
                    callBack(std::get<std::string>(def->defaultValue.value()));
                },
        }, optionDefinition.valueRef);

    }

    return true;
}

void io::cli::Parser::stringToValue(std::reference_wrapper<double>& out, const char* in)
{
    double value;
    std::istringstream ss { in };
    ss >> value;

    ASSERT_ERROR(!ss.fail(), ("The"s + in + " to double conversion is failed.").c_str());

    out.get() = value;
}

std::string io::cli::Parser::makeHelpMessage() const
{
    std::stringstream ssHelp;

    if (!m_strApplicationDescription.empty())
    {
        ssHelp << m_strApplicationDescription << std::endl << std::endl;
    }

    for (const auto&[optionName, optionDefinition] : m_mapOptionNameToValueDefinition)
    {
        ssHelp << optionName << ": " << optionDefinition.strDescription << std::endl << std::endl;
    }

    ssHelp << std::endl;
    return std::move(ssHelp).str();
}


void io::cli::Parser::stringToValue(std::reference_wrapper<int>& out, const char* in)
{
    int value;
    std::istringstream ss { in };
    ss >> value;

    ASSERT_ERROR(!ss.fail(), ("The"s + in + " to int conversion is failed.").c_str());

    out.get() = value;
}

void io::cli::Parser::stringToValue(std::reference_wrapper<std::string>& out, const char* in)
{
    out.get() = std::string(in);
}
