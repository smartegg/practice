#include "ServerSocket.h"

#include "ServerSocketImpl.h"
#include "Exception.h"

ServerSocket::ServerSocket() : Socket(new ServerSocketImpl) {

}

ServerSocket::ServerSocket(const Socket& socket) : Socket(socket) {
  if (!dynamic_cast<ServerSocketImpl*>(impl()))
    throw InvalidArgumentException("incompatible socket");
}

ServerSocket::ServerSocket(const SocketAddress& address, int backlog) : Socket(new ServerSocketImpl) {
  impl()->bind(address, true);
  impl()->listen(backlog);
}

ServerSocket::ServerSocket(uint16_t port, int backlog) : Socket(new ServerSocketImpl) {
  IPAddress wildcardAddress;
  SocketAddress address(wildcardAddress, port);
  impl()->bind(address, true);
  impl()->listen(backlog);
}

ServerSocket::ServerSocket(SocketImpl* pImpl, bool) : Socket(pImpl) {

}

ServerSocket::~ServerSocket() {

}

ServerSocket& ServerSocket::operator= (const ServerSocket& socket) {
  if (dynamic_cast<ServerSocketImpl*>(socket.impl()))
      Socket::operator=(socket);
  else
    throw InvalidArgumentException("incompatible socket");
  return *this;
}

void ServerSocket::bind(const SocketAddress& address, bool reuseAddress) {
  impl()->bind(address, reuseAddress);
}

void ServerSocket::bind(uint16_t port, bool reuseAddress) {
  IPAddress wildcardAddress;
  SocketAddress address(wildcardAddress, port);
  impl()->bind(address, reuseAddress);
}

void ServerSocket::listen(int backlog) {
  impl()->listen(backlog);
}

StreamSocket ServerSocket::acceptConnection(SocketAddress& clientAddress) {
  return StreamSocket(impl()->acceptConnection(clientAddress)); 
}

StreamSocket ServerSocket::acceptConnection() {
  SocketAddress clientAddr;
  return StreamSocket(impl()->acceptConnection(clientAddr));
}


