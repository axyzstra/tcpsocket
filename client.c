#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "socket.h"

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        return -1;
    }
    int ret = connectHost(fd, "124.223.59.159", 9898);
    if (ret == -1) {
        perror("connect fail...\n");
    }
    // 开始通信
    int fd1 = open("LQ.txt", O_RDONLY);
    int length = 0;
    char tmp[128];
    while ((length = read(fd1, tmp, sizeof(tmp))) > 0) {
        sendMessage(fd, tmp, length);
        memset(tmp, 0, sizeof(tmp));
        usleep(300);
        // sleep(3);
    }
    sleep(10);
    closeSocket(fd);
    return 0;
}