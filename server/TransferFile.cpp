#include "TransferFile.h"
#include <iostream>
#include <string>
#include "socket.h"

TransferFile::TransferFile()
:_file(NULL)
{

}

TransferFile::~TransferFile()
{
    if(FileIsVaild())
    {
        fclose(_file);
    }
}

bool TransferFile::FileOpenRead(const std::string & message)
{
    _file = fopen(message.c_str(),"r");

    if(!FileIsVaild())
    {
        std::cout << "open readfile error" << std::endl;
        return false;
    }
    return true;
}

bool TransferFile::FileOpenWrite(const std::string & message)
{
    _file = fopen(message.c_str(),"w+");

    if(!FileIsVaild())
    {
        std::cout << "open readfile error" << std::endl;
        return false;
    }
    return true;
}

bool TransferFile::FiLeRead(Socket& socket,std::string& message)
{
    char buff[100] = {0};
    message.clear();
    const int TS_PACKET_SIZE = 188;
    const int FILE_MAX_LEN = 64;
    int total = 0;
    int len;

    while(!feof(_file))
    {
        len = fread(buff,1,sizeof(buff),_file);
        total += len;
        message = buff;
        if(socket.Send(socket,message) == false)
        {
            perror("send file :");
            return false;
        }
    }
    std::cout << "send succeffful" << std::endl;
    return true;
}

bool TransferFile::FileWrite(Socket& socket,std::string& message)
{
    int len;
    int total = 0;
    std::string buff;
    while(1)
    {
        len = socket.Receive(socket,buff);
        if(-1 == len)
        {
           std::cout << "recvfile error" << std::endl;
           return false;
        }
        if(len == 0)
        {
            return true;
        }
        total += len;
        const char *buf = buff.c_str();
        cout << buf << endl;
        fwrite(buf,1,len,_file);
    }
}


bool TransferFile::FileIsVaild() const
{
    return _file != NULL;
}