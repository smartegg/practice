// this class aims to dispatch events to events_hander
#ifndef INITIATION_DISPATCHER
#define INITIATION_DISPATCHER

#include <map>

enum EventType {
  kAcceptEvent = 01,
  kReadEvent = 02,
  kWriteEvent = 04,
  kTimeoutEvent = 010,
  kSignalEvnet = 020,
  kCloseEvent = 040
};

class TimeValue;
class EventHandler;



class InitiationDispatcher {
 public:
  //singleton pattern, just maintain one instance.
  static InitiationDispatcher* GetInstance();
  //Register an Event handler of  a particular EventType(kAcceptEvent ...etc.)
  int RegisterHandler(EventHandler* eh, EventType et);
  //remote an Event handler of a particular event
  int RemoveHandler(EventHandler* eh, EventType et);
  //entry into the event loop
  int HandleEvents(TimeValue* timeout = 0);
 private:
  static std::map<EventHandler*, EventType> handler_table_;
 
  //forbid all kinds of constructors and assignments.
  InitiationDispatcher();
  InitiationDispatcher(const InitiationDispatcher&);
  InitiationDispatcher& operator=(const InitiationDispatcher&);
  
};
#endif
