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

#define BITS_PER_VERTEX 19
#define BITS_TO_SHIFT 13 // 32 - 19
#define AREA_SIZE 1001

using namespace std;

enum DebugLevel {
    PRINT_NOTHING,
    PRINT_TIME,
    PRINT_COMMON,
    PRINT_INLINE,
    PRINT_LINES_COUNT
};

int main(int argc, char *argv[])
{
    DebugLevel debugLevel = (DebugLevel)atoi(argv[2]);

    clock_t start, end;
    if (debugLevel >= PRINT_TIME)
        start = clock();

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

    unsigned int buffer = 0;
    char bufferOffset = 0;

    // Reading from .asc file, binary encoding and saving data in binary file.
    for (x = 0; x < AREA_SIZE; x++) {
        for (y = 0; y < AREA_SIZE; y++) {
            // Read from file
            char inputLine[28];
            dmrf >> inputLine;
            if (debugLevel >= PRINT_INLINE)
                printf("line: %s\n", inputLine);
            currentLinePtr = inputLine;

            // Ignore coordinates
            heightString = currentLinePtr + 20;
            // Ignore newline symbol
            heightString[strlen(heightString)] = '\0';
            if (debugLevel >= PRINT_INLINE)
                printf("Raw height: %s\n", heightString);
            
            float heightFloat = atof(heightString);
            if (debugLevel >= PRINT_INLINE)
                printf("Float height: %f\n", heightFloat);

            // Change to int
            heightFloat *= 100;
            unsigned int heightInt = (unsigned int)heightFloat;
            if (debugLevel >= PRINT_INLINE)
                printf("Int height: %u\n", heightInt);

            // Write value to buffer
            unsigned int heightTmp = (heightInt << BITS_TO_SHIFT) >> bufferOffset;
            if (debugLevel >= PRINT_INLINE)
                printf("Buffer: %u and heightTmp: %u\n", buffer, heightTmp);

            buffer = buffer | heightTmp;
            if (debugLevel >= PRINT_INLINE)
                printf("Buffer: %u and bufferOffset: %d\n\n", buffer, bufferOffset);

            // If buffer is full, write number to file
            if ((bufferOffset + BITS_PER_VERTEX) >= 32) {
                binf.write((char *)&buffer, sizeof(int));

                // Prepare buffer for next value
                bufferOffset = (bufferOffset + BITS_PER_VERTEX) - 32;
                buffer = heightInt << (32 - bufferOffset);
            } else {
                // Prepare buffer for next value
                bufferOffset += BITS_PER_VERTEX;
            }
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

