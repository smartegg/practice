#include "StreamSocket.h"

#include "StreamSocketImpl.h"
#include "Exception.h"

StreamSocket::StreamSocket() : Socket(new StreamSocketImpl) {

}

StreamSocket::StreamSocket(const SocketAddress& address)
  : Socket(new StreamSocketImpl) {
  connect(address);
}

StreamSocket::StreamSocket(IPAddress::Family family) 
  : Socket(new StreamSocketImpl(family)) {

}

StreamSocket::StreamSocket(const StreamSocket& socket) 
  : Socket(socket) {
  if (!dynamic_cast<StreamSocketImpl*>(impl())) {
    throw InvalidArgumentException("incompatible socket");
  }
}

StreamSocket::StreamSocket(SocketImpl* pImpl) 
  : Socket(pImpl) {
  if (!dynamic_cast<StreamSocketImpl*>(impl())) {
    throw InvalidArgumentException("incompatible socket");
  }
}

StreamSocket::~StreamSocket() {

}

StreamSocket& StreamSocket::operator= (
    const StreamSocket& socket) {
  if (dynamic_cast<StreamSocketImpl*>(socket.impl()))
    Socket::operator=(socket);
  else
    throw InvalidArgumentException("incompatible socket");
  return *this;
}

void StreamSocket::connect(const SocketAddress& address) {
  impl()->connect(address);
}

void StreamSocket::connect(const SocketAddress& address,
                           const Timespan& timeout) {
  impl()->connect(address, timeout);
}
void StreamSocket::connectNB(const SocketAddress& address) {
  impl()->connectNB(address);
}

void StreamSocket::shutdownReceive() {
  impl()->shutdownReceive();
}

void StreamSocket::shutdownSend() {
  impl()->shutdownSend();
}

void StreamSocket::shutdown() {
  impl()->shutdown();
}

int StreamSocket::sendBytes(const void* buffer,
                            int length,
                            int flags) {
  return impl()->sendBytes(buffer, length, flags);
}

int StreamSocket::receiveBytes(void* buffer,
                               int length,
                               int flags) {
  return impl()->receiveBytes(buffer,length,flags);
}


void StreamSocket::sendUrgent(unsigned char data) {
  impl()->sendUrgent(data);
}


  


                        
