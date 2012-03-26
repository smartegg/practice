#ifndef IPADDRESS_H
#define IPADDRESS_H
#include <string>
#include <arpa/inet.h>
class IPAddressImpl;

class IPAddress {
 public:
  enum Family {
    IPv4,
    IPv6
  };
  
  //create a wildcard IPv4 address
  IPAddress();
  //copy ctor
  IPAddress(const IPAddress& addr);
  //create a wildcard address by family
  explicit IPAddress(Family family);
  //create by string
  explicit IPAddress(const std::string& addr);
  IPAddress(const std::string& addr, Family family);
  //create by a pointer to in_addr, in6_addr address
  IPAddress(const void* addr, socklen_t length);

  ~IPAddress();

  IPAddress& operator= (const IPAddress& addr);
  void swap(IPAddress& addr);

  Family family() const;

  std::string toString() const;

  bool isWildCard() const;
  bool isBroadCast() const;
  bool isLoopback() const;
  bool isMulticast() const;
  bool isUnicast() const;


  bool operator == (const IPAddress& addr) const;
  bool operator != (const IPAddress& addr) const;
  bool operator <  (const IPAddress& addr) const;
  bool operator <= (const IPAddress& addr) const;
  bool operator >  (const IPAddress& addr) const;
  bool operator >= (const IPAddress& addr) const;

  //return the length of the netaddr.
  socklen_t length() const;
  //return the addr structure;
  const void* addr() const;
  //return AF_INET, AF_INET6
  int af() const;

  static IPAddress parse(const std::string& addr);
  static bool tryParse(const std::string& addr, IPAddress& result);
  static IPAddress wildcard(Family family = IPv4);
  static IPAddress broadcast();
  enum {
    MAX_ADDRESS_LEN = sizeof(struct in6_addr)
  };
 protected:
  void init(IPAddressImpl* pImpl);
 private:
  IPAddressImpl* pImpl_;
};

inline void swap(IPAddress& addr1, IPAddress& addr2) {
  addr1.swap(addr2);
}
#endif
