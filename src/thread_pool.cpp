#include <assert.h>
#include <iostream>
#include "../include/thread_pool.h"

using namespace std;

/*Static dec */
pthread_mutex_t ThreadPool::mutexS;
pthread_mutex_t ThreadPool::mutexC;
size_t ThreadPool::threadsInUse;
queue<ThreadPool::Job> ThreadPool::jobs;
sem_t ThreadPool::jobsToDo;

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
    numJobsToDo = 0;
    sem_init(&jobsToDo, 0, 0);
    sem_init(&availableWorkers, 0, maxNumThreads);
    initPool();
}//end constructor

ThreadPool::~ThreadPool()
{

}//end destructor

int ThreadPool::dispatch_thread(void dispatch_function(void*), void *arg)
{
    //make job object
    Job* new_job = new Job;

    new_job->to_run = dispatch_function;
    new_job->arg    = arg;

    jobs.push(*new_job);

    sem_post(&jobsToDo);
    //wait for job to finish and get status

    return 0;
}//end dispatch thread

bool ThreadPool::thread_avail()
{
    bool to_ret;
    pthread_mutex_lock(&mutexC);
      to_ret = (threadsInUse == maxNumThreads);
    pthread_mutex_unlock(&mutexC);
    return to_ret;
}//end thread_avail

void* ThreadPool::workPlace(void* param)
{
    int run = 1;
    while(run)
    {
        cout << "Waiting for job \n";

        sem_wait(&jobsToDo);
            //wait for work (think on unlocking updates sem)

        Job my_job = threadPop();
            //get a job need
        pthread_mutex_lock(&mutexC);
            threadsInUse++;
        pthread_mutex_unlock(&mutexC);

        cout << "got job getting read to run \n";
        (*my_job.to_run)(my_job.arg);
            //run job
        cout << "done running job \n";
            //post thread available

        pthread_mutex_lock(&mutexC);
            threadsInUse--;
        pthread_mutex_unlock(&mutexC);
    }

}//end workPlace

void ThreadPool::initPool()
{
    long p_create_status;
    for( size_t i = 0; i < maxNumThreads; i++)
    {
        pthread_t tmp;
        p_create_status = pthread_create(&tmp, NULL , ThreadPool::workPlace, NULL);
        my_threads.push(tmp);
        assert(p_create_status < 0);
    }
}//end initPool

ThreadPool::Job ThreadPool::threadPop()
{
    Job to_ret;
    pthread_mutex_lock(&mutexS);
    sem_post(&jobsToDo);
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