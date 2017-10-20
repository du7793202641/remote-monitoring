#ifndef __SERVERSOCKET_H
#define __SERVERSOCKET_H

#include "socket.h"

class ServerSocket:public Socket
{
public:
        ServerSocket(const int port);
        ServerSocket();
        virtual ~ServerSocket();

        void Accept(Socket& socket);
};

#endif