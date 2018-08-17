#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *pFile;
    int c;
    pFile = stdout;
    int i = 1;
    int bool = 1;
    int verboseW = 1;
    int verboseC = 1;
    int verboseL = 1;
    int LenCounter = 0;
    int Longest = 0;
    int chars = 0;
    int j = 1;

    for (i = 1; i < argc; i++) { //checking which parameter recived
        if (strcmp(argv[i], "-w") == 0)
            verboseW = 0;
        if (strcmp(argv[i], "-c") == 0)
            verboseC = 0;
        if (strcmp(argv[i], "-l") == 0)
            verboseL = 0;
        }

    if(verboseC==1 && verboseL==1 && verboseW==1) {
        verboseW = 0;
    }

    
    while ((c = fgetc(stdin)) != EOF) { //looping untill user insert ctrl+D

            if (c == ' ' || c == '\t') { // counting words
                if (bool == 0) {
                    j++;
                    bool = 1;
                }
            } else {
                bool = 0;
            }


            if (c != ' ' && c != '\t' && c != '\n') { // counting chars
                chars++;
            }

            if (c != ' ' && c != '\t' && c != '\n') { //counting longest word
                LenCounter++;
                if (LenCounter>Longest){
                    Longest=LenCounter;
                }
            }
            else {
                LenCounter = 0;
            }


    }
    printf("^D\n");
	
	//checks what parameter recived and print accordinly
    for (i = 1; i < argc; i++) { 
        
         if (strcmp(argv[i], "-w") == 0){
           if (verboseW==0) {
                printf("%d\n", j);
            }
         }
         
        if (strcmp(argv[i], "-c") == 0)
            if (verboseC==0) {
                printf("%d\n", chars);
            }
            
        if (strcmp(argv[i], "-l") == 0)
            if (verboseL==0) {
                printf("%d\n", Longest);
        }
  
    }

    fclose(pFile);
    return 0;
}
