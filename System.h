#ifndef SYSTEM_H
#define SYSTEM_H

#include <pthread.h>
#include "Data.h"
#include "simtimer.h"
#include "Util.h"

#define NO_END 2

int run( struct configInfo *configData, struct process *processHead );

int PCBCreate( struct processList *processList, struct process *processHead, struct configInfo *configData );

int runProcess( struct processList *processList, struct configInfo *config, struct fileOutput *output );

#endif
