#ifndef __SOCKET_H
#define __SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

const int MAXCONNECTION = 5;
const int MAXRECEIVE = 500;
const int ERRORFD = -1;

class Socket
{
public:
    Socket();
    virtual ~Socket();
    bool Create();
    bool Bind(const int port);
    bool Listen() const;
    bool Accept(Socket & ClientSocket) const;
    bool Connect(const std::string& host,const int port);
    bool Send(Socket& socket,const std::string& message) const;
    int Receive(Socket& socket,std::string& message) const;
    bool IsValid() const;
    int getfd();
private:
    int _sockfd;
    struct sockaddr_in _address;
};

#endif