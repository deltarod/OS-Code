// Program Information ////////////////////////////////////////////////
/**
 * @file Util.c
*
* @brief Utilites for other functions
*
*
* @version 1.00
* Sim01
*
* @note Requires Util.h, data.h, FileOps.h, simtimer.h
*/


#include <stdio.h>
#include <stdlib.h>
#include "FileOps.h"
#include "Util.h"
#include "Data.h"
#include "simtimer.h"

const char CFGDELIMITER = ':';

const char MDFDELIMITER = ';';

//extracts the setting from config file line, regardless of which line it is
//returns the data via the output pointer
void extractInfo( char *input, char *output )
{
  int increment = 0;
  int outputLen = 0;

  int stringlen = strlength( input );

  //finds where the delimiter is (:)
  while( input[ increment ] != CFGDELIMITER )
  {
    increment++;
  }

  //add two to skip space and delimiter
  increment += 2;

  while( increment < stringlen )
  {
    output[ outputLen++ ] = input[ increment++ ];
  }

  //format value back to string
  output[ outputLen ] = '\0';

}

int extractProcess(char *line, int startOfProcess, int endOfProcess, struct process *node)
{
  int increment, startIncrement, closeBracketIdx, parsed;

  const char openBracket = '(', closeBracket = ')',
       semiColon = ';', period = '.';

  char strInt[ MAXSTRLEN ] = "";

  node->command = line[ startOfProcess ];

  node->nextProcess = NULL;

  //check for opening bracket
  if( line[startOfProcess + 1] != openBracket )
  {
    return MALFORMED;
  }

  increment = startOfProcess;

  //finds closing bracket
  while( line[ increment ] != semiColon || line[ increment ] == period )
  {
    if( line[ increment ] == closeBracket )
    {
      closeBracketIdx = increment;

      break;
    }
    if( increment >= endOfProcess )
    {
      return MALFORMED;
    }
    increment++;
  }

  //gets number at end of process and parses
  startIncrement = 0;

  for( increment = closeBracketIdx + 1; increment < endOfProcess; increment++ )
  {
    //check if end is actually number
    if( line[ increment ] < '0' || line[ increment ] > '9' )
    {
      return MALFORMED;
    }

    strInt[startIncrement++ ] = line[ increment ];
  }

  parsed = parseInt( strInt );

  startIncrement = 0;

  for( increment = startOfProcess + 2; increment < closeBracketIdx; increment++ )
  {
    node->operation[ startIncrement++ ] = line[ increment ];
  }

  //end of string
  node->operation[ startIncrement ] = '\0';

  switch( node->command )
  {
    case 'S':
      if( strCompare( node->operation, "end" ) == 0 )
      {
        node->cycleTime = parsed;
        return END_OF_METADATA;
      }
    case 'M':
      node->memory = parsed;
      break;
  }

  node->cycleTime = parsed;

  return CORRECT;
}

int checkMalformedConfig( FILE* config )
{
  char inputBuffer[ MAXSTRLEN ];

  int error = 0;

  //first line ignored
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  //Version Check
  error += checkMalformedConfigHelper( "Version/Phase:", config );

  //FilePath check
  error += checkMalformedConfigHelper( "File Path:", config );

  //CPU Schedule check
  error += checkMalformedConfigHelper( "CPU Scheduling Code:", config );

  //Quantum check
  error += checkMalformedConfigHelper( "Quantum Time (cycles):", config );

  //Memory check
  error += checkMalformedConfigHelper( "Memory Available (KB):", config );

  //Processor Cycle time check
  error += checkMalformedConfigHelper( "Processor Cycle Time (msec):", config );

  //I/O Cycle Time check
  error += checkMalformedConfigHelper( "I/O Cycle Time (msec):", config );

  //log to check
  error += checkMalformedConfigHelper( "Log To:", config );

  //Log File Path check
  error += checkMalformedConfigHelper( "Log File Path:", config );


  // makes fgets start from beginning
  rewind( config );

  if( error == 0 )
  {
    return CORRECT;
  }
  return MALFORMED;
}


int checkMalformedConfigHelper( char *lineToCheck, FILE* config )
{
  char inputBuffer[ MAXSTRLEN ];

  fgets( inputBuffer, sizeof( inputBuffer ), config );

  if( strCompareToDelim( inputBuffer, lineToCheck, CFGDELIMITER ) != 0 )
  {

    return MALFORMED;
  }

  return CORRECT;
}

