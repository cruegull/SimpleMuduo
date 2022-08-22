#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

namespace simplemuduo
{
    class ThreadPool
    {
    private:
        std::vector<std::thread>            m_threads;
        std::queue<std::function<void()>>   m_tasks;
        std::mutex                          m_mutex;
        std::condition_variable             m_cond;
        bool                                m_run;

    public:
        ThreadPool(int poolsize = 0);
        ~ThreadPool();
        int getSize();
        // void pushTask(std::function<void()> task);
        template<class F, class... Args>
        auto pushTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    };

    template<class F, class... Args>
    auto ThreadPool::pushTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
            
        std::future<return_type> ret = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            if(!m_run)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            m_tasks.emplace([task](){ (*task)(); });
        }
        m_cond.notify_one();
        return ret;
    }
}