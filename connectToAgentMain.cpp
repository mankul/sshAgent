
#define SERVER_SOCKET "server.sock"

#include "connectToAgent.h"
#include "executeOnTerminal.h"
int main(){


	ConnectToAgent connectionToAgent;
	const char * sshAgentPath = "echo $SSH_AUTH_SOCK";
	bool localServer = false;
	// bool localServer = true;
	size_t r = 1000;
	std::cout<<"string length "<<sizeof(int);//((const char *)(&r))<<std::endl;
	if(localServer)
	{
		const char * pathToServer = SERVER_SOCKET;	
		r = connectionToAgent.connectSocket(pathToServer);
	
	}
	else{
		std::string pathToSSHAgent = execute_command(sshAgentPath);
		std::cout<<pathToSSHAgent<<std::endl;
		const char * pathToServer = pathToSSHAgent.c_str();
		r = connectionToAgent.connectSocket(pathToServer);
	}
	if(r){
		const char * streamKey = "private key for ssh ";
		connectionToAgent.getAddedIdentities();
		// connectionToAgent.removeAllAddedIdentities();
		// connectionToAgent.removeAllRSAAddedIdentities();
		
	}
	return 0;
}
