#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
//#include "reactor.h"

typedef int Handle;

enum  EventType{
    kReadMask = (1<<0),
    kWriteMask = (1 << 1),
    kExceptMask = (1 << 2),
    kAcceptMask = (1 << 3),
    kConnectMask = (1<< 4),
    kTimerMask = ( 1<< 5),
    kSignalMask = (1 << 6),
    kDcontCall = (1 << 7)
};

class TimeValue;
class EventHandler {
 public:

  //below all hook methods for derieve class to implement. 

  //hook method that returns the I/O handle
  virtual Handle  GetHandle(void) const = 0;
  //hook method called when event handler removed from a <Reactor>
  virtual int HandleClose(Handle, EventType) = 0;


  //hook method called when input become readiness
  virtual int HandleInput(Handle) = 0;
  //hook method called when output become ready
  virtual int HandleOutput(Handle) = 0;
  //hook method called when urgent data is available
  virtual int HandleExcept(Handle) = 0;
  //hook method called when timer expires, 
  //@param tv  stores the current time 
  //@param act  the argument given when the handle was scheduled originally.
  virtual int HandleTimeout(const TimeValue& tv,
                            const void* act = 0) = 0;
  //called when signal is triggered.
  virtual int HandleSignal(int signum) = 0;

};
#endif
