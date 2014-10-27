#include <algorithm>
#include "../include/event_scheduler.h"

using namespace std;

EventScheduler::EventScheduler(size_t maxEvents)
{

}//end Event Scheduler

void EventScheduler::eventCancel(int eventId)
{

}//end eventCancel

int EventScheduler::eventScheduler(void (*evFunction)(void *), void *arg, size_t timeout) {
    return 0;
}
