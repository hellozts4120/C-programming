/*
 * CacheLab - PartA
 * Created By 14307110190 Tianshu Zhou
 * ICS(1) of Mr.Tang, Fudan University, 2016 Fall
 */

#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

void printUsage() {
    printf( "Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n" );
}

// line struct
typedef struct {
    int valid;
    long long tag;
    char* block;

    // how many cycles it's used
    int age;
} Line;

// set struct
typedef struct {
    Line* lines;
} Set;

// cache struct
typedef struct {
    int E;
    long long B, S;
    int b, s;
    int tagSize;
    Set* sets;

    // Output
    int hits;
    int misses;
    int evicts;  
} Cache;

Line lineInit(int B) {
    Line line;
    line.block = (char*)malloc(B * sizeof(char));
    line.valid = 0;

    return line;
}

void lineFree(Line line, int B) {
    char* block = line.block;
    free(block);

    return;
}

void increaseLineAge(Set* set, int E) {
    int i;
    for (i = 0; i < E; i++) {
        Line* line = &(set->lines[i]);
        line->age++;
    }
}

Set setInit(int E, int B) {
    Set set;
    set.lines = (Line*)malloc(E * sizeof(Line));

    int i;
    Line* lines = set.lines;
    for (i = 0; i < E; i++) {
        lines[i] = lineInit(B);
    }

    return set;
}

void setFree(Set set, int E, int B) {
    Line* lines = set.lines;

    int i;
    for (i = 0; i < E; i++) {
        lineFree(lines[i], B);
    }
    free(lines);

    return;
}

Line* setSearch(Set* set, long long tag, int E) {
    int i;
    Line* line = NULL; 
    for (i = 0; i < E; i++) {
        line = &(set->lines[i]);
        if(line->tag == tag && line->valid) {
            return line;
        }
    }

    return NULL;
}

Cache* cacheInit(int s, int E, int b) {
    Cache* cache;
    cache = (Cache*)malloc(sizeof(Cache));
    cache->E = E;
    cache->b = b;
    cache->s = s;

    cache->hits = 0;
    cache->misses = 0;
    cache->evicts = 0;

    cache->S = 2 << s;
    cache->B = 2 << b;
    cache->tagSize = (sizeof(long long)*8) - (s + b);

    int i;
    int S = cache->S;
    int B = cache->B;

    cache->sets = (Set*)malloc(S * sizeof(Set));
    Set* sets = cache->sets;

    for(i = 0; i < S; i++) {
        sets[i] = setInit(E, B);
    }

    return cache;
}

void cacheFree(Cache* cache) {
    Set* sets = cache->sets;
    int S = cache->S;
    int E = cache->E;
    int B = cache->B;

    int i;
    for (i = 0; i < S; i++) {
        setFree(sets[i], E, B);
    }
    free(sets);
    free(cache);

    return;
}

void cacheHit(Cache* cache, Set* set, Line* line, int E) {
    increaseLineAge(set, E);
    line->age = 0;
    cache->hits++;
    return;
}

void cacheMiss(Cache* cache, Set* set, int E, long long tag) {
    cache->misses++;
    Line* oldestLine = NULL;
    increaseLineAge(set, E);
    int i;
    for (i = 0; i < E; i++) {
        Line* line = &(set->lines[i]);
        if (!line->valid) {
            line->tag = tag;
            line->age = 0;
            line->valid = 1;
            return;
        } else if (oldestLine == NULL || line->age > oldestLine->age) {
            oldestLine = line;
        }
    }

    oldestLine->tag = tag;
    oldestLine->age = 0;
    cache->evicts++;
    return;
}

void cacheSearch(Cache* cache, long long address) {
    int s = cache->s;
    int E = cache->E;
    int b = cache->b; 
    int tagSize = cache->tagSize;

    int setNum = (address >> b) & ~(~0 << s); // Mask Relevant Set Number from Address //
    long long tag = (address >> (s + b)) & ~(~0 << tagSize); // Mask Tag to Search For //

    Set* set = &(cache->sets[setNum]); // Set Based on Masked Number //
    Line* line = setSearch(set, tag, E); // Search Set for Line Matching Tag //

    if(line != NULL) {
        cacheHit(cache, set, line, E);
        return;
    } else {
        cacheMiss(cache, set, E, tag);
        return;
    }


}

int main(int argc, char* argv[]) {
    FILE* trace;
    int s, E, b;
    //int v = 0;
    char option;
    char* t;
    while ((option = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (option) {
            case 'h': {
                printUsage();
                exit(EXIT_SUCCESS);
            }
            case 'v': {
                // v = 1;
                break;
            }
            case 's': {
                s = atoi(optarg);
                break;
            }
            case 'E': {
                E = atoi(optarg);
                break;
            }
            case 'b': {
                b = atoi(optarg);
                break;
            }
            case 't': {
                t = optarg;
                break;
            }
            default: {
                printf( "Unknown option encountered! Please enter option as usage!\n" );
                printUsage();
                exit(EXIT_FAILURE);
            }
        }
    }

    // Check that valid arguments were provided
    if (s == 0 || E == 0 || b == 0 || t == NULL) {
        printf("Please Provide Valid Arguments for the Cache as usage!\n");
        printUsage();
        exit(EXIT_FAILURE);
    }

    Cache* cache = cacheInit(s,E,b);

    trace = fopen(t, "r");
    if (trace != NULL) {
        char instruction;
        long long address;
        int size;

        while (fscanf(trace, " %c %llx,%d", &instruction, &address, &size) == 3) {
            switch (instruction) {
                case 'I': {
                    break;
                }
                case 'L': {
                    cacheSearch(cache, address);
                    break;
                }
                case 'S': {
                    cacheSearch(cache, address);
                    break;
                }
                case 'M': {
                    cacheSearch(cache, address); // Data Load //
                    cacheSearch(cache, address); // Data Store //
                    break;
                }
                default: {
                    break;
                }
            }
        }
    } else {
        printf("No trace file find!\n");
    }

    // Print Resulting Hits, Misses, Evictions //
    printSummary(cache->hits, cache->misses, cache->evicts);

    fclose(trace);
    cacheFree(cache);
    return 0;
}
