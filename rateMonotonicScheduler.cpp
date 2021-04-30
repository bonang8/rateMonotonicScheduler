#include <thread>
#include <iostream>
#include <chrono>
#include <semaphore.h> // have to use named semaphores on macOS
#include <unistd.h> // sleep

using std::chrono::milliseconds;
using namespace std;

void* T1 (void*arg);
void* T2 (void*arg);
void* T3 (void*arg);
void* T4 (void*arg);
void* Scheduler (void*arg);
void doWork();
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
    if (pthread_create(&scheduler1, &thread_scheduler_attributeS, Scheduler,NULL) != 0)
            printf("Failed to create thread1\n");
    // join threads
    pthread_join(ptid1,NULL);
    pthread_join(ptid2,NULL);
    pthread_join(ptid3,NULL);
    pthread_join(ptid4,NULL);
    pthread_join(scheduler1,NULL);
    pthread_exit(NULL);

    return 0;
}

// Thread function
void* Scheduler (void*arg)
{
    // Diag:
    cout << "Herrrro" << endl;
  int count = 0;
  while(clock()%10000 != 0)
  {
      cout << "In scheduluer while loop" << endl;
    if(clock()%50 == 0)
    {
      cout << "T1 gets called. \n";
      cout << "Signaling Thread1 " << endl;
      sem_post(semaphore_1);
    }
    if(clock()%100 == 0)
    {
      cout << "T2 gets called. \n";
      cout << "Signaling Thread2 " << endl;
        sem_post(semaphore_2);
    }
    if(clock()%200 == 0)
    {
      cout << "T3 gets called. \n";
      cout << "Signaling Thread3 " << endl;
        sem_post(semaphore_3);
    }
    if(clock()%800 == 0)
    {
      cout << "T4 gets called. \n";
      cout << "Signaling Thread4 " << endl;
        sem_post(semaphore_4);
    }
    count++;
      // changed # from 10 to 1000 for now
    if(count > 1000)
    {
        run = false;
        break;
    }
  }
  pthread_exit(NULL);
}

void* T1 (void*arg)
{
    cout << "In thread1" << endl;
    //pthread_cond_wait(&cond1, &Lock1);
    while(run){
        sem_wait(semaphore_1);
        // TODO: sem_wait and wait on the semaphore id
        cout << "Thread 1 \n";
        cout << "Are we changing anything? \n";
    }
    pthread_exit(NULL);
}

void* T2 (void*arg)
{
    cout << "In thread2" << endl;
  //pthread_cond_wait(&cond2, &Lock2);
    while(run){
      sem_wait(semaphore_2);
      cout << "Thread 2 \n";
      //pthread_mutex_unlock(&Lock3);
    }
  pthread_exit(NULL);
}

void* T3 (void*arg)
{
    cout << "In thread3" << endl;
  //pthread_cond_wait(&cond3, &Lock3);
    while(run){
      sem_wait(semaphore_3);
      cout << "Thread 3 \n";
      //pthread_mutex_unlock(&Lock4);
    }
  pthread_exit(NULL);
}

void* T4 (void*arg)
{
    cout << "In thread4" << endl;
  //pthread_cond_wait(&cond4, &Lock4);
    while(run){
      sem_wait(semaphore_4);
      cout << "Thread 4 \n";
      //pthread_mutex_unlock(&Lock1);
    }
  pthread_exit(NULL);
}

