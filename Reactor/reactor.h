// this class aims to dispatch events to events_hander
#ifndef REACTOR_H
#define REACTOR_H

#include <map>
#include <cstddef>
#include <sys/select.h>
#include "event_handler.h"
#include "reactor_impl.h"

class TimeValue;
class EventHandler;
class ReactorImpl;


class Reactor {
 public:
  Reactor(ReactorImpl* impl = 0, bool delete_impl = false);
  virtual ~Reactor();


  enum {
    kGetMask = 1,
    kSetMask = 2,
    kAddMask = 3, 
    kClrMask = 4
  };
  //called after each iteration of event handling.
  typedef int (*ReactorEventHook) (Reactor*);

  //singleton pattern, just maintain one instance.
  static Reactor* GetInstance();

  //delte the dynamic allocated singleton.
  static void CloseSingleton();

  //return the Reactor's Name
  static const char* Name();

  //initialize reactor to manage @a max_number_of_handles.
  int Open(size_t max_number_of_handles);

  //close down and release all resources.
  int Close();

  //return non-zero if there are I/O events "ready" for dispatching.
  //but does not actually dispatch the events and does not block.
  int WorkPending(const TimeValue& max_wait_time);

  // this event loop block for up to @a max_wait_time before returning.
  // if max_wait_time = 0, this function block indefinitely until 
  // events occur.
  int HandleEvents(TimeValue* max_wait_time = 0);
  //event loop
  int RunReactorEventLoop(ReactorEventHook hook= 0);

  //Register an Event handler of  a particular EventType(kAcceptEvent ...etc.)
  virtual int RegisterHandler(EventHandler* eh, EventType et);
  //remote an Event handler of a particular event
  virtual int RemoveHandler(EventHandler* eh, EventType et);

  //Get the implementation class
  ReactorImpl* implementation()const;
 protected:
  void set_implementation(ReactorImpl* impl);
  ReactorImpl* implementation_;
  bool delete_implementation_;
  static Reactor* reactor_;
  static bool delete_reactor_;

  Reactor(const Reactor&);
  Reactor& operator=(const Reactor&);
  
};
#endif
