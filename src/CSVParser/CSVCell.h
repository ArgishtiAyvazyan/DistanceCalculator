/**
 * @file        CSVCell.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for CSVCell.
 * @date        30-10-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <string_view>

namespace io
{

/**
 * @class CSVCell
 * @brief The C++ representation for CSV cell.
 */
class CSVCell
{
public:

    /**
     * @brief Construct and initialize CSVCell object.
     * @param strCell The string_view to the string representation of CSV cell.
     */
    explicit CSVCell(std::string_view strCell);

    /**
     * @brief Gets the call value for the given type.
     *
     * @example CSVCell cell = ...
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
}; // class CSVCell


extern template short CSVCell::get<short>() const;
extern template unsigned short CSVCell::get<unsigned short>() const;
extern template int CSVCell::get<int>() const;
extern template unsigned CSVCell::get<unsigned>() const;
extern template long CSVCell::get<long>() const;
extern template unsigned long CSVCell::get<unsigned long>() const;
extern template long long CSVCell::get<long long>() const;
extern template unsigned long long CSVCell::get<unsigned long long>() const;
extern template float CSVCell::get<float>() const;
extern template double CSVCell::get<double>() const;
extern template long double CSVCell::get<long double>() const;
extern template std::string CSVCell::get<std::string>() const;
extern template std::string_view CSVCell::get<std::string_view>() const;

} // namespace io
