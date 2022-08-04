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
	
	int ack = writeInt8ToAgent(SSH_AGENTC_REQUEST_IDENTITIES);	
	int32 lengthOfResponse;

	char * buffer;
	
	if(ack){

		// ack = readCompletePacket(buffer); 	
		// read the length of response packet
		ack - readInt8FromSSASocket(buffer);
		// std::cout<<ack<<std::endl;
	
		ack = readInt32FromSSASocket(&lengthOfResponse, buffer);
		// // read the packet
		// free(buffer);
		// if(ack){
		// 	ack = readFromSSASocket(lengthOfResponse, buffer);
		// 	std::cout<<ack<<std::endl;
		// }
	}

	return ack;
}


bool ConnectToAgent::removeAllRSAAddedIdentities(){
	bool ack;
	std::cout<<"connectToAgent::RemoveAllRSAAddedIdentities ";
	std::cout<<unsigned(SSH_AGENTC_REMOVE_ALL_IDENTITIES)<<std::endl;

	char * buffer;

	ack = writeInt8ToAgent(SSH_AGENTC_REMOVE_ALL_RSA_IDENTITIES);	
	if(ack){
		ack = readFrom(buffer);
	}
	return ack;
}



bool ConnectToAgent::removeAllAddedIdentities(){		
	bool ack ;
	std::cout<<"connectToAgent::RemoveAllAddedIdentities "<<std::endl;
	std::cout<<unsigned(SSH_AGENTC_REMOVE_ALL_IDENTITIES)<<std::endl;
	
	char * buffer;

	ack = writeInt8ToAgent(SSH_AGENTC_REMOVE_ALL_IDENTITIES);	


	if(ack){
		ack = readFrom(buffer);
	}
	return ack> 0 ? true: false;
}


 	
int ConnectToAgent::writeInt8ToAgent(int8 en){
	int32 packetSize = sizeof(en);
	std::cout<<"ConnectToAgent::writeInt8ToAgent---packet size send to server "<<packetSize<<std::endl;
	const char * message = reinterpret_cast<const char *>(&en);
	return prepareMessageAndSend(packetSize, (char *)message);
}	


int ConnectToAgent::writeInt32ToAgent(int en){
	int32 packetSize = sizeof(en);
	const char * message = reinterpret_cast<const char *>(&en);
	return prepareMessageAndSend(packetSize, (char *)message);
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



bool ConnectToAgent::prepareMessageAndSend(int32 packetSize, char * message){
	const char * messageLength = (std::to_string(packetSize)).c_str();//(const char *)(&packetSize);

	int ack = 1;
	std::cout<<"bool ConnectToAgent::prepareMessageAndSend(int32 packetSize, char * message) messageLength "<<sizeof(messageLength)<<std::endl;

	// send packet size
	// ack = reserveBufferAndSend(sizeof(packetSize) , (char *)messageLength);
	if(ack)	{
		// send packet
		ack = reserveBufferAndSend(packetSize, message);
	}
	else
		std::cout<<"error in writing message-length to ssh-agent server"<<std::endl;
	
	return ack;
}

 
bool ConnectToAgent::reserveBufferAndSend(int32 messageLength, char * message){

	// char * p = (char *)malloc(messageLength);
	int ack;
	// strcpy(&p[0],message);

	ack = writeContentToSSASocket( message, messageLength);
	if (ack)
		std::cout<<"ConnectToAgent::writeInt8ToAgent message "<<message<<" total message length "<<messageLength<<std::endl;
	else
		std::cout<<"error in writing to socket"<<std::endl;

	// free(p);
	return ack;

}

int ConnectToAgent::readInt8FromSSASocket(char * buffer){
	int sizeOfMessage = sizeof(int8);
	return readFromSSASocket(sizeOfMessage, buffer);
}


int ConnectToAgent::readInt32FromSSASocket(int32 * lengthOfResponse, char * buffer){
	int sizeOfMessage = sizeof(int32);
	int ack;
	
	ack = readFromSSASocket(sizeOfMessage, buffer);
	if(ack){
		*lengthOfResponse = atoi(buffer);
	}
	else{
		return 0;
	}
}


int ConnectToAgent::readFrom(char * buffer){
	int ack = readInt8FromSSASocket(buffer);
	int32 lengthOfResponse;
	std::cout<<ack<<std::endl;
	ack = readInt32FromSSASocket(&lengthOfResponse, buffer);
	std::cout<<ack<<std::endl;
	return ack;
}

int ConnectToAgent::readCompletePacket(char * buffer){
	int32 maxsize = 256 * 1024;
	free(buffer);
	buffer = (char *)malloc(sizeof(maxsize));
	int valRead = read(socketFd, buffer, maxsize);
}





int ConnectToAgent::readFromSSASocket(int32 sizeOfMessage, char * buffer){
	int valRead = 1;
	buffer = (char *)malloc(sizeOfMessage);
	bzero(buffer, sizeOfMessage);
	valRead = read(socketFd, buffer, sizeOfMessage);
	if(valRead >= 0 ){
		std::cout<<"data read is "<<buffer<<std::endl;
	}
	else{
		std::cout<<"error in reading data "<<std::endl;
		exit(1);
	}
	return true;
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

