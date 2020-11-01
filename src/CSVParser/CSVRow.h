/**
 * @file        Row.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for Row.
 * @date        30-10-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <memory>

#include "CSVCell.h"

namespace csv
{

/**
 * @class Row
 * @brief The C++ representation for CSV row.
 */
class Row
{
public:

    /**
     * @class const_iterator
     * @brief The const forward iterator for traversing row cells.
     *
     * @detals Provides the traversing on row cells.
     */
    class const_iterator
    {
        friend class Row;

        /**
         * @internal
         * @brief Constructs and initialize iterator.
         * @param pRowString The shared pointer to the string representation of CSV row.
         * @param lastPos The index of the first cell, if the equal of pRowString size makes the end iterator.
         */
        explicit const_iterator(std::shared_ptr<std::string> pRowString, std::size_t lastPos = 0);

    public:

        const_iterator() = default;

        const_iterator(const const_iterator&) = default;

        const_iterator(const_iterator&&) = default;

        const_iterator& operator=(const_iterator&&) = default;

        const_iterator& operator=(const const_iterator&) = default;

        ~const_iterator() = default;

        const_iterator& operator++();

        [[maybe_unused]] const_iterator operator++(int);

        bool operator==(const const_iterator& other) const noexcept;

        bool operator!=(const const_iterator& other) const noexcept;

        Cell operator*() const;

    private:

        /**
         * @internal
         * @brief The shared pointer to the string representation of CSV row.
         */
        std::shared_ptr<std::string> m_pRowString { nullptr };

        /**
         * @internal
         * @brief The index of current cell.
         */
        std::size_t m_iLastPosition { 0 };
    }; // class const_iterator


public:

    /**
     * @brief Contracts and initialized Row object.
     * @param strRow The string representation of CSV row.
     */
    explicit Row(std::string strRow);

    /**
     * @brief Gets the string representation of the CSV row.
     * @return The string with CSV row.
     */
    [[nodiscard]] const std::string& as_string() const;

    /**
     * @brief Returns the iterator to the memory representation of the first cell of the CSV file row.
     * @return The Row::const_iterator.
     */
    [[nodiscard]] const_iterator begin() const noexcept;


    /**
      * @brief Returns the iterator to the memory representation of the past-the-end cell of the CSV file row.
      * @return The Row::const_iterator.
      */
    [[nodiscard]] const_iterator end() const noexcept;

    /**
     * @brief Returns the iterator to the memory representation of the first cell of the CSV file row.
     * @return The Row::const_iterator.
     */
    [[nodiscard]] auto cbegin() const noexcept;


    /**
      * @brief Returns the iterator to the memory representation of the past-the-end cell of the CSV file row.
      * @return The Row::const_iterator.
      */
    [[nodiscard]] auto cend() const noexcept;


private:
    std::shared_ptr<std::string> m_pRowString;
}; // class Row
} // namespace csv
