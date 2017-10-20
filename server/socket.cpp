#include "socket.h"
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

Socket::Socket()
:_sockfd(ERRORFD)
{

}

Socket::~Socket()
{
    if(IsValid())
    {
        close(_sockfd);
    }
}

bool Socket::Create()
{
    _sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(!IsValid())
    {
        return false;
    }
    return true;
}

bool Socket::Bind(const int port)
{
    if(!IsValid())
    {   
        return false;
    }
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    _address.sin_port=htons(port);

    int bindReturn;
    bindReturn 
        = bind
                (
                _sockfd,
                (struct sockaddr*)&_address,
                sizeof(_address)
                );
    if(bindReturn == ERRORFD)
    {
        return false;
    }
    return true;
}

bool Socket::Listen() const
{
    if(!IsValid())
    {
        return false;
    }
    int listenReturn = listen(_sockfd,MAXCONNECTION);
    if(listenReturn == ERRORFD)
    {
        return false;
    }
    return true;
}

bool Socket::Accept(Socket& clientSocket) const
{
    int clientaddrLength = sizeof(clientSocket._address);
    clientSocket._sockfd 
        = ::accept
            (
            _sockfd,
            (struct sockaddr*)&clientSocket._address,
            (socklen_t *)&clientaddrLength
            );

    if(clientSocket._sockfd == ERRORFD)
    {
        return false;
    }
    return true;
}

bool Socket::Connect(const std::string& host,const int port)
{
    if(!IsValid())
    {
        return false;
    }      
    _address.sin_family=AF_INET;
    _address.sin_port=htons(port);
    _address.sin_addr.s_addr=inet_addr(host.c_str());

    int connectReturn 
        = ::connect
            (
            _sockfd,(struct sockaddr*)&_address,
            sizeof(_address)
            );
    if(connectReturn == ERRORFD)
    {
        return false;
    }   
    return true;
}

bool Socket::Send(Socket& socket,const std::string& message) const
{
    int result 
        =::send
                (
                socket._sockfd,
                message.c_str(),
                message.length(),
                MSG_NOSIGNAL
                );
    if(result == ERRORFD)
    {
        return false;
    }     
    return true;
}

int Socket::Receive(Socket& socket,std::string& message) const
{
    char buffer[MAXRECEIVE+sizeof(char)];
    message.clear();
    memset(buffer,0,MAXRECEIVE+sizeof(char));

    int numberRead
        =::recv
                (
                socket._sockfd,
                buffer,
                MAXRECEIVE,
                0
                );
        
    if(numberRead == ERRORFD)
    {
        std::cout<<"error in Socket::Receive\n" << std::endl;
        return -1;
    }
    else if(numberRead == 0)
    {
        return 0;
    }    
    else
    {
        message = buffer;
        return numberRead;
    }

}
int Socket::getfd()
{
    return _sockfd;
}

bool Socket::IsValid() const
{
    return _sockfd != ERRORFD;
}

