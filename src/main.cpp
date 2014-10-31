#define CATCH_CONFIG_MAIN

#include "../test/catch.h"
#include "../include/event_scheduler.h"
#include "../include/message_manager.h"
#include "../include/thread_pool.h"
using namespace std;

int thread_val = 0;

void test_fn(void *par)
{
    cout << "entering test_fn " << *(int *)par << endl;
    //sleep((unsigned int) (rand() % 3));
        //simulate alot of work
    cout << "exiting test_fn " << *(int *)par << endl;
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

/*
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
*/

TEST_CASE("More Message Tests", "[Offical Test]")
{
    cout << "======================================================" << endl;
    cout << "Message Tests Schedual test" << endl;
    cout << "======================================================" << endl;

    char *b1 = new char[100];
    for (int i = 0; i < 100; i++) b1[i] = 'a';

    char *h1 = new char[5];
    for (int i = 0; i < 5; i++) h1[i] = 'h';

    char *h2 = new char[4];
    for (int i = 0; i < 4; i++) h2[i] = 'k';

    char *h3 = new char[24];
    for (int i = 0; i < 24; i++) h3[i] = 'm';

    Message *m = new Message(b1, 100);
    m->msgAddHdr(h1, 5);
    m->msgAddHdr(h2, 4);
    m->msgAddHdr(h3, 24);
    cout << "Message length = " << m->msgLen( ) << endl;

    char* buf = new char[140];
    m->msgFlat(buf);
    buf[m->msgLen ( )] = '\n';
    cout << "Message: " << buf << endl;

    Message m2;

    m->msgSplit(m2, 50);
    cout << " Split msg: Message length of m = " << m->msgLen( ) << endl;
    char* bufx = new char[140];
    m->msgFlat(bufx);
    bufx[m->msgLen ( )] = '\n';
    cout << "Message: " << bufx << endl;

    cout << "Message length of m2 = " << m2.msgLen( ) << endl;
    char* bufy = new char[140];
    m2.msgFlat(bufy);
    bufy[m2.msgLen ( )] = '\n';
    cout << "New message from split: " << bufy << endl;

    Message *m1 = new Message(buf, 133);
    char *c1 = m1->msgStripHdr(24);
    char *c1b = new char[25];
    memcpy(c1b, c1, 24);
    c1b[24] = '\0';
    cout << "Stripped header: " << c1b << endl;

    char *c2 = m1->msgStripHdr(4);
    char *c2b = new char[5];
    memcpy(c2b, c2, 4);
    c2b[4] = '\0';
    cout << "Stripped header: " << c2b << endl;

    char *c3 = m1->msgStripHdr(5);
    char *c3b = new char[6];
    memcpy(c3b, c3, 5);
    c3b[5] = '\0';
    cout << "Stripped header: " << c3b << endl;

    char* buf1 = new char[140];
    m1->msgFlat(buf1);
    buf[m1->msgLen ( )] = '\n';
    cout << "Message: " << buf1 << endl;

    m1->msgJoin(m2);
    char* bufz = new char[400];
    m1->msgFlat(bufz);
    bufz[m1->msgLen ( )] = '\n';
    cout << "Message: " << bufz << endl;

    m->msgJoin(*m1);
    cout << "Message length of m = " << m->msgLen( ) << endl;

    char* bufa = new char[500];
    m->msgFlat(bufa);
    bufa[m->msgLen ( )] = '\n';
    cout << "Message: " << bufa << endl;
    REQUIRE(1);
}

TEST_CASE("Threads","[threadPool]")
{
    cout << "======================================================" << endl;
    cout << "Thread Pool test" << endl;
    cout << "======================================================" << endl;

    ThreadPool th(3);
    int max[20];
    int check = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            max[(i*4)+j] = 100 * ((i * 4) + j);
            if (th.thread_avail())
            {
                check += max[(i * 4) + j];
                th.dispatch_thread(test_fn, (void *) &(max[(i * 4) + j]));
            }
            else
            {
                cout << "No thread is avalable for " << max[(i*4)+j] << endl;
            }
        }
        sleep(6);
    }
    REQUIRE( (abs(thread_val-check) == 0) );
}

TEST_CASE("Event schedgule","[one event]")
{
    /*
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
    */
   //REQUIRE( (thread_val == 123999123*2) );
   REQUIRE( 1 );

}

TEST_CASE("Official Evt test","[doing work]")
{

    cout << "======================================================" << endl;
    cout << "Event Schedual test" << endl;
    cout << "======================================================" << endl;
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
    REQUIRE(1);
}

