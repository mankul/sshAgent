#include"connectToAgent.h"
#include<unistd.h>
#include<iostream>
#include<stdio.h>


#define CLIENT_SOCKET "client.sock"

#define MAX_BUFFER_SIZE 4096


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


		ack = readFrom(buffer);

		// ack = readCompletePacket(buffer); 	
		// read the length of response packet
		// ack = readInt8FromSSASocket(buffer);
		// std::cout<<ack<<std::endl;
	
		// ack = readInt32FromSSASocket(&lengthOfResponse, buffer);
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
	// const char * messageLength = (std::to_string(packetSize)).c_str();//(const char *)(&packetSize);
	int size = sizeof (int32) + sizeof (message);
	char buffer[size];
	convertIntToStream(packetSize, buffer);
	int ack = 1;
	std::cout<<"bool ConnectToAgent::prepareMessageAndSend(int32 packetSize, char * message) messageLength "<<size<<std::endl;



	// strcpy(&buffer[4], message);
	memcpy(&buffer[4], message, sizeof(message) );

	// send packet size

	ack = reserveBufferAndSend(sizeof(buffer) , buffer);




	std::cout<<"acknowledgement is "<<ack<<std::endl;
	// if(ack)	{
		// send packet
		// ack = reserveBufferAndSend(packetSize, message);
	// }
	// else
		// std::cout<<"error in writing message-length to ssh-agent server"<<std::endl;	
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
		if(buffer != NULL){
			std::cout<<"readIn32FromSSASocket size of message is "<<std::endl;
			*lengthOfResponse = convertStreamToInt(buffer);
		}
	}
	else{
		return 0;
	}
	return ack;
}


int ConnectToAgent::readFrom(char * buffer){
	int ack;
	// ack  = readInt8FromSSASocket(buffer);
	int32 lengthOfResponse = 0;
	std::cout<<ack<<std::endl;
	ack = readInt32FromSSASocket(&lengthOfResponse, buffer);
	std::cout<<ack<<"length of packet "<<lengthOfResponse<<std::endl;
	return ack;
}

int ConnectToAgent::readCompletePacket(char * buffer){
	int32 maxsize = 256 * 1024;
	free(buffer);
	buffer = (char *)malloc(sizeof(maxsize));
	int valRead = read(socketFd, buffer, maxsize);
	return valRead;
}


void ConnectToAgent::convertIntToStream(int32 size, char * stream){
	stream[0] = ((size >> 24 ) & 0xff);
	stream[1] = ((size >> 16 ) & 0xff);
	stream[2] = ((size >> 8 ) & 0xff);
	stream[3] = ((size) & 0xff);
}

int32 ConnectToAgent::convertStreamToInt(char * stream){

	int32 sizeOfPacket= (int32)(stream[0] << 24) | (int32)(stream[1] << 16) | (int32)(stream[2] << 8) | (int32)stream[3] ;
	return sizeOfPacket;

}


int ConnectToAgent::readFromSSASocket(int32 sizeOfMessage, char * buffer){

	std::cout<<"int ConnectToAgent::readFromSSASocket(int32 sizeOfMessage, char * buffer) size of message is "<<sizeOfMessage<<std::endl;
	int valRead = 1;

	// buffer = (char *)malloc(sizeOfMessage);
	buffer = (char *)malloc(MAX_BUFFER_SIZE);

	bzero(buffer, sizeOfMessage);
	// valRead = read(socketFd, buffer, sizeOfMessage);
	// valRead = recv(socketFd, buffer, sizeOfMessage, 0);
	valRead = recv(socketFd, buffer, MAX_BUFFER_SIZE, 0);


	std::cout<<"val read is "<<valRead<<std::endl;
	if(valRead >= 0 ){
		for(int i = 0; i < valRead; i ++)
		std::cout<<"data read is "<<buffer[i]<<std::endl;
	}
	else{
		std::cout<<"error in reading data "<<std::endl;
		exit(1);
	}
	return valRead;
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
	// close(clientSocketFd);
	close(socketFd);
}

