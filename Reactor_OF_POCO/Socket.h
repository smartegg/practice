#ifndef SOCKET_H
#define SOCKET_H

#include "Timespan.h"
#include "IPAddress.h"
#include "SocketAddress.h"
#include "SocketImpl.h"

#include <vector>


class Socket {
 public:
  enum SelectMode {
    SELECT_READ = 1,
    SELECT_WRITE = 2,
    SELECT_ERROR = 4
  };

  typedef std::vector<Socket> SocketList;

  Socket();
  Socket(const Socket& socket);
  Socket& operator= (const Socket& socket);

  virtual ~Socket();

  bool operator == (const Socket& socket) const;
  bool operator != (const Socket& socket) const;
  bool operator <  (const Socket& socket) const;
  bool operator <= (const Socket& socket) const;
  bool operator >  (const Socket& socket) const;
  bool operator >= (const Socket& socket) const;

  void close();

  static int select(SocketList& readList, SocketList& writeList,
                    SocketList& exceptList, const Timespan& timeout);

  bool poll(const Timespan& timeout, int mode) const;
  int available() const;
  void setSendBufferSize(int size);
  int getSendBufferSize() const;

  void setReceiveBufferSize(int size);
  int getReceiveBufferSize()const;

  void setSendTimeout(const Timespan& timeout);
  Timespan getSendTimeout() const;

  void setReceiveTimeout(const Timespan& timeout);
  Timespan getReceiveTimeout() const;

  void setOption(int level, int option, int value);
  void setOption(int level, int option, unsigned  value);
  void setOption(int level, int option, unsigned char value);
  void setOption(int level, int option, const Timespan& value);
  void setOption(int level, int option, const IPAddress& value);

  void getOption(int level, int opotion, int& value)const ;
  void getOption(int level, int opotion, unsigned& value)const;
  void getOption(int level, int opotion, unsigned char& value)const;
  void getOption(int level, int opotion, Timespan& value)const;
  void getOption(int level, int opotion, IPAddress& value)const;

  void setLinger(bool on, int seconds);
  void getLinger(bool& on, int& seconds)const ;

  void setNoDelay(bool flag);
  bool getNoDelay()const ;

  void setKeepAlive(bool flag);
  bool getKeepAlive()const;

  void setReuseAddress(bool flag);
  bool getReuseAddress()const ;

  void setReusePort(bool flag);
  bool getReusePort()const ;

  void setOOBInline(bool flag);
  bool getOOBInline() const;

  void setBlocking(bool flag);
  bool getBlocking()const;

  SocketAddress address() const;
  SocketAddress peerAddress() const;

  SocketImpl* impl() const;

  static bool supportsIPv4();
  static bool supportsIPv6();
protected:
  Socket(SocketImpl* pImpl);
  int sockfd() const;
private:
  SocketImpl* pImpl_;
};

inline bool Socket::operator== (const Socket& socket) const {
  return pImpl_ == socket.pImpl_;
}

inline bool Socket::operator!= (const Socket& socket) const {
  return pImpl_ != socket.pImpl_;
}

inline bool Socket::operator<  (const Socket& socket) const {
  return pImpl_ < socket.pImpl_;
}

inline bool Socket::operator<= (const Socket& socket) const {
  return pImpl_ <= socket.pImpl_;
}

inline bool Socket::operator>  (const Socket& socket) const {
  return pImpl_ > socket.pImpl_;
}

inline bool Socket::operator>= (const Socket& socket) const {
  return pImpl_ >= socket.pImpl_;
}

inline void Socket::close() {
  pImpl_->close();
}

inline bool Socket::poll(const Timespan& timeout, int mode) const{
  return pImpl_->poll(timeout, mode);
}

inline int Socket::available() const {
  return pImpl_->available();
}

inline void Socket::setSendBufferSize(int size) {
  return pImpl_->setSendBufferSize(size);
}

inline int Socket::getSendBufferSize() const{
  return pImpl_->getSendBufferSize();
}

inline void Socket::setReceiveBufferSize(int size) {
 pImpl_->setReceiveBufferSize(size);
}

inline int Socket::getReceiveBufferSize() const {
  return pImpl_->getReceiveBufferSize();
}

inline void Socket::setSendTimeout(const Timespan& timeout) {
  pImpl_->setSendTimeout(timeout);
}

inline Timespan Socket::getSendTimeout() const{
  return pImpl_->getSendTimeout();
}

inline void Socket::setReceiveTimeout(const Timespan& timeout) {
  pImpl_->setReceiveTimeout(timeout);
}

inline Timespan Socket::getReceiveTimeout() const {
  return pImpl_->getReceiveTimeout();
}

inline void Socket::setOption(int level, int option, int value) {
  pImpl_->setOption(level, option, value);
}

inline void Socket::setOption(int level, int option, unsigned value) {
  pImpl_->setOption(level, option, value);
}
inline void Socket::setOption(int level, int option, unsigned char value) {
  pImpl_->setOption(level, option, value);
}

inline void Socket::setOption(int level, int option,
                              const Timespan& value) {
  pImpl_->setOption(level, option, value);
}

inline void Socket::setOption(int level, int option, const IPAddress& value) {
  pImpl_->setOption(level, option, value);
}

inline void Socket::getOption(int level, int option, int& value) const{
  pImpl_->getOption(level, option, value);
}
inline void Socket::getOption(int level, int option, unsigned& value) const{
  pImpl_->getOption(level, option, value);
}
inline void Socket::getOption(int level, int option,
                              unsigned char& value) const{
  pImpl_->getOption(level, option, value);
}
inline void Socket::getOption(int level, int option,
                              IPAddress& value) const{
  pImpl_->getOption(level, option, value);
}

inline void Socket::getOption(int level, int option, Timespan& value) const{
  pImpl_->getOption(level, option, value);
}

inline void Socket::setLinger(bool on, int seconds) {
  pImpl_->setLinger(on, seconds);
}

inline void Socket::getLinger(bool& on, int& seconds) const{
  pImpl_->getLinger(on, seconds);
}


inline void Socket::setNoDelay(bool flag){
  pImpl_->setNoDelay(flag);
}

inline bool Socket::getNoDelay() const{
  return pImpl_->getNoDelay(); 
}

inline void Socket::setKeepAlive(bool flag) {
  pImpl_->setKeepAlive(flag);
}

inline bool Socket::getKeepAlive() const{
  return pImpl_->getKeepAlive();
}

inline void Socket::setReuseAddress(bool flag) {
  pImpl_->setReuseAddress(flag);
}

inline bool Socket::getReuseAddress() const {
  return pImpl_->getReuseAddress();
}

inline void Socket::setReusePort(bool flag) {
  pImpl_->setReusePort(flag);
}

inline bool Socket::getReusePort() const{
  return pImpl_->getReusePort();
}

inline void Socket::setOOBInline(bool flag) {
  return pImpl_->setOOBInline(flag);
}

inline bool Socket::getOOBInline() const{
  return pImpl_->getOOBInline();
}

inline void Socket::setBlocking(bool flag) {
  pImpl_->setBlocking(flag);
}

inline bool Socket::getBlocking() const{
  return pImpl_->getBlocking();
}

inline SocketImpl* Socket::impl() const{
  return pImpl_;
}


inline int Socket::sockfd() const {
  return pImpl_->sockfd();
}
inline SocketAddress Socket::address() const{
  return pImpl_->address();
}

inline SocketAddress Socket::peerAddress() const{
  return pImpl_->peerAddress();
}

inline bool Socket::supportsIPv4() {
  return true;
}
#endif
