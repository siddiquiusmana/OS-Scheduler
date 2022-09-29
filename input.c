/* LIBRARIES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

/* INPUT FILE */
FILE *ifp;

/* SchedulerConfig Implementation */
SchedulerConfig readConfigParams(){
  // Open the input file
  ifp = fopen(INPUTFILE, "r");

  // SchedulerConfig type declaration to be returned
  SchedulerConfig configs;

  // Temporary variable for storing keyword
  char keyword[50];

  fscanf(ifp, "%s", keyword);

  // Start parsing the document
  while(strcmp(keyword, "end") != 0)
  {
    // If the keyword is "processcount"
    if(strcmp(keyword, "processcount") == 0)
    {
      // Extract the processCount
      fscanf(ifp, "%d", &processCount);
    }

    // If the keyword is "runfor"
    if(strcmp(keyword, "runfor") == 0)
    {
      // Extract the runFor
      fscanf(ifp, "%d", &(configs.runFor));
    }

    // If the keyword is "runfor"
    if(strcmp(keyword, "use") == 0)
    {
      // Variable for scheduler to be used
      char schedulerToUse[50];

      // Extract schedulerToUse
      fscanf(ifp, "%s", schedulerToUse);

      // Set selectedScheduler
      if(strcmp(schedulerToUse, "fcfs") == 0)
        configs.selectedScheduler = 1;

      else if(strcmp(schedulerToUse, "sjf") == 0)
        configs.selectedScheduler = 2;

      else if(strcmp(schedulerToUse, "rr") == 0)
        configs.selectedScheduler = 3;
    }

    // If the keyword is "quantum" AND selectedScheduler is Round Robin
    if(strcmp(keyword, "quantum") == 0 && configs.selectedScheduler == 3)
      fscanf(ifp, "%d", &(configs.quantum));

    // Scan the next entry in input file
    fscanf(ifp, "%s", keyword);
  }
  
  // Close the input file
  fclose(ifp);

  printf("%d processes\n", processCount);

  // return the SchedulerConfigs type
  return configs;
}

/* readProcess Implementation */
Process** readProcesses(SchedulerConfig config)
{
  // Allocate memory for an array of Processes
  Process** listOfProcesses = (Process**) malloc(sizeof(Process*) * processCount);
  
  // Open the input file
  ifp = fopen(INPUTFILE, "r");

  // Temporary variable for iterations
  int i=0;
  
  // Temporary variable for storing keyword
  char keyword[50];

  fscanf(ifp, "%s", keyword);

  // Start parsing the document
  while(strcmp(keyword, "end") != 0)
  {
    // If the keyword is "process "
    if(strcmp(keyword, "process") == 0)
    {
      // Temporary string variable to store the process name
      char name[21];

      // Temporary variable to store time of arrival
      int timeOfArrival;

      // Temporary variable to store the number of bursts required
      int burstsRequired;
      
      // Discard the next word: "name"
      fscanf(ifp, "%s", keyword);
      
      // Extract name
      fscanf(ifp, "%s", name);

      // Discard the next word: "arrival"
      fscanf(ifp, "%s", keyword);

      // Extract timeOfArrival
      fscanf(ifp, "%d", &timeOfArrival);

      // Discard the next word: "burst"
      fscanf(ifp, "%s", keyword);

      // Extract burstsRequired
      fscanf(ifp, "%d", &burstsRequired);

      // Create a process and add it to the listOfProcesses
      listOfProcesses[i++] = createProcess(name, burstsRequired, timeOfArrival);
    }

    // Fetch the next keyword
    fscanf(ifp, "%s", keyword);
  }

  // Close the input file
  fclose(ifp);

  return listOfProcesses;
}