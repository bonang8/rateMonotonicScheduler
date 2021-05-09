/* 
g++ rateMonotonicScheduler.cpp -pthread -o RMS -lrt
./RMS
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <unistd.h> // defines usleep
#include <semaphore.h> // have to use named semaphores on macOS
#define LINUX // uncomment this when running on linux
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
int doWorkT1 = 100;
int doWorkT2 = 200;
int doWorkT3 = 400;
int doWorkT4 = 1600;
int dWT1 = 0;
int dWT2 = 0;
int dWT3 = 0;
int dWT4 = 0;
bool doingWork1 = false;
bool doingWork2 = false;
bool doingWork3 = false; 
bool doingWork4 = false; 
int overrun1 = 0; 
int overrun2 = 0; 
int overrun3 = 0; 
int overrun4 = 0; 

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
    // cpu id:: identifies the cpu that you are going to run
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
           cpu_set_t cpu_set1;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set1);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set1);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attribute1, sizeof(cpu_set_t), &cpu_set1);
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
           cpu_set_t cpu_set2;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set2);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set2);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attribute2, sizeof(cpu_set_t), &cpu_set2);
#endif
    if (pthread_create(&ptid2, &thread_scheduler_attribute2, T2,NULL) != 0)
            printf("Failed to create thread2\n");



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
           cpu_set_t cpu_set3;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set3);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set3);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attribute3, sizeof(cpu_set_t), &cpu_set3);
#endif
    if (pthread_create(&ptid3, &thread_scheduler_attribute3, T3,NULL) != 0)
            printf("Failed to create thread3\n");


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
           cpu_set_t cpu_set4;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set4);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set4);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attribute4, sizeof(cpu_set_t), &cpu_set4);
#endif
    if (pthread_create(&ptid4, &thread_scheduler_attribute4, T4,NULL) != 0)
            printf("Failed to create thread4\n");


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
           cpu_set_t cpu_set5;
           // sets all processors to zero; no processors in the set currently
           CPU_ZERO(&cpu_set5);
           // adds processor zero to set (cpuID = 0)
           CPU_SET(cpuID, &cpu_set5);
           // adds the cpu set to the threads attributes
           pthread_attr_setaffinity_np(&thread_scheduler_attributeS, sizeof(cpu_set_t), &cpu_set5);
#endif
    if (pthread_create(&scheduler1, &thread_scheduler_attributeS, Scheduler,NULL) != 0)
            printf("Failed to create threadS\n");

    // join threads
    pthread_join(ptid1,NULL);
    pthread_join(ptid2,NULL);
    pthread_join(ptid3,NULL);
    pthread_join(ptid4,NULL);
    pthread_join(scheduler1,NULL);
    cout << "DEBUG\n";
   //     usleep(100000);
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
           if(doingWork1 == true) //we overran our time
           {
            overrun1++;
           } 
           sem_post(semaphore_1);
           // thread T2 has to run every 2 time periods
           if((j % 2) == 0){
             if(doingWork2 == true) //we overran our time
              {
                overrun2++;
              } 
            sem_post(semaphore_2);
           }
           // thread T3 has to run every 4 time periods
           if((j % 4) == 0){
             if(doingWork3 == true) //we overran our time
            {
              overrun3++;
            } 
            sem_post(semaphore_3);
           }
           // thread T4 has to run every 16 time periods
           if((j % 16) == 0){
             if(doingWork4 == true) //we overran our time
            {
              overrun4++;
            } 
            sem_post(semaphore_4);
           }
           usleep(timePeriodInMilliseconds*1000);
       }
  }
  cout << "T1 missed it's deadline " << overrun1 << " times\n";
  cout << "T2 missed it's deadline " << overrun2 << " times\n";
  cout << "T3 missed it's deadline " << overrun3 << " times\n";
  cout << "T4 missed it's deadline " << overrun4 << " times\n";

  cout << "doWork ran: " << dWT1 << " times for T1." << endl;
  cout << "doWork ran: " << dWT2 << " times for T2." << endl;
  cout << "doWork ran: " << dWT3 << " times for T3." << endl;
  cout << "doWork ran: " << dWT4 << " times for T4." << endl;

  run = false; // believe assignment of bool is atomic (otherwise, need mutex)
    cout << "T1 Iterations: " << countIterations1 << endl << flush;
    cout << "T2 Iterations: " << countIterations2 << endl << flush;
    cout << "T3 Iterations: " << countIterations3 << endl << flush;
    cout << "T4 Iterations: " << countIterations4 << endl << flush;

    // signal semaphores again to avoid deadlock
    sem_post(semaphore_1);
    sem_post(semaphore_2);
    sem_post(semaphore_3);
    sem_post(semaphore_4);
  pthread_exit(NULL);
}

void* T1 (void*arg)
{
    while(run){
        sem_wait(semaphore_1);
        // boolean set to true
        doingWork1 = true;
        // TODO: sem_wait and wait on the semaphore id
        cout << "Thread 1 \n";
        if(run){
            for(int i = 0; i < doWorkT1; i++){
              doWork();
              dWT1++;
            }
            countIterations1++;
        }
        // boolean set to false
    }
    pthread_exit(NULL);
}

void* T2 (void*arg)
{
    while(run){
      sem_wait(semaphore_2);
      doingWork2 = true;
      cout << "Thread 2 \n";
        if(run){
          for(int i = 0; i < doWorkT2; i++){
            doWork();
            dWT2++;
          }
          countIterations2++;
        }
    }

  pthread_exit(NULL);
}
void* T3 (void*arg)
{
    while(run){
      sem_wait(semaphore_3);
      doingWork3 = true;
      cout << "Thread 3 \n";
        if(run){
          for(int i = 0; i < doWorkT3; i++){
            doWork();
            dWT3++;
          }
          countIterations3++;
        }
    }

  pthread_exit(NULL);
}

void* T4 (void*arg)
{
    while(run){
      sem_wait(semaphore_4);
      doingWork4 = true; 
      cout << "Thread 4 \n";
      for(int i = 0; i < doWorkT4; i++){
        doWork();
        dWT4++;
      }
      countIterations4++;
      doingWork4 = false; 
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
