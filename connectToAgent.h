#ifndef CONNECT_TO_AGENT
#define CONNECT_TO_AGENT



#include<sys/socket.h>
#include<sys/un.h>
#include<cstdint>

typedef const unsigned char int8;


class ConnectToAgent{


	private:
		int socketFd;
		int clientSocketFd;

		int8 SSH_AGENT_FAILURE = 5;
		int8 SSH_AGENT_SUCCESS = 6;
		int8 SSH_AGENTC_REMOVE_ALL_RSA_IDENTITIES = 9;
		int8 SSH_AGENTC_REQUEST_IDENTITIES = 11;
		int8 SSH_AGENT_IDENTITIES_ANSWER = 12;
		int8 SSH_AGENTC_ADD_IDENTITY = 17;
		int8 SSH_AGENTC_REMOVE_IDENTITY = 18;
		int8 SSH_AGENTC_ADD_ID_CONSTRAINED = 25;
		int8 SSH_AGENTC_REMOVE_ALL_IDENTITIES = 19;

		int8 SSH_AGENT_CONSTRAIN_LIFETIME = 1;
		int8 SSH_AGENT_CONSTRAIN_CONFIRM = 2;
		int8 SSH_AGENT_CONSTRAIN_EXTENSION = 255;	
	
	
	
	public:
		ConnectToAgent();
		~ConnectToAgent();

		bool writeInt8ToAgent(int8 en);
		bool getAddedIdentities();
		bool removeAllAddedIdentities();
		bool removeAllRSAAddedIdentities();
		bool connectSocket( const char * sockAddr);
		int writeContentToSSASocket(char * stream, int sizeOfStream);
		char * readFromSSASocket();
		void closeConnection();
};


#endif
