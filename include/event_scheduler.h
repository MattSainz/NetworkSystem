/*
 * For Network Systems 4273
 * Matthias Sainz
 */

#include <stddef.h>
#include <algorithm>

class EventScheduler
{

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
    int eventScheduler(void evFunction(void *), void * arg, size_t timeout);

    /*
     * cancels a previously scheduled event
     */
    void eventCancel(int eventId);

};

