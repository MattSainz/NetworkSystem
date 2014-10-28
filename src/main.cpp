#define CATCH_CONFIG_MAIN

#include "../test/catch.h"
#include "../include/event_scheduler.h"
#include "../include/message_manager.h"
#include "../include/thread_pool.h"
using namespace std;

int thread_val = 0;

void test_fn(void *par)
{
   // cout << "entering test_fn " << *(int *)par << endl;
    //sleep((unsigned int) (rand() % 3));
        //simulate alot of work
   // cout << "exiting test_fn " << *(int *)par << endl;
    thread_val += *(int *) par;
}

void test_fn1(void *par)
{
    cout << "in test_fn1 " << *(int *)par << endl;
}

void test_fn2(void *par)
{
    cout << "in test_fn2 " << *(int *)par << endl;
}

void test_fn3(void *par)
{
    cout << "in test_fn3 " << *(int *)par << endl;
}

TEST_CASE("Insanity Check", "[hello world]")
{
   REQUIRE( true );
}

TEST_CASE("Message constructor test", "[Constructor]")
{
    int len = 4;
    Message* test = new Message("test", len);
    char* buff = new char[test->msgLen()];
    test->msgFlat(buff);
    bool all_good = strcmp(buff, "test") == 0;
    REQUIRE(all_good);
}

TEST_CASE("Add Header", "[msgAddHdr]")
{
    int len = 4;
    Message* test = new Message("test", len);
    test->msgAddHdr("foo", 3);

    char* buff = new char[len + 3];
    test->msgFlat(buff);
    bool all_good = strcmp(buff, "footest") == 0;
    REQUIRE(all_good);
}

TEST_CASE("Strip Header","[msgStripHdr]")
{
    int len = 4;
    Message* test = new Message("test", len);
    test->msgAddHdr("foo", 3);

    char* buff = test->msgStripHdr(3);

    bool all_good = strcmp(buff, "foo") == 0;

    REQUIRE(all_good);
}

/*
TEST_CASE("Add large header", "[msgAddHdr]")
{
    char test_msg[259];
    memset(test_msg, 'a', 259);
    Message* test = new Message(test_msg,256);
    test->msgAddHdr("foo", 3);
    test_msg[0] = 'f';
    test_msg[1] = 'o';
    test_msg[2] = 'o';
    //test_msg[259] = '\0';
    char* buff; //= new char[25];
    test->msgFlat(buff);
    bool all_good = strcmp(buff, test_msg) == 0;
    //REQUIRE(all_good);
}
*/

TEST_CASE("Split","[msgSplit]")
{
    int len = 7;
    Message* test = new Message("testfoo", len);
    char* t2 = "aaa";
    test->msgAddHdr(t2, 3);
    Message* split = new Message();
    test->msgSplit(*split, 7);
    char* buff = new char[split->msgLen()];
    char* buf2 = new char[split->msgLen()];
    split->msgFlat(buff);
    bool all_good = strcmp(buff, "testfoo") == 0;
    test->msgFlat(buf2);
    all_good = strcmp(buf2, "aaa") == 0;
    REQUIRE(all_good);
}

TEST_CASE("Join","[msgJoin]")
{
    int len = 7;
    Message* test = new Message("testfoo", len);
    char* t2 = "aaa";
    test->msgAddHdr(t2, 3);
    Message* split = new Message();
    test->msgSplit(*split, 7);
    char* buff = new char[split->msgLen()];
    char* buf2 = new char[split->msgLen()];
    split->msgFlat(buff);
    test->msgFlat(buf2);

    test->msgJoin(*split);
    buff = new char[test->msgLen()];
    test->msgFlat(buff);
    REQUIRE(strcmp(buff, "aaatestfoo") == 0);
}

TEST_CASE("Threads","[threadPool]")
{
    ThreadPool th(3);
    int max[20];
    int check = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            max[(i*4)+j] = 100 * ((i * 4) + j);
            if (th.thread_avail()) {
                check += max[(i * 4) + j];
                th.dispatch_thread(test_fn, (void *) &(max[(i * 4) + j]));
            }
            else
            {
                cout << "No thread is avalable for " << max[(i*4)+j] << endl;
            }
        }
        sleep(2);
    }
    REQUIRE( (abs(thread_val-check) == 0) );
}

TEST_CASE("Event schedgule","[one event]")
{
   EventScheduler my_sch(5);
   thread_val = 0;
   int max[0];
   max[0] = 123999123;
   my_sch.eventSchedule(test_fn,(void*) &(max[0]), 1);
   my_sch.eventSchedule(test_fn,(void*) &(max[0]), 2);
   my_sch.eventSchedule(test_fn,(void*) &(max[0]), 3);
   my_sch.eventCancel(2);
   sleep(4);
    //note actual program will waite in destructor for all events to finish
   REQUIRE( (thread_val == 123999123*2) );
}

TEST_CASE("Official Evt test","[doing work]")
{
    EventScheduler es(10);

    int m1 = 100, m2 = 200, m3 = 300, m4 = 5000, m5 = 1100, m6 = 1120;
    int a1 = 100, a2 = 200, a3 = 300, a4 = 5000, a5 = 1100, a6 = 1120;
    int i1, i2, i3, i4, i5, i6;

    i1 = es.eventSchedule(test_fn1, (void *)&a5, m5);
    i2 = es.eventSchedule(test_fn2, (void *)&a1, m1);
    i3 = es.eventSchedule(test_fn3, (void *)&a4, m4);
    i4 = es.eventSchedule(test_fn1, (void *)&a3, m3);
    es.eventCancel(i3);
    i5 = es.eventSchedule(test_fn2, (void *)&a2, m2);
    i6 = es.eventSchedule(test_fn3, (void *)&a6, m6);

}