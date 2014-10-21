#include "../include/message_manager.h"

using namespace std;

Message::Message()
{

}//end constructor

Message::Message(char* msg, size_t len)
{

}//end msg constructor

Message::~Message()
{

}//end destructor

void Message::msgAddHdr(char *hdr, size_t len)
{

}//end msgAdd

char * Message::msgStripHdr(char *hdr, size_t len)
{

}//end msg StripHdr

int Message::msgSplit(Message& secondMsg, int len)
{

}//end msgSplit

void Message::msgJoin(Message& secondMsg)
{

}//end msgJoin

size_t Message::msgLen()
{

}//end msgLen
