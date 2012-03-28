#include "SocketNotifier.h"

#include "SocketNotification.h"
#include "SocketReactor.h"

SocketNotifier::SocketNotifier(const Socket& socket) 
  : socket_(socket){

}

SocketNotifier::~SocketNotifier() {

}

void SocketNotifier::addObserver(
    SocketReactor* pReactor,
    const AbstractObserver& observer) {
  nc_.addObserver(observer);
  if (observer.accepts(pReactor->pReadableNotification_)) {
    events_.insert(pReactor->pReadableNotification_.get());
  } else if (observer.accepts(
          pReactor->pWritableNotification_)) {
    events_.insert(pReactor->pWritableNotification_.get());
  } else if (observer.accepts(
          pReactor->pErrorNotification_)) {
    events_.insert(pReactor->pErrorNotification_);
  } else if (observer.accepts(
          pReactor->pTimeoutNotification_)) {
    events_.insert(pReactor->pTimeoutNotification_.get());
  }
}

void SocketNotifier::removeObserver(
    SocketReactor* pReactor,
    const AbstractObserver& observer) {
  nc_.removeObserver(observer);
  EventSet::iterator it = events_.end();

  if (observer.accepts(pReactor->pReadableNotification_)) {
    it = events_.find(pReactor->pReadableNotification_.get());
  } else if (observer.accepts(
          pReactor->pWritableNotification_)) {
    it = events_.find(pReactor->pWritableNotification_.get());
  } else if (observer.accepts(pReactor->pErrorNotification_)) {
    it = events_.find(pReactor->pErrorNotification_.get());
  } else if (observer.accepts(pReactor->pTimeoutNotification_)) {
    it = events_.find(pReactor->pTimeoutNotification_.get());
  }
  if (it != events_.end())
    events_.erase(it);

}

namespace {
  static Socket nullSocket;
}

void SocketNotifier::dispatch(SocketNotification* pNotification) {
  pNotification->setSocket(socket_);
  pNotification->duplicate();
  try {
    nc_.postNotification(pNotification);
  }catch(...) {
    pNotification->setSocket(nullSocket);
  }
  pNotification->setSocket(nullSocket);
}



