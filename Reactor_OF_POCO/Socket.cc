#include "Socket.h"

#include <sys/epoll.h>
#include <cstring>
#include "StreamSocketImpl.h"

Socket::Socket() : pImpl_(new StreamSocketImpl) {
  
}

Socket::Socket(SocketImpl* impl) : pImpl_(impl) {

}

Socket::Socket(const Socket& socket) : pImpl_(socket.pImpl_){
  pImpl_->duplicate();
}

Socket& Socket::operator=(const Socket& socket) {
  if (&socket != this) {
    if(pImpl_) pImpl_->release();
    pImpl_ = socket.pImpl_;
    if(pImpl_) pImpl_->duplicate();
  }

  return *this;
}


Socket::~Socket() {
  pImpl_->release();
}

int Socket::select(SocketList& readList, SocketList& writeList,
                   SocketList& exceptList, const Timespan& timeout) {
  int epollSize = readList.size() + writeList.size() + exceptList.size();
  if (epollSize == 0)
    return 0;
  int epollfd = -1;

  struct epoll_event eventsIn[epollSize];
  std::memset(eventsIn, 0, sizeof(eventsIn));
  struct epoll_event* eventLast = eventsIn;

  for (SocketList::iterator it = readList.begin(); it != readList.end();
       ++it) {
    int sockfd = it->sockfd();
    if (sockfd != -1) {
      struct epoll_event* e = eventsIn;
      for (;e != eventLast; ++e) {
        if (reinterpret_cast<Socket*>(e->data.ptr)->sockfd() == sockfd) {
          break;
        }
      }
      if (e == eventLast) {
        e->data.ptr = &(*it);
        ++eventLast;
      }
      e->events |= EPOLLIN;
    }
  }

  for (SocketList::iterator it = writeList.begin();
       it != writeList.end();
       ++it) {
    int sockfd = it->sockfd();
    if (sockfd != -1) {
      struct epoll_event* e = eventsIn;
      for(; e != eventLast; ++e) {
        if (reinterpret_cast<Socket*>(e->data.ptr)->sockfd() == sockfd) {
          break;
        }
      }
      if (e == eventLast) {
        e->data.ptr = &(*it);
        ++eventLast;
      }
      e->events |= EPOLLOUT;
    }
  }

  for (SocketList::iterator it = exceptList.end(); it!= exceptList.end();
       ++it) {
    int sockfd = it->sockfd();
    if (sockfd != -1) {
      struct epoll_event* e = eventsIn;
      for (;e != eventLast; ++e) {
        if (reinterpret_cast<Socket*>(e->data.ptr)->sockfd() == sockfd) {
          break;
        }
      }
      if (e == eventLast) {
        e->data.ptr = &(*it);
        ++eventLast;
      }
      e->events |= EPOLLERR;
    }
  }

  epollSize = eventLast - eventsIn;
  epollfd = epoll_create(epollSize);
  if (epollfd < 0) {
    char buf[1024];
    strerror_r(errno, buf, sizeof(buf));
    SocketImpl::error(std::string("can't create epoll queue: ") + buf);
  }

  for (struct epoll_event* e = eventsIn; e != eventLast; ++e) {
    int sockfd = reinterpret_cast<Socket*>(e->data.ptr)->sockfd();
    if (sockfd != -1) {
      if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, e) < 0) {
        char buf[1024];
        strerror_r(errno, buf, sizeof(buf));
        ::close(epollfd);
        SocketImpl::error(std::string("can't insert socket into epoll queue:")
                                      + buf);
      }
    }
  }
  Timespan remainingTime(timeout);
  epoll_event eventOut[epollSize];
  memset(eventOut, 0, sizeof(eventOut));
  int rc;

  do {
    Timestamp start;
    rc = epoll_wait(epollfd,eventOut, epollSize,
                    remainingTime.totalMilliseconds());
    if (rc < 0 && SocketImpl::lastError() == EINTR) {
      Timestamp end;
      Timespan waited = end - start;
      if (waited <= remainingTime)
        remainingTime-=waited;
      else
        remainingTime = 0;
    }
  }while (rc < 0 && SocketImpl::lastError() == EINTR);
  ::close(epollfd);
  if (rc < 0)
    SocketImpl::error();
  
  SocketList readyReadList;
  SocketList readyWriteList;
  SocketList readyExceptList;

  for (int n = 0; n < rc; n++ ) {
    if (eventOut[n].events & EPOLLIN) 
      readyReadList.push_back(*reinterpret_cast<Socket*>(eventOut[n].data.ptr));
    if (eventOut[n].events & EPOLLOUT)
      readyWriteList.push_back(*reinterpret_cast<Socket*>(eventOut[n].data.ptr));
    if (eventOut[n].events & EPOLLERR) 
      readyExceptList.push_back(*reinterpret_cast<Socket*>(eventOut[n].data.ptr));
  }
  std::swap(readList, readyReadList);
  std::swap(writeList, readyWriteList);
  std::swap(exceptList, readyExceptList);
  return readList.size() + writeList.size() + exceptList.size();


}
