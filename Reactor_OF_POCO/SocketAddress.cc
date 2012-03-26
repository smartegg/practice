#include "SocketAddress.h"

#include "RefCountedObject.h"
#include "Exception.h"
#include <cstring>

struct AFLT {
  bool operator() (const IPAddress& a1, const IPAddress& a2) {
    return a1.af() < a2.af();
  }
};

class SocketAddressImpl : public RefCountedObject {
 public:
  virtual IPAddress host() const = 0;
  //in network byte order
  virtual uint16_t port() const = 0;
  virtual socklen_t length() const = 0;
  virtual const struct sockaddr* addr() const = 0;
  virtual int af() const = 0;

 protected:
  SocketAddressImpl() {
  }
  ~SocketAddressImpl() {
  }
 private:
  SocketAddressImpl(const SocketAddressImpl&);
  SocketAddressImpl& operator= (const SocketAddressImpl&);
};


class IPv4SocketAddressImpl : public SocketAddressImpl {
 public:
  IPv4SocketAddressImpl() {
    std::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
  }
  IPv4SocketAddressImpl(const struct sockaddr_in* addr) {
    std::memcpy(&addr_, addr, sizeof(addr_));
  }

  IPv4SocketAddressImpl(const void* addr, uint16_t port) {
    std::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    std::memcpy(&addr_.sin_family, addr, sizeof(addr_.sin_family));
    addr_.sin_port = port;
  }

  IPAddress host() const {
    return IPAddress(&addr_.sin_addr, sizeof(addr_.sin_family));
  }
  
  uint16_t port() const{
    return addr_.sin_port;
  }

  socklen_t length() const {
    return sizeof(addr_);
  }

  const struct sockaddr* addr() const{
    return reinterpret_cast<const struct sockaddr*>(&addr_);
  }

  int af() const{
    return addr_.sin_family;
  }


 private:
  struct sockaddr_in addr_;
};


SocketAddress::SocketAddress() {
  pImpl_ = new IPv4SocketAddressImpl();
}

SocketAddress::SocketAddress(const IPAddress& host, uint16_t port) {
  init(host, port);
}

SocketAddress::SocketAddress(const std::string& host, uint16_t port) {
  init(host, port);
}

SocketAddress::SocketAddress(const std::string& host,
                             const std::string& port) {
  init(host, resolveService(port));
}

SocketAddress::SocketAddress(const std::string hostAndport) {
  throw NotImplementedException("socketaddres()");
}

SocketAddress::SocketAddress(const SocketAddress& addr) {
  pImpl_ = addr.pImpl_;
  pImpl_->duplicate();
}

SocketAddress::SocketAddress(const struct sockaddr* addr,
                             socklen_t length) {
  if (length == sizeof(struct sockaddr_in)) {
    pImpl_ = new IPv4SocketAddressImpl(
        reinterpret_cast<const struct sockaddr_in*>(addr));
  } else {
    throw InvalidArgumentException("invalid addr type");
  }
}

SocketAddress::~SocketAddress() {
  pImpl_->release();
}

SocketAddress& SocketAddress::operator = (const SocketAddress& addr) {
  if (&addr != this) {
    pImpl_->release();
    pImpl_ = addr.pImpl_;
    pImpl_->duplicate();
  }

  return *this;
}

void SocketAddress::swap(SocketAddress& addr) {
  std::swap(pImpl_, addr.pImpl_);
}

IPAddress SocketAddress::host() const {
  return pImpl_->host();
}

uint16_t SocketAddress::port() const {
  return ntohs(pImpl_->port());
}

socklen_t SocketAddress::length() const {
  return pImpl_->length();
}

int SocketAddress::af() const {
  return pImpl_->af();
}

std::string SocketAddress::toString() const {
  std::string result;
  result.append(host().toString());

  char buf[64];
  std::sprintf(buf, "%u", port());
  result.append(": ");
  result.append(buf);

  return result;
}


void SocketAddress::init(const IPAddress& host, uint16_t port) {
  if (host.family() == IPAddress::IPv4) {
    pImpl_ = new IPv4SocketAddressImpl(host.addr(), htons(port));
  }
  else 
    throw InvalidArgumentException("init()");
}



void SocketAddress::init(const std::string& host, uint16_t port) {
  IPAddress ip;
  if (IPAddress::tryParse(host, ip)) {
    init(ip, port);
  } else {
    throw NotImplementedException("dns service not implemented");
  }
}

uint16_t SocketAddress::resolveService(const std::string& service) {
  uint32_t port;
  int r;
  char buf[64];

  r = std::sscanf(service.c_str(), "%u", &port);
  if (r == 1) {
    return port;
  } else {
    throw NotImplementedException("not implement port service lookup");
  }

}
