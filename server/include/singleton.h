#ifndef PLSA_SINGLETON_H
#define PLSA_SINGLETON_H

#include <new>
#include "mutexes.h"

namespace xone {
// Singleton design pattern
// the way you properly use it is as followed:
// class YourClass {
//
// private :
//   YourClass();  // [Notice] this step is need or you can new object
//   friend class Singleton<YourClass>;  
// };
// YourClass *instance = Singleton<YourClass>::GetInstance();  // enjoy it
template <class TYPE>
class Singleton {
public:
  // [Notice] caller aren't allowed to delete the returned object
  static TYPE* GetInstance() {
    static Releaser releaser;
    if (NULL == instance_) {
      mutex_.Lock();
      if (NULL == instance_) {
        instance_ = new (std::nothrow) TYPE();
      }
      mutex_.UnLock();
    }
    return instance_;
  }

  class Releaser {
  public:
    ~Releaser() {
      if (NULL != Singleton<TYPE>::instance_) {
        delete Singleton<TYPE>::instance_;
        Singleton<TYPE>::instance_ = NULL;
      }
    }

  };
private:
  Singleton();
  ~Singleton();
  Singleton(const Singleton&);
  Singleton& operator = (const Singleton&);

  static TYPE* instance_;
  static Mutex mutex_;
};

template <class TYPE>
TYPE* Singleton<TYPE>::instance_ = NULL;

template <class TYPE>
Mutex Singleton<TYPE>::mutex_;

}  // namespace xone

#endif

