// Program Information ////////////////////////////////////////////////
/**
 * @file FileOps.c
*
* @brief Deals with file input
*
*
* @version 1.00
* Sim01
*
* @note Requires FileOps.h, Util.h, data.h
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileOps.h"
#include "Util.h"
#include "Data.h"

int configRead( FILE* config, struct configInfo *configStruct )
{
  char inputBuffer[ MAXSTRLEN ];
  char extractOutput[ MAXSTRLEN ];

  int versionNumber, quantumTime, maxMemory, processTime, ioTime;

  if( config == NULL )
  {
    return CONFIG_FILE_ERROR;
  }

  if( checkMalformedConfig( config ) == MALFORMED )
  {
    return MALFORMED_CONFIG_FILE;
  }

  //ignore first line
  //fgets(StrPointer/output, Chars to be read, file stream)
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  //resets output array to empty
  //memset(strPointer/output, value to set, number of bytes to set)
  memset( inputBuffer, 0, sizeof( inputBuffer ) );

  //version line
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput );

  versionNumber = parseInt( extractOutput );

  if( versionNumber < 0 || versionNumber > 10 )
  {
      return INVALID_VERSION_NUMBER;
  }

  configStruct->versNum = versionNumber;

  memset( inputBuffer, 0, sizeof(inputBuffer));

  //file path
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, configStruct->filePath );

  memset( inputBuffer, 0, sizeof(inputBuffer));


  //CPU Scheduling code
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput );

  if( cpuCodeCheck( extractOutput, configStruct ) == CPUSCHEDULEERROR )
    {
      return INVALID_CPU_SCHEDULE;
    }


  memset( inputBuffer, 0, sizeof(inputBuffer));

  //quantum cycle time
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput );

  quantumTime = parseInt( extractOutput );

  if( quantumTime < 0 || quantumTime > 100 )
  {
    return INVALID_QUANT_TIME;
  }

  configStruct->quantTime = quantumTime;

  memset( inputBuffer, 0, sizeof(inputBuffer));


  //Available Memory
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput );

  //multiply by 1k to convert to bytes from kilobytes
  maxMemory = parseInt( extractOutput )*1000;

  if( quantumTime < 0 || maxMemory > 1048576 )
  {
    return INVALID_MEMORY_SIZE;
  }

  configStruct->memory = maxMemory;

  memset( inputBuffer, 0, sizeof(inputBuffer));

  //Processor Cycle Time
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput );

  processTime = parseInt( extractOutput );

  if( processTime < 1 || processTime > 1000 )
  {
    return INVALID_PROCESS_TIME;
  }

  configStruct->procCycle = processTime;

  memset( inputBuffer, 0, sizeof(inputBuffer));


  //IO Cycle Time
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput );

  ioTime = parseInt( extractOutput );

  if( ioTime < 1 || ioTime > 10000 )
  {
    return INVALID_IO_TIME;
  }

  configStruct->ioCycle = ioTime;

  memset( inputBuffer, 0, sizeof(inputBuffer));


  //log To:
  fgets(inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, extractOutput );

  if( strCompare( extractOutput, "Monitor") == 0 )
  {
    configStruct->logLocation = MONITOR_LOG;
  }

  else if( strCompare( extractOutput, "File") == 0 )
  {
    configStruct->logLocation = FILE_LOG;
  }

  else if( strCompare( extractOutput, "Both") == 0 )
  {
    configStruct->logLocation = BOTH_LOG;
  }
  else
  {
    return INVALID_LOG_LOCATION;
  }

  //file path
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  extractInfo( inputBuffer, configStruct->logPath );

  memset( inputBuffer, 0, sizeof(inputBuffer));


  return 0;
}

int metaDataRead( FILE* mdf, struct process *processHead )
{
  int startOfOp, increment, lineLength, parseOutput, numOfProcess = 0;

  const char semiColon = ';', period = '.';

  char inputBuffer[ MAXSTRLEN ];

  struct process * current = processHead;

  //skip first line
  fgets( inputBuffer, sizeof( inputBuffer ), mdf );

  while( 1 )
  {
    startOfOp = 0;

    fgets( inputBuffer, sizeof( inputBuffer ), mdf );

    lineLength = strlength( inputBuffer );

    for( increment = 0; increment < lineLength; increment++ )
    {

      if( inputBuffer[ increment ] == semiColon ||
          inputBuffer[ increment ] == period)
      {
        if( numOfProcess == 0 )
        {
          numOfProcess++;

          parseOutput = extractProcess( inputBuffer, startOfOp,
                                        increment, current );
        }
        else
        {
          numOfProcess++;

          current->nextProcess = malloc(sizeof(struct process));

          current = current->nextProcess;

          parseOutput = extractProcess( inputBuffer, startOfOp,
                                        increment, current );
        }

        switch ( parseOutput )
        {
          case MALFORMED:

            return MALFORMED;

          case END_OF_METADATA:

            return END_OF_METADATA;

        }

        //add one to skip space and semicolon
        startOfOp = increment + 2;
      }
    }
  }
  return 0;
}
