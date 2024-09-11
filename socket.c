#include "socket.h"
#include <stdio.h>

int createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket fail...\n");
        return -1;
    }
    printf("套接字创建成功, fd=%d\n", fd);
    return fd;
}

int readn(int fd, char *buf, int size)
{
    char *pt = buf;
    int count = size;
    while (count > 0) {
        int len = recv(fd, pt, count, 0);
        if (len == -1) {
            return -1;
        } else if (len == 0) {
            return size - count;
        }
        pt += len;
        count -= len;
    }
    return size;
}


int recvMessage(int cfd, char **msg)
{
    int len = 0;
    readn(cfd, (char*)&len, 4);
    len = ntohl(len);
    printf("数据大小 %d\n", len);

    char *buf = (char*)malloc(len + 1);
    int ret = readn(cfd, buf, len);

    if (ret != len) {
        close(cfd);
        free(buf);
        return -1;
    }
    buf[len] = '\0';
    *msg = buf;
    return ret;
}

// 将数据一次性发送出去
int writen(int fd, const char* msg, int size) {
    int count = size;
    const char* buf = msg;
    while (count > 0) {
        int len = send(fd, buf, count, 0);
        if (len == -1) {
            close(fd);
            return -1;
        } else if (len == 0) {
            continue;
        } 
        buf += len;
        count -= len;
    }
    return size;
}



int sendMessage(int cfd, const char *buff, int size)
{
    if (buff == NULL || size <= 0 || cfd <= 0){
        return -1;
    }
    char *msg = (char*)malloc(size + 4);
    int bigLen = htonl(size);
    memcpy(msg, &bigLen, 4);
    memcpy(msg + 4, buff, size);
    int ret = writen(cfd, msg, size + 4);
    if (ret == -1) {
        perror("send fail...\n");
        close(cfd);
    }
    free(msg);
    return ret;
}

int closeSocket(int fd)
{
    int ret = close(fd);
    if (ret == -1) {
        perror("close fail...\n");
    }
    return ret;
}

int bindSocket(int fd, unsigned short port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("bind fail...\n");
        return -1;
    }
    printf("套接字绑定成功, ip: %s, port: %d\n",
            inet_ntoa(addr.sin_addr), port);
    return ret;
}

int listenCli(int fd, int size)
{
    int ret = listen(fd, size);
    if (ret == -1) {
        perror("listen fail...\n");
        return -1;
    }
    printf("设置监听成功...\n");
    return ret;
}

int acceptConn(int fd, struct sockaddr_in *addr)
{
    int size = sizeof(addr);
    int cfd = accept(fd, (struct sockaddr*)addr, &size);
    if (cfd == -1) {
        perror("accept fail...\n");
        return -1;
    }
    printf("成功和客户端建立连接...\n");
    return cfd;
}

int connectHost(int fd, const char *ip, unsigned short port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);
    addr.sin_port = htons(port);
    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect fail...\n");
        return -1;
    }
    printf("成功和服务器建立连接...\n");
    return ret;
}


