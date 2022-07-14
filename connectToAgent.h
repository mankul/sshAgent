#ifndef CONNECT_TO_AGENT
#define CONNECT_TO_AGENT



#include<sys/socket.h>
#include<sys/un.h>

class ConnectToAgent{


	private:
		int socketFd;
		int clientSocketFd;
		struct sockaddr_un * sock_addr;
	public:
	ConnectToAgent();
	~ConnectToAgent();

	void connectBySocket( char * sockAddr);
	void sendSSHContentToAgent(const char * stream);
	void closeConnection();
};


#endif
