#include "SocketImpl.h"

#include "Exception.h"
#include "StreamSocketImpl.h"
#include <cstring>
#include  <sys/ioctl.h>
#include <sys/epoll.h>

SocketImpl::SocketImpl() : sockfd_(-1), blocking_(true){

}

SocketImpl::SocketImpl(int sockfd) : sockfd_(-1), blocking_(true){

}

SocketImpl::~SocketImpl() {

}

SocketImpl* SocketImpl::acceptConnection(SocketAddress& clientAddr) {
  if (sockfd_ == -1) 
    throw InvalidSocketException("fd not initialized");
  char buffer[SocketAddress::MAX_ADDRESS_LEN];
  struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
  socklen_t saLen = sizeof(buffer);
  socklen_t sd;
  do {
    ::accept(sockfd_, pSA, &saLen);
  }while(sd == -1 && lastError() == EINTR);

  if (sd != -1) {
    clientAddr = SocketAddress(pSA, saLen);
    return new StreamSocketImpl(sd);
  }
  error();
  return 0;
}

void SocketImpl::connect(const SocketAddress& address) {
  if (sockfd_ == -1) {
    init(address.af());
  }
  int rc;
  do {
    rc = ::connect(sockfd_, address.addr(), address.length());
  }while(rc != 0 && lastError() == EINTR);
  if (rc != 0) {
    int err = lastError();
    error(err, address.toString());
  }
}

void SocketImpl::connect(const SocketAddress& address,
                         const Timespan& timespan) {
  if (sockfd_ == -1) {
    init(address.af());
  }
  setBlocking(false);
  try {
    int rc = ::connect(sockfd_, address.addr(), address.length());
    if (rc != 0) {
      int err = lastError();
      if (err != EINPROGRESS && err != EWOULDBLOCK)
        error(err, address.toString());
      if (!poll(timespan, SELECT_READ | SELECT_WRITE | SELECT_ERROR))
        throw TimeoutException("connect time out", address.toString());
      err = socketError();
      if (err != 0)
        error(err);
    }
  }catch(Exception&) {
    setBlocking(true);
    throw;
  }
  setBlocking(true);
}

void SocketImpl::connectNB(const SocketAddress& address) {
  if (sockfd_ == -1) {
    init(address.af());
  }
  setBlocking(false);
  int rc = ::connect(sockfd_, address.addr(), address.length());
  if (rc != 0) {
    int err = lastError();
    if (err !=  EINPROGRESS && err != EWOULDBLOCK) {
      error(err, address.toString());
    }
  }
}

void SocketImpl::bind(const SocketAddress& address,
                      bool reuseAddress) {
  if (sockfd_ == -1) {
    init(address.af());
  }

  if (reuseAddress) {
    setReusePort(true);
    setReuseAddress(true);
  }

  int rc = ::bind(sockfd_, address.addr(), address.length());
  if (rc != 0)
    error(address.toString());

}

void SocketImpl::listen(int backlog) {
  if (sockfd_ == -1)
    throw RuntimeException();
  int rc = ::listen(sockfd_, backlog);
  if (rc != 0)
    error();
}

void SocketImpl::close() {
  if (sockfd_ != -1) {
    ::close(sockfd_);
    sockfd_ = -1;
  }
}

void SocketImpl::shutdownReceive() {
  if (sockfd_ == -1)
    throw RuntimeException();
  int rc = ::shutdown(sockfd_, SHUT_RD);
  if (rc != 0)
    error();
}

void SocketImpl::shutdownSend() {
  if (sockfd_ == -1)
    throw RuntimeException();
  int rc = ::shutdown(sockfd_, SHUT_WR);
  if (rc != 0)
    error();
}

void SocketImpl::shutdown() {
  if (sockfd_ == -1)
    throw RuntimeException();
  int rc = ::shutdown(sockfd_, SHUT_RDWR);
  if (rc != 0)
    error();
}

int SocketImpl::sendBytes(const void* buffer, int length,
                          int flags) {

  /*if (sendTimeout_.totalMicroseconds() != 0) {
    if (!poll(sendTimeout_,SELECT_WRITE))
      throw TimeoutException();
  }*/

  int rc;
  do {
    if (sockfd_ == -1)
      throw RuntimeException("sockfd = -1");
    rc = ::send(sockfd_, reinterpret_cast<const char*>(buffer), length,
                flags);
  }while (rc < 0 && lastError() == EINTR);

  if (rc < 0)
    error();
  return rc;
}

