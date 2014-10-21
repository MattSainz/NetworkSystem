/*
 * For Network Systems 4273
 * Matthias Sainz
 */

#include <stddef.h>
#include <algorithm>

class EventScheduler
{

  public:
    EventScheduler(size_t maxEvents);

    /*
    */
    int eventScheduler(size_t maxEvents);

    /*
    */
    void eventCancel(int eventId);

};

