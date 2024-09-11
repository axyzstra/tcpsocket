#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
using namespace std;

class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(int socket);
    ~TcpSocket();
    int connectToHost(string ip, unsigned short port);
    int sendMsg(string msg);
    string recvMsg();
private:
    int readn(char* buf, int size);
    int writen(const char* msg, int size);
private:
    int m_fd;
};


#endif  // _TCPSOCKET_H_