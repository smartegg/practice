//@brief this is a naive implement of an epoll-blocking impl of echo server.

#include "Exception.h"
#include "StreamSocket.h"
#include "ServerSocket.h"
#include "SocketReactor.h"
#include "SocketAcceptor.h"
#include "SocketConnector.h"
#include "AutoPtr.h"
#include "ErrorHandler.h"

#include <iostream>
#include <string>
#include <cstring>

class TReactor : public SocketReactor {
 protected:
  void onTimeout()  {
    std::cout << "on Timeout \n" ;
    SocketReactor::onTimeout();
    sleep(1);
  }

  void onIdle() {
    std::cout << "\n on Idle no observers" << std::endl;
    SocketReactor::onIdle();
    sleep(1);
  }

  void onShutdown() {
    std::cout << "\n on shutdown " << std::endl;
    SocketReactor::onShutdown();
    sleep(1);
  }

  void onBusy() {
    std::cout << "\n on busy " << std::endl;
    SocketReactor::onBusy();
    sleep(1);
  }
};


//@brief this class contian the main routine of the echo task.
class ServiceHandler {
 public:
  void onReadable(ReadableNotification* pNotification) {
    AutoPtr<WritableNotification> ptr;
    StreamSocket& socket = static_cast<StreamSocket&>(ptr->socket());

    char buf[1024];

    socket.receiveBytes(buf, sizeof(buf));
    std::string str = std::string(buf);

    std::cout << "received " << str.size()  << "bytes" << std::endl;
    std::cout << std::string(buf) << std::endl;

    socket.close();

    pNotification->source().removeEventHandler(socket,
                            Observer<ServiceHandler, ReadableNotification>(*this, &ServiceHandler::onReadable));
  }
};


//@brief this is the event handler 
//occured when serversocket's accept() return 
// and return a connected socket.
// just register the connected socket to SocketReactor.
class AcceptorHandler {
 public:
  //@param socket  the connected socket return by accept
  //@param reactor  you can use this reactor to register other event handler.
  AcceptorHandler(StreamSocket& socket, SocketReactor& reactor) {
    ServiceHandler* handler = new ServiceHandler();

    reactor.addEventHandler(socket,
                            Observer<ServiceHandler, ReadableNotification>(*handler, &ServiceHandler::onReadable));
  }
};

int main() {
  try {
    //create  a socket , automatic bind and listen on port 9877
    ServerSocket serverSocket(9877);
    StreamSocket socket = serverSocket.acceptConnection();

    TReactor reactor;
    SocketAcceptor<AcceptorHandler> acceptor(serverSocket, reactor);
    
    reactor.run();
  }
  catch(Exception& exc) {
    ErrorHandler::handle(exc);
  }
  catch(std::exception& exc) {
    ErrorHandler::handle(exc);
  }
  catch(...) {
    ErrorHandler::handle();
  }
  return 0;
}
