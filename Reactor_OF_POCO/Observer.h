#ifndef OBSERVER_H
#define OBSERVER_H

#include "AbstractObserver.h"
//@brief 
//This template class implements an adapter that sits between 
//a NotificationCenter and an object receving notifications from it.

//@param C :  the target object
//@param N :  the notification  the target object interested in.
template<class C, class N>
class Observer : public AbstractObserver{
 public:
  typedef void (C::*Callback)(N*);
  
  Observer(C& object, Callback method) : pObject_(object), method_(method) {

  }

  Observer(const Observer& observer) : AbstractObserver(observer),
                                       pObject_(observer.pObject_),
                                       method_(observer.method_) {

  }

  ~Observer() {

  }

  Observer& operator= (const Observer& observer) {
    if (&observer != this) {
      pObject_ = observer.pObject_;
      method_ = observer.method_;
    }
    return *this;
  }

  void notify(Notification* pNf) const {
    if (pObject_) {
      N* pCastNf = dynamic_cast<N*>(pNf);
      if (pCastNf) {
        pCastNf->duplicate();
        (pObject_->*method_)(pCastNf);
      }
    }
  }

  bool equals(const AbstractObserver& abstractObserver) const {
    const Observer* pObs = dynamic_cast<const Observer*>(&abstractObserver);
    return pObs && pObs->pObject_ == pObject_ && pObs->method_ == method_;
  }

  bool accepts(Notification* pNf) const {
    return dynamic_cast<N*>(pNf) != 0;
  }

  AbstractObserver* clone() const {
    return new Observer(*this);
  }

  void disable() {
    pObject_ = 0;
  }



 protected:
 private:
  Observer();

  C* pObject_;
  Callback method_;
};
#endif
