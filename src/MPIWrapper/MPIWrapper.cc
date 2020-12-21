/**
 * @file        MPIWrapper.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for MPIWrapper.
 * @date        12-11-2020
 * @copyright   Copyright (c) 2020
 */

#include "MPIWrapper.h"

#include <vector>
#include <algorithm>
#include <iterator>

#include <DBGHAssert.h>
#include <mpi.h>


namespace
{

/**
 * @brief       The helper classes for converting the C++ type to MPI type.
 * @tparam T    The c++ type.
 */
template <typename T> struct MPIType;
template <> struct MPIType <char>               { [[maybe_unused]] static constexpr auto value = MPI_CHAR; };
template <> struct MPIType <signed char>        { [[maybe_unused]] static constexpr auto value = MPI_SIGNED_CHAR; };
template <> struct MPIType <unsigned char>      { [[maybe_unused]] static constexpr auto value = MPI_UNSIGNED_CHAR; };
template <> struct MPIType <wchar_t>            { [[maybe_unused]] static constexpr auto value = MPI_WCHAR; };
template <> struct MPIType <short>              { [[maybe_unused]] static constexpr auto value = MPI_SHORT; };
template <> struct MPIType <unsigned short>     { [[maybe_unused]] static constexpr auto value = MPI_UNSIGNED_SHORT; };
template <> struct MPIType <int>                { [[maybe_unused]] static constexpr auto value = MPI_INT; };
template <> struct MPIType <unsigned>           { [[maybe_unused]] static constexpr auto value = MPI_UNSIGNED; };
template <> struct MPIType <long>               { [[maybe_unused]] static constexpr auto value = MPI_LONG; };
template <> struct MPIType <unsigned long>      { [[maybe_unused]] static constexpr auto value = MPI_UNSIGNED_LONG; };
template <> struct MPIType <float>              { [[maybe_unused]] static constexpr auto value = MPI_FLOAT; };
template <> struct MPIType <double>             { [[maybe_unused]] static constexpr auto value = MPI_DOUBLE; };
template <> struct MPIType <long double>        { [[maybe_unused]] static constexpr auto value = MPI_LONG_DOUBLE; };
template <> struct MPIType <long long int>      { [[maybe_unused]] static constexpr auto value = MPI_LONG_LONG_INT; };
template <> struct MPIType <unsigned long long> { [[maybe_unused]] static constexpr auto value = MPI_UNSIGNED_LONG_LONG; };
} // unnamed namespace


namespace mpi
{

using namespace std::string_literals;

namespace util
{
    /**
     * @brief           Sends the data to another process.
     *
     * @tparam T        The value type.
     * @param data      The data to be sent.
     * @param target    The destination process id. (rank).
     * @param tag       The data tag.
     */
    template <typename T>
    void send (const T& data, const int target, const int tag)
    {
        MPI_Rsend(&data, 1, MPIType<T>::value, target, tag, MPI_COMM_WORLD);
    }

    /**
     * @brief           Sends the data to another process. The specialization for std::vector.
     *
     * @tparam T        The value type.
     * @param data      The data to be sent.
     * @param target    The destination process id. (rank).
     * @param tag       The data tag.
     */
    template <typename T>
    void send (const std::vector<T>& data, const int target, const int tag)
    {
        MPI_Rsend(data.data(), static_cast<int>(std::size(data)), MPIType<T>::value, target, tag, MPI_COMM_WORLD);
    }

