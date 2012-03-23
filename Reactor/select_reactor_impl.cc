#include "select_reactor_impl.h"
#include <cerrno>

SelectReactorHandlerRepository::SelectReactorHandlerRepository(
    SelectReactorImpl& impl) : select_reactor_(impl),
                               maxhandlep1_(0),
                               table_(){

}

bool SelectReactorHandlerRepository::InvalidHandle(Handle handle) {
  if (handle < 0 | handle >= this->table_.size()) {
    errno = EINVAL;
    return true;
  }
  return false;
}

bool SelectReactorHandlerRepository::HandleInRange(Handle handle) {
  if (handle >= 0 && handle < this->maxhandlep1_) {
    return true;
  }
  return false;
}

int SelectReactorHandlerRepository::Open(size_t size) {
  table_.clear();
  for(size_t i = 0; i < size; i++)
    table_[i] = static_cast<EventHandler*>(0);
  this->maxhandlep1_ = 0;
  return 0;
}

int SelectReactorHandlerRepository::Close() {
  return this->UnBindAll();
}

int SelectReactorHandlerRepository::UnBindAll() {
  map_type::iterator pos = this->table_.begin();
  for (Handle handle = 0; handle < this->maxhandlep1_; ++handle) {
    this->UnBind(handle, pos, kAllEventsMask);
    ++pos;
  }
  return 0;
}

int SelectReactorHandlerRepository::Bind(Handle handle, EventHandler* handler,
                                         EventType et) {
  if (handler == 0)
    return -1;
  if (handle == -1)
    return -1;
  table_[handle] = handler;
  if (this->maxhandlep1_ < handle + 1)
    this->maxhandlep1_ = handle + 1;

  this->select_reactor_.BitOps(handle, et, this->select_reactor_.wait_set_, 
                               Reactor::kAddMask);

  return 0;
}

int SelectReactorHandlerRepository::UnBind(Handle handle,
                                           map_type::iterator pos,
                                           EventType et) {
  EventHandler*  event_handler;
  if (pos == table_.end())
    event_handler = 0;
  else 
    event_handler = (*pos).second;
  this->select_reactor_.BitOps(handle, et, this->select_reactor_.wait_set_,
                               Reactor::kClrMask);
  this->select_reactor_.state_changed_ = 1;

  const bool has_any_wait_mask = 
      (this->select_reactor_.wait_set_.rd_mask_.IsSet(handle) ||
       this->select_reactor_.wait_set_.wr_mask_.IsSet(handle) ||
       this->select_reactor_.wait_set_.ex_mask_.IsSet(handle));
  
  if (!has_any_wait_mask) {
    this->table_[handle] = 0;
    if (this->maxhandlep1_ == handle + 1) {
      const Handle wait_rd_max = 
          this->select_reactor_.wait_set_.rd_mask_.MaxSet();
      const Handle wait_wr_max =
          this->select_reactor_.wait_set_.wr_mask_.MaxSet();
      const Handle wait_ex_max = 
          this->select_reactor_.wait_set_.ex_mask_.MaxSet();
      this->maxhandlep1_ = wait_rd_max;
      if (this->maxhandlep1_ < wait_wr_max)
        this->maxhandlep1_ = wait_wr_max;
      if (this->maxhandlep1_ < wait_ex_max) 
        this->maxhandlep1_ = wait_ex_max;
      ++this->maxhandlep1_;
    }
  }

  if (event_handler == 0)
    return -1;
  if ((et & kDontCall) == 0) {
    event_handler->HandleClose(handle, et);
  }
  
  return 0;
}


//return the origin mask, or  -1 on error.
int SelectReactorImpl::BitOps(Handle handle, EventType mask, 
                              SelectReactorHandleSet& handle_set,
                              int ops) {
  if (this->handler_repo_.HandleInRange(handle) == false)
    return -1;
  Fds_Ptmf ptmf = &HandleSet::SetBit;
  int  omask = kNullMask;
  if (handle_set.rd_mask_.IsSet(handle))
    omask |= kReadMask; 
  if (handle_set.wr_mask_.IsSet(handle))
    omask |= kWriteMask;
  if (handle_set.ex_mask_.IsSet(handle))
    omask |= kExceptMask;

  switch (ops) {
    case Reactor::kGetMask: {
      break;
    }
    case Reactor::kClrMask: {
      ptmf = &HandleSet::ClrBit;
      this->ClearDispatchMask(handle, mask);
    }
    case Reactor::kSetMask: 
    case Reactor::kAddMask: {
      //read,accept and connnect flag will place the handles in the read set.
      if ((mask & kReadMask) || (mask & kAcceptMask) 
          || (mask & kConnectMask)) {
        (handle_set.rd_mask_.*ptmf)(handle);
      } else if (ops == Reactor::kSetMask) {
        handle_set.rd_mask_.ClrBit(handle);
      }
      //write and connect flag will place the handles in the write set.
      if ((mask & kWriteMask) || (mask & kConnectMask)) {
        (handle_set.wr_mask_.*ptmf)(handle);
      } else if (ops == Reactor::kSetMask) {
        handle_set.wr_mask_.ClrBit(handle);
      }

      if ((mask & kExceptMask)) {
        (handle_set.ex_mask_.*ptmf)(handle);
      } else if (ops == Reactor::kSetMask) {
        handle_set.ex_mask_.ClrBit(handle);
      }
    }
    default: {
      return -1;
    }
    
  }
  return omask;  
}


void SelectReactorImpl::ClearDispatchMask(Handle handle,
                                          EventType mask) {
  if ((mask & kReadMask) || (mask & kAcceptMask)) {
    this->dispatche_set_.rd_mask_.ClrBit(handle);
  }
  if ((mask & kWriteMask)) {
    this->dispatche_set_.wr_mask_.ClrBit(handle);
  }
  if ((mask & kExceptMask)) {
    this->dispatche_set_.ex_mask_.ClrBit(handle);
  }
}
