/*
 * ThradPool libary for Network Systems CSCI 4273
 * Matthias Sainz
 */
#include <pthread.h>
#include <algorithm>
#include <fcntl.h>
#include <queue>
#include <semaphore.h>

class ThreadPool
{
    typedef struct
    {
        void (*to_run)(void*);
        void *arg;
        int id;
    }Job;

    public:
        /*
        * Creates a ThreadPool obecject consisting
        * a set of threadCounts threads (Default value 10)
        */
        ThreadPool(size_t threadCount);

        /*
        * Cleans up
        */
        ~ThreadPool();

        /*
        * Dispatches a thread from the pool to execute the
        * function that is passed in. After execution the
        * thread returns back to the pool
        */
        int dispatch_thread(void dispatch_function(void *), void *arg);

        /*
        * Notifies the caller if there is a thread amiable
        * to service the dispatch function
        */
        bool thread_avail();

        static pthread_mutex_t mutexS;

        static pthread_mutex_t mutexC;

        /*
         * the method the thread will execute to run the job that was given to
         * it from the user calling the dispatch_thread function
         */

    private:
        size_t DEFAULT_THREAD_COUNT = 10;


        static sem_t jobsToDo;
            // lets the threads wait and then work on jobs
        static sem_t availableWorkers;
            //lets anyone asking know how many free workers there are

        static size_t threadsInUse;

        size_t maxNumThreads;
            //the number of threads aviable to the application

        static std::queue<Job> jobs;
            //stores all the jobs to be done

        std::queue<pthread_t> my_threads;
            //stores all of the p threads (may not be needed)

        void initPool();

        static Job threadPop();

        static void threadPush(Job& new_job);

        static void* workPlace(void *param);

        static bool keepRunning;
};

