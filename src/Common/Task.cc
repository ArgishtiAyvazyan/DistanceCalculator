/**
 * @file        Task.cc
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Implementation for TaskManager.
 * @date        09-11-2020
 * @copyright   Copyright (c) 2020
 */


#include <fstream>
#include "Task.h"

task::TaskManager::~TaskManager()
{
    release();
}

void task::TaskManager::updateDuration(const char* name, double newDuration, int32_t numOfCall)
{
    if (!m_logeDurations)
    {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    auto& performance = m_taskNameToDurationMap[name];
    performance.duration += newDuration;
    performance.numOfCall += numOfCall;
}

void task::TaskManager::release() const
{
    std::fstream fLogOut { "time.log", std::ios_base::app };
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto&[name, performance] : m_taskNameToDurationMap)
    {
        fLogOut << "Task: " << name << " | count of call: " << performance.numOfCall
                  << " | duration: " << performance.duration << std::endl;
    }
}
