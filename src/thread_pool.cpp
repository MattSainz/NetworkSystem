#include <assert.h>
#include <iostream>
#include <unistd.h>
#include "../include/thread_pool.h"

using namespace std;

/*Static dec */
pthread_mutex_t ThreadPool::mutexS;
pthread_mutex_t ThreadPool::mutexC;
size_t ThreadPool::threadsInUse;
queue<ThreadPool::Job> ThreadPool::jobs;
sem_t ThreadPool::jobsToDo;
sem_t ThreadPool::availableWorkers;

bool ThreadPool::keepRunning;

ThreadPool::ThreadPool(size_t threadCount)
{
    if(threadCount < 1)
    {
       maxNumThreads = DEFAULT_THREAD_COUNT;
    }
    else
    {
       maxNumThreads = threadCount;
    }
    mutexS = PTHREAD_MUTEX_INITIALIZER;
    mutexC = PTHREAD_MUTEX_INITIALIZER;
    sem_init(&jobsToDo, 0, 0);
    sem_init(&availableWorkers, 0, maxNumThreads);
    keepRunning= true;
    initPool();
}//end constructor

ThreadPool::~ThreadPool()
{
    keepRunning = false;
    /*
    figure out how to kill any left overs nicely
    for(int i = 0; i < maxNumThreads; i++)
    {
        sem_post(&jobsToDo);
            //wake up all of the threads so they can stop running
    }
    */
    sem_destroy(&jobsToDo);
    sem_destroy(&availableWorkers);
    pthread_mutex_destroy(&mutexC);
    pthread_mutex_destroy(&mutexS);

}//end destructor

int ThreadPool::dispatch_thread(void dispatch_function(void*), void *arg)
{
    //make job object
    Job* new_job = new Job;

    new_job->to_run = dispatch_function;
    new_job->arg    = arg;

    jobs.push(*new_job);
        //add a job to the queue

    sem_post(&jobsToDo);
        //let it be know work needs to be done

    sem_wait(&availableWorkers);
        //forces calling program to wait for a worker to start the task
        //does not wait for task to end

    return 0;
}//end dispatch thread

bool ThreadPool::thread_avail()
{
    bool to_ret;
    pthread_mutex_lock(&mutexC);
      to_ret = threadsInUse != maxNumThreads;
    pthread_mutex_unlock(&mutexC);
    return to_ret;
}//end thread_avail

void* ThreadPool::workPlace(void* param)
{
    while(keepRunning)
    {

        sem_wait(&jobsToDo);
            //wait for work (think on unlocking updates sem)

        Job my_job = threadPop();
            //get a job need

        (*my_job.to_run)(my_job.arg);
            //run job

        pthread_mutex_lock(&mutexC);
            threadsInUse--;
        pthread_mutex_unlock(&mutexC);

        sem_post(&availableWorkers);
            //there is a worker who is free
    }
    return 0;
}//end workPlace

void ThreadPool::initPool()
{
    long p_create_status;
    for( size_t i = 0; i < maxNumThreads; i++)
    {
        pthread_t tmp;
        p_create_status = pthread_create(&tmp, NULL , ThreadPool::workPlace, (void*) i);
        my_threads.push(tmp);
        //assert(p_create_status > 0);
    }
}//end initPool

ThreadPool::Job ThreadPool::threadPop()
{
    Job to_ret;
    pthread_mutex_lock(&mutexS);

    threadsInUse++;
    to_ret = jobs.front();
    jobs.pop();

    pthread_mutex_unlock(&mutexS);

    return to_ret;
}//end threadPop

void ThreadPool::threadPush(Job& new_job)
{
    pthread_mutex_lock(&mutexS);
    jobs.push(new_job);
    pthread_mutex_unlock(&mutexS);
}//end threadPush