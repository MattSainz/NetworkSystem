/*
 * For Network Systems 4273
 * Manages network messages
 * avoids direct memory manipulation
 * Matthias Sainz
 */
#include <stddef.h>
#include <algorithm>
#include <list>

class Message
{
    public:
        /*
         * Creates a message object
         */
        Message();

        /*
         * Creates a message based on some parameters
         */
        Message(char * msg, size_t len);

        /*
         * Standard destructor
         */
        ~Message();

        /*
         * Attaches a header of len bytes to the front of the message object

         */
        void msgAddHdr(char *hdr, size_t len);

        /*
         * Strips len bytes from the message and returns a pointer
         * to the location of the stripped bytes
         */
        char * msgStripHdr(size_t len);

        /*
         * Splits a message into two messages
         * the original messages is reduced to length len bytes
         * from the beginning of the beginning and the secondMsg is
         * the remaining part of the original message
         * returns 1 or 0 based on success or failure
         */
        int msgSplit(Message& secondMsg, int len);

        /*
         * Joins the original message with the secondMsg
         * after this method runs secondMsg will be an empty message
         * assumes message being joined was the same that was split
         */
        void msgJoin(Message& secondMsg);

        /*
         * returns the length of the message
         */
        size_t msgLen();

        void setLen(size_t new_len);
        /*
         * copies the message to the buffer
         */
        void msgFlat(char* buffer);

        std::list<char> get_msg();

        /*
         * just reassigns pointers, assumes user did all the
         * memory management
         */
        void set_msg(std::list<char> new_msg);

    private:
        size_t msg_len;
        char *msg_content;
        std::list<char> raw_data;
        size_t buff_size = 256;
        /*
         * uses an over allocated array and generates a sub char array
         * using a pointer of an element in the over sized array i.e msg_content = &big_arr[buffsize-len];
         * works similar to std vector for resizing but avoids memcpy unless a resize is needed
         * this hopefully improves performance
         *
         * note realloc was not used because it appends memory to the end of the array
         * and we need to add headers to the beginning meaning we would have to shift everything down
         * a linked list could have been used but the expense of converting the list to a string
         * would probably negate the performance advantages (just a hypothesis tests needed)
         */
        void resize_arr(size_t len);

};

