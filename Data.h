// Header File Information /////////////////////////////////////
/**
* @file data.h
*
* @brief used for storing structs
*
* @version 1.00
* Sim01
*
*/

#ifndef DATA_H
#define DATA_H

#include <pthread.h>

#define MONITOR_LOG 1
#define FILE_LOG 2
#define BOTH_LOG 3

#define NEW 0
#define READY 1
#define RUNNING 2
#define BLOCKED 3
#define EXIT 4

#define MAXSTRLEN 1024

//structure for storing the configInfo
struct configInfo
{
  int versNum;
  char filePath[255];
  char cpuScheduleCode[10];
  int quantTime;
  int memory;
  int procCycle;
  int ioCycle;
  int logLocation;
  char logPath[255];
};

struct process
{
  char command;
  char operation[18];
  int cycleTime;
  int memory;

  struct process *nextProcess;
};

struct processList
{
  int processNum;
  int totalTime;
  int state;
  struct mmu *MMU;
  struct process *current;
  struct process *head;
  struct process *tail;

  struct processList *nextProcess;
};

struct mmu
{
  int segment;
  int processNum;
  int memory[999];
  struct mmu *next;
  struct mmu *previous;
};

struct mmuArgs
{
  pthread_t timerT;
  pthread_attr_t timerAttr;
  struct mmu *MMU;
  struct process *current;
  struct configInfo *config;
  struct fileOutput *output;
};

struct fileOutput
{
  char outputStr[ MAXSTRLEN ];

  struct fileOutput *nextOutput;
};

#endif
