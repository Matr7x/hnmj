#ifndef SERVER_MESSAGE_H_
#define SERVER_MESSAGE_H_

#define TCP_PKG_BUF_SIZE 65536
#define TCP_MSG_BUF_SIZE (TCP_PKG_BUF_SIZE-sizeof(MsgHead))

namespace server {

typedef struct {
  unsigned char                     type;                       // 数据类型
  unsigned char                     check_code;                 // 校验码
  unsigned short                    package_size;               // 包大小
} MsgInfo;

typedef struct {
  unsigned short                    main_cmd;                   // 主命令
  unsigned short                    sub_cmd;                    // 子命令
} MsgCmd;

typedef struct {
  MsgInfo                           msg_info;
  MsgCmd                            msg_cmd;
} MsgHead;

typedef struct {
  MsgHead                           msg_head;                   // 消息头
  unsigned char                     msg_buf[TCP_MSG_BUF_SIZE];  // 消息包缓冲区
} MsgBuf;

} // namespace server

#endif//SERVER_MESSAGE_H_
