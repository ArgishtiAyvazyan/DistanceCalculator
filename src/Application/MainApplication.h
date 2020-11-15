/**
 * @file        MainApplication.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for MainApplication.
 * @date        07-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <filesystem>

#include "DistanceCalculator.h"
#include "CSVUtils.h"
#include "MPIWrapper.h"

namespace dc
{

/**
 * @class       MainApplication
 * @brief       The main class of application.
 *
 * @details     The MainApplication class manages the application control flow and main settings.
 *              MainApplication is an entry point.
 */
class MainApplication
{
    /**
     * @brief The type of supported value.
     */
    using TValueType = float;

public:

    /**
     * @brief       Constructs and initializes MainApplication object.
     *
     * @param argc  Argument Count is int and stores number of command-line arguments passed
     *              by the user including the name of the program.
     * @param argv  Argument Vector is array of character pointers listing all the arguments.
     */
    MainApplication(int argc, char** argv) noexcept;

    /**
     * @brief   Runs execute application.
     *
     * @return  The execution status, 0 if the application executes successfully.
     */
    int run();

private:

    /**
     * @internal
     * @brief       Parses command line options end initializes the application parameters.
     *
     * @param argc  Argument Count is int and stores number of command-line arguments passed
     *              by the user including the name of the program.
     * @param argv  Argument Vector is array of character pointers listing all the arguments.
     * @return      Return false if option is -help, otherwise return true.
     */
    [[nodiscard]] bool parseAndInitParameters(int argc, char** argv);

    /**
     * @brief   Validates command line options.
     *
     * @throw   \ref dbgh::CAssertException if arguments is not valid.
     */
    void checkArguments() const;

    /**
     * @brief Displays summary of application status.
     */
    void showSummary() const;

    /**
     * @brief   Creates the \ref math::DistanceCalculator.
     *
     * @details Depend on command line options creates different engines.
     *
     * @return  The distance calculator object.
     */
    [[nodiscard]] auto createDistanceCalculator() const -> math::DistanceCalculator<TValueType>;

    /**
     * @brief   Loads query set and data set.
     *
     * @details Depend on command line options executed parallel or sequential.
     *          If the file path is empty the set generated randomly.
     *
     * @throw   \ref dbgh::CAssertException if the CSV file contains invalid value.
     *
     * @return  The pair of \ref csv::util::Table<TValueType>. first query set second data set.
     */
    [[nodiscard]] auto loadCSVFiles() const -> std::pair<csv::util::Table<TValueType>, csv::util::Table<TValueType>>;

    /**
     * @brief           Computes distances and returns distance matrix.
     *
     * @throw           \ref dbgh::CAssertException if the vector sizes is not equal.
     * @param query     The query set.
     * @param dataSet   The data set.
     *
     * @return          Distance matrix.
     */
    [[nodiscard]] auto computeDistances(const csv::util::Table<TValueType>& query
                                        , const csv::util::Table<TValueType>& dataSet) -> csv::util::Table<TValueType>;

    /**
     * @brief       Displays  the given csv table.
     *
     * @param table The csv table.
     */
    static void displayResult(const csv::util::Table<TValueType>& table);

    /**
     * @brief       Writes the given csv table to the output file.
     *
     * @param table The csv table.
     */
    void writeCSV(const csv::util::Table<TValueType>& table);

    /**
     * @brief   Uncovers and displays the exception details.
     *
     * @note    Executes only in debug mode.
     */
    void uncoverException(const dbgh::CAssertException&) const;

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
     * @brief True if the executed in debug mode otherwise false.
     */
    bool m_dbg;

    /**
     * @brief True if the executed in parallel mode otherwise false.
     */
    bool m_executeParallel;

    /**
     * @brief The path to the query file.
     */
    std::filesystem::path m_queryCSVFile;

    /**
     * @brief The path to the data set file.
     */
    std::filesystem::path m_dataSetCSVFile;

    /**
     * @brief The path to the output file.
     */
    std::filesystem::path m_outFile;

    /**
     * @brief The name of math metric.
     */
    std::string m_strMetric;

    /**
     * @brief Pointer to the MPI wrapper.
     */
     std::unique_ptr<mpi::MPIWrapper> m_pMPIWrapper;
};


} // namespace dc
