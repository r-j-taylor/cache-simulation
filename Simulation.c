/*
 *  File: Simulation.c ... a skeleton file
 *  Provided by: Professor Tarek Idriss
 *  Last update : 07 December 2019 by Ryan Taylor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pageInfo {
    int tracker;
    char* pageNumber;
};

struct pageInfo createPageInfo() {
    struct pageInfo *pageInfo = (struct pageInfo *)malloc(sizeof(struct pageInfo));
    pageInfo->tracker = 0;
    pageInfo->pageNumber = (char *)malloc(sizeof(char) * 6);
    strncpy(pageInfo->pageNumber, "-1\0", 3);
    return *pageInfo;
}

// Function simulating an LRU algorithm.
void LRU(char* inputFile, int framesNumber, int period) {

    // Open file.
    FILE *filePointer = fopen(inputFile, "r");
    if (filePointer == NULL) {
        printf("\nERROR: Could not open file of name %s!\n\n", inputFile);
        exit(-1);
    }
    
    // Set up variables.
    char *line;
    size_t n = 0;
    int tick = 0, loops = 0, faults = 0;
    struct pageInfo pageTable[framesNumber];

    // Set up page table.
    for (int j = 0; j < framesNumber; j++) {
        pageTable[j] = createPageInfo();
    }

    // Loop through input file.
    while (getline(&line, &n, filePointer) != -1) {
        
        // Set up more variables.
        int i, j, min = -1;
        char pageNumber[6];
        strncpy(pageNumber, line, 5);
        pageNumber[5] = '\0';

        // Handles periods.
        if (tick == period) {
            tick = 0;
            for (i = 0; i < framesNumber; i++) {
                pageTable[i].tracker = 0;
            }
        }

        // Loop through page table.
        for (i = 0; i <= framesNumber; i++) {
            if (i == framesNumber) { // Page wasn't in table.
                faults++;
                strncpy(pageTable[j].pageNumber, pageNumber, 6);
                pageTable[j].tracker = tick;
                break;
            } else if (strcmp(pageTable[i].pageNumber, pageNumber) == 0) { // Page was in table.
                pageTable[i].tracker = tick;
                break;
            } else if (strcmp(pageTable[i].pageNumber, "-1\0") == 0) { // Table is not full.
                faults++;
                strncpy(pageTable[i].pageNumber, pageNumber, 6);
                pageTable[i].tracker = tick;
                break;
            } else if (min == -1 || pageTable[i].tracker < min) { // Keep track of least recently used page.
                j = i;
                min = pageTable[i].tracker;
            }
        }
        
        tick++;
        loops++;
    }
	printf("\tThere were a total of %d faults in %d total memory accesses.\n", faults, loops);
}

// Function simulating a second chance algorithm.
void secondChance(char *inputFile, int framesNumber) {
    
    // Open file.
    FILE *filePointer = fopen(inputFile, "r");
    if (filePointer == NULL) {
        printf("\nERROR: Could not open file of name %s!\n\n", inputFile);
        exit(-1);
    }
    
    // Set up variables.
    char *line;
    size_t n = 0;
    int loops = 0, faults = 0;
    struct pageInfo pageTable[framesNumber];

    // Set up page table.
    for (int j = 0; j < framesNumber; j++) {
        pageTable[j] = createPageInfo();
    }

    // Loop through input file.
    while (getline(&line, &n, filePointer) != -1) {
        
        // Set up more variables.
        int i, j = framesNumber, min = 1;
        char pageNumber[6];
        strncpy(pageNumber, line, 5);
        pageNumber[5] = '\0';

        // Loop through page table.
        for (i = 0; i <= framesNumber; i++) {
            if (i == framesNumber) { // Page wasn't in table.
                faults++;
                
                for (int t = 0; t < j; t++) {
                    pageTable[t].tracker = 0;
                }
                if (j == framesNumber) j = 0;

                strncpy(pageTable[j].pageNumber, pageNumber, 6);
                pageTable[j].tracker = 1;
                break;
            } else if (strcmp(pageTable[i].pageNumber, pageNumber) == 0) { // Page was in table.
                pageTable[i].tracker = 1;
                break;
            } else if (strcmp(pageTable[i].pageNumber, "-1\0") == 0) { // Table is not full.
                faults++;
                strncpy(pageTable[i].pageNumber, pageNumber, 6);
                pageTable[i].tracker = 1;
                break;
            } else if (pageTable[i].tracker < min) { // Keep track of first 0 in chance bit.
                j = i;
                min = pageTable[i].tracker;
            }
        }
        
        loops++;
    }
	printf("\tThere were a total of %d faults in %d total memory accesses.\n", faults, loops);
}

// Helper to find the distance to the next occurrence of a given page.
int timeToNextOccurrence(char pageNumber[6], FILE *filePointer) {

    // Set up variables.
    int i = 0;
    char *line;
    size_t n = 0;
    long offset = ftell(filePointer);

    // Loop through 500 lines of the input file, at most.
    while (getline(&line, &n, filePointer) != -1) {
        if (strstr(line, pageNumber)) break;
        if (++i == 500) {
            i = 0;
            break;
        }
    }

    // Return to original position in file.
    if (fseek(filePointer, offset, SEEK_SET)) {
        printf("\nERROR: Could not seek to correct position in file!\n\n");
        exit(-1);
    }

    return i;
}

// Function simulating an optimal algorithm.
void optimal(char *inputFile, int framesNumber) {
    
    // Open file.
    FILE *filePointer = fopen(inputFile, "r");
    if (filePointer == NULL) {
        printf("\nERROR: Could not open file of name %s!\n\n", inputFile);
        exit(-1);
    }
    
    // Set up variables.
    char *line;
    size_t n = 0;
    int loops = 0, faults = 0;
    struct pageInfo pageTable[framesNumber];

    // Set up page table.
    for (int j = 0; j < framesNumber; j++) {
        pageTable[j] = createPageInfo();
    }

    // Loop through input file.
    while (getline(&line, &n, filePointer) != -1) {
        
        // Set up more variables.
        int i, j, max = -1;
        char pageNumber[6];
        strncpy(pageNumber, line, 5);
        pageNumber[5] = '\0';

        // Loop through page table.
        int check = 1;
        for (i = 0; i <= framesNumber; i++) {
            if (check && i == framesNumber) { // Page wasn't in table.
                faults++;
                check = 0;

                strncpy(pageTable[j].pageNumber, pageNumber, 6);
                pageTable[j].tracker = timeToNextOccurrence(pageNumber, filePointer);
            } else if (check && strcmp(pageTable[i].pageNumber, pageNumber) == 0) { // Page was in table.
                check = 0;
            } else if (check && strcmp(pageTable[i].pageNumber, "-1\0") == 0) { // Table is not full.
                faults++;
                check = 0;

                strncpy(pageTable[i].pageNumber, pageNumber, 6);
                pageTable[i].tracker = timeToNextOccurrence(pageNumber, filePointer);
            } else if (check && max != 0 && (max == -1 || pageTable[i].tracker > max || pageTable[i].tracker == 0)) { // Keep track of longest distance to next occurrence.
                max = pageTable[i].tracker;
                j = i;
            }
            if (pageTable[i].tracker) pageTable[i].tracker--;
        }
        
        loops++;
    }
	printf("\tThere were a total of %d faults in %d total memory accesses.\n", faults, loops);
}

void Simulate(char *inputFile, int framesNumber, int period) {

    if (framesNumber > 0) {
        printf("\nLRU - Timestamp\n-------------------------------------\n");
        LRU(inputFile, framesNumber, -1);
        printf("-------------------------------------\n");

        if (period > 0) {
            printf("\nLRU - Counter\n-------------------------------------\n");
            LRU(inputFile, framesNumber, period);
            printf("-------------------------------------\n");
        }

        printf("\nSecond Chance\n-------------------------------------\n");
        secondChance(inputFile, framesNumber);
        printf("-------------------------------------\n");

        printf("\nOptimal\n-------------------------------------\n");
        optimal(inputFile, framesNumber);
        printf("-------------------------------------\n");
    } else {
        printf("\nLRU - Timestamp\n-------------------------------------\n");
        for (int i = 8; i <= 64; i = i * 2) {
            printf("\nWith %d frames:\n", i);
            LRU(inputFile, i, -1);
        }
        printf("-------------------------------------\n");

        printf("\nLRU - Counter\n-------------------------------------\n");
        for (int i = 8; i <= 64; i = i * 2) {
            printf("\nWith %d frames:\n", i);
            for (int j = 25; j <= 1600; j = j * 2) {
                printf("\tWith a period of %d:\n\t", j);
                LRU(inputFile, i, j);
            }
        }
        printf("-------------------------------------\n");

        printf("\nSecond Chance\n-------------------------------------\n");
        for (int i = 8; i <= 64; i = i * 2) {
            printf("\nWith %d frames:\n", i);
            secondChance(inputFile, i);
        }
        printf("-------------------------------------\n");

        printf("\nOptimal\n-------------------------------------\n");
        for (int i = 8; i <= 64; i = i * 2) {
            printf("\nWith %d frames:\n", i);
            optimal(inputFile, i);
        }
        printf("-------------------------------------\n");
    }
}

int main(int argumentCount, char* argumentArray[]) {

	if (argumentCount != 4) {
        printf("\nExpected Usage: ./Simulation inputfile framesNumber period\n\n");
        exit(-1);
    }

    char* inputFile  = argumentArray[1];
    int framesNumber = atoi(argumentArray[2]);
    int period = atoi(argumentArray[3]);

    Simulate(inputFile, framesNumber, period);

    exit(0);
}
