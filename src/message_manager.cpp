#include <iostream>
#include <string.h>
#include "../include/message_manager.h"

using namespace std;

Message::Message()
{
    msg_len = 0;
    msg_content = NULL;
}//end constructor

Message::Message(char* msg, size_t len)
{
    msg_len = len;
    for(int i = 0; i < len; i++)
        raw_data.push_back(msg[i]);

}//end msg constructor

Message::~Message()
{
  raw_data.empty();
  msg_len = 0;
}//end destructor

void Message::msgAddHdr(char *hdr, size_t len)
{
    for(int i = 0; i < len; i++)
    {
      raw_data.push_front(hdr[i]);
    }
    msg_len += len;
}//end msgAdd

char * Message::msgStripHdr(size_t len)
{
    char* to_ret = new char[len];
    for(int i = 0; i < len; i++ )
    {
      to_ret[i] = raw_data.front();
      raw_data.pop_front();
    }
    msg_len -= len;
    return to_ret;
}//end msg StripHdr

int Message::msgSplit(Message& secondMsg, int len)
{
  int to_ret = 0;

  if( len < msg_len && (len > 0) )
  {
    to_ret = 1;
    std::list<char> new_raw;
    int i = 0;
    //O(1)
    while(i <= len)
    {
        new_raw.push_front(raw_data.back());
        raw_data.pop_back();
        i++;
    }
    secondMsg.set_msg(new_raw);
    secondMsg.setLen(len);
    msg_len -= len;
  }
  return to_ret;
}//end msgSplit

void Message::msgJoin(Message& secondMsg)
{
  for( int i = 0; i < secondMsg.msgLen(); i++)
  {
    raw_data.push_back(secondMsg.get_msg().back());
    secondMsg.get_msg().pop_back();
  }
  msg_len += secondMsg.msgLen();
  secondMsg.setLen(0);
}//end msgJoin

size_t Message::msgLen()
{
  return msg_len;
}//end msgLen

void Message::msgFlat(char *buffer)
{
  int idx = 0;
  bzero(buffer, sizeof(buffer));
  std::list<char>::iterator it = raw_data.begin();

  int i = 0;
  while( it != raw_data.end() )
  {
    buffer[i++] = *it++;
  }

}//end msg_flat

std::list<char> Message::get_msg()
{
    return raw_data;
}//end get_msg

void Message::set_msg(std::list<char> new_msg)
{
   raw_data = new_msg;
}//end set_msg

void Message::setLen(size_t new_len)
{
   msg_len = new_len;
}//end setLen


