CC = g++
CFLAGS = -Wall -g

testAgentConnection: connectToAgentMain.o connectToAgent.o executeOnTerminal.o
	$(CC) $(CFLAGS) connectToAgentMain.o connectToAgent.o  executeOnTerminal.o

all: mainFile.o sshAgent.o executeOnTerminal.o 
	$(CC) $(CFLAGS)  mainFile.o  sshAgent.o connectToAgent.o executeOnTerminal.o

mainFile.o : mainFile.cpp
	$(CC) mainFile.cpp

connectToAgent.o : connectToAgent.cpp connectToAgent.h
	$(CC) $(CFLAGS) connectToAgent.cpp
sshAgent.o : sshAgent.cpp sshAgent.h
	$(CC) sshAgent.cpp
