// Header File Information /////////////////////////////////////
/**
* @file Util.h
*
* @brief Header File for Util
*
* @version 1.00
* Sim01
*
*/

#ifndef UTIL_H
#include <stdio.h>
#include "Data.h"
#include <pthread.h>

#define CPUSCHEDULEERROR 1
#define CPUSCHEDULECORRECT 0

#define CORRECT 0
#define MALFORMED 1
#define END_OF_METADATA 2
#define FINISHED 3

#define SEGFAULT 1

#define NOT_IMPLEMENTED 3

extern const char CFGDELIMITER;

extern const char MDFDELIMITER;

void extractInfo( char *input, char *output);

int extractProcess(char *line, int startOfProcess, int endOfProcess, struct process *node);

int checkMalformedConfig( FILE* config );

int checkMalformedConfigHelper( char *lineToCheck, FILE* config );

int configErrorCheck( int errorNum );

int strlength( char input[] );

void strCopy ( char *input, char *output );

int strCompare( char *oneStr, char *otherStr );

int strCompareToDelim( char *oneStr, char *otherStr, char DELIMITER );

int checkForSpace( char *str );

int parseInt( char *str );

int lenOfIntStr( char *str );

int powerOf( int base, int power );

int cpuCodeCheck( char *cpuCode, struct configInfo *configStruct );

void strConcat( char *firstStr, char *secondStr, char *output );

int selectProcess( struct processList *firstProcessList,
                    struct processList **desiredProcessList,
                    struct configInfo *config,
                    struct fileOutput *output);

void logLine( char *infoStr, struct configInfo *config, struct fileOutput *output );

void writeToFile( struct fileOutput *outputList,  struct configInfo *config );

int allocateMMU( struct mmuArgs *args );

void initalizeMMUArr( struct mmu *MMU );

int accessMMU( struct mmuArgs *args );

void deleteProcessList( struct process *processList );

void deletePCB( struct processList *processList );

void deleteMMU( struct mmu *MMU );

void deleteOutputList( struct fileOutput *outputList );

#endif
