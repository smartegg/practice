// this class aims to dispatch events to events_hander
#ifndef REACTOR_H
#define REACTOR_H

#include <map>
#include <sys/select.h>
#include "event_handler.h"

class TimeValue;
class EventHandler;



class Reactor {
 public:
  enum {
    DEFAULT_SIZE = FD_SETSIZE
  };
  //singleton pattern, just maintain one instance.
  static Reactor* GetInstance();
  Reactor(int size, int restart = 0);
  virtual ~Reactor();
  virtual int open(int size = DEFAULT_SIZE, int restart = 0);
  void close();
  //Register an Event handler of  a particular EventType(kAcceptEvent ...etc.)
  virtual int RegisterHandler(EventHandler* eh, EventType et);
  //remote an Event handler of a particular event
  virtual int RemoveHandler(EventHandler* eh, EventType et);

  virtual long schedule_timer(EventHandler* handler,
                              const void* act,
                              const TimeValue& delta,
                              const TimeValue& interval);
  virtual void cancel_timer(EventHandler* eh);
  virtual void cancel_timer(long timer_id, const void** arg = 0);

  //entry into the event loop
  int HandleEvents(TimeValue* timeout = 0);
 private:
  static std::map<EventHandler*, EventType> handler_table_;
   
  
};
#endif
