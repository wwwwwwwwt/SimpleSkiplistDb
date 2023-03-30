/*
 * @Author: zzzzztw
 * @Date: 2023-03-16 15:10:04
 * @LastEditors: Do not edit
 * @LastEditTime: 2023-03-30 18:15:36
 * @FilePath: /SimpleSkiplistDB/myRedis_server/include/skiplist.h
 */

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include "./node.h"
#include "./random.h"
#include <fstream>
#include <mutex>
#include <string>
#include <mutex>
#include <iostream>

#define STORE_PATH   "../store/dumpFile.txt"
#define DIR_PATH "../store/"
std::string delimiter =":";


using std::string;
using std::ifstream;
using std::ofstream;
using std::mutex;

template<typename K, typename V>
class Skiplist
{
public:

    Skiplist();
    Skiplist(int level);
    ~Skiplist();
    Node<K,V>* create_node(K k, V v, int level);
    void display_list();

    int insert_element(K k, V v);
    bool search_element(K k,V &v);
    bool delete_element(K k);

    int size()const;

    void dump_file();
    void load_file();
    void dump_file_path(const string &str);
    void load_file_path(const string &str);

private:
    int get_random_level();
    void find(V target, vector<Node<K,V>*>& pre); 

    void get_key_val_from_string(const string&str, string* key, string *val);
    bool is_valid_string(const string &str);
    
private:

    int max_level;


    int cur_onlevel;

    int element_count_;

    ofstream fileWriter_;
    ifstream fileReader_;

    Node<K,V>* header_; //跳表头节点
    mutex mtx_;
    Random rnd;
};

//找到每一层的小于目标值的最大节点，开一个pre数组用于保存这个节点
template<typename K, typename V>
void Skiplist<K,V>::find(V target,vector<Node<K,V>*>& pre)
{
    auto p = header_;
    for(int i = max_level - 1; i>=0; i--){
        while(p->next[i] && p->next[i]->get_key() < target)p = p->next[i];
        pre[i] = p;
    }
}

template<typename K, typename V>
Node<K,V>* Skiplist<K,V>::create_node(K k, V v, int level)
{
    Node<K,V>* node = new Node<K,V>(k, v, level);
    return node;
}

template<typename K, typename V>
Skiplist<K,V>::Skiplist():rnd(0x12345678)
{
    max_level = 32;
    cur_onlevel = 0;
    element_count_ = 0;
    /*创造虚拟头节点，初始化为null null max_level*/
    K k;
    V v;
    rnd = {0x12345678};
    header_ = new Node<K,V>(k, v, max_level);
}

template<typename K, typename V>
Skiplist<K,V>::Skiplist(int level):rnd(0x12345678)
{
    max_level = level;
    cur_onlevel = 0;
    element_count_ = 0;
    /*创造虚拟头节点，初始化为null null max_level*/
    K k;
    V v;
    header_ = new Node<K,V>(k, v, max_level);
}

template<typename K, typename V>
Skiplist<K,V>::~Skiplist()
{
    delete header_;
    if(fileWriter_.is_open()){
        fileWriter_.close();
    }
    if(fileReader_.is_open()){
        fileReader_.close();
    }

}

template<typename K, typename V>
int Skiplist<K,V>::get_random_level(){

    int level = static_cast<int>(rnd.Uniform(max_level));
    if(level == 0){
        level = 1;
    }
    return level;

}

template<typename K, typename V>
int Skiplist<K,V>::insert_element(K key, V val)
{
    std::lock_guard<mutex>locker(mtx_);
    vector<Node<K,V>*>pre(max_level);
    find(key,pre);

    auto p = pre[0]->next[0];
    if(p && p->get_key() == key ){
        p->set_val(val);
        return 0;
    }



    if(!p || p->get_key() != key){
        int random_level = get_random_level();
        Node<K,V> * insert_node = new Node<K,V>(key,val,random_level);

        for(int i = 0;i< random_level ;i ++){
            insert_node->next[i] = pre[i]->next[i];
            pre[i]->next[i] = insert_node;
        }
        element_count_++;

        return 1;
    }
    return -1;

}

