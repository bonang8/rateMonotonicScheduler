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

pthread_mutex_t Lock1, Lock2, Lock3, Lock4; 

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond4 = PTHREAD_COND_INITIALIZER;
 
    int main(int argc, const char * argv[]) {


    pthread_mutex_init(&Lock1, NULL);
    pthread_mutex_init(&Lock2, NULL);
    pthread_mutex_init(&Lock3, NULL);
    pthread_mutex_init(&Lock4, NULL);

    pthread_mutex_lock(&Lock2); 
    pthread_mutex_lock(&Lock3); 
    pthread_mutex_lock(&Lock4); 
    

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

    //pthread_join(scheduler1,NULL);
    pthread_exit(NULL);

    return 0;
}

// Thread function
void* Scheduler (void*arg)
{
  while(clock()%10000 != 0)
  {
    if(clock()%50 == 0)
    {
      cout << "T1 gets called. \n";
      pthread_cond_signal(&cond1); 
    }
    if(clock()%100 == 0)
    {
      cout << "T2 gets called. \n";
      pthread_cond_signal(&cond2); 
    }
    if(clock()%200 == 0)
    {
      cout << "T3 gets called. \n";
      pthread_cond_signal(&cond3); 
    }
    if(clock()%800 == 0)
    {
      cout << "T4 gets called. \n";
      pthread_cond_signal(&cond4); 
    }
  }
  pthread_exit(NULL);
}

void* T1 (void*arg)
{
    pthread_cond_wait(&cond1, &Lock1); 
    cout << "Thread 1 \n";
    cout << "Are we changing anything? \n";
    pthread_mutex_unlock(&Lock2); 
    pthread_exit(NULL);
}

void* T2 (void*arg)
{
  pthread_cond_wait(&cond2, &Lock2); 
  cout << "Thread 2 \n";
  pthread_mutex_unlock(&Lock3);   
  pthread_exit(NULL);
}

void* T3 (void*arg)
{
  pthread_cond_wait(&cond3, &Lock3); 
  cout << "Thread 3 \n";
  pthread_mutex_unlock(&Lock4); 
  pthread_exit(NULL);
}

void* T4 (void*arg)
{
  pthread_cond_wait(&cond4, &Lock4); 
  cout << "Thread 4 \n";
  pthread_mutex_unlock(&Lock1);
  pthread_exit(NULL);
}


    //join the threads

    //run them 10 times

    //print out how many times they run
