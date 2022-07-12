#include<iostream>
#include<string>
#include"executeOnTerminal.h"
using namespace std;



void execute_command(const char * command){

	char buffer[128];
	std::string result = "";
	FILE * pipe = popen(command, "r");

	if(!pipe) throw std::runtime_error("popen failed");
	try{
		while( fgets(buffer, sizeof(buffer), pipe) != NULL){
			result += buffer;
		}
	}
	catch(...){
		std::cout<<"exception happened";
		pclose(pipe);
	}

	std::cout<<result;
	pclose(pipe);
}

/*

int main(){
	const char * command = "echo $SSH_AUTH_SOCK";
	execute_command(command);
	return 0;
}*/
