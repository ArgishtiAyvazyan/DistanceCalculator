/**
 * @file        Msg.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for io::Msg.
 * @date        07-11-2020
 * @copyright   Copyright (c) 2020
 */

#include "Msg.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

namespace io
{

enum class Color
{
    FG_RED [[maybe_unused]] = 31,
    FG_GREEN [[maybe_unused]] = 32,
    FG_YELLOW [[maybe_unused]] = 33,
    FG_BLUE [[maybe_unused]] = 34,
    FG_DEFAULT [[maybe_unused]] = 39,
    BG_RED [[maybe_unused]] = 41,
    BG_GREEN [[maybe_unused]] = 42,
    BG_BLUE [[maybe_unused]] = 44,
    BG_DEFAULT [[maybe_unused]] = 49
};

static int toColor(MsgType type)
{
    static const std::map<MsgType, Color> s_mapMessageColors
        {
                std::make_pair(MsgType::Info, Color::FG_GREEN)
                , std::make_pair(MsgType::Warning, Color::FG_BLUE)
                , std::make_pair(MsgType::Error, Color::FG_RED),
        };
    return static_cast<int>(s_mapMessageColors.at(type));
}

void Msg::Write(std::string_view svMessage, MsgType eMessageType)
{
    if (s_messagesDisabled)
    {
        return;
    }

    static const std::map<MsgType, std::string_view> s_mapMessageTypeName
        {
                std::make_pair(MsgType::Info, "Info")
                , std::make_pair(MsgType::Warning, "Warning")
                , std::make_pair(MsgType::Error, "Error")
        };

    const auto color = toColor(eMessageType);

    std::fstream fOut { logFileName.data(), std::ios_base::app };
    std::cout << "\033[4;" << color << "m";

    const auto& strLogType = s_mapMessageTypeName.at(eMessageType);
    std::cout << strLogType << "\t";
    fOut << strLogType << "\t";
    std::cout << "\033[0;" << color << "m";
    std::cout << " > " << " : " << svMessage << std::endl;
    fOut << " > " << " : " << svMessage << std::endl;
    std::cout << "\033[0m";
}

} // namespace io
