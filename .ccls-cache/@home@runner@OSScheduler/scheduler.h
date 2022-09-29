/* PROCESS */
typedef struct Process{
  /* Name of the process */
  char name[21];

  /* Process length in bursts */
  int totalBurstsRequired;

  /* Bursts remaining till process completion */
  int burstsRequired;

  /* Current status of process:
     1. Arrived - When the process is first discovered
     2. Waiting - When the process is in queue to be executed
     3. Execution - When the process is in execution */
  int processStatus;

  /* Time at which, the process was discovered */
  int timeOfArrival;

} Process;

/*********************************************************************/

/* FINISHEDPROCESSES */
typedef struct FinishedProcess{

  /* Finished procecss */
  Process* process;

  /* Time of process completion */
  int completionTime;

  /* Waiting time for the process. How long from entering the waiting
     queue till its first execution. */
  int wait;

  /* Turn around time for the process. How long from time of arrival
     till process completion. */
  int turnAround;

}FinishedProcess;

/**********************************************************************/

/* PROCESSQUEUE */
typedef struct ProcessQueue{
  /* Process */
  Process* process;

  /* Next on the Queue */
  struct ProcessQueue* next;

} ProcessQueue;

/**********************************************************************/

/* SCHEDULER CONFIG */
typedef struct SchedulerConfig{
  /* How long the processor will run for */
  int runFor;

  /* Which scheduler is processing. Use the following configuration:
     1. fcfs - First Come First Serve
     2. sjf - Shortest Job First
     3. rr - Round Robin */
  int selectedScheduler;

  /* Time Quantum - How long each processing burst will last. Only to 
     be used with RR. */ 
  int quantum;

} SchedulerConfig;

/**********************************************************************/

/* SCHEDULER */
typedef struct Scheduler{

  /* Scheduler's clock */
  int clock;

  /* Current process' continuous burst */
  int currentBurst;

  /* Arrival Queue: Discovered processes sorted in chronological 
     order */
  Process** arrivalQueue;

  /* Number of processes */
  int processCount;

  /* Linked list waiting Queue for processes that are ready */
  ProcessQueue *waitingQueue;

  /* Process currently executing */
  Process* processInExecution;

  /* List of completed processes */
  FinishedProcess** finishedProcesses;

  /* Number of finished processes */
  int numFinishedProcesses;

} Scheduler;

/**********************************************************************/

/* Index up until which the processes have already been accounted for. */
int idxUnaccountedFor;

/* A Process type constructor. Takes a string for name and two int 
   fields burstsRequired and timeOfArrival. */
Process* createProcess(char* processName, int burstsRequired, int timeOfArrival);

/* Quick Sort Algorithm for Processes type. Sorts by timeOfArrival
   attribute if selectedScheduler is FCFS or RR, otherwise, sorts by
   burstsRequired. */
void sortbySchedulerType(SchedulerConfig config, Process** processes, int low, int high);

/* Quick Sort Algorithm for Processes type. Sorts by timeOfArrival
   attribute. */
void sortbyTimeOfArrival(SchedulerConfig config, Process** processes, int low, int high);

/* Auxilary function for sortbySchedulerType. Partitions based on 
   timeOfArrival. */
int partitionByTimeOfArrival(Process **processes, int low, int high);

/* Auxilary function for sortbySchedulerType. Partitions based on 
   burstsRequired. */
int partitionByBurstsRequired(Process **processes, int low, int high);

/* Auxilary function for QuickSort. */
void swap(Process **a, Process **b);

/* Create a ProcessQueueNode and return it. */
ProcessQueue* createQueueNode(Process* process);

/* Process Queue push function. Add a process by order of its burst size */
void processPushByBurstSize(Process* process, Scheduler* scheduler);

/* Process Queue push function. Add a process to the back of queue linked list. */
void processPush(Process* process, Scheduler* scheduler);

/* Process Queue pop function. Returns */
Process* processPop(ProcessQueue** queue);

/* Execute Processes using Scheduler */
void executeProcesses(SchedulerConfig config, Scheduler* scheduler);

/* Add any new discovered process from arrivalQueue to the waitingQueue. */
void discoverNewProcesses(SchedulerConfig config, Scheduler* scheduler);

/* Handle process execution. Selects the appropriate process and executes it. */
void executeProcess(SchedulerConfig config, Scheduler* scheduler);

/* Show the list of processes in waitingQueue. */
void showQueue(ProcessQueue* head);

/* Create a FinishedProcess node and return it */
FinishedProcess* createFinishedProcessNode(Process* process);

/* Function to free a process memory once its finished and set current process to NULL */
void handleProcessFinish(SchedulerConfig config, Scheduler* scheduler, int* lock);

/* First Come First Serve Scheduler. The first process to be discovered occupies the CPU 
   for the length of its execution. */
void fcfs(SchedulerConfig config, Scheduler* scheduler);

/* Selects the process from waitingQueue based on First Come First Served methodology */
void fcfsSelectProcess(SchedulerConfig config, Scheduler* scheduler);

/* Round Robim Scheduler. Each waiting process gets a burst of CPU time through preemption */
void rr(SchedulerConfig config, Scheduler* scheduler);

/* Selects the process from waitingQueue based on Round Robin methodology */
void rrSelectProcess(SchedulerConfig config, Scheduler* scheduler, int* lock);

/* Shortest Job First Scheduler. Preemptively selects the job with shortest burstsRequired */
void sjf(SchedulerConfig config, Scheduler* scheduler);

/* Selects the process from waitingQueue based on Shortest Job First methodology */
void sjfSelectProcess(SchedulerConfig config, Scheduler* scheduler);

/* sortByName Auxilary function. Swaps two finishedProcess nodes */
void swapFinishedProcess(FinishedProcess **a, FinishedProcess **b);

/* QuickSort function to sort FinishedProcess array by name (String) */
void sortByName(FinishedProcess** arr, unsigned int length);

/* Displays all relevant metrics including how many processes failed to be scheduled or did
   not finish executing. Additionally, displays the final wait and turnaround time for 
   completed processes. */
void showFinalMetrics(Scheduler* scheduler);

/* Clean up all allocated memory */
void cleanUp(Scheduler* scheduler);