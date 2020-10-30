/**
 * @file        CSVRow.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for CSVRow.
 * @date        30-10-2020
 * @copyright   Copyright (c) 2020
 */
#include <Assert.h>

#include "CSVRow.h"

namespace io
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The CSVRow::const_iterator class implementation.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSVRow::const_iterator::const_iterator(std::shared_ptr<std::string> pRowString, std::size_t lastPos)
        : m_pRowString(std::move(pRowString)),
        m_iLastPosition(lastPos)
{ }

auto CSVRow::const_iterator::operator++() -> const_iterator&
{
    ASSERT_ERROR(nullptr != m_pRowString, "The iterator is invalid.");
    ASSERT_ERROR(m_iLastPosition < m_pRowString->size(), "The iterator is invalid.");
    const auto endOfCell = m_pRowString->find_first_of("\t ,", m_iLastPosition);

    m_iLastPosition = (std::string::npos == endOfCell)
                      ? std::size(*m_pRowString)
                      : endOfCell + 1;

    return *this;
}

auto CSVRow::const_iterator::operator++(int) -> const_iterator
{
    auto tmp { *this };
    ++(*this);
    return tmp;
}

bool CSVRow::const_iterator::operator==(const CSVRow::const_iterator& other) const noexcept
{
    return (m_iLastPosition == other.m_iLastPosition) && (m_pRowString == other.m_pRowString);
}

bool CSVRow::const_iterator::operator!=(const CSVRow::const_iterator& other) const noexcept
{
    return !(*this == other);
}

auto CSVRow::const_iterator::operator*() const -> CSVCell
{
    ASSERT_ERROR(nullptr != m_pRowString, "The iterator is invalid.");
    ASSERT_ERROR(m_iLastPosition < m_pRowString->size(), "The iterator is invalid.");

    const auto endOfCell = m_pRowString->find_first_of("\t ,", m_iLastPosition);

    const auto strCell = (std::string::npos == endOfCell)
                         ? std::string_view { *m_pRowString }.substr(m_iLastPosition)
                         : std::string_view { *m_pRowString }.substr(m_iLastPosition, endOfCell - m_iLastPosition);

    return CSVCell { strCell };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The CSVRow class implementation.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSVRow::CSVRow(std::string strRow)
        : m_pRowString(std::make_shared<std::string>(std::move(strRow)))
{ }

auto CSVRow::as_string() const -> const std::string&
{
    ASSERT_ERROR(nullptr != m_pRowString, "The row is empty.");
    return *m_pRowString;
}

auto CSVRow::begin() const noexcept -> const_iterator
{
    return const_iterator { m_pRowString };
}

[[nodiscard]] auto CSVRow::cbegin() const noexcept
{
    return begin();
}

auto CSVRow::end() const noexcept -> const_iterator
{
    return const_iterator { m_pRowString, std::size(*m_pRowString) };
}

auto CSVRow::cend() const noexcept
{
    return end();
}

} // namespace io
