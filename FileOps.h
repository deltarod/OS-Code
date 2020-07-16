// Header File Information /////////////////////////////////////
/**
* @file FileOps.h
*
* @brief Header File for FileOps
*
* @version 1.00
* Sim01
*
*/

#ifndef FILEOPS_H

#include <stdio.h>
#include "Data.h"

#define CONFIG_FILE_ERROR 1
#define INVALID_VERSION_NUMBER 2
#define INVALID_CPU_SCHEDULE 3
#define INVALID_QUANT_TIME 4
#define INVALID_MEMORY_SIZE 5
#define INVALID_PROCESS_TIME 6
#define INVALID_IO_TIME 7
#define INVALID_LOG_LOCATION 8
#define MALFORMED_CONFIG_FILE 9

int configRead( FILE* config, struct configInfo *configStruct );

int metaDataRead( FILE* mdf, struct process *processHead );

#endif
