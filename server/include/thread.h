#ifndef PLSA_THREAD_H
#define PLSA_THREAD_H

#include "log_printer.h"
#include <pthread.h>

namespace server {

// Thread class, you can use it by inheriting it and 
// overwriting virtual Run() for completing what you want do. 
// if you want start thread class, just needing call Start().
class Thread {
public :
  Thread() : thd_(0), thread_status_(true), is_start_(false) {}

  // Starting the thread
  // return 0 if the thread can start properly
  int Start();

  // return true if the thread's status is proper or correct
  bool is_valid() const { 
    return thread_status_;
  }

  // getting mutable handle of thread
  pthread_t *get_handle() { 
    return &thd_;
  }

  // getting handle of thread
  const pthread_t *get_handle() const { 
    return &thd_;
  }

  // waiting until thread terminates
  // return 0 if thread terminates properly
  int Join();

  int Cancel();

  virtual void *Run() = 0;
  
  virtual ~Thread() {};

private :
  // callback function of thread
  static void *func(void *arg) {
    Thread *thread = static_cast<Thread*>(arg);
    return thread->Run();
  }

  pthread_t thd_;
  bool thread_status_;
  bool is_start_;
};

}  // namespace plsa_deserializer

#endif
