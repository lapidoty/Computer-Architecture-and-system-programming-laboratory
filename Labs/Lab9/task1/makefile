#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: client server

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
client: common.o LineParser.o client.o
	gcc -g -Wall -o client common.o LineParser.o client.o

client.o:  client.c LineParser.h common.h
	gcc -g -Wall -c -o client.o client.c

server: common.o LineParser.o server.o
	gcc -g -Wall -o server common.o LineParser.o server.o

server.o:  server.c LineParser.h common.h
	gcc -g -Wall -c -o server.o server.c

# Depends on the source and header files
LineParser.o:  LineParser.c LineParser.h
	gcc -g -Wall -c -o LineParser.o LineParser.c

common.o:  common.c common.h
	gcc -g -Wall -c -o common.o common.c



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o client server
