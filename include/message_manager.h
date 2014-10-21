/*
 * For Network Systems 4273
 * Manages network messages
 * Matthias Sainz
 */
#include <stddef.h>
#include <algorithm>

class Message
{
    public:
        /*
         */
        Message();

        /*
         */
        Message(char * msg, size_t len);

        ~Message();

        /*
         */
        void msgAddHdr(char *hdr, size_t len);

        /*
         */
        char * msgStripHdr(char *hdr, size_t len);

        /*
         */
        int msgSplit(Message& secondMsg, int len);

        /*
         */
        void msgJoin(Message& secondMsg);

        /*
         */
        size_t msgLen();

        /*
         */
        void msgFlat(char* buffer);
};

