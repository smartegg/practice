#ifndef SINGLETON_HOLDER_H
#define SINGLETON_HOLDER_H

template <class S>
class SingletonHolder {
 public:
  SingletonHolder() : pS_(0) {

  }

  ~SingletonHolder() {
    delete pS_;
  }

  S* get() {
    if (!pS_)
      pS_ = new S;
    return pS_;
  }

 private:
  S* pS_;
};
#endif
