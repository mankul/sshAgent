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
	writeInt8ToAgent(SSH_AGENTC_REQUEST_IDENTITIES);	
	return true;
}

bool ConnectToAgent::removeAllAddedIdentities(){
		
	std::cout<<"connectToAgent::RemoveAllAddedIdentities "<<std::endl;
	std::cout<<unsigned(SSH_AGENTC_REMOVE_ALL_IDENTITIES)<<std::endl;
	writeInt8ToAgent(SSH_AGENTC_REQUEST_IDENTITIES);	
	return true;
}

 	
 	
 
bool ConnectToAgent::writeInt8ToAgent(int8 en){
	int dataTypeSize = sizeof(1);
		const char * message = reinterpret_cast<const char *>(&en);
	const char * messageLength = reinterpret_cast<const char *>(&dataTypeSize);
	
	int totalMessageLength = sizeof(int) + sizeof(int8);
	char * p = (char *)malloc(totalMessageLength + 1);
	strcpy(&p[0],messageLength );
	strcpy(&p[4],message);
	
	std::cout<<"ConnectToAgent::writeInt8ToAgent message is "<<unsigned (en)<< " size is "<<sizeof(en)<<"total message length "<<totalMessageLength<<"total message "<<p<<std::endl;

	// bzero(p, )

	int ack = writeContentToSSASocket(p, totalMessageLength);
	if(ack){
		std::cout<<"acknowledgement is "<<ack<<std::endl;
		readFromSSASocket();
	}

}




char * ConnectToAgent::readFromSSASocket(){

	int valRead ;
	char buffer [BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
	int sizeOfOffset = sizeof(int8);
	valRead = read(socketFd, buffer, sizeOfOffset);
	// do{
		// valRead = read(socketFd, buffer, BUFFER_SIZE );
	std::cout<<"response from ssh agent "<< valRead << " and response "<< buffer<<std::endl;
	// }while(valRead >=0);
// else
	// std::cout<<"not read from  socket"<<std::endl;
	return buffer;
}


int ConnectToAgent::writeContentToSSASocket(char * stream, int sizeOfStream){	
	int ack ;
	ack =  write(socketFd, stream, sizeOfStream);//buffer, strlen(buffer));
	if(ack >= 0)
		std::cout<<"ConnectToAgent::writeContentToSSASocket content written "<<stream<<std::endl;	
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

