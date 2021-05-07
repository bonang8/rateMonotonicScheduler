#define MACOS // enables macOS thread processor affinity // delete this
#include <thread>
#include <iostream>
#include <chrono>
#include <unistd.h> // defines usleep
#include <semaphore.h> // have to use named semaphores on macOS
//#define LINUX // uncomment this when running on linux
using std::chrono::milliseconds;
using namespace std;
void* T1 (void*arg);
void* T2 (void*arg);
void* T3 (void*arg);
void* T4 (void*arg);
void* Scheduler (void*arg);
void doWork();
int countIterations1 = 0;
int countIterations2 = 0;
int countIterations3 = 0;
int countIterations4 = 0;

pthread_mutex_t Lock1, Lock2, Lock3, Lock4;

sem_t * semaphore_1 = NULL; // protects the shared resource
sem_t * semaphore_2 = NULL; // may have to do with toggling back and forth
sem_t * semaphore_3 = NULL; // protects the shared resource
sem_t * semaphore_4 = NULL; // may have to do with toggling back and forth

pthread_attr_t thread_scheduler_attribute;

// limits for the priority
// need to assign the scheduluer thread max priority
// thread 1 should get priority max - 1
// all priorities must be greater than the min priority
int fifo_max_priority = sched_get_priority_max(SCHED_FIFO);
int fifo_min_priority = sched_get_priority_min(SCHED_FIFO);
//
bool run = true;

    int main(int argc, const char * argv[]) {
    // cpu id:: identifies the cup that you are going to run
    int cpuID = 0;
    
    // Set all to 0, because scheduler will go first and set them to 1
    semaphore_1 = sem_open("/semaphore_1",O_CREAT, S_IRUSR | S_IWUSR, 0);
    semaphore_2 = sem_open("/semaphore_2",O_CREAT, S_IRUSR | S_IWUSR, 0);
    semaphore_3 = sem_open("/semaphore_3",O_CREAT, S_IRUSR | S_IWUSR, 0);
    semaphore_4 = sem_open("/semaphore_4",O_CREAT, S_IRUSR | S_IWUSR, 0);
    
   // Thread 1
        pthread_t ptid1, ptid2, ptid3, ptid4, scheduler1;
        // The code below ensures that the threads each have a priority assigned to them
        // and uses a FIFO scheduler
        pthread_attr_t thread_scheduler_attribute1;
        pthread_attr_init(&thread_scheduler_attribute1);
        pthread_attr_setschedpolicy(&thread_scheduler_attribute1, SCHED_FIFO);
        // sets the first in first out parameters
        struct sched_param fifo_parameter1;
        fifo_parameter1.sched_priority = fifo_max_priority - 1;
        // sets all the attributes
        pthread_attr_setschedparam(&thread_scheduler_attribute1, &fifo_parameter1);
        // schedule all the threads on a per process basis rather than system wide
        pthread_attr_setscope(&thread_scheduler_attribute1, PTHREAD_SCOPE_PROCESS);
#if defined(LINUX)
        // Linux-specific thread-affinity code; use #define LINUX to enable this block of code.
           // declares a set of processors that the threads can run on;
           // only processor zero will be in the set
           cpu_set_t cpu_set;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attribute1, sizeof(cpu_set_t), &cpu_set);
#endif
        
    if (pthread_create(&ptid1, &thread_scheduler_attribute1, T1,NULL) != 0)
            printf("Failed to create thread1\n");
        
        
    // Thread 2
        pthread_attr_t thread_scheduler_attribute2;
        pthread_attr_init(&thread_scheduler_attribute2);
        pthread_attr_setschedpolicy(&thread_scheduler_attribute2, SCHED_FIFO);
        struct sched_param fifo_parameter2;
        fifo_parameter2.sched_priority = fifo_max_priority - 2;
        // sets all the attributes
        pthread_attr_setschedparam(&thread_scheduler_attribute2, &fifo_parameter2);
        // schedule all the threads on a per process basis rather than system wide
        pthread_attr_setscope(&thread_scheduler_attribute2, PTHREAD_SCOPE_PROCESS);
     // Linux
#if defined(LINUX)
        // Linux-specific thread-affinity code; use #define LINUX to enable this block of code.
           // declares a set of processors that the threads can run on;
           // only processor zero will be in the set
           cpu_set_t cpu_set;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attribute2, sizeof(cpu_set_t), &cpu_set);
#endif
    if (pthread_create(&ptid2, &thread_scheduler_attribute2, T2,NULL) != 0)
            printf("Failed to create thread1\n");
        
        
        
    // Thread 3
        pthread_attr_t thread_scheduler_attribute3;
        pthread_attr_init(&thread_scheduler_attribute3);
        pthread_attr_setschedpolicy(&thread_scheduler_attribute3, SCHED_FIFO);
        struct sched_param fifo_parameter3;
        fifo_parameter3.sched_priority = fifo_max_priority - 3;
        // sets all the attributes
        pthread_attr_setschedparam(&thread_scheduler_attribute3, &fifo_parameter3);
        // schedule all the threads on a per process basis rather than system wide
        pthread_attr_setscope(&thread_scheduler_attribute3, PTHREAD_SCOPE_PROCESS);
     // Linux
#if defined(LINUX)
        // Linux-specific thread-affinity code; use #define LINUX to enable this block of code.
           // declares a set of processors that the threads can run on;
           // only processor zero will be in the set
           cpu_set_t cpu_set;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attribute3, sizeof(cpu_set_t), &cpu_set);