int SocketImpl::receiveBytes(void* buffer, int length, int flags) {
/*  if (recvTimeout_.totalMicroseconds() != 0) {
    if (!poll(recvTimeout_, SELECT_READ))
      throw TimeoutException();
  } */

  int rc;
  do {
    if (sockfd_ == -1)
      throw RuntimeException("sockfd = -1");
    rc = ::recv(sockfd_, reinterpret_cast<char*>(buffer), length,
                flags);
  }while(rc < 0 && lastError() == EINTR);

  if (rc < 0) {
    int err = lastError();
    if (err == EAGAIN || err == ETIMEDOUT) {
      throw TimeoutException();
    } else {
      error(err);
    }
  }
  return rc;
}
int SocketImpl::sendTo(const void* buffer, int length,
                     const SocketAddress& address, int flags) {
  int rc;
  do {
    if (sockfd_ == -1)
      throw RuntimeException("socket = -1");
    ::sendto(sockfd_, reinterpret_cast<const char*>(buffer),
             length, flags, address.addr(), address.length());
  }while (rc < 0 && lastError() == EINTR);
  if (rc < 0)
    error();
  return rc;
}
int SocketImpl::receiveFrom(void* buffer, int length, SocketAddress& address,
                          int flags) {
/*  if (recvTimeout_.totalMicroseconds() != 0) {
    if (!poll(recvTimeout_, SELECT_READ))
      throw TimeoutException();
  } */

  char abuffer[SocketAddress::MAX_ADDRESS_LEN];
  struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(abuffer);
  socklen_t saLen = sizeof(abuffer);

  int rc;

  do {
    if(sockfd_ == -1)
      throw InvalidSocketException();
    rc = ::recvfrom(sockfd_, reinterpret_cast<char*>(buffer), length, flags,
                    pSA, &saLen);
  }while(rc < 0 && lastError() == EINTR);

  if (rc >= 0) {
    address = SocketAddress(pSA, saLen);
  }

  else {
    int err = lastError();
    if (err == EAGAIN || err == ETIMEDOUT) {
      throw TimeoutException();
    } else {
      error(err);
    }
  }

  return rc;
}


void SocketImpl::sendUrgent(unsigned char data) {
  if (sockfd_ == -1) 
    throw InvalidSocketException();

  int rc = ::send(sockfd_, reinterpret_cast<const char*>(&data),
                  sizeof(data), MSG_OOB);
  if (rc < 0)
    error();
}

int SocketImpl::available() {
  int result;
  ::ioctl(FIONREAD, result);
  return result;
}

bool SocketImpl::poll(const Timespan& timeout, int mode) {
  int sockfd = sockfd_;
  if (sockfd == -1)
    throw InvalidSocketException();
  int epollfd = epoll_create1(0);

  if (epollfd < 0)  {
    char buf[1024];
    strerror_r(errno, buf, sizeof(buf));
    error(std::string("can't create epoll queue:") + buf);
  }

  struct epoll_event evin;
  memset(&evin, 0, sizeof(evin));
  if (mode & SELECT_READ)
    evin.events |= EPOLLIN;
  if (mode & SELECT_WRITE)
    evin.events |= EPOLLOUT;
  if (mode & SELECT_ERROR) 
    evin.events |= EPOLLERR;

  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &evin) < 0) {
    char buf[1024];
    strerror_r(errno, buf, sizeof(buf));
    ::close(epollfd);
    error(std::string("can't insert socket to epoll queue: " ) + buf);
  }

  Timespan remainingTime(timeout);

  int rc;
  do {
    struct epoll_event evout;
    memset(&evout, 0,sizeof(evout));
    Timestamp start;

    rc = epoll_wait(epollfd, &evout, 1, remainingTime.totalMicroseconds());
    if (rc < 0 && lastError() == EINTR) {
      Timestamp end;
      Timespan waited = end - start;
      if (waited < remainingTime)
        remainingTime -= waited;
      else
        remainingTime = 0;

    }
  }while (rc < 0 && lastError() == EINTR);;

  ::close(epollfd);
  if (rc < 0)
    error();
  return rc > 0;
}

void SocketImpl::setSendBufferSize(int size) {
  setOption(SOL_SOCKET, SO_SNDBUF, size);
}

int SocketImpl::getSendBufferSize() {
  int result;
  getOption(SOL_SOCKET, SO_SNDBUF, result);
  return result;
}

void SocketImpl::setReceiveBufferSize(int size) {
  setOption(SOL_SOCKET, SO_RCVBUF, size);
}

int SocketImpl::getReceiveBufferSize() {
  int result;
  getOption(SOL_SOCKET, SO_RCVBUF, result);
  return result;
}

void SocketImpl::setSendTimeout(const Timespan& timeout) {
  setOption(SOL_SOCKET, SO_SNDTIMEO, timeout);
}

Timespan SocketImpl::getSendTimeout() {
  Timespan result;
  int value;
  getOption(SOL_SOCKET, SO_SNDTIMEO, result);
  return result;
}

