#ifndef __TRANSFERFILE_H
#define __TRANSFERFILE_H

#include <stdio.h>

class Socket;

class TransferFile
{
public:
    TransferFile();
    ~TransferFile();
    bool FileOpenRead(const std::string & message);
    bool FileOpenWrite(const std::string & message);
    bool FiLeRead(Socket& socket,std::string& message);
    bool FileWrite(Socket& socket,std::string& message);
    bool FileIsVaild() const:
private:
    FILE *_file;
};

#endif