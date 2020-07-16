#include "Data.h"
#include "simtimer.h"
#include "Util.h"
#include "System.h"
#include <pthread.h>
#include <stdlib.h>

int run( struct configInfo *configData, struct process *processHead )
{
  struct process *current = processHead;

  struct fileOutput *output = malloc( sizeof( struct fileOutput ) );

  struct processList *tempProcessList, *processList = malloc( sizeof( struct processList ));

  //error output for PCBCreate
  int error;

  if( current->command == 'S' && strCompare(current->operation, "start") == 0 )
  {
    logLine( "System Start", configData, output );
  }
  else
  {
    return MALFORMED;
  }

  logLine("OS: Begin PCB Creation", configData, output );

  error = PCBCreate( processList, processHead->nextProcess, configData );

  if( error != CORRECT )
  {
    return error;
  }

  logLine( "OS: All processes initialized in New state", configData, output );

  tempProcessList = processList;

  while( tempProcessList != NULL )
  {
    tempProcessList->state = READY;

    tempProcessList = tempProcessList->nextProcess;
  }

  logLine( "OS: All processes now set in Ready state", configData, output );



  //process loop
  while( selectProcess( processList, &tempProcessList, configData, output ) == CORRECT )
  {
    tempProcessList->state = RUNNING;

    if( runProcess( tempProcessList, configData, output) != CORRECT )
    {
      return error;
    }

  }

  logLine( "System stop", configData, output );

  //end of running, logs the outputList to the desired log file
  if( configData->logLocation == FILE_LOG || configData->logLocation == BOTH_LOG )
  {
    writeToFile( output, configData );
  }

  deleteOutputList( output );

  deletePCB( processList );

  return 0;
}

int PCBCreate( struct processList *processList, struct process *processHead, struct configInfo *config )
{
  struct process *processIncrement = processHead;

  struct processList *currentProcess = processList;

  int processNumber = 0, processTime;

  //loop for each process
  while( processIncrement->command != 'S' && strCompare( processIncrement->operation, "end") != 0 )
  {


    //if no S(end)
    if( processIncrement == NULL )
    {
      return NO_END;
    }

    if( processIncrement->command == 'A' && strCompare( processIncrement->operation, "start" ) == 0 )
    {
      //should be at start of next process
      currentProcess->head = processIncrement;

      currentProcess->state = NEW;

      currentProcess->processNum = processNumber++;

      currentProcess->totalTime = 0;

      currentProcess->nextProcess = NULL;

      processIncrement = processIncrement->nextProcess;

      currentProcess->MMU = malloc(sizeof(struct mmu));

      //makes sure that the current MMU is always the head
      currentProcess->MMU->segment = 0;

      currentProcess->MMU->processNum = currentProcess->processNum;

      currentProcess->MMU->next = NULL;

      currentProcess->MMU->previous = NULL;

      initalizeMMUArr( currentProcess->MMU );

      //loop to find end of each process
      while( processIncrement->command != 'A' && strCompare( processIncrement->operation, "end" ) != 0 )
      {
        //if no A(end)
        if( processIncrement == NULL )
        {
          return MALFORMED;
        }

        processTime = processIncrement->cycleTime;

        //adding total time to the process for I/O operations
        if( processIncrement->command == 'I' || processIncrement->command == 'O' )
        {
          currentProcess->totalTime += processTime * config->ioCycle;
        }
        else if( processIncrement->command == 'P')
        {
          currentProcess->totalTime += processTime * config->procCycle;
        }

        processIncrement = processIncrement->nextProcess;

        currentProcess->tail = processIncrement;
      }

      //moves from A(end) to next one
      processIncrement = processIncrement->nextProcess;

      //if there is another process
      if( processIncrement->command == 'A' && strCompare( processIncrement->operation, "start") == 0 )
      {
        currentProcess->nextProcess = malloc( sizeof( struct processList ) );

        currentProcess = currentProcess->nextProcess;

        currentProcess->nextProcess = NULL;
      }
    }
    else
    {
      return MALFORMED;
    }

  }
  return CORRECT;
}