int configErrorCheck( int errorNum )
{
  switch( errorNum )
  {
    case CONFIG_FILE_ERROR:
      printf("Config file does not exist\n");
      return 1;

    case MALFORMED_CONFIG_FILE:
      printf( "Malformed Configuration File \n" );
      return 1;

    case INVALID_VERSION_NUMBER:
      printf("Version number invalid\n");
      return 1;

    case INVALID_CPU_SCHEDULE:
      printf("Invalid CPU schedule code\n");
      return 1;

    case INVALID_QUANT_TIME:
      printf("Invalid Quantum Time\n");
      return 1;

    case INVALID_MEMORY_SIZE:
      printf("Invalid Memory Amount\n");
      return 1;

    case INVALID_PROCESS_TIME:
      printf("Invalid Process Time\n");
      return 1;

    case INVALID_IO_TIME:
      printf("Invalid I/O Time\n");
      return 1;
  }

  return 0;
}


//finds length of string, until \n
int strlength( char input[] )
{
  int length = 0;

  while( input[length] != '\n' )
  {
    length++;
  }

  return length;
}

void strCopy ( char *input, char *output )
{
  int increment;

  int size = 0;

  while( input[ size ] != '\0' )
  {
    size++;
  }

  for( increment = 0; increment < size; increment++ )
  {
    output[ increment ] = input[ increment ];
  }

  //show end of string
  output[ increment ] = '\0';

}

//found from https://stackoverflow.com/questions/14232990/comparing-two-strings-in-c
int strCompare( char *oneStr, char *otherStr )
{
  int increment = 0;

  while( oneStr[ increment ] == otherStr [ increment ] )
  {
    if( oneStr [ increment ] == '\0' || otherStr[ increment ] == '\0' )
    {
      break;
    }
    increment++;
  }

  if( oneStr[ increment ] == '\0' && otherStr[ increment ] == '\0')
  {
    return 0;
  }

  return 1;
}

//found from https://stackoverflow.com/questions/14232990/comparing-two-strings-in-c
int strCompareToDelim( char *oneStr, char *otherStr, char DELIMITER )
{
  int increment = 0;

  while( oneStr[ increment ] == otherStr [ increment ] )
  {
    if( oneStr [ increment ] == DELIMITER || otherStr[ increment ] == DELIMITER )
    {
      break;
    }
    increment++;
  }

  if( oneStr[ increment ] == DELIMITER && otherStr[ increment ] == DELIMITER )
  {
    return 0;
  }

  return 1;
}

int parseInt( char *str )
{
  //increment for loop
  int increment;
  //finds length of
  int lenOfStrParse = lenOfIntStr( str );
  //used for a power function, parsing int sort of using scientific notation
  //as in for 56, 5 is multiplied by 10, then add 6 to it
  int currentPower = lenOfStrParse - 1;

  int output = 0;

  for( increment = 0; increment < lenOfStrParse; increment++ )
  {
    output = output + (str[ increment ] - '0') * powerOf( 10, currentPower--);
  }

  return output;
}

int lenOfIntStr( char *str )
{
  int increment = 0;

  while( str[ increment ] >= '0' && str[ increment ] <= '9' )
  {
    increment++;
  }

  return increment;
}

int powerOf( int base, int power )
{
  if( power == 0 )
  {
    return 1;
  }
  return base * powerOf( base, power - 1 );
}

int cpuCodeCheck( char *cpuCode, struct configInfo *configStruct )
{
  //CPU code checks
  if( strCompare( cpuCode, "NONE") == 0 )
  {
    strCopy("FCFS-N", configStruct->cpuScheduleCode);
    return CPUSCHEDULECORRECT;
  }

  else if( strCompare( cpuCode, "FCFS-N") == 0 )
  {
    strCopy("FCFS-N", configStruct->cpuScheduleCode);

    return CPUSCHEDULECORRECT;
  }

  //TODO Error here?
  else if( strCompare( cpuCode, "SJF-N") == 0 )
  {
    strCopy("SJF-N", configStruct->cpuScheduleCode);

    return CPUSCHEDULECORRECT;
  }

  else if( strCompare( cpuCode, "SRTF-P") == 0 )
  {
    strCopy("SRTF-P", configStruct->cpuScheduleCode);

    return CPUSCHEDULECORRECT;
  }

  else if( strCompare( cpuCode, "FCFS-P") == 0 )
  {
    strCopy("FCFS-P", configStruct->cpuScheduleCode);

    return CPUSCHEDULECORRECT;
  }

  else if( strCompare( cpuCode, "RR-P") == 0 )
  {
    strCopy("RR-P", configStruct->cpuScheduleCode);

    return CPUSCHEDULECORRECT;
  }
  return CPUSCHEDULEERROR;
}

