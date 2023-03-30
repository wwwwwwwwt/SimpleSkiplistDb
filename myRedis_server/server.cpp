#include "./include/server.h"

using namespace std;

Server* server = new Server();

void setCommand(Server* server, Client* client, string key, string& value, bool& flag) {
    flag=client->db.insert_element(key, value);
    //flag = userToDB_[client->username_]->insert_element(key,value);
}

//get命令
void getCommand(Server* server, Client* client, string key, string& value, bool& flag) {
    flag = client->db.search_element(key, value);

}

//del命令
void delCommand(Server* server, Client* client, string key, string& value, bool& flag) {
    flag=client->db.delete_element(key);
    
}

//load命令
void loadCommand(Server* server, Client* client, string key, string& value, bool& flag) {
    client->db.load_file_path(client->username_ + ".txt");
}
//dump命令
void dumpCommand(Server* server, Client* client, string key, string& value, bool& flag) {
    client->db.dump_file_path(client->username_ + ".txt");
}

void loginCommand(Server* server, Client* client, string key, string& value, bool& flag) { // 登录时从对应用户名的文件中读取数据
   client->db.load_file_path(client->username_ + ".txt");
}

void quitCommand(Server* server, Client* client, string key, string& value, bool& flag) { // 登出时，将数据写入对应用户名文件
    // client->db.dump_file_path(client->username + ".txt");

    client->db.load_file_path(client->username_ + ".txt");

}

void initCommand(Server*& server){

    server->commands_.insert({"set", &setCommand});
    server->commands_.insert({"get", &getCommand});
    server->commands_.insert({"del", &delCommand});
    server->commands_.insert({"load", &loadCommand});
    server->commands_.insert({"dump", &dumpCommand});
   // server->commands_.insert({"login",&loginCommand});
   // server->commands_.insert({"quit", &quitCommand});

}

void initServer(Server*& server){

    server->listenFD_ = socket(AF_INET,SOCK_STREAM,0);

    initCommand(server);
    server->addr.sin_family = AF_INET;
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server->addr.sin_port = htons(PORT);

    bind(server->listenFD_,(struct sockaddr*)&server->addr, sizeof(server->addr));

    listen(server->listenFD_, 10);
}


int start(Client* client){

    Client * c = client;
    c->username_ = "ztw";

    char szMessage[MSGSIZE];

    int ret = 0;
    while(true && c->fd_){

        ret = recv(c->fd_, szMessage, MSGSIZE,0);
        if(ret <= 0){
            return 0;
        }

        szMessage[ret] = '\0';
        string msg(szMessage);
        
        Command C(msg);

        C.split_command();

        if(C.arg_[0] == "set"){
            string key = C.arg_[1];
            string val = C.arg_[2];

            bool flag = true;
            auto it = server->commands_.find("set");
            CommandFun cmd = it->second;
            cmd(server, c, key,val,flag);

            char _char_arry[] = "OK";
            char* str = _char_arry;
            send(c->fd_, str, strlen(str) + sizeof(char),NULL);
            continue;
        }

        if(C.arg_[0] == "get"){
            string key = C.arg_[1];
            string val = "";

            bool flag = true;
            auto it = server->commands_.find("get");
            CommandFun cmd = it->second;

            cmd(server, c, key, val, flag);

            if(flag){
                char* str = new char[strlen(val.c_str())];
                strcpy(str, val.c_str());
                send(c->fd_, str, strlen(str) + sizeof(char),NULL);
                delete []str;
            }

            else{
                char _char_arry[] = "(NULL)";
                char* str = _char_arry;
                send(c->fd_, str, strlen(str) + sizeof(char),NULL);
            }
            continue;
        }

        if(C.arg_[0] == "del"){
            string key = C.arg_[1];
            string val = "";
            bool flag = true;
            auto it = server->commands_.find("del");
            CommandFun cmd = it->second;
            cmd(server, c, key, val, flag);

            if(flag){
                string tempmsg = "del successful";
                char *str = new char[strlen(tempmsg.c_str()) + 1];
                strcpy(str, tempmsg.c_str());
                send(c->fd_, str, strlen(str) + sizeof(char), NULL);
                delete[]str;
            }
            else{
                string tempmsg = "del failed";
                char *str = new char[strlen(tempmsg.c_str()) + 1];
                strcpy(str, tempmsg.c_str());
                send(c->fd_, str, strlen(str) + sizeof(char), NULL);
                delete[]str;
            }
            continue;
            
        }

        if(C.arg_[0] == "load"){
            string key = "";
            string val = "";

            auto it = server->commands_.find("load");
            CommandFun cmd = it->second;
            bool flag = true;
            cmd(server, c, key, val, flag);

            string tempmsg = "load successful";
            char* str = new char[strlen(tempmsg.c_str()) + 1];
            strcpy(str, tempmsg.c_str());
            send(c->fd_, str, strlen(str) + sizeof(char), NULL);
            delete[]str;
            continue;
        }

        if(C.arg_[0] == "dump"){

            string key = "";
            string val = "";

            auto it = server->commands_.find("dump");
            CommandFun cmd = it->second;
            bool flag = true;
            cmd(server, c, key, val, flag);

            string tempmsg = "dump successful";
            char* str = new char[strlen(tempmsg.c_str()) + 1];
            strcpy(str, tempmsg.c_str());
            send(c->fd_, str, strlen(str) + sizeof(char), NULL);
            delete[]str;
            continue;
        }

    }
    return 0;
}

int main(){

    initServer(server);
    

    for(int i = 0; i< DBNUM; i++)
    {
        Client* client = new Client();
        socklen_t len = sizeof(client->addr_);
        cout<<"next client: "<<i<<endl;
        client->fd_ = accept(server->listenFD_, (struct sockaddr*)&(client->addr_), &len);
        cout<<"Accepted client:" << inet_ntoa((client->addr_).sin_addr) << " : "
			<< ntohs((client->addr_).sin_port) << endl;
        
        server->DB[i] = &(client->db);

        my_thread t(start,client);
    }

    close(server->listenFD_);
}