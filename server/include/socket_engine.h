#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <netinet/in.h>
#include <sys/epoll.h>
#include <list>

using std::list;

#define EPOLL_SIZE 10000
#define PORT 3456
#define HOST "115.28.223.181"
#define BUF_SIZE 65536
#define EPOLL_RUN_TIMEOUT -1

namespace server {

class SocketEngine {

public:
  SocketEngine();

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
  void Loop();
  void Accept(int fd);
  void Read(int fd);
  void Write(int fd);

private:
  int listener_;                                // 监听socket
  struct sockaddr_in addr_;                     // 监听地址
  struct sockaddr_in client_addr_;              // 客户端地址
  struct epoll_event event_;                    // 监听者epoll事件
  struct epoll_event events_[EPOLL_SIZE];// 客户端epoll事件
  int epoll_fd_;                                // epoll fd
  list<int> clients_;                           // 客户端列表
  char* buffer_[BUF_SIZE];                      // 缓冲区大小


};

} // namespace server

#endif//SERVER_SOCKET_H_
