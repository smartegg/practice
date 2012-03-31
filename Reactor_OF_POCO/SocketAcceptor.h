#ifndef SOCKET_ACCEPTOR_H
#define SOCKET_ACCEPTOR_H

#include "ServerSocket.h"
#include "SocketReactor.h"
#include "Observer.h"

//@param ServiceHandler must provide a constructor 
//    MyClass(const StreamSocket& socket, ServiceReactor& reactor)
template <class ServiceHandler>
class SocketAcceptor {
 public:
  SocketAcceptor(ServerSocket& socket) 
      : socket_(socket),
        pReactor_(0) {
  }

  SocketAcceptor(ServerSocket& socket, 
                 SocketReactor& reactor) 
      : socket_(socket),
        pReactor_(0) {

    registerAcceptor(reactor);
  }
  
  virtual ~SocketAcceptor() {
    unregisterAcceptor();
  }
  
  virtual void registerAcceptor(SocketReactor& reactor) {
    pReactor_ = &reactor;
    pReactor_->addEventHandler(socket_,
                              Observer<SocketAcceptor, ReadableNotification>(*this, &SocketAcceptor::onAccept));

  }

  virtual void unregisterAcceptor() {
    if (pReactor_) {
      pReactor_->removeEventHandler(socket_,
                                    Observer<SocketAcceptor, ReadableNotification>(*this, &SocketAcceptor::onAccept));
    }
  }

  void onAccept(ReadableNotification* pNotification) {
    pNotification->release();
    StreamSocket sock = socket_.acceptConnection();
    createServiceHandler(sock);
  }
  
 protected:
  virtual ServiceHandler* createServiceHandler(StreamSocket& socket) {
    return new ServiceHandler(socket, *pReactor_);//just delete in user code!!!!!!
  }

  SocketReactor* reactor() {
    return pReactor_;
  }

  Socket& socket() {
    return socket_;
  }

 private:
  SocketAcceptor();
  SocketAcceptor(const SocketAcceptor&);
  SocketAcceptor& operator= (const SocketAcceptor&);

  ServerSocket socket_;
  SocketReactor* pReactor_;
};
#endif