    /**
     * @brief           Receives the data of another process.
     *
     * @tparam T        The data type.
     * @param source    The source process id (rank).
     * @param tag       The data tag.
     * @return          The received data.
     */
    template <typename T>
    T receive(const int source, const int tag)
    {
        T data;
        MPI_Recv(&data, 1, MPIType<T>::value, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return data;
    }

    /**
     * @brief           Receives the data of another process. The specialization for std::vector.
     *
     * @tparam T        The data type.
     * @param source    The source process id (rank).
     * @param tag       The data tag.
     * @return          The received data.
     */
    template <typename T>
    std::vector<T> receive(const int size , const int source, const int tag)
    {
        std::vector <T> res (static_cast<size_t>(size));
        MPI_Recv(res.data(), size, MPIType<T>::value, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return res;
    }

} // namespace util


MPIWrapper::MPIWrapper(int* argc, char*** argv)
{
    registerParamsForSync();
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &m_iMPIRank);
    MPI_Comm_size(MPI_COMM_WORLD, &m_iMPISize);

    m_strProcessorName.resize(128);
    int length;
    MPI_Get_processor_name(m_strProcessorName.data(), &length);
    m_strProcessorName.shrink_to_fit();

}

MPIWrapper::~MPIWrapper()
{
    MPI_Finalize();
}

void MPIWrapper::distributeQuery(const TTable& query)
{
    const auto querySize = std::size(query);
    m_querySize = querySize;
    const auto [blockSize, numProcessor] = getBlockSize(querySize);
    const auto vectorSize = std::size(query[0]);

    size_t blockStart = 0;
    for (size_t pid = 1; pid < numProcessor; ++pid)
    {
        util::send<size_t>(vectorSize, static_cast<int>(pid), getVariableTag("vectorSize"));
        util::send<size_t>(querySize, static_cast<int>(pid), getVariableTag("querySize"));

        const auto flatMatrix = toFlatMatrix(std::span{ query }.subspan(blockStart, blockSize));
        blockStart += blockSize;
        util::send(flatMatrix, static_cast<int>(pid), getVariableTag("queryMatrix"));
    }

    for (size_t j = blockStart; j < querySize; ++j)
    {
        m_selfQuery.push_back( std::vector<TValueType> { query[j].begin(), query[j].end() } );
    }
}

void MPIWrapper::sendDataSet(const MPIWrapper::TTable& dataSet)
{
    ASSERT_ERROR(!dataSet.empty(), "dataSet is empty the computing is impossible.");
    const auto rowCount = std::size(dataSet);
    m_dataSetSize = rowCount;
    const auto flatMatrix = toFlatMatrix(dataSet);

    for (int pid = 1; pid < numOfProcessor(); ++pid)
    {
        util::send<size_t>(rowCount, pid, getVariableTag("dataSetSize"));
        util::send(flatMatrix, pid, getVariableTag("dataSetMatrix"));
    }

}

int MPIWrapper::registerVariable(std::string&& varName)
{
    static int uniqueVarID = 0;
    const auto search = m_mapVarNameToID.find(varName);
    if (search == m_mapVarNameToID.end())
    {
        const auto [nameToIDPair, _] = m_mapVarNameToID.emplace(std::move(varName), uniqueVarID++);
        return nameToIDPair->second;
    }
    return search->second;
}

void MPIWrapper::registerParamsForSync()
{
    registerVariable("vectorSize");
    registerVariable("querySize");
    registerVariable("dataSetSize");
    registerVariable("queryMatrix");
    registerVariable("dataSetMatrix");
    registerVariable("distanceMatrix");
}

int MPIWrapper::getVariableTag(const std::string& name) const
{
    try
    {
        return m_mapVarNameToID.at(name);
    }
    catch (...)
    {
        ASSERT_ERROR(false, ("The variable is not registered: "s + name).c_str());
    }
    return {};
}

std::pair<size_t, size_t> MPIWrapper::getBlockSize(size_t length) const
{
    const auto minPerProcessor = 1;
    const auto maxThreads = (length + minPerProcessor - 1 ) / minPerProcessor;
    const auto availableProcessorCount = numOfProcessor();
    const auto numProcessor = std::min(
            static_cast<size_t>(availableProcessorCount != 0
            ? availableProcessorCount
            : 2)
                , maxThreads );
    const auto blockSize = length / numProcessor;
    return { blockSize, numProcessor };
}

MPIWrapper::TTable MPIWrapper::receiveQuery()
{
    if (isMain())
    {
        return m_selfQuery;
    }

    if (!m_vectorSize.has_value())
    {
        m_vectorSize = util::receive<size_t>(mainPID, getVariableTag("vectorSize"));
    }
    const auto querySize = util::receive<size_t>(mainPID, getVariableTag("querySize"));

    const auto [blockSize, numProcessor] = getBlockSize(querySize);

    const auto flatMatrix = util::receive<TValueType>(static_cast<int>(blockSize * m_vectorSize.value())
            , 0, getVariableTag("queryMatrix"));

    TTable query(blockSize);

    auto blockStart = std::begin(flatMatrix);
    for (size_t i = 0; i < blockSize; ++i)
    {
        auto blockEnd = blockStart + static_cast<TDifferenceType>(m_vectorSize.value());
        query[i].insert(query[i].end(), blockStart, blockEnd);
        blockStart = blockEnd;
    }
    return query;
}

MPIWrapper::TTable MPIWrapper::receiveDataSet()
{
    ASSERT_ERROR(!isMain(), "The function not supported by the main processor.");

    if (!m_vectorSize.has_value())
    {
        m_vectorSize = util::receive<size_t>(mainPID, getVariableTag("vectorSize"));
    }
    if (!m_dataSetSize.has_value())
    {
        m_dataSetSize = util::receive<size_t>(mainPID, getVariableTag("dataSetSize"));
    }
    const auto flatMatrix = util::receive<TValueType>(static_cast<int>(m_dataSetSize.value() * m_vectorSize.value())
            , mainPID, getVariableTag("dataSetMatrix"));

   TTable dataSet(m_dataSetSize.value());

    auto blockStart = std::begin(flatMatrix);
    for (size_t i = 0; i < m_dataSetSize.value(); ++i)
    {
        auto blockEnd = blockStart + static_cast<TDifferenceType>(m_vectorSize.value());
        dataSet[i].insert(dataSet[i].end(), blockStart, blockEnd);
        blockStart = blockEnd;
    }
    return dataSet;
}

void MPIWrapper::sendDistanceMatrix(const TTable& distanceMatrix)
{
    if (isMain())
    {
        m_selfDistanceMatrix = distanceMatrix;
        return;
    }
    const auto flatMatrix = toFlatMatrix(distanceMatrix);
    util::send(flatMatrix, mainPID, getVariableTag("distanceMatrix"));
}

auto MPIWrapper::toFlatMatrix(std::span<const std::vector<TValueType>> table) -> std::vector<TValueType>
{
    if (std::empty(table))
    {
        return { };
    }

    std::vector<TValueType> flatMatrix;
    flatMatrix.reserve(std::size(table) * std::size(table[0]));

    for (const auto& row : table)
    {
        flatMatrix.insert(std::cend(flatMatrix), std::cbegin(row), std::cend(row));
    }

    ASSERT_DEBUG(std::size(flatMatrix) == std::size(table) * std::size(table[0])
                 , "The size of a flat matrix is not expected.");
    return flatMatrix;
}

auto MPIWrapper::receiveDistanceMatrix() -> TTable
{
    ASSERT_DEBUG(m_querySize.has_value(), "The query size is not exists");
    ASSERT_DEBUG(m_dataSetSize.has_value(), "The dataSet size is not exists");
    ASSERT_DEBUG(!m_selfDistanceMatrix.empty(), "The self distance matrix is not exists.");

    const auto [blockSize, _] = getBlockSize(m_querySize.value());

    TTable distanceMatrix;

    const auto flatSize = static_cast<int>(blockSize * m_dataSetSize.value());
    for (int pid = 1; pid < numOfProcessor(); ++pid)
    {
        std::vector<TValueType> flatMatrix = util::receive<TValueType>(
                flatSize, pid, getVariableTag("distanceMatrix"));
        TTable matrix = splitFlatMatrix(flatMatrix, m_dataSetSize.value());

        std::move(std::begin(matrix), std::end(matrix), std::back_inserter(distanceMatrix));
    }
    std::move(std::begin(m_selfDistanceMatrix), std::end(m_selfDistanceMatrix)
              , std::back_inserter(distanceMatrix));
    m_selfDistanceMatrix.clear();
    return distanceMatrix;
}

auto MPIWrapper::splitFlatMatrix(const std::vector<TValueType>& flatMatrix, const size_t rowSize) -> TTable
{
    const size_t matrixSize = std::size(flatMatrix) / rowSize;
    TTable matrix (matrixSize);

    auto blockStart = std::begin(flatMatrix);
    for (size_t i = 0; i < matrixSize; ++i)
    {
        auto blockEnd = blockStart + static_cast<TDifferenceType>(rowSize);
        matrix[i].insert(matrix[i].end(), blockStart, blockEnd);
        blockStart = blockEnd;
    }

    return matrix;
}

void MPIWrapper::distributeTask(const TTable& query, const TTable& dataSet)
{
    distributeQuery(query);
    sendDataSet(dataSet);
}

} // namespace mpi
