#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H

#include "IPAddress.h"
#include <stdint.h>
#include <string>

class SocketAddressImpl;

class SocketAddress {
 public:
  SocketAddress();
  SocketAddress(const IPAddress& host, uint16_t port);
  SocketAddress(const std::string& host, uint16_t port);
  SocketAddress(const std::string& host, const std::string& port);
  //host:port 
  explicit SocketAddress(const std::string hostAndport);

  SocketAddress(const SocketAddress& addr);
  SocketAddress(const struct sockaddr* addr, socklen_t length);
  ~SocketAddress();

  SocketAddress& operator= (const SocketAddress& addr);

  void swap(SocketAddress& addr);

  IPAddress host() const;
  uint16_t port() const;

  socklen_t length() const;

  const struct sockaddr* addr() const;

  int af() const;

  std::string toString() const;

  IPAddress::Family family() const;
  
  bool operator ==(const SocketAddress& addr) const;
  bool operator !=(const SocketAddress& addr) const;

  enum {
    MAX_ADDRESS_LEN = sizeof(struct in6_addr)
  };
 protected:
  void init(const IPAddress& host, uint16_t port);
  void init(const std::string& host, uint16_t port);
  uint16_t resolveService(const std::string& service);
 private:
  SocketAddressImpl* pImpl_;
};

inline void swap(SocketAddress& a1, SocketAddress& a2) {
  a1.swap(a2);
}

inline bool SocketAddress::operator== (const SocketAddress& addr) const {
  return host() == addr.host() && port() == addr.port();
}

inline bool SocketAddress::operator!=(const SocketAddress& addr) const {
  return host() != addr.host() || port() != addr.port();
}
#endif
