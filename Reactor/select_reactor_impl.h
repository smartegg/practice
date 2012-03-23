#ifndef SELECT_REACTOR_IMPL_H
#define SELECT_REACTOR_IMPL_H
#include <map>
#include "reactor.h"
#include <sys/select.h>
#include "handle_set.h"
#include "reactor_impl.h"

typedef  void (HandleSet::*Fds_Ptmf)(Handle);

//@brief track handles we are interested for some events.
class SelectReactorHandleSet {
 public:
  //read events(e.g. input pending, accept pending).
  HandleSet rd_mask_;
  //write events(e.g. flow control abated, non-blocking connection)
  HandleSet wr_mask_;
  //e.g. SIG_URG
  HandleSet ex_mask_;
};

class SelectReactorImpl;
class SelectReactorHandlerRepository {
 public:
  typedef std::map<Handle, EventHandler*> map_type;
  SelectReactorHandlerRepository(SelectReactorImpl&); 
  int Open(size_t size); 
  int Close();
  EventHandler* Find(Handle handle);
  int Bind(Handle handle, EventHandler* handler, EventType et);
  int UnBind(Handle handle, EventType mask);
  int UnBindAll();
  bool InvalidHandle(Handle handle);
  bool HandleInRange(Handle handle);
  size_t Size() const;
  size_t MaxHandleP1()const;

  
 private:
  int UnBind(Handle handle, map_type::iterator pos, EventType et);
  SelectReactorImpl& select_reactor_;
  Handle maxhandlep1_;
  std::map<Handle, EventHandler*> table_;
};

class SelectReactorImpl : public ReactorImpl {
 public:
  friend class SelectReactorHandlerRepository;
  enum {
    DEFAULT_SIZE = FD_SETSIZE
  };

  SelectReactorImpl();

 protected:
  virtual int BitOps(Handle handle, EventType mask,
                     SelectReactorHandleSet& handle_set,
                     int ops);
  virtual void ClearDispatchMask(Handle handle, EventType et);
  //true if state has changed during the dispatching of select reactor.
  bool state_changed_;
  //whether initialized.
  bool initialized_; 

  //track handles ready for dispatch from <select>
  SelectReactorHandleSet ready_set_;
  //track handles wait for by <select>
  SelectReactorHandleSet dispatche_set_;
  //track handles we are interested for various events that must be 
  //dispatched without going through <select>
  SelectReactorHandleSet wait_set_;
  SelectReactorHandlerRepository handler_repo_;
 private:
  SelectReactorImpl(const SelectReactorImpl&);
  SelectReactorImpl& operator=(const SelectReactorImpl&);

};
#endif
