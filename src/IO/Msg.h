/**
 * @file        Msg.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       A file containing an interface for output.
 * @date        07-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <string_view>

namespace io
{

/**
 * @brief The types of messages.
 *          Depending on the type of message, it is processed differently.
 */
enum class MsgType : char
{
    Info, Warning, Error
};

/**
 * @class Msg
 * @brief A class that is a wrapper for console output methods.
 *          It also saves all the conclusions to the log file.
 */
class Msg
{

    static constexpr int s_iIndentLevel = 25;
    static constexpr std::string_view logFileName = "msg.log";
public:

    /**
     * @brief               Writes message in console and log file.
     *
     * @param message       Message for writing.
     * @param eMessageType  The message type. \n
     */
    static void Write(std::string_view message, MsgType eMessageType = MsgType::Info);

private:
};

} // namespace io
