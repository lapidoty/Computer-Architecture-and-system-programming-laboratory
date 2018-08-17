#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: hexeditplus

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
hexeditplus: Task1a.o
	gcc -g -Wall -o hexeditplus Task1a.o


Task1a.o:  Task1a.c
	gcc -g -Wall -c -o Task1a.o Task1a.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o hexeditplus
