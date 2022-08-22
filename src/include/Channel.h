#pragma once

#include <functional>

namespace simplemuduo
{
    class EventLoop;

    class Channel
    {
    private:
        EventLoop&              m_eventloop;        // 属于哪个EventLoop
        int                     m_fd;               // 一个channel对于一个fd
        int                     m_events;           // 监听事件
        int                     m_revents;          // 发生事件
        bool                    m_listened;         // 是否被监听
        bool                    m_threadpooled;     // 是否使用线程池处理
        std::function<void()>   m_read_callback;    // 事件回调
        std::function<void()>   m_write_callback;   // 事件回调

    public:
        Channel(EventLoop& m_eventloop, int fd);
        ~Channel();
        int getFd() { return m_fd; };

        void setEvents(int events) { m_events = events; };
        int getEvents() { return m_events; };

        void setRevents(int revents) { m_revents = revents; };
        int getREvents() { return m_revents; };

        void setListened(bool listened) { m_listened = listened; };
        bool getListened() { return m_listened; };

        void setThreadPooled(bool threadpooled) { m_threadpooled = threadpooled; };

        void setReadEvents();

        void setHandleReadEvent(std::function<void()> call);
        void setHandleWriteEvent(std::function<void()> call);
        void handleEvent();
    };
}