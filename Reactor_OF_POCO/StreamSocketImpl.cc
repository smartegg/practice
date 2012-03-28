#include "StreamSocketImpl.h"

#include "IPAddress.h"
#include "Exception.h"

StreamSocketImpl::StreamSocketImpl() {
}

StreamSocketImpl::StreamSocketImpl(IPAddress::Family addressFamily) {
  if (addressFamily == IPAddress::IPv4) 
    init(AF_INET);
  else
    throw InvalidArgumentException("Invalid or unsupported  family");
}

StreamSocketImpl::StreamSocketImpl(int sockfd) : SocketImpl(sockfd) {

}

StreamSocketImpl::~StreamSocketImpl() {

}

int StreamSocketImpl::sendBytes(const void* buffer, int length,
                                int flags) {
  const char* p = reinterpret_cast<const char*>(buffer);
  int remaining = length;
  int sent = 0;
  bool blocking = getBlocking();
  while (remaining > 0) {
    int n = SocketImpl::sendBytes(p, remaining, flags);
    p += n;
    sent += n;
    remaining -= n;
// here we can yield other thread.
  }
  return sent;
}
