/**
 * @file        Main.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for main function.
 * @date        07-11-2020
 * @copyright   Copyright (c) 2020
 */

#include "Msg.h"
#include "MainApplication.h"

namespace
{

/**
 * @internal
 * @class   AssertExecutor
 * @brief   The custom executor for DBGH_ASSERT library.
 */
class AssertExecutor : public dbgh::CHandlerExecutor
{
public:
    /**
     * @brief           Override the Log method, disable logging.
     * @param message   The log message.
     */
    void Logs([[maybe_unused]] std::string_view message) override
    {
    }
};

} // unnamed namespace

int main(int argc, char** argv)
try
{
    // Sets the new executor.
    dbgh::CAssertConfig::Get().SetExecutor(std::make_unique<AssertExecutor>());

    dc::MainApplication app { argc, argv };
    return app.run();
}
catch (const std::exception& e)
{
    io::Msg::Write(e.what(), io::MsgType::Error);
    return -2;
}
catch (...)
{
    io::Msg::Write("Unknown error.", io::MsgType::Error);
    return -3;
}
