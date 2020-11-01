/**
 * @file        Row.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for Row.
 * @date        30-10-2020
 * @copyright   Copyright (c) 2020
 */
#include <DBGHAssert.h>

#include "CSVRow.h"

namespace csv
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Row::const_iterator class implementation.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Row::const_iterator::const_iterator(std::shared_ptr<std::string> pRowString, std::size_t lastPos)
        : m_pRowString(std::move(pRowString)),
        m_iLastPosition(lastPos)
{ }

auto Row::const_iterator::operator++() -> const_iterator&
{
    ASSERT_ERROR(nullptr != m_pRowString, "The iterator is invalid.");
    ASSERT_ERROR(m_iLastPosition < m_pRowString->size(), "The iterator is invalid.");
    const auto endOfCell = m_pRowString->find_first_of("\t ,", m_iLastPosition);

    m_iLastPosition = (std::string::npos == endOfCell)
                      ? std::size(*m_pRowString)
                      : endOfCell + 1;

    return *this;
}

auto Row::const_iterator::operator++(int) -> const_iterator
{
    auto tmp { *this };
    ++(*this);
    return tmp;
}

bool Row::const_iterator::operator==(const Row::const_iterator& other) const noexcept
{
    return (m_iLastPosition == other.m_iLastPosition) && (m_pRowString == other.m_pRowString);
}

bool Row::const_iterator::operator!=(const Row::const_iterator& other) const noexcept
{
    return !(*this == other);
}

auto Row::const_iterator::operator*() const -> Cell
{
    ASSERT_ERROR(nullptr != m_pRowString, "The iterator is invalid.");
    ASSERT_ERROR(m_iLastPosition < m_pRowString->size(), "The iterator is invalid.");

    const auto endOfCell = m_pRowString->find_first_of("\t ,", m_iLastPosition);

    const auto strCell = (std::string::npos == endOfCell)
                         ? std::string_view { *m_pRowString }.substr(m_iLastPosition)
                         : std::string_view { *m_pRowString }.substr(m_iLastPosition, endOfCell - m_iLastPosition);

    return Cell { strCell };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Row class implementation.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Row::Row(std::string strRow)
        : m_pRowString(std::make_shared<std::string>(std::move(strRow)))
{ }

auto Row::as_string() const -> const std::string&
{
    ASSERT_ERROR(nullptr != m_pRowString, "The row is empty.");
    return *m_pRowString;
}

auto Row::begin() const noexcept -> const_iterator
{
    return const_iterator { m_pRowString };
}

[[nodiscard]] auto Row::cbegin() const noexcept
{
    return begin();
}

auto Row::end() const noexcept -> const_iterator
{
    return const_iterator { m_pRowString, std::size(*m_pRowString) };
}

auto Row::cend() const noexcept
{
    return end();
}

} // namespace csv
