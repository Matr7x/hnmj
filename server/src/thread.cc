#include "thread.h"

namespace thread {

int Thread::Start()
{
  int ret = 0;

  if (false == is_start_) {
    ret = pthread_create(&thd_, NULL, Thread::func, this);
    if (0 != ret) {
      thread_status_ = false;
    } else {
      is_start_ = true;
    }
  }
  return ret;
}

int Thread::Join()
{
  void *retval = 0;
  if (true == thread_status_ && true == is_start_) {
    pthread_join(thd_, &retval);
    if (static_cast<void *>(0) != retval) {
      return -1;
    } else {
      return 0;
    }
  }
  return -1;
}

int Thread::Cancel()
{
  return pthread_cancel(thd_);
}

}  // namespace plsa_deserializer
