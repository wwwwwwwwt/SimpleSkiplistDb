/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 13:13:40
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-07 09:39:00
 * @FilePath: /SimpleDb/myRedis_server/include/command.h
 */

#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_set>
using std::string;
using std::vector;
using std::istringstream;
const std::unordered_set<string> All_order = {"set", "get", "del", "load", "dump","login","quit"};//redis基础命令
const int order_num = 5;
const string lastSpace = " ";//用于将最后一个命令分开

class Command
{
public:
    Command() = default;
    ~Command() = default;
    Command(const string &s):command_(s){};
    string get_command(); //获得命令
    void split_command();//分割客户端传来的字符串，得到命令
    bool is_valid_command();//判断命令是否有效
    void print_error(int errornum);//非法命令报错
    vector<string>arg_;
private:
    string command_;

};


string Command::get_command()
{
    return  command_;
}


void Command::split_command()
{
    if(command_ == "")return;
    string str = get_command() + lastSpace;
    string order;
    
    istringstream ss(str);

    while(getline(ss,order,' ')){
        arg_.push_back(order);
        std::cout<<order<<std::endl;
    }


}

bool Command::is_valid_command()
{
    split_command();
    if(arg_.size() == 0)
    {
        print_error(0);
        return false;
    }
    if(All_order.find(arg_[0]) == All_order.end())
    {
        print_error(1);
        return false;
    }

    if(arg_[0] == "set" && arg_.size() != 3){
        print_error(2);
        return false;
    }

    if(arg_[0] == "get" && arg_.size() != 2)
    {
        print_error(3);
        return false;
    }

    if(arg_[0] == "del" && arg_.size() != 2)
    {
        print_error(4);
        return false;
    }

    if(arg_[0] == "dump" && arg_.size() != 1)
    {
        print_error(5);
        return false;
    }

    if(arg_[0] == "load" && arg_.size() != 1)
    {
        print_error(6);
        return false;
    }

    if(arg_[0] == "login" && arg_.size() != 2){
        print_error(7);
        return false;
    }

    if(arg_[0] == "quit" && arg_.size() != 1){
        print_error(8);
        return false;
    }

    if(arg_[0] == "dumppath" && arg_.size() != 2)
    {
        print_error(6);
        return false;
    }
    
    if(arg_[0] == "loadpath" && arg_.size() != 2)
    {
        print_error(6);
        return false;
    }
    

    std::cout<<"success"<<std::endl;
    return true;
}

void Command::print_error(int errornum){

    switch (errornum)
    {
    case 0:
        std::cout<<"(error) Non command!\n";
        break;
    case 1:
        std::cout<<"(error) unkwnon command\n";
        break;
    case 2:
        std::cout<<"(error) wrong number of arguments for \"set\" command\n";
        break;
    case 3:
        std::cout<<"(error) wrong number of arguments for \"get\" command\n";
        break;
    case 4:
        std::cout<<"(error) wrong number of arguments for \"del\" command\n";
        break;        
    case 5:
        std::cout<<"(error) wrong number of arguments for \"dump\" command\n";
        break; 
    case 6:
        std::cout<<"(error) wrong number of arguments for \"load\" command\n";
        break; 
    case 7:
        std::cout<<"(error) wrong number of arguments for \"login\" command\n";
        break; 
    case 8:
        std::cout<<"(error) wrong number of arguments for \"quit\" command\n";
        break; 
    case 9:
        std::cout<<"(error) wrong number of arguments for \"load\" command\n";
        break; 
    
    default:
        std::cout<<"Unknown error\n";
        break;
    }
}

#endif