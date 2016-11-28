#include "../../server/include/message.h"

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define HOST "115.28.223.181"
#define PORT 3456
#define BUF_SIZE 64436

using std::cout;
using std::endl;

using server::MsgBuf;
using server::MsgHead;

int main(int argc, char* argv[]) {
  // Init addr
  struct sockaddr_in addr;
  addr.sin_family = PF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = inet_addr(HOST);

  // 创建sock
  int sock = socket(PF_INET, SOCK_STREAM, 0);   
  if (-1 == sock) {
    cout << "Create socket error" << endl;
    return 255;
  }

  // 连接服务器
  int code = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
  if (-1 == code) {
    cout << "Connect to server error. error [" << errno << "]" << endl;
    return 255;
  }

  MsgBuf msg_buf;

  msg_buf.msg_head.msg_info.type = 0;
  msg_buf.msg_head.msg_info.check_code = 0;
  msg_buf.msg_head.msg_info.package_size = sizeof(MsgHead);
  msg_buf.msg_head.msg_cmd.main_cmd = 0;
  msg_buf.msg_head.msg_cmd.sub_cmd = 0;
  memset(msg_buf.msg_buf, 0, sizeof(msg_buf.msg_buf));
  snprintf((char*)msg_buf.msg_buf, BUF_SIZE, "Hello server, I am process %d\n", getpid()); 
  int n = strlen((char*)msg_buf.msg_buf);
  msg_buf.msg_head.msg_info.package_size += n;
  send(sock, &msg_buf, msg_buf.msg_head.msg_info.package_size, 0);

  //cout << n << endl;
  //send(sock, buffer, n, 0);

  //memset(buffer, 0, sizeof(buffer));
  //snprintf(buffer, BUF_SIZE, "This is second message\n"); 
  //n = strlen(buffer);
  //cout << n << endl;
  //send(sock, buffer, n, 0);

  close(sock);

  return 0;
}
