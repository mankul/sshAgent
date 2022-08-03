
#define SERVER_SOCKET "server.sock"

#include "connectToAgent.h"
#include "executeOnTerminal.h"
int main(){


	ConnectToAgent connectionToAgent;
	char * sshAgentPath = "echo $SSH_AUTH_SOCK";
	bool localServer = false;
	// bool localServer = true;
	bool r = 1;
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
		// connectionToAgent.getAddedIdentities();
		connectionToAgent.removeAllAddedIdentities();
		// connectionToAgent.removeAllRSAAddedIdentities();
		
	}
	return 0;
}
