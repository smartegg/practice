#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H
#include "Socket.h"

class StreamSocketImpl;

class StreamSocket : public Socket{
 public:
  StreamSocket();
  explicit StreamSocket(const SocketAddress& address);
  explicit StreamSocket(IPAddress::Family family);

  StreamSocket(const StreamSocket& socket);

  virtual ~StreamSocket();

  StreamSocket& operator = (const StreamSocket& socket);

  void connect(const SocketAddress& address);

  void connect(const SocketAddress& address, const Timespan& timeout);

  void connectNB(const SocketAddress& address);

  void shutdownReceive();

  void shutdownSend();

  void shutdown();

  int sendBytes(const void* buffer, int length, int flags = 0);

  int receiveBytes(void* buffer, int length, int flags = 0);

  void sendUrgent(unsigned char data);

 public:
  StreamSocket(SocketImpl* pImpl);
  friend class ServerSocket;
  friend class SocketIOS;
};
#endif
