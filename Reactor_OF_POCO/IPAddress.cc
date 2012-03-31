#include "IPAddress.h"

#include "RefCountedObject.h"
#include "Exception.h"
#include <stdint.h>
#include <cstring>

class IPAddressImpl : public RefCountedObject {
 public:
  virtual std::string toString() const = 0;
  virtual socklen_t length() const = 0;
  virtual const void* addr() const = 0;
  virtual IPAddress::Family family() const = 0;
  virtual int af() const = 0;
  virtual bool isWildCard() const = 0;
  virtual bool isBroadCast() const = 0;
  virtual bool isLoopback() const = 0;
  virtual bool isMulticast() const = 0;
  
  virtual IPAddressImpl* clone() const = 0;

 protected:
  IPAddressImpl() {

  }

  virtual ~IPAddressImpl() {
  }

 private:
  IPAddressImpl(const IPAddressImpl&);
  IPAddressImpl& operator= (const IPAddressImpl&);
};


class IPv4AddressImpl : public IPAddressImpl {
 public:
  IPv4AddressImpl() {
    std::memset(&addr_, 0, sizeof(addr_));
  }

  IPv4AddressImpl(const void* addr) {
    std::memcpy(&addr_, addr, sizeof(addr_));
  }

  std::string toString() const {
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&addr_);
    std::string result;
    result.reserve(16);

    char buf[64];
    memset(buf, 0, sizeof buf);
    if (inet_ntop(AF_INET, &addr_, buf, 64))  {
      result.append(buf);
    } else {
      result.append("parsing error");
    }
    return result;
  }

  socklen_t length() const {
    return sizeof(addr_);
  }

  const void* addr() const {
    return &addr_;
  }

  IPAddress::Family family() const {
    return IPAddress::IPv4;
  }

  int af() const {
    return AF_INET;
  }

  bool isWildCard() const {
    return addr_.s_addr == INADDR_ANY;  
  }

  bool isBroadCast() const {
    return addr_.s_addr == INADDR_NONE;
  }

  bool isLoopback() const {
    // 127.0.0.1 to 127.255.255.255
    return (ntohl(addr_.s_addr) & 0xFF000000) == 0x7F000000; 
  }

  bool isMulticast() const {
    // 224.0.0.0/24 to 239.0.0.0/24 
    return (ntohl(addr_.s_addr) & 0xF0000000) == 0xE0000000; 
  }

  static IPv4AddressImpl* parse(const std::string& addr) {
    if (addr.empty())
      return 0;
    struct in_addr ia;
    if (inet_pton(AF_INET, addr.c_str(), &ia))
      return new IPv4AddressImpl(&ia);
    else 
      return 0;
  }

  IPAddressImpl* clone() const {
    return new IPv4AddressImpl(&addr_);
  }
 private:
  struct in_addr addr_;

};

IPAddress::IPAddress() : pImpl_(new IPv4AddressImpl) {

}

IPAddress::IPAddress(const IPAddress& addr) : pImpl_(addr.pImpl_) {

}

IPAddress::IPAddress(Family family) {
  if (family == IPv4) {
    pImpl_ = new IPv4AddressImpl();
  } else {
    throw InvalidArgumentException("Invalid or unsupported address family "
                                   "passed to IPaddress()");
  }
}

IPAddress::IPAddress(const std::string& addr) {
  pImpl_ = IPv4AddressImpl::parse(addr);
  if (!pImpl_)
    throw InvalidArgumentException("Invalid address string"
                                   "passed to IPaddress()");
}

IPAddress::IPAddress(const void* addr, socklen_t length) {
  if (length == sizeof(struct in_addr))
    pImpl_ = new IPv4AddressImpl(addr);
  else
    throw InvalidArgumentException("Invalid address length "
                                   "passed to IPAddress()");

}

IPAddress::~IPAddress() {
  pImpl_->release();
}

IPAddress& IPAddress::operator= (const IPAddress& addr) {
  if (&addr != this) {
    pImpl_->release();
    pImpl_ = addr.pImpl_;
    pImpl_->duplicate();
  }
  return *this;
}

void IPAddress::swap(IPAddress& address) {
  std::swap(pImpl_, address.pImpl_);
}

IPAddress::Family IPAddress::family() const {
  return pImpl_->family();
}

std::string IPAddress::toString() const {
  return pImpl_->toString();
}

bool IPAddress::isWildCard()const {
  return pImpl_->isWildCard();
}

bool IPAddress::isBroadCast() const {
  return pImpl_->isBroadCast();
}

bool IPAddress::isLoopback() const {
  return pImpl_->isLoopback();
}

bool IPAddress::isMulticast() const {
  return pImpl_->isMulticast();
}

bool IPAddress::isUnicast() const {
  return !isWildCard() && !isMulticast() && !isMulticast();
}

bool IPAddress::operator== (const IPAddress& a) const {
  socklen_t l1 = length();
  socklen_t l2 = a.length();
  if (l1 == l2)
    return std::memcmp(addr(), a.addr(), l1) == 0;
  else 
    return false;
}

bool IPAddress::operator!= (const IPAddress& a) const {
  socklen_t l1 = length();
  socklen_t l2 = a.length();
  if (l1 == l2)
    return std::memcmp(addr(), a.addr(), l1) != 0;
  else 
    return true;
}

bool IPAddress::operator< (const IPAddress& a) const {
  socklen_t l1 = length();
  socklen_t l2 = a.length();
  if (l1 == l2)
    return std::memcmp(addr(), a.addr(), l1) < 0;
  else 
    return l1 < l2;
}

bool IPAddress::operator<= (const IPAddress& a) const {
  socklen_t l1 = length();
  socklen_t l2 = a.length();
  if (l1 == l2)
    return std::memcmp(addr(), a.addr(), l1) <= 0;
  else 
    return l1 <= l2;
}
bool IPAddress::operator> (const IPAddress& a) const {
  socklen_t l1 = length();
  socklen_t l2 = a.length();
  if (l1 == l2)
    return std::memcmp(addr(), a.addr(), l1) > 0;
  else 
    return l1 > l2;
}

bool IPAddress::operator>= (const IPAddress& a) const {
  socklen_t l1 = length();
  socklen_t l2 = a.length();
  if (l1 == l2)
    return std::memcmp(addr(), a.addr(), l1) >= 0;
  else 
    return l1 >= l2;
}

socklen_t IPAddress::length() const {
  return pImpl_->length();
}

const void* IPAddress::addr() const {
  return pImpl_->addr();
}

int IPAddress::af() const {
  return pImpl_->af();
}

void IPAddress::init(IPAddressImpl* pImpl) {
  pImpl_->release();
  pImpl_ = pImpl;
}

IPAddress IPAddress::parse(const std::string& addr) {
  return IPAddress(addr);
}

bool IPAddress::tryParse(const std::string& addr, IPAddress& result) {
  IPAddressImpl* pImpl = IPv4AddressImpl::parse(addr);
  if (pImpl) {
    result.init(pImpl);
    return true;
  }
  else
    return false;

}

IPAddress IPAddress::wildcard(Family family) {
  return IPAddress(family);
}

IPAddress IPAddress::broadcast() {
  struct in_addr ia;
  ia.s_addr = INADDR_NONE;
  return IPAddress(&ia, sizeof(ia));
}

