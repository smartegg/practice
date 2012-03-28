#ifndef  STREAM_SOCKET_IMPL_H
#define  STREAM_SOCKET_IMPL_H
#include "SocketImpl.h"

class StreamSocketImpl : public SocketImpl{
 public :
  StreamSocketImpl();
  explicit StreamSocketImpl(IPAddress::Family addressFamily);
  StreamSocketImpl(int sockfd);
  virtual int sendBytes(const void* buffer, int length, int flags = 0);
 protected:
  virtual ~StreamSocketImpl();
};
#endif
