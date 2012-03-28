#ifndef SERVER_SOCKET_H
#define SERVED_SOCKET_H
#include "Socket.h"
#include "StreamSocket.h"

class ServerSocket : public Socket{
 public:
  ServerSocket();
  ServerSocket(const Socket& socket);
  ServerSocket(const SocketAddress& address, int backlog = 64);
  ServerSocket(uint16_t port, int backlog = 64);

  virtual ~ServerSocket();
  
  ServerSocket& operator = (const ServerSocket& socket);

  virtual void bind(const SocketAddress& address, bool reuseAddress = false);

  virtual void bind(uint16_t port, bool reuseAddress = false);

  virtual void listen(int backlog = 64);

  virtual StreamSocket acceptConnection(SocketAddress& clientAddr);

  virtual StreamSocket acceptConnection();
protected:
  //bool arg is useless.
  ServerSocket(SocketImpl* pImpl, bool);
};
#endif