int runProcess( struct processList *processList, struct configInfo *config, struct fileOutput *output )
{
  //next process to skip A(start)
  struct process *current = processList->head->nextProcess;

  struct mmu *MMU = processList->MMU;

  char outputStr[ MAXSTRLEN ];

  //thread stuff for the sleep/runTimer function
  pthread_t timerT;

  pthread_attr_t timerAttr;

  pthread_attr_init( &timerAttr );

  int processTime = 0;

  //goes through the processes
  while( current != processList->tail )
  {

    processList->current = current;

    //input
    if( current->command == 'I')
    {

      processTime = config->ioCycle * current->cycleTime;

      //hard drive input
      if( strCompare( current->operation, "hard drive") == 0 )
      {
        sprintf( outputStr, "Process %d, hard drive input start", processList->processNum );

        logLine( outputStr, config, output );

        pthread_create(&timerT, &timerAttr, runTimer, &processTime );

        pthread_join( timerT, NULL );

        sprintf( outputStr, "Process %d, hard drive input end", processList->processNum );

        logLine( outputStr, config, output );
      }

      //keyboard input
      else if( strCompare( current->operation, "keyboard") == 0 )
      {
        sprintf( outputStr, "Process %d, keyboard input start", processList->processNum );

        logLine( outputStr, config, output );

        pthread_create(&timerT, &timerAttr, runTimer, &processTime );

        pthread_join( timerT, NULL );

        sprintf( outputStr, "Process %d, keyboard input end", processList->processNum );

        logLine( outputStr, config, output );
      }
      else
      {
        return MALFORMED;
      }

    }

    //output
    else if( current->command == 'O')
    {
      //hard drive output
      if( strCompare( current->operation, "hard drive") == 0 )
      {
        sprintf( outputStr, "Process %d, hard drive output start", processList->processNum );

        logLine( outputStr, config, output );

        pthread_create(&timerT, &timerAttr, runTimer, &processTime );

        pthread_join( timerT, NULL );

        sprintf( outputStr, "Process %d, hard drive output end", processList->processNum );

        logLine( outputStr, config, output );
      }

      //monitor output
      else if( strCompare( current->operation, "monitor") == 0 )
      {
        sprintf( outputStr, "Process %d, monitor output start", processList->processNum );

        logLine( outputStr, config, output );

        pthread_create(&timerT, &timerAttr, runTimer, &processTime );

        pthread_join( timerT, NULL );

        sprintf( outputStr, "Process %d, monitor output end", processList->processNum );

        logLine( outputStr, config, output );
      }

      //printer output
      else if( strCompare( current->operation, "printer") == 0 )
      {
        sprintf( outputStr, "Process %d, printer output start", processList->processNum );

        logLine( outputStr, config, output );

        pthread_create(&timerT, &timerAttr, runTimer, &processTime );

        pthread_join( timerT, NULL );

        sprintf( outputStr, "Process %d, printer output end", processList->processNum );

        logLine( outputStr, config, output );
      }
      else
      {
        return MALFORMED;
      }

    }

    //P run
    else if( current->command == 'P')
    {
      //run
      if( strCompare( current->operation, "run") == 0 )
      {
        sprintf( outputStr, "Process %d, run operation start", processList->processNum );

        logLine( outputStr, config, output );

        pthread_create(&timerT, &timerAttr, runTimer, &processTime );

        pthread_join( timerT, NULL );

        sprintf( outputStr, "Process %d, run operation end", processList->processNum );

        logLine( outputStr, config, output );
      }

      else
      {
        return MALFORMED;
      }

    }

    //memory
    else if( current->command == 'M')
    {
      //500 mSec for memory operation
      processTime = 500;

      struct mmuArgs *args = malloc(sizeof(struct mmuArgs));

      //memory allocate
      if( strCompare( current->operation, "allocate") == 0 )
      {
        args->timerT = timerT;

        args->timerAttr = timerAttr;

        args->MMU = MMU;

        args->current = current;

        args->config = config;

        args->output = output;

        if( allocateMMU( args ) != CORRECT )
        {
          //Time: 15.101012, OS: Process 2, Segmentation Fault - Process ended

          sprintf( outputStr, "OS: Process %d, Segmentation Fault - Process ended", processList->processNum );

          logLine( outputStr, config, output );

          free( args );
          //if segfault, break out of execution
          break;
        }
      }

      //memory access
      else if( strCompare( current->operation, "access") == 0 )
      {
        args->timerT = timerT;

        args->timerAttr = timerAttr;

        args->MMU = MMU;

        args->current = current;

        args->config = config;

        args->output = output;

        if( accessMMU( args ) != CORRECT )
        {
          //Time: 15.101012, OS: Process 2, Segmentation Fault - Process ended

          sprintf( outputStr, "OS: Process %d, Segmentation Fault - Process ended", processList->processNum );

          logLine( outputStr, config, output );

          free( args );
          //if segfault, break out of execution
          break;

        }
      }
      else
      {
        return MALFORMED;
      }

      free( args );

    }

    current = current->nextProcess;
  }

  processList->state = EXIT;

  sprintf( outputStr, "Process %d set in Exit state", processList->processNum );

  logLine( outputStr, config, output );

  return CORRECT;
}
