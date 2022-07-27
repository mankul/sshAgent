#include "serverSocket.h"
#include<iostream>
#include<unistd.h>


#define SERVER_SOCKET "server.sock"
#define BUFFER_SIZE 1024

ServerSocket::ServerSocket(){
    std::cout<<"server socket initialized"<<std::endl;
}

ServerSocket::~ServerSocket(){
    std::cout<<"server socket closed"<<std::endl;
}

bool ServerSocket::initializeServer(){

    sockaddr_un sock_address;
    serverSocketFD = socket(AF_UNIX, SOCK_STREAM,0 );
    int r = 1;
    if(serverSocketFD < 0){
        r = 0;
        std::cout<<"server socket not created"<<std::endl;
    } 
    if(r){
        bzero((char *)&sock_address, sizeof(sock_address));
        sock_address.sun_family = AF_UNIX;
        strcpy(sock_address.sun_path , SERVER_SOCKET);
        unlink(SERVER_SOCKET);
        if(bind(serverSocketFD, (struct sockaddr *)&sock_address, sizeof(sock_address)) < 0 )
        {
            std::cout<<"error in bindind with server socket "<<std::endl;
            r = 0;
        }
    }
    if(r){
        if(listen(serverSocketFD, 5) < 0){
            r = 0;
            std::cout<<"server socket, error in listening"<<std::endl;
        }
    }
    if(r){
        int newSocket = accept(serverSocketFD, (struct sockaddr *)&sock_address, (socklen_t *)&sock_address);
        if(newSocket < 0){
            r = 0;
            std::cout<<"server not accepting socket"<<std::endl;
        }
        else{
            char buffer[BUFFER_SIZE];
            while (1)
            {
                int valRead = read(newSocket, buffer, sizeof(buffer) );
                std::cout<<"value received is "<<buffer<<std::endl;
                write(newSocket, buffer, sizeof(buffer));
            }

        }
    }
    return true;
}


int main(){
    ServerSocket s;
    s.initializeServer();
}