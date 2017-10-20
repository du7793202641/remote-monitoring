#include "ClientSocket.h"
#include <iostream>
#include <stdlib.h>

ClientSocket::ClientSocket(const std::string& host,const int port)
{
    if(!Socket::Create())
    {
        std::cout<< "Could not create client socket." << std::endl;
    }
    if(!Socket::Connect(host,port))
    {
        std::cout << "Could not connect to port." << std::endl;        
    }
}

ClientSocket::~ClientSocket()
{}

bool ClientSocket::Send(const std::string& message)
{
    return Socket::Send(static_cast<Socket&>(*this),message);
}

int ClientSocket::Receive(std::string& message)
{
    return Socket::Receive(static_cast<Socket&>(*this),message);
}