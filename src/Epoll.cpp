#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

#include "./include/Epoll.h"
#include "./include/Channel.h"

using namespace simplemuduo;

Epoll::Epoll():
    m_epollfd(epoll_create(EPOLL_CLOEXEC)),
    m_events(16)
{
}

Epoll::~Epoll()
{
    ::close(m_epollfd);
}

void Epoll::epollDel(int fd)
{
    if (::epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, nullptr) == -1)
        printf("epoll_del err m_epollfd=%d fd=%d\n", m_epollfd, fd);
}

void Epoll::epollMod(struct epoll_event& ev)
{
    if (::epoll_ctl(m_epollfd, EPOLL_CTL_MOD, ev.data.fd, &ev) == -1)
        printf("epoll_mod err m_epollfd=%d fd=%d\n", m_epollfd, ev.data.fd);
}

void Epoll::updateChannel(Channel* channel)
{
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    // ev.data.fd = channel->getFd(); // 因为是union，同一时刻只能一个有值，所以不能同时赋值fd和ptr
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if (channel->getListened())
    {
        epollMod(ev);
    }
    else
    {
        if (::epoll_ctl(m_epollfd, EPOLL_CTL_ADD, channel->getFd(), &ev) == -1)
        {
            printf("epoll_add err m_epollfd=%d fd=%d\n", m_epollfd, channel->getFd());
        }
        else
        {
            channel->setListened(true);
        }
    }
}

std::vector<Channel*> Epoll::epoll(int timeout)
{
    std::vector<Channel*> active_events;
    int nready = ::epoll_wait(m_epollfd, &*m_events.begin(),
        static_cast<int>(m_events.size()), timeout);
        
    if (nready == static_cast<int>(m_events.size()))
    {
        m_events.resize(m_events.size() * 2);
    }

    for (int i = 0; i < nready; ++i)
    {
        static_cast<Channel*>(m_events[i].data.ptr)->setRevents(m_events[i].events);
        active_events.push_back(static_cast<Channel*>(m_events[i].data.ptr));
    }

    return std::move(active_events);
}