void strConcat( char *firstStr, char *secondStr, char *output )
{
  int strLen = 0;
  int concatStrLen = 0;

  //goes to end of first string
  while( firstStr[ strLen ] != '\0')
  {
    output[ strLen ] = firstStr[ strLen ];

    strLen++;
  }

  while( secondStr[ concatStrLen ] != '\0' )
  {
    output[ strLen ] = secondStr[ concatStrLen ];

    strLen++;
    secondStr++;
  }
  output[ strLen ] = '\0';
}

int selectProcess( struct processList *firstProcessList,
                    struct processList **desiredProcessList,
                    struct configInfo *config,
                    struct fileOutput *output)
{
  struct processList *shortest, *temp = firstProcessList;

  char *scheduleCode = config->cpuScheduleCode;

  char outputStr[ MAXSTRLEN ] = "";


  if( strCompare( scheduleCode, "FCFS-N") == 0 )
  {

    //gets next ready process
    while( temp != NULL && temp->state != READY )
    {
      temp = temp->nextProcess;
    }

    if( temp == NULL )
    {
      return FINISHED;
    }

    *desiredProcessList = temp;
  }

  else if( strCompare( scheduleCode, "SJF-N") == 0 )
  {


    //gets first ready process
    while( temp != NULL && temp->state != READY )
    {
      temp = temp->nextProcess;
    }

    if( temp == NULL )
    {
      return FINISHED;
    }

    shortest = temp;

    //gets shortest process, comparing to first ready process
    while( temp != NULL )
    {
      if( temp->state == READY && shortest->totalTime > temp->totalTime)
      {
        shortest = temp;
      }

      temp = temp->nextProcess;
    }

    *desiredProcessList = shortest;
  }

  else if( strCompare( scheduleCode, "SRTF-P") == 0 )
  {
    return NOT_IMPLEMENTED;
  }

  else if( strCompare( scheduleCode, "FCFS-P") == 0 )
  {
    return NOT_IMPLEMENTED;
  }

  else if( strCompare( scheduleCode, "RR-P") == 0 )
  {
    return NOT_IMPLEMENTED;
  }

  sprintf( outputStr, "OS: %s Strategy selects Process %d with time: %dmSec",
           scheduleCode, (*desiredProcessList)->processNum, (*desiredProcessList)->totalTime );

  logLine( outputStr, config, output );

  return CORRECT;
}

void logLine( char *infoStr, struct configInfo *config, struct fileOutput *output )
{
  struct fileOutput *endOfOutput;

  char timeValueStr[ MAXSTRLEN ] = "";

  char concatOutput[ MAXSTRLEN ];

  //if system is starting, time is 0
  if( strCompare( infoStr, "System Start" ) == 0 )
  {
    accessTimer( ZERO_TIMER, timeValueStr );
  }
  else
  {
    accessTimer( LAP_TIMER, timeValueStr );
  }

  sprintf( concatOutput, "time: %s, %s\n", timeValueStr, infoStr );

  if( config->logLocation == MONITOR_LOG || config->logLocation == BOTH_LOG )
  {
    printf( concatOutput );
  }

  //puts output into a list to later be put into a file at end of execution
  if( config->logLocation == FILE_LOG || config->logLocation == BOTH_LOG )
  {
    endOfOutput = output;

    //if it is the first one logged
    if( strCompare( "System Start", infoStr ) == 0 )
    {
      strCopy( concatOutput, endOfOutput->outputStr );

      endOfOutput->nextOutput = NULL;
    }
    else
    {
      while( endOfOutput->nextOutput != NULL )
      {
        endOfOutput = endOfOutput->nextOutput;
      }

      endOfOutput->nextOutput = malloc( sizeof( struct fileOutput ));

      endOfOutput->nextOutput->nextOutput = NULL;

      strCopy( concatOutput, endOfOutput->nextOutput->outputStr );

    }
  }
}

//writes to file after finish
void writeToFile( struct fileOutput *outputList,  struct configInfo *config )
{
  struct fileOutput *current = outputList;

  FILE *logFile = fopen(config->logPath, "w+");


  while( current != NULL )
  {
    fputs( current->outputStr, logFile );

    current = current->nextOutput;
  }
}

