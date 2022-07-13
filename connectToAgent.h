#ifndef CONNECT_TO_AGENT
#define CONNECT_TO_AGENT



#include<sys/socket.h>
#include<sys/un.h>

class ConnectToAgent{

	public:
	ConnectToAgent();
	~ConnectToAgent();

	void connectBySocket( char * sockAddr);


};


#endif
