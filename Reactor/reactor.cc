#include "reactor.h"

Reactor* Reactor::GetInstance() {
  return 0;
}

int Reactor::RegisterHandler(EventHandler* eh, EventType et) {
  handler_table_[eh] = et; 
  return 0;
}

int Reactor::RemoveHandler(EventHandler* eh, EventType et) {
  int num = handler_table_.erase(eh);
  return num == 1;
}

int Reactor::HandleEvents(TimeValue* timeout) {
  return 0;
}