int allocateMMU( struct mmuArgs *args )
{
  //half second for memory operations
  int segment, base, allocated, index, endIndex, processTime = 500;

  char outputStr[ MAXSTRLEN ];

  struct mmu *current = args->MMU;

  //gets first 2 digits which is segment
  segment = args->current->memory/1000000;

  //gets memory base
  base = ( args->current->memory%1000000)/1000;

  allocated = args->current->memory%1000;

  endIndex = base+allocated;

  sprintf( outputStr, "Process %d, MMU Allocation: %d/%d/%d", current->processNum, segment, base, allocated );

  logLine(outputStr, args->config, args->output );

  //since each segment can only store up to 999 locations
  if( (allocated + base) >= 999 )
  {
    sprintf( outputStr, "Process %d, MMU Allocation: Failed", current->processNum );

    logLine(outputStr, args->config, args->output );

    return SEGFAULT;
  }

  while( current->next != NULL )
  {

    if( current->segment < segment )
    {
    current = current->next;
    }
    else
    {
      break;
    }
  }
  if( current->segment == segment )
  {
    for( index = base; index < endIndex; index++ )
    {
      current->memory[index] = args->current->memory;
    }
  }
  else
  {
    //if current is end of list
    if( current->next == NULL )
    {
      current->next = malloc(sizeof(struct mmu));

      current->next->previous = current;

      current = current->next;

      current->next = NULL;

      current->processNum = current->previous->processNum;

      current->segment = segment;

      initalizeMMUArr( current );

      for( index = base; index < endIndex; index++ )
      {
        current->memory[index] = args->current->memory;
      }
    }
    //if new mmu struct needs to go inbetween two already setup structs
    else
    {
      struct mmu *temp = current->next;

      current->next = malloc(sizeof(struct mmu));

      current->next->previous = current;

      current = current->next;

      current->next = temp;

      temp->previous = current;

      initalizeMMUArr( current );

      for( index = base; index < endIndex; index++ )
      {
        current->memory[index] = args->current->memory;
      }
    }
  }

  pthread_create(&args->timerT, &args->timerAttr, runTimer, &processTime );

  pthread_join( args->timerT, NULL );

  sprintf( outputStr, "Process %d, MMU Allocation: Successful", current->processNum );

  logLine(outputStr, args->config, args->output );

  return CORRECT;

}

void initalizeMMUArr( struct mmu *MMU )
{
  int index;

  for( index = 0; index <= 999; index++ )
  {
    MMU->memory[index] = 0;
  }
}

int accessMMU( struct mmuArgs *args )
{
  int segment, base, allocated, index, endIndex, processTime = 500;

  char outputStr[ MAXSTRLEN ];

  struct mmu *current = args->MMU;

  //gets first 2 digits which is segment
  segment = args->current->memory/1000000;

  //gets memory base
  base = ( args->current->memory%1000000)/1000;

  allocated = args->current->memory%1000;

  endIndex = base+allocated;

  sprintf( outputStr, "Process %d, MMU Access: %d/%d/%d", current->processNum, segment, base, allocated );

  logLine(outputStr, args->config, args->output );

  while( current->next != NULL )
  {

    if( current->segment < segment )
    {
    current = current->next;
    }
    else
    {
      break;
    }
  }

  pthread_create(&args->timerT, &args->timerAttr, runTimer, &processTime );

  pthread_join( args->timerT, NULL );

  if( current->segment == segment )
  {
    for( index = base; index < endIndex; index++ )
    {
      if( current->memory[index] == 0 )
      {
        sprintf( outputStr, "Process %d, MMU Access: Failed", current->processNum );

        logLine(outputStr, args->config, args->output );

        return SEGFAULT;
      }
    }
  }
  else
  {
    sprintf( outputStr, "Process %d, MMU Access: Failed", current->processNum );

    logLine(outputStr, args->config, args->output );

    return SEGFAULT;
  }

  sprintf( outputStr, "Process %d, MMU Access: Successful", current->processNum );

  logLine(outputStr, args->config, args->output );

  return CORRECT;
}

void deleteProcessList( struct process *processList )
{
  if( processList->nextProcess != NULL )
  {
    deleteProcessList( processList->nextProcess );
  }

  free( processList );
}

void deletePCB( struct processList *processList )
{
  if( processList->nextProcess != NULL )
  {
    deletePCB( processList->nextProcess );
  }

  deleteMMU( processList->MMU );

  free( processList );
}

void deleteMMU( struct mmu *MMU )
{
  if( MMU->next != NULL )
  {
    deleteMMU( MMU->next );
  }

  free( MMU );
}

//recursivly free the outputs from memory
void deleteOutputList( struct fileOutput *output )
{
  if( output->nextOutput != NULL )
  {
    deleteOutputList( output->nextOutput );
  }

free( output );
}
