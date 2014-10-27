#include <iostream>
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
    char* tmp = new char[len+1];
    memcpy(tmp, msg, len+1);
    raw_data.push_back(tmp);
}//end msg constructor

Message::~Message()
{
  std::list<char*>::iterator doomed = raw_data.begin();
  while(doomed!= raw_data.end())
  {
      delete *doomed;
      doomed++;
  }
  raw_data.empty();
  msg_len = 0;
}//end destructor

void Message::msgAddHdr(char *hdr, size_t len)
{
    char* tmp= new char[len+1];
    // O(1) where hdr << msg_len
    memcpy(tmp, hdr, len+1);
    //O(1)
    raw_data.push_front(tmp);
    msg_len += len;
}//end msgAdd

char * Message::msgStripHdr(size_t len)
{
    char* to_ret = new char[len];
    char* seg;
    int i = 0;
    while( i < len )
    {
      seg = raw_data.front();
      raw_data.pop_front();
      i += strlen(seg) + 1;
      memcpy(to_ret, seg, i);
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
    std::list<char*> new_raw;
    int i = 0;
    //O(1)
    while(i <= len)
    {
        new_raw.push_front(raw_data.back());
        raw_data.pop_back();
        i += strlen(new_raw.front())+1;
    }
    secondMsg.set_msg(new_raw);
    msg_len -= len;
  }
  return to_ret;
}//end msgSplit

void Message::msgJoin(Message& secondMsg)
{
  //O(n)
  msg_len += secondMsg.msgLen();
  raw_data.splice(raw_data.end(), raw_data, secondMsg.get_msg().begin());
  secondMsg.setLen(0);
}//end msgJoin

size_t Message::msgLen()
{
  return msg_len;
}//end msgLen

void Message::msgFlat(char *buffer)
{
  std::list<char*>::iterator it = raw_data.begin();
  while( it != raw_data.end() )
  {
    strcat(buffer, *it++);
  }
}//end msg_flat

std::list<char*> Message::get_msg()
{
    return raw_data;
}//end get_msg

void Message::set_msg(std::list<char*> new_msg)
{
   raw_data = new_msg;
}//end set_msg

void Message::setLen(size_t new_len)
{
   msg_len = new_len;
}//end setLen


