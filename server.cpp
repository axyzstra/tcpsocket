#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <pthread.h>
#include "TcpServer.h"
using namespace std;

struct SockInfo {
    TcpServer* s;
    TcpSocket* tcp;
    sockaddr_in addr;
};

void* working(void* arg) {
   SockInfo* info = static_cast<SockInfo*> (arg);
   char ip[32];
   printf("客户端的IP: %s, 端口: %d\n",
        inet_ntop(AF_INET, &info->addr.sin_addr.s_addr, ip, sizeof(ip)),
        ntohs(info->addr.sin_port));
    
    while (1) {
        cout << "接收数据" << endl;
        string msg = info->tcp->recvMsg();
        if (!msg.empty()) {
            cout << msg << endl << endl << endl;
        } else {
            break;
        }
    }
    delete info->tcp;
    delete info;
    return nullptr;
}



int main() {
    TcpServer s;
    int ret = s.BindAndListen(9898);
    if (ret == -1) {
        return -1;
    }
    while (1) {
        SockInfo *info = new SockInfo;
        TcpSocket* tcp = s.acceptConn(&info->addr);
        if (tcp == nullptr) {
            cout << "重试" << endl;
            continue;
        }
        // 建立连接后，创建客户端子进程进行通信
        pthread_t tid;
        info->s = &s;
        info->tcp = tcp;
        pthread_create(&tid, nullptr, working, info);
        pthread_detach(tid);
    }
    return 0;
}