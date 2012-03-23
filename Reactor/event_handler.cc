#include "event_handler.h"

EventHandler::EventHandler(Reactor* r) : reactor_(r){
}

EventHandler::~EventHandler() {
}

Handle EventHandler::handle() const{
  return -1; 
}

void EventHandler::set_handle(Handle handle) {
  //do nothing
}

int EventHandler::HandleClose(Handle handle, EventType mask) {
  return -1;
}

int EventHandler::HandleInput(Handle handle) {
  return -1;
}

int EventHandler::HandleOutput(Handle handle) {
  return -1;
}

int EventHandler::HandleExcept(Handle handle) {
  return -1;
}


int EventHandler::HandleTimeout(const TimeValue& curr_time,
                                const void* act) {
  return -1;
}

int EventHandler::HandleSignal(int signum) {
  return -1;
}


void EventHandler::set_reactor(Reactor* reactor) {
  reactor_ = reactor;
}

Reactor* EventHandler::reactor() const {
  return reactor_;
}
