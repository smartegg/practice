#ifndef LOGGING_HANDLER_H
#define LOGGING_HANDLER_H
#include "event_handler.h"

class SockStream;
class LoggingHandler : public EventHandler {
 public:
  //initialize client sockstream
  LoggingHandler(SockStream& cs);
  //handle reception of clients records
  virtual int HandleEvent(EventType et);

  virtual Handle GetHandle(void) const {
    return peer_stream_.GetHandle();
  }
 private:
  SockStream peer_stream_;


};
#endif
