/* LIBRARIES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

// createProcess Implementation
Process* createProcess(char* processName, int burstsRequired, int timeOfArrival)
{
  // Allocate memory for a process
  Process* process = (Process*) malloc(sizeof(Process));

  // Populate the fields with passed in parameters
  strcpy(process->name, processName);
  process->burstsRequired = burstsRequired;
  process->timeOfArrival = timeOfArrival;

  // set processStatus to Arrived
  process->processStatus = 1;

  // return the process
  return process;
}

// swap Implementation
void swap(Process **a, Process **b) {
	Process *temp = *a;
	*a = *b;
	*b = temp;
}

// partitionByBurstsRequired Implementation
int partitionByBurstsRequired(Process **processes, int low, int high)
{
  int i = low + rand()%(high-low+1);
  
  swap(&processes[low], &processes[i]);
  
  int lowpos = low; 
  low++;
  
  while(low<=high)
  {
    while(low<=high && processes[low]->burstsRequired<=processes[lowpos]->burstsRequired) 
      low++;
    
    while(high>=low && processes[high]->burstsRequired > processes[lowpos]->burstsRequired)   
      high--;
    
    if (low<high)
     swap(&processes[low], &processes[high]);
  }
  swap(&processes[lowpos], &processes[high]);
  return high; 
}

// partition Implementation
int partitionByTimeOfArrival(Process **processes, int low, int high)
{
  int i = low + rand()%(high-low+1);
  
  swap(&processes[low], &processes[i]);
  
  int lowpos = low; 
  low++;
  
  while(low<=high)
  {
    while(low<=high && processes[low]->timeOfArrival<=processes[lowpos]->timeOfArrival) 
      low++;
    
    while(high>=low && processes[high]->timeOfArrival > processes[lowpos]->timeOfArrival)   
      high--;
    
    if (low<high)
     swap(&processes[low], &processes[high]);
  }
  swap(&processes[lowpos], &processes[high]);
  return high; 
}

// sortbySchedulerType Implementation
void sortbySchedulerType(SchedulerConfig config, Process** processes, int low, int high) {
  if (low < high) {
    int split;
    if(config.selectedScheduler == 2){
      split = partitionByBurstsRequired(processes, low, high);
      printf("here!");
    }
    else
      split = partitionByTimeOfArrival(processes,low,high);
    sortbySchedulerType(config, processes,low,split-1);
    sortbySchedulerType(config, processes,split+1,high);
  }
}

// sortbyTimeOfArrival Implementation
void sortbyTimeOfArrival(SchedulerConfig config, Process** processes, int low, int high) {
    if (low < high) {
        int split;
        split = partitionByTimeOfArrival(processes, low, high);
        sortbyTimeOfArrival(config, processes,low,split-1);
        sortbyTimeOfArrival(config, processes,split+1,high);
    }
}

// createQueueNode Implementation
ProcessQueue* createQueueNode(Process* process)
{
  // Allocate memory for the node
  ProcessQueue* node = (ProcessQueue*) malloc(sizeof(ProcessQueue));

  // Initialize ProcessQueue node with process and set next to NULL
  node->process = process;
  node->next = NULL;

  // Return the node
  return node;
}

void processPushByBurstSize(Process* process, Scheduler* scheduler)
{
  ProcessQueue** head = &((*scheduler).waitingQueue);

  // If queue is empty, initialize the head
  if(*head == NULL){
    *head = createQueueNode(process);
    return;
  }

  // Traversal variable
  ProcessQueue *traverse = (*head);

  // If the process to be inserted has a shorter burst size, make it the new head
  if(process->burstsRequired <= traverse->process->burstsRequired)
  {
    ProcessQueue* node = createQueueNode(process);
    node->next = traverse;
    node->process->processStatus = 2;
    *head = node;
    return;
  }

  // Continue traversing for as long as the next link has a smaller burst size
  else if(traverse->process->burstsRequired <= process->burstsRequired)
  {
    while(traverse->next != NULL)
    {
      // If the node ahead has a bigger burst size then break
      if(process->burstsRequired <= traverse->next->process->burstsRequired)
        break;

      traverse = traverse->next;
    }
  }
  // Unlink the Queue
  ProcessQueue* temp = traverse->next;

  // Add the processQueue node to be next
  traverse->next = createQueueNode(process);

  // Relink the Queue
  traverse->next->next = temp;

  // Update process status to waiting in queue
  traverse->next->process->processStatus = 2;
  
}

// processPush Implementation
void processPush(Process* process, Scheduler* scheduler)
{
  ProcessQueue** head = &((*scheduler).waitingQueue);
  
  // If queue is empty, initialize the head
  if(*head == NULL){
    *head = createQueueNode(process);
    return;
  }

  // Traverse through the queue
  ProcessQueue *traverse = (*head);
  while(traverse->next != NULL)
  {
    // Check if the process is already in the queue 
    if(strcmp(traverse->process->name, process->name) == 0 ){
      return;
    }  
    traverse = traverse->next;
  }
  // Add the process to the end of the Queue
  traverse->next = createQueueNode(process);

  // Update process status to waiting in queue
  traverse->next->process->processStatus = 2;
  
}

// processPop Implementation
Process* processPop(ProcessQueue** head)
{
  // Check if the queue is empty
  if(*head == NULL)
    return NULL;

  // Extract the process to be popped
  ProcessQueue* poppedNode = *head;
  Process* poppedProcess = poppedNode->process;

  // Set the new head
  (*head) = (*head)->next;

  // Free the poppedNode
  free(poppedNode);

  // Return the poppedProcess
  return poppedProcess;
}

// showQueue Implementation
void showQueue(ProcessQueue* head)
{
  if(head == NULL)
    return;

  printf("%s ->", head->process->name);
  
  ProcessQueue* t = head;
  while(t->next != NULL){
    printf("%s ->", t->next->process->name);
    t = t->next;
  }
  printf("\n");
}

// executeProcesses Implementation
void executeProcesses(SchedulerConfig config, Scheduler* scheduler)
{  
  // Contact the appropriate scheduler based on configs  
  if(config.selectedScheduler == 1){
    printf("Using First Come First Serve\n\n");
    fcfs(config, scheduler);
  }
  else if(config.selectedScheduler == 2){
    printf("Using Shortest Job First\n\n");
    sjf(config, scheduler);
  }
  else if(config.selectedScheduler == 3){
    printf("Using Round Robin\n\n");
    rr(config, scheduler);    
  }
}

// discoverNewProcesses Implementation
void discoverNewProcesses(SchedulerConfig config, Scheduler* scheduler)
{
  // From last process unaccounted for till the end of queue
  for(int i=idxUnaccountedFor; i<scheduler->processCount; i++)
  {
    // Break if the process is outside the time frame in concern
    if(scheduler->arrivalQueue[i]->timeOfArrival > scheduler->clock)
    {
      idxUnaccountedFor = i;
      break;
    }

    // Otherwise add it to the waitingQueue
    else if(scheduler->arrivalQueue[i]->processStatus == 1)
    {
      // Set the processs status to "Waiting"
      scheduler->arrivalQueue[i]->processStatus = 2;
      
      printf("Time %d: %s arrived\n", scheduler->clock, scheduler->arrivalQueue[i]->name);

      // If selected scheduler is fcfs or rr
      if(config.selectedScheduler == 1 || config.selectedScheduler == 3)
      {
        // Push the process into the waitingQueue
        processPush(scheduler->arrivalQueue[i], scheduler);
      }

      // If the selected scheduler is sjf
      else if(config.selectedScheduler == 2)
      {
        processPushByBurstSize(scheduler->arrivalQueue[i], scheduler);
      }

      // Show waitingQueue
      // showQueue(scheduler->waitingQueue);
    }
  }
}

// handleProcessFinish Implementation 
void handleProcessFinish(SchedulerConfig config, Scheduler* scheduler, int* lock)
{
  // If current process burstsRequired are 0
  if(scheduler->processInExecution != NULL && scheduler->processInExecution->burstsRequired == 0)
  {
    printf("Time %d: %s finished\n", scheduler->clock, scheduler->processInExecution->name);

    // Free the process
    Process* finishedProcess = scheduler->processInExecution;
    free(finishedProcess);

    // Set current process to NULL
    scheduler->processInExecution = NULL;

    // Unlock for RR
    *lock = 0;
  }
}


// executeProcess Implementation 
void executeProcess(SchedulerConfig config, Scheduler* scheduler)
{
  // Check if there's no process selected
  if(scheduler->processInExecution == NULL){
    printf("Time %d: IDLE\n", scheduler->clock);
    return;
  }

  // Update the proces status
  scheduler->processInExecution->processStatus = 3;

  // Execute the process
  scheduler->processInExecution->burstsRequired--;

  // Update the currentBurst
  scheduler->currentBurst++;
}

// fcfsSelectProcess Implementation 
void fcfsSelectProcess(SchedulerConfig config, Scheduler* scheduler)
{
  int temp = 0;
  
  // Check if a process is finished and handle that case
  handleProcessFinish(config, scheduler, &temp);
  
  // If current process is not set, dequeue from the waitingQueue
  if(scheduler->processInExecution == NULL)
  {
    scheduler->processInExecution = processPop(&scheduler->waitingQueue);
    if(scheduler->processInExecution != NULL)
      printf("Time %d: %s selected (burst %d)\n", scheduler->clock, scheduler->processInExecution->name, scheduler->processInExecution->burstsRequired);
  }
}

// fcfs Implementation 
void fcfs(SchedulerConfig config, Scheduler* scheduler)
{
  idxUnaccountedFor = 0;
  scheduler->clock = 0;

  // Run for the length of CPU Time specified by config.runFor
  while(scheduler->clock <= config.runFor)
  {
    // Check if any new processes arrived
    discoverNewProcesses(config, scheduler);
    
    // Select the appropriate process
    fcfsSelectProcess(config, scheduler);

    if(scheduler->clock == config.runFor)
      break;

    // Handle process execution
    executeProcess(config, scheduler);
    
    // Increment clock
    scheduler->clock++;
  }
  
  printf("Finished at time %d", scheduler->clock);
}

// rrSelectProcess Implementation
void rrSelectProcess(SchedulerConfig config, Scheduler* scheduler, int* lock)
{
  // Check if a process is finished and handle that case
  handleProcessFinish(config, scheduler, lock);

  // Check if the current process has ran for a quantum's length, then unlock scheduler
  int isQuantumSatisfied = (scheduler->currentBurst % config.quantum) == 0;
  if(isQuantumSatisfied)
    *lock = 0;

  // If there's no current process and scheduler is allowed to switch
  if(scheduler->processInExecution == NULL && !(*lock))
  {
    // Dequeue a process from the waitingQueue and set lock = 1
    scheduler->processInExecution = processPop(&scheduler->waitingQueue);
    (*lock) = 1;

    // If successful, reset current burst
    if(scheduler->processInExecution != NULL)
    {
      printf("Time %d: %s selected (burst %d)\n", scheduler->clock, scheduler->processInExecution->name, scheduler->processInExecution->burstsRequired);
      scheduler->currentBurst = 0;
    }
  }

  // If a process is in execution but scheduler is allowed to select a different process
  else if(scheduler->processInExecution != NULL && !(*lock))
  {
    // Push the current process into the queue
    processPush(scheduler->processInExecution, scheduler);
    scheduler->processInExecution = NULL;

    // Pop a process from the queue and set lock = 1
    scheduler->processInExecution = processPop(&scheduler->waitingQueue);
    *lock = 1;

    // If successful, reset currentBurst
    if(scheduler->processInExecution != NULL)
    {
      printf("Time %d: %s selected (burst %d)\n", scheduler->clock, scheduler->processInExecution->name, scheduler->processInExecution->burstsRequired);
      scheduler->currentBurst = 0;
    }
  }
}

// rr Implementation
void rr(SchedulerConfig config, Scheduler* scheduler)
{
  scheduler->clock = 0;
  int lock = 0;

  while(scheduler->clock < config.runFor)
  {
    // Check if any new processes arrived
    discoverNewProcesses(config, scheduler);

    // Select the appropriate process
    rrSelectProcess(config, scheduler, &lock);

    if(scheduler->clock == config.runFor)
      break;

    // Handle Process Execution
    executeProcess(config, scheduler);
    
    // Increment clock
    scheduler->clock++;
  }

  printf("Finished at time %d", scheduler->clock);
}

// sjfSelectProcess Implementation
void sjfSelectProcess(SchedulerConfig config, Scheduler* scheduler)
{
  int temp = 0;
  
  // Check if a process is finished and handle that case
  handleProcessFinish(config, scheduler, &temp);

  // If there is no current process then dequeue from the waitingQueue
  if(scheduler->processInExecution == NULL)
  {
    scheduler->processInExecution = processPop(&scheduler->waitingQueue);
    
    if(scheduler->processInExecution != NULL)
    {
      printf("Time %d: %s selected (burst %d)\n", scheduler->clock, scheduler->processInExecution->name, scheduler->processInExecution->burstsRequired);
      scheduler->currentBurst = 0;
    }
    
  }

  
  // If there is a current process, look in queue for a better one
  else if(scheduler->processInExecution != NULL && scheduler->waitingQueue != NULL)
  {
    // If the front of queue has a process with smaller burst size
    if(scheduler->waitingQueue->process->burstsRequired < scheduler->processInExecution->burstsRequired)
    {
      // Push current process back to its appropriate spot
      processPushByBurstSize(scheduler->processInExecution, scheduler);
      scheduler->processInExecution = NULL;

      // Dequeue to get the process with shortest burst
      scheduler->processInExecution = processPop(&scheduler->waitingQueue);

      if(scheduler->processInExecution != NULL)
      {
        printf("Time %d: %s selected (burst %d)\n", scheduler->clock, scheduler->processInExecution->name, scheduler->processInExecution->burstsRequired);
        scheduler->currentBurst = 0;
      }
    }
  }
}

// sjf Implementation
void sjf(SchedulerConfig config, Scheduler* scheduler)
{
  scheduler->clock = 0;

  while(scheduler->clock < config.runFor)
  {
    // Check if any new processes arrived
    discoverNewProcesses(config, scheduler);

    // Select the appropriate process
    sjfSelectProcess(config, scheduler);

    if(scheduler->clock == config.runFor)
      break;

    // Handle Process Execution
    executeProcess(config, scheduler);
    
    // Increment clock
    scheduler->clock++;
  }

  printf("Finished at time %d", scheduler->clock);
}