#ifndef SSHAGENT
#define SSHAGENT

#include<iostream>
#include<string>
#include "executeOnTerminal.h"
class SshAgent{
	public:
	SshAgent();
	~SshAgent();
	std::string getPathToSocket();
};
#endif SSHAGENT
