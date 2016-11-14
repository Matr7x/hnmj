#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <netinet/in.h>
#include <sys/epoll.h>

#define EPOLL_SIZE 10000
#define PORT 9999
#define HOST "127.0.0.1"
#define BUF_SIZE 1024

namespace server {

class Socket {

public:
  Socket();

public:
  int Init();
  
  int Run();

private:
  int InitAddr();
  int InitSocket();
  int SetNonBlocking(int sock);
  int InitEpoll();
  int HandleMessage(int client);
  int ProcessMessage(char * buf, int size);

private:
  int listener_;                                // 监听socket
  struct sockaddr_in addr_;                     // 监听地址
  struct sockaddr_in client_addr_;              // 客户端地址
  struct epoll_event event_;                    // 监听者epoll事件
  struct epoll_event events_[EPOLL_SIZE];// 客户端epoll事件
  int epoll_fd_;                                // epoll fd
  list<int> clients_;                           // 客户端列表


};

} // namespace server

#endif//SERVER_SOCKET_H_
