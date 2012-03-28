#include "NotificationCenter.h"

#include "SingletonHolder.h"

NotificationCenter::NotificationCenter() {

}

NotificationCenter::~NotificationCenter() {

}

void NotificationCenter::addObserver(const AbstractObserver& observer) {
  observers_.push_back(observer.clone());
}

void NotificationCenter::removeObserver(const AbstractObserver& observer) {
  for (ObserverList::iterator it = observers_.begin(); it != observers_.end();
       ++it) {
    if (observer.equals(**it)) {
      (*it)->disable(); 
      observers_.erase(it);
      return;
    }
  }
}

void NotificationCenter::postNotification(Notification::Ptr pNotification) {
  ObserverList observersToNotify(observers_);
  for (ObserverList::iterator it = observersToNotify.begin();
       it != observersToNotify.end();
       ++it) {
    (*it)->notify(pNotification);
  }
}

bool NotificationCenter::hasObservers() const {
  return !observers_.empty();
}

std::size_t NotificationCenter::countObservers() const {
  return observers_.size();
}

namespace {
  static SingletonHolder<NotificationCenter> sh;
}

NotificationCenter& NotificationCenter::defaultCenter() {
  return *sh.get();
}
