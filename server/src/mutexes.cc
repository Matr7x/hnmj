#include <sys/types.h>
#include <unistd.h>
#include "mutexes.h"
#include "log_printer.h"

namespace server {

Mutex::Mutex() {
  int ret = pthread_mutex_init(&mutex_, NULL);
  if(ret < 0) {
    LOGGING_FATAL("pthread_mutex_init(%p) failed(%s)", &mutex_, strerror(ret));
  }
}

Mutex::~Mutex() {
  int ret = pthread_mutex_destroy(&mutex_);
  if(ret < 0) {
   LOGGING_FATAL("pthread_mutex_destroy(%p) failed(%s)", &mutex_, strerror(ret));
  }
}

void Mutex::Lock() {
  int ret = pthread_mutex_lock(&mutex_);
  if(ret < 0) {
    LOGGING_FATAL("pthread_mutex_lock(%p) failed(%s)", &mutex_, strerror(ret));
  }
}

int Mutex::TryLock() {
  int ret = pthread_mutex_trylock(&mutex_);
  if(ret < 0) {
    LOGGING_FATAL("pthread_mutex_trylock(%p) failed(%s)", &mutex_, strerror(ret));
    return ret;
  }
  return ret;
}

void Mutex::UnLock() {
  int ret = pthread_mutex_unlock(&mutex_);
  if(ret < 0) {
    LOGGING_FATAL("pthread_mutex_unlock(%p) failed(%s)", &mutex_, strerror(ret));
  }
}

} // namespace plsa

