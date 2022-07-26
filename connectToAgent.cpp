#include"connectToAgent.h"
#include<unistd.h>
#include<iostream>
#include<stdio.h>

ConnectToAgent::ConnectToAgent(){
	std::cout<<"initialized the class connect to agent"<<std::endl;
}

ConnectToAgent::~ConnectToAgent(){
	std::cout<<"Connect to agent object destroyed"<<std::endl;
}



bool ConnectToAgent::getAddedIdentities(){
	std::cout<<"connectToAgent::getAddedIdentities "<<std::endl;
	writeInt8ToAgent(SSH_AGENTC_REQUEST_IDENTITIES);	
	return true;
}



bool ConnectToAgent::writeInt8ToAgent(uint8_t en){
	std::cout<<"ConnectToAgent::writeInt8ToAgent message is "<<(int)en<<std::endl;
	// message = reinterpret_cast<char *>(&en);
	char * message = reinterpret_cast<char *> (&en);
	std::cout<<"message turned "<<message<<strlen(message)<<std::endl;
	writeContentToSSASocket(message);
	return true;
}


void ConnectToAgent::writeContentToSSASocket(char * stream){
	
	char buffer [1024];
	std::cout<<"ConnectToAgent::writeContentToSSASocket "<<stream<<std::endl;
	//send(socketFd, stream, strlen(stream), 0);
	bzero(buffer, sizeof(buffer));

	// strncpy(buffer, stream, sizeof(buffer)-1) );
	strncpy(buffer, stream, sizeof stream);
	buffer[sizeof stream ] = '\0';

	std::cout<<buffer<<std::endl;

	int n =  write(socketFd, buffer, strlen(buffer));
	std::cout<<"ConnectToAgent::writeContentToSSASocket content written "<<stream<<std::endl;

	if(n < 0)
	{
		std::cout<<"bug in writing to socket"<<std::endl;
	}
	bzero(buffer,sizeof(buffer));
	
	int valRead ;
	while( valRead = read(socketFd, buffer, sizeof buffer - 1) > 0 )
		std::cout<<"response from ssh agent "<< valRead << " and response "<< buffer<<std::endl;
}

void ConnectToAgent::connectSocket(char * sockAddr){
	
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
	bzero(buffer , sizeof(buffer));
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

