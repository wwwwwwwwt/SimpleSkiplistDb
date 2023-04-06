/*
 * @Author: zzzzztw
 * @Date: 2023-03-30 17:10:32
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-06 22:56:30
 * @FilePath: /SimpleSkiplistDB/myRedis_client/client.cpp
 */
#include <iostream>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cassert>
#include <errno.h>
#include <unistd.h>
#include "command.h"
using namespace std;

#define SERVER_ADDRESS "192.168.159.129"
#define PORT 5150
#define MSGSIZE 1024

int main(){

    char szMessage[MSGSIZE];

    int clienFd_ = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0 , sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_addr.sin_port = htons(PORT);

    connect(clienFd_, (struct sockaddr*)&server_addr, sizeof(server_addr));

    cout << "Redis_client by Stack start-up...\n"; 
    int cnt = 1;
    while(true){
        if(cnt == 1){
            cout<<"please enter your username>";
            string message;
            cin>>message;
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            strcpy(szMessage, message.c_str());
            send(clienFd_, szMessage,strlen(szMessage), NULL);
            char buffer[1024];
            
            memset(buffer, 0, sizeof buffer);
            if(recv(clienFd_, buffer, 1024,NULL) <= 0){
                break;
            }
            else if(buffer[0] == 'e' && buffer[1] == 'r'){
                cout<<"username is not register, error"<<endl;
                break;
            }
            else{
                 cout << "Server reply: " << buffer << endl;
            }
            
        }
        else{
            cout<<"Stack_Redis>";
            string message;
            getline(cin,message);
            strcpy(szMessage,message.c_str());
            Command C(message);
            if(!C.is_valid_command()){
                continue;
            }

            send(clienFd_, szMessage,strlen(szMessage), NULL);

            char buffer[1024];
            memset(buffer, 0, sizeof buffer);

            if(recv(clienFd_, buffer, 1024,NULL) <= 0){
                break;
            }

            cout << "Server reply: " << buffer << endl;

        }
        cnt++;

    }

    close(clienFd_);
    return 0;
}