#include"connectToAgent.h"

#include<iostream>

ConnectToAgent::ConnectToAgent(){
	std::cout<<"initialized the class connect to agent"<<std::endl;
}

ConnectToAgent::~ConnectToAgent(){
	std::cout<<"object destroyed"<<std::endl;
}

void ConnectToAgent::connectBySocket(char * sockAddr){
	int socketFd ;
	struct sockaddr_un sock_addr;
	
	socketFd = socket(AF_UNIX, SOCK_DGRAM, 0);
	
	if(socketFd < 1)
		std::cout<<"error connecting to server"<<std::endl;

	
	sock_addr.sun_family = AF_UNIX;
	strcpy(sock_addr.sun_path, sockAddr);
	char buffer[128];
	memset(buffer, '0' , sizeof(buffer));
	int servLen = sizeof(sock_addr.sun_path) + sizeof(sock_addr.sun_family);	
	int connection = connect(socketFd, (struct sockaddr *) &sock_addr, servLen );
	std::cout<<connection;
	

}

