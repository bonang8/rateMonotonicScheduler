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

sem_t Lock1, Lock2, Lock3, Lock4; 
 
    int main(int argc, const char * argv[]) {

    sem_init(&Lock1,0,1);
    sem_init(&Lock2,0,0);
    sem_init(&Lock3,0,0);
    sem_init(&Lock4,0,0);

    pthread_t ptid1, ptid2, ptid3, ptid4, scheduler1;

    if (pthread_create(&ptid1, NULL, T1,NULL) != 0)
            printf("Failed to create thread1\n");

    if (pthread_create(&ptid2, NULL, T2,NULL) != 0)
            printf("Failed to create thread1\n");

    if (pthread_create(&ptid3, NULL, T3,NULL) != 0)
            printf("Failed to create thread1\n");

    if (pthread_create(&ptid4, NULL, T4,NULL) != 0)
            printf("Failed to create thread1\n");

    if (pthread_create(&scheduler1, NULL, Scheduler,NULL) != 0)
            printf("Failed to create thread1\n");

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
  //wake up every time unit and 


  pthread_exit(NULL);
}

void* T1 (void*arg)
{
  sem_wait(&Lock1); 
  cout << "Thread 1 \n";
  sem_post(&Lock2); 
  pthread_exit(NULL);
}

void* T2 (void*arg)
{
  sem_wait(&Lock2); 
  cout << "Thread 2 \n";
  sem_post(&Lock3);   
  pthread_exit(NULL);
}

void* T3 (void*arg)
{
  sem_wait(&Lock3);
  cout << "Thread 3 \n";
  sem_post(&Lock4); 
  pthread_exit(NULL);
}

void* T4 (void*arg)
{
  sem_wait(&Lock4);
  cout << "Thread 4 \n";
  sem_post(&Lock1);
  pthread_exit(NULL);
}


    //join the threads

    //run them 10 times

    //print out how many times they run