void SocketImpl::setReceiveTimeout(const Timespan& timeout)  {
  setOption(SOL_SOCKET, SO_RCVTIMEO, timeout);
}

Timespan SocketImpl::getReceiveTimeout() {
  Timespan result;
  getOption(SOL_SOCKET, SO_RCVTIMEO, result);
  return result;
}

SocketAddress SocketImpl::address() {
  if (sockfd_ == -1)
    throw InvalidSocketException();
  char buffer[SocketAddress::MAX_ADDRESS_LEN];
  struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
  socklen_t saLen = sizeof(buffer);

  int rc = ::getsockname(sockfd_, pSA, &saLen);
  if (rc == 0)
    return SocketAddress(pSA, saLen);
  else 
    error();
  return SocketAddress();
}

SocketAddress SocketImpl::peerAddress() {
  if (sockfd_ == -1)
    throw InvalidSocketException();
  char buffer[SocketAddress::MAX_ADDRESS_LEN];
  struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
  socklen_t saLen = sizeof(buffer);

  int rc = ::getpeername(sockfd_, pSA, &saLen);
  if (rc == 0)
    return SocketAddress(pSA, saLen);
  else 
    error();
  return SocketAddress();
}

void SocketImpl::setOption(int level, int option, int value) {
  setRawOption(level, option, &value, sizeof(value));
}

void SocketImpl::setOption(int level, int option, unsigned value) {
  setRawOption(level, option, &value, sizeof(value));
}

void SocketImpl::setOption(int level, int option, unsigned char value) {
  setRawOption(level, option, &value, sizeof(value));
}

void SocketImpl::setOption(int level, int option, const IPAddress& value) {
  setRawOption(level, option, value.addr(), value.length());
}
void SocketImpl::setOption(int level, int option, const Timespan& value) {
  struct timeval tv;
  tv.tv_sec = value.totalSeconds();
  tv.tv_usec = value.useconds();
  setRawOption(level, option, &tv, sizeof(tv));
}

void SocketImpl::setRawOption(int level, int option, const void* value,
                              socklen_t length) {
  if (sockfd_ == -1) {
    throw InvalidSocketException();
  }

  int rc = ::setsockopt(sockfd_, level, option,
                        reinterpret_cast<const char*>(value),
                        length);
  if (rc == -1)
    error();
}

void SocketImpl::getOption(int level, int option, int& value) {
  socklen_t len = sizeof(value);
  getRawOption(level, option, &value, len);
}

void SocketImpl::getOption(int level, int option, unsigned& value) {
  socklen_t len = sizeof(value);
  getRawOption(level, option, &value, len);
}

void SocketImpl::getOption(int level, int option, unsigned char& value) {
  socklen_t len = sizeof(value);
  getRawOption(level, option, &value, len);
}

void SocketImpl::getOption(int level, int option, Timespan& value) {
  struct timeval tv;
  socklen_t len = sizeof(tv);
  getRawOption(level, option, &tv, len);
  value.assign(tv.tv_sec, tv.tv_usec);
}

void SocketImpl::getOption(int level, int option, IPAddress& value) {
  char buffer[SocketAddress::MAX_ADDRESS_LEN];
  socklen_t len = sizeof(buffer);
  getRawOption(level, option, buffer, len);
  value = IPAddress(buffer, len);
} 

void SocketImpl::getRawOption(int level, int option, void* value,
                              socklen_t& length) {
  if (sockfd_ == -1)
    throw InvalidSocketException();
  int rc = ::getsockopt(sockfd_, level, option,
                        reinterpret_cast<char*>(value),
                        &length);
  if (rc == -1)
    error();
}

void SocketImpl::setLinger(bool on, int seconds) {
  struct linger l;
  l.l_onoff = on ? 1 : 0;
  l.l_linger = seconds;
  setRawOption(SOL_SOCKET, SO_LINGER, &l, sizeof(l));
}

void SocketImpl::getLinger(bool& on, int& seconds) {
  struct linger l;
  socklen_t len = sizeof(l);
  getRawOption(SOL_SOCKET, SO_LINGER, &l, len);
  on = l.l_onoff;
  seconds = l.l_linger;
}

void SocketImpl::setNoDelay(bool flag) {
  throw NotImplementedException();
}

bool SocketImpl::getNoDelay() {
  throw NotImplementedException();
}

void SocketImpl::setKeepAlive(bool flag) {
  int value = flag? 1 : 0;
  setOption(SOL_SOCKET, SO_KEEPALIVE, value);
}

bool SocketImpl::getKeepAlive() {
  int value(0);
  getOption(SOL_SOCKET, SO_KEEPALIVE, value);
  return value != 0;
}

