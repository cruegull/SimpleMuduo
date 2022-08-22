#include <sys/epoll.h>

#include "./include/Channel.h"
#include "./include/EventLoop.h"

using namespace simplemuduo;

Channel::Channel(EventLoop& eventloop, int fd):
    m_eventloop(eventloop),
    m_fd(fd),
    m_events(0),
    m_revents(0),
    m_listened(false),
    m_threadpooled(true)
{
}

Channel::~Channel()
{
}

void Channel::setReadEvents()
{
    m_events = EPOLLIN | EPOLLET;
    m_eventloop.updateChannel(this);
}

void Channel::setHandleReadEvent(std::function<void()> call)
{
    m_read_callback = call;
}

void Channel::setHandleWriteEvent(std::function<void()> call)
{
    m_write_callback = call;
}

void Channel::handleEvent()
{
    if ((m_revents & EPOLLHUP) && !(m_revents & EPOLLIN))
    {
        // Todo close callback
    }

    if (m_revents & EPOLLERR)
    {
        // Todo error callback
    }

    if (m_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        m_read_callback();
    }

    if (m_revents & EPOLLOUT)
    {
        m_write_callback();
    }
}