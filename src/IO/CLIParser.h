/**
 * @file        CLIParser.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for cli::Parser.
 * @date        03-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <map>
#include <variant>
#include <optional>
#include <string>
#include <functional>

#include <DBGHAssert.h>


namespace io::cli
{

using namespace std::string_literals;

namespace impl
{
struct required_t
{
    constexpr required_t() noexcept = default;
};
} // namespace impl

/**
 * @brief The required_t instance using for describe options for required.
 */
static constexpr impl::required_t required;

/**
 * @class   Parser
 * @brief   The command line argument parser.
 *
 * @details The helper class for parsing command line arguments.
 *          The parser support bool, int, double and string arguments parsing.
 */
class Parser
{
    using TCallBack = std::function<void()>;
    using TCallBackWithArgument = std::function<void(const std::string&)>;

    using TValueRef = std::variant<
            std::reference_wrapper<std::string>
            , std::reference_wrapper <bool>
            , std::reference_wrapper <int>
            , std::reference_wrapper <double>
            , TCallBack
            , TCallBackWithArgument>;

    using TValue = std::variant<std::string, bool, int, double, const impl::required_t>;

    static constexpr int32_t requireIndex = 4;

    /**
     * @internal
     * @struct  ValueDefinition
     * @brief   The structure contains information about option.
     */
    struct ValueDefinition
    {
        /**
         * @brief The reference to the out store.
         */
        TValueRef valueRef;

        /**
         * @brief The default value.
         */
        std::optional<TValue> defaultValue;

        /**
         * @brief The definition for option.
         */
        std::string strDescription;

        /**
         * @brief Flag for check the option is initialized or not.
         */
        bool initialized = false;
    };


public:

    Parser() noexcept;

    /**
     * @brief               Constructs and initializes parser object.
     * @param argc          Argument Count is int and stores number of command-line arguments passed
     *                          by the user including the name of the program.
     * @param argv          Argument Vector is array of character pointers listing all the arguments.
     * @param description   The application description.
     */
    Parser(int argc, char** argv, std::string description = {}) noexcept;

    /**
     * @brief               Registers a new command line option.
     *
     * @throw               \ref dbgh::CAssertException if the option is already registered.
     *
     * $throw               \ref dbgh::CAssertException If the value type and default value type does not match.
     *
     * @param optionName    The option name.
     * @param valueRef      The reference to the storage for returns option value or the call back function.
     * @param defaultValue  The default value. io::cli::required if this argument is required.
     * @param description   The description for option.
     */
    void addOption(std::string optionName, const TValueRef& valueRef, TValue defaultValue, std::string description);

    /**
     * @brief               Registers a new command line option. (The specialization for string.)
     *
     * @details             Helper function to avoid converting char * to bool.
     *
     * @throw               \ref dbgh::CAssertException if the option is already registered.
     *
     * $throw               \ref dbgh::CAssertException If the value type and default value type does not match.
     *
     * @param optionName    The option name.
     * @param valueRef      The reference to the storage for returns option value or the call back function.
     * @param defaultValue  The default value.
     * @param description   The description for option.
     */
    void addOption(std::string optionName, std::string& valueRef, const char* defaultValue, std::string description)
    {
        addOption(std::move (optionName), valueRef, std::string {defaultValue}, std::move(description));
    }

public:

    /**
     * @brief   Starts option parsing and  initializes all registered options.
     *
     * @note    If the command line argument is '-help' prints help massage and returns false.
     *
     * @throw   \ref dbgh::CAssertException if the option is not found.
     * @throw   \ref dbgh::CAssertException if the is option has duplicates.
     *
     * @return  Return false if option is -help, otherwise return true.
     */
    bool parse();

private:

    /**
     * @internal
     * @brief       Start option parsing.
     *
     * @throw       \ref dbgh::CAssertException if the option is not found.
     * @throw       \ref dbgh::CAssertException if the is option has duplicates.
     *
     * @param argc  The arguments count.
     * @param argv  The option array.
     * @return      Return false if option is -help, otherwise return true.
     */
    bool parse(int argc, char** argv);

    static void stringToValue(std::reference_wrapper<std::string>& out, const char* in);

    static void stringToValue(std::reference_wrapper<int>& out, const char* in);

    static void stringToValue(std::reference_wrapper<double>& out, const char* in);

private:

    /**
     * @brief   Makes the massage.
     * @return  The string contains the help massage.
     */
    [[nodiscard]] std::string makeHelpMessage() const;

private:

    /**
     * @brief Argument Count is int and stores number of command-line arguments passed
     *                          by the user including the name of the program.
     */
    int m_argc;

    /**
     * @brief Argument Vector is array of character pointers listing all the arguments.
     */
    char** m_argv;

    /**
     * @brief The application description.
     */
    std::string m_strApplicationDescription;


    /**
     * @brief Option name to option definition map.
     */
    std::map<std::string, ValueDefinition> m_mapOptionNameToValueDefinition;
};

} // namespace io:cli
