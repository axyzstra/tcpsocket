#include "TcpSocket.h"

TcpSocket::TcpSocket()
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::TcpSocket(int socket)
{
    m_fd = socket;
}

TcpSocket::~TcpSocket()
{
    if (m_fd > 0) {
        close(m_fd);
    }
}

int TcpSocket::connectToHost(string ip, unsigned short port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.data(), &addr.sin_addr.s_addr);
    addr.sin_port = htons(port);
    int ret = connect(m_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect fail...\n");
        return -1;
    }
    cout << "和服务端建立连接成功...\n" << endl;
    return ret;
}

int TcpSocket::sendMsg(string msg)
{
    int len = msg.size();
    char* buf = new char[len + 4];
    int bigLen = htonl(len);
    memcpy(buf, &bigLen, 4);
    memcpy(buf + 4, msg.data(), len);
    int ret = writen(buf, len + 4);
    delete[] buf;
    return ret;
}

string TcpSocket::recvMsg()
{
    int len = 0;
    readn((char*)&len, 4);
    len = ntohl(len);
    cout << "信息大小为" << len << endl;
    char* buf = new char[len + 1];
    int ret = readn(buf, len);
    if (ret != len) {
        return string();
    }
    buf[len] = '\0';
    string res(buf);
    delete[] buf;
    return res;;
}

int TcpSocket::readn(char *buf, int size)
{
    char* p = buf;
    int count = size;
    while (count > 0) {
        int ret = recv(m_fd, p, count, 0);
        if (ret == -1) {
            perror("recv fail...\n");
            return -1;
        } else if (ret == 0) {
            continue;
        }
        count -= ret;
        p += ret;
    }
    return size;
}

int TcpSocket::writen(const char *msg, int size)
{
    const char *p = msg;
    int count = size;
    while (count > 0) {
        int ret = write(m_fd, p, count);
        if (ret == -1) {
            return -1;
        } else if (ret == 0) {
            continue;
        }
        count -= ret;
        p += ret;
    }
    return size;
}
