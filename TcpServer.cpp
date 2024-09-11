#include "TcpServer.h"
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
using namespace std;

TcpServer::TcpServer()
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_fd == -1) {
        cout << "socket fail..." << endl;
    }
}

TcpServer::~TcpServer()
{
    if (m_fd > 0) {
        close(m_fd);
    }
}

int TcpServer::BindAndListen(unsigned short port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(m_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        cout << "bind fail..." << endl;
        return -1;
    }
    // cout << "socket 绑定成功" << endl;
    cout << "套接字绑定成功, ip: "
        << inet_ntoa(addr.sin_addr)
        << ", port: " << port << endl;
    
    ret = listen(m_fd, 128);
    if (ret == -1) {
        cout << "listen fail..." << endl;
        return -1;
    }
    cout << "设置监听成功" << endl;
    return ret;
}

TcpSocket* TcpServer::acceptConn(sockaddr_in *addr)
{
    if (addr == NULL) {
        return nullptr;
    }

    socklen_t addrlen = sizeof(sockaddr_in);
    int cfd = accept(m_fd, (sockaddr*)&addr, &addrlen);
    if (cfd == -1) {
        cout << "accept fail..." << endl;
        return nullptr;
    }
    cout << "成功和客户端建立连接" << endl;
    return new TcpSocket(cfd);
}
