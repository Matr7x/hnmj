#include "message_handler.h"

#include <iostream>

using std::cout;
using std::endl;

namespace server {

MessageHandler::MessageHandler() {}

MessageHandler::~MessageHandler() {}

void MessageHandler::Parse(void* buf, unsigned int size, unsigned int& processed) {
  processed = 0;
  do {
    // 判断MsgHead是否被截断
    if (processed + sizeof(MsgHead) > size) return;

    // 将buffer转成MsgBuf指针
    MsgBuf* msg_buf = static_cast<MsgBuf*>(buf);

    // 判断包知否被截断
    if (processed + msg_buf->msg_head.msg_info.package_size > size) return;

    // 处理消息
    ProcessMessage(msg_buf);

    // 更新被处理的字节数
    processed += msg_buf->msg_head.msg_info.package_size;

  } while(processed < size);
}

void MessageHandler::ProcessMessage(MsgBuf* msg_buf) {
  cout << msg_buf->msg_head.msg_info.type << " " 
    << msg_buf->msg_head.msg_info.package_size << " " 
    << msg_buf->msg_head.msg_cmd.main_cmd << " " 
    << msg_buf->msg_head.msg_cmd.sub_cmd << endl;
}

} // namespace server