void SocketImpl::setReuseAddress(bool flag) {
  int value = flag ? 1 : 0;
  setOption(SOL_SOCKET, SO_REUSEADDR, value);
}

bool SocketImpl::getReuseAddress() {
  int value(0);
  getOption(SOL_SOCKET, SO_REUSEADDR, value);
  return value != 0;
} 

void SocketImpl::setReusePort(bool flag) {
}

bool SocketImpl::getReusePort() {
  return false;
}

void SocketImpl::setOOBInline(bool flag) {
  throw NotImplementedException();
}

bool SocketImpl::getOOBInline() {
  throw NotImplementedException();
}

void SocketImpl::setBroadcast(bool flag) {
  int value = flag? 1 : 0;
  setOption(SOL_SOCKET, SO_BROADCAST, value);
}

bool SocketImpl::getBroadcast() {
  int value(0);
  getOption(SOL_SOCKET, SO_BROADCAST, value);
  return value != 0;
}

void SocketImpl::setBlocking(bool flag) {
  int arg = flag ? 1 : 0;
  ioctl(FIONBIO, arg);
  blocking_ = flag;
}

int SocketImpl::socketError() {
  int result(0);
  getOption(SOL_SOCKET, SO_ERROR, result);
  return result;
}

void SocketImpl::init(int af) {
  initSocket(af, SOCK_STREAM);
}

void SocketImpl::initSocket(int af, int type, int proto) {
  sockfd_ = ::socket(af, type, proto);
  if (sockfd_ == -1)
    error();
}

void SocketImpl::ioctl(int request, int& arg) {
  int rc = ::ioctl(sockfd_, request, &arg);
  if (rc != 0)
    error();
}

void SocketImpl::ioctl(int request, void* arg) {
  int rc = ::ioctl(sockfd_ , request, arg);
  if (rc != 0)
    error();
}

void SocketImpl::reset(int fd) {
  sockfd_ = fd;
}

void SocketImpl::error() {
  int err = lastError();
  std::string empty;
  error(err, empty);
}

void SocketImpl::error(const std::string& arg) {
  error(lastError(), arg);
}

void SocketImpl::error(int code) {
  std::string arg;
  error(code, arg);
}


void SocketImpl::error(int code, const std::string& arg) {
  switch(code) {
    case EINTR:
      throw IOException("interrupted");
    case EACCES:
      throw IOException("permission denied");
    case EFAULT:
      throw IOException("Bad address");
    case EINVAL:
      throw InvalidArgumentException();
    case EMFILE:
      throw IOException("too many open file");
    case EWOULDBLOCK:
      throw IOException("operation would block");
    case EINPROGRESS:
      throw IOException("operation now in progress");
    case EALREADY:
      throw IOException("operation already in progress");
    case ENOTSOCK:
      throw IOException("socket operation attempted on non-socket");
    case EDESTADDRREQ:
      throw NetException("Destination address Requried");
    case EMSGSIZE:
      throw NetException("message too long");
    case EPROTOTYPE:
      throw NetException("wrong protocol type");
    case ENOPROTOOPT:
      throw NetException("protocol not available");
    case EPROTONOSUPPORT:
      throw NetException("protocol not supported");
    case ESOCKTNOSUPPORT:
      throw NetException("socket type not supported");
    case ENOTSUP:
      throw NetException("operation not supported");
    case EPFNOSUPPORT:
      throw NetException("protocol family not supported");
    case EAFNOSUPPORT:
      throw NetException("address family not supported");
    case EADDRINUSE:
      throw NetException("address already in use", arg);
    case EADDRNOTAVAIL:
      throw NetException("can not assign requested address", arg);
    case ENETDOWN:
      throw NetException("Network is down");
    case ENETUNREACH :
      throw NetException("network is unreachable");
    case ENETRESET:
      throw NetException("Network dropped connection on reset");
    case ECONNABORTED: 
      throw ConnectionAbortedException();
    case ECONNRESET:
      throw ConnectionResetException();
    case ENOBUFS :
      throw IOException("no buffer space availble");
    case EISCONN :
      throw NetException("socket is already connected");
    case ENOTCONN:
      throw NetException("socket is not connected");
    case ESHUTDOWN:
      throw NetException("cannot send after socket shutdown");
    case ETIMEDOUT:
      throw TimeoutException();
    case ECONNREFUSED: 
      throw ConnectionRefusedException(arg);
    case EHOSTDOWN:
      throw NetException("Host is down", arg);
    case EHOSTUNREACH:
      throw NetException("No route to host", arg);
    default: {
      char buf[64];
      sprintf(buf, "%d", code);
      throw IOException(buf, arg);
    }
  };
}
