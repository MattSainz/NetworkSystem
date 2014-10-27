#define CATCH_CONFIG_MAIN

#include "../test/catch.h"
#include "../include/event_scheduler.h"
#include "../include/message_manager.h"
#include "../include/thread_pool.h"
using namespace std;

void test_fn(void *par)
{
    cout << "entering test_fn " << *(int *)par << endl;
    cout << "exiting test_fn " << *(int *)par << endl;
}

TEST_CASE("Insanity Check", "[hello world]")
{
   REQUIRE( true );
}

TEST_CASE("Message constructor test", "[Constructor]")
{
    int len = 4;
    Message* test = new Message("test", len);
    char* buff = new char[len];
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

TEST_CASE("Threads","[hopefull]")
{
    ThreadPool th(3);
    int max[20];

    for (int i = 0; i < 5; i++)
    {

        for (int j = 0; j < 4; j++)
        {
            max[(i*4)+j] = 100 * ((i * 4) + j);
            if (th.thread_avail())
                th.dispatch_thread(test_fn, (void *)&(max[(i*4)+j]));
            else
                cout << "No thread is avalable for " << max[(i*4)+j] << endl;
        }
        sleep(6);
    }

    REQUIRE(0);
}