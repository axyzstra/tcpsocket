#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include "TcpSocket.h"
using namespace std;

int main() {
    TcpSocket tcp;
    int ret = tcp.connectToHost("124.223.59.159", 9898);
    if (ret == -1) {
        return -1;
    }
    int fd1 = open("LQ.txt", O_RDONLY);
    int len = 0;
    char tmp[128];
    memset(tmp, 0, sizeof(tmp));
    while ((len = read(fd1, tmp, sizeof(tmp))) > 0) {
        tcp.sendMsg(string(tmp, len));
        cout << "send Msg" << endl;
        cout << tmp << endl << endl << endl << endl;
        memset(tmp, 0, sizeof(tmp));
        usleep(300);
    }
    sleep(10);
    return 0;
}