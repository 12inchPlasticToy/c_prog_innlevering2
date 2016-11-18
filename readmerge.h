#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE **loadDirectory(char *pDir);

void printLines(FILE **files);

void closeFiles(FILE **files);

void parseGraphSize(char *lastFileName);