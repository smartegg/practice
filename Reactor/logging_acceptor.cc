#include "logging_acceptor.h"
#include "initiation_dispatcher.h"

LoggingAcceptor::LoggingAcceptor(const InetAddr& addr) : acceptor_(addr) {
    InitiationDispatcher::GetInstance()->RegisterHandler(this, kAcceptEvent);
}

void LoggingAcceptor::handle_event (EventType et) {
  assert(et == kAcceptEvent);
  SockStream new_connection;
  acceptor_.accept(new_connection);
  LoggingHandler* handler = new LoggingHandler(new_connection);
}
					
