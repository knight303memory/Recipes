//
// Created by Killua on 2017/11/28.
//

#ifndef LIBEVENTTEST_CREATESOCKET_H
#define LIBEVENTTEST_CREATESOCKET_H
#include <sys/socket.h>
//ipv4
#include <netinet/in.h>
//地址转换
#include <arpa/inet.h>
//名称解析
#include <netdb.h>


class SocketAddress
{
public:
    SocketAddress(uint32_t inAddress, uint16_t inPort)
    {
        GetAsSockAddrIn()->sin_family = AF_INET;
        GetAsSockAddrIn()->sin_addr.s_addr = htonl(inAddress);
        GetAsSockAddrIn()->sin_port = htons(inPort);
    }

    SocketAddress(const sockaddr &inSockAddr)
    {
        memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
    }

    size_t GetSize() const {return sizeof(sockaddr);}

private:
    sockaddr mSockAddr;
    friend class UDPSocket;
    sockaddr_in *GetAsSockAddrIn()
    {
        return reinterpret_cast<sockaddr_in *>(&mSockAddr);
    }

};

typedef shared_ptr<SocketAddress> SocketAddressPtr;

#endif //LIBEVENTTEST_CREATESOCKET_H
