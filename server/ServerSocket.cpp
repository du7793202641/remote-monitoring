#include "ServerSocket.h"
#include <stdlib.h>
#include <iostream>

ServerSocket::ServerSocket(const int port)
{
    if(!Socket::Create())
    {
        std::cout << "Could not create server socket." << std::endl;
        exit(1);
    }

    if(!Socket::Bind(port))
    {
        std::cout << "Could not bind to port." << std::endl;
        exit(1);
    }

    if(!Socket::Listen())
    {
        std::cout << "Could not listen to socket." << std::endl;
        exit(1);
    }
}

ServerSocket::~ServerSocket()
{
}

void ServerSocket::Accept(Socket& socket)
{
    if(!Socket::Accept (socket))
    {
        std::cout<< "Could not accept socket." << std::endl;
        exit(1);
    }
}