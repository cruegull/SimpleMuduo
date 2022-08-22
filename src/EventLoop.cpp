#include "./include/EventLoop.h"
#include "./include/Epoll.h"
#include "./include/Channel.h"

using namespace simplemuduo;

EventLoop::EventLoop():
    m_epoll(new Epoll),
    m_loop(false)
{
}

EventLoop::~EventLoop()
{
}

void EventLoop::updateChannel(Channel* channel)
{
    m_epoll->updateChannel(channel);
}

void EventLoop::loop()
{
    m_loop = true;
    
    for (;;)
    {
        if (!m_loop) break;

        std::vector<Channel*> active_events =  m_epoll->epoll();
        for (const auto i : active_events)
        {
            i->handleEvent();
        }
    }
}

void EventLoop::stop()
{
    m_loop = false;
}