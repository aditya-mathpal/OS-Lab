/*
Write a program to copy character-by character copy is accomplished using calls to the
functions referenced in stdio.h
*/

#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[]) {
    FILE *sourceFile, *destFile;
    int ch;
    sourceFile = fopen(argv[1], "r");
    if(sourceFile == NULL) {
        perror("error opening source file");
        exit(EXIT_FAILURE);
    }
    destFile = fopen(argv[2], "w");
    if(destFile == NULL) {
        perror("Error opening destination file");
        fclose(sourceFile);
        exit(EXIT_FAILURE);
    }
    while((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destFile);
    }
    printf("File copied successfuly.\n");
    fclose(sourceFile);
    fclose(destFile);
    return 0;
}

/*
output:

initially:
sample_text.txt:
one
two
three

dest.txt:


$ ./5.out sample_text.txt dest.txt

dest.txt:
one
two
three
*/