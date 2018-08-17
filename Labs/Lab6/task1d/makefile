#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: myshell

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
myshell: job_control.o LineParser.o myshell.o
	gcc -g -Wall -o myshell job_control.o LineParser.o myshell.o


myshell.o:  myshell.c LineParser.h job_control.h
	gcc -g -Wall -c -o myshell.o myshell.c

# Depends on the source and header files
LineParser.o:  LineParser.c LineParser.h
	gcc -g -Wall -c -o LineParser.o LineParser.c

job_control.o:  job_control.c job_control.h
	gcc -g -Wall -c -o job_control.o job_control.c



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o myshell
