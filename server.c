#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "socket.h"

typedef struct {
    int fd;
    pthread_t tid;
    struct sockaddr_in addr;
} SockInfo;

SockInfo sockinfos[128];


void* working(void* arg) {
    SockInfo* sockInfo = (SockInfo*)arg;
    char ip[32];
    printf("客户端 ip: %s, 端口：%d\n",
            inet_ntop(AF_INET, &sockInfo->addr.sin_addr.s_addr, ip, sizeof(ip)),
            ntohs(sockInfo->addr.sin_port));
    // 开始通信
    while (1) {
        char *buff;
        printf("接收数据\n");
        int ret = recvMessage(sockInfo->fd, &buff);
        if (ret > 0) {
            printf("%s\n\n\n\n", buff);
        } else {
            break;
        }
        sleep(1);
    }
    return NULL;
}



int main () {
    // 1. 创建 socket 文件描述符，使用 ipv4 流式协议 TCP
    int fd = createSocket();
    if (fd == -1) {
        return -1;
    }
    // 2. socket 和本机ip 端口绑定
    int ret = bindSocket(fd, 9898);
    if (ret == -1) {
        return -1;
    }
    // 3. 监听客户端
    ret = listenCli(fd, 128);
    if (ret == -1) {
        return -1;
    }

    // 4. 阻塞等待客户端连接

    // 数据初始化
    int size = sizeof(sockinfos) / sizeof(sockinfos[0]);
    for (int i = 0; i < size; i++) {
        memset(&sockinfos[i], 0, sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }

    while (1) {
        // 创建子线程
        SockInfo* pInfo;
        for (int i = 0; i < size; i++) {
            // 使用一个未被使用的 sockinfo 作为当前子线程的信息存储
            if (sockinfos[i].fd == -1) {
                pInfo = &sockinfos[i];
                break;
            }
            // 若 sockinfos 已经被全部占用，则等待直到别的线程结束
            if (i == size - 1) {
                sleep(1);
                i--;
            }
        }
        int len = sizeof(struct sockaddr);
        pInfo->fd = acceptConn(fd, &pInfo->addr);
        if (pInfo->fd == -1) {
            return -1;
        }
        printf("主线程，文件描述符为 %d\n", pInfo->fd);
        pthread_create(&pInfo->tid, NULL, working, pInfo);
        pthread_detach(pInfo->tid);
    }
    close(fd);
    return 0;
}