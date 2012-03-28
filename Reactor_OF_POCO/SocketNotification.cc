#include "SocketNotification.h"

SocketNotification::SocketNotification(SocketReactor* pReactor)
  : pReactor_(pReactor){

}

SocketNotification::~SocketNotification(){

}

void SocketNotification::setSocket(const Socket& socket) {
  socket_ = socket;
}

ReadableNotification::ReadableNotification(
    SocketReactor* pReactor) : SocketNotification(pReactor) {

}

ReadableNotification::~ReadableNotification() {

}

WritableNotification::WritableNotification(
    SocketReactor* pReactor) : SocketNotification(pReactor) {

}

WritableNotification::~WritableNotification() {

}

ErrorNotification::ErrorNotification( 
    SocketReactor* pReactor) : SocketNotification(pReactor) {

}

ErrorNotification::~ErrorNotification() {

}

TimeoutNotification::TimeoutNotification(
    SocketReactor* pReactor) : SocketNotification(pReactor) {

}

TimeoutNotification::~TimeoutNotification() {

}

IdleNotification::IdleNotification(
    SocketReactor* pReactor) : SocketNotification(pReactor) {

}

IdleNotification::~IdleNotification() {

}

ShutdownNotification::ShutdownNotification(
    SocketReactor* pReactor) : SocketNotification(pReactor) {

}

ShutdownNotification::~ShutdownNotification() {

}
