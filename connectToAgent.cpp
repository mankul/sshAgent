#include"connectToAgent.h"
#include<unistd.h>
#include<iostream>

ConnectToAgent::ConnectToAgent(){
	std::cout<<"initialized the class connect to agent"<<std::endl;
}

ConnectToAgent::~ConnectToAgent(){
	std::cout<<"Connect to agent object destroyed"<<std::endl;
}




void ConnectToAgent::sendSSHContentToAgent(const char * stream){
	
	char buffer [1024];
	send(socketFd, stream, strlen(stream), 0);
	int valRead = read(socketFd, buffer, 1024);
	std::cout<<"response from ssh agent "<< valRead << " and response "<< buffer<<std::endl;
}

void ConnectToAgent::connectBySocket(char * sockAddr){
	
	socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
	sock_addr = new sockaddr_un();
	if(socketFd < 1)
		std::cout<<"error connecting to server"<<std::endl;
	else
		std::cout<<"Connecting socket file descriptor "<<socketFd<<std::endl;
	
	bzero((char *)sock_addr, sizeof(sock_addr) );


	sock_addr->sun_family = AF_UNIX;
	strcpy(sock_addr->sun_path, sockAddr);
	
	char buffer[128];
	memset(buffer, '0' , sizeof(buffer));
	int servLen = sizeof(sock_addr->sun_path) + sizeof(sock_addr->sun_family);	
	int clientSocketFd = connect(socketFd, (struct sockaddr *) sock_addr, servLen );
	if(clientSocketFd < 0)
		std::cout<<"error in connecting to ssh agent server"<<std::endl;
	else
		std::cout<<"connected to ssh agent server "<<clientSocketFd<<std::endl;


}

void ConnectToAgent::closeConnection(){
	close(clientSocketFd);
}

