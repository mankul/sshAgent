#ifndef CONNECT_TO_AGENT
#define CONNECT_TO_AGENT



#include<sys/socket.h>
#include<sys/un.h>
#include<cstdint>


class ConnectToAgent{


	private:
		int socketFd;
		int clientSocketFd;
		struct sockaddr_un * sock_addr;

		const uint8_t SSH_AGENT_FAILURE = 5;
		const uint8_t SSH_AGENT_SUCCESS = 6;
		const uint8_t SSH_AGENTC_REQUEST_IDENTITIES = 128;//11;
		const uint8_t SSH_AGENT_IDENTITIES_ANSWER = 12;
		const uint8_t SSH_AGENTC_ADD_IDENTITY = 17;
		const uint8_t SSH_AGENTC_REMOVE_IDENTITY = 18;
		const uint8_t SSH_AGENTC_ADD_ID_CONSTRAINED = 25;

		const uint8_t SSH_AGENT_CONSTRAIN_LIFETIME = 1;
		const uint8_t SSH_AGENT_CONSTRAIN_CONFIRM = 2;
		const uint8_t SSH_AGENT_CONSTRAIN_EXTENSION = 255;	
	
	
	
	public:
		ConnectToAgent();
		~ConnectToAgent();

		bool writeInt8ToAgent(uint8_t en);
		bool getAddedIdentities();
		void connectSocket( char * sockAddr);
		void writeContentToSSASocket(char * stream);
		void closeConnection();
};


#endif
