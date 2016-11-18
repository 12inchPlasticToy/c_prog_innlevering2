#include <stdio.h>
#include <stdlib.h>

#include "readmerge.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("You must give a folder as argument. \n"
                       "Example: ./readmerge myFolder\n"
                       "Program exits now...\n");
        return 0;
    }


    FILE **files;
    if ( (files = loadDirectory(argv[1]) ) == NULL){
        printf("No files could be loaded. Program exits now...\n");
        return EXIT_FAILURE;
    }


    printLines(files);

    closeFiles(files);

    return 0;
}

/**
 * Test method - use on a single file to
 * print it as it should appear in the end
 * @param file
 */
void formatCheck(FILE *file){
    FILE *out = fopen("testQuadrant.txt", "w");
    int c;
    int isPrintable = 1;
    while( (c = fgetc(file)) != EOF){

        if( c == '\0'){
            if(isPrintable)
            {
                fputc('\n', out);
                isPrintable = 0;
            }
        } else{
            fputc(c, out);
            isPrintable = 1;
        }
    }
    rewind(file);
    fclose(out);
}


