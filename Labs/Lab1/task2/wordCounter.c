#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *iFile;
    FILE *oFile;
    int c;
    int i;
    int bool = 1;
    int verboseW = 1;
    int verboseC = 1;
    int verboseL = 1;
    int verboseN = 1;
    int verboseFILE = 1;
    int LenCounter = 0;
    int Longest = 0;
    int chars = 0;
    int Lines = 1;
    int iIndex=0;
    for (i = 1; i < argc; i++) { //checking which parameter recived
        if (strcmp(argv[i], "-w") == 0)
            verboseW = 0;
        if (strcmp(argv[i], "-c") == 0)
            verboseC = 0;
        if (strcmp(argv[i], "-l") == 0)
            verboseL = 0;
        if (strcmp(argv[i], "-n") == 0)
            verboseN = 0;
        if (strcmp(argv[i], "-i") == 0)
            verboseFILE = 0;
            iIndex = i;

    }

    if(verboseC==1 && verboseL==1 && verboseW==1 && verboseN==1 && verboseFILE==1) {
        verboseW = 0;
    }

    if (verboseFILE==0) { //change the file type

        oFile = fopen(argv[iIndex], "r");
        if (oFile==NULL) perror ("Error opening file");
        iFile = oFile;
    }
    else {

        oFile = stdout;
        iFile = stdin;
    }


    int j = 0;
    while ((c = fgetc(iFile)) != EOF) { //looping untill EOF

        if (c == ' ' || c == '\t' || c == '\n') { // counting words
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

        if (c== '\n'){ //counting lines
            Lines++;
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
        
        if (strcmp(argv[i], "-n") == 0)
            if (verboseN==0) {
                printf("%d\n", Lines);
    }
  
    }

    

    fclose(oFile);
    return 0;
}