#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <QDebug>

class Socket
{
public:
    Socket();
    ~Socket();

    void Connect();
    void Disconnect();
    void Send();

private:
    WSADATA wsaData;
    SOCKET cSocket;
    SOCKADDR_IN addr;
};
