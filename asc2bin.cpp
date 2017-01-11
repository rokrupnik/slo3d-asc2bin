/**
 * The base for this algorithm was written by a student in 
 * LGM laboratory at FRI, UL, Ljubljana, Slovenia.
 * Unfortunately, I don't know her/his name. :(
 **/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>

using namespace std;

enum DebugLevel {
    PRINT_NOTHING,
    PRINT_TIME,
    PRINT_COMMON,
    PRINT_LINES_COUNT,
    PRINT_INLINE
};

int main(int argc, char *argv[])
{
    DebugLevel debugLevel = PRINT_COMMON;

    clock_t start, end;
    if (debugLevel >= PRINT_TIME)
        start = clock();

    int sizeX = 1001;
    int sizeY = 1001;
    int bufferSize = sizeX * sizeY * sizeof(float);
    if (debugLevel >= PRINT_COMMON)
        printf("bufferSize: %d\n\n", bufferSize);

    char *fileName = argv[1];
    int fileLength = strlen(fileName);
    
    ifstream dmrf;
    dmrf.open(fileName);
    if (!dmrf.is_open()) {
        printf("Error opening the dmr file.\n");
        return 1;
    }

    if (debugLevel >= PRINT_LINES_COUNT) {
        int fileLines = count(istreambuf_iterator<char>(dmrf), 
            istreambuf_iterator<char>(), '\n');
        printf("File lines count: %d\n", fileLines);
    }

    // Open binary file.
    fileName[fileLength - 3] = 'b';
    fileName[fileLength - 2] = 'i';
    fileName[fileLength - 1] = 'n';

    ofstream binf (fileName, ios::out | ios::binary);
    if (!binf.is_open()) {
        printf("Error opening the binary file.\n");
        return 1;
    }

    char* currentLinePtr;
    char* heightString;

    int x = 0;
    int y = 0;

    // Reading from .asc file, binary encoding and saving data in binary file.
    for (x = 0; x < sizeX; x++) {

        for (y = 0; y < sizeY; y++) {
            // Read from file
            char inputLine[28];
            dmrf >> inputLine;
            if (debugLevel >= 3)
                printf("line: %s\n", inputLine);
            currentLinePtr = inputLine;

            // Ignore coordinates
            heightString = currentLinePtr + 20;
            // Ignore newline symbol
            heightString[strlen(heightString)] = '\0';
            if (debugLevel >= 3)
                printf("Raw height: %s\n", heightString);
            
            float heightFloat = atof(heightString);
            if (debugLevel >= 3)
                printf("Float height: %f\n", heightFloat);

            // Change to int
            heightFloat *= 100;
            int heightInt = 12345;

            // Change endian
            char *ptr = (char *)&heightInt;
            char t0, t1;
            t0 = ptr[0];
            t1 = ptr[1];
            ptr[0] = ptr[3];
            ptr[1] = ptr[2];
            ptr[2] = t1;
            ptr[3] = t0;

            binf.write((char *)&heightInt, sizeof(int));
        }
    }

    dmrf.close();
    binf.close();

    if (debugLevel >= PRINT_TIME) {
        end = clock();
        double elapsedSecs = double(end - start) / CLOCKS_PER_SEC;
        printf("%f\n", elapsedSecs);
    }
    
    return 0;
}

