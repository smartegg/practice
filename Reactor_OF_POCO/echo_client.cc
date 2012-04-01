//@brief this is a naive implement of an epoll-blocking impl of echo client.

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

class TReactor : public SocketReactor {
 protected:
  void onTimeout()  {
    std::cout << "on Timeout\n" ;
    SocketReactor::onTimeout();
    sleep(1);
  }

  void onIdle() {
    std::cout << "on Idle\n" << std::endl;
    SocketReactor::onIdle();
    sleep(1);
  }

  void onShutdown() {
    std::cout << "\n on shutdown\n" << std::endl;
    SocketReactor::onShutdown();
    sleep(1);
  }

  void onBusy() {
    std::cout << "\n on busy\n" << std::endl;
    SocketReactor::onBusy();
    sleep(1);
  }
};



//@brief this class contian the main routine of the echo client task.
class ServiceHandler {
 public:
  //connect 
  ServiceHandler(StreamSocket& socket, SocketReactor& reactor) {
    std::string str_info = "hello world\n";
    socket.sendBytes(str_info.c_str(), str_info.size());
    std::cout << "send successfully " << std::endl;
    socket.shutdownSend();
    delete this;
  }
};


int main() {
  try {
    //create  a local socket , automatic bind and listen on port 9877
    IPAddress localAddress("127.0.0.1");
    SocketAddress address(localAddress, 9877);
    TReactor reactor;
    SocketConnector<ServiceHandler> connector(address, reactor);
    
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
