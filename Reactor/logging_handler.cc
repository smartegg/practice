#include "logging_handler.h"

LoggingHandler::LoggingHandler(SockStream& cs) : peer_stream_(cs){
  InitiationDispatcher::GetInstance()->RegisterHandler(this, kReadEvent);
}

void LoggingHandler::HandleEvent(EvnetType et) {
  if (et == kReadEvent) {
    LogRecord log_record;

    peer_stream_.recv((void*)log_record, sizeof log_record);
    log_record.write(STDOUT);
  } else if (et == kCloseEvent) {
    peer_stream_.close();
    delete (void*)this;
  }
}
