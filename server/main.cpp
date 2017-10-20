#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "ServerSocket.h"
//#include "TransferFile.h"

using namespace std;

void CommandToSee(string &message,Socket* socket);
void ReceiveFile(string &message,Socket* socket);
void SendFile(string &message,Socket* socket);
void photovFile(string &message,Socket* socket);

int flag = 1;

map<string,Socket> gmapClient;

void* ProcessMessage(void *arg)
{
    string message ;
    Socket* clientSocket = static_cast<Socket*>(arg);
    clientSocket->Receive(*clientSocket,message);

    cout << message << " user successful connection" << endl;
    gmapClient[message] = *clientSocket;
    int one = flag;
    while(true&&one)
    {   
        flag--;
        cout << "specify a user" << endl;
        getline(cin,message);
        if( gmapClient.find(message) != gmapClient.end())
        {
            int commad;
            cout << "please choose function" << endl;
            cin >> commad;
            getchar();
            switch(commad)
            {
                case 1:
                {
                    CommandToSee(message,clientSocket);
                    break;
                }
                case 2:
                {
                    string str = "sendfile";
                    clientSocket->Send(gmapClient[message],str);
                    ReceiveFile(message,clientSocket);
                    break;
                }
                case 3:
                {
                    string str = "recvfile";
                    clientSocket->Send(gmapClient[message],str);
                    SendFile(message,clientSocket);
                }
                case 4:
                {
                    string str = "photofile";
                    clientSocket->Send(gmapClient[message],str);
                    photoFile(message,clientSocket);
                }
                default:
                    break;
            }
        }
        else
        {
            cout << "input name error" << endl;
        }
        if(message == "quit")
            break;
    }
    pthread_exit(NULL);
    flag++;
    return NULL;
}

int main()
{
    cout<<"Running server..."<<endl;
    
    const int port = 8080;
    ServerSocket server(port);

    while(true)
    {
        Socket *newSocket = new Socket;
        server.Accept(*newSocket);
        
        pthread_t newThread;
        int result = pthread_create(&newThread,NULL,ProcessMessage,static_cast<void*>(newSocket));
        if(result !=  0)
        {
            return 1;
        }  
    }
    return 0;
}

void CommandToSee(string &message,Socket* socket)
{
    string str;
    getline(cin,str);
    socket->Send(gmapClient[message],str);
    while(1)
    {   
        char buff[500] = {0};
        int len = recv(gmapClient[message].getfd(),buff,sizeof(buff),0);
        if(len == 0)
            break;
        if(len < 0)
            break;
        cout << buff << endl;
        if(len < sizeof(buff))
            break;
    }
}

void ReceiveFile(string &message,Socket* socket)
{
    FILE *file = NULL;
    file = fopen("test1.txt","w");
    while(1)
    {   
        char buff[500] = {0};
        int len = recv(gmapClient[message].getfd(),buff,sizeof(buff),0);
        if(len == 0)
            break;
        if(len < 0)
            break;
        //cout << buff << endl;
        fwrite(buff,1,len,file);
        if(len < sizeof(buff))
            break;
    } 
    cout << "file have get" << endl;
    fclose(file);
}

void photoFile(string &message,Socket* socket)
{
    FILE *file = NULL;
    file = fopen("1.png","w");
    while(1)
    {   
        char buff[500] = {0};
        int len = recv(gmapClient[message].getfd(),buff,sizeof(buff),0);
        if(len == 0)
            break;
        if(len < 0)
            break;
        //cout << buff << endl;
        fwrite(buff,1,len,file);
        if(len < sizeof(buff))
            break;
    } 
    cout << "photo have get" << endl;
    system("feh 1.png");
    fclose(file);
}

void SendFile(string &message,Socket* socket)
{
    FILE *file = NULL;
    file = fopen("demo","r");
    if(file == NULL)
        exit(1);
    fseek(file,0,SEEK_SET);
    while(!feof(file))
    {
        char buff[500] = {0};
        int len = fread(buff,1,sizeof(buff),file);
        if(send(gmapClient[message].getfd(),buff,len,0) < 0)
        {
            perror("send file :");
            break;
        }
        if(len < sizeof(buff))
            break;
    }
    fclose(file);
}
