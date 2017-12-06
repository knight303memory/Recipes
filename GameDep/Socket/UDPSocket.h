//
// Created by Killua on 2017/11/29.
//

#ifndef LIBEVENTTEST_UDPSOCKET_H
#define LIBEVENTTEST_UDPSOCKET_H
#include <unistd.h>
#include "SocketAddressFactory.h"
class UDPSocket
{
public:
    ~UDPSocket();
    int Bind(const SocketAddress &inToAddress);
    int SendTo(const void *inData, int inLen, const SocketAddress &inTo);
    int ReceiveFrom(const void *inBuffer, int inLen, SocketAddress &outFrom);
    int SetNoBlockingMode(bool inShouldBeNonBlocking);
private:
   friend class SocketUtil;
    UDPSocket(int inSocket) : mSocket(inSocket){}
    int mSocket;
};


typedef shared_ptr<UDPSocket> UDPSocketPtr;

int UDPSocket::Bind(const SocketAddress &inBindAddress) {
    int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());

}

int UDPSocket::SendTo(const void *inData, int inLen, const SocketAddress &inTo)
{
    int byteSentCount = sendto(mSocket, static_cast<const char*>(inData), inLen, 0, &inTo.mSockAddr, inTo.GetSize());

    if(byteSentCount >= 0)
    {
        return byteSentCount;
    }
}

int UDPSocket::ReceiveFrom(const void *inBuffer, int inLen, SocketAddress &outFrom)
{
    int formLength = outFrom.GetSize();
    int readByteCount = recvfrom(mSocket, static_cast<char *>(inBuffer), inLen, 0, &outFrom.mSockAddr, &formLength);

    if(readByteCount >= 0)
    {
        return readByteCount;
    }
}

int UDPSocket::SetNoBlockingMode(bool inShouldBeNonBlocking) {
#if _WIN32
    u_long arg = inShouldBeNonBlocking ? 1 : 0;
    int result = ioctlsocket(mSocket, FIONBIO, &arg);
#else
    int flags = fcntl(mSocket, F_GETFL, 0);
    flags = inShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    int result = fcntl(mSocket, F_SETFL, flags);
#endif

    return 0;
}

UDPSocket::~UDPSocket() {
    close(mSocket);
}


enum SocketAddressFamily
{
    INET = AF_INET,
    INET6 = AF_INET6
};

class SocketUtil
{

    UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily)
    {
        int s = socket(inFamily, SOCK_STREAM, IPPROTO_UDP);

        if(s > 0)
        {
            return UDPSocketPtr(new UDPSocket(s));
        }

    }
};


void DoGameLoop()
{
    UDPSocket mySock = SocketUtil::CreateUDPSocket(INET);

    mySock.SetNoBlockingMode(true);

    while (gISGameRunning)
    {
        char data[1500];
        SocketAddress socketAddress;

        int bytesReceived = mySock.ReceiveFrom(data, sizeof(data), socketAddress);

        if(bytesReceived > 0)
        {
            ProcessReveivedData(Data, bytesReceived, socketAddress);
        }

        DoGameFrame();
    }
}

#endif //LIBEVENTTEST_UDPSOCKET_H
