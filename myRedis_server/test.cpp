/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 13:49:12
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-30 18:16:43
 * @FilePath: /SimpleSkiplistDB/myRedis_server/test.cpp
 */
#include "./include/command.h"
#include "./include/skiplist.h"

#include <thread>

using namespace std;
int main(){
   /* string str = "get 456 56";
    Command a(str);
    a.is_valid_command();*/
    Skiplist<string,string> a(32);
   a.load_file_path("ztw.txt");
  //  a.insert_element("LSJ","1");
   // thread t1(&Skiplist<string,string>::insert_element,&a,"LSJ","1");
    cout<< a.size() <<endl;
  //  a.insert_element("ZTW","100");
  //  a.insert_element("WGL","10");
       cout<< a.size() <<endl;
    //t1.join();
    a.display_list();

  //  a.dump_file_path("ztw.txt");

    return 0;
}