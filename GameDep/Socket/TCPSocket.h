//
// Created by Killua on 2017/11/29.
//

#ifndef LIBEVENTTEST_TCPSOCKET_H
#define LIBEVENTTEST_TCPSOCKET_H

#include <memory>
#include "CreateSocket.h"
#include <sys/select.h>
#include <vector>

class TCPSocket
{
public:
    ~TCPSocket();
    int Connect(const SocketAddress &inAddress);
    int Bind(const SocketAddress &inToAddress);
    int Listen(int inBackLog = 32);
    std::shared_ptr<TCPSocket> Accept(SocketAddress &inFromAddress);
    int Send(const void *inData, int inLen);
    int Receive(void *inBuffer, int inLen);

private:
    friend class SocketUtil;
    TCPSocket(int inSocket) : mSocket(inSocket){}
    int mSocket;
};

typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

int TCPSocket::Connect(const SocketAddress &inAddress) {
    int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
    return 0;
}


int TCPSocket::Listen(int inBackLog) {
    int err = listen(mSocket, inBackLog);
    return 0;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress &inFromAddress) {
    int length = inFromAddress.GetSize();
    int newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);

    if(newSocket != -1)
    {
        return TCPSocketPtr(new TCPSocket(newSocket));
    }
}


int TCPSocket::Send(const void *inData, int inLen) {
    int bytesSentCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);

    return bytesSentCount;
}

int TCPSocket::Receive(void *inData, int inLen) {
    int bytesReceivedCount = recv(mSocket, static_cast<char *>(inData), inLen, 0);

    if(bytesReceivedCount < 0)
    {

    }
    return bytesReceivedCount;
}


enum SocketAddressFamily
{
    INET = AF_INET,
    INET6 = AF_INET6
};
class SocketUtil
{

    TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily)
    {
        int s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
        return TCPSocketPtr(new TCPSocket(s));
    }

    fd_set *FillSetFromVector(fd_set & outSet, const vector<TCPSocketPtr> *inSockets)
    {
        if(inSockets)
        {
            FD_ZERO(&outSet);
            for(const TCPSocketPtr &socket : *inSockets)
            {
                FD_SET(socket->mSocket, & outSet);
            }
            return &outSet;
        } else{
            return nullptr;
        }
    }


    void FillVectorFromSet(std::vector<TCPSocketPtr> *outSockets, const vector<TCPSocketPtr>* inSockets, const fd_set& inSet)
    {
        if(inSockets && outSockets)
        {
            outSockets->clear();
            for(const TCPSocketPtr &socket : *inSockets)
            {
                if(FD_ISSET(socket->mSocket, &inSet))
                {
                    outSockets->push_back(socket);
                }
            }
        }
    }


    int Select(const std::vector<TCPSocketPtr> *inReadSet, std::vector<TCPSocketPtr> *outReadSet,
    const std::vector<TCPSocketPtr> *inWriteSet, std::vector<TCPSocketPtr>* outWriteSet,
    const std::vector<TCPSocketPtr> *inExceptSet, std::vector<TCPSocketPtr>* outExceptSet)
    {
        fd_set read, write, except;
        fd_set *readPtr = FillSetFromVector(read, inReadSet);
        fd_set *writePtr = FillSetFromVector(write, inWriteSet);
        fd_set *exceptPtr = FillSetFromVector(except, inExceptSet);

        int toRet = select(0, readPtr, writePtr, exceptPtr, nullptr);

        if(toRet > 0)
        {
            FillVectorFromSet(outReadSet, inReadSet, read);
            FillVectorFromSet(outWriteSet, inWriteSet, write);
            FillVectorFromSet(outExceptSet, inExceptSet, except);
        }
    }



};


void DoTcpLoop()
{
    TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);

    SocketAddress recevingAddress(INADDR_ANY, 48000);
    if(listenSocket->Bind(recevingAddress) != -1)
    {
        return;
    }

    std::vector<TCPSocketPtr> readBlockSoctets;
    readBlockSoctets.push_back(listenSocket);

    std::vector<TCPSocketPtr> readableSockets;

    while(gIsGameRunning)
    {
        if(SocketUtil::Select(&readBlockSoctets, &readableSockets, nullptr, nullptr, nullptr, nullptr))
        {
            for(const TCPSocketPtr &socket : readableSockets)
            {
                if(socket == listenSocket)
                {
                    SocketAddress newClientAddress;

                    auto newSocket = listenSocket->Accept(newClientAddress);
                    readBlockSoctets.push_back(newSocket);
                    ProcessNewClient(newSocket, newClientAddress);
                }

                else
                {
                    char segment[GOOD_SEGMENT_SIZE];
                    int dataReceived = socket->Receive(segment, GOOD_SEGMENT_SIZE);
                    if(dataReceived > 0)
                    {
                        ProcessDataFromClient(socket, segment, dataReceived);
                    }

                }
            }
        }
    }
}

#endif //LIBEVENTTEST_TCPSOCKET_H
