#ifndef NOTIFICATION_CENTER_H
#define NOTIFICATION_CENTER_H

#include "AbstractObserver.h"
#include "SharedPtr.h"

//@brief
//dispatch all notifications posted by othe objects to all observers.
//about handleNotification():
//  void MyClass::handleNotification(MyNotification* pNf) {
//    AutoPtr<MyNotification> nf(pNf);
//  }
class NotificationCenter {
 public:
  NotificationCenter();
  ~NotificationCenter();
  
  //Usage:
  //  Observer<MyClass, MyNotification> obs(*this, MyClass::handleNotification);
  //  notificationCenter.addObserver(obs);
  void addObserver(const AbstractObserver& observer);
  void removeObserver(const AbstractObserver& observer);
  //post a notification to the notification center
  void postNotification(Notification::Ptr pNotification);

  bool hasObservers() const;

  std::size_t countObservers() const;

  static NotificationCenter& defaultCenter();
 private:
  typedef SharedPtr<AbstractObserver> AbstractObserverPtr;
  typedef std::vector<AbstractObserverPtr> ObserverList;

  ObserverList observers_;
};
#endif
