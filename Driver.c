// Program Information ////////////////////////////////////////////////
/**
 * @file Driver.c
*
* @brief Main driver
*
*
* @version 1.00
* Sim01
*
* @note Requires Util.h, data.h, simtimer.h, FileOps.h System.h
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "simtimer.h"
#include "Data.h"
#include "System.h"
#include "FileOps.h"
#include "Util.h"
#include "System.h"


int main( int arg, const char *args[])
{
  FILE *config, *mdf;

  //creates new config struct to pass in and store data
  struct configInfo *configData = malloc(sizeof(struct configInfo));

  struct process *processHead = malloc(sizeof(struct process));

  int configReturn, mdfReturn, runReturn;

  //checking for correct number of args
  if( arg != 2 )
  {
    printf("Invalid Number of args, input a config file \n");

    return 0;
  }

  //open args[1] since args[0] is the executable name
  //r for read
  config = fopen(args[1], "r");

  configReturn = configRead( config, configData );

  if( configErrorCheck( configReturn ) == 1 )
  {
    return 0;
  }

  fclose( config );

  mdf = fopen(configData->filePath, "r");

  mdfReturn = metaDataRead( mdf, processHead );



  switch( mdfReturn )
  {
    case MALFORMED:
      printf( "Malformed Meta Data File\n");
      fclose( mdf );
      return 0;
  }

  fclose( mdf );

  runReturn = run( configData, processHead );

  switch( runReturn )
  {
    case NO_END:
      printf( "No end to metaData\n" );

      return 0;

    case NOT_IMPLEMENTED:
      printf( "%s Schedule code not yet implemented", configData->cpuScheduleCode );

      return 0;
  }

  free( configData );

  deleteProcessList( processHead );

  return 0;
}
