#ifndef SERVERSOCKET
#define SERVERSOCKET

#include<sys/socket.h>
#include<sys/un.h>

class ServerSocket{

	private:
		int serverSocketFD;

	public:
		ServerSocket();
		~ServerSocket();

		bool initializeServer();

};

#endif
