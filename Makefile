CC = g++
CFLAGS = -Wall -g


target = connectAgent

all : $(target)

$(target) : connectToAgentMain.o connectToAgent.o executeOnTerminal.o
	$(CC) $(CFLAGS) --std c++11  -o $(target)  connectToAgentMain.o connectToAgent.o executeOnTerminal.o

connectToAgentMain.o: connectToAgentMain.cpp connectToAgent.h executeOnTerminal.h
	$(CC) $(CFLAGS) -c connectToAgentMain.cpp

connectToAgent.o :  connectToAgent.cpp connectToAgent.h

executeOnTerminal.o :  executeOnTerminal.cpp executeOnTerminal.h
	$(CC) $(CFLAGS) -c executeOnTerminal.cpp
	

