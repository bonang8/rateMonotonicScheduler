# rateMonotonicScheduler


Name:
Project_04: Rate Monotonic Scheduler <br> 
Team Members: Clarisse Bonang, Scott Cummings, Lisa Pham <br> 
Date: May 10, 2021 <br> 
Source Code <br> 
Submitted via RateMonotonic.zip on Canvas <br> 
# Report
# Contributions
Clarisse: Linux API and processor affinity, setting priority on the threads, thread synchronization <br> 
with semaphores, checking for overruns, other thread attributes, terminating the threads using <br> 
run boolean, final semaphore signals to prevent deadlock during termination, set up Oracle <br> 
virtualBox and Fedora Linux system. <br> 
Lisa: Linux API and processor affinity, doWork implementation in threads.<br> 
Scott: Linux API, Thread synchronization with semaphores, checking for overruns, doWork <br> 
implementation.<br> 
# Synchronization
The Scheduler function is awakened every 100 ms by a linux-based timer and signal <br> 
handler function. It handles synchronization of the threads by use of semaphores that force only <br> 
one thread to be running at a time. Priorities are set on the threads so that threads with shorter <br> 
periods preempt threads with longer periods. <br> 
# Threads
Each thread (T1, T2, T3, T4) is given a priority respective to their time period. The thread <br> 
with the shortest time period has the highest priority (T1). To ensure that the threads are <br> 
dispatched properly, the scheduler has the highest priority overall and schedules each thread. In <br> 
the scheduler, the threads must complete their tasks when signaled within their time period and <br> 
the program’s major frame period. For instance, the Scheduler() signals thread 2’s semaphore <br> 
every 2 time periods within the major frame period.<br> 
Scheduler: Scheduled the execution of threads T1, T2, T3, and T4, based on their <br> 
respective time periods of 1, 2, 4, and 16. This was achieved by using for loops, semaphores, <br> 
and mods. <br> The Scheduler was also responsible for tracking the number of overruns for each
thread.<br> 
T1: Thread 1 was responsible for running doWork() 100 times. <br> 
T2: Thread 2 was responsible for running doWork() 200 times. <br> 
T3: Thread 3 was responsible for running doWork() 400 times. <br> 
T4: Thread 4 was responsible for running doWork() 1600 times.<br> 
# Scheduling
Each thread is signaled based on the current clock tick by modular arithmetic. First we <br> 
check a boolean value to determine if the thread is working. If the thread is working, that means <br> 
it missed its deadline and we increment a value that we print out at the end. We also reschedule <br> 
the thread regardless of whether or not an overrun has occurred.<br> 
In the case that a thread fails to execute, such as in T2, T3, and T4, the program simply <br> 
increments the respective overrun counter and reschedules the thread as normal. As our <br> 
overruns are handled within our Scheduler in if blocks sans the “else”, there is no need for any <br> 
external helper functions to reschedule the thread, rather the related semaphore is posted as <br> 
usual and everything continues running. <br> 
# Running the program <br> 
To compile: g++ rateMonotonicScheduler.cpp -pthread -o RMS -lrt <br> 
To run: ./RMS <br> 
