

#include "connectToAgent.h"
#include "executeOnTerminal.h"
int main(){


	ConnectToAgent connectionToAgent;
	char * sshAgentPath = "echo $SSH_AUTH_SOCK";
	std::string pathToSSHAgent = execute_command(sshAgentPath);
	std::cout<<pathToSSHAgent<<std::endl;
	char * path = new char[pathToSSHAgent.length()];
	strcpy(path, pathToSSHAgent.c_str());
	connectionToAgent.connectSocket( path );
	const char * streamKey = "private key for ssh ";
	connectionToAgent.getAddedIdentities();
	return 0;
}
