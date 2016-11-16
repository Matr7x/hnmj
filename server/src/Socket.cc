#include "Socket.h"

#include <iostream>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

using std::cout;
using std::endl;

namespace server {

Socket::Socket() {}

int Socket::Init() {
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

int Socket::Run() {
  while (true) {
    // 等待事件
    int count = epoll_wait(epoll_fd_, events_, EPOLL_SIZE, EPOLL_RUN_TIMEOUT);
    if (count < 0) {
      cout << "Fatal: wait epoll event failed. errno[" << errno << "]" << endl;
      return 255;
    }

    cout << count << endl;
    cout << EPOLLERR << endl;
    cout << EPOLLHUP << endl;

    // 解析事件
    for (int i = 0; i < count; i++) {
      cout << events_[i].events << endl;
      if ((events_[i].events & EPOLLERR) ||
              (events_[i].events & EPOLLHUP) ||
              (!(events_[i].events & EPOLLIN)))
      {
              /* An error has occured on this fd, or the socket is not
                 ready for reading (why were we notified then?) */
cout << "epoll error" << endl;
        continue;
      }
      // 来自监听的事件
      if (events_[i].data.fd == listener_) {
        // 接收请求
        socklen_t sock_len = sizeof(struct sockaddr_in);
        int client = accept(listener_, 
            (struct sockaddr *)&client_addr_, 
            &sock_len);

        cout << client << endl;
        // 设置nonblocking
        SetNonBlocking(client);

        // 添加client监听事件
        event_.data.fd = client;
        int code = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, client, &event_);
        if (code < 0) {
          cout << "Client add epoll event failed" << endl;
          return 255;
        }
      } 
      // 来自客户端的事件
      else {
        int code = HandleMessage(events_[i].data.fd);
        if (0 != code) {
          cout << "Handle Message failed." << endl;
          return 255;
        }
      }
    }
  }
}

int Socket::HandleMessage(int client) {
  // 初始化buffer和length
  char buf[BUF_SIZE];
  bzero(buf, BUF_SIZE);
  int len = 0;

  len = recv(client, buf, BUF_SIZE, 0);

  if (len < 0) {
    cout << "Recv failed." << endl;
    return 255;
  }
  
  if (0 == len) {
    // 客户端关闭
    close(client);
    clients_.remove(client);
  } else {
    ProcessMessage(buf, len);
  }

  return 0;
}

int Socket::ProcessMessage(char * buf, int size) {
  // TODO: 处理Message
  return 0;
}

int Socket::InitAddr() {
  addr_.sin_family = PF_INET;
  addr_.sin_port = htons(PORT);
  addr_.sin_addr.s_addr = inet_addr(HOST);
  return 0;
}

int Socket::InitSocket() {
  listener_ = socket(PF_INET, SOCK_STREAM, 0);

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

int Socket::SetNonBlocking(int sock) {
  int code = fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
  if (code < 0) {
    cout << "Set non blocking failed! errno[" << errno << "]"<< endl;
    return 255;
  }
  return 0;
}

int Socket::InitEpoll() {
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
