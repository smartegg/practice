#include "handle_set.h"

void HandleSet::Reset() {
  this->max_handle_ = -1;
  this->size_ = 0;
  FD_ZERO(&this->mask_);
}

Handle HandleSet::MaxSet() const {
  return this->max_handle_;
}

int HandleSet::IsSet(Handle handle) const {
  return FD_ISSET(handle, &this->mask_);
}

void HandleSet::SetBit(Handle handle) {
  if (handle != -1 && (!this->IsSet(handle))) {
    FD_SET(handle, &this->mask_);
    ++this->size_;
  }
}

void HandleSet::ClrBit(Handle handle) {
  if (handle != -1 && this->IsSet(handle)) {
    --this->size_;
    FD_CLR(handle, &this->mask_);
  }
}

int HandleSet::NumSet() const {
  return this->size_;
}

HandleSet::operator fd_set *() {
  if (this->size_ > 0)
    return &this->mask_;
  else 
    return (fd_set*)0;
}

fd_set* HandleSet::fdset() {
  if (this->size_ > 0)
    return &this->mask_;
  else 
    return (fd_set*)0;
}
