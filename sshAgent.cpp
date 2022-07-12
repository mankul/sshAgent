#include"sshAgent.h"


SshAgent::SshAgent(){
	std::cout<<"ssh agent initialized"<<std::endl;
}

SshAgent::~SshAgent(){
	std::cout<<"ssh agent is destroyed"<<std::endl;
}


std::string SshAgent::getPathToSocket(){
	std::cout<<" Get LocalSocket for unix"<<std::endl;
	const char * SSH_AUTH_SOCK = "echo $SSH_AUTH_SOCK";
	execute_command(SSH_AUTH_SOCK);
	return "";
}

