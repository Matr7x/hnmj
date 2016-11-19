#ifndef SERVER_MESSAGE_HANDLER_H_
#define SERVER_MESSAGE_HANDLER_H_

#include "message.h"

namespace server {

class MessageHandler {

public:
  MessageHandler();

  ~MessageHandler();

public:
  void Parse(void* buf, unsigned int size, unsigned int& processed);
  void ProcessMessage(MsgBuf* msg_buf);

};

} // namespace server

#endif//SERVER_MESSAGE_HANDLER_H_
