#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
#include "initiation_dispatcher.h"

class Handle;
class EventHandler {
 public:
  //hook method that is called back by the InitiationDispatcher
  //to handle events
  virtual int HandleEvent(EventType et) = 0;
  //hook method that returns the I/O handle
  virtual Handle  GetHandle(void) const = 0;

};
#endif
