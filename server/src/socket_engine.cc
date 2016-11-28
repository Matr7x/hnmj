#include "socket_engine.h"

#include "singleton.h"
#include "message_handler.h"

#include <iostream>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <netinet/tcp.h>

using std::cout;
using std::endl;

#define DEBUG

namespace server {

SocketEngine::SocketEngine() {}

int SocketEngine::Init() {
  int code = 0;

  // 初始化地址
  code = InitAddr();
  if (0 != code) {
    cout << "Init addr failed." << endl;
    return 255;
  }

  // 初始化Socket
  code = InitSocket();
  if (0 != code) {
    cout << "Init socket failed." << endl;
    return 255;
  }

  code = InitEpoll();
  if (0 != code) {
    cout << "Init epoll failed." << endl;
    return 255;
  }

  return 0;
}

int SocketEngine::Run() {
  while (true) {
    Loop();
  }
  return 0;
}

void SocketEngine::Loop() {
  // 等待事件
  int count = epoll_wait(epoll_fd_, events_, EPOLL_SIZE, EPOLL_RUN_TIMEOUT);
  if (count < 0) {
    cout << "Fatal: wait epoll event failed. errno[" << errno << "]" << endl;
    return;
  }

  // 解析事件
  for (int i = 0; i < count; i++) {
    int fd = events_[i].data.fd;
    int events = events_[i].events;

    if (events & EPOLLIN ) {
      if (fd == listener_) {
        Accept(fd);
      } else {
        Read(fd);
      }
    } else if (events & EPOLLOUT) {
      Write(fd);
    } else {
      cout << "Fatal: Unknow event "<< endl;
      return;
    }
  }
}

void SocketEngine::Accept(int fd) {
  // 接收请求
  socklen_t sock_len = sizeof(struct sockaddr_in);
  int client = accept(fd, (struct sockaddr *)&client_addr_, &sock_len);

  cout << "Accept client sock[" << client << "]" << " ip[" << inet_ntoa(client_addr_.sin_addr) 
    << "]" << endl;

  // 设置nonblocking
  SetNonBlocking(client);

  // 添加client监听事件
  event_.data.fd = client;
  event_.events = EPOLLIN | EPOLLOUT | EPOLLET; // 读，边沿触发
  int code = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client, &event_);
  if (code < 0) {
    cout << "Client add epoll event failed" << endl;
    return;
  }

  clients_.push_back(client);
}

void SocketEngine::Read(int fd) {
  int total_read = 0;
  int buf_read = 0;
  memset(buffer_, 0, sizeof(buffer_));
  while ((buf_read = read(fd, buffer_, BUF_SIZE)) > 0) {
    //cout << buf_read;
    cout << buffer_ << endl;

    // TODO: Process readed stream
    unsigned int processed = 0;
    Singleton<MessageHandler>::GetInstance()->Parse(buffer_, buf_read, processed);
    
    memset(buffer_, 0, sizeof(buffer_));
    total_read += buf_read;
  }

  // 判断连接
  /*
  struct tcp_info info; 
  int len=sizeof(info); 
  getsockopt(fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len); 
  if((info.tcpi_state == TCP_ESTABLISHED)) {
    cout << "connected" << endl;
  } else {
    cout << "closed" << endl;
  }
  */

  // 判断是否链接
  if (0 == total_read && 0 == buf_read) {
    // 处理关闭连接
    cout << "client " << fd << " closed" << endl;
    return;
  } 
  
  // 判断是否正常读取0
  if (0 == buf_read) {
    return;
  }

  // 本次读取完毕，返回EAGAIN
  if (-1 == buf_read && EAGAIN == errno) {
    return;
  } 
  // 位置错误
  else {
    cout << "Fatal: Unknow read error. errno[" << errno << "]" << endl;
  }
}

void SocketEngine::Write(int fd) {
}

int SocketEngine::ProcessMessage(char * buf, int size) {
  // TODO: 处理Message
  return 0;
}

int SocketEngine::InitAddr() {
  addr_.sin_family = PF_INET;
  addr_.sin_port = htons(PORT);
  addr_.sin_addr.s_addr = inet_addr(HOST);
  return 0;
}

int SocketEngine::InitSocket() {
  listener_ = socket(PF_INET, SOCK_STREAM, 0);

#ifdef DEBUG
  int on = 1;
  setsockopt(listener_,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
#endif

  if (listener_ < 0) {
    cout << "Create socket failed!" << endl;
    return 255;
  }

  int code = SetNonBlocking(listener_);
  if (0 != code) {
    cout << "Set non blocking for listerner failed." << endl;
    return 255;
  }

  code = bind(listener_, (struct sockaddr *)&addr_, sizeof(addr_));
  if (0 != code) {
    cout << "Bind failed. errno[" << errno << "]" << endl;
    return 255;
  }

  code = listen(listener_, 128);
  if (0 != code) {
    cout << "Listen failed. errno[" << errno << "]" << endl;
    return 255;
  }

  return 0;
}

int SocketEngine::SetNonBlocking(int sock) {
  // TODO: 需要区分开get和set
  int code = fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
  if (code < 0) {
    cout << "Set non blocking failed! errno[" << errno << "]"<< endl;
    return 255;
  }
  return 0;
}

int SocketEngine::InitEpoll() {
  // 创建epoll
  epoll_fd_ = epoll_create(EPOLL_SIZE);
  if (epoll_fd_ < 0) {
    cout << "Create epoll failed" << endl;
    return 255;
  }

  // 注册事件
  event_.events = EPOLLIN | EPOLLET; // 读，边沿触发
  event_.data.fd = listener_;
  int code = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, listener_, &event_);
  if (code < 0) {
    cout << "Epoll ctl add failed" << endl;
    return 255;
  }

  return 0;
}

} // namnespace server
