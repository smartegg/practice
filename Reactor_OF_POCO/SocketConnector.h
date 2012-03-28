#ifndef SOCKET_CONNECTOR_H
#define SOCKET_CONNECTOR_H
#include "StreamSocket.h"
#include "SocketReactor.h"
#include "Observer.h"

template <class ServiceHandler>
class SocketConnector {
 public:
  explicit SocketConnector(SocketAddress& address) : pReactor_(0) {
    socket_.connectNB(address);
  }

  SocketConnector(SocketAddress& address, SocketReactor& reactor) {
    socket_.connectNB(address);
    registerConnector(reactor);
  }

  virtual ~SocketConnector() {
    unregisterConnector();
  }

  virtual void registerConnector(SocketReactor& reactor) {
    pReactor_ = &reactor;
    pReactor_->addEventHandler(socket_,
                               Observer<SocketConnector, ReadableNotification>(*this, &SocketConnector::onReadable));
    pReactor_->addEventHandler(socket_,
                               Observer<SocketConnector, WritableNotification>(*this, &SocketConnector::onWritable));
    pReactor_->addEventHandler(socket_,
                               Observer<SocketConnector, ErrorNotification>(*this, &SocketConnector::onError));
  }

  virtual void unregisterConnector() {
    if (pReactor_) {
      pReactor_->removeEventHandler(socket_,
                                    Observer<SocketConnector, ReadableNotification>(*this, &SocketConnector::onReadable));
      pReactor_->removeEventHandler(socket_,
                                    Observer<SocketConnector, WritableNotification>(*this, &SocketConnector::onWritable));
      pReactor_->removeEventHandler(socket_,
                                    Observer<SocketConnector, ErrorNotification>(*this, &SocketConnector::onError));
    }
  }

  void onReadable(ReadableNotification* pNotification) {
    pNotification->release();
    int err = socket_.impl()->socketError();
    if (err) {
      onError(err);
      unregisterConnector();
    } else {
      onConnect();
    }
  }

  void onWritable(WritableNotification* pNotification) {
    pNotification->release();
    onConnect();
  }

  void onConnect() {
    socket_.setBlocking(true);
    createServiceHandler();
    unregisterConnector();
  }

  void onError(ErrorNotification* pNotification) {
    pNotification->release();
    onError(socket_.impl()->socketError());
    unregisterConnector();
  }

 protected:
  virtual ServiceHandler* createServiceHandler() {
    return new ServiceHandler(socket_, *pReactor_);
  }

  virtual void onError(int errCode) {
    
  }

  SocketReactor* reactor() {
    return pReactor_;
  }

  Socket& socket() {
    return socket_;
  }
  
 private:
  SocketConnector();
  SocketConnector(const SocketConnector&);
  SocketConnector& operator= (const SocketConnector&);

  StreamSocket socket_;
  SocketReactor* pReactor_;
};
#endif