#endif
    if (pthread_create(&ptid3, &thread_scheduler_attribute3, T3,NULL) != 0)
            printf("Failed to create thread1\n");
        
        
    // Thread 4
        pthread_attr_t thread_scheduler_attribute4;
        pthread_attr_init(&thread_scheduler_attribute4);
        pthread_attr_setschedpolicy(&thread_scheduler_attribute4, SCHED_FIFO);
        struct sched_param fifo_parameter4;
        fifo_parameter4.sched_priority = fifo_max_priority - 4;
        // sets all the attributes
        pthread_attr_setschedparam(&thread_scheduler_attribute4, &fifo_parameter4);
        // schedule all the threads on a per process basis rather than system wide
        pthread_attr_setscope(&thread_scheduler_attribute4, PTHREAD_SCOPE_PROCESS);
    // Linux
#if defined(LINUX)
        // Linux-specific thread-affinity code; use #define LINUX to enable this block of code.
           // declares a set of processors that the threads can run on;
           // only processor zero will be in the set
           cpu_set_t cpu_set;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attribute4, sizeof(cpu_set_t), &cpu_set);
#endif
    if (pthread_create(&ptid4, &thread_scheduler_attribute4, T4,NULL) != 0)
            printf("Failed to create thread1\n");
        
        
    // Thread Scheduler
        pthread_attr_t thread_scheduler_attributeS;
        pthread_attr_init(&thread_scheduler_attributeS);
        pthread_attr_setschedpolicy(&thread_scheduler_attributeS, SCHED_FIFO);
        struct sched_param fifo_parameterS;
        // give scheduler max priority
        fifo_parameterS.sched_priority = fifo_max_priority;
        // sets all the attributes
        pthread_attr_setschedparam(&thread_scheduler_attributeS, &fifo_parameterS);
        // schedule all the threads on a per process basis rather than system wide
        pthread_attr_setscope(&thread_scheduler_attributeS, PTHREAD_SCOPE_PROCESS);
    //  Linux
#if defined(LINUX)
        // Linux-specific thread-affinity code; use #define LINUX to enable this block of code.
           // declares a set of processors that the threads can run on;
           // only processor zero will be in the set
           cpu_set_t cpu_set;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attributeS, sizeof(cpu_set_t), &cpu_set);
#endif
    if (pthread_create(&scheduler1, &thread_scheduler_attributeS, Scheduler,NULL) != 0)
            printf("Failed to create thread1\n");
        
    // join threads
    pthread_join(ptid1,NULL);
    pthread_join(ptid2,NULL);
    pthread_join(ptid3,NULL);
    pthread_join(ptid4,NULL);
    pthread_join(scheduler1,NULL);
    cout << "T3 Iterations: " << countIterations3 << endl << flush;
        usleep(100000);
   // pthread_exit(NULL);
        cout << "exiting main" << endl << flush;
    return 0;
}

// Thread function
void* Scheduler (void*arg)
{
    const int maxIterations = 10;
    const int numTimeUnits = 16;
    const int timePeriodInMilliseconds = 100;
  for(int i = 0; i < maxIterations; i++)
   {
       for(int j = 0; j < numTimeUnits; j++)
       {
           // thread T1 has to run every time period
           sem_post(semaphore_1);
           // thread T2 has to run every 2 time periods
           if((j % 2) == 0){
            sem_post(semaphore_2);
           }
           // thread T3 has to run every 4 time periods
           if((j % 4) == 0){
            sem_post(semaphore_3);
           }
           // thread T4 has to run every 16 time periods
           if((j % 16) == 0){
            sem_post(semaphore_4);
           }
           usleep(timePeriodInMilliseconds*1000);
       }
  }
  run = false; // believe assignment of bool is atomic (otherwise, need mutex)
    cout << "exiting scheduler " << endl << flush;
    cout << "T1 Iterations: " << countIterations1 << endl << flush;
    cout << "T2 Iterations: " << countIterations2 << endl << flush;
    cout << "T3 Iterations: " << countIterations3 << endl << flush;
    cout << "T4 Iterations: " << countIterations4 << endl << flush;
  pthread_exit(NULL);
}

void* T1 (void*arg)
{
    while(run){
        sem_wait(semaphore_1);
        // TODO: sem_wait and wait on the semaphore id
        cout << "Thread 1 \n";
        countIterations1++;
    }
    pthread_exit(NULL);
}

void* T2 (void*arg)
{
    while(run){
      sem_wait(semaphore_2);
      cout << "Thread 2 \n";
      countIterations2++;
    }
  pthread_exit(NULL);
}

void* T3 (void*arg)
{
    while(run){
      sem_wait(semaphore_3);
      cout << "Thread 3 \n";
      countIterations3++;
    }
  pthread_exit(NULL);
}

void* T4 (void*arg)
{
    while(run){
      sem_wait(semaphore_4);
      cout << "Thread 4 \n";
        countIterations4++;
    }
  pthread_exit(NULL);
}

void doWork()
{
  int matrix[10][10];
  //randomly generate a matrix to "do work" on
  for(int i = 0;i < 10;++i)
  {
    for(int j = 0;j < 10;++j)
    {
      matrix[i][j] = rand() % 10 + 1;
    }
  }

  int startingValue1, startingValue2;
  //iterate through and set the values
  for(int i = 0; i < 10; ++i)
  {
    startingValue1 = 0;
    startingValue2 = 5;
    while(startingValue1 < 5)
    {
      matrix[i][startingValue1] *= matrix[i][startingValue2];
      matrix[i][startingValue2] = matrix[i][startingValue1];
      startingValue1++;
      startingValue2++;
    }
  }
}

