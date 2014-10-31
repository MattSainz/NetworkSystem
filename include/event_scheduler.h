 /*
 * For Network Systems 4273
 * Matthias Sainz
 */

#include <stddef.h>
#include <unordered_map>
#include <algorithm>

 class EventScheduler
{

  typedef struct
  {
    void (*to_run)(void*);
    void* args;
    long timeout;
    size_t id;
  }Event;

  public:
    /*
     * Constructor that creates an EventScheduler object that can
     * schedule up to maxEvents (default value: 10) events.
     */
    EventScheduler(size_t maxEvents);

    /*
     * Standard destructor function
     */
    ~EventScheduler();

    /*
     * Schedules an event to execute the function evFunction
     * with one argument arg after a delay of timeout
     * returns an integer that is used as the event id of the event scheduled
     */
    int eventSchedule(void evFunction(void *), void * arg, size_t timeout);

    /*
     * cancels a previously scheduled event
     */
    void eventCancel(int eventId);

    static pthread_mutex_t mutexS;

  private:
    size_t DEFAULT_MAX_EVT = 10;

    size_t max_evt;

    static size_t id;

    static std::unordered_map<int,pthread_t > events;

    static void* event(void * arg);

    static void add_event(pthread_t ev);

    static bool remove_event(size_t id);

};

