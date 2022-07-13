all: mainFile.o sshAgent.o executeOnTerminal.o 
	g++ mainFile.o  sshAgent.o executeOnTerminal.o

mainFile.o : g++ mainFile.cpp
