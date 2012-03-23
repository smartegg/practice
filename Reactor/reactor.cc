#include "reactor.h"
#include "reactor_impl.h"

Reactor::Reactor(ReactorImpl* impl, bool delete_implementation) 
  : implementation_(0),
    delete_implementation_(delete_implementation){
    this->set_implementation(impl);
    if (this->implementation() == 0) {
      impl = new SelectReactorImpl();
      this->set_implementation(impl);
      this->delete_implementation_ = true;
    }
}

Reactor::~Reactor() {
  this->implementation()->Close();
  if (this->delete_implementation_)
    delete this->implementation();
}

Reactor* Reactor::reactor_ = 0;
bool Reactor::delete_reactor_ = false;

Reactor* Reactor::GetInstance() {
  if (Reactor::reactor_ == 0) {
    Reactor::reactor_ = new Reactor();
    Reactor::delete_reactor_ = true;
  }
  return Reactor::reactor_;
}


void Reactor::CloseSingleton() {
  if (Reactor::delete_reactor_) {
    delete Reactor::reactor_;
    Reactor::reactor_ = 0;
    Reactor::delete_reactor_ = false;
  }
}

const char* Reactor::Name() {
  return "Reactor";
}

int Reactor::RunReactorEventLoop(ReactorEventHook eh) {
  while (1) {
    const int result = this->implementation_->HandleEvents();
    if (eh != 0 && (*eh)(this))
      continue;
    else if (result == -1 && this->implementation_->deactivated())
      return 0;
    else if (result == -1)
      return -1;
  }
  return 0;
}

int Reactor::RegisterHandler(EventHandler* event_handler, EventType et) {
  Reactor* old_reactor = event_handler->reactor();
  event_handler->set_reactor(this);

  int result = this->implementation()->RegisterHandler(event_handler, et);
  if (result == -1)
    event_handler->set_reactor(old_reactor);
  return result;
}


