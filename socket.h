#ifndef _SOCKET_H_
#define _SOCKET_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
// 通用
// 创建 socket
int createSocket();
int recvMessage(int cfd, char** msg);
int sendMessage(int cfd, const char* buff, int size);

int closeSocket(int fd);

// 服务端
// 绑定套接字
int bindSocket(int lfd, unsigned short port);
// 服务器监听客户端请求
int listenCli(int lfd, int size);
// 接收请求
int acceptConn(int lfd, struct sockaddr_in* addr);


// 客户端
// 和服务端建立请求
int connectHost(int fd, const char* ip, unsigned short port);


int writen(int fd, const char* msg, int size);
int readn(int fd, char* buf, int size);

#endif  // _SOCKET_H_