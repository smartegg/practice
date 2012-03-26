#ifndef SOCKETIMPL_H
#define SOCKETIMPL_H
#include "RefCountedObject.h"
#include "SocketAddress.h"
#include "Timespan.h"

class SocketImpl : public RefCountedObject {
 public:
  enum SelectMode {
    SELEcT_READ = 1,
    SELECT_WRITE = 2,
    SELECT_ERROR = 4
  };

  //return a new tcp socket, (block unitl get one)
  //and returnthe clientAddr.
  virtual SocketImpl* acceptConnection(SocketAddress& clientAddr);
  //connect a tcp server  socket or a unp socket.
  virtual void connect(const SocketAddress& address);
  //connect or set timeout of   timespan  microseconds.
  virtual void connect(const SocketAddress& address,
                       const Timespan& timespan);
  //assume the socket is set to nonblock.
  virtual void connectNB(const SocketAddress& address);
  
  //bind a local address(only use in server)
  virtual void bind(const SocketAddress& address, bool reuseAddress = false);
  //puts the socket into listening state and specify the maximum 
  //number of connections that can be queued.
  virtual void listen(int backlog = 64);
  //close the socket
  virtual void close();
  
  //shutdown the receive part of the connection.
  virtual void shutdownReceive();
  //shutdown the send part of the connection.
  virtual void shutdownSend();
  //shutdown both parts of the connection.
  virtual void shutdown();
  //send the data in buffer to the other end.
  //return the number of data successfully sended.
  virtual int sendBytes(const void* buffer, int length, int flags = 0);
  //store the received data into @param buffer
  //return the number of nbytes received.
  virtual int receiveBytes(void* buffer, int length, int flags = 0);

  virtual int sendTo(const void* buffer, int length,
                     const SocketAddress& address, int flags = 0);
  virtual int receiveFrom(void* buffer, int length, SocketAddress& address,
                          int flags = 0);
  //send one byte of urgent data 
  virtual void sendUrgent(unsigned char data);
  //return the number of bytes that can be read without block.
  virtual int available();
  //check the @mode operation if can cause socket block.
  //return false if can block.
  //impl: using select()
  virtual bool poll(const Timespan& timeout, int mode);

  virtual void setSendBufferSize(int size);
  virtual int getSendBufferSize();

  virtual void setReceiveBufferSize(int size);
  virtual int getReceiveBufferSize();

  virtual void setSendTimeout(const Timespan& timeout);
  virtual Timespan getSendTimeout();

  virtual void setReceiveTimeout(const Timespan& timeout);
  virtual Timespan getReceiveTimeout();

  virtual SocketAddress address();
  virtual SocketAddress peerAddress();

  void setOption(int level, int option, int value);
  void setOption(int level, int option, unsigned value);
  void setOption(int level, int option, unsigned char value);
  void setOption(int level, int option, const Timespan& value);
  void setOption(int level, int option, const IPAddress& value);

  virtual void setRawOption(int level, int option, const void* value,
                            socklen_t length);
  void getOption(int level, int option, int& value);
  void getOption(int level, int option, unsigned& value);
  void getOption(int level, int option, unsigned char & value);
  void getOption(int level, int option, Timespan& value);
  void getOption(int level, int option, IPAddress& value);

  virtual void getRawOption(int level, int option, void* value,
                            socklen_t length);

  void setLinger(bool on, int seconds);
  void getLinger(bool& on, int& seconds);

  void setNoDelay(bool flag);
  bool getNoDelay();

  void setKeepAlive(bool flag);
  bool getKeepAlive();

  void setReuseAddress(bool flag);
  bool getReuseAddress();

  void setReusePort(bool flag);
  bool getReusePort();

  void setOOBInline(bool flag);
  bool getOOBInline();

  void setBroadcast(bool flag);
  bool getBroadcast();

  virtual bool getBlocking() const;
  virtual void setBlocking(bool flag);

  //return the SO_ERROR socket option
  int socketError();

  int sockfd() const;

  void ioctl(int request, int& arg);
  void ioctl(int request, void* arg);
  //return whether the socket is initialized.
  bool initialized() const;
 protected:
  SocketImpl();
  SocketImpl(int sockfd);
  virtual ~SocketImpl();
  virtual void init(int af);

  virtual void initSocket(int af, int type, int proto = 0);
  void reset(int fd = -1);
  static int lastError;
  static void error();
  static void error(const std::string& arg);
  static void error(int code);
  static void error(int code, std::string& arg);
 private:
  int sockfd_;
  Timespan recvTimeout_;
  Timespan sendTimeout_;
  bool blocking_;

  SocketImpl(const SocketImpl& );
  SocketImpl& operator = (const SocketImpl&);
  
  friend class Socket;
};
#endif
