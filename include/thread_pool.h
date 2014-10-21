/*
 * ThradPool libary for Network Systems CSCI 4273
 * Matthias Sainz
 */
#include <pthread.h>
#include <algorithm>

class ThreadPool
{
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
        * Notifies the caller if there is a thread aviable
        * to service the dispatch function
        */
        bool thread_avail();
};

