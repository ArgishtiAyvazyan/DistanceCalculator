/**
 * @file        Cell.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for Cell.
 * @date        30-10-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <string_view>

namespace csv
{

/**
 * @class Cell
 * @brief The C++ representation for CSV cell.
 */
class Cell
{
public:

    /**
     * @brief Construct and initialize Cell object.
     * @param strCell The string_view to the string representation of CSV cell.
     */
    explicit Cell(std::string_view strCell);

    /**
     * @brief Gets the call value for the given type.
     *
     * @example Cell cell = ...
     *          const auto value = cell.get<float>();
     *
     * @throws If dbgh::CAssertException If the value to TRet conversion fails.
     *
     * @tparam TRet The The value type, available all C++ fundamental types, std::string and std::string_view.
     * @return Returns the cell value.
     */
    template<typename TRet>
    [[nodiscard]] TRet get() const;

private:
    std::string_view m_strCell;
}; // class Cell


extern template short Cell::get<short>() const;
extern template unsigned short Cell::get<unsigned short>() const;
extern template int Cell::get<int>() const;
extern template unsigned Cell::get<unsigned>() const;
extern template long Cell::get<long>() const;
extern template unsigned long Cell::get<unsigned long>() const;
extern template long long Cell::get<long long>() const;
extern template unsigned long long Cell::get<unsigned long long>() const;
extern template float Cell::get<float>() const;
extern template double Cell::get<double>() const;
extern template long double Cell::get<long double>() const;
extern template std::string Cell::get<std::string>() const;
extern template std::string_view Cell::get<std::string_view>() const;

} // namespace csv
