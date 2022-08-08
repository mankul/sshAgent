#ifndef CONNECT_TO_AGENT
#define CONNECT_TO_AGENT



#include<sys/socket.h>
#include<sys/un.h>
#include<cstdint>
#include"sshbuffer.h"



typedef const unsigned char int8;
typedef unsigned int int32;

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

		bool getAddedIdentities();
		bool removeAllAddedIdentities();
		bool removeAllRSAAddedIdentities();
		bool connectSocket( const char * sockAddr);
		
		int writeInt32ToAgent(int en);
		int writeInt8ToAgent(int8 en);
		int writeContentToSSASocket(char * stream, int sizeOfStream);

		bool prepareMessageAndSend(int32 dataLength, char * message);
		bool reserveBufferAndSend(int32 messageLength, char * message);

		int readFrom(char * buffer);
		int readFromSSASocket(int32 sizeOfMessage, char * buffer);
		int readInt32FromSSASocket(int32 * lengthOfResponse, char * buffer);
		int readInt8FromSSASocket(char * buffer);
		int readPacketFromSSASocket(int32 packetLength, char * buffer);
		int readCompletePacket(char * buffer);


		void convertIntToStream(int32 size, char * stream);
		int32 convertStreamToInt(char * stream);

		void closeConnection();
};


#endif
