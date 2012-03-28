#include "SocketReactor.h"

#include "Socket.h"
#include "AbstractObserver.h"
#include "ErrorHandler.h"

SocketReactor::SocketReactor() 
  : stop_(false),
    timeout_(DEFAULT_TIMEOUT),
    pReadableNotification_(new ReadableNotification(this)),
    pWritableNotification_(new WritableNotification(this)),
    pErrorNotification_(new ErrorNotification(this)),
    pTimeoutNotification_(new TimeoutNotification(this)),
    pIdleNotification_(new IdleNotification(this)),
    pShutdownNotification_(new ShutdownNotification(this)) {

}

SocketReactor::SocketReactor(const Timespan& timeout) 
  : stop_(false),
    timeout_(timeout),
    pReadableNotification_(new ReadableNotification(this)),
    pWritableNotification_(new WritableNotification(this)),
    pErrorNotification_(new ErrorNotification(this)),
    pTimeoutNotification_(new TimeoutNotification(this)),
    pIdleNotification_(new IdleNotification(this)),
    pShutdownNotification_(new ShutdownNotification(this)) {

}

SocketReactor::~SocketReactor() {

}

void SocketReactor::stop() {
  stop_ = true;
}

void SocketReactor::run() {
  Socket::SocketList readable;
  Socket::SocketList writable;
  Socket::SocketList except;

  while(!stop_) {
    try {
      readable.clear();
      writable.clear();
      except.clear();

      int nSockets = 0;

      for (EventHandlerMap::iterator it = handlers_.begin();
           it != handlers_.end();
           ++it) {
        if (it->second->accepts(pReadableNotification_)) {
          readable.push_back(it->first); 
          nSockets++;
        }

        if (it->second->accepts(pWritableNotification_)) {
          writable.push_back(it->first);
          nSockets++;
        }

        if (it->second->accepts(pErrorNotification_)) {
          except.push_back(it->first);
          nSockets++;
        }
      }
      
      if (nSockets == 0) {
        onIdle();
      } else if (Socket::select(readable, writable,
                                except, timeout_)) {
        onBusy();
        for (Socket::SocketList::iterator it = readable.begin();
            it != readable.end();
            ++it) {
          dispatch(*it, pReadableNotification_);
        }

        for (Socket::SocketList::iterator it = writable.begin();
            it != writable.end();
            ++it) {
          dispatch(*it, pWritableNotification_);
        }

        for (Socket::SocketList::iterator it = except.begin();
             it != except.end();
             ++it) {
          dispatch(*it, pErrorNotification_);
        }
      } else {
        onTimeout();
      }
    }catch (Exception& exc) {
      ErrorHandler::handle(exc);
    }catch (std::exception& exc) {
      ErrorHandler::handle(exc);
    }catch(...) {
      ErrorHandler::handle();
    }
  }
  onShutdown();
}

void SocketReactor::setTimeout(const Timespan& timeout) {
  timeout_ = timeout;
}

const Timespan& SocketReactor::getTimeout() const {
  return timeout_;
}

void SocketReactor::addEventHandler(
    const Socket& socket,
    const AbstractObserver&  observer) {
  NotifierPtr pNotifier;

  EventHandlerMap::iterator it = handlers_.find(socket);
  
  if (it == handlers_.end()) {
    pNotifier = new SocketNotifier(socket);
    handlers_[socket] = pNotifier;  
  } else {
    it->second = pNotifier;
  }
  pNotifier->addObserver(this, observer);
}

void SocketReactor::removeEventHandler(
    const Socket& socket,
    const AbstractObserver& observer) {
  NotifierPtr pNotifier;

  EventHandlerMap::iterator it = handlers_.find(socket);

  if (it != handlers_.end()) {
    pNotifier = it->second;

    if (pNotifier->countObservers() == 1) {
      handlers_.erase(it); 
    }
  }
  
  if (pNotifier) {
    pNotifier->removeObserver(this, observer);
  }
}

void SocketReactor::onTimeout() {
  dispatch(pTimeoutNotification_);
}

void SocketReactor::onIdle() {
  dispatch(pIdleNotification_);
}

void SocketReactor::onShutdown() {
  dispatch(pShutdownNotification_);
}

void SocketReactor::onBusy() {

}

void SocketReactor::dispatch(const Socket& socket,
                             SocketNotification* pNotification) {
  NotifierPtr pNotifier;

  EventHandlerMap::iterator it = handlers_.find(socket);

  if (it != handlers_.end())
    pNotifier = it->second;
  else
    return;
  dispatch(pNotifier, pNotification);
}

void SocketReactor::dispatch(SocketNotification* pNotification) {
  std::vector<NotifierPtr> delegates;
  delegates.reserve(handlers_.size());

  for (EventHandlerMap::iterator it = handlers_.begin();
       it != handlers_.end();
       ++it) {
    delegates.push_back(it->second);
  }

  for (std::vector<NotifierPtr>::iterator it = delegates.begin();
       it != delegates.end();
       ++it) {
    dispatch(*it, pNotification);
  }
}

void SocketReactor::dispatch(NotifierPtr& pNotifier,
                             SocketNotification* pNotification) {
  try {
    pNotifier->dispatch(pNotification);
  } 
  catch (Exception& exc) {
    ErrorHandler::handle(exc);
  }
  catch (std::exception& exc) {
    ErrorHandler::handle(exc);
  }
  catch (...) {
    ErrorHandler::handle();
  }
}

