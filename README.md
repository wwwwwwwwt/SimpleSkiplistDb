<!--
 * @Author: zzzzztw
 * @Date: 2023-03-30 18:19:50
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-04-20 19:08:43
 * @FilePath: /SimpleDb/README.md
-->
# 基于跳表的简单KV数据库

## version 1.0
1. 使用跳表作为底层的存储数据结构。

2. 使用和Redis相同的语法，进行增删改查。

3. 使用简单封装的线程api，不需要担心线程未回收造成段错误。

4. 使用网络编程技术，将该程序部署到服务器。

## 目前支持操作
### 
1. 简单的增删改查 
* 例如：``` set ztw 25000```, ```set wgl 16000```, ```get ztw``` ```del wgl``` 
2. 支持读盘与落盘操作```load```与```dump```，记录后的文件将存储到store文件夹
3. 使用socket网络编程，可以将服务部署到服务器

## version 2.0 将进行的改进（to do）
1. 将客户名与落盘文件名进行映射，即第一次连接时，首先会强制用户输入用户名，然后根据用户名进行load，非注册用户将提示没有注册，不可使用这个数据库。(finish, 实现失败后的注册函数)
2. 改善服务端server的整体文件结构，更加面向对象。
3. 实现仿事务功能（开一个）deque / stack，如果是命令set xx xx 就先get查看当前数据中有没有xx数据，如果有就把set xx 当前值加入队列， 如果没有就将del xx 加入队列。如果是del ，先get查看有没有，如果有就set xx xx 如果没有就什么也不做，其他命令直接执行。如果最后commit 就直接break掉循环，如果rockback就将stack依次拿出来执行直到为空。（nextTodo）
```cpp
std::deque<std::string> trans_list;
template<typename K, typename V>
void SkipList<K, V>::startTransaction(){
    std::cout << "Transaction begins..." << std::endl;
    std::string str;
    while(1){
        std::cin >> str;
        if(str == "DROP"){
            std::cout << "Transaction dropped..." << std::endl;
            this->trans_list.clear();
            break;      
        }

        if(str == "SUBMIT"){
            if(this->trans_list.size() == 0){
                std::cout << "Transaction is empty, dropped..." << std::endl;
                break;
            } else {
                this->submitTransaction();
                break;
            }
        }

        if(str == "BEGIN"){
            std::cout << "Transaction is being edited..." << std::endl;
        }
        this->trans_list.push_front(str);
    }
}

template<typename K, typename V>
void SkipList<K,V>::submitTransaction(){
    std::cout << "Transaction submitted..." << std::endl;
    while(this->trans_list.size() >0){
        this->parseInput(this->trans_list.back());
        trans_list.pop_back();
    }
    this->writeToDisk();
}


```
1. 使用epoll io端口复用技术将主线程变为单线程，并且创建子线程在后台进行定时落盘等操作。
