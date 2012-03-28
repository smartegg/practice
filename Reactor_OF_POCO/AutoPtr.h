#ifndef AUTOPTR_H
#define AUTOPTR_H
#include "Exception.h"
#include <algorithm>
//a generic smarter pointer similar to tr1::shared_ptr
//CAUTION: class C must implements RefCountedObject 
template<class C>
class AutoPtr {
 public:
  AutoPtr() : ptr_(0) {

  }

  AutoPtr(C* ptr) : ptr_(ptr) {

  }

  AutoPtr(C* ptr, bool shared) : ptr_(ptr) {
    if (shared) {
      ptr_->duplicate();
    }
  }

  AutoPtr(const AutoPtr& ptr) : ptr_(ptr.ptr_) {
    if (ptr_) {
      ptr_->duplicate();
    }
  }

  template<class Other>
  AutoPtr(const AutoPtr<Other>& ptr) : ptr_(const_cast<Other*>((ptr.get()))) {
    if (ptr_) {
      ptr_->duplicate();
    }
  }

  ~AutoPtr() {
    if (ptr_) {
      ptr_->release();
    }
  }

  AutoPtr& assign(C* ptr) {
    if(ptr != ptr_) {
      if (ptr_) 
        ptr_->release();
      ptr_ = ptr;
    }
    return *this;
  }

  AutoPtr& assign(C* ptr, bool shared) {
    if (ptr != ptr_) {
      if (ptr_)
        ptr_->release();
      ptr_ = ptr;
      if (shared && ptr_)
        ptr_->duplicate();
    }
    return *this;
  }

  AutoPtr& assign(const AutoPtr& ptr) {
    if(ptr_ != ptr.ptr_) {
      if (ptr_)
        ptr_->release();
      ptr_ = ptr.ptr_;
      if (ptr_)
        ptr_->duplicate();
    }
    return *this;
  }
  
  template <class Other>
  AutoPtr& assign(const AutoPtr<Other>& ptr) {
    if (ptr_ != ptr.get()) {
      if (ptr_)
        ptr_->release();
      ptr_ = const_cast<Other*>(ptr.get());
      if (ptr_)
        ptr_->duplicate();
    }
    return *this;
  }

  AutoPtr& operator= (C* ptr) {
    return assign(ptr);
  }

  AutoPtr& operator= (const AutoPtr& ptr) {
    return assign(ptr);
  }

  template <class Other>
  AutoPtr& operator= (const AutoPtr<Other>& ptr) {
    return assign<Other>(ptr);
  }

  void swap(AutoPtr& ptr) {
    std::swap(ptr_, ptr.ptr_);
  }
  //cast by dynamic cast
  //Usage:
  //  (assume class Sub: class Super)
  //  AutoPtr<Super> super(new Sub());
  //  AutoPtr<Sub> sub = super.cast<Sub>();
  template <class Other> 
  AutoPtr<Other> cast()const {
    Other* pOther = dynamic_cast<Other*>(ptr_);
    return AutoPtr<Other>(pOther, true);
  }
  //cast by static cast
  //usage:
  //  (assume class sub :class super)
  //  AutoPtr<Super> super(new sub())
  //  AUtoPtr<Sub> sub = super.unsafeCast<Sub>();
  template <class Other>
  AutoPtr<Other> unsafeCase() const {
    Other* pOther = static_cast<Other*>(ptr_);
    return AutoPtr<Other>(pOther, true);
  }

  C* operator-> () {
    if (ptr_)
      return ptr_;
    else 
      throw NullPointerException();
  }

  const C* operator->() const{
    if (ptr_)
      return ptr_;
    else 
      throw NullPointerException();
  }

  C& operator* () {
    if (ptr_)
      return *ptr_;
    else
      throw NullPointerException();
  }

  const C& operator*() const {
    if (ptr_)
      return *ptr_;
    else
      throw NullPointerException();
  }

  C* get() {
    return ptr_;
  }

  const C* get() const {
    return ptr_;
  }

  operator C*() {
    return ptr_;
  }

  operator const C*() const {
    return ptr_;
  }

  bool operator! () const {
    return ptr_ == 0;
  }

  bool isNull() const {
    return ptr_ == 0;
  }

  C* duplicate() {
    if (ptr_)
      ptr_->duplicate();
    return ptr_;
  }

  bool operator== (const AutoPtr& ptr) const {
    return ptr_ == ptr.ptr_;
  }

  bool operator == (const C* ptr) const {
    return ptr_ == ptr;
  }

  bool operator == (C* ptr) const {
    return ptr_ == ptr;
  }

  bool operator != (const AutoPtr& ptr) const {
    return ptr_ != ptr.ptr_;
  }

  bool operator != (const C* ptr) const {
    return ptr_ != ptr;
  }

  bool operator != (C* ptr) const {
    return ptr_ != ptr;
  }

  bool operator< (const AutoPtr& ptr) const {
    return ptr_ < ptr.ptr_;
  }

  bool operator< (const C* ptr) const {
    return ptr_ < ptr;
  }

  bool operator < (C* ptr)const {
    return ptr_ < ptr;
  }

  bool operator <= (const AutoPtr& ptr) const {
    return ptr_ <= ptr.ptr_;
  }

  bool operator <= (const C* ptr) const {
    return ptr_ <= ptr;
  }

  bool operator <= (C* ptr) const {
    return ptr_ <= ptr;
  }

  bool operator > (const AutoPtr& ptr) const {
    return ptr_ > ptr.ptr_;
  }

  bool operator > (const C* ptr) const {
    return ptr_ > ptr;
  }

  bool operator > (C* ptr) const {
    return ptr_ > ptr;
  }

  bool operator >= (const AutoPtr& ptr) const {
    return ptr_ >= ptr.ptr_;
  }

  bool operator >= (const C* ptr) const {
    return ptr_ >= ptr;
  }

  bool operator >= (C* ptr) const {
    return ptr_ >= ptr;
  } 

 private:
  C* ptr_;

};

template <class C>
inline void swap(AutoPtr<C>& p1, AutoPtr<C>& p2) {
  p1.swap(p2);
}
#endif

