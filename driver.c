/* LIBRARIES */
#include <stdio.h>
#include "input.h"

// Begin Main
int main(void) {
  // Get the Scheduler Configs
  SchedulerConfig config = readConfigParams();

  // Scheduler type declaration
  Scheduler scheduler;
  scheduler.waitingQueue = NULL;
  scheduler.arrivalQueue = NULL;
  scheduler.processInExecution = NULL;
  scheduler.finishedProcesses = NULL;
  scheduler.processCount = processCount;
  scheduler.numFinishedProcesses = 0;
  scheduler.currentBurst = 0;
  
  // Read all the processes from input file
  scheduler.arrivalQueue = readProcesses(config);

  // Sort the processes by timeOfArrival
  sortbyTimeOfArrival(config, scheduler.arrivalQueue, 0, processCount-1);

  // Call the appropriate schedulers and execute processes
  executeProcesses(config, &scheduler);

  // Display final metrics for scheduler's runtime
  showFinalMetrics(&scheduler);

  // Deallocate all memory for the program
  cleanUp(&scheduler);

  return 0;
}