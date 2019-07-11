/*
    This file is part of Elixir, an open-source cross platform physically
    based renderer.

    Copyright (c) 2019 Samuel Van Allen - All rights reserved.

    Elixir is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <condition_variable>
#include <mutex>
#include <future>
#include <queue>

exrBEGIN_NAMESPACE

struct ThreadTask
{
    template <typename Func, typename... Args>
    ThreadTask(exrFloat priority, Func&& func, Args&&... args)
    {
        // package task function with its arguments
        auto pTask = std::make_shared<std::packaged_task<void()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        // convert packaged task to a void std::function
        m_Task = ([pTask]() { (*pTask)(); });
        m_Priority = priority;
    };

    inline exrBool operator<(const ThreadTask& t) const { return m_Priority < t.m_Priority; }

    std::function<void()> m_Task;
    exrFloat m_Priority;
};

template <exrU32 NumThreads>
class ThreadPool
{
public:
    ThreadPool() : m_Stop(false)
    {
        for (auto i = 0; i < NumThreads; ++i)
        {
            m_Threads.emplace_back([this]
            {
                while (true)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->m_QueueMutex);
                        this->m_Condition.wait(lock, [this] { return this->m_Stop || !this->m_Tasks.empty(); });

                        if (this->m_Stop && this->m_Tasks.empty())
                            return;

                        task = std::move(this->m_Tasks.top().m_Task);
                        this->m_Tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_Stop = true;
        }

        m_Condition.notify_all();

        for (std::thread& thread : m_Threads)
            thread.join();
    };

    template <typename Task, typename... Args>
    void ScheduleTask(exrFloat priority, Task&& task, Args&&... args)
    {
        std::lock_guard<std::mutex> lock(m_QueueMutex);

        if (m_Stop)
            throw std::runtime_error("Task enqueued on a stopped ThreadPool!");

        m_Tasks.emplace(priority, std::forward<Task>(task), std::forward<Args>(args)...);
        m_Condition.notify_one();
    };

private:
    std::priority_queue<ThreadTask> m_Tasks;
    std::mutex m_QueueMutex;
    std::condition_variable m_Condition;
    std::vector<std::thread> m_Threads;
    exrBool m_Stop;
};

exrEND_NAMESPACE