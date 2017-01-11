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
    PRINT_INLINE
};

int main(int argc, char *argv[])
{
    DebugLevel debugLevel = PRINT_TIME;

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
        return 0;
    }

    int fileLines = count(istreambuf_iterator<char>(dmrf), 
             istreambuf_iterator<char>(), '\n');
    if (debugLevel >= PRINT_COMMON)
        printf("File lines count: %d\n", fileLines);

    // Open binary file.
    fileName[fileLength - 3] = 'b';
    fileName[fileLength - 2] = 'i';
    fileName[fileLength - 1] = 'n';
    
    // FILE* binf;
    // 
    // if ((binf = fopen(fileName, "wb")) != 0) {
    //     printf("Error opening the bin file.\n");
    // }

    ofstream binf (fileName, ios::out | ios::binary);
    if (debugLevel >= PRINT_COMMON)
        printf("Binary file opened.\n");

    // Prepare the structure to write in binary file.
    //char *buffer = (char*) malloc (bufferSize);
    //if (buffer==NULL) exit (1);

    //printf("byte buffer structure created.\n");

    char* currentLinePtr;
    char* heightString;

    int x = 0;
    int y = 0;
    // Buffer pointer
    //int i = 0;

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

            // Normalisation
            //heightFloat = (heightFloat / 2864.0) * 65536 + 0.5;
            //if (debugLevel >= 3)
            //    printf("Normalisation: %f\n", heightFloat);

            // Change type
            //unsigned short heightUshort = (unsigned short)heightFloat;
            //if (debugLevel >= 3)
            //    printf("ushort: %u\n\n", heightUshort);

            // Change endian
            //char *ptr = (char *)&heightUshort;
            //char t;
            //t = ptr[0];
            //ptr[0] = ptr[1];
            //ptr[1] = t;

            //char *ptr = (char *)&heightFloat;
            //char t0, t1;
            //t0 = ptr[0];
            //t1 = ptr[1];
            //ptr[0] = ptr[3];
            //ptr[1] = ptr[2];
            //ptr[2] = t1;
            //ptr[3] = t0;

            binf.write((char *)&heightFloat, sizeof(float));


            // Write height to buffer
            //buffer[i] = heightFloat;
            //i += 2;
        }
    }

    //// Actually write data to file
    //binf.write(buffer, bufferSize);

    // Cleanup.
    //free(buffer);

    dmrf.close();
    binf.close();

    if (debugLevel >= PRINT_TIME) {
        end = clock();
        double elapsedSecs = double(end - start) / CLOCKS_PER_SEC;
        printf("%f\n", elapsedSecs);
    }
    
    return 0;
}

