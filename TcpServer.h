#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_
#include "TcpSocket.h"

class TcpServer {
public:
    TcpServer();
    ~TcpServer();
    int BindAndListen(unsigned short port);
    TcpSocket* acceptConn(sockaddr_in* addr);
private:
    int m_fd;
};

#endif  // _TCPSERVER_H_