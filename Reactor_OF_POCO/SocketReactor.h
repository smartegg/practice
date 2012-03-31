#ifndef SOCKET_REACTOR_H
#define SOCKET_REACTOR_H

#include "SocketNotification.h"
#include "Timespan.h"
#include "SocketNotifier.h"
#include <map>

class Socket;
class AbstractObserver;


//@brief the reactor design pattern
class SocketReactor {
 public:
  SocketReactor();
  explicit SocketReactor(const Timespan& timeout);
  virtual ~SocketReactor();

  void run();
  void stop();

  //if no event occur in the given timespan, 
  //a timeout event is sent to all event listeners.
  //the default timeout is 250 milliseconds.
  void setTimeout(const Timespan& timeout);
  
  const Timespan& getTimeout() const;

  //@brief
  //  Observer<MyEventHandler, SocketNotification> obs(*this,
  //  &MyEventHandler::handlMyEvent);
  //  reactor.addEventHandler(socket, obs);
  void addEventHandler(const Socket& socket, 
                       const AbstractObserver& observer);

  
  //@brief
  // Observer<MyEventHandler, SocketNotification> obs(*this,
  //  &MyEventHandler::handleMyEvent);
  //  reactor.removeEventHandler(socket, observer);
  void removeEventHandler(const Socket& socket,
                          const AbstractObserver& observer);
  
 protected:
  //called if timeout and no other events are available.
  //default impl:  dispatch TimeoutNotification to all handler.
  virtual void onTimeout();

  //called if no sockets available.
  //default impl : dispatch the IdleNotification
  virtual void onIdle();

  //called if the socketReactor is about to shutdown.
  virtual void onShutdown();
  //called when the reactor is busy and at leat one notification 
  //has been dispatched.
  virtual void onBusy();
  //dispatch the given notification to all observers registered for
  //the given socket
  void dispatch(const Socket& socket, SocketNotification* pNotification);

  void dispatch(SocketNotification* pNotification);

 private:
  typedef AutoPtr<SocketNotifier> NotifierPtr;
  typedef AutoPtr<SocketNotification> NotificationPtr;
  typedef std::map<Socket, NotifierPtr> EventHandlerMap;

  void dispatch(NotifierPtr& pNotifier, SocketNotification* pNotification);

  enum {
    DEFAULT_TIMEOUT = 1000000//250000
  };

  bool stop_;
  Timespan timeout_;
  EventHandlerMap handlers_;
  NotificationPtr pReadableNotification_;
  NotificationPtr pWritableNotification_;
  NotificationPtr pErrorNotification_;
  NotificationPtr pTimeoutNotification_;
  NotificationPtr pIdleNotification_;
  NotificationPtr pShutdownNotification_;

  friend class SocketNotifier;

};
#endif
