#ifndef REACTOR_IMPL_H
#define REACTOR_IMPL_H
#include <cstddef>
#include "event_handler.h"
class TimeValue;
class ReactorImpl {
 public:
  virtual ~ReactorImpl();

  virtual int Open(size_t size) = 0;
  virtual int Close() = 0;
  virtual int WorkPending(const TimeValue& max_wait_time) = 0;
  virtual int HandleEvents(TimeValue* max_wait_time = 0) = 0; 
  //return the status of the reactor
  //return 0 if the reactor is actively handing events, or 
  //<HandleEvents> return -1
  virtual int deactivated() = 0;
  //control whether the Reactor will handle any more incoming events or not. 
  virtual int deactivate(int do_stop) = 0;
  
  virtual int RegisterHandler(EventHandler* event_handler,
                              EventType et) = 0;
  virtual int RemoveHandler(EventHandler* event_handler,
                            EventType et) = 0;

  //whether the reactor is initialized
  virtual bool Initialized() = 0;
  virtual size_t Size() const;

};
#endif //END OF REACTOR_IMPL
