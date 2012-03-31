#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include "Exception.h"
#include <algorithm>

class ReferenceCounter {
 public:
  ReferenceCounter(): cnt_(1) {

  }

  void duplicate() {
    ++cnt_;
  }

  int release() {
    return --cnt_;
  }

  int referenceCount() {
    return cnt_;
  }

 private:
  int cnt_;
};

//@brief default release policy for SharedPtr.
template <class C>
class ReleasePolicy {
  public:
   static void release(C* pObj) {
     delete pObj;
   }
};

//@brief default release policy for SharedPtr holding array
template <class C>
class ReleaseArrayPolicy {
 public:
  static void release(C* pObj) {
    delete []pObj;
  }
};

//@biref similar with tr1::sharedPtr<>
template <class C, class RC = ReferenceCounter, class RP = ReleasePolicy<C> >
class SharedPtr {
 public:
  SharedPtr() : pCounter_(new RC), ptr_(0) {

  }

  SharedPtr(C* ptr) : pCounter_(new RC), ptr_(ptr) {

  }

  template <class Other, class OtherRP>
  SharedPtr(const SharedPtr<Other, RC, OtherRP>& ptr) 
      : pCounter_(ptr.pCounter_), ptr_(const_cast<Other*>(ptr.get())) {
    pCounter_->duplicate();
  }

  SharedPtr(const SharedPtr& ptr) : pCounter_(ptr.pCounter_), ptr_(ptr.ptr_) {
    pCounter_->duplicate();
  }

  ~SharedPtr() {
    release();
  }

  SharedPtr& assign(C* ptr) {
    if(get() != ptr) {
      RC* pTmp = new RC;
      release();
      pCounter_ = pTmp;
      ptr_ = ptr;
    }
    return *this;
  }

  SharedPtr& assign(const SharedPtr& ptr) {
    if (&ptr != this) {
      SharedPtr tmp(ptr);
      swap(tmp);
    }
    return *this;
  }

  template <class Other, class OtherRP>
  SharedPtr& assign(const SharedPtr<Other, RC, OtherRP>& ptr) {
    if (ptr.get() != ptr_) {
      SharedPtr tmp(ptr);
      swap(tmp);
    }
    return *this;
  }

  SharedPtr& operator = (C* ptr) {
    return assign(ptr);
  }

  SharedPtr& operator = (const SharedPtr& ptr) {
    return assign(ptr);
  }

  template <class Other, class OtherRP>
  SharedPtr& operator = (const SharedPtr<Other, RC, OtherRP>& ptr) {
    return assign<Other>(ptr);
  }

  void swap(SharedPtr& ptr) {
    std::swap(ptr_, ptr.ptr_);
    std::swap(pCounter_, ptr.pCounter_);
  }

  template <class Other>
  SharedPtr<Other, RC, RP> cast() const {
    Other* pOther = dynamic_cast<Other*>(ptr_);
    if (pOther)
      return SharedPtr<Other, RC, RP>(pCounter_, pOther);
    return SharedPtr<Other, RC, RP>();
  }

  template <class Other>
  SharedPtr<Other, RC, RP> unsafeCast() const {
    Other* pOther = static_cast<Other*>(ptr_);
    return SharedPtr<Other, RC, RP>(pCounter_, pOther);
  }

  C* operator -> () {
    return decref();
  }
  
  const C* operator -> () const{
    return decref();
  }

  C& operator * () {
    return *decref();
  }

  const C& operator * () const {
    return *decref();
  }

  C* get() {
    return ptr_;
  }

  const C* get() const {
    return ptr_;
  }

  operator C* () {
    return ptr_;
  }

  operator const C* () {
    return ptr_;
  }

  bool operator ! () const {
    return ptr_ == 0;
  }

  bool isNull() const {
    return ptr_ == 0;
  }

  bool operator == (const SharedPtr& ptr) const {
    return get() == ptr.get();
  }

  bool operator == (const C* ptr) const {
    return get() == ptr;
  }

  bool operator == (C* ptr) const {
    return get() == ptr;
  }

  bool operator != (const SharedPtr& ptr) const {
    return get() != ptr.get();
  }

  bool operator != (const C* ptr) const {
    return get() != ptr;
  }

  bool operator != (C* ptr) const {
    return get() != ptr;
  } 

  bool operator < (const SharedPtr& ptr) const {
    return get() < ptr.get();
  }

  bool operator < (const C* ptr) const {
    return get() < ptr;
  }

  bool operator < (C* ptr) const {
    return get() < ptr;
  } 

 bool operator <= (const SharedPtr& ptr) const {
    return get() <= ptr.get();
  }

  bool operator <= (const C* ptr) const {
    return get() <= ptr;
  }

  bool operator <= (C* ptr) const {
    return get() <= ptr;
  }

  bool operator > (const SharedPtr& ptr) const {
    return get() > ptr.get();
  }

  bool operator > (const C* ptr) const {
    return get() > ptr;
  }

  bool operator > (C* ptr) const {
    return get() > ptr;
  } 

  bool operator >= (const SharedPtr& ptr) const {
    return get() >= ptr.get();
  }

  bool operator >= (const C* ptr) const {
    return get() >= ptr;
  }

  bool operator >= (C* ptr) const {
    return get() >= ptr;
  } 

  int referenceCount() const {
    return pCounter_->referenceCount();
  }
                                                        
 private:
  C* decref() const {
    if (!ptr_) {
      throw NullPointerException();
    }
    return ptr_;
  }

  void release() {
    int i = pCounter_->release();
    if (i == 0) {
      RP::release(ptr_);
      ptr_ = 0;
      delete pCounter_;
      pCounter_ = 0;
    }
  }

  SharedPtr(RC* pCounter, C* ptr) : pCounter_(pCounter), ptr_(ptr) {
    pCounter_->duplicate();
  }

 private:
  RC* pCounter_;
  C*  ptr_;

  template <class OtherC, class OtherRc, class OtherRP> friend class SharedPtr;
};
#endif
