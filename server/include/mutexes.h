#ifndef PLSA_MUTEXES_H
#define PLSA_MUTEXES_H

#include <pthread.h>

namespace xone { 

// When we successfully initialized Mutex, we think Lock() and 
// Unlock() would not be error based on linux interface manpage,
class Mutex {
public:
    Mutex();
    ~Mutex();
    void Lock();
    int TryLock();
    void UnLock();
    pthread_mutex_t *get_mutable_mutex() { return &mutex_; }
    const pthread_mutex_t *get_mutex() const { return &mutex_; }
private:
    pthread_mutex_t mutex_;
};

// you can use AutoMutex like this :
// you have already defined Mutex mutex, then defining AutoMutex 
// automutex(&mutex), which automutex will automaticly lock until 
// automutex leave his variable scope and then automaticly unlock.
class AutoMutex { 
public:
  AutoMutex(Mutex* mutex) : mutex_(mutex) {
    mutex_->Lock();
  }
  ~AutoMutex() {
    mutex_->UnLock();
  }
private:
  Mutex* mutex_;
};

}  // namespace plsa


#endif
