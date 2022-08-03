#include"connectToAgent.h"
#include<unistd.h>
#include<iostream>
#include<stdio.h>




#define CLIENT_SOCKET "client.sock"

#define BUFFER_SIZE 4096


struct sshMessage{
	char * message;
};



ConnectToAgent::ConnectToAgent(){
	std::cout<<"initialized the class connect to agent"<<std::endl;
}

ConnectToAgent::~ConnectToAgent(){
	std::cout<<"Connect to agent object destroyed"<<std::endl;
}



bool ConnectToAgent::getAddedIdentities(){
	std::cout<<"connectToAgent::getAddedIdentities "<<std::endl;
	std::cout<<unsigned(SSH_AGENTC_REQUEST_IDENTITIES)<<std::endl;
	bool ack = writeInt8ToAgent(SSH_AGENTC_REQUEST_IDENTITIES);	
	if(ack){
		ack = readInt8FromSSASocket();
		std::cout<<ack<<std::endl;
		ack = readInt32FromSSASocket();
		std::cout<<ack<<std::endl;
	}

	return ack;
}


bool ConnectToAgent::removeAllRSAAddedIdentities(){
	bool ack;
	std::cout<<"connectToAgent::RemoveAllRSAAddedIdentities ";
	std::cout<<unsigned(SSH_AGENTC_REMOVE_ALL_IDENTITIES)<<std::endl;
	ack = writeInt8ToAgent(SSH_AGENTC_REMOVE_ALL_RSA_IDENTITIES);	
	if(ack){
		ack = readFrom();
	}
	return ack;
}



bool ConnectToAgent::removeAllAddedIdentities(){		
	bool ack ;
	std::cout<<"connectToAgent::RemoveAllAddedIdentities "<<std::endl;
	std::cout<<unsigned(SSH_AGENTC_REMOVE_ALL_IDENTITIES)<<std::endl;
	ack = writeInt8ToAgent(SSH_AGENTC_REMOVE_ALL_IDENTITIES);	
	if(ack){
		ack = readFrom();
	}
	return ack;
}


bool ConnectToAgent::readFrom(){
	bool ack = readInt8FromSSASocket();
	std::cout<<ack<<std::endl;
	ack = readInt32FromSSASocket();
	std::cout<<ack<<std::endl;
	return ack;
}
 	
bool ConnectToAgent::writeInt8ToAgent(int8 en){
	int dataTypeSize = sizeof(en);
	const char * message = reinterpret_cast<const char *>(&en);
	return prepareMessage(dataTypeSize, (char *)message);
}	


bool ConnectToAgent::writeInt32ToAgent(int en){
	int dataTypeSize = sizeof(en);
	const char * message = reinterpret_cast<const char *>(&en);
	return prepareMessage(dataTypeSize, (char *)message);
}

bool ConnectToAgent::prepareMessage(int dataTypeSize, char * message){
	// const char * messageLength = reinterpret_cast<const char *>(&dataTypeSize);
	const char * messageLength = (const char *)(&dataTypeSize);
	
	int totalMessageLength = sizeof(int);
	char * p = (char *)malloc(totalMessageLength);
	
	strcpy(&p[0],messageLength );
	int ack = writeContentToSSASocket( (char *)message, totalMessageLength);

	if(ack)	{
		std::cout<<"ConnectToAgent::writeInt8ToAgent message is "<<messageLength<<" total message length "<<totalMessageLength<<std::endl;
		free(p);

		totalMessageLength = sizeof(int8);
		p = (char *)malloc(sizeof(totalMessageLength));
		strcpy(&p[0],message);
		
		ack = writeContentToSSASocket( (char *)message, totalMessageLength);
		
		if(ack)
			std::cout<<"ConnectToAgent::writeInt8ToAgent message is "<<message<<" total message length "<<totalMessageLength<<std::endl;
		else
			std::cout<<"error in writing message to ssh-agent server"<<std::endl;
		
	}
	else
		std::cout<<"error in writing message-length to ssh-agent server"<<std::endl;
	

	free(p);
	return ack;
}

 

char * ConnectToAgent::readInt8FromSSASocket(){
	int sizeOfMessage = sizeof(int8);
	return readFromSSASocket(sizeOfMessage);
}


char * ConnectToAgent::readInt32FromSSASocket(){
	int sizeOfMessage = sizeof(int);
	return readFromSSASocket(sizeOfMessage);
}

char * ConnectToAgent::readFromSSASocket(int sizeOfMessage){
	int valRead = 1;
	char * buffer = (char *)malloc(sizeOfMessage);
	bzero(buffer, sizeOfMessage);
	valRead = read(socketFd, buffer, sizeOfMessage);
	if(valRead >= 0 ){
		std::cout<<"data read is "<<buffer<<std::endl;
	}
	else{
		std::cout<<"error in reading data "<<std::endl;
		exit(1);
	}
	return buffer;
}


int ConnectToAgent::writeContentToSSASocket(char * stream, int sizeOfStream){	
	int ack ;
	// ack =  write(socketFd, stream, sizeOfStream);//buffer, strlen(buffer));
	ack = send(socketFd, stream, sizeOfStream, 0);
	if(ack >= 0)
		std::cout<<"ConnectToAgent::writeContentToSSASocket content written "<<stream<<std::endl;	
	else
		exit(1);
	return ack;

	
}
bool ConnectToAgent::connectSocket(const char * sockAddr){
	int r = 1;
	struct sockaddr_un sock_addr;
	std::cout<<"ConnectToAgent::connectSocket. Path of socket is "<<sockAddr<<std::endl;	
	socketFd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(socketFd < 1){
		std::cout<<"error creating client FD"<<std::endl;
		r = 0;
	}
	else
		std::cout<<"Connecting socket file descriptor "<<socketFd<<std::endl;
	
	
	if(r){
		bzero((char *)&sock_addr, sizeof(sock_addr) );
		
		// bzero(sock_addr, sizeof(sock_addr) );
		sock_addr.sun_family = AF_UNIX;
		strcpy(sock_addr.sun_path , CLIENT_SOCKET);
		std::cout<<sock_addr.sun_path<<" "<<sock_addr.sun_family<<std::endl;
		unlink(CLIENT_SOCKET);
		if(bind(socketFd, (struct sockaddr *)&sock_addr, sizeof(sock_addr) )< 0 ){
			r = 0;
			std::cout<<"Could not bind with local client path"<<std::endl;
		}
	}

	if(r){
		// bzero((char *)sock_addr, sizeof(sock_addr) );
		bzero(&sock_addr, sizeof(sock_addr) );
		sock_addr.sun_family = AF_UNIX;
		strcpy(sock_addr.sun_path , sockAddr);
		
		std::cout<<"path is "<<sock_addr.sun_path<<std::endl;
		std::cout<<"sun family is "<<sock_addr.sun_family<<std::endl;	

		clientSocketFd = connect(socketFd, (struct sockaddr *)& sock_addr, sizeof(sock_addr) );
		if(clientSocketFd < 0){
			r = 0;
			std::cout<<"error in connecting to ssh agent server"<<std::endl;
		}
		else
			std::cout<<"connected to ssh agent server "<<clientSocketFd<<std::endl;
	}
	return r;
}

void ConnectToAgent::closeConnection(){
	close(clientSocketFd);
}

