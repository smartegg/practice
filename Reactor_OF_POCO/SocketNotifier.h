#ifndef SOCKET_NOTIFIER_H
#define SOCKET_NOTIFIER_H
#include <set>
#include "RefCountedObject.h"
#include "AbstractObserver.h"
#include "NotificationCenter.h"
#include "Socket.h"

class Socket;
class SocketReactor;
class SocketNotification;

class SocketNotifier : public RefCountedObject {
 public:
  explicit SocketNotifier(const Socket& socket);
  void addObserver(SocketReactor* pReactor, const AbstractObserver& observer);
  void removeObserver(SocketReactor* pReactor, const AbstractObserver& observer);
  //check whethe there has any observers registed to this notification. 
  bool accepts(SocketNotification* pNotification);
  void dispatch(SocketNotification* pNotification);

  bool hasObservers() const;
  std::size_t countObservers() const;

 protected:
  ~SocketNotifier();
 private:
  typedef std::multiset<SocketNotification*> EventSet;

  EventSet events_;
  NotificationCenter nc_;
  Socket socket_;
};

bool SocketNotifier::accepts(SocketNotification* pNotification) {
  return events_.find(pNotification) != events_.end();
}

bool SocketNotifier::hasObservers() const {
  return nc_.hasObservers();
}

std::size_t SocketNotifier::countObservers() const {
  return nc_.countObservers();
}

#endif
