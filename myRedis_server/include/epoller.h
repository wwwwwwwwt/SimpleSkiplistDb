/*
 * @Author: zzzzztw
 * @Date: 2023-03-21 19:27:50
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-21 19:34:24
 * @FilePath: /cpptest/skiplistpro/myRedis_server/epoller.h
 */
#ifndef EPOLLER_H
#define EPOOLER_H

#include <sys/epoll.h>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <errno.h>


class Epoller
{
public:

    explicit Epoller(int MaxEvents = 1024);

    ~Epoller();

    bool AddFd(int fd, uint32_t events);

    bool ModFd(int fd, uint32_t events);

    bool DelFd(int fd);

    int Wait(int timeoutMs = -1); // 定时器

    int GetEventFd(size_t i)const;

    uint32_t GetEvents(size_t i)const;
private:

    int epollFd_;
    std::vector<struct epoll_event>events_;
};


#endif