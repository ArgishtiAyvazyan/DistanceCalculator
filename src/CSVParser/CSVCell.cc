/**
 * @file        Cell.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for Cell.
 * @date        30-10-2020
 * @copyright   Copyright (c) 2020
 */

#include <charconv>
#include <sstream>

#include <DBGHAssert.h>

#include "CSVCell.h"

namespace
{
/**
 * @brief Compile-time check for exists std::from_chars algorithm for given T type.
 * @tparam T The std::from_chars out value type.
 */
template<typename T, typename = void>
struct ExistsFromChars : std::false_type
{};

/**
 * @brief Compile-time check for exists std::from_chars algorithm for given T type.
 * @tparam T The std::from_chars out value type.
 */
template<typename T>
struct ExistsFromChars<T
        , std::enable_if_t<!std::is_same_v<decltype(
                std::from_chars(std::declval<char* const>(), std::declval<char* const>(), std::declval<T&>()))
                , void>>> : std::true_type
{};

/**
 * @brief Helper variable template for ExistsFromChars.
 * @tparam T The std::from_chars out value type.
 */
template<typename T>
constexpr bool ExistsFromChars_v = ExistsFromChars<T>::value;

} // unnamed namespace


namespace csv
{


Cell::Cell(std::string_view strCell)
        : m_strCell(strCell) { }

template<typename TRet>
TRet Cell::get() const
{
    if constexpr (std::is_same_v<TRet, std::string>)
    {
        return { std::begin(m_strCell), std::end(m_strCell) };
    }
    else if constexpr (std::is_same_v<TRet, std::string_view>)
    {
        return m_strCell;
    }
    else if constexpr (ExistsFromChars_v<TRet>) // Use std::from_chars whenever possible.
    {
        TRet result;
        auto[p, ec] = std::from_chars(m_strCell.data(), m_strCell.data() + m_strCell.size(), result);
        ASSERT_ERROR(ec == std::errc(), "Type mismatch, cannot convert.");
        return result;
    }
    else // stdlib does not support std::from_chars so we use std::istringstream.
    {
        std::istringstream iss { std::string { m_strCell.begin(), m_strCell.end() } };
        TRet value;
        iss >> value;
        ASSERT_ERROR(!iss.fail(), "Type mismatch, cannot convert.");
        return value;
    }
}

template short Cell::get<short>() const;
template unsigned short Cell::get<unsigned short>() const;
template int Cell::get<int>() const;
template unsigned Cell::get<unsigned>() const;
template long Cell::get<long>() const;
template unsigned long Cell::get<unsigned long>() const;
template long long Cell::get<long long>() const;
template unsigned long long Cell::get<unsigned long long>() const;
template float Cell::get<float>() const;
template double Cell::get<double>() const;
template long double Cell::get<long double>() const;
template std::string Cell::get<std::string>() const;
template std::string_view Cell::get<std::string_view>() const;
} // namespace csv
