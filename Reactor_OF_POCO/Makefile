OBJ = ErrorHandler.o StreamSocketImpl.o StreamSocket.o ServerSocket.o echo_server.o ServerSocketImpl.o NotificationCenter.o echo_client.o RefCountedObject.o Timespan.o SocketNotifier.o Socket.o SocketAddress.o AbstractObserver.o Timestamp.o IPAddress.o SocketImpl.o SocketReactor.o SocketNotification.o Notification.o Exception.o 

client : $(OBJ)
	 rm echo_server.o
	 g++ -o $@ *.o -g
ErrorHandler.o: ErrorHandler.h SingletonHolder.h ErrorHandler.cc
	g++ -o ErrorHandler.o -c ErrorHandler.cc -g 
StreamSocketImpl.o: StreamSocketImpl.h IPAddress.h Exception.h StreamSocketImpl.cc
	g++ -o StreamSocketImpl.o -c StreamSocketImpl.cc -g 
StreamSocket.o: StreamSocket.h StreamSocketImpl.h Exception.h StreamSocket.cc
	g++ -o StreamSocket.o -c StreamSocket.cc -g 
ServerSocket.o: ServerSocket.h ServerSocketImpl.h Exception.h ServerSocket.cc
	g++ -o ServerSocket.o -c ServerSocket.cc -g 
echo_server.o: Exception.h StreamSocket.h ServerSocket.h SocketReactor.h SocketAcceptor.h SocketConnector.h AutoPtr.h ErrorHandler.h echo_server.cc
	g++ -o echo_server.o -c echo_server.cc -g 
ServerSocketImpl.o: ServerSocketImpl.h ServerSocketImpl.cc
	g++ -o ServerSocketImpl.o -c ServerSocketImpl.cc -g 
NotificationCenter.o: NotificationCenter.h SingletonHolder.h NotificationCenter.cc
	g++ -o NotificationCenter.o -c NotificationCenter.cc -g 
echo_client.o: Exception.h StreamSocket.h ServerSocket.h SocketReactor.h SocketAcceptor.h SocketConnector.h AutoPtr.h ErrorHandler.h echo_client.cc
	g++ -o echo_client.o -c echo_client.cc -g 
RefCountedObject.o: RefCountedObject.h RefCountedObject.cc
	g++ -o RefCountedObject.o -c RefCountedObject.cc -g 
Timespan.o: Timespan.h Timespan.cc
	g++ -o Timespan.o -c Timespan.cc -g 
SocketNotifier.o: SocketNotifier.h SocketNotification.h SocketReactor.h SocketNotifier.cc
	g++ -o SocketNotifier.o -c SocketNotifier.cc -g 
Socket.o: Socket.h StreamSocketImpl.h Socket.cc
	g++ -o Socket.o -c Socket.cc -g 
SocketAddress.o: SocketAddress.h RefCountedObject.h Exception.h SocketAddress.cc
	g++ -o SocketAddress.o -c SocketAddress.cc -g 
AbstractObserver.o: AbstractObserver.h AbstractObserver.cc
	g++ -o AbstractObserver.o -c AbstractObserver.cc -g 
Timestamp.o: Timestamp.h Exception.h Timestamp.cc
	g++ -o Timestamp.o -c Timestamp.cc -g 
IPAddress.o: IPAddress.h RefCountedObject.h Exception.h IPAddress.cc
	g++ -o IPAddress.o -c IPAddress.cc -g 
SocketImpl.o: SocketImpl.h Exception.h StreamSocketImpl.h SocketImpl.cc
	g++ -o SocketImpl.o -c SocketImpl.cc -g 
SocketReactor.o: SocketReactor.h Socket.h AbstractObserver.h ErrorHandler.h SocketReactor.cc
	g++ -o SocketReactor.o -c SocketReactor.cc -g 
SocketNotification.o: SocketNotification.h SocketNotification.cc
	g++ -o SocketNotification.o -c SocketNotification.cc -g 
Notification.o: Notification.h Notification.cc
	g++ -o Notification.o -c Notification.cc -g 
Exception.o: Exception.h Exception.cc
	g++ -o Exception.o -c Exception.cc -g 
