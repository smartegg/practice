#ifndef LOGGING_ACCEPTOR_H
#define LOGGING_ACCEPTOR_H
#include "event_handler.h"
class SockAcceptor;

class LoggingAcceptor : public EventHandler {
 public:
  LoggingAcceptor(const InetAddr& addr);
  virtual void HandleEvent(EventType et);
  virtual Handle GetHandle(void) const {
    return  acceptor_.GetHandle();
  }
 private:
  SockAcceptor acceptor_;

};
#endif
