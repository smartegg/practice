#include "initiation_dispatcher.h"

InitiationDispatcher* InitiationDispatcher::GetInstance() {
  return 0;
}

int InitiationDispatcher::RegisterHandler(EventHandler* eh, EventType et) {
  handler_table_[eh] = et; 
  return 0;
}

int InitiationDispatcher::RemoveHandler(EventHandler* eh, EventType et) {
  int num = handler_table_.erase(eh);
  return num == 1;
}

int InitiationDispatcher::HandleEvents(TimeValue* timeout) {
  return 0;
}
