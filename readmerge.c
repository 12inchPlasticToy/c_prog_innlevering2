#include <dirent.h>
#include <ctype.h>

#include "readmerge.h"

int fileCount, rows, columns;
char *inputDirName;

const short MAX_FILE_COUNT = 20; //an arbitrary limit of 20 quadrants;
const short LINES_PER_FILE = 30;
const short MAX_QUADRANT_SIZE = 30 * 30;

void parseGraphSize(char *lastFileName);
void insertByPriority(char **filenames, char *filename);
void swap(char **str1, char **str2);
char *getFilepath(char *dirname, char *filename);

FILE **loadDirectory(char *container) {

    inputDirName = container;

    DIR *pDirectory = opendir(container);

    if (pDirectory == NULL) {
        printf("Could not open the given folder. Program exits now...\n");
        return NULL;
    }

    fileCount = 0;
    char **filenames;
    struct dirent *dirEntry;

    filenames = (char **) malloc(MAX_FILE_COUNT * sizeof(char *));

    while ((dirEntry = readdir(pDirectory)) && fileCount < MAX_FILE_COUNT) {

        char *filename = dirEntry->d_name;
        // filter out directory entries and hidden files
        // - we assume all other files are valid picture quadrants with conforming names
        if (isalnum(filename[0])) {
            insertByPriority(filenames, filename);
            fileCount++;
        }
    }
    closedir(pDirectory);

    // grab the last ordered file to get the size of the picture in quadrants
    // the values are stored in the local "rows" and "columns" variables
    parseGraphSize(filenames[fileCount - 1]);

    FILE **files;
    files = (FILE **) malloc(fileCount * sizeof(FILE *));
    for (int i = 0; i < fileCount; i++) {
        char *filePath = getFilepath(container, filenames[i]);
        if ((files[i] = (FILE *) malloc(MAX_QUADRANT_SIZE * sizeof(char))) == NULL)
            printf("Could not allocate memory for index files[%d]\n",i);
        if ((files[i] = fopen(filePath, "r")) == NULL)
            printf("Could not open:\n%s\n", filePath);
        //free(filenames[i]);
    }

    free(filenames);

    return files;
}


/**
 * Clean up files[]
 * @param files
 */
void closeFiles(FILE **files){
    for(int i = 0; i < fileCount; i++){
        fclose(files[i]);
        files[i] = NULL;
    }
}


/**
 * Extract the x and y values from a file name.
 * @param lastFileName
 */
void parseGraphSize(char *lastFileName) {

    /*
    char *parsed = malloc(strlen(lastFileName) * sizeof(char)); //freed
    parsed = strncpy(parsed, lastFileName, strlen(lastFileName));
*/
    sscanf(lastFileName, "part_%d_%d", &columns, &rows);

    printf("Columns: %d\n", ++columns);
    printf("Rows: %d\n", ++rows);
    //free(parsed);
}

/**
 * inserts and sort filenames, ordered by their (x,y) coordinates.
 * all files must follow the same naming pattern.
 * @param filenames
 * @param filename
 */
void insertByPriority(char *filenames[], char *filename) {
    int count = fileCount;
    if (count == 0) {
        filenames[0] = filename;
    } else {
        filenames[count] = filename;
        while ((count > 0) && (strcmp(filenames[count], filenames[count - 1]) < 0)) {
            swap(filenames + count, filenames + (count - 1));
            count--;
        }
    }
}

void swap(char **str1, char **str2) {
    char *tmp = *str1;
    *str1 = *str2;
    *str2 = tmp;
}


void printLines(FILE **files) {

    // create a 2D string buffer to store all lines
    // as if they were pixels with (x,y) coordinates
    char *lines[columns][rows * LINES_PER_FILE ];

    for (int i = 0; i < columns; i++) {

        for (int j = 0; j < rows; j++) {


            int fileIndex = i * rows + j;
            printf("file index : %d\n", fileIndex);

            for (int k = j * LINES_PER_FILE; k < (j + 1) * LINES_PER_FILE; k++) {
                lines[i][k] = (char *) calloc(30, sizeof(char));

                if ( lines[i][k] == NULL){
                    printf("Could not allocate memory for lines[%d][%d]\n", i, k);
                }
                fread(lines[i][k], sizeof(char), 30, files[fileIndex]);
            }
        }
    }

    char *outputName = malloc( strlen( inputDirName) + 5); //freed
    snprintf(outputName, strlen(inputDirName)+5,"%s.txt",inputDirName);

    FILE *outputFile = fopen(outputName, "w");

    printf("out is open\n");
    for (int i = 0; i < rows * LINES_PER_FILE; i++) {


        char *str = calloc((size_t) 31 * columns, sizeof(char)); //freed
        for (int j = 0; j < columns; j++) {
            strncat(str, lines[j][i], strlen(lines[j][i]));
        }

        char *newLine = strrchr(str, '\n');
        if( newLine){
            *newLine = '\0';
            strcat(str, "\n");
        }

        fputs(str, outputFile);
        free(str);
    }
    free(outputName);
    fclose(outputFile);
}


char *getFilepath(char *dirname, char *filename) {

    char *filepath = (char *) calloc(strlen(dirname) + 1 + strlen(filename), sizeof(char));
    strncat(filepath, dirname, strlen(dirname));
    strncat(filepath, "/", 1);
    strncat(filepath, filename, strlen(filename));

    return filepath;
}