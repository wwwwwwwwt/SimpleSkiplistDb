/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 14:13:23
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-06 22:03:13
 * @FilePath: /SimpleSkiplistDB/myRedis_server/include/server.h
 */
#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cassert>
#include <errno.h>
#include <unistd.h>
#include <functional>
#include <utility>
#include <memory>
#include <map>
#include <cstring>
#include "./epoller.h"
#include "skiplist.h"
#include "./my_thread.h"
#include "command.h"
class Server;
class Client;

const int DBNUM = 16;
//发送消息的最大字节长度
#define MSGSIZE        1024   
#define PORT 5150 
using mydb = Skiplist<std::string, std::string>; 
using CommandFun = void (*) (Server*, Client*, string, string&,bool&);





class Client // 保存客户的用户名和对应数据库
{
public:

    Client() = default;
    int fd_;// 通信的文件描述符
    struct sockaddr_in addr_;
    std::string username_;
    mydb db;
};

class Server
{
public:

    int  SetNoBlock(int fd);
    //set命令
    
    mydb* DB[DBNUM];
    std::unordered_map<std::string, CommandFun>commands_;
    std::unordered_map<std::string, bool >userToClient_; // 用户名与客户的映射, 在集合中就证明一定有一个对应名字的txt
    std::unordered_map<std::string, mydb*>userToDB_; // 用户名和对应数据库的映射
    std::unique_ptr<Epoller>epoller_;
    int listenFD_;
    struct sockaddr_in addr;
    //int port_;
    bool openLinger_;
    int listenEvent_;
    int connEvent_;
};





#endif