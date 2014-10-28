#include <algorithm>
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <thread>
#include "../include/event_scheduler.h"

using namespace std;

pthread_mutex_t EventScheduler::mutexS;
std::unordered_map<int,pthread_t> EventScheduler::events;
size_t EventScheduler::id;

EventScheduler::EventScheduler(size_t maxEvents)
{
  max_evt = (maxEvents > 0) ? maxEvents : DEFAULT_MAX_EVT;
  pthread_mutex_init(&mutexS, NULL);
  id = 0;
}//end Event Scheduler

EventScheduler::~EventScheduler()
{
    while(events.size() > 0)
    {
    }
    //todo clean up other stuffs
}

void EventScheduler::eventCancel(int eventId)
{
  remove_event(eventId);
}//end eventCancel

int EventScheduler::eventSchedule(void (*evFunction)(void *), void *arg, size_t timeout) {
    Event* new_event = new Event;

    //TODO check if there is a better way to init struct
    new_event->to_run  = evFunction;
    new_event->args    = arg;
    new_event->timeout = timeout;
    new_event->id      = id;

    pthread_t new_event_thread;
    pthread_create(&new_event_thread, NULL , event, (void*) new_event);
    add_event(new_event_thread);

    return (int) id;
}

void *EventScheduler::event(void *arg)
{
    Event* to_do = (Event*) arg;

    usleep(to_do->timeout*1000);
    if(!remove_event(to_do->id))  return 0;

    (*(to_do->to_run))(to_do->args);

    return 0;
}

void EventScheduler::add_event(pthread_t new_evt)
{
    pthread_mutex_lock(&mutexS);
    events[id] = new_evt;
    id++;
    pthread_mutex_unlock(&mutexS);
}

bool EventScheduler::remove_event(size_t doomed)
{
    bool to_ret;
    pthread_mutex_lock(&mutexS);
    to_ret = (events.erase( doomed ) > 0);
    pthread_mutex_unlock(&mutexS);
    return to_ret;
}
