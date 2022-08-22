#pragma once

#include <memory>

namespace simplemuduo
{
    class Epoll;
    class Channel;

    class EventLoop
    {
    private:
        std::unique_ptr<Epoll>  m_epoll;
        bool                    m_loop;

    public:
        EventLoop();
        ~EventLoop();
        void updateChannel(Channel* channel);
        void loop();
        void stop();
    };
}