template<typename K, typename V>
bool Skiplist<K,V>::search_element(K key, V &val)
{
    std::lock_guard<mutex>locker(mtx_);
    /*通过val将查询到的值传出去*/
    vector<Node<K,V>* >pre(max_level);
    find(key, pre);

    auto p = pre[0]->next[0];
    if(p && p->get_key() ==  key){
        val = p->get_val();
        return true;
    }
    return false;
}


template<typename K, typename V>
bool Skiplist<K,V>::delete_element(K key)
{
    std::lock_guard<mutex>locker(mtx_);
    vector<Node<K,V>*>pre(max_level);
    find(key,pre);
    auto p = pre[0]->next[0];
    if(!p||p->get_key() != key){
        return false;
    }
    else{
        for(int i = 0;i<=max_level && pre[i]->next[i] == p; i++){
            pre[i]->next[i] = p->next[i];
        }
        delete p;
    }
    element_count_--;
    return true;
}

template<typename K, typename V>
void Skiplist<K,V>::display_list()
{
    std::cout<<"SkipList:\n";

    Node<K,V> *current;

    for(int i= max_level; i >= 0; i--){
        current=header_->next[i];
        std::cout<<"Level "<<i<<":\n";
        while(current!=nullptr){
            std::cout<<' '<<current->get_key()<<" : "<<current->get_val();
            current=current->next[i];
        }
        std::cout<<"\n";
    }
    return;
}

template<typename K, typename V>
int Skiplist<K,V>:: size()const
{
    return this->element_count_;
}


template<typename K, typename V>
void Skiplist<K,V>::dump_file()
{
    
    fileWriter_.open(STORE_PATH);

    string line;
    
    Node<K,V>* node = header_->next[0];

    while(node != nullptr){
        fileWriter_<<node->get_key()<< " " << node->get_val()<<"\n";
        node = node->next[0];
    }
    fileWriter_.flush();
    fileWriter_.close();
    return;

}

template<typename K, typename V>
void Skiplist<K,V>::load_file()
{
    fileReader_.open(STORE_PATH);

    string line;
    string *key = new string();
    string *val = new string();

    while(getline(fileReader_, line)){
        get_key_val_from_string(line,key,val);
        if(key->empty() || val->empty())continue;

        insert_element(*key, *val);
    }
    fileReader_.close();
    return;
}

template<typename K, typename V>
void Skiplist<K,V>::dump_file_path(const string &str)
{
    string path_ = DIR_PATH + str;
    fileWriter_.open(path_);
    string line;
    
    Node<K,V>* node = header_->next[0];

    while(node != nullptr){
        fileWriter_<<node->get_key()<< ":" << node->get_val()<<"\n";
        node = node->next[0];
    }
    fileWriter_.flush();
    fileWriter_.close();
    return;

}



template<typename K, typename V>
void Skiplist<K,V>::load_file_path(const string &str)
{   string path_ = DIR_PATH + str;
    fileReader_.open(path_);

    string line;
    string *key = new string();
    string *val = new string();
    while(getline(fileReader_, line)){
        get_key_val_from_string(line,key,val);
        if(key->empty() || val->empty())continue;

        insert_element(*key, *val);
    }
    fileReader_.close();
    return;
}


template<typename K, typename V>
void Skiplist<K,V>::get_key_val_from_string(const string& str, string* key, string* val)
{
    if(!is_valid_string(str))return;

    int pos = str.find(delimiter);
    *key = str.substr(0,str.find(delimiter));
    *val = str.substr(str.find(delimiter)+1, str.length());

    return;
}

template<typename K, typename V>
bool Skiplist<K,V>::is_valid_string(const string& str)
{
    
    if(str.size() == 0)return false;

    if(str.find(delimiter) == string::npos)return false;


    return true;

}

#endif
