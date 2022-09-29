#include "scheduler.h"

/* File to be read in */
#define INPUTFILE "set4_process.in"

/* Number of processes to be read */
int processCount;

/* 
  Function to populate SchedulerConfig type with input from a file and return 
*/
SchedulerConfig readConfigParams();

/*
  Function to read processes from the input file and return an array of type Process
*/
Process** readProcesses(SchedulerConfig config);