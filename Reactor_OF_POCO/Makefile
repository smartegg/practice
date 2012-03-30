OBJ = ErrorHandler.o StreamSocketImpl.o StreamSocket.o ServerSocket.o echo_server.o ServerSocketImpl.o NotificationCenter.o RefCountedObject.o Timespan.o SocketNotifier.o Socket.o SocketAddress.o AbstractObserver.o Timestamp.o IPAddress.o SocketImpl.o SocketReactor.o SocketNotification.o Notification.o Exception.o 

app : $(OBJ)
	 g++ -o $@ *.o
ErrorHandler.o: ErrorHandler.h SingletonHolder.h ErrorHandler.cc
	g++ -o ErrorHandler.o -c ErrorHandler.cc
StreamSocketImpl.o: StreamSocketImpl.h IPAddress.h Exception.h StreamSocketImpl.cc
	g++ -o StreamSocketImpl.o -c StreamSocketImpl.cc
StreamSocket.o: StreamSocket.h StreamSocketImpl.h Exception.h StreamSocket.cc
	g++ -o StreamSocket.o -c StreamSocket.cc
ServerSocket.o: ServerSocket.h ServerSocketImpl.h Exception.h ServerSocket.cc
	g++ -o ServerSocket.o -c ServerSocket.cc
echo_server.o: Exception.h StreamSocket.h ServerSocket.h SocketReactor.h SocketAcceptor.h SocketConnector.h AutoPtr.h ErrorHandler.h echo_server.cc
	g++ -o echo_server.o -c echo_server.cc
ServerSocketImpl.o: ServerSocketImpl.h ServerSocketImpl.cc
	g++ -o ServerSocketImpl.o -c ServerSocketImpl.cc
NotificationCenter.o: NotificationCenter.h SingletonHolder.h NotificationCenter.cc
	g++ -o NotificationCenter.o -c NotificationCenter.cc
RefCountedObject.o: RefCountedObject.h RefCountedObject.cc
	g++ -o RefCountedObject.o -c RefCountedObject.cc
Timespan.o: Timespan.h Timespan.cc
	g++ -o Timespan.o -c Timespan.cc
SocketNotifier.o: SocketNotifier.h SocketNotification.h SocketReactor.h SocketNotifier.cc
	g++ -o SocketNotifier.o -c SocketNotifier.cc
Socket.o: Socket.h StreamSocketImpl.h Socket.cc
	g++ -o Socket.o -c Socket.cc
SocketAddress.o: SocketAddress.h RefCountedObject.h Exception.h SocketAddress.cc
	g++ -o SocketAddress.o -c SocketAddress.cc
AbstractObserver.o: AbstractObserver.h AbstractObserver.cc
	g++ -o AbstractObserver.o -c AbstractObserver.cc
Timestamp.o: Timestamp.h Exception.h Timestamp.cc
	g++ -o Timestamp.o -c Timestamp.cc
IPAddress.o: IPAddress.h RefCountedObject.h Exception.h IPAddress.cc
	g++ -o IPAddress.o -c IPAddress.cc
SocketImpl.o: SocketImpl.h Exception.h StreamSocketImpl.h SocketImpl.cc
	g++ -o SocketImpl.o -c SocketImpl.cc
SocketReactor.o: SocketReactor.h Socket.h AbstractObserver.h ErrorHandler.h SocketReactor.cc
	g++ -o SocketReactor.o -c SocketReactor.cc
SocketNotification.o: SocketNotification.h SocketNotification.cc
	g++ -o SocketNotification.o -c SocketNotification.cc
Notification.o: Notification.h Notification.cc
	g++ -o Notification.o -c Notification.cc
Exception.o: Exception.h Exception.cc
	g++ -o Exception.o -c Exception.cc
