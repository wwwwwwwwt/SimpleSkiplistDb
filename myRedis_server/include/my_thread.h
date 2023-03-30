/*
 * @Author: zzzzztw
 * @Date: 2023-03-21 19:45:26
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-21 19:45:30
 * @FilePath: /cpptest/skiplistpro/myRedis_server/my_thread.h
 */
#include <thread>

class my_thread
{
public:

    template<typename ...Args>
    my_thread(Args&& ...args):thread_(std::forward<Args>(args)...){};

    my_thread(my_thread && other):thread_(std::move(other.thread_)){};

    my_thread(const my_thread & other) = delete;

    ~my_thread(){
        if(thread_.joinable()){
            thread_.join();
        }
    }

private:
    std::thread thread_;
};