/**
 * @file        Task.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaration for Task.
 * @date        09-11-2020
 * @copyright   Copyright (c) 2020
 */

#pragma once

#include <chrono>
#include <map>
#include <mutex>
#include <string>

#include "Msg.h"

namespace task
{

/**
 * @class TaskManager
 * @brief Contains and logging all tasks execution times.
 */
class TaskManager
{
    TaskManager() = default;

    struct TaskPerformance
    {
        /**
         * @brief The task duration.
         */
        double duration = 0;

        /**
         * @brief The count of calls.
         */
        int32_t numOfCall = 0;
    };

public:

    TaskManager(TaskManager&&) = delete;

    TaskManager(const TaskManager&) = delete;

    TaskManager& operator=(TaskManager&&) = delete;

    TaskManager& operator=(const TaskManager&) = delete;

    ~TaskManager();

    static TaskManager& get()
    {
        static TaskManager tc;
        return tc;
    }

    /**
     * @brief               Updates the execution time.
     * @param name          The task name.
     * @param newDuration   The new execution time.
     * @param numOfCall     The num of calls.
     */
    void updateDuration(const char* name, double newDuration, int32_t numOfCall);

    /**
     * @brief           Enables logging in to the file.
     * @param enable    true for start logging.
     */
    void enableLogging(bool enable) noexcept
    {
        m_logeDurations = enable;
    }

private:

    /**
     * @internal
     * @brief   Dump all logs in to the time.log file.
     */
    void release() const;

private:
    bool m_logeDurations = false;
    mutable std::mutex m_mutex;
    std::map<const char*, TaskPerformance> m_taskNameToDurationMap;
};

/**
 * @brief           The Task class computes and logs the execution time.
 *
 * @details         Class implemented using the RAII pattern. The class is a wait-free thread-safe.
 * @tparam TaskName The string literal. The task name.
 */
template<char TaskName[]>
class Task
{
    /**
     * @internal
     * @class Duration
     * @brief Class Duration contains thread depend information about execution time.
     */
    class Duration
    {
    public:
        Duration() noexcept = default;

        /**
         * @brief Registrants the sum of duration in the TaskManager.
         */
        ~Duration()
        {
            TaskManager::get().updateDuration(TaskName, m_duration, m_numOfCall);
        }

        /**
         * @brief Updates duration time.
         * @param newDuration The new duration.
         */
        void addDuration(const double newDuration)
        {
            ++m_numOfCall;
            m_duration += newDuration;
        }

    private:

        int32_t m_numOfCall = 0;
        double m_duration = 0;
    };

public:

    /**
     * @brief Contracts object and saves the start time. Then print the task name.
     */
    Task()
        : m_startPoint(std::chrono::steady_clock::now())
    {
        using namespace std::string_literals;
        const std::string massage = "Start: "s + TaskName + "\n";
        io::Msg::Write(massage);
    }

    /**
     * @brief Computes the execution time, registrants that in the TaskManager after that displayed execution time.
     */
    ~Task()
    {
        using namespace std::string_literals;
        const auto endPoint = std::chrono::steady_clock::now();
        const std::chrono::duration<double, std::milli> duration = (endPoint - m_startPoint);
        const auto elapsed = duration.count();
        getThreadLocalDuration().addDuration(elapsed);
        const auto massage = "Task "s + TaskName + " finished. Execution time: " + std::to_string(elapsed) + "ms\n";
        io::Msg::Write(massage);
    }

private:

    /**
     * @brief   Gets the thread local Duration object.
     *
     * @return  Reference to the Duration object.
     */
    static Duration& getThreadLocalDuration()
    {
        thread_local static Duration s_threadLocalDuration;
        return s_threadLocalDuration;
    }

private:
    std::chrono::steady_clock::time_point m_startPoint;
}; // class Task

} // namespace task

/**
 * @brief The helper macros. Creates the task then measures the execution time in the current scope.
 */
#define START_TASK(_name_)              \
    static char taskName__[] = _name_;  \
    task::Task<taskName__> task__;      \
    (void)0
