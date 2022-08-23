#include <stdio.h>

#include "./include/ThreadPool.h"

using namespace simplemuduo;

ThreadPool::ThreadPool(int poolsize)
{
    m_run = true;

    int thread_num = 0;
    if (poolsize <= 0)
    {
        thread_num = std::max(std::thread::hardware_concurrency(), (unsigned int)1);
    }
    else
    {
        thread_num = poolsize;
    }
    m_threads.resize(thread_num);
    printf("m_thread size=%d\n", static_cast<int>(m_threads.size()));

    for (size_t i = 0; i < m_threads.size(); ++i)
    {
        m_threads[i] = std::thread([this](){
            for (;;)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    while (m_tasks.empty())
                    {
                        m_cond.wait(lock);
                        if (!m_run) return;
                    }
                    task = m_tasks.front();
                    m_tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    m_run = false;
    m_cond.notify_all();
    for (auto& i : m_threads)
    {
        if (i.joinable())
            i.join();
    }
}

int ThreadPool::getSize()
{
    return m_threads.size();
}

/*void ThreadPool::pushTask(std::function<void()> task)
{
    if (!m_run) return;
    std::unique_lock<std::mutex> lock(m_mutex);
    m_tasks.emplace(task);
    m_cond.notify_one();
}*/