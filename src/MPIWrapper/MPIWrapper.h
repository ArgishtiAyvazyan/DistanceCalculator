/**
 * @file        MPIWrapper.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for MPIWrapper.
 * @date        12-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <string>
#include <vector>

#include <map>
#include <optional>
#include <span>

namespace mpi
{

/**
 * @class MPIWrapper
 * @brief The wrapper class for MPI helps for working in multiprocess flow.
 */
class MPIWrapper
{
    using TValueType = float;
    using TTable = std::vector<std::vector<TValueType>>;
    using TDifferenceType = std::iterator_traits<std::vector<TValueType>::iterator>::difference_type;


    static constexpr int mainPID = 0;
public:

    MPIWrapper(int* argc, char*** argv);
    ~MPIWrapper();


    /**
     * @brief   Gets the current process rank.
     * @return  The rank.
     */
    [[nodiscard]] int rank() const noexcept
    {
        return m_iMPIRank;
    }

    /**
     * @brief   Checks, the current process is main or not.
     * @return  True the current process is main, otherwise false.
     */
    [[nodiscard]] bool isMain() const noexcept
    {
        return 0 == rank();
    }

    /**
     * @brief   The number of available processor.
     * @return  The number of processor.
     */
    [[nodiscard]] int numOfProcessor() const noexcept
    {
        return m_iMPISize;
    }

    /**
     * @brief   Checks, running in the multi process flow.
     * @return  True running in the multi process flow, otherwise false.
     */
    [[nodiscard]] bool isMPF() const noexcept
    {
        return numOfProcessor() > 1;
    }

    /**
     * @brief   Gets the current processor name.
     *
     * @return  The processor name.
     */
    [[nodiscard]] const std::string& processorName() const noexcept
    {
        return m_strProcessorName;
    }


    /**
     * @brief           Distributes the query and dataSet to all processes.
     *
     * @param query     The query vectors.
     * @param dataSet   The data set.
     */
    void distributeTask(const TTable& query, const TTable& dataSet);

    /**
     * @brief   Returns the query vectors for current processor.
     * @return  The query vectors.
     */
    TTable receiveQuery();

    /**
     * @brief   Returns the data set for current processor.
     * @return  The data set.
     */
    TTable  receiveDataSet();

    /**
     * @brief Sends the distance matrix to the main processor.
     *
     * @param distanceMatrix The matrix of distance.
     */
    void sendDistanceMatrix(const TTable& distanceMatrix);

    /**
     * @brief   Receives the child processors distance matrices.
     * @return  The matrix of distance.
     */
    TTable receiveDistanceMatrix();
private:

    /**
     * @brief       Distributes the query to all processes.
     * @param query The query vectors.
     */
    void distributeQuery(const TTable& query);

    /**
     * @brief           Sends the dataSet to all processes.
     * @param dataSet   The data set.
     */
    void sendDataSet(const TTable& dataSet);

    /**
     * @brief           Registrations the new variable.
     *
     * @param varName   The variable name.
     * @return          The variable tag.
     */
    int registerVariable (std::string&& varName);

    /**
     * @brief           Computes and returns the optimal count of processes and query block size for one process.
     * @param length    The number of query vectors.
     * @return          The std::pair, first the block size, second number of process.
     */
    [[nodiscard]] std::pair<size_t, size_t> getBlockSize(size_t length) const;

    /**
     * @brief       Converts matrix to flat matrix.
     *
     * @param table The matrix.
     * @return      The flat matrix.
     */
    static std::vector<TValueType> toFlatMatrix(std::span<const std::vector<TValueType>> table);

    /**
     * @brief               Converts the flat matrix to the normal matrix.
     *
     * @param flatMatrix    The flat matrix.
     * @param rowSize       The size of row.
     * @return              The matrix.
     */
    static TTable splitFlatMatrix(const std::vector<TValueType>& flatMatrix, size_t rowSize);

    /**
     * @brief       Gets the tag for variable.
     *
     * @throw   \ref dbgh::CAssertException if variable is not exists.
     *
     * @param name  The name of variable.
     * @return      The variable tag.
     */
    [[nodiscard]] int getVariableTag (const std::string& name) const;

    /**
     * @internal
     * @brief   Registrations of the variables for sync into processes.
     */
    void registerParamsForSync();

private:

    /**
     * @brief The current process rank.
     */
    int m_iMPIRank{};

    /**
     * @brief The count of available processes.
     */
    int m_iMPISize{};

    /**
     * @brief The current procesor name.
     */
    std::string m_strProcessorName;

    /**
     * @brief The variable name to tag map.
     */
    std::map <std::string, int> m_mapVarNameToID;

    /**
     * @brief The query vectors for current process.
     */
    TTable m_selfQuery;

    /**
     * @brief The computed distance matrix.
     */
    TTable m_selfDistanceMatrix;

    /**
     * @brief The vectors size.
     */
    std::optional<size_t> m_vectorSize;

    /**
     * @brief The num of vectors in query.
     */
    std::optional<size_t> m_querySize;

    /**
     * @brief The num of vectors in dataSet.
     */
    std::optional<size_t> m_dataSetSize;
};

} // namespace mpi
