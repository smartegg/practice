#include "SocketNotifier.h"

#include "SocketNotification.h"

SocketNotifier::SocketNotifier(const Socket& socket) 
  : socket_(socket){

}

SocketNotifier::~SocketNotifier() {

}

void SocketNotifier::addObserver(SocketReactor* pReactor, const AbstractObserver& observer) {

}

void SocketNotifier::removeObserver(SocketReactor* pReactor, const AbstractObserver& observer) {

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



