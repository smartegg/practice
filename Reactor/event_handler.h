#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
typedef int Handle;

enum  EventType{
    kNullMask = 0,
    kReadMask = (1<<0),
    kWriteMask = (1<<1),
    kExceptMask = (1<<2),
    kAcceptMask = (1 << 3),
    kConnectMask = (1<< 4),
    kTimerMask = ( 1<< 5),
    kSignalMask = (1 << 6),
    kAllEventsMask = kReadMask | kWriteMask | kExceptMask | kAcceptMask |
                     kConnectMask | kTimerMask | kSignalMask,
    kRWEMask = kReadMask | kWriteMask | kExceptMask ,
    kDontCall = (1 << 7)
};

class TimeValue;
class Reactor;

class EventHandler {
 public:
  virtual ~EventHandler();

  //getter/setter of the I/O handle
  virtual Handle  handle(void) const ;
  virtual void set_handle(Handle);

  //todo: get/set priority
  
  //hook method called when event handler removed from a <Reactor>
  //or handle_*() method return -1
  virtual int HandleClose(Handle, EventType);


  //hook method called when input become readiness
  virtual int HandleInput(Handle);
  //hook method called when output become ready
  virtual int HandleOutput(Handle);
  //hook method called when urgent data is available
  virtual int HandleExcept(Handle);

  //hook method called when timer expires, 
  //@param current_time  stores the current time when it is selected 
  //@param act  the argument given when the handle was scheduled originally.
  virtual int HandleTimeout(const TimeValue& current_time,
                            const void* act = 0);
  //called when signal is triggered.
  virtual int HandleSignal(int signum);

  //getter/setter of the @a Reactor
  virtual void set_reactor(Reactor* reactor);
  virtual Reactor* reactor() const; 
 protected:
  EventHandler(Reactor* r =0);
 private:
  Reactor* reactor_;
};
#endif
