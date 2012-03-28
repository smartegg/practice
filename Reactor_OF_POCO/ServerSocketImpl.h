#ifndef SERVER_SOCKET_IMPL_H
#define SERVER_SOCKET_IMPL_H

#include "SocketImpl.h"

class ServerSocketImpl : public SocketImpl{
 public:
  ServerSocketImpl();
 protected:
  virtual ~ServerSocketImpl();
};
#